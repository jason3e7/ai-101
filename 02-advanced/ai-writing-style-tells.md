---
title: AI 101 - AI 的文風與語氣：破折號、三段式與那些藏不住的習慣
tags: [ai, 文風, 語氣, em-dash, rlhf, 辨識, 進階]
created: 2026-09-19
---

# AI 的文風與語氣 — The Tells of AI Writing

[← 回主頁](../index.md)

> [!NOTE]
> 你大概也發現了：Claude Opus 5 寫中文特別愛用破折號（`——`）。這不是巧合，也不是它「喜歡」什麼 - 是訓練留下的痕跡。這篇拆解 AI 最常見的幾個文風習慣、它們**為什麼**會出現，以及一個對你更有用的問題：既然改不掉根源，怎麼在 prompt 端把它們壓下去。

> **TL;DR (EN):** AI writing has recurring tells - the em dash above all, plus tricolons, antithesis ("not just X, but Y"), bold-everywhere, and constant hedging. They are not quirks; they come from markdown-saturated pretraining and RLHF preferring "polished-looking" prose. A 2026 study measured it: Claude Opus 4.6 emits 9.09 em dashes per 1,000 words unconstrained but drops to 0.19 when told not to - highly suppressible, unlike GPT-4.1 which keeps 3.86 even under an explicit ban. Practical upshot: name the specific tic and give the replacement, don't just say "write naturally".

---

## 那個破折號，到底哪來的 — Where the Em Dash Comes From

先講你觀察到的那個。破折號（em dash，英文 `—`，中文全形 `——`）是所有 AI 文風習慣裡**最頑固、最好辨識**的一個。它頑固到有一篇 2026 年的研究專門在講它，標題就叫〈The Last Fingerprint〉（最後一個指紋）。

它的成因有兩層，兩層都不是「模型的選擇」：

**一、Markdown 洩漏進了散文。** 模型的訓練資料裡塞滿了 markdown - 標題、粗體、清單、破折號。這些結構標記在被要求「寫成純散文」時，大部分能被壓掉（標題、清單都會消失），**唯獨破折號壓不掉**。因為它同時是「合法的標點」又是「結構標記」 - 你沒辦法在禁止它當結構符號的同時，還允許它當正常標點。它是那套結構習慣**最小的、活下來的那個單位**。

**二、RLHF 放大了它。** 破折號在高品質的編輯過的文本（《紐約客》《大西洋月刊》那類）裡出現得特別多。模型從這些資料學到「好文章長這樣」，而 RLHF 階段的人類評分者 - 多半是習慣結構化寫作的技術背景 - 又傾向給這種「看起來工整」的文字更高分。一個正回饋迴圈就這樣把它放大了。

> [!IMPORTANT]
> 關鍵證據：**這件事不是 LLM 的必然，是特定訓練選擇的結果。** 同一篇研究測了十二個模型，破折號密度天差地遠 - 有些模型（如 Llama）幾乎是零。所以它不是「AI 都這樣」，而是「**某些訓練管線**這樣」。

---

## 一張表：常見的文風習慣 — The Tells

破折號只是最有名的一個。實務上會反覆看到的有這幾類：

| 習慣 | 長什麼樣 | 為什麼會出現 |
|:---|:---|:---|
| **破折號** | 「這不是巧合 —— 是訓練的痕跡」 | markdown 洩漏 ＋ RLHF 放大 |
| **三段式**（tricolon） | 幾乎每個列舉都剛好三項；「快、狠、準」 | 三拍在訓練資料裡讀起來最「完整」 |
| **對立句**（antithesis） | 「不只是 X，而是 Y」「重點不在 A，而在 B」 | 二元對立聽起來果斷、乾淨 |
| **粗體到處撒** | 一段裡標粗好幾個詞組 | 同樣是 markdown 結構習慣的殘留 |
| **無所不在的避險** | 「在某些情況下」「可能」「值得注意的是」 | 訓練目標獎勵「不要講死」，避免被判錯 |
| **虛設的防守** | 先講一個沒人反對的小論點，再煞有其事地辯護它 | 對齊訓練養出的「先自我辯護」傾向 |
| **過度熱情的開場** | 「這是一個很棒的問題！」 | RLHF 的討好傾向（sycophancy） |

> [!WARNING]
> **單獨一個都不算證據。** 破折號用了幾個世紀，三段式是正常的修辭手法。真正像 AI 的是**成群出現** - 三段式、對立句、反問句擠在同一段，而且整篇沒有一個具體的、會讓作者擔責任的判斷。單一特徵抓不了人，密度和缺乏擔當才是訊號。

---

## 為什麼「寫自然一點」沒有用 — Why "Write Naturally" Fails

理解了成因，就知道為什麼一句「寫自然一點」幾乎無效：**那些習慣是訓練烙進去的，不是它臨時決定的風格，你用一個同樣模糊的形容詞蓋不過一整套訓練。**

