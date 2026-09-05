---
title: AI 101 - 鐵人賽 Day 03：它做不到什麼
tags: [ai, 鐵人賽, ironman, llm, 極限, 限制, jagged-frontier, 草稿]
created: 2026-09-03
status: draft
---

# Day 03｜它做不到什麼：三種極限，和最危險的那一種

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!IMPORTANT]
> 「做不到」有三種，混在一起談就是多數人踩坑的原因：**結構性的**（原理決定，短期改不掉）、**暫時的**（下一代就補上了）、**鋸齒狀的**（你根本看不見邊界在哪）。把暫時的當永久，你會低估它；把鋸齒狀的當結構性，你會被它坑。

> **TL;DR (EN):** Three kinds of limits, and they call for three different responses. Structural ones follow from next-token prediction and won't scale away: it cannot reliably catch its own errors, it doesn't generalise "A is B" into "B is A", it never learns from your conversation, and it doesn't actually compute. Temporary ones — context rot, collapse past a complexity threshold — shrink with each generation. The dangerous one is the jagged frontier: the boundary is uneven and invisible, and the model sounds exactly as confident on both sides of it.

---

## 三種極限，要分開看 — Three Kinds of Limits

[Day 01](./day01-llm-is-statistics.md) 講它在做什麼，[Day 02](./day02-why-it-got-strong.md) 講它為什麼變強。今天講反面：**它做不到什麼。**

但「做不到」不是一件事，是三件性質完全不同的事。分不清楚，你的判斷就會一直錯邊。

---

## 結構性的：原理決定的天花板 — Structural Limits

這幾件事，是「猜下一個字」這個原理直接推出來的。模型變大不會讓它們消失。

### 它不會自己發現自己錯了

ICLR 2024 的《Large Language Models Cannot Self-Correct Reasoning Yet》測了一件很單純的事：**不給任何外部回饋**，只叫模型自己再檢查一遍答案。結果是——不但沒變好，**有時候還變差**。

原因不難理解：「檢查」跟「作答」用的是同一套機率分布。它第一次覺得對的東西，第二次還是會覺得對。

**自己試：**

1. 叫它寫一個處理邊界情況的函式。
2. 問「這段有沒有 bug？再檢查一次」——它多半回你「看起來沒問題」。
3. 改成把 `pytest` 的失敗訊息貼回去——它立刻就改對了。

差別不在它有沒有再看一次，而在**你有沒有給它一個外部的檢查結果**。

> [!TIP]
> 這條直接決定了 Day 20 的主題：**驗證必須來自外部**——測試、編譯器、另一個獨立的模型，或者你自己。叫它「再檢查一次」，多數時候只是讓它把同一個錯誤講得更有信心。

### 學過的東西，反過來問就不會

《The Reversal Curse》（Berglund et al., NeurIPS 2023）發現：模型學過「A 是 B」，**不會自動學會「B 是 A」**。研究者拿 1000 位名人測 GPT-4——問「湯姆克魯斯的媽媽是誰」答得出來；反過來問「Mary Lee Pfeiffer 的兒子是誰」就答不出來，而且正確答案的機率並不比隨便一個名字高。

**自己試：** 挑一組**不太有名**的事實（越冷門越明顯）：

| 問法 | 結果 |
|---|---|
| 「《某本冷門書》的作者是誰？」 | 答得出來 |
| 「（那位作者）寫過哪些書？」 | 常常漏掉那一本 |

實務上更常遇到的版本：你剛請它把「開啟 A 設定會導致 B 錯誤」寫進文件；過一陣子問「B 錯誤是什麼造成的」，它未必接得回 A。

對人來說這是同一件事，對模型不是。它學的是「這個字後面接哪個字」，方向是單向的。

→ 心法：**換個方向問同一件事，是最便宜的驗證手段之一。**

### 它不會從你的對話裡學會

權重是凍結的。你這次糾正它，換一個新對話它照樣犯同樣的錯。它看起來記得，只是因為東西還留在當下的脈絡裡。

**自己試：** 這次對話跟它說「不要用表情符號」，它照做。關掉、開一個新對話再問同樣的問題——表情符號就回來了。同一句話寫進 `CLAUDE.md`，才會每一輪重新注入。

→ 這就是為什麼需要 `CLAUDE.md`、memory、RAG——不是為了讓它變聰明，是因為**它不會學**。

### 精確計算不是它的工作

《Faith and Fate: Limits of Transformers on Compositionality》（Dziri et al., NeurIPS 2023）把多位數乘法拆開來看，發現模型不是在執行演算法，而是在做**子圖比對**——把看過的模式拼起來。所以位數一多就崩。

**自己試：**

| 問法 | 結果 |
|---|---|
| 「4823 × 7591 等於多少？」 | 常錯，位數再加長幾乎必錯 |
| 「寫一段 Python 算 4823 × 7591 並執行」 | 對 |

→ 心法：**能交給程式算的，就別讓它心算。** 叫它「寫一段程式算給我看」，比叫它直接報答案可靠得多。

---

## 暫時的：會被下一代抹平 — Temporary Limits

這一類別當成永久的判斷，不然你會一直低估它。

**一、上下文「用不好」，不是「放不下」。**
Liu et al. 2023 的《Lost in the Middle》發現：脈絡塞得越滿，模型越偏好開頭和結尾，**中間的東西會掉**。Chroma 在 2025 年的《Context Rot》測了 18 個前沿模型，**每一個**都隨輸入變長而退步。

但要分清楚：這是「利用率」的問題，不是「容量」的問題。窗口越開越大，利用率也一直在改善。

