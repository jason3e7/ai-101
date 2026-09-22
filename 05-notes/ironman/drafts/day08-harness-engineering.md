---
title: "AI 101 - 鐵人賽 Day 08: Harness Engineering, 你已經在用只是不知道"
tags: [ai, 鐵人賽, ironman, harness-engineering, claude-code, codex, 草稿]
created: 2026-09-21
status: draft
---

# Day 08｜Harness Engineering: 你已經在用只是不知道

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> [Day 07](./day07-context-engineering.md) 拆完 Context Engineering: 動的是模型看到的 95%. 今天再往外一層: **Context 是被誰塞進去的? 塞的過程誰在管? 出錯誰處理?** 答案是 harness, 包裹 LLM 的完整執行環境. 你如果在用 Claude Code 或 Codex, 你已經在用一個成熟的 harness 了, 只是它藏在指令背後. 這一天教你認出它, 順便告訴你**你真正能動的位置只有四個, 其他都自動了**.

> **TL;DR (EN):** Harness = the whole execution scaffold wrapping the LLM: orchestration loop, context management, tools, memory, guardrails, error recovery. LangChain benchmark: changing only the harness (not the model) moved a coding agent from outside top-30 to top-5. Good news for 2026-09: Claude Code and Codex CLI have almost all six layers built in — orchestration loop, compaction, ~43 built-in tools + MCP, permissions system, hooks, circuit breakers, model fallback. You still touch four knobs: (1) CLAUDE.md/AGENTS.md for standing rules, (2) permissions and hooks for interception, (3) MCP scope for context budget, (4) when to spawn sub-agents. Retry loops, compaction schedules, sandbox isolation used to be homework — now they are included. Why still learn harness engineering? To debug where things break, to know the ceiling before you hit it, and to design multi-agent orchestration on top of the built-in primitives.

---

## 為什麼今天講 harness — Zooming Out

前兩天走的是**往內看**: prompt 是那一句 (Day 06), context 是模型讀到的所有東西 (Day 07). 今天**往外看**一層.

Context 不是憑空出現在模型面前的. 有一個系統負責:

- 決定 context 這一輪要塞什麼 (system prompt、對話歷史、tool 定義、檔案內容)
- 呼叫模型, 拿回應
- 解析回應裡的 tool call, 執行, 把結果餵回去
- 判斷該不該繼續、該不該壓縮、該不該停
- 出錯時決定要 retry 還是回報給你

這一整套叫 **harness**, 是包裹 LLM 的完整基礎設施. 模型只提供智慧, harness 提供控制.

LangChain 官方有一組實測: **只改 harness、不換模型, 一個 coding agent 從排行榜前 30 名外跳到前 5 名**. 這個數字很嚇人, 但方向對: 效能大頭不在模型本身.

---

## 六大組成 — Six Layers

不管哪家 harness, 骨架都是這六塊:

| 層 | 做什麼 |
|:---|:---|
| **編排迴圈** | 主循環: 呼叫模型 → 執行工具 → 判斷停止 → 重來 |
| **Context 管理** | 每輪決定塞什麼、壓縮什麼、清空什麼 |
| **工具集** | 模型能呼叫的 tool: bash / read / grep / MCP 等 |
| **狀態與記憶** | in-context (單輪) / short-term (跨輪) / long-term (跨 session) / external |
| **護欄** | 執行前檢查、執行中攔截、輸出前驗證 |
| **錯誤恢復** | Retry、fallback、部分完成、人工介入 |

Day 07 的 Context Engineering 其實是第二層 (Context 管理) 加第四層 (狀態記憶) 的組合. 這一天把周圍的四層一起補上.

---

## Claude Code / Codex 已經做了多少 — What's Already Done

短答: **六大組成裡, 幾乎全部預設就有.** 你不用寫任何一行 harness code, 開 Claude Code 進去就用.

