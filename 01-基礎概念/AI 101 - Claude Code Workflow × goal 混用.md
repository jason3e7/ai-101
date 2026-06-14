---
title: AI 101 - Claude Code Workflow × goal 混用
tags: [ai, claude-code, workflow, goal, dynamic-workflows, ultracode, autonomous, orchestration]
created: 2026-06-14
---

# Claude Code Workflow × goal 混用

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> `/goal`（深度迴圈）和 Dynamic Workflows（廣度編排）是兩個正交的機制，可以組合但只能「鬆散」組合——關鍵限制是 goal 的評估器看不到 workflow 內部。這篇說明各自是什麼、怎麼混用、以及五種實用 pattern。

---

## 先搞清楚：兩個功能各是什麼

### Dynamic Workflows（動態工作流）

> [!info]
> 需要 Claude Code **v2.1.154+**，付費方案皆可（Pro 需在 `/config` 開啟）。

**本質：** 一個由 Claude **自動寫出來的 JavaScript 腳本**，在背景編排大量 sub-agent。腳本（不是 Claude 一個 turn 一個 turn）持有迴圈、分支和中間結果；你的 session 只收到**最後的答案**。

**怎麼觸發：**

```
# 1. 在 prompt 裡用 ultracode 關鍵字
ultracode: 稽核 src/routes/ 底下每個 API endpoint 是否缺少 auth 檢查

# 2. 設成整個 session 都自動規劃 workflow
/effort ultracode

# 3. 內建的 deep-research workflow
/deep-research <問題>

# 4. 跑已儲存的 workflow
/<name>
```

**管理指令：**

```
/workflows    # 列出進行中/已完成的 workflow，看進度（phase、agent 數、token）
```

存檔位置：`.claude/workflows/`（專案）或 `~/.claude/workflows/`（個人）。

**限制：** 最多 16 個並行 agent、單次 1,000 個 agent；執行中不能插入使用者輸入；腳本本身不能碰檔案系統/shell（只有它的 agent 可以）；只能在同一 session 內 resume。

### `/goal`

> [!info]
> 需要 Claude Code **v2.1.139+**。

**本質：** 設定一個完成條件，每個 turn 結束後一個小模型（預設 Haiku）讀「條件 + 對話內容」判斷是否達成，沒達成就繼續。底層是 **session 範圍的 prompt-based Stop hook**。

**關鍵限制：** 評估器**不會執行工具、不會讀檔案**——它只能判斷 Claude 已經在對話裡呈現出來的內容。

---

## 核心架構事實：深度 vs 廣度

| | `/goal` | Dynamic Workflow |
|---|---|---|
| 維度 | **深度**：一個 agent 一個 turn 一個 turn 跑到條件滿足 | **廣度/結構**：背景腳本編排很多 sub-agent |
| 控制權 | 每個 turn 結束回到你的 session | 跑到完才回傳，**不會逐 turn 交還控制權** |
| 中間結果 | 在對話 transcript 裡 | 在腳本變數裡，**不會進你的 context** |
| 本質 | 你 session 上的 Stop hook | 背景的單一 job |

---

## 最大的 Gotcha：評估器看不到 Workflow 內部

這是混用時最重要的一件事：

```
Dynamic workflow 跑成一個背景 job
  → 對你的 session 來說只浮現「一個結果」
    → /goal 評估器（在你 session 的 turn 邊界觸發）
        把整個 workflow 當成「一步」，不是一連串它能監督的步驟
```

評估器只讀你的 session transcript、從不呼叫工具，所以：

- ❌ 條件不能寫「workflow 的 phase 3 完成了」（內部 phase 永遠不進你的 context）
- ✅ 條件可以寫「workflow 產出的報告顯示沒有剩餘的 auth 漏洞」（針對**產物**，不是步驟）

> [!warning]
> 混用的鐵則：**goal 條件要針對 workflow 產出的 artifact，不要針對 workflow 的內部步驟。**

---

## 五種實用混用 Pattern

### Pattern A：Goal 當外層迴圈，Workflow 當單一內層步驟

最常見的組合思路：goal 負責**深度**（跑到驗證通過），需要**廣度**時讓 Claude 在 turn 中啟動 workflow（`/deep-research` 或 `ultracode`）。Goal 條件針對 workflow 浮現的結果。

```
/goal src/routes/ 底下所有 endpoint 都有 auth 檢查，
且 audit_report.md 顯示 0 個漏洞，或在 15 個 turn 後停止
```

Claude 在某個 turn 判斷需要大規模掃描時，自己跑 `ultracode` workflow，結果寫進 `audit_report.md`，評估器讀檔案內容判斷。

### Pattern B：`/goal` + `/effort ultracode`（官方building block）

```
/effort ultracode
/goal 所有測試通過且程式碼覆蓋率 > 80%，或在 20 個 turn 後停止
```

`ultracode` 讓 Claude 每個任務自動規劃 workflow，goal 負責驗證迴圈。**Token 成本最高**。

