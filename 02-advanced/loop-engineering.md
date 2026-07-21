---
title: AI 101 - Loop Engineering（迴圈工程）
tags: [ai, loop-engineering, agent, 自動化, claude-code, 進階]
created: 2026-07-21
---

# Loop Engineering（迴圈工程）— Designing the Loop That Prompts Your Agent

[← 回主頁](../index.md)

> [!NOTE]
> **Loop Engineering = 設計「會自己提示 agent 的系統」，而不是你每一步都手動打字叫它做。** 讓 agent 自己「做事 → 檢查結果 → 決定下一步 → 重來」，直到目標達成。2026 年的共識是：**分出好 agent 和普通 agent 的，往往不是模型，而是這個迴圈。**

> **TL;DR (EN):** Loop Engineering is designing the system that prompts, checks, remembers, and re-runs an agent — so you stop being the one typing every next instruction and become the one who designs the system that does. In 2026 the loop (trigger, verifier, context, stop rules), not the model or the prompt, is where the leverage lives.

---

## 為什麼重要 — Why It Matters

這是一條清楚的演進線，KB 前面幾篇剛好排成順序：

**Prompt（怎麼問）→ [Context](./context-engineering.md)（餵什麼資料）→ [Harness](./harness-engineering.md)（外層框架）→ Loop（自主迴圈）**

每一棒的槓桿都往「離模型更遠、離系統更近」移動。到了 Loop 這一棒：**你不再是「提示 agent 的人」，而是「設計那個提示 agent 的系統的人」。**

**迴圈（loop）和鏈（chain）差在哪：**

| | 鏈 chain | 迴圈 loop |
|---|---|---|
| 形狀 | 直線、固定（A → B → C） | 循環、可改（會重複、分支、換方向） |
| 行為 | 跑一次就結束 | 看結果再決定，反覆到目標達成 |

Claude Code、Codex、Devin 這些工具的核心運作就是迴圈：**讀檔 → 寫 code → 跑測試 → 讀錯誤 → 修 → 再跑**，不用你每一步重新下指令。

---

## 一個 loop 由什麼組成 — Anatomy of a Loop

最小骨架四件事：

1. **觸發（trigger）**：什麼時候開始跑（手動、排程 cron、CI 失敗、收到訊息）
2. **可驗證的目標 + 驗證者（verifier）**：怎樣算「做完了」，而且**能被檢查**（測試通過、lint 乾淨）——沒有可驗證的目標，迴圈不知道何時該停
3. **上下文管理（context management）**：每一輪 agent「看得到什麼」。它記不記得 8 輪前試過、失敗過什麼？**每一輪看到的，比你一開始講的更重要**
4. **停止規則 + 護欄（stop rules & guardrails）**：防止無限迴圈、防止燒錢失控

Addy Osmani 進一步把「一個完整迴圈系統」拆成六塊積木：

| 積木 | 作用 | 在 Claude Code 對應 |
|---|---|---|
| **自動化 Automations** | 排程自動觸發 | `/loop`、`/goal`、cron、GitHub Actions |
| **Worktree** | 隔離並行 agent，避免改到同一份檔案打架 | `git worktree`（每個任務一份獨立工作區） |
| **Skills** | 把專案知識寫成檔，不用每次重講 | `SKILL.md` |
| **外掛／連接器 Connectors** | 接外部工具 | MCP（接 Linear、Slack、DB、API） |
| **Sub-agents** | 把「寫的人」和「驗的人」分開 | 不同指令／模型的子 agent，[分工與計費](./subagent-usage-and-billing.md) |
| **狀態檔** | 記已完成／待辦，讓明天的迴圈接得上 | Markdown 或 Linear 看板 |

> [!TIP]
> 上下文管理有個實作重點：**要跨越壓縮（compaction）存活的指令，寫進 `CLAUDE.md`**——它每一輪都會重新注入，不會因為對話被壓縮而遺失。

---

## 在 Claude Code 怎麼做 — In Practice

Claude Code 內建了迴圈工程的關鍵零件：

- **`/goal`**：設一個可驗證的完成條件，讓它自己跑到達成（見 [Claude Code goal](../01-fundamentals/claude-code/goal.md)）
- **`/loop`**：讓它按節奏反覆執行某個任務
- **強制力 Hook**：用 [goal 強制力 Hook](../01-fundamentals/claude-code/goal-enforcement-hooks.md) 防止它中途放棄、跑偏、忘記目標
- **Worktree + Sub-agent**：並行多個 agent，寫的和驗的分開