| 層 | Claude Code 內建 | Codex CLI 內建 |
|:---|:---|:---|
| **編排迴圈** | `query()` async generator + mutable state | Codex core agent loop, Item / Turn / Thread |
| **Context 管理** | 微壓縮 (每輪) + 反應式壓縮 (滿了觸發) + 5 分鐘 prompt cache | Thread resume/fork/archive + compaction |
| **工具集** | ~43 內建 tool + MCP | 內建 + MCP + sandbox |
| **狀態與記憶** | 對話自動存 + CLAUDE.md + MEMORY.md (~200 行) | AGENTS.md + plugins |
| **護欄** | 權限系統 (deny/allow/classifier) + Hooks (PreToolUse / PostToolUse / Stop / SubagentStop / PreCompact) | Approval policy + sandbox modes |
| **錯誤恢復** | Circuit breaker (compaction 連錯 3 次停) + context overflow collapse + 529 fallback | 內建 retry + sandbox 隔離 |

2023 年真的要自己寫的 retry、compaction、tool 呼叫循環、sandbox 隔離, 現在都在 harness 裡. 這也是為什麼從 2024 之後「一般人也能用 agent」變成事實.

---

## 你真正能動的四個位置 — The Four Knobs

前面表格全自動的部分不用管. 你能動 (也需要動) 的其實只有這四個:

**一、`CLAUDE.md` / `AGENTS.md`, 常駐規則**

每輪自動注入, 適合放專案風格、禁止事項、你希望它每次都記得的事. [Day 07](./day07-context-engineering.md) 例一那個 refactor 從三輪變一輪, 就是這件事在做.

**二、Permissions ＋ Hooks, 攔截點**

Permissions 決定哪些工具動作要問你、哪些不用. `allow all Read/Grep, ask on Write` 是最常見設定, 大幅減少互動摩擦. Hooks 更進一步: 用 shell 指令綁定 lifecycle 事件, 例如**每次 Write 後自動跑 lint、每次 commit 前自動跑測試**.

**三、MCP scope, 你接哪些外部服務**

每個 MCP 都吃 context (見 [Day 07](./day07-context-engineering.md) 例二: 20 個 MCP 每輪多吃 5K 到 10K tokens). 挑最相關的 3 到 5 個就好. Claude Code 支援 project-level 設定, 特定 repo 才掛 GitHub MCP.

**四、Sub-agent 什麼時候派**

Explore、Plan 是 Claude Code 內建的 sub-agent, 打「幫我找 X」它會自動派. 自訂 sub-agent 用 `AgentTool` 呼叫. 什麼時候該派: **會產生大量中間結果的任務** (搜尋、探索、驗證), 派給 sub-agent, 只把結論回主對話.

**其他你都不用管.** 這是重點.

---

## Claude Code 上的三個具體例子 — Concrete Cases

**例一: Hook 讓 commit 訊息格式強制對**

`.claude/hooks/pre-commit.sh` 綁 PostToolUse (Bash + `git commit`), 檢查 message 有沒有符合 project 規範 (中文、動詞開頭、附 Co-Authored-By). 不對就中止. 這樣不用每次 code review 提醒 commit message, harness 幫你擋掉.

**例二: Permissions 設對, 減少 80% 的互動打斷**

預設每次 tool 呼叫都問你. 活躍的 refactor 任務一小時可以問你幾十次. `settings.json` 加:

```json
{
  "permissions": {
    "allow": ["Read", "Grep", "Glob", "Bash(git status)", "Bash(git diff)"],
    "ask": ["Write", "Edit", "Bash(git commit)"]
  }
}
```

Read / Grep 完全不打擾, 要寫入才問. 節省的時間非常明顯.

**例三: Explore sub-agent 找函式定義, 主 context 不被搜尋結果污染**

打「幫我找專案裡所有處理 auth 的地方」, Claude Code 會派 Explore sub-agent 去 grep 全 repo, 只回一個幾百 tokens 的摘要到主對話. 如果你直接在主對話 `Grep`, 那幾千行搜尋結果會全部留在 context 裡, 之後每輪都被讀一次.