### Pattern C：`/goal` + Auto mode（官方明確推薦）

```bash
claude --permission-mode auto
```

再設 `/goal`。Auto mode 消除每個工具的確認提示，goal 消除每個 turn 的等待——這是文件**明確推薦**的無人值守組合。

### Pattern D：Skill 定義多步驟流程，包在 Goal 裡（最乾淨）

把結構化步驟寫成 `SKILL.md`（skill 在 session 內執行，所以評估器**看得到每步的輸出**），再用 goal 包住整個流程：

```markdown
---
name: secure-audit
description: 多步驟資安稽核流程
---
1. 列出所有 API endpoint
2. 對每個 endpoint 檢查 auth middleware
3. 把發現寫進 audit_report.md
```

```
/secure-audit
/goal audit_report.md 存在且列出所有 endpoint 的 auth 狀態
```

> [!tip]
> 這是**官方支援度最高**的「結構化流程 + goal 迴圈」組合方式——因為 skill 在 session 內跑，評估器看得到 transcript 裡每一步的輸出。

### Pattern E：用 Stop Hook 取代 `/goal` 做確定性步驟把關

當你需要迴圈根據**腳本對 workflow 步驟的確定性檢查**（而不是模型讀 transcript）來把關時，自己寫 Stop hook 跑腳本。這是評估器「只看 transcript」不夠用時的官方逃生出口。

詳見 [[AI 101 - Claude Code goal 強制力 Hook]]。

---

## Plan Mode 和 Goal 的關係

**沒有直接整合。** Plan mode 是唯讀研究階段，結束時呈現計畫給你批准；`/goal` 驅動連續編輯 turn——兩者服務相反的階段。

自然順序：**先 plan → 批准 → 再設 `/goal` 驅動執行到驗收標準。** 但這是手動交接，不是整合功能。

---

## 衝突與注意事項

| 注意事項 | 說明 |
|---|---|
| **評估器對 workflow 內部盲目** | 最大陷阱：條件要針對 artifact 不是步驟 |
| **Workflow 是一個不透明步驟** | 背景 workflow 不逐 turn 交還控制，沒有自然的 per-step checkpoint |
| **Hooks 依賴** | `disableAllHooks` 會讓 `/goal` 完全失效；`disableWorkflows` 單獨關 workflow |
| **成本相乘** | goal 跑數十 turn × 每 turn 在 ultracode 下啟動 workflow（最多 16 並行/1000 總）= 非常貴。一定要加 turn/time 上限，先在小範圍測試 |
| **權限模式意外** | workflow sub-agent 一律跑 `acceptEdits` 模式；非白名單的 shell/MCP/web 呼叫仍可能中途跳確認，卡住原本無人值守的 goal 迴圈。先把需要的指令加進白名單 |
| **兩個方向都不是 wired** | Skill 不能設 goal；goal 不能宣告式呼叫 workflow——兩個方向都是 Claude 自己判斷（emergent），不是接線觸發 |

---

## 官方 vs 社群名詞

| 類型 | 名詞 |
|---|---|
| **官方支援** | Dynamic workflows（`ultracode`、`/effort ultracode`、`/workflows`、`/deep-research`、`.claude/workflows/`）；`/goal`；`/goal` + Auto mode（推薦）|
| **社群 pattern（非官方）** | 「depth × width」goal-loop worker 在 workflow fan-out 裡；`/goal` + `/loop` 做 validator 把關；把這個拆分叫「ultrawork / ultrareview」|

> [!warning]
> 沒有 `/workflow`（單數）指令；「ultrawork / ultrareview」不是官方關鍵字。官方詞是 `ultracode`、`/workflows`、`/deep-research`。

---

## 怎麼選

| 你的需求 | 建議 |
|---|---|
| 結構化多步驟 + 要驗證每步 | **Pattern D**（Skill + goal，官方支援度最高）|
| 完全無人值守跑到完成 | **Pattern C**（goal + Auto mode）|
| 需要大規模並行掃描/研究 | **Pattern A 或 B**（goal 外層 + workflow 內層）|
| 需要確定性腳本把關 | **Pattern E**（自寫 Stop hook）|

---

## 相關筆記

- [[AI 101 - Claude Code goal]] — `/goal` 完整說明
- [[AI 101 - Claude Code goal 強制力 Hook]] — Pattern E 的 Stop hook 實作
- [[AI 101 - Claude Code 行為結構設計]] — Skill / Sub-agent / Hook 的 Hello World

## Sources

- [Keep Claude working toward a goal](https://code.claude.com/docs/en/goal)
- [Dynamic Workflows](https://code.claude.com/docs/en/workflows)
- [Introducing Dynamic Workflows in Claude Code](https://claude.com/blog/introducing-dynamic-workflows-in-claude-code)
- [Claude Code /goal vs Dynamic Workflows](https://www.mindstudio.ai/blog/claude-code-goal-command-vs-dynamic-workflows)
- [Multi-agent surfaces comparison](https://code.claude.com/docs/en/agents)
