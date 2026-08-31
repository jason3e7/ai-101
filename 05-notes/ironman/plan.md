---
title: AI 101 - 2026 iThome 鐵人賽參賽規劃：AI 心法三十篇
tags: [ai, 鐵人賽, ironman, 寫作, 規劃, 個人筆記]
created: 2026-08-31
---

# 2026 iThome 鐵人賽參賽規劃 — Planning a 30-Day "AI 心法" Series

[← 回主頁](../../index.md)

> [!NOTE]
> jason3e7 想用這個知識庫去參加 2026 iThome 鐵人賽，題目《AI 心法》，結構是「**承先啟後**」——前面基礎與概念，中間心法，最後未來與應對。這篇記錄查證後的賽制事實、其他選手的實際狀況、以及三十篇的排法。

> **TL;DR (EN):** Sign up now, pick 2026-09-15 as the start date, and bank 12–15 drafts during the two weeks before. The "承先啟後" arc is sound — structure is an explicit judging criterion — but the history-first opening needs to be broken up, every mindset piece needs a real experiment attached, and the closing section should describe what to do now rather than predict the future. Recommended group: Claude AI (27 entrants) over AI Engineering (59, and someone is already writing the same four-layer framework).

---

## 賽制重點 — The Rules That Matter

先確認規則，其他討論才有意義。以下全部出自官方簡章。

| 項目 | 規定 | 對我們的意義 |
|---|---|---|
| 報名 + 開賽 | 08/01 – **09/15 23:59** | 今天是 08/31，還有 15 天 |
| 開賽日 | 自選，**選定後不可改** | 選 09/15，用剩下兩週存稿 |
| 連續性 | 每日一篇、連續 30 天，斷一天即失敗 | 存稿是唯一保險 |
| 篇幅 | 每篇 **>300 中文字**（含標點） | 現有筆記多為 600–2500 字，門檻不是問題 |
| 引用 | **不得逾全文 1/3** | `06-external/` 的整理文不能直接投稿 |
| 修改 | 只能當日改，評審看當日快照 | 上稿前就要定稿 |
| 賽後刪文 | **取消完賽資格、追回獎金** | 三十篇要留在站上 |
| 版權 | 著作權歸自己，但**博碩文化取得 5 年優先出版權** | repo 目前沒有 LICENSE，先知道這件事 |

**評審四個要點**：主題切題 / **結構（三十篇組織良好、能引導讀者理解）** / 內容專業深入 / 表達清楚。

> [!IMPORTANT]
> 「結構」是明文評分項目。這代表 jason3e7 想做的**承先啟後本身就是加分項**——不是自我要求而已，是直接對到評分表。

**開賽日建議 09/15**：晚開賽不影響評審（評審不看開賽早晚），但多出來的每一天都能拿來存稿。目標是開賽前手上有 12–15 篇成品。

---

## 對手在寫什麼 — What Others Are Writing

抓了官網選手列表 386 筆報名資料，分組人數如下：

| 組別 | 人數 | 組別 | 人數 |
|---|---:|---|---:|
| Software Development | 61 | Modern Web | 20 |
| **AI Engineering** | **59** | Security | 18 |
| 自我挑戰 | 33 | ChatGPT & Codex | 17 |
| **Claude AI** | **27** | JavaScript | 16 |
| Build on Google AI | 24 | IT Operation / AI Security | 各 14 |
| Vibe Coding / AI 自動化 | 各 22 | 佛心分享各組 | 1–12 |

**最大的發現：幾乎全部是實作型系列。** 標題清一色「30 天打造 X」「從零到一做 Y」。純方法論、講心法的系列是個位數——**差異化是真的存在的**。

但有兩個系列跟這個 repo 直接撞題，要正面處理：

| 撞題系列 | 組別 | 撞到哪 |
|---|---|---|
| 「模型動不了，那你能動什麼？AI Engineering 四層工程觀：Prompt、Context、Harness、Loop」 | AI Engineering | [Context](../../02-advanced/context-engineering.md)、[Harness](../../02-advanced/harness-engineering.md)、[Loop](../../02-advanced/loop-engineering.md) 三篇，骨架幾乎一樣 |
| 「Claude 用得對，也用得省：工程師帶你搞懂選模型、Token 優化與底層邏輯」 | Claude AI | [模型成本比較](../../01-fundamentals/model-cost-comparison.md)、[Subagent 計費](../../02-advanced/subagent-usage-and-billing.md) |

