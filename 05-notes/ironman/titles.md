---
title: AI 101 - 鐵人賽三十篇標題與素材對照
tags: [ai, 鐵人賽, ironman, 寫作, 規劃, 個人筆記]
created: 2026-08-31
---

# 三十篇標題與素材對照 — The 30 Titles

[← 回主頁](../../index.md)｜[參賽規劃](./plan.md)

> [!NOTE]
> 完成的草稿放在 [`drafts/`](./drafts/)。依照 [參賽規劃](./plan.md) 的骨架（實際排成 **承 7 / 轉 16 / 合 7**；Day 29「新瓶裝舊酒」已併入 Day 30），把三十篇的標題定出來，並標上每篇用哪份現成筆記、還缺什麼。**這是草案，等 jason3e7 過目後再定稿。**

> **TL;DR (EN):** Thirty working titles for the Claude AI group, mapped to existing notes in this repo. Twenty-two are rewrites of material that already exists; eight need new writing. Five articles carry a first-hand experiment — those are placed early, where readers decide whether to follow the series.

---

## 系列名稱候選 — Series Title Options

「AI 心法」四個字太抽象、搜尋不到，也讀不出承諾。三個候選：

| # | 候選名稱 | 走向 |
|---|---|---|
| A | **AI 心法三十帖：用 Claude Code 當實驗場，從提問、驗證到突破自己的想像** | 完整、切題、說得清楚要給什麼。稍長 |
| B | **不是學工具，是學心法：30 天把 Claude Code 的踩坑蒸餾成方法** | 有對比、有態度，呼應今年「蒸餾」的賽事主視覺 |
| C | **AI 心法：問得準、驗得出、想得更遠的 30 天** | 最短，三個動詞直接對到系列的三段結構 |

> [!TIP]
> 個人建議 **A**。iThome 的搜尋與列表只看得到標題，「Claude Code」「提問」「驗證」這幾個關鍵字要出現在標題裡。

---

## 三十篇標題 — The Titles

狀態欄：**改寫** = 現成筆記改寫即可；**新寫** = 沒有現成素材；**補實測** = 有內容但要補自己的實驗才有份量。

### 承：基礎與定位（Day 1–7）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 01 | 它只是在猜下一個字：LLM 的原理，決定了後面 29 天的所有心法 | [core-concepts](../../01-fundamentals/core-concepts.md) + 新研究 | ✅ [已完成](./drafts/day01-llm-is-statistics.md) |
| 02 | 不是它突然變強，是它跨過了你的門檻 | 新研究（METR、scaling laws、MCP 採用曲線） | ✅ [已完成](./drafts/day02-why-it-got-strong.md) |
| 03 | 它做不到什麼：三種極限，和最危險的那一種 | 新研究（自我修正、反轉詛咒、context rot、鋸齒狀前沿） | ✅ [已完成](./drafts/day03-what-it-cannot-do.md) |
| 04 | 你其實只用了 AI 的兩種能力：一張全景圖看完它會什麼 | [ai-capability-landscape](../../02-advanced/ai-capability-landscape.md) | ✅ [已完成](./drafts/day04-capability-landscape.md) |
| 05 | Prompt Engineering：哪些技巧真的有效，哪些只是傳說 | [tips-and-best-practices](../../01-fundamentals/tips-and-best-practices.md) + 新研究 | ✅ [已完成](./drafts/day05-prompt-engineering.md) |
| 06 | 四種能力怎麼用：摘要、解釋、發想、重構的實戰配方 | [four-capabilities-playbook](../../02-advanced/four-capabilities-playbook.md) | 改寫 |
| 07 | 選模型與省錢：同一件事，成本可以差十倍 | [model-cost-comparison](../../01-fundamentals/model-cost-comparison.md)、[subagent 計費](../../02-advanced/subagent-usage-and-billing.md) | 改寫 |

### 轉·提問：問得準（Day 8–11）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 08 | 一個好的 prompt 長什麼樣？先把「詳細一點」這種話戒掉 | [meta-prompting](../meta-prompting.md) | 改寫 |
| 09 | XY Problem：你問的問題，通常不是你真正的問題 | [meta-prompting](../meta-prompting.md) | 改寫 |
| 10 | 用 prompt 生 prompt：一個可以直接複製的 MVP 模板 | [meta-prompting](../meta-prompting.md) | 改寫 |
| 11 | 讓 prompt 自己檢查自己：把驗證寫進提示裡 | [meta-prompting](../meta-prompting.md) | 改寫 |

