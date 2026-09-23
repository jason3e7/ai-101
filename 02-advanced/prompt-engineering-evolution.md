---
title: AI 101 - 從 Prompt 到 Graph：五個時代的技術演進
tags: [ai, prompt-engineering, context-engineering, harness, loop, graph, 時間軸, 歷史, 進階]
created: 2026-09-23
updated: 2026-09-23
---

# 從 Prompt 到 Graph：五個時代 — Prompt to Graph: Five Eras

[← 回主頁](../index.md)

> [!NOTE]
> 這幾年冒出一堆「XX Engineering」：Prompt、Context、Harness、Loop、Graph。它們不是彼此取代，也不是誰包含誰那麼乾淨。這篇用一條時間軸把它們串起來，並點出真正貫穿全部的那條主線 - **你控制模型的「單位」，一直在往外退。**

> **TL;DR (EN):** The mainstream spine is now five labels in sequence - prompt (2022–23), context (2024–25), harness (2025–26), loop (2026), graph (2026, frontier) - a *unit of control* zooming out: one message → one turn's input → the machine that runs turns → one autonomous run → a directed graph of many runs. It is not a clean nesting and the ordering has no industry consensus yet. Beyond the spine: Flow (AlphaCodium, 2024) is a parallel lineage, and Memory / RAG / Evals / Spec cut across every era. Each era wraps the last rather than replacing it; the skills all still matter.

---

## 一條主線：控制單位一直往外退 — The Through-Line

先給你一句能記住的話，剩下的時間軸都是它的註腳：

> **每一個時代，你 engineering 的「單位」都往外退了一格。**

| 時代 | 你在雕的單位 | 你的身分 |
|:---|:---|:---|
| **Prompt** | 一句話 | 下指令的人 |
| **Context** | 一輪的完整輸入 | 佈置資訊的人 |
| **Harness** | 那台跑每一輪的機器 | 配置系統的人 |
| **Loop** | 一個自動流程 | 設好目標、走人的人 |
| **Graph** | 多個流程的協作地圖 | 畫流程圖、定路徑的人 |

> [!IMPORTANT]
> 注意這**不是**乾淨的俄羅斯娃娃。prompt ⊂ context ⊂ harness 是成立的（前者是後者的一部分），但 **loop 其實是 harness 裡面的一個零件**（就是那個「編排迴圈」），graph 則是「把很多個 loop 排成一張圖」。它們被一個個單獨拉出來命名，是因為每一次「站得更遠」都是一種不同的心態，不是多包一層。**串起五者的是「站多遠」，不是「包幾層」。**

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
> **ReAct 是這一段的樞紐。** 在它之前，一個 prompt 是「送出去一次就結束」的字串；在它之後，prompt 變成「想 → 做 → 看結果 → 再想」的循環。後面幾個時代，全都是在把這個循環一層一層工程化。

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

### 第四時代：Loop（2026） - 你退出每一輪

當 harness 夠穩，可以「設好就走人」時，重點外移：從「陪它跑每一輪」，退到「設計那個會自己跑的迴圈」。

| 時間 | 事件 | 意義 |
|:---|:---|:---|
| 2026-06 | **Loop Engineering** 一詞浮現 | 由 Peter Steinberger、Boris Cherny、Addy Osmani 各自提出，Osmani 命名成文 |

Claude Code 之父 Boris Cherny 的說法最傳神：

> 「我已經不 prompt Claude 了。我讓一堆迴圈自己跑、自己去 prompt Claude、自己決定要做什麼。**我的工作是寫迴圈。**」

### 第五時代：Graph（2026，前沿） - 把很多個迴圈排成一張圖

一個 loop 顧一件事。當你要**同時指揮一群 agent 把整個專案做完**，就需要先畫一張圖：哪些是獨立的工作步驟、步驟之間怎麼交棒、進度與成本怎麼記錄。這就是 Graph Engineering。

它跟 loop 的差別很精準：

> **在 loop 裡，你設好目標，agent 自己挑路；在 graph 裡，你宣告哪些路徑合法、每條路上有哪些檢查。**

換句話說 loop 是「放手讓它跑」，graph 是「把跑法本身畫成一張受控的地圖」，坐在 loop 再外面一格。Karpathy 的 autoresearch 專案就是這個方向的示範。

> [!WARNING]
> **Graph 跟 Loop 幾乎同時在 2026 冒出來，順序還沒有業界共識。** 有人把 graph 當 loop 的外一層，也有人把它當 loop 的另一種形狀（宣告式 vs 放手式）。這一格是現在進行式，講法還會變 - 別把它背成鐵律。

---

## 不只這條主軸 — Beyond the Spine

那五格是**最主流的那條主軸**（好幾篇文章正好用同一條線在講，見 Sources）。但如果你去翻，會發現還有別的名字，分兩種：

**一、平行的另一條血脈：Flow Engineering**

