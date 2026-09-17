---
title: AI 101 - LLM 的極限：三類做不到，和最危險的那一類
tags: [ai, llm, 極限, 限制, jagged-frontier, 校準, 驗證, 進階]
created: 2026-09-17
---

# LLM 的極限 — What LLMs Cannot Do

[← 回主頁](../index.md)

> [!IMPORTANT]
> 「做不到」不是一件事，是三類性質完全不同的事：**結構性的**（原理決定，短期改不掉）、**暫時的**（下一代就補上了）、**鋸齒狀的**（你根本看不見邊界在哪）。把暫時的當永久，你會低估它；把鋸齒狀的當結構性，你會被它坑。

> **TL;DR (EN):** Structural limits follow from next-token prediction and won't scale away — it cannot reliably catch its own errors, doesn't generalise "A is B" into "B is A", never learns from your conversation, and doesn't actually compute. Temporary limits (context rot, collapse past a complexity threshold) shrink each generation. The dangerous one is the jagged frontier, because the tool you normally use to predict capability — analogy from similar tasks — is exactly the tool that fails, its confidence signal runs backwards, and when challenged it argues back rather than backing down.

---

## 結構性的：原理決定的天花板 — Structural Limits

這幾件事，是「猜下一個字」這個原理直接推出來的。模型變大不會讓它們消失。

### 它不會自己發現自己錯了

ICLR 2024 的《Large Language Models Cannot Self-Correct Reasoning Yet》測了一件很單純的事：不給任何外部回饋，只叫模型自己再檢查一遍答案。結果是 - 不但沒變好，有時候還變差。

原因不難理解：「檢查」跟「作答」用的是同一套機率分布。它第一次覺得對的東西，第二次還是會覺得對。

**自己試：** 叫它寫一個處理邊界情況的函式，問「這段有沒有 bug？再檢查一次」 - 它多半回你「看起來沒問題」；改成把 `pytest` 的失敗訊息貼回去 - 它立刻就改對了。

> [!TIP]
> **驗證必須來自外部** - 測試、編譯器、另一個獨立的模型，或者你自己。叫它「再檢查一次」，多數時候只是讓它把同一個錯誤講得更有信心。

### 學過的東西，反過來問就不會

《The Reversal Curse》（Berglund et al., NeurIPS 2023）發現：模型學過「A 是 B」，不會自動學會「B 是 A」。研究者拿 1000 位名人測 GPT-4 - 問「湯姆克魯斯的媽媽是誰」答得出來；反過來問「Mary Lee Pfeiffer 的兒子是誰」就答不出來。而且不是「一時想不起來」那種答不出來 - 它給「湯姆克魯斯」這個答案的機率，跟隨便講一個名字一樣低。

對人來說這是同一件事，對模型不是。它學的是「這個字後面接哪個字」，方向是單向的。

→ **換個方向問同一件事，是最便宜的驗證手段之一。**

### 它不會從你的對話裡學會

權重是凍結的。你這次糾正它，換一個新對話它照樣犯同樣的錯。它看起來記得，只是因為東西還留在當下的脈絡裡。

→ 這就是為什麼需要 `CLAUDE.md`、memory、RAG - 不是為了讓它變聰明，是因為它不會學。

### 精確計算不是它的工作

《Faith and Fate》（Dziri et al., NeurIPS 2023）把多位數乘法拆開來看，發現模型不是在執行演算法，而是在做子圖比對 - 把看過的模式拼起來。所以位數一多就崩。

→ **能交給程式算的，就別讓它心算。** 叫它「寫一段程式算給我看」，比叫它直接報答案可靠得多。

---

## 暫時的：會被下一代抹平 — Temporary Limits

這一類別當成永久的判斷，不然你會一直低估它。

**一、上下文「用不好」，不是「放不下」。**
Liu et al. 2023 的《Lost in the Middle》發現：脈絡塞得越滿，模型越偏好開頭和結尾，中間的東西會掉。Chroma 在 2025 年的《Context Rot》測了 18 個前沿模型，每一個都隨輸入變長而退步。但要分清楚：這是「利用率」的問題，不是「容量」的問題。

→ **位置有價。** 最重要的資訊放頭尾。詳見 [Context Engineering](./context-engineering.md)。

**二、複雜度一過線就崩。**
Apple 在 2025 年的《The Illusion of Thinking》找到三個區間：簡單任務上，不思考的模型反而贏；中等複雜度，會推理的模型佔優；高複雜度，兩種都掉到接近零。不過這篇被打了回馬槍 -《The Illusion of the Illusion of Thinking》指出部分崩潰其實是題目設計與輸出長度上限造成的。後續複現的結論折衷：批評的細節有問題，但核心站得住。

