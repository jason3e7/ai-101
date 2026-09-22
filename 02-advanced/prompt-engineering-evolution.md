---
title: AI 101 - 從 Prompt 到 Loop：四個時代的技術演進
tags: [ai, prompt-engineering, context-engineering, harness, loop, 時間軸, 歷史, 進階]
created: 2026-09-23
---

# 從 Prompt 到 Loop：四個時代 — Prompt to Loop: Four Eras

[← 回主頁](../index.md)

> [!NOTE]
> 這幾年冒出一堆「XX Engineering」：Prompt、Context、Harness、Loop。它們不是彼此取代，也不是誰包含誰那麼乾淨。這篇用一條時間軸把它們串起來，並點出真正貫穿全部的那條主線 - **你控制模型的「單位」，一直在往外退。**

> **TL;DR (EN):** Four labels appeared in sequence - prompt (2022–23), context (2024–25), harness (2025–26), loop (2026) - but they are not a clean nesting. The real through-line is the *unit of control* zooming out: one message → one turn's full input → the machine that runs turns → the whole autonomous run. Each era did not replace the last; it wrapped it. The skills all still matter.

---

## 一條主線：控制單位一直往外退 — The Through-Line

先給你一句能記住的話，剩下的時間軸都是它的註腳：

> **每一個時代，你 engineering 的「單位」都往外退了一格。**

| 時代 | 你在雕的單位 | 你的身分 |
|:---|:---|:---|
| **Prompt** | 一句話 | 下指令的人 |
| **Context** | 一輪的完整輸入 | 佈置資訊的人 |
| **Harness** | 那台跑每一輪的機器 | 配置系統的人 |
| **Loop** | 整個自動流程 | 設好目標、走人的人 |

> [!IMPORTANT]
> 注意這**不是**乾淨的俄羅斯娃娃。prompt ⊂ context ⊂ harness 是成立的（前者是後者的一部分），但 **loop 其實是 harness 裡面的一個零件**（就是那個「編排迴圈」），被單獨拉出來命名，是因為「讓它自己跑」是一種完全不同的心態，不是多包一層。**串起四者的是「站多遠」，不是「包幾層」。**

---

## 時間軸 — The Timeline

### 第一時代：Prompt（2020–2023） - 把話說對

一切從 **GPT-3（2020-05）** 開始：它展示了 in-context learning - 不用重訓練，光靠 prompt 裡的幾個範例（few-shot）就能學會新任務。從此「怎麼問」本身變成一門手藝。

接著兩年，關鍵招式一個接一個冒出來：

| 時間 | 招式 | 幹嘛的 |
|:---|:---|:---|
| 2022-01 | **Chain-of-Thought**（Wei et al.） | 「一步一步想」讓多步推理變可能 |
| 2022-03 | **Self-Consistency**（Wang et al.） | 同題跑多次、取多數決，壓低隨機錯誤 |
| 2022-05 | **Zero-shot CoT**（Kojima et al.） | 一句「Let's think step by step」就有效 |
| 2022-10 | **ReAct**（Yao et al.） | Thought → Action → Observation，讓 prompt 第一次變成**迴圈** |
| 2022-11 | **ChatGPT** | 把這一切推到所有人面前 |
| 2023-05 | **Tree of Thoughts** | 讓它同時展開多條思路再挑 |

> [!TIP]
> **ReAct 是這一段的樞紐。** 在它之前，一個 prompt 是「送出去一次就結束」的字串；在它之後，prompt 變成「想 → 做 → 看結果 → 再想」的循環。後面三個時代，全都是在把這個循環一層一層工程化。

### 第二時代：Context（2024–2025） - 餵對東西比問得好更重要

當大家發現「模型答不好，常常不是問法爛，是它根本沒看到該看的東西」，重點就從那一句話，移到**它這一輪看到的全部**。

| 時間 | 事件 | 意義 |
|:---|:---|:---|
| 2024-11-25 | **Anthropic 發布 MCP**（Model Context Protocol） | 一套標準，讓模型能接上外部資料與工具 - 把「餵 context」變成基礎設施 |
| 2025-06-19 | **Tobi Lütke（Shopify CEO）發推** | 說他更愛「context engineering」這個詞：「把任務能被解出來所需的全部脈絡都準備好」 |
| 2025-06-25 | **Karpathy 接力** | 給了更利的定義：「為下一步，把 context window 填進剛剛好的資訊」，這個詞就此爆紅 |

這一跳其實最大：prompt 只是模型看到的約 5%，另外 95%（system prompt、對話歷史、工具定義、RAG 結果、記憶）都是 context。

### 第三時代：Harness（2025–2026） - 誰在跑那個迴圈

Context 不會自己出現在模型面前。有一整套系統負責：這一輪塞什麼、呼叫模型、解析並執行工具、判斷該不該停、出錯怎麼辦。這套「包裹 LLM 的執行環境」叫 harness。