早在 2024-01，CodiumAI 的 AlphaCodium 論文標題就叫〈From Prompt Engineering to Flow Engineering〉。它主張把「一問一答」換成**多階段、測試驅動、反覆修正的流程** - GPT-4 在程式競賽題上的 pass@5 從 19% 拉到 44%。

Flow 其實是 harness／loop 那套「別一次問完，讓它跑一個結構化流程」的**早期、程式領域的祖型**。它沒有長成主流詞，但方向完全一致，值得知道它比 loop 早了兩年。

**二、橫跨所有時代的「切面」，不是某一階段**

還有幾個常聽到的，它們不是主軸上的某一格，而是**每一格裡都要處理**的橫向能力：

| 名字 | 在做什麼 | 為什麼是切面 |
|:---|:---|:---|
| **Retrieval / RAG** | 把外部知識檢索進來餵給模型 | context 時代的主力，但 harness／loop 裡照樣要用 |
| **Memory Engineering** | 設計 agent 怎麼記事實、經驗、跨 session 狀態 | 是 context 與 harness 的共同組成 |
| **Evals** | 用標準化題組量化「到底有沒有變好」 | 每一個時代都需要它來驗收 |
| **Spec-driven** | 把需求寫成精確規格，當成生成的依據 | 貫穿 prompt 到 graph 的輸入端 |

> [!NOTE]
> 這些橫向能力現在常被一個更大的傘狀詞收攏：**Agent Engineering** - 有人把它定義為「涵蓋 prompt、context、harness、推論、記憶、評估、程式碼」的整合學科。也就是說，主軸五格 ＋ 這些切面，合起來才是完整的「做 agent」。

> [!WARNING]
> **別被名字的數量嚇到。** 這些詞多半是同一個轉變（重點從模型內部移到模型周圍）在不同角度、不同社群、不同時間點被各自命名的結果，連業界都還沒對順序有共識。抓住那條主軸（控制單位往外退），其他的都掛得上去 - 有些是往外的延伸（Graph），有些是早期的別名（Flow），有些是每一格都要做的橫向功夫（Memory、RAG、Evals、Spec）。

---

## 一個提醒：後浪沒有淘汰前浪 — It Stacks, Not Replaces

每次新詞出來，都有人喊「prompt engineering 已死」。這是誤讀。

**五個時代是疊加，不是取代。** 你畫一張 graph，裡面每個節點是一個 loop，loop 每一輪還是要有好的 context，context 裡那一句指令還是要用到 prompt 的功夫。往外退一格，不代表裡面那格就不用管了 - 只是不再是你**唯一**在管的東西。Databricks 的說法很傳神：prompt 和 context engineering「住在」harness engineering 裡面。

> [!WARNING]
> 真正變的不是「哪個技巧還有用」，而是**槓桿的位置**。2023 年，改一句 prompt 能救一個結果；2026 年，同樣的力氣花在「把目標寫成能自動驗證的樣子」或「設對停止規則」上，回報大得多。技巧沒過期，**但最值得投資的那一格一直在往外移。**

---

## 常見問題 — FAQ

**Q：所以這是五種不同的技術？**
不是五種技術，是**同一件事的五種尺度**。底層機制沒變（模型還是在猜下一個字），變的是你介入的位置：一句話、一輪、一台機器、一個流程、一張協作圖。

**Q：為什麼每一代都隔沒多久就換？**
因為每一代都在「補上上一代的手工活」。ReAct 讓循環成立、MCP 讓餵 context 標準化、Claude Code 讓 harness 內建、loop 讓整套能無人值守、graph 讓多個 loop 能協調。每補完一層，重點就自然往外移一格。

**Q：Graph 是不是已經定案的「第五代」？**
還沒。它和 loop 幾乎同時在 2026 出現，業界對「graph 到底是 loop 的外一層、還是另一種形狀」都還沒共識。把它當「現在進行式的前沿」比當「蓋棺論定的一代」更準。

**Q：新手該從哪一代學起？**
從第一代。往外退的每一格都建立在裡面那格之上 - 不會寫好 prompt，畫出來的 graph 每個節點都在產垃圾。**由內而外學，別跳。**

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
- [從 prompt 到 graph engineering：AI agent 的五層工程 — 數位時代](https://www.bnext.com.tw/article/91632/graph-engineering-ai-agent)
- [Prompt, Context, Harness & Loop Engineering — Avi Chawla, Daily Dose of DS](https://blog.dailydoseofds.com/p/prompt-context-harness-and-loop-engineering)
- [Loop, Harness, Context Engineering: The Terms Explained — codecentric](https://www.codecentric.de/en/knowledge-hub/blog/loop-harness-context-engineering-explained)
- [Harness, Graph, and Loop Engineering — How to Evolve From Prompts and Context — Sarthak AI](https://sarthakai.substack.com/p/harness-graph-and-loop-engineering)
- [Code Generation with AlphaCodium: From Prompt Engineering to Flow Engineering — Ridnik et al., 2024](https://arxiv.org/abs/2401.08500)
- [Harness Engineering for Agentic AI Coding Tools: An Exploratory Study — 2026](https://arxiv.org/abs/2602.14690)