→ **與其挑邊站，不如自己測一次。** 拿河內塔當尺：步數應該是 2ⁿ−1，層數往上加，某個點之後會突然全錯，而且它不會事先警告你。

---

## 鋸齒狀的：最危險的那一類 — The Jagged Frontier

前面兩類至少你知道邊界大概在哪。這一類的麻煩是：**邊界不規則，而且看不見。**

### 支撐這個說法的研究

這不是單一研究的比喻，而是一整批研究在講同一件事：

| 研究 | 內容 |
|---|---|
| **Dell'Acqua et al.** | 原始的 758 人 BCG 實驗。已經從 working paper 升級成期刊論文，2025 年發表在 *Organization Science* - 不再只是預印本 |
| **Mollick 的後續四個實驗** | 包含 P&G 的 **776 人**實驗，跨產業、跨團隊複製，結論成立 |
| **Morris et al.（Stanford）** | 《Characterizing Model Jaggedness Supports Safety and Usability》，專門在量測 jaggedness |
| **Gans 2026** | 《A Model of Artificial Jagged Intelligence》，把它變成一個經濟學模型 |
| **同儕審查場景實測** | AI 審稿能抓出人類漏掉的技術錯誤與方法瑕疵，卻在**詮釋性錯誤、敘事連貫、領域判斷**上遠不如人 - 典型的鋸齒 |
| **Karpathy** | 推廣了「jagged intelligence」一詞：*「同時是天才博學家，又是搞不清楚狀況的小學生。」* |

Salesforce 甚至把它寫成企業導入指南，Melanie Mitchell 在 Yale Review 專文討論 - **這個詞已經從一篇論文的比喻，變成產業共同語言。**

### 原始實驗的數字

| 位置 | 結果 |
|---|---|
| 邊界**之內**的 18 個顧問任務 | 多完成 12.2% 的任務、快 25.1%、品質高 40% 以上 |
| 邊界**之外**（AI 會錯但看起來合理） | **新手掉了 19 個百分點**；能識破瑕疵的專家反而勝出 |

### 為什麼看不見 —— 四個機制

**① 你唯一的預測工具壞了。**

> 對人來說，一項技能做得好，可以合理推論相近的技能也不差。在 AI 的鋸齒地景上，這條推論不成立。

這是核心。你判斷「這件事它應該做得來」靠的是**類推** - 而類推正是鋸齒前沿唯一會失效的地方。所以不是「你不小心看不見」，是**你用來看的那個工具本身失效了**。

**② 信心訊號是反的。**
校準研究發現 **hard-easy effect**：題目越難，它越有信心。更麻煩的是：

> 模型**產生確信語氣的時候，反而比較不準確**。

也就是說 - **你最需要警訊的時候，訊號指向反方向。** 而使用者又天然把「講得有信心」讀成「比較可靠」（《Humans overrely on overconfident language models》）。

**③ 它不是消極地錯，是主動說服你。**
Mollick 後續實驗中最驚人的發現，叫 **persuasion bombing**：

> 當專業人士去查證 GPT-4 在複雜任務上的產出時，它並不會承認限制，而是用**結構化論證**和**道歉式的修正**，把原本錯誤的建議講得更有說服力。

所以「你去查證」這道防線，本身會被攻擊。這比單純「它會錯」嚴重一個量級。

> [!CAUTION]
> 這條最值得記住：**「我會去查證」這道防線本身會被攻擊。** 所以驗證必須靠外部事實（測試、原始資料、獨立來源），不能靠跟它辯論。

**④ 進步是看不見的，而學會邊界很貴。**
Gans 的經濟模型點出兩件事：

- **檢查悖論（inspection paradox）**：使用者遇到錯誤的頻率，比整體統計顯示的高
- **規模的不透明**：模型變大，平均品質提升，但**鋸齒還在** - 所以「它進步了」這件事你感覺不到

他的結論很實用：**真正的精通，是學會一張「可靠度地圖」** - 知道它在哪裡可信。但畫這張地圖的成本，本身就是採用的最大障礙。

### 一個反面：鋸齒也是資產

有研究主張鋸齒不全是壞事 -《LLM Jaggedness Unlocks Scientific Creativity》認為，正因為它的能力分布跟人類不一樣，才會提出人類想不到的組合。

