---
title: 研究：Subagent 使用方式與計費
tags: [ai, subagent, multi-agent, claude-code, anthropic-api, billing, orchestrator]
created: 2026-05-13
updated: 2026-05-13
---

# Subagent 使用方式與計費研究

> [!info]
> 這份筆記整理三個層次的 subagent：Claude Code UI 操作、Claude Code SDK、Anthropic API。
> 重點：如何用、怎麼計費、如何讓 agent 呼叫另一個 agent。

---

## 什麼是 Subagent

**Subagent（子代理人）** 是由主 agent（orchestrator）派出去執行特定子任務的獨立 agent。

```
你 → Orchestrator → Subagent A（研究）
                  → Subagent B（寫程式）
                  → Subagent C（測試）
```

每個 subagent：
- 有**自己獨立的 context**（不共享主 agent 的對話歷史）
- 收到一份任務說明（prompt），完成後把結果回傳給 orchestrator
- 可以用**不同的模型**（例如 orchestrator 用 Opus，subagent 用 Sonnet 省錢）

---

## 兩種入口：訂閱 vs API Key

| | Claude Code 訂閱（Pro/Max）| API Key（pay-as-you-go）|
|---|---|---|
| **Subagent 支援** | ✅ 內建，直接用 | ✅ 自己寫 orchestrator |
| **如何觸發** | Claude 自動判斷，或 prompt 要求 | 程式碼呼叫 `client.messages.create()` |
| **計費** | 共用訂閱配額，大量使用觸發 rate limit | 每次 API call 獨立計費 |
| **彈性** | 固定幾種 agent 類型 | 完全自訂 system prompt、模型、邏輯 |

> [!info] 你現在就在用 subagent
> 用 Claude Code 對話時，Claude 每次派出去搜尋或執行任務的就是 subagent。
> 不需要 API key，訂閱就包含了。API key 的路線是給**自己開發 agent 系統**用的。

---

## 一、Claude Code 中使用 Subagent（UI 層）

在 Claude Code 對話裡，Claude 自己會決定何時派出 subagent。
你也可以主動透過提示觸發：

```
# 明確要求 Claude 用 subagent 並行處理
請用多個 subagent 同時處理以下三件事：
1. 分析 src/ 目錄的程式碼結構
2. 搜尋有沒有 TODO 和 FIXME 的地方
3. 檢查 package.json 的依賴有沒有過期
```

### 內建 Subagent 類型

| 類型 | 用途 |
|---|---|
| `general-purpose` | 通用任務（預設）|
| `Explore` | 唯讀搜尋、定位程式碼，不修改檔案 |
| `Plan` | 規劃實作方案，不執行 |

> [!tip] 何時 Claude 會自動派 subagent？
> - 任務可以**平行處理**（多個獨立子任務）
> - 需要**保護主 context**（大量搜尋結果不塞進主對話）
> - 需要**獨立判斷**的子任務（避免主 agent 偏見干擾）

---

## 二、用 Agent SDK 讓 Agent 呼叫 Agent

### 架構概念

```
Orchestrator（主控）
  ├─ 接收使用者任務
  ├─ 拆解成子任務
  ├─ 呼叫 Subagent（新的 API call）
  └─ 整合結果回傳
```

### Python 範例：Orchestrator + Subagent

```python
import anthropic

client = anthropic.Anthropic()

# ── Subagent（負責單一專責任務）──
def run_subagent(task: str, context: str = "") -> str:
    """一個獨立的 subagent，接任務、回結果"""
    response = client.messages.create(
        model="claude-sonnet-4-6",        # subagent 用便宜模型
        max_tokens=2048,
        system="你是一個程式碼審查專家，只做程式碼品質分析，回傳結構化報告。",
        messages=[{
            "role": "user",
            "content": f"任務：{task}\n\n背景：{context}"
        }]
    )
    return response.content[0].text


# ── Orchestrator（主控，決定派誰去做什麼）──
def orchestrator(user_request: str) -> str:
    """主 agent，拆解任務並協調 subagent"""

    # 第一步：讓主模型規劃子任務
    plan_response = client.messages.create(
        model="claude-opus-4-7",          # orchestrator 用強模型
        max_tokens=1024,
        system="你是一個任務協調者，把複雜任務拆成 2-3 個獨立子任務，用 JSON 格式輸出。",
        messages=[{"role": "user", "content": user_request}]
    )
    # 假設回傳 ["子任務A", "子任務B", "子任務C"]
    subtasks = ["分析程式碼結構", "找出潛在 bug", "建議優化方向"]  # 實際解析 JSON

    # 第二步：並行派出 subagent（這裡示範循序，並行需用 threading/asyncio）
    results = []
    for subtask in subtasks:
        result = run_subagent(
            task=subtask,
            context="目標程式庫：FastAPI 後端，Python 3.12"
        )
        results.append(f"## {subtask}\n{result}")

    # 第三步：整合結果
    combined = "\n\n".join(results)
    final_response = client.messages.create(
        model="claude-opus-4-7",
        max_tokens=2048,
        system="整合多個分析報告，輸出一份精簡的最終建議。",
        messages=[{"role": "user", "content": combined}]
    )
    return final_response.content[0].text


# 使用
result = orchestrator("幫我全面分析這個 Python 專案的程式碼品質")
print(result)
```

