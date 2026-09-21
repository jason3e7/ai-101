---
title: AI 101 - Harness Engineering
tags: [ai, harness-engineering, agent, architecture, advanced]
created: 2026-04-22
updated: 2026-04-22
---

# Harness Engineering

[← 回主頁](../index.md)

> [!quote]
> "The challenge of production AI is not about the model — it is about the harness.
> A well-designed harness can make even a modest model reliable and safe,
> while a poorly designed harness can make even the most capable model dangerous."

---

## 什麼是 Harness Engineering

**Harness** 是包裹 LLM 的完整軟體基礎設施——模型本身只提供「智慧」，Harness 提供「控制」。

```
┌──────────────────────────────────────────┐
│              Agent Harness               │
│                                          │
│  ┌────────────┐    ┌──────────────────┐  │
│  │ 編排迴圈   │    │   工具集（Tools） │  │
│  │ Orch. Loop │    │ bash/read/write.. │  │
│  └────────────┘    └──────────────────┘  │
│  ┌────────────┐    ┌──────────────────┐  │
│  │ Context    │    │  State & Memory   │  │
│  │ 管理       │    │  狀態與記憶       │  │
│  └────────────┘    └──────────────────┘  │
│  ┌────────────┐    ┌──────────────────┐  │
│  │ Guardrails │    │  Error Recovery   │  │
│  │ 護欄       │    │  錯誤恢復         │  │
│  └────────────┘    └──────────────────┘  │
│                ↕                         │
│         ┌──────────┐                     │
│         │   LLM    │  ← 只是其中一塊     │
│         └──────────┘                     │
└──────────────────────────────────────────┘
```

> [!info] 關鍵數字
> LangChain 的研究顯示：**只改 Harness、不換模型**，
> 可以讓一個 coding agent 從排行榜前 30 名外跳到前 5 名。
> AI Agent 70% 的表現取決於 Harness，而非模型本身。

---

## Harness 的六大組成

### 1. 編排迴圈（Orchestration Loop）

Agent 的主循環，控制「做什麼、何時停」。

```
輸入
  → 呼叫 LLM
    → 有工具呼叫？→ 執行工具 → 回到 LLM
    → 完成條件？ → 輸出結果
    → 錯誤？     → 錯誤恢復 → 重試或中止
```

Claude Code 使用的是 **"continue sites pattern"**：多個退出點，可以終止 turn 或帶著更新的狀態繼續下一輪迭代，實現精細的錯誤恢復。

### 2. Context 管理

控制 LLM「能看到什麼」，避免 context 爆滿或被雜訊佔據。

| 策略 | 說明 | 適用情境 |
|---|---|---|
| **Compaction（壓縮）** | 摘要歷史對話，同一 agent 繼續執行 | 一般情況 |
| **Context Reset（重置）** | 清空 context，新 agent 接手，透過結構化 artifact 交接狀態 | 長時間任務、context anxiety |
| **Adaptive Compaction** | 讓 agent 自己決定何時壓縮（不是到上限才壓），避免在關鍵任務中途被打斷 | 進階設計 |

> [!warning] Context Anxiety
> Anthropic 觀察到 Claude Sonnet 4.5 在 context 快滿時會表現出「焦慮」行為（匆忙結束、品質下降）。
> 解法：提前進行 Context Reset，而非等到 context 爆滿。
> Claude Opus 4.5 之後，此問題大幅改善。

### 3. 工具集（Tools）

Harness 決定 agent 能用哪些工具，以及如何呼叫。

**工具設計原則：**
- 只給任務需要的工具（不是越多越好）
- 工具的描述要清楚，避免 LLM 誤用
- 工具執行失敗要有明確的錯誤訊息

### 4. 狀態與記憶（State & Memory）

| 類型 | 生命週期 | 範例 |
|---|---|---|
| **In-context** | 單次 session | 對話歷史 |
| **Short-term** | 跨 turn | 任務進度 |
| **Long-term** | 跨 session | CLAUDE.md、MEMORY.md |
| **External** | 永久 | 資料庫、檔案系統 |

### 5. 護欄（Guardrails）

防止 agent 做出危險或不預期的行為。

三層護欄架構（OpenAI SDK 模式）：

```
Input Guardrails   → 第一個 agent 執行前檢查
Tool Guardrails    → 每次工具呼叫前檢查
Output Guardrails  → 最終輸出前檢查

任一層觸發「Tripwire」→ 立即中止 agent
```

### 6. 錯誤恢復（Error Recovery）

良好的 Harness 不應在遇到錯誤時直接崩潰。

**常見策略：**
- 重試（帶 backoff）
- 降級到備用工具
- 部分完成後儲存進度
- 回報錯誤並請求人工介入

---

## Claude Code / Codex 已經做了多少 — What's Built In (2026-09)

短答: **六大組成裡, Claude Code 幾乎全部預設就有, Codex CLI (2026-08 開源 harness 後) 也差不多.** Harness Engineering 這個詞聽起來像另一門要學的學問, 但你如果在用 Claude Code 或 Codex, 你已經在用一個成熟的 harness, 只是它藏在指令背後.

### 六大組成的實作對照

