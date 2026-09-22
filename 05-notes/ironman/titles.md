---
title: AI 101 - 鐵人賽三十篇標題與素材對照
tags: [ai, 鐵人賽, ironman, 寫作, 規劃, 個人筆記]
created: 2026-08-31
---

# 三十篇標題與素材對照 — The 30 Titles

[← 回主頁](../../index.md)｜[參賽規劃](./plan.md)

> [!NOTE]
> 完成的草稿放在 [`drafts/`](./drafts/)。依照 [參賽規劃](./plan.md) 的骨架（實際排成 **承 8 / 轉 16 / 合 6**；Loop Engineering 提前到 Day 07 當「先看終點」，Day 29「新瓶裝舊酒」併入 Day 30），把三十篇的標題定出來，並標上每篇用哪份現成筆記、還缺什麼。**這是草案，等 jason3e7 過目後再定稿。**

> **TL;DR (EN):** Thirty working titles for the Claude AI group, mapped to existing notes in this repo. Twenty-two are rewrites of material that already exists; eight need new writing. Five articles carry a first-hand experiment — those are placed early, where readers decide whether to follow the series.

---

## 報名定案 — Registered

**參賽題目**

```
AI 心法三十天：用 Claude Code 當實驗場，從提問、驗證到拓展自己的想像
```

**題目簡介**（修訂版，約 165 字）

> [!NOTE]
> 這是 `[fixButNotPublish]` 修訂後的版本，**iThome 上仍是報名當下的原句**。原句與修改原因見 [fix-log.md](./fix-log.md)。

```
從「LLM 只是在猜下一個字」這個原理出發，推導出什麼時候該驗證、為何給對脈絡比問對問題更重要，以及哪些 prompt 技巧其實缺乏證據。每篇以 Claude Code 當實驗場，指令與設定照講，但重點在「為什麼這樣做」，並附上可以自己動手試一次的例子與我自己踩過的坑。最後收在 AI 會取代什麼、不會取代什麼，以及人怎麼跟著它繼續成長。
```

| 項目 | 值 |
|---|---|
| 組別 | **Claude AI** |
| 開賽日 | **2026-09-15**（選定後不可異動） |
| 完賽日 | 2026-10-14 |

> [!NOTE]
> 簡介刻意不寫「可重現的測試」 - LLM 是機率機器，同一題跑兩次結果就可能不同，宣稱可重現等於打自己臉。改成「可以自己動手試一次」。

---

## 三十篇標題 — The Titles

狀態欄：**改寫** = 現成筆記改寫即可；**新寫** = 沒有現成素材；**補實測** = 有內容但要補自己的實驗才有份量。

> [!IMPORTANT]
> **Day 05 是後來插進來的**（2026-09-18，jason3e7 指定）。原本的 Day 05 以後全部順延一天，Day 30 維持不動，被擠出編號的「ClickFix 實測」移到文末的[候補題目池](#候補題目池--backlog)。

### 承：基礎與定位（Day 1–8）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 01 | 它只是在猜下一個字：LLM 的原理，決定了後面 29 天的所有心法 | [core-concepts](../../01-fundamentals/core-concepts.md) + 新研究 | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10411345) |
| 02 | 不是它突然變強，是它跨過了你的門檻 | 新研究（METR、scaling laws、MCP 採用曲線） | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10411919) |
| 03 | 它做不到的事分三類，最危險的那類你看不見 | [llm-limitations](../../02-advanced/llm-limitations.md) ＋ [實測](../llm-limitations-field-test.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10412787) |
| 04 | 你其實只用了 AI 的兩種能力：一張全景圖看完它會什麼 | [ai-capability-landscape](../../02-advanced/ai-capability-landscape.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10413054) |
| 05 | AI 怎麼知道該用哪種能力 | [how-ai-picks-capability](../../02-advanced/how-ai-picks-capability.md) | ✅ [已完成](./drafts/day05-how-it-picks.md) |
| 06 | Prompt Engineering：哪些技巧真的有效，哪些只是傳說 | [tips-and-best-practices](../../01-fundamentals/tips-and-best-practices.md)＋新研究 | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10414480) |
| 07 | Context Engineering：prompt 只是它看到的 5% | [context-engineering](../../02-advanced/context-engineering.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10415067) |
| 08 | Harness Engineering：你已經在用只是不知道 | [harness-engineering](../../02-advanced/harness-engineering.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10415590) |

### 轉·提問：問得準（Day 9–12）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 09 | Loop Engineering：你不再是提示 AI 的那個人 | [loop-engineering](../../02-advanced/loop-engineering.md) | ✅ [已完成](./drafts/day09-loop-engineering.md) |
| 10 | XY Problem：你問的問題，通常不是你真正的問題 | [meta-prompting](../meta-prompting.md) | 改寫 |
| 11 | 用 prompt 生 prompt：一個可以直接複製的 MVP 模板 | [meta-prompting](../meta-prompting.md) | 改寫 |
| 12 | 讓 prompt 自己檢查自己：把驗證寫進提示裡 | [meta-prompting](../meta-prompting.md) | 改寫 |

### 轉·脈絡：走回頭路補上跳過的兩級（Day 13–14）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 13 | Context Engineering：餵什麼，比怎麼問更重要 | [context-engineering](../../02-advanced/context-engineering.md) | 補實測 |
| 14 | Harness Engineering：模型動不了，但外面那層可以 | [harness-engineering](../../02-advanced/harness-engineering.md) | 補實測 |