**自己試：** 把同一份長文件貼三次，把你要問的那一行分別放在**開頭 / 正中間 / 結尾**，問同一個問題。中間那一次的失敗率明顯最高。

→ 心法：**位置有價。** 最重要的資訊放頭尾，別指望它在幾十萬 token 的正中間撈到那一行。這是 Day 12 Context Engineering 的核心。

**二、複雜度一過線就崩。**
Apple 在 2025 年的《The Illusion of Thinking》測了 o1、o3-mini、Claude 3.7 Sonnet Thinking、DeepSeek-R1，找到三個區間：簡單任務上，**不思考的模型反而贏**；中等複雜度，會推理的模型佔優；高複雜度，兩種都掉到接近零。他們還觀察到「過度思考」——模型找到正確答案後不停下來，繼續生出錯的路徑。

不過這篇被打了回馬槍。《The Illusion of the Illusion of Thinking》指出部分崩潰其實是題目設計與輸出長度上限造成的，不是推理本身的極限。後續複現的結論折衷：**批評的細節有問題，但核心站得住——它的「思考」只在中間區段有效，而且不太會類推。**

**自己試：** 拿河內塔（Tower of Hanoi）當尺。3 層、5 層它處理得很漂亮；層數一路往上加，到某個點會**突然整個崩掉**——而且它不會事先警告你「這題我不行」。

→ 心法：**兩邊都別信滿。** 不要因為一篇論文說「AI 不會推理」就放棄，也不要因為它某次表現驚人就以為沒有天花板。唯一可信的，是**你自己在你自己的任務上實測出來的邊界**。

---

## 鋸齒狀的：最危險的那一種 — The Jagged Frontier

前面兩類至少你知道邊界大概在哪。這一類的麻煩是：**邊界不規則，而且看不見。**

Harvard 與 BCG 在 2023 年做了一個 758 人的實驗（Dell'Acqua et al.，約占 BCG 顧問的 7%），提出了「**鋸齒狀前沿**（jagged frontier）」這個詞：AI 的能力邊界不是一條平滑的線，而是鋸齒狀的——**兩個在你看來難度差不多的任務，一個它做得漂亮，另一個它做得離譜。**

數字很有說服力：

| 位置 | 結果 |
|---|---|
| 邊界**之內**的 18 個顧問任務 | 用 GPT-4 的人多完成 **12.2%** 的任務、快 **25.1%**、品質高 **40%** 以上 |
| 邊界**之外**（AI 會錯但看起來合理） | **新手掉了 19 個百分點**（照單全收）；能識破瑕疵的專家反而勝出 |

最危險的地方在於：**它不會告訴你現在踩在鋸齒的哪一邊。** 語氣一樣自信、格式一樣漂亮、讀起來一樣合理。

> **真正的風險不是「它做不到」，而是「它做不到，但看起來做到了」。**

這一條撐起了後面兩整段的內容：因為邊界看不見，所以要養成驗證習慣（Day 19–21）；因為新手掉最多，所以**用 AI 的人自己有多專業，決定了他能拿到多少收益**（Day 22–23）。

---

## 整理 — Recap

| 極限 | 類型 | 長什麼樣 | 你該做的 |
|---|---|---|---|
| 不會自己發現自己錯 | 結構性 | 「再檢查一次」→ 同一個錯講得更有信心 | 驗證一定要來自外部 |
| 反過來問就不會 | 結構性 | 答得出作者，答不出他寫過哪些書 | 換方向問，當成廉價的驗證 |
| 不會從對話中學會 | 結構性 | 新對話又用回你禁止過的寫法 | 用 `CLAUDE.md`、memory、RAG 補 |
| 精確計算不可靠 | 結構性 | `4823 × 7591` 直接算會錯 | 叫它寫程式算，別叫它心算 |
| 上下文中段會掉 | 暫時 | 同一行放中間就撈不到 | 重要的放頭尾 |
| 複雜度過線就崩 | 暫時 | 河內塔加到某層數突然全錯 | 把任務拆小，並自己實測邊界 |
| 鋸齒狀前沿 | 看不見 | 兩題難度相仿，一題完美一題離譜 | 預設它隨時可能站在錯的那一邊 |

Day 01 說它是一台機率機器，Day 02 說這台機器跨過了實用門檻。今天要記住的是：

> **那道門檻是鋸齒狀的。你不會知道自己什麼時候正好踩在缺口上——除非你養成了驗的習慣。**

---

## Sources

- [Large Language Models Cannot Self-Correct Reasoning Yet — Huang et al., ICLR 2024](https://arxiv.org/abs/2310.01798)
- [The Reversal Curse: LLMs trained on "A is B" fail to learn "B is A" — Berglund et al., 2023](https://arxiv.org/abs/2309.12288)
- [Faith and Fate: Limits of Transformers on Compositionality — Dziri et al., 2023](https://arxiv.org/abs/2305.18654)
- [Lost in the Middle: How Language Models Use Long Contexts — Liu et al., 2023](https://arxiv.org/abs/2307.03172)
- [Context Rot: How Increasing Input Tokens Impacts LLM Performance — Chroma Research, 2025](https://research.trychroma.com/context-rot)
- [The Illusion of Thinking — Apple Machine Learning Research, 2025](https://machinelearning.apple.com/research/illusion-of-thinking)
- [The Illusion of the Illusion of Thinking（回應）— 2025](https://arxiv.org/abs/2507.01231)
- [Navigating the Jagged Technological Frontier — Dell'Acqua et al., HBS Working Paper 24-013](https://www.hbs.edu/faculty/Pages/item.aspx?num=64700)