> [!NOTE]
> 同一個特性，**在收斂類任務是風險，在發散類任務是資產**。用 [AI 能力全景圖](./ai-capability-landscape.md) 的話說：越靠收斂端越要防鋸齒，越靠發散端越可以利用它。

---

## 整理 — Recap

| 極限 | 類型 | 長什麼樣 | 你該做的 |
|---|---|---|---|
| 不會自己發現自己錯 | 結構性 | 「再檢查一次」→ 同一個錯講得更有信心 | 驗證一定要來自外部 |
| 反過來問就不會 | 結構性 | 答得出作者，答不出他寫過哪些書 | 換方向問，當成廉價的驗證 |
| 不會從對話中學會 | 結構性 | 新對話又用回你禁止過的寫法 | 用 `CLAUDE.md`、memory、RAG 補 |
| 精確計算不可靠 | 結構性 | `4823 × 7591` 直接算會錯 | 叫它寫程式算，別叫它心算 |
| 上下文中段會掉 | 暫時 | 同一行放中間就撈不到 | 重要的放頭尾 |
| 複雜度過線就崩 | 暫時 | 河內塔加到某層數突然全錯 | 拆小，並自己實測邊界 |
| 鋸齒狀前沿 | 看不見 | 兩題難度相仿，一題完美一題離譜 | 為自己的工作畫可靠度地圖 |

> **真正的風險不是「它做不到」，而是「它做不到，但看起來做到了」 - 而且你去查證時，它還會反過來說服你。**

---

## 相關筆記 — Related

- [LLM 極限實測](../05-notes/llm-limitations-field-test.md) —— jason3e7 實跑這些檢測的結果：**兩個極限它都「答對」了，因為它偷偷換了工具**
- [LLM 極限實測（二）](../05-notes/llm-limitations-field-test-2.md) —— 零工具重測：精確計算四題全過，但出現新的失敗模式「**假裝自己有工具**」
- [LLM 極限實測（三）](../05-notes/llm-limitations-field-test-3.md) —— 換模型比較：Haiku 4.5 算錯 `17^13`，Sonnet 5 的河內塔邊界在 N=10
- [AI 能力全景圖](./ai-capability-landscape.md) —— 收斂／發散決定了幻覺是 bug 還是 feature
- [Context Engineering](./context-engineering.md) —— 「位置有價」的完整版
- [先驗證，再用它突破自己](../05-notes/ai-verify-then-expand.md) —— 外部驗證的六種方法
- [AI 打破知識壁壘](../05-notes/ai-and-knowledge-barriers.md) —— 鋸齒前沿為什麼放大了判斷力的價值

## Sources

- [Large Language Models Cannot Self-Correct Reasoning Yet — Huang et al., ICLR 2024](https://arxiv.org/abs/2310.01798)
- [The Reversal Curse — Berglund et al., 2023](https://arxiv.org/abs/2309.12288)
- [Faith and Fate: Limits of Transformers on Compositionality — Dziri et al., 2023](https://arxiv.org/abs/2305.18654)
- [Lost in the Middle — Liu et al., 2023](https://arxiv.org/abs/2307.03172)
- [Context Rot — Chroma Research, 2025](https://research.trychroma.com/context-rot)
- [The Illusion of Thinking — Apple, 2025](https://machinelearning.apple.com/research/illusion-of-thinking)
- [The Illusion of the Illusion of Thinking（回應）](https://arxiv.org/abs/2507.01231)
- [Navigating the Jagged Technological Frontier — Dell'Acqua et al.（HBS 工作論文）](https://www.hbs.edu/faculty/Pages/item.aspx?num=64700)
- [同上，期刊版 — Organization Science, 2025](https://pubsonline.informs.org/doi/10.1287/orsc.2025.21838)
- [Discovering AI's jagged frontier — and what we've learned since — Ethan Mollick](https://professorkl.substack.com/p/discovering-ais-jagged-frontier-and)
- [A Model of Artificial Jagged Intelligence — Gans, 2026](https://arxiv.org/abs/2601.07573)
- [Humans overrely on overconfident language models, across languages](https://arxiv.org/abs/2507.06306)
- [Jagged AI in Scientific Peer Review](https://arxiv.org/abs/2605.07855)
- [LLM Jaggedness Unlocks Scientific Creativity](https://arxiv.org/abs/2605.10574)
- [The Dangerous Unknowns of Jagged Intelligence — Melanie Mitchell, Yale Review](https://yalereview.org/article/melanie-mitchell-jagged-intelligence)