**一個完整迴圈長這樣**（Addy 的範例）：

```
每天排程觸發 → 分類 skill 讀 CI 失敗與待辦 issue
   → 每項發現開一個獨立 worktree
   → 一個 sub-agent 草擬修復，另一個 sub-agent 驗證
   → connector 自動開 PR、更新票、通知團隊
   → 狀態檔記錄進度，明天的迴圈接著跑
```

---

## 陷阱 — Pitfalls

迴圈跑得越自動，越要小心這幾個坑：

- **無人監督的驗證**：迴圈自己驗自己，若驗證機制不可靠，錯的東西會被自動放行——**驗證者要獨立、要可信**（呼應 [先驗證，再突破](../05-notes/ai-verify-then-expand.md) 的「獨立驗算」）
- **理解債（understanding debt）**：code 生成越快，你對自己專案的理解欠得越多，總有一天要還
- **認知投降（cognitive surrender）**：設計迴圈很容易變成「用它來避免思考」，而不是「用它來加速思考」
- **無限迴圈 / 燒錢失控**：沒有停止規則與成本護欄，agent 會一直跑

> [!IMPORTANT]
> Addy Osmani 的提醒：**「以打算留任的工程師身份來建迴圈，而不是只按下執行鍵的人。」** 迴圈是放大器——放大你的產出，也放大你偷懶的後果。

---

## 是真突破，還是新瓶裝舊酒？— Breakthrough or Rebrand?

老實說：**Loop Engineering 沒有技術突破，零件全是舊的。**

- act → observe → decide → repeat 的迴圈，就是 2022 年的 **ReAct**
- `/goal`、`/loop`、sub-agent、worktree、skills、MCP —— 全是現成工具
- 上下文管理、外層框架 —— 就是 [Context](./context-engineering.md) 和 [Harness Engineering](./harness-engineering.md)

沒有新演算法、新模型能力、新原語。那它到底新在哪？只有兩件事：

1. **命名 + 重點轉移**：像「DevOps」——沒發明任何技術，只是幫一種做法取名。一旦「迴圈」成為你思考的單位，你才會開始問對的問題（verifier 是什麼？停止規則？狀態怎麼存？）。這是心智重構，不是新能力。
2. **一個門檻被跨過**：早期 agent 跑幾步就漂移，你非盯著不可。2026 因為自動壓縮 + `CLAUDE.md` 重注入、worktree 安全並行、長時自主更穩，**「設好迴圈、走人」第一次變得實際可行**——不是新原語，是舊零件終於穩到能組成「你能放著不管」的系統。

> [!NOTE]
> 誠實的結論：Loop Engineering 比較像「**DevOps 時刻**」（幫一種實踐命名），不是「發明」。覺得它「跟原本差不多」是準確的判斷——差別在**可靠度跨過了能無人值守的門檻**，再加上一個讓大家對齊注意力的名字。值不值得叫一個新「XX Engineering」，見仁見智。

---

## 相關筆記 — Related

- [Context Engineering](./context-engineering.md)、[Harness Engineering](./harness-engineering.md) —— 演進線的前兩棒
- [Claude Code goal](../01-fundamentals/claude-code/goal.md)、[goal 強制力 Hook](../01-fundamentals/claude-code/goal-enforcement-hooks.md) —— 迴圈的「可驗證目標」與「護欄」
- [Subagent 使用與計費](./subagent-usage-and-billing.md) —— 寫者／驗者分工
- [先驗證，再用它突破自己](../05-notes/ai-verify-then-expand.md) —— 迴圈裡「驗證」為什麼是關鍵

## Sources

- [Loop Engineering — Addy Osmani](https://addyosmani.com/blog/loop-engineering/)
- [What Is Loop Engineering? The New Meta for AI Coding Agents — MindStudio](https://www.mindstudio.ai/blog/what-is-loop-engineering-ai-coding-agents)
- [Agentic Loops: From ReAct to Loop Engineering (2026) — Data Science Dojo](https://datasciencedojo.com/blog/agentic-loops-explained-from-react-to-loop-engineering-2026-guide/)
- [Loop Engineering for AI Agents (2026 Guide) — HappyCapy](https://happycapy.ai/blog/loop-engineering-ai-agents)
- [How the agent loop works — Claude Code Docs](https://code.claude.com/docs/en/agent-sdk/agent-loop)