方向接近但不算撞的還有：「AI 時代的軟體開發—人如何與 AI 更有效協作」、「觀察 AI，也觀察自己：30 天重新學會如何學習」、「中文系 MIS 的 Claude 協作嘗試」。

### 組別選擇 — Which Group

| 選項 | 好 | 壞 |
|---|---|---|
| **Claude AI（建議）** | 27 人、冠軍同樣 1 名；repo 約六成是 Claude Code；HTB 實測、goal hook、權限設定全部原生切題 | 抽象的幾篇（能力全景圖、DIKW、驗證方法論）要刻意繫回 Claude 才算切題 |
| AI Engineering | 心法定位不突兀 | 59 人、硬工程重災區（RAG／MLOps／推論最佳化），且**已有人在寫同一套四層架構** |
| 自我挑戰、佛心分享 | 主題完全自由 | **不評審、不頒獎**，只有完賽證明 |

**主軸建議定成：「以 Claude Code 當實驗場，萃取跨模型通用的心法」。** 這樣抽象篇切題，實測篇有料。

---

## 結構決策 — Structure Decisions

jason3e7 的原案是「基礎與歷史 → 心法 → 未來」。方向對，但有三處要調整。

### 一、不要開專章講歷史

「AI 發展史」是全網最廉價的內容，容易被讀成灌水；而且 Day 1–5 決定讀者要不要追下去。

> 改法：**把「承先」拆散。** 每篇心法開頭兩三句交代它的來歷（Guilford 1956 的收斂／發散思考、Bloom 認知層次、2022 年的 ReAct、Ackoff 1989 的 DIKW 金字塔）。這本來就是這個 repo 的寫法，直接沿用。基礎壓到 4–6 篇，Day 1 放「全系列地圖 + 一個吸睛的實測結果」。

### 二、每篇心法都要綁一個實測

純觀念文對上「我做了一個會賺錢的 SaaS」，在「內容專業性、豐富性、深入性」這項會吃虧。

> 改法：**心法是骨，實測是肉。** 最強的武器是 [HTB 目標綁架三案例](../htb/htb-abducted-goal-case.md)——Claude Code 在真實靶機上目標被綁架的完整記錄，全場沒有第二個人有，而且是 repo 裡最長的三篇（4573／2911／2638 中文字）。

### 三、結尾不要預測未來

預測會變成空話，也無法驗證。

> 改法：寫**「已經在發生、現在該怎麼接」**——[AI 內容標記與辨識](../../01-fundamentals/ai-content-watermark.md)、隱私遮蔽、agent 已經會自己打靶、ClickFix 被濫用、[知識中產的處境](../ai-and-knowledge-barriers.md)。最後用 [Loop Engineering](../../02-advanced/loop-engineering.md) 那節「是真突破，還是新瓶裝舊酒」收尾——誠實比預言值錢。

---

## 三十篇骨架 — The 30-Article Outline

承 6 篇、轉 16 篇、合 8 篇。

