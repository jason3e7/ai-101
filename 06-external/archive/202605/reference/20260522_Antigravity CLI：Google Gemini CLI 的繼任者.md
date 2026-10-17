---
title: "Antigravity CLI：Google Gemini CLI 的繼任者"
tags: [ai, 外部觀點, google, cli, agent, antigravity, gemini, coding-agent, google-io-2026]
source: https://developers.googleblog.com/an-important-update-transitioning-gemini-cli-to-antigravity-cli/
author: Google
created: 2026-05-22
---

# Antigravity CLI：Google Gemini CLI 的繼任者

> [!info]
> 官方公告：[Transitioning Gemini CLI to Antigravity CLI](https://developers.googleblog.com/an-important-update-transitioning-gemini-cli-to-antigravity-cli/)（2026-05-19）
> **一句話：** Google 在 I/O 2026 發布 Antigravity 2.0 平台，以 Antigravity CLI 取代 Gemini CLI，是 Google 對標 Claude Code 和 Cursor 的 Agentic 開發工具。

---

## 是什麼

**Antigravity CLI** 是 Google 的 terminal-based AI coding agent，
是 **Gemini CLI 的繼任者**，同時也是更大平台 **Antigravity 2.0** 的一部分。

Antigravity 2.0 是 Google 在 I/O 2026（2026-05-19）發布的整套 Agentic 開發環境，
包含：**桌面應用、CLI 工具、SDK**，定位對標 Cursor 和 Claude Code。

---

## 與 Gemini CLI 的差異

| 項目 | Gemini CLI | Antigravity CLI |
|---|---|---|
| **語言** | — | Go（更快、更低延遲）|
| **多 Agent** | 基本支援 | 非同步多 Agent 編排，支援背景執行 |
| **架構** | 獨立工具 | 與桌面應用共用後端 |
| **開源** | ✅ Apache 2.0 | ❌ 閉源（GitHub 只有 changelog 和 README）|
| **可用性** | 結束日：2026-06-18 | 2026-05-19 起全面開放 |

---

## 保留的核心功能

從 Gemini CLI 延續下來的功能：

- **Agent Skills** — 自訂技能擴充
- **Hooks** — 事件觸發 shell 指令
- **Subagents** — 派出子 Agent 平行執行
- **Plugins**（原 Extensions）— 第三方整合

> [!warning] 非 1:1 相容
> Google 官方聲明：「不會一開始就有完整的功能對等。」
> 部分 Gemini CLI 功能在 Antigravity CLI 初版尚未實作。

---

## Antigravity 2.0 平台全貌

| 元件 | 說明 |
|---|---|
| **桌面應用** | 整合 Google AI Studio、Android、Firebase |
| **CLI** | Terminal 環境的 Agentic 開發工具（本文主題）|
| **SDK** | 開發者自建客製 Agent 的框架 |
| **Voice** | 原生語音指令支援 |
| **AI Studio 匯出** | 從 AI Studio 匯出專案到本地繼續開發 |

底層模型：**Gemini 3.5 Flash**（官方表示這個模型本身就是用 Antigravity 協助開發的）。

---

## 遷移時程

| 日期 | 事件 |
|---|---|
| **2026-05-19** | Antigravity CLI 正式開放所有人使用 |
| **2026-06-18** | Gemini CLI 停止服務（免費、Pro、Ultra 用戶）；IDE Extension 個人版下線 |
| 企業例外 | 付費 Gemini Code Assist Standard/Enterprise 用戶仍可繼續使用 Gemini CLI |

---

## 社群反應

> [!warning] 開源倒退引發強烈反彈

開發者社群的主要批評：

1. **開源倒退**：Gemini CLI 是 Apache 2.0 開源，Antigravity CLI 改成閉源，社群認為 Google 利用開源貢獻打造了一個閉源繼任者
2. **使用限制**：用量配額嚴格，部分用戶反映每週配額做幾個 request 就耗盡
3. **不平等存取**：免費和個人用戶直接失去存取權，企業客戶才有保障

---

## 與其他 AI Coding Agent 的對比

| 工具 | 公司 | 開源 | 定位 |
|---|---|---|---|
| **Antigravity CLI** | Google | ❌ 閉源 | Cursor 競爭者，Gemini 生態系 |
| **Claude Code** | Anthropic | ❌ 閉源 | Terminal-based，訂閱制 |
| **Codex** | OpenAI | ❌ 閉源 | Cloud sandbox，JSON 輸出 |
| **OpenCode** | 社群 | ✅ 開源 | Claude Code 替代，支援 75+ 模型 |

---

## 定價更新（Antigravity 2.0 同期）

| 方案 | 價格 | 用量上限 |
|---|---|---|
| AI Pro | 原價 | 基準 |
| AI Ultra（新）| $100/月 | Pro 的 5 倍 |
| 原 $250 方案 | 降至 $200/月 | Pro 的 20 倍 |

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 的架構與生態
- [[AI 101 - Subagent 使用與計費]] — Agentic 開發的 Subagent 模式
- [[實驗：Claude 指揮 Codex 與 Codex Multi-Agent 測試]] — Claude + Codex 跨平台協作實驗
- [[AI 101 - OpenCode.ai]] — 開源的 Claude Code 替代方案

---

## 來源

- Google Developers Blog：[Transitioning Gemini CLI to Antigravity CLI](https://developers.googleblog.com/an-important-update-transitioning-gemini-cli-to-antigravity-cli/)
- TechCrunch：[Google launches Antigravity 2.0 at IO 2026](https://techcrunch.com/2026/05/19/google-launches-antigravity-2-0-with-an-updated-desktop-app-and-cli-tool-at-io-2026/)
- The Register：[Bye-bye, Gemini CLI; Google nudges devs toward Antigravity](https://www.theregister.com/ai-ml/2026/05/20/bye-bye-gemini-cli-google-nudges-devs-toward-antigravity/5243605)
- Virtualization Review：[Google Moves Gemini CLI Into Antigravity CLI](https://virtualizationreview.com/articles/2026/05/19/google-moves-gemini-cli-into-antigravity-cli-as-agent-platform-expands.aspx)
