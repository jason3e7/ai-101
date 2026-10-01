---
title: AI 101 - 鐵人賽三十篇標題與素材對照
tags: [ai, 鐵人賽, ironman, 寫作, 規劃, 個人筆記]
created: 2026-08-31
---

# 三十篇標題與素材對照 — The 30 Titles

[← 回主頁](../../index.md)｜[參賽規劃](./plan.md)

> [!NOTE]
> 三十篇的**現況板**. 已發布 15 篇 (Day 1-15), draft 3 篇 (Day 16、25、29), 其餘待寫或待重排. 每次排定或變更, 順手記在下方 [變更歷程](#變更歷程--changelog).

> **TL;DR (EN):** Thirty working titles for the Claude AI group. Fifteen published as of 2026-09-29 (Day 1-15); the middle stretch (Day 17-24) is being re-planned because four originally-planned slots (Day 20/21/23/24) got pulled forward into Days 11/14/15, and Day 26 was vacated when Day 16 absorbed the same lab-scan topic.

---

## 報名定案 — Registered

**參賽題目**

```
AI 心法三十天：用 Claude Code 當實驗場，從提問、驗證到拓展自己的想像
```

**題目簡介**（修訂版，約 165 字）

> [!NOTE]
> 這是 `[fixButNotPublish]` 修訂後的版本，**iThome 上仍是報名當下的原句**。原句與修改原因見 [fix-log.md](./logs/fix-log.md)。

```
從「LLM 只是在猜下一個字」這個原理出發，推導出什麼時候該驗證、為何給對脈絡比問對問題更重要，以及哪些 prompt 技巧其實缺乏證據。每篇以 Claude Code 當實驗場，指令與設定照講，但重點在「為什麼這樣做」，並附上可以自己動手試一次的例子與我自己踩過的坑。最後收在 AI 會取代什麼、不會取代什麼，以及人怎麼跟著它繼續成長。
```

| 項目 | 值 |
|---|---|
| 組別 | **Claude AI** |
| 開賽日 | **2026-09-15**（選定後不可異動） |
| 完賽日 | 2026-10-14 |

---

## 三十篇標題 — The Titles

**狀態符號**:

- 🚀 **已發布** — 上到 iThome, 連結指向文章
- ✅ **已完成** — draft 已寫, 未上稿
- 🚧 **草稿中** — 有 draft, 未定稿
- 📅 **預計** — 題目與素材已決定, 未動筆
- 🔄 **待重排** — 位置或內容未定 (候選見 [Day 17-24 候選](#day-17-24-候選--candidates-for-re-slotting))
- 未標 — 尚未動工

### 承: 基礎與定位 (Day 1-8)

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 01 | 它只是在猜下一個字：LLM 的原理，決定了後面 29 天的所有心法 | [core-concepts](../../01-fundamentals/core-concepts.md) + 新研究 | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10411345) |
| 02 | 不是它突然變強，是它跨過了你的門檻 | 新研究（METR、scaling laws、MCP 採用曲線） | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10411919) |
| 03 | 它做不到的事分三類，最危險的那類你看不見 | [llm-limitations](../../02-advanced/limits-and-verification/llm-limitations.md) ＋ [實測](../experiments/llm-limitations-field-test.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10412787) |
| 04 | 你其實只用了 AI 的兩種能力：一張全景圖看完它會什麼 | [ai-capability-landscape](../../02-advanced/capabilities/ai-capability-landscape.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10413054) |
| 05 | AI 怎麼知道該用哪種能力 | [how-ai-picks-capability](../../02-advanced/capabilities/how-ai-picks-capability.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10413765) |
| 06 | Prompt Engineering：哪些技巧真的有效，哪些只是傳說 | [tips-and-best-practices](../../01-fundamentals/tips-and-best-practices.md)＋新研究 | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10414480) |
| 07 | Context Engineering：prompt 只是它看到的 5% | [context-engineering](../../02-advanced/engineering/context-engineering.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10415067) |
| 08 | Harness Engineering：你已經在用只是不知道 | [harness-engineering](../../02-advanced/engineering/harness-engineering.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10415590) |

### 轉·工程與判斷力 (Day 9-15)

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 09 | Loop Engineering：你不再是提示 AI 的那個人 | [loop-engineering](../../02-advanced/engineering/loop-engineering.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10416150) |
| 10 | xxx Engineering: 名字會變, 智慧是自己的 | [prompt-engineering-evolution](../../02-advanced/engineering/prompt-engineering-evolution.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10416410) |
| 11 | AI 給的答案, 你怎麼知道是對的 | [ai-verify-then-expand](../essays/ai-verify-then-expand.md) ＋ [verifying-ai-output](../../02-advanced/limits-and-verification/verifying-ai-output.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10417119) |
| 12 | 為了方便人類驗證而生的兩個 skill：condense / expand-mindmap | [mindmap-skills-design](../design-and-guides/mindmap-skills-design.md) ＋ [skills/](../../skills/) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10417425) |
| 13 | AI 用久了會鈍化: 兩種鈍, 五招破 | [ai-atrophy](../../02-advanced/limits-and-verification/ai-atrophy.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10417978) |
| 14 | AI 抹平的是中產, 頂層反而變貴 | [ai-and-knowledge-barriers](../essays/ai-and-knowledge-barriers.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10418241) |
| 15 | 用 AI 拓展自己: 5 個手段主動破舒適圈 | [ai-verify-then-expand](../essays/ai-verify-then-expand.md) 線二 | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10418980) |