| 段 | Day | 主題 | 現成素材 |
|---|---|---|---|
| **承** | 1 | 三十天要證明什麼：全系列地圖 + 一個實測結果 | 新寫 |
| | 2 | AI 能力全景圖：收斂／發散 × 認知層次 | [ai-capability-landscape](../../02-advanced/ai-capability-landscape.md) |
| | 3–4 | 四種能力的實戰配方（摘要／解釋／發想／重構），最省 token 與最有成效 | [four-capabilities-playbook](../../02-advanced/four-capabilities-playbook.md) |
| | 5 | 模型怎麼選、token 怎麼省 | [model-cost-comparison](../../01-fundamentals/model-cost-comparison.md)、[subagent 計費](../../02-advanced/subagent-usage-and-billing.md) |
| | 6 | 權限與邊界：你願意讓它動到哪 | [permissions](../../01-fundamentals/claude-code/permissions.md) |
| **轉·提問** | 7–10 | 好 prompt 的特性 → XY Problem → MVP 元提示模板 → 讓 prompt 自我驗證 | [meta-prompting](../meta-prompting.md)（2079 字，可拆四篇） |
| **轉·脈絡** | 11–13 | Context → Harness → Loop 三棒接力 | 三篇現成，但要加自己的實測以避開撞題 |
| **轉·目標** | 14–16 | `/goal` → 強制力 Hook → workflow × goal 組合技 | [goal](../../01-fundamentals/claude-code/goal.md)、[goal-enforcement-hooks](../../01-fundamentals/claude-code/goal-enforcement-hooks.md)、[workflow-goal-combo](../../01-fundamentals/claude-code/workflow-goal-combo.md) |
| **轉·驗證** | 17–19 | 數學式六種驗證法 → 獨立驗算為什麼最強 → **HTB 目標綁架實測** | [ai-verify-then-expand](../ai-verify-then-expand.md)、[htb/](../htb/htb-goal-prompt-guide.md) |
| **轉·擴展** | 20–22 | 突破想像的邊界 → 知識壁壘與 DIKW → 專家 +45%／新手 +20% 的放大器效應 | [ai-verify-then-expand](../ai-verify-then-expand.md)、[ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) |
| **合** | 23–26 | AI 內容標記與辨識 → 隱私遮蔽 → agent 自主滲透 → ClickFix 被濫用實測 | [watermark](../../01-fundamentals/ai-content-watermark.md)、[pii-masking](../../03-tools/pii-masking.md)、[自主滲透工具比較](../../03-tools/security/autonomous-pentest-tools-comparison.md)、06 的 ClickFix 筆記 |
| | 27–28 | 什麼時候該離線跑 → 知識中產被擠壓的處境 | [04-local-llm](../../04-local-llm/ollama-guide.md)、[ai-and-knowledge-barriers](../ai-and-knowledge-barriers.md) |
| | 29 | 是真突破，還是新瓶裝舊酒：對熱詞的誠實評估 | [loop-engineering](../../02-advanced/loop-engineering.md) |
| | 30 | 三十天蒸餾出來的幾條心法 | 新寫 |

素材盤點：01–05 共約 30 篇原創筆記，多數 600–2500 中文字，可直接改寫成參賽文章。

---

## 執行與地雷 — Execution & Pitfalls

> [!WARNING]
> **`06-external/` 的 60+ 篇不能直接投稿。** 那些是別人內容的整理，引用比例會超過全文 1/3，也踩到「發文內容以自創為限」。可以當佐證引用，不能當文章主體。

**其他要注意的：**

- **標題太抽象。**「AI 心法」三個字搜不到、也讀不出承諾。保留「心法」當骨幹，加上具體的東西，例如《AI 心法三十帖：用 Claude Code 當實驗場，從提問、驗證到突破自己的想像》。
- **repo 沒有 LICENSE。** 參賽等於授予博碩文化 5 年優先出版權，內容同時公開在 GitHub 上——不衝突，但要先知道。
- **存稿節奏。** 08/31–09/14 每天寫 1 篇，開賽時手上 12–15 篇；開賽後維持每天寫 1 篇，庫存始終不見底。
- **每篇的固定骨架**：一句話結論 → 來歷（兩三句） → 心法 → **我自己的實測** → 你可以怎麼做。有實測的篇章優先排在讀者還在觀望的前十天。

---

## Sources

- [2026 iThome 鐵人賽 — 競賽主題與活動說明](https://ithelp.ithome.com.tw/2026ironman/event)
- [2026 iThome 鐵人賽活動簡章（PDF）](https://r.itho.me/ironguide2026)
- [2026 iThome 鐵人賽 — 選手列表](https://ithelp.ithome.com.tw/2026ironman/signup/list)
- [2026 iThome 鐵人賽 — Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai)
- [2026 iThome 鐵人賽 — AI Engineering 組](https://ithelp.ithome.com.tw/2026ironman/ai-engineering)