| 時間 | 事件 | 意義 |
|:---|:---|:---|
| 2022-10 | **LangChain** 釋出 | 最早把「串工具、串步驟」框架化 |
| 2023-03 | **BabyAGI（03-28）／AutoGPT（03-30）** | 第一批公開的端到端自主 agent，驚艷但脆弱 |
| 2023-06-13 | **OpenAI Function Calling** | 讓工具呼叫變得結構化、可靠、可量產 |
| 2025-02-24 | **Claude Code**（研究預覽） | 成熟 harness 進到一般開發者手上；2025-05-22 正式版 |

到這一代，2023 年還要自己手寫的 retry、壓縮、工具循環、沙箱隔離，全都內建了。這是「一般人也能跑 agent」變成事實的轉捩點。

### 第四時代：Loop（2026–） - 你退出每一輪

當 harness 夠穩，可以「設好就走人」時，重點最後一次外移：從「陪它跑每一輪」，退到「設計那個會自己跑的迴圈」。

| 時間 | 事件 | 意義 |
|:---|:---|:---|
| 2026-06 | **Loop Engineering** 一詞浮現 | 由 Peter Steinberger、Boris Cherny、Addy Osmani 各自提出，Osmani 命名成文 |

Claude Code 之父 Boris Cherny 的說法最傳神：

> 「我已經不 prompt Claude 了。我讓一堆迴圈自己跑、自己去 prompt Claude、自己決定要做什麼。**我的工作是寫迴圈。**」

---

## 一個提醒：後浪沒有淘汰前浪 — It Stacks, Not Replaces

每次新詞出來，都有人喊「prompt engineering 已死」。這是誤讀。

**四個時代是疊加，不是取代。** 你設計 loop 的時候，裡面每一輪還是要有好的 context，context 裡那一句指令還是要用到 prompt 的功夫。往外退一格，不代表裡面那格就不用管了 - 只是不再是你**唯一**在管的東西。

> [!WARNING]
> 真正變的不是「哪個技巧還有用」，而是**槓桿的位置**。2023 年，改一句 prompt 能救一個結果；2026 年，同樣的力氣花在「把目標寫成能自動驗證的樣子」或「設對停止規則」上，回報大得多。技巧沒過期，**但最值得投資的那一格一直在往外移。**

---

## 常見問題 — FAQ

**Q：所以這是四種不同的技術？**
不是四種技術，是**同一件事的四種尺度**。底層機制沒變（模型還是在猜下一個字），變的是你介入的位置：一句話、一輪、一台機器、一整個流程。

**Q：為什麼每一代都隔沒多久就換？**
因為每一代都在「補上上一代的手工活」。ReAct 讓循環成立、MCP 讓餵 context 標準化、Claude Code 讓 harness 內建、loop 讓整套能無人值守。每補完一層，重點就自然往外移一格。

**Q：新手該從哪一代學起？**
從第一代。往外退的每一格都建立在裡面那格之上 - 不會寫好 prompt，設出來的 loop 每一輪都在產垃圾。**由內而外學，別跳。**

---

## 相關筆記 — Related

- [AI 怎麼知道該用哪種能力](./how-ai-picks-capability.md) - Prompt 那一代最核心的機制：它從你的話裡推論任務
- [Context Engineering](./context-engineering.md) - 第二時代的主題，餵什麼比怎麼問更重要
- [Harness Engineering](./harness-engineering.md) - 第三時代：模型動不了，但外面那層可以
- [Loop Engineering](./loop-engineering.md) - 第四時代：設計會自己 prompt agent 的迴圈
- [六種能力執行手冊](./capabilities-playbook.md) - 第一、二代的實戰打法

## Sources

- [Prompt engineering — Wikipedia（技術與時間線總覽）](https://en.wikipedia.org/wiki/Prompt_engineering)
- [Chain-of-Thought Prompting Elicits Reasoning in LLMs — Wei et al., 2022](https://arxiv.org/abs/2201.11903)
- [ReAct: Synergizing Reasoning and Acting in Language Models — Yao et al., 2022](https://arxiv.org/abs/2210.03629)
- [Introducing the Model Context Protocol — Anthropic, 2024-11-25](https://www.anthropic.com/news/model-context-protocol)
- [Shopify CEO and ex-OpenAI researcher agree that context engineering beats prompt engineering — The Decoder, 2025](https://the-decoder.com/shopify-ceo-and-ex-openai-researcher-agree-that-context-engineering-beats-prompt-engineering/)
- [Context engineering — Simon Willison, 2025-06-27](https://simonwillison.net/2025/Jun/27/context-engineering/)
- [Claude Code Timeline: Release Date and Major Updates — ScriptByAI](https://www.scriptbyai.com/claude-code-timeline/)
- [The Anthropic leader who built Claude Code says he ditched prompting — now he just writes loops — The New Stack, 2026](https://thenewstack.io/loop-engineering/)
- [Loop Engineering — Addy Osmani](https://addyosmani.com/blog/loop-engineering/)