---

## Web chat 版本呢 — What About Web Chat

上面例子都是 Claude Code. 如果你用的是 web chat (Claude.ai、ChatGPT、Gemini), **web app 本身就是一個 harness**, 只是形狀跟 Claude Code 不同: 沒有 hooks、沒有 permissions 規則、沒有 worktree, 但也不用你配那些. 你要動的四個位置在 web chat 上都有對應:

| Claude Code 上你要動的 | Web chat 對應 |
|:---|:---|
| **CLAUDE.md / AGENTS.md** | Custom Instructions / Claude Projects Instructions / ChatGPT Custom GPT instructions |
| **Permissions ＋ Hooks** | 沒有 hooks; 但每則訊息可 toggle 啟用哪些工具 (web search、artifacts、extended thinking 等) |
| **MCP scope** | Claude Connectors (Notion、GDrive、Slack…) / ChatGPT GPT Actions / Gemini Extensions |
| **Sub-agent 派出去** | Custom GPTs / Claude Projects 當「專用助手」, 分不同 context |

具體幾條:

1. **每個 toggle 都是 harness 選擇**. Web search、artifacts、extended thinking、canvas 這些 mode 都吃 context 和延遲. **不需要就別開**, 例如寫詩不需要 web search
2. **模型選擇也是 harness 選擇**. Sonnet 做快速迭代、Opus 做深度思考、o-series 做長推理. 選錯 mode 等於選了不合適的 harness
3. **Custom GPTs / Projects 就是 web 版的 harness config**. 固定工作流 (審 code、翻譯、市場分析) 建一個 dedicated GPT / Project, 綁好 instructions、files、tools, 別每次從零開始
4. **Connectors 精選**. 每個 connector 也吃 context. 挑「這個 project 一定會用到」的就好, 別把 Notion / GDrive / GitHub / Slack 全掛
5. **沒 hooks 的替代**: 想要「每次回答前先驗證、完成後檢查」這種 hook 行為, 寫進 project instructions 用 prompt 模擬. 例如「每次寫完 code 後, 列出你沒 handle 的 edge case」. 效果比 hook 弱但可用

## 那還要學 Harness Engineering 幹嘛 — Why Still Learn It

既然預設都做好了, 為什麼還要懂:

- **看得懂錯在哪**: agent 卡住、context 爆、tool 選錯, 知道是哪一層在管才能修
- **知道天花板在哪**: 有些場景硬要用 Claude Code 會撞牆 (100+ 並行、CI 級 audit log), 不是 harness 的錯, 是選錯工具形狀
- **自己蓋 harness 的時候**: 六大組成一個一個對, 少掉三分之二的坑
- **多 agent 協作**: 內建的 sub-agent 是骨架, orchestration 邏輯 (Planner → Generator → Evaluator 那類) 還是你要設計

---

## 這一天要記的一句話 — One Line

> **Harness 是模型的鎧甲: 你不用打造, 但要知道它有哪些關節. 認得關節, 就知道哪裡要綁緊、哪裡不能硬砸.**

---

## Sources

- [Harness Engineering (本 repo)](../../../02-advanced/harness-engineering.md), 六大組成完整拆解
- [Effective harnesses for long-running agents — Anthropic Engineering](https://www.anthropic.com/engineering/effective-harnesses-for-long-running-agents)
- [Harness design for long-running apps — Anthropic Engineering](https://www.anthropic.com/engineering/harness-design-long-running-apps)
- [The Anatomy of Claude Code — sidbharath](https://sidbharath.com/blog/the-anatomy-of-claude-code/)
- [Unrolling the Codex agent loop — OpenAI](https://openai.com/index/unrolling-the-codex-agent-loop/)
- [Unlocking the Codex harness — OpenAI (2026-08)](https://openai.com/index/unlocking-the-codex-harness/)
- [How we built our coding agent harness — LangChain Blog](https://blog.langchain.com/)