這跟 [AI 怎麼知道該用哪種能力](./how-ai-picks-capability.md)是同一個道理：**模糊的指令 = 模糊的結果。** 「自然」對模型來說是一個沒有座標的詞，它只會往「它以為的自然」收斂 - 而那恰好就是這些習慣。

好消息是，那篇研究也量出了一件很實用的事：**Claude 的破折號極度好壓。**

| 模型 | 沒限制時（每千字） | 明確要求不要用時 |
|:---|:---|:---|
| **Claude Opus 4.6** | 9.09 | **0.19**（幾乎歸零） |
| GPT-4.1 | 10.62 | 3.86（禁了還在用） |
| 人類基準 | 3.23（範圍 0.33–17.12） | — |

Claude 只要你**明確點名**，就幾乎完全照做；GPT 就算明令禁止還是壓不下來。這是 Claude 這條訓練管線的特性，對你是好消息 - **點名就有效。**

---

## 怎麼壓下去：點名 ＋ 給替代 — How to Suppress Them

原則一條：**指名那個具體的習慣，並給它一個替代做法。** 不要用「自然」「不要像 AI」這種模糊詞。

| 不要這樣說 | 改成這樣說 |
|:---|:---|
| 「寫自然一點」 | 「不要用破折號，需要停頓就用逗號或句號分成兩句」 |
| 「不要像 AI」 | 「不要每個列舉都湊成三項；該兩項就兩項，該五項就五項」 |
| 「口語一點」 | 「不要用『不只是 X，而是 Y』這種對立句型」 |
| 「不要太多廢話」 | 「拿掉『值得注意的是』『在某些情況下』這類避險詞，有把握就直接講」 |

> [!TIP]
> 最省事的做法：把這幾條寫進 `CLAUDE.md` 或你的自訂風格（style）設定，它每一輪都會重新套用，不必每次重講。這個 repo 自己就這樣做 - 專案規範裡明訂「引用名言用一般 `>`，破折號一律換成 ` - `」，所以你在這些筆記裡看到的是 ` - ` 不是 `——`。

### 一個要注意的副作用

壓文風習慣壓過頭，文章會變得**乾巴巴、句子長度一致、不敢下判斷** - 那也是一種「AI 味」，只是換了個方向。目標不是消滅所有結構，是**讓結構服務內容**，而不是內容去遷就結構。留幾個破折號、留一個漂亮的三段式都沒問題；問題永遠是**成群的、空洞的**那種。

---

## 常見問題 — FAQ

**Q：所以看到破折號就能斷定是 AI 寫的？**
不行。破折號是人類用了幾百年的標點，很多作者本來就愛用（研究裡人類基準最高到每千字 17 次）。它只是機率上的提示，不是證據。真要判斷，看的是**多個習慣的密度**加上**有沒有具體、擔責任的內容**，而不是單一符號。詳見 [AI 生成內容怎麼標記與辨識](../01-fundamentals/ai-content-watermark.md)：連官方浮水印都不能當定論，何況一個標點。

**Q：不同模型的文風差很多嗎？**
差很多，而且是訓練決定的。破折號密度從接近零到每千字十幾個都有。所以「AI 都愛用破折號」是錯的 - 是**某些訓練管線**的產物。這也意味著文風會隨版本改變：新一代模型常常補掉舊的習慣，又長出新的。

**Q：這跟 prompt engineering 有關嗎？**
有，而且是同一條原理。文風習慣壓不掉，跟能力選不準，都來自「模糊指令 → 模糊結果」。解法也一樣：**具體、可檢查、給替代**。

---

## 相關筆記 — Related

- [AI 怎麼知道該用哪種能力](./how-ai-picks-capability.md) - 為什麼模糊指令得到模糊結果，文風也適用
- [AI 生成內容怎麼標記與辨識](../01-fundamentals/ai-content-watermark.md) - 為什麼「看起來像 AI」永遠不是證據
- [六種能力執行手冊](./capabilities-playbook.md) - 「給標準」是拉成效的通用槓桿，文風就是一種標準

## Sources

- [The Last Fingerprint: How Markdown Training Shapes LLM Prose — E. M. Freeburg, 2026](https://arxiv.org/abs/2603.27006)
- [OpenAI says it's fixed ChatGPT's em dash problem — TechCrunch, 2025](https://techcrunch.com/2025/11/14/openai-says-its-fixed-chatgpts-em-dash-problem/)
- [Why do AI models use so many em-dashes? — Sean Goedecke](https://www.seangoedecke.com/em-dashes/)
- [Why Did LLMs Steal Our Em-Dashes? — McGill Office for Science and Society](https://www.mcgill.ca/oss/article/critical-thinking-student-contributors-technology/why-did-llms-steal-our-em-dashes)
- [Why ChatGPT writes like that — Colin Gorrie](https://www.deadlanguagesociety.com/p/rhetorical-analysis-ai)
- [Indicators that suggest something was written by AI — Cherryleaf](https://www.cherryleaf.com/2026/02/indicators-that-suggest-something-was-written-by-ai/)