### 轉·脈絡：餵什麼比怎麼問更重要（Day 12–14）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 12 | Context Engineering：餵什麼，比怎麼問更重要 | [context-engineering](../../02-advanced/context-engineering.md) | 補實測 |
| 13 | Harness Engineering：模型動不了，但外面那層可以 | [harness-engineering](../../02-advanced/harness-engineering.md) | 補實測 |
| 14 | Loop Engineering：從「我提示 AI」到「我設計那個提示 AI 的系統」 | [loop-engineering](../../02-advanced/loop-engineering.md) | 補實測 |

### 轉·目標：讓它自己跑（Day 15–18）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 15 | 權限：你願意讓 AI 動到哪裡？五種模式與一條紅線 | [permissions](../../01-fundamentals/claude-code/permissions.md) | 改寫 |
| 16 | `/goal`：給它一個能驗證的終點，它才知道什麼時候該停 | [goal](../../01-fundamentals/claude-code/goal.md) | 改寫 |
| 17 | 光有目標還不夠：用 Hook 逼它別中途放棄（含一個我實測失敗的 Hook） | [goal-enforcement-hooks](../../01-fundamentals/claude-code/goal-enforcement-hooks.md) | 改寫 |
| 18 | Workflow × Goal：讓它自己排隊、自己交差 | [workflow-goal-combo](../../01-fundamentals/claude-code/workflow-goal-combo.md) | 改寫 |

### 轉·驗證與擴展：驗得出、想得遠（Day 19–23）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 19 | 人要怎麼驗證 AI？跟數學借六種驗算法 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 20 | 獨立驗算為什麼最強：別讓它改自己的考卷 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 21 | 實測：Claude Code 在 HTB 靶機上，目標是怎麼被綁架的 | [htb/](../htb/htb-abducted-goal-case.md) 三案例 | 改寫 |
| 22 | 人做不到想像之外的事：用 AI 拓展視野的五個手段 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 23 | 知識金字塔：AI 打掉哪一層壁壘，誰被擠壓了 | [ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) | 改寫 |

### 合：已經在發生的事，怎麼接（Day 24–30）

| Day | 標題 | 素材 | 狀態 |
|---|---|---|---|
| 24 | 為什麼專家用 AI 賺更多，新手卻只快了一點 | [ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) | 改寫 |
| 25 | 這段字是 AI 寫的嗎？浮水印怎麼運作、為什麼不能當證據 | [ai-content-watermark](../../01-fundamentals/ai-content-watermark.md) | 改寫 |
| 26 | 送出去之前：把敏感的東西留在自己手上 | [pii-masking](../../03-tools/pii-masking.md)、[ollama-guide](../../04-local-llm/ollama-guide.md) | 改寫 |
| 27 | 拿掉「拒絕」的真正代價：無審查模型實測 | [qwen3-6-27b-uncensored](../../04-local-llm/qwen3-6-27b-uncensored.md) | 改寫 |
| 28 | AI 已經會自己打靶了：自主滲透工具的現況 | [autonomous-pentest-tools-comparison](../../03-tools/security/autonomous-pentest-tools-comparison.md) | 改寫 |
| 29 | 我親手測了一次 ClickFix：AI 分享頁怎麼被拿來騙人 | 06 的 ClickFix 筆記（自己的截圖） | 改寫 |
| 30 | 三十天蒸餾：如果只能留下幾條心法 | **方向：跟著 AI 持續成長**（jason3e7 指定） | 新寫 |

**盤點：已完成 2 篇、改寫 22 篇、補實測 4 篇、新寫 2 篇。** 帶第一手實測的有 Day 16、20、27，加上 12–14 補完後共 6 篇——其中三篇落在前二十天，正好是讀者決定要不要追下去的區間。

---

## 存稿排程 — Drafting Schedule

09/15 開賽、10/14 完賽。開賽前要有 12–15 篇成品。

| 期間 | 天數 | 目標 | 內容 |
|---|---:|---|---|
| 08/31 – 09/07 | 8 天 | 寫完 Day 01–10 | 承段 6 篇 + 提問段 4 篇（素材最完整，先清掉） |
| 09/08 – 09/14 | 7 天 | 寫完 Day 11–19 | 脈絡／目標／驗證段，**補實測的三篇排在這裡**，需要實際跑一輪並截圖 |
| 09/15 – 10/14 | 30 天 | 每天發 1 篇 + 寫 1 篇 | 庫存維持 10 篇以上；Day 28–30 留到最後寫，可以回收賽期中的讀者回饋 |

> [!WARNING]
> Day 26 的 ClickFix 那篇涉及惡意網址與 payload，發到公開站台時要**沿用 repo 既有的做法**：文字用防禦式寫法（`hxxp://` 與 `[.]`）、截圖只遮中段保留前綴。原始未遮蔽的圖不要上傳。

---

## Sources

- [2026 iThome 鐵人賽 — 競賽主題與活動說明](https://ithelp.ithome.com.tw/2026ironman/event)
- [2026 iThome 鐵人賽 — Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai)
