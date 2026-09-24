---
title: "AI 101 - 鐵人賽 Day 10: xxx Engineering 收整, 名字會變, 智慧是自己的"
tags: [ai, 鐵人賽, ironman, xxx-engineering, 收整, 歷史, 心法, 草稿]
created: 2026-09-23
status: draft
---

# Day 10｜xxx Engineering 收整: 名字會變, 智慧是自己的

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 這 4 天 (Day 06 到 09) 我一次丟了四個 xxx Engineering 給你: Prompt / Context / Harness / Loop. 這種文章寫多了, 有個公認副作用: 讀者開始焦慮「怎麼又冒新典範了, 我上個都還沒學會」. 今天不加新詞, 把這四代攤成一條歷史時間軸, 然後回答那個焦慮.

> **TL;DR (EN):** Four sequential "XXX Engineering" labels have appeared in the past few years: Prompt (2020-2023), Context (2024-2025), Harness (2025-2026), Loop (2026). The through-line is that your unit of control keeps zooming out — one message → one turn's input → the runtime → one autonomous run. Each era doesn't replace the last, it wraps it and moves the leverage outward. New labels appear because each generation automates the last one's manual work (ReAct → MCP → Claude Code → loop). Don't panic when new terms drop: the underlying mechanic (LLM predicting next token) hasn't changed, and the durable skills (spotting fuzzy specs, external verification, systems decomposition, cross-mapping new tools) travel across all four. Techniques expire, judgment doesn't.

---

## 從一句話, 到一整條流程 — Four Eras at a Glance

過去五年, 你能對 LLM 出手的**位置**一直往外退. 一張表看完:

| 時代 | 何時浮現 | 你在雕的「單位」 | 你的身分 |
|:---|:---|:---|:---|
| **Prompt Engineering** | 2020–2023 | 一句話 | 下指令的人 |
| **Context Engineering** | 2024–2025 | 一輪的完整輸入 | 佈置資訊的人 |
| **Harness Engineering** | 2025–2026 | 那台跑每一輪的機器 | 配置系統的人 |
| **Loop Engineering** | 2026 | 一個自動流程 | 設好目標、走人的人 |

**一句話串起四代**:

> **每個時代, 你 engineering 的「單位」都往外退了一格.**

從 prompt 到 loop, 表面上是四個學科, 底下是**同一件事的四種尺度**. 這條主線一路貫穿; 更完整的時間軸 (包括 ReAct、MCP、Claude Code 這些節點, 以及 2026 後半才冒頭的 Graph Engineering) 在 [Prompt 到 Graph 進階筆記](../../../02-advanced/prompt-engineering-evolution.md).

---

## 為什麼一代接一代冒出來 — Why the Cascade

不是新技術突然發明, 是**每一代都在補上一代的手工活**:

| 什麼補了什麼 |
|:---|
| **ReAct (2022)** 讓 prompt 從一次性字串變成能循環, 開啟 agent 這條路 |
| **MCP (2024)** 讓「餵 context」有標準, 不用每個工具都自己接 |
| **Claude Code / Codex (2025-2026)** 把 retry、壓縮、tool 循環、sandbox 全部內建, harness 變預設 |
| **Loop (2026)** 因為 harness 夠穩了, 可以「設好目標、走人」而不必守著 |

**每次一個手工步驟被自動化, 你的注意力就自然被推到更外一格.** 這不是誰在強迫你追潮流, 是**科技把你往外推**.

也因為這樣, 前浪沒被後浪淘汰. Loop 裡每一輪還是要好的 context, context 裡那句指令還是要用到 prompt 的功夫. **四代是疊加, 不是取代.**

---

## 別焦慮: 名字會變, 底層沒變 — Don't Panic

**回到 [Day 01](./day01-llm-is-statistics.md).** LLM 到今天還是在做同一件事: 看完前面所有 token, 猜下一個. 這件事從 GPT-3 (2020) 到 Opus 4.7 (2026) 沒變過.

所以四代 xxx Engineering 之間變的是什麼? **是你介入的槓桿位置.**

- 2023 年: 改一句 prompt 能救一個結果
- 2024 年: 同樣的力氣花在「餵對 context」回報大得多
- 2026 年: 花在「把目標寫成能自動驗證的樣子」回報又大得多

**技巧沒過期, 只是最值得投資的那一格一直在往外移.** 你會焦慮, 通常是因為以為「新一代出來了, 舊的白學了」— 事實不是那樣. 舊的是新的地基, 蓋樓不能拆地基.

---

## 智慧是自己的 — What You Own

名詞會過期 (2028 年可能又出 Fleet Engineering 之類的). 但下面這些能力不會:

| 能力 | 做什麼 | 為什麼跨代通用 |
|:---|:---|:---|
| **分辨模糊 vs 明確** | 一眼看出「幫我改好」跟「所有測試通過」的差別 | 每一代都需要「怎樣算做完了」這一步變精 |
| **外部驗證思維** | 不信 AI 自報「做完了」, 一定要有 verifier | Prompt 時代靠人眼, Loop 時代靠測試, 邏輯一樣 |
| **拆解關鍵資訊** | 這件事情, 模型該看到的最小資訊集是什麼 | Context Engineering 的核心, 但每個時代都在做 |
| **類比新工具** | 看到新東西能秒問「它在補之前哪個手工活?」 | 就是這篇的整條 through-line 教你的能力 |
| **停止規則的直覺** | 判斷「什麼時候該停手不再讓 AI 跑」 | 從 prompt 時代的「別讓它繼續解釋」到 loop 時代的「circuit breaker」, 都是這個 |

**這些能力沒有版本號.** 你三年前為了 prompt 練出來的「怎麼問清楚」, 現在花在寫 goal 上, 效果直接遷移. 你為了 context 練出來的「什麼資訊要餵進去」, 現在拿去設計 subagent 的 handoff, 也直接遷移.

**技術會迭代, 這些判斷力不會.** 這就是「智慧是自己的」的意思.

---

## 活到老學到老 — Habits That Keep You Sane

給還在焦慮的自己四條: 

1. **新名詞出來, 先問「它在補之前哪個手工活?」** 找到這個問題的答案, 你就知道它值不值得學、跟舊東西怎麼銜接
2. **別急著追新典範**, 先確定舊那格會不會用. 由內而外學, 不會 prompt 就跳去寫 loop, 每一輪都在產垃圾
3. **承認你會落後一格.** 業界普遍狀態就是「上一代還在消化, 下一代已經在講」. 一格落差是常態, 兩三格才要擔心
4. **對照歷史軸自我盤點**: 我現在最常改的是哪一格 (prompt / context / harness / loop)? 這格的效能上限在哪? 下一格能不能挪一點注意力過去?

---

## Sources

- [從 Prompt 到 Graph: 五個時代的技術演進 (本 repo)](../../../02-advanced/prompt-engineering-evolution.md), 完整時間軸、Graph 前沿補充、五代熱度對照
- [The Anthropic leader who built Claude Code says he ditched prompting — The New Stack, 2026](https://thenewstack.io/loop-engineering/)
- [Prompt, Context, Harness & Loop Engineering — Avi Chawla, Daily Dose of DS](https://blog.dailydoseofds.com/p/prompt-context-harness-and-loop-engineering)
- [Graph Engineering是什麼？迴圈工程、Harness Engineering⋯5種AI工程術語差異一次看懂 — 陳建鈞, 數位時代, 2026-07](https://www.bnext.com.tw/article/91632/graph-engineering-ai-agent)