### 轉·實測與量化 (Day 16-24)

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 16 | 掃當屆所有文章, 量一次「AI 味」有多少 | [lab01](../lab01/README.md) | 🚀 [已發布](https://ithelp.ithome.com.tw/articles/10419414) |
| 17 | 從 1 個訊號擴到 6 個, 綜合分數再測「AI 味」 | [lab01 結果](../lab01/results-v06b.md) ＋ [兩算法對照](../lab01/v06-vs-v06b.md) | 🚧 [草稿中](./drafts/day16-20/day17-composite-score.md) |
| 18 | 翻面問題: 哪些文章是純手寫, 完全沒碰 AI 的? | [lab01](../lab01/README.md) ＋ jason3e7 翻面提問 | 🚧 [草稿骨架](./drafts/day16-20/day18-detecting-pure-human.md) |
| 19 | 這段字是 AI 寫的嗎? 浮水印怎麼運作、為什麼不能當證據 | [ai-content-watermark](../../01-fundamentals/ai-content-watermark.md) | 🚧 [草稿中](./drafts/day16-20/day19-ai-watermark.md) |
| 20 | AI 文風入侵: 五個記號一次講完 + 把味道壓回去的抗體 + 子系列小結 (Day 16-19) | [ai-writing-style-tells](../../02-advanced/writing-style/ai-writing-style-tells.md) ＋ [jason3e7 手筆改寫版](../../02-advanced/writing-style/ai-writing-style-tells-jason3e7-voice.md) ＋ [pgplay-writeup-style-guide](../design-and-guides/pgplay-writeup-style-guide.md) ＋ 子系列回顧 | 📅 預計 |
| 21 | (待重排) | 見 [Day 21-24 候選](#day-17-24-候選--candidates-for-re-slotting) | 🔄 |
| 22 | (待重排) |  | 🔄 |
| 23 | (待重排) |  | 🔄 |
| 24 | (待重排) |  | 🔄 |

### 合: 已經在發生的事，怎麼接 (Day 25-30)

| Day | 標題 | 素材 | 狀態 |
|:---|:---|:---|:---|
| 25 | AI 可能會取代什麼, 目前不會取代什麼 | Stanford Canaries ＋ Anthropic Economic Index ＋ [ai-and-knowledge-barriers](../essays/ai-and-knowledge-barriers.md) | ✅ [已完成](./drafts/day21-25/day25-what-ai-replaces.md) |
| 26 | (待想) | 原「AI 風格橫行掃 40 系列」跟 Day 16 lab01 重疊, 已撤空 | 🔄 |
| 27 | (待想, 原「AI 文風入侵」已前移到 Day 20) | — | 🔄 |
| 28 | (待想, 原「浮水印」已前移到 Day 19) | — | 🔄 |
| 29 | 自架本地 LLM：什麼時候該把 AI 搬回自己機器上 | [ollama-guide](../../04-local-llm/ollama-guide.md)、[vllm](../../04-local-llm/vllm.md)、[pii-masking](../../03-tools/privacy/pii-masking.md) | ✅ [已完成](./drafts/day26-30/day27-self-hosted-llm.md) |
| 30 | 三十天蒸餾：如果只能留下幾條心法 | **方向：跟著 AI 持續成長**（jason3e7 指定） | 新寫 |

**盤點**: 🚀 已發布 16 篇 (Day 1-16) · ✅ 已完成 draft 2 篇 (Day 25、29) · 🚧 草稿中 3 篇 (Day 17、18、19) · 📅 預計 1 篇 (Day 20) · 🔄 待重排/待想 7 篇 (Day 21-24、26、27、28) · 未動工 1 篇 (Day 30).

---

## 變更歷程 — Changelog

- **2026-10-01 (下午)**: Day 20 合併原 Day 27「AI 文風入侵: 五個記號 + 把味道壓回去的抗體」+「子系列小結 (Day 16-19)」, 兩個主題相關就一次講; Day 27 slot 空出變 🔄 待想; Day 19 draft 寫完 (🚧 草稿中)
- **2026-10-01**: Day 16 已發布 (10419414); Day 17 lab01 第二篇「V06B 綜合分數」draft 寫完; Day 18-20 排定「文風測量 + 浮水印」子系列 (Day 18 lab01 收尾, Day 19 浮水印 (ai-content-watermark), Day 20 子系列小結); Day 28 原「浮水印」題目前移到 Day 19, 原 slot 待想
- **2026-09-29**: Day 16 從「權限：AI 動到哪裡」pivot 到「lab01 雙破折號掃描」; Day 17-24 全部 mark 🔄 待重排 (原內容中 Day 20/21/23/24 已被 Day 11/14/15 用掉); Day 26 撤空 (原「AI 風格橫行掃 40 系列」跟 Day 16 lab01 重疊); 移除已無效的「Day 11-20 主題重定」與「選模型實測預產」兩節
- **2026-09-24**: Day 11-20 主軸重定成「驗證 → 判斷力」主題; 廢棄原 Day 11「用 prompt 生 prompt」與 Day 10「XY Problem」(已被 xxx Engineering 收整)
- **2026-09-22**: Day 26-27 插入「AI 風格三部曲」前兩篇 (風格橫行 → 文風入侵), 原 Day 28-29「無審查模型」「自主滲透工具」回候補池
- **2026-09-18**: Day 05 後加插「how it picks」, 原 Day 05 起全部順延, 被擠出的「ClickFix 實測」回候補池

### Day 17-24 候選 — Candidates for Re-slotting

八格全開, 從下面挑或另想. 上面數字排序不代表優先度.

| 候選 | 素材 | 動作 |
|:---|:---|:---|
| 權限：AI 動到哪裡, 五種模式與紅線 | [permissions](../../01-fundamentals/claude-code/permissions.md) | 改寫 |
| `/goal`: 給它能驗證的終點 | [goal](../../01-fundamentals/claude-code/goal.md) | 改寫 |
| 光有目標不夠: 用 Hook 逼它別放棄 | [goal-enforcement-hooks](../../01-fundamentals/claude-code/goal-enforcement-hooks.md) | 改寫 |
| Workflow × Goal: 讓它自己排隊、自己交差 | [workflow-goal-combo](../../01-fundamentals/claude-code/workflow-goal-combo.md) | 改寫 |
| Claude Code 四層行為系統: goal + sub-agent + skill + hook | [behavior-design](../../01-fundamentals/claude-code/behavior-design.md) | 改寫 (tentative) |
| Claude Code 在 HTB 靶機上, 目標怎麼被綁架 | [htb/](../htb/htb-abducted-goal-case.md) 三案例 | 改寫 |
| 選模型 × 實測: 同一題 Opus/Sonnet/Haiku 各跑一次 | [model-cost-comparison](../../01-fundamentals/models/model-cost-comparison.md) + 新實測 | 高 (要跑) |
| 驗證疲勞: 什麼時候該關掉驗證 | 待研究 | 觀念 |

---

## 候補題目池 — Backlog

> [!NOTE]
> 想到題目就先丟進來, 要用的時候再去佔一個 Day.

| 題目 | 來源／素材 | 備註 |
|:---|:---|:---|
| 怎麼把 AI 的能力，變成自己的能力 | 待補 | jason3e7 提 (2026-09-18); 跟 Day 30「跟著 AI 持續成長」是同一條線, 可能是它的前一棒 |
| 把對話紀錄變成筆記本: 匯出、自動分類、收整成可長期用的東西 | [curate-notes](../../skills/curate-notes.md)、[refactor-note](../../skills/refactor-note.md)、[note-lifecycle（待啟用）](../../skills/tmp/note-lifecycle.md) | jason3e7 提 (2026-09-18); 本 repo 就是實例. 匯出檔會夾帶路徑與環境資訊, 公開前得先清 |
| 讓 prompt 自己檢查自己: 把驗證寫進提示裡 | [meta-prompting](../essays/meta-prompting.md) | 原 Day 12, 已被 mindmap skills 取代 (2026-09-26) |
| 親手測 ClickFix: AI 分享頁怎麼被拿來騙人 | 06 的 ClickFix 筆記 (自己的截圖) | 原 Day 29 被新 Day 05 擠出. 發文時要沿用防禦式寫法 |
| 拿掉「拒絕」的真正代價: 無審查模型實測 | [qwen3-6-27b-uncensored](../../04-local-llm/qwen3-6-27b-uncensored.md) | 原 Day 28, 被 AI 風格三部曲擠出 (2026-09-22) |
| AI 已經會自己打靶: 自主滲透工具現況 | [autonomous-pentest-tools-comparison](../../03-tools/security/autonomous-pentest-tools-comparison.md) | 原 Day 29, 被 AI 風格三部曲擠出 (2026-09-22) |

---

## 存稿排程 — Drafting Schedule (歷史)

09/15 開賽、10/14 完賽, 每天發 1 篇. 開賽前存稿 12-15 篇成品的原始排程 (**已過**, 保留作為 retrospective):

| 期間 | 天數 | 目標 |
|:---|---:|:---|
| 08/31 – 09/07 | 8 天 | 寫完 Day 01–11 |
| 09/08 – 09/14 | 7 天 | 寫完 Day 12–21 |
| 09/15 – 10/14 | 30 天 | 每天發 1 篇 + 寫 1 篇 |

> [!WARNING]
> ClickFix 相關題材涉及惡意網址與 payload, 發到公開站台時要沿用 repo 既有做法: 文字用防禦式寫法 (`hxxp://` 與 `[.]`)、截圖只遮中段保留前綴. 原始未遮蔽的圖不要上傳.

---

## Sources

- [2026 iThome 鐵人賽 — 競賽主題與活動說明](https://ithelp.ithome.com.tw/2026ironman/event)
- [2026 iThome 鐵人賽 — Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai)