### 轉·目標與成本：讓它自己跑（Day 15–19）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 15 | 選模型與省錢：同一件事，成本可以差十倍 | [model-cost-comparison](../../01-fundamentals/model-cost-comparison.md)、[subagent 計費](../../02-advanced/subagent-usage-and-billing.md) | 改寫 |
| 16 | 權限：你願意讓 AI 動到哪裡？五種模式與一條紅線 | [permissions](../../01-fundamentals/claude-code/permissions.md) | 改寫 |
| 17 | `/goal`：給它一個能驗證的終點，它才知道什麼時候該停 | [goal](../../01-fundamentals/claude-code/goal.md) | 改寫 |
| 18 | 光有目標還不夠：用 Hook 逼它別中途放棄（含一個我實測失敗的 Hook） | [goal-enforcement-hooks](../../01-fundamentals/claude-code/goal-enforcement-hooks.md) | 改寫 |
| 19 | Workflow × Goal：讓它自己排隊、自己交差 | [workflow-goal-combo](../../01-fundamentals/claude-code/workflow-goal-combo.md) | 改寫 |

### 轉·驗證與擴展：驗得出、想得遠（Day 20–24）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 20 | 人要怎麼驗證 AI？跟數學借六種驗算法 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 21 | 獨立驗算為什麼最強：別讓它改自己的考卷 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 22 | 實測：Claude Code 在 HTB 靶機上，目標是怎麼被綁架的 | [htb/](../htb/htb-abducted-goal-case.md) 三案例 | 改寫 |
| 23 | 人做不到想像之外的事：用 AI 拓展視野的五個手段 | [ai-verify-then-expand](../ai-verify-then-expand.md) | 改寫 |
| 24 | 知識金字塔：AI 打掉哪一層壁壘，為什麼專家反而賺更多 | [ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) | 改寫 |

### 合：已經在發生的事，怎麼接（Day 25–30）

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 25 | AI 可能會取代什麼，目前不會取代什麼 | Stanford Canaries ＋ Anthropic Economic Index ＋ [ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) | ✅ [已完成](./drafts/day25-what-ai-replaces.md) |
| 26 | 這段字是 AI 寫的嗎？浮水印怎麼運作、為什麼不能當證據 | [ai-content-watermark](../../01-fundamentals/ai-content-watermark.md) | 改寫 |
| 27 | 自架本地 LLM：什麼時候該把 AI 搬回自己機器上 | [ollama-guide](../../04-local-llm/ollama-guide.md)、[vllm](../../04-local-llm/vllm.md)、[pii-masking](../../03-tools/pii-masking.md) | ✅ [已完成](./drafts/day27-self-hosted-llm.md) |
| 28 | 拿掉「拒絕」的真正代價：無審查模型實測 | [qwen3-6-27b-uncensored](../../04-local-llm/qwen3-6-27b-uncensored.md) | 改寫 |
| 29 | AI 已經會自己打靶了：自主滲透工具的現況 | [autonomous-pentest-tools-comparison](../../03-tools/security/autonomous-pentest-tools-comparison.md) | 改寫 |
| 30 | 三十天蒸餾：如果只能留下幾條心法 | **方向：跟著 AI 持續成長**（jason3e7 指定） | 新寫 |

**盤點：已完成 6 篇（含 4 篇已發布）、改寫 20 篇、補實測 2 篇、新寫 2 篇。** 分段為 **承 8 / 轉 16 / 合 6**。

---

## 候補題目池 — Backlog

> [!NOTE]
> Day 20 以後的排法**還沒定案**。後段預計會放比較多**實驗與實作**，結尾（Day 30）再把它們收整成心法。想到題目就先丟進這張表，要用的時候再去佔一個 Day。

| 題目 | 來源／素材 | 備註 |
|:---|:---|:---|
| 怎麼把 AI 的能力，變成自己的能力 | 待補 | **jason3e7 提（2026-09-18）**。跟 Day 30「跟著 AI 持續成長」是同一條線上的，可能是它的前一棒 |
| 把對話紀錄變成筆記本：匯出、自動分類、收整成可長期用的東西 | [curate-notes](../../skills/curate-notes.md)、[refactor-note](../../skills/refactor-note.md)、[note-lifecycle（待啟用）](../../skills/tmp/note-lifecycle.md) | **jason3e7 提（2026-09-18）**。這個 repo 本身就是這條流程的產物，可以拿實際的 `.jsonl` 與 skill 當案例。要注意匯出檔會夾帶路徑與環境資訊，公開前得先清 |
| 我親手測了一次 ClickFix：AI 分享頁怎麼被拿來騙人 | 06 的 ClickFix 筆記（自己的截圖） | 原 Day 29，被新 Day 05 擠出編號。發文時要沿用防禦式寫法 |

---

## 存稿排程 — Drafting Schedule

09/15 開賽、10/14 完賽。開賽前要有 12–15 篇成品。

| 期間 | 天數 | 目標 | 內容 |
|---|---:|---|---|
| 08/31 – 09/07 | 8 天 | 寫完 Day 01–11 | 承段 7 篇 ＋ 提問段 4 篇（素材最完整，先清掉） |
| 09/08 – 09/14 | 7 天 | 寫完 Day 12–21 | 脈絡／目標／驗證段，**補實測的幾篇排在這裡**，需要實際跑一輪並截圖 |
| 09/15 – 10/14 | 30 天 | 每天發 1 篇 + 寫 1 篇 | 庫存維持 10 篇以上；Day 28–30 留到最後寫，可以回收賽期中的讀者回饋 |

> [!WARNING]
> Day 29 的 ClickFix 那篇涉及惡意網址與 payload，發到公開站台時要**沿用 repo 既有的做法**：文字用防禦式寫法（`hxxp://` 與 `[.]`）、截圖只遮中段保留前綴。原始未遮蔽的圖不要上傳。

---

## Sources

- [2026 iThome 鐵人賽 — 競賽主題與活動說明](https://ithelp.ithome.com.tw/2026ironman/event)
- [2026 iThome 鐵人賽 — Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai)