| 組成 | Claude Code | Codex CLI | 誰要動 |
|:---|:---|:---|:---|
| **編排迴圈** | 內建 `query()` async generator, mutable state, 循環到 stop | Codex core 的 agent loop, Item / Turn / Thread 三 primitives | 全自動 |
| **Context 管理** | 微壓縮 (每輪), 反應式壓縮 (滿了觸發), 5 分鐘 prompt cache | Thread 支援 resume/fork/archive, 有 compaction | 全自動 ＋ 你可以 `/clear`、`/compact` 干預 |
| **工具集** | ~43 內建 tool (bash/read/write/edit/grep/glob/agent/task/web) ＋ MCP | 內建 tool ＋ MCP ＋ 沙盒執行 | 內建自動; MCP 你決定接哪些 |
| **狀態與記憶** | 對話記錄自動存, CLAUDE.md 常駐, `MEMORY.md` 開機讀 ~200 行 | AGENTS.md (等同 CLAUDE.md), plugins 打包分發 | 你寫 CLAUDE.md / AGENTS.md |
| **護欄** | 權限系統 (deny/allow/classifier/prompt), Hooks (PreToolUse / PostToolUse / Stop / SubagentStop / PreCompact 等 lifecycle 事件) | Approval policy ＋ sandbox modes | 你設 permissions 跟 hooks |
| **錯誤恢復** | Circuit breaker (連 3 次 compaction 失敗停), context overflow → collapse → 重試, 529 fallback | 內建 retry, sandbox 隔離失敗 | 全自動 |

### 你還需要主動做的四件事

前面表格「你要動」那欄整理起來, 實際上就這四個:

1. **`CLAUDE.md` / `AGENTS.md`** — 常駐規則 (Context Engineering 的 write 那一招). 沒寫的話, harness 還是會跑, 但沒有專案風格
2. **Permissions 跟 Hooks** — 你想在哪些操作前檢查、哪些操作後觸發. 沒設的話, 預設會每次問你確認
3. **MCP 要接哪些** — 每個 MCP 都吃 context, 選擇要精 (見 [Day 07 Context Engineering](../05-notes/ironman/drafts/day07-context-engineering.md) 例二)
4. **Sub-agent 什麼時候派** — Explore / Plan 是內建的; 自訂的 subagent 用 `AgentTool` 呼叫. 沒派的話, 主 agent 會自己扛所有 context

**其他都自動了.** 你不用自己實作重試邏輯、compaction、tool 呼叫循環、sandbox 隔離, 那些 2023 年真的要自己寫, 現在都在 harness 裡.

### 那還要學 Harness Engineering 幹嘛

既然預設都做好了, 為什麼還要懂:

- **看得懂錯在哪**: 遇到 agent 卡住、context 爆、tool 選錯, 知道是哪一層在管才能修
- **知道天花板在哪**: harness 有既定行為, 有些場景硬要用它會撞牆 (例: 100+ 並行 agent, 或需要 CI 級 audit log, Claude Code 不是最適合的形狀)
- **自己蓋 harness 的時候**: 拿 Claude Code / Codex 當範本, 六大組成一個一個對, 少掉三分之二的坑
- **多 agent 協作**: 下一節那個 Planner / Generator / Evaluator 三 agent 架構, 內建的 sub-agent 是骨架但 orchestration 邏輯還是你要設計

---

## Multi-Agent Harness 設計

### Anthropic 三 Agent 架構

Anthropic 工程部落格揭露的長時間任務 harness 設計：

```
┌─────────────┐     artifact     ┌─────────────┐
│   Planner   │ ──────────────→ │  Generator  │
│  規劃 Agent │                  │  產生 Agent │
└─────────────┘                  └──────┬──────┘
                                        │ output
                                        ↓
                                 ┌─────────────┐
                                 │  Evaluator  │
                                 │  評估 Agent │
                                 └──────┬──────┘
                                        │ feedback
                                        └──→ Generator（下一輪）
```

**設計原則：**
- **分離關注點**：Planning / Generation / Evaluation 各司其職
- **結構化 Handoff**：Agent 間用定義好的 artifact 交接，不依賴 context 傳遞
- **Context 隔離**：每個 agent 有獨立的 context window

### Claude Code 的 Harness

```
Claude Code Harness
├── 一個主 Agent Loop
├── Tools（bash、read、write、edit、glob、grep、browser）
├── 按需載入的 Skills
├── 自動 Context Compaction
├── Subagent 派生（獨立 context）
├── Task system（有依賴圖的任務管理）
├── 非同步 mailbox（Agent 間通訊）
├── Worktree isolation（平行執行）
└── Permission governance（權限管理）
```

---

## Harness Engineering vs 其他概念

| 概念 | 關注點 |
|---|---|
| **Prompt Engineering** | 如何寫好 prompt |
| **Context Engineering** | 設計 AI 能看到什麼資訊 |
| **Harness Engineering** | 設計包裹 LLM 的完整執行基礎設施 |

三者是**遞進關係**：Harness 是最外層，Context Engineering 在其中，Prompt 在最內層。

```
Harness Engineering（最外層：基礎設施）
  └── Context Engineering（中層：資訊設計）
        └── Prompt Engineering（最內層：指令撰寫）
```

---

## Sources

- [Anthropic：Harness design for long-running apps](https://www.anthropic.com/engineering/harness-design-long-running-apps)
- [Anthropic：Effective harnesses for long-running agents](https://www.anthropic.com/engineering/effective-harnesses-for-long-running-agents)
- [The Anatomy of an Agent Harness](https://blog.dailydoseofds.com/p/the-anatomy-of-an-agent-harness)
- [Claude Code Agent Harness Architecture](https://wavespeed.ai/blog/posts/claude-code-agent-harness-architecture/)
- [LangChain：How we built our coding agent harness](https://blog.langchain.com/)
- [OpenAI Agent SDK：Guardrails 文件](https://platform.openai.com/docs/guides/agents)

---

## 相關筆記

- [AI 101 - Context Engineering](./context-engineering.md) — Harness 中的 Context 管理層
- [AI 101 - Claude Code 生態系](../01-fundamentals/claude-code/ecosystem.md) — Hooks、Subagents 是 Harness 的實作
- [AI 101 - 核心概念](../01-fundamentals/core-concepts.md) — Agent 基礎概念