### 並行呼叫 Subagent（真正同時執行）

```python
import anthropic
from concurrent.futures import ThreadPoolExecutor, as_completed

client = anthropic.Anthropic()

def run_subagent(task: str) -> tuple[str, str]:
    response = client.messages.create(
        model="claude-sonnet-4-6",
        max_tokens=1024,
        messages=[{"role": "user", "content": task}]
    )
    return task, response.content[0].text

subtasks = [
    "分析 src/ 的目錄結構",
    "找出所有 TODO 和 FIXME 註解",
    "檢查有沒有未處理的 exception",
]

# 並行執行，最多同時跑 3 個
with ThreadPoolExecutor(max_workers=3) as executor:
    futures = {executor.submit(run_subagent, task): task for task in subtasks}
    results = {}
    for future in as_completed(futures):
        task, result = future.result()
        results[task] = result
        print(f"✓ 完成：{task}")
```

---

## 三、計費方式

> [!warning] Subagent 的費用比你想的多
> 每個 subagent = 一次獨立的 API call，各自計費。
> Orchestrator 本身也是一次 API call。

### 計費結構

```
總費用 = orchestrator 費用
       + subagent_1 費用
       + subagent_2 費用
       + ... (每個都是獨立 API call)
```

### Token 怎麼算

| 角色 | Input tokens 包含 | Output tokens |
|---|---|---|
| **Orchestrator** | system prompt + 使用者訊息 + tools 定義 | 規劃結果 + 最終整合 |
| **每個 Subagent** | 它自己的 system prompt + 任務描述 + 傳入的 context | subagent 的回應 |

### 成本估算範例

假設：1 個 orchestrator + 3 個 subagent，都用 Sonnet 4.6（$3/$15 per MTok）

| 角色 | Input | Output | 費用 |
|---|---|---|---|
| Orchestrator（規劃） | 1,000 tokens | 500 tokens | ~$0.011 |
| Subagent × 3（各 2,000 in / 1,000 out）| 6,000 tokens | 3,000 tokens | ~$0.063 |
| Orchestrator（整合）| 4,000 tokens | 1,000 tokens | ~$0.027 |
| **合計** | | | **~$0.10** |

vs. 同樣任務單一 agent 處理：約 $0.03–0.05

→ **Multi-agent 約貴 2–3 倍，但可以並行、速度更快**

### 省錢策略

```python
# ❌ 貴：orchestrator 和 subagent 都用 Opus
model="claude-opus-4-7"  # $5 / $25 per MTok

# ✅ 省：orchestrator 用 Opus 決策，subagent 用 Sonnet 執行
orchestrator_model = "claude-opus-4-7"   # 決策用強模型
subagent_model = "claude-sonnet-4-6"     # 執行用便宜模型
```

> [!tip] 傳給 subagent 的 context 越少越省錢
> Context 只傳 subagent **需要的部分**，不要把整個對話歷史丟給它。
> 每多 1,000 input tokens，每個 subagent call 就多 $0.003（Sonnet 價）。

---

## 四、Claude Code 訂閱用戶的計費

> [!info] Claude Code Pro/Max 訂閱
> - 訂閱費用**包含**一定量的 Claude Code 使用量
> - Subagent 使用和一般對話**共用同一個配額**
> - 大量 subagent 任務（例如一次跑 50 個 subagent）可能觸發 **rate limit**
> - 超過配額後需等待重置，或升級方案

> [!warning] API key 用戶（pay-as-you-go）
> 每個 subagent call 都直接計費，無月費上限。
> 建議設定 `max_tokens` 上限避免意外超支。

---

## 五、設計 Multi-Agent 的注意事項

### Subagent 要「冷啟動」——不知道你的上下文

```python
# ❌ 錯誤：subagent 不知道「那個 bug」是什麼
run_subagent("修復那個 bug")

# ✅ 正確：把必要 context 一起傳
run_subagent(
    task="修復以下 bug",
    context="""
    檔案：src/auth.py 第 42 行
    錯誤訊息：KeyError: 'user_id'
    相關程式碼：
    def get_user(data):
        return data['user_id']  # 沒有檢查 key 是否存在
    """
)
```

### 常見架構模式

| 模式 | 說明 | 適合情境 |
|---|---|---|
| **Fan-out / Fan-in** | 1 個 orchestrator → N 個 subagent → 整合結果 | 可並行的分析任務 |
| **Pipeline** | Agent A → Agent B → Agent C（循序）| 有依賴關係的步驟 |
| **Specialist Pool** | Orchestrator 按任務類型分配給不同專家 agent | 多領域任務 |
| **Recursive** | Agent 自己再呼叫自己（有深度限制）| 遞迴分解問題 |

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Subagent 在 Claude Code 的位置
- [[AI 101 - Harness Engineering]] — Agent 執行框架設計
- [[AI 101 - Context Engineering]] — 如何設計傳給 subagent 的 context
- [[AI 101 - 模型費用與效果比較]] — 各模型定價，選哪個當 subagent
