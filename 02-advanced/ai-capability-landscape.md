---
title: AI 101 - AI 能力全景圖
tags: [ai, 能力地圖, 認知框架, convergent, divergent, bloom, 進階]
created: 2026-07-04
---

# AI 能力全景圖

[← 回主頁](../index.md)

> [!info]
> 大家習慣把 AI 能力列成一長串平行條目(摘要、翻譯、分類、發想⋯⋯),但這樣看不出彼此的關係,也不知道自己漏用了哪些。這篇用兩條**有學術依據**的軸,把 AI 的能力攤成一張二維地圖,讓你一眼看出:每個能力的本質、風險高低、以及你目前偏用哪一區。

---

## 是什麼:AI 能力是二維的,不是一張清單

任何一個 AI 任務,都可以用兩個互相獨立(正交)的問題定位:

1. **資訊量往哪個方向走?** —— 是把多變少(收斂)、等量換形式(轉換),還是把少變多(發散)?
2. **需要多少思考?** —— 是照著資料做(低階),還是要推理判斷(高階)?

這兩個問題來自兩套經典理論,底下分別說明。

---

## 軸一(橫):資訊流向 —— 收斂 / 轉換 / 發散

這條軸的兩端來自心理學家 **J.P. Guilford 在 1956 年**提出的兩種思考模式:

| 思考模式 | Guilford 的定義 | 本質 |
|---|---|---|
| **收斂思考**(Convergent) | 用邏輯、準確性、演繹,從多個選項中評估出**那個最佳解**——通常有正確答案 | **評估**(evaluation) |
| **發散思考**(Divergent) | 開放地產生多個新穎的想法、解法、可能性 | **變異**(variation) |

> [!quote]
> Guilford 的核心區分:「divergent thinking involves variation, convergent thinking involves evaluation.」發散是製造變化,收斂是做評估。

中間還有一段 Guilford 沒特別命名、但實務上極重要的區帶——**轉換**(Transformation)。**OpenAI 官方 prompt 指南**把摘要、改寫、翻譯、抽取歸為同一類,理由是它們有一個共同性質:

> [!quote]
> Transformation 類任務「must remain faithful to the input content」(必須忠於輸入內容)。

### 這條軸最有用的性質:忠實度梯度

把三段連起來,會得到一條「**你有多需要 AI 忠於原文**」的梯度:

```
收斂 ←──────────────── 轉換 ────────────────→ 發散
必須忠於輸入、可驗證                     要的就是無中生有
幻覺 = bug(最怕它亂加)                幻覺 = feature(就要它亂想)
────────────────────────────────────────────────────
摘要 統整 抽取 分類 排序     翻譯 改寫 解釋 格式轉換     續寫 舉例 發想 替代方案
```

> [!tip]
> 這解釋了一個你一定遇過的現象:**發想時 AI「亂講」你不介意,整理資料時卻很在意。** 因為兩端對「忠實度」的要求是相反的。用 AI 前先問自己在哪一端,就知道該不該擔心幻覺、該不該逐句查證。

---

## 軸二(縱):認知深度 —— Bloom's Taxonomy

同樣是「摘要」,把三段新聞濃縮成一句,跟讀完十篇論文提煉出一個沒人講過的觀點,難度天差地遠。差別在**認知深度**,依據是教育學的 **Bloom's Taxonomy(布魯姆分類法)修訂版**,把認知由淺到深分六層:

| 層級 | 中文 | 在做什麼 | 高/低階 |
|---|---|---|---|
| Remember | 記憶 | 回想事實、定義 | 低階 |
| Understand | 理解 | 用自己的話解釋、摘要 | 低階 |
| Apply | 應用 | 把已知套到新情境 | 低階 |
| Analyze | 分析 | 拆解、找出組成與因果關係 | **高階** |
| Evaluate | 評估 | 下判斷、給理由 | **高階** |
| Create | 創造 | 綜合各元素形成一個原創整體 | **高階** |

**關鍵:這條軸和橫軸無關(正交)。** 「摘要」可以是低階(單純濃縮 = Understand),也可以是高階(統整多份來源 + 評估重要性 = Analyze + Evaluate + Create)。認知深度是獨立於資訊流向的第二個維度。

---

## 全景圖:兩軸交叉

把兩條軸交叉,AI 的常見能力就各就各位:

| | **收斂**(多→少) | **轉換**(等量換形式) | **發散**(少→多) |
|---|---|---|---|
| **低階**(照著資料做) | 摘要 Summarization<br>抽取 Extraction<br>分類 Classification | 翻譯 Translation<br>改寫 Rewriting<br>格式轉換 Reformatting | 續寫 Continuation<br>舉例 Exemplification |
| **高階**(需要推理) | 統整 Synthesis<br>排序 Prioritization<br>評估 Evaluation | 解釋 Explanation<br>風格轉換 Style transfer | 發想 Ideation<br>替代方案 Alternatives |

### 四個容易卡住的位置

**統整 vs 摘要 —— 差在來源有幾份。**
摘要是把**一份**文件濃縮;統整是把**多份**來源整合成一個新的整體判斷。讀完十篇論文寫出「這個領域的共識在哪、分歧在哪」,那句話**任何一篇論文都沒寫過** —— 那才是統整。
（原文用詞是 Synthesis,中文譯「綜合」較常見,但「統整」更貼近「把分散的東西組成一個有結構的整體」這個動作。）

**摘要 vs 抽取 —— 差在輸出長什麼樣。**
摘要要的是這份文件的縮小版,輸出是**一段話**;抽取要的是文件裡的某一類東西,輸出是**一份清單或表格**。「把會議記錄摘要成三點」和「把裡面所有待辦事項與負責人列出來」是兩件事 —— 後者可以逐項回原文核對,前者不行。

**分類為什麼是低階收斂。**
輸入一整篇文件、輸出一個標籤,資訊壓縮得非常徹底,所以是收斂。而只要類別是**你給定的**,它就只是模式比對,所以低階。唯一會往上跳的情況:要它**自己想出該分哪些類**。

**排序為什麼是高階收斂 —— 這格有張力,值得知道。**
排序不會讓東西變少:十項進去、十項出來。照「資訊量多→少」的字面定義,它應該在轉換。

它放收斂,是因為這條軸其實有兩種讀法:

| 讀法 | 出處 | 排序符合嗎 |
|---|---|---|
| 資訊量:多 → 少 | 本文的直觀講法 | ❌ 數量沒變 |
| **收斂 = 評估,發散 = 製造變化** | **Guilford 1956 的原始定義** | ✅ 排序就是評估 |

多數能力兩種讀法一致,所以平常不會察覺;**排序剛好是它們分岔的地方。** 從實用角度放收斂是對的 —— 這條軸真正的用處是忠實度梯度,而排序的結果可以完全回原文查證(每一項的排名有沒有依據、有沒有冒出不存在的項目)。**它的驗法跟摘要一樣,不跟發想一樣。**

### 跨全圖的「複合能力」

有些能力不落在單一格子,而是把多格組合起來完成一個目標:

> [!IMPORTANT]
> 單一格是原料,實際工作幾乎都是把好幾格**串起來**:
>
> **分析 Analysis** ＝ 抽取（低階收斂）→ 統整（高階收斂）→ 評估（高階收斂）
>
> **規劃 Planning** ＝ 分析現況 ＋ 發想選項（高階發散）＋ 收斂成步驟（高階收斂）

> [!WARNING]
> **這兩條等式沒有直接來源,是本文自己的表述。** 沒有任何文獻把「分析」或「規劃」寫成這樣的公式。不過拆解的形狀各有一半站得住:
>
> | 部分 | 有沒有依據 |
> |---|---|
> | 分析的前兩步 | ✅ Anderson & Krathwohl 修訂版把 **Analyze 的子歷程**定為 *differentiating*（區辨,挑出相關的）→ *organizing*（組織,找出各部分怎麼組合）,對應抽取與統整 |
> | 分析的第三步「評估」 | ❌ **是本文接上去的**。Bloom 的 Analyze 第三個子歷程是 *attributing*（歸因,判斷立場與意圖）;而 Evaluate 在 Bloom 裡是**另一個層級**,不屬於分析 |
> | 規劃的發散→收斂 | ✅ 形狀與英國設計協會 2005 年的 **Double Diamond** 一致:兩輪「發散探索 → 收斂決定」 |
> | 規劃的三段切法 | ❌ Double Diamond 分四階段（Discover / Define / Develop / Deliver）,不是三段。**本文的切法是為了對上這張圖而簡化的** |

其他兩個常見的複合能力:

- **推論 Reasoning** —— 從已知推未知,橫跨理解到創造
- **問答 QA** —— 依問題可能落在任何一格

> [!info]
> 你跟 AI「討論一個主題」看似單一動作,其實是在快速切換多個格子:它先**抽取**你話裡的重點(收斂低階)、**分析**你的邏輯(高階)、再**發想**反例(發散高階)。一次對話用掉半張圖。

---

## 對實務的三個意義

### 1. 先定位,再決定要不要查證

用 AI 前先問「這在哪一區」。**越靠收斂/轉換端,越該逐項核對**(它應該忠於原文,加料就是錯);**越靠發散端,越不必**(你要的就是它發揮)。

### 2. 高階任務要給它「思考空間」

落在**高階列**的任務(統整、評估、發想),用 chain-of-thought、讓它先列思路再給結論,品質差很多。低階任務(翻譯、分類)通常直接給答案即可,不必囉嗦。

### 3. 檢查自己的使用盲區

大多數人只重度使用**兩個角落**:左上(摘要)和右下(發想)。中間那整欄**轉換**(翻譯、改寫、換格式、把難的解釋成簡單的)最穩、最少幻覺,卻常被忽略。看看這張圖,你有哪幾格幾乎沒用過?

---

## 常見問題

**Q:收斂 / 發散 這組詞是 AI 圈發明的嗎?**
不是。是心理學家 Guilford 在 1956 年研究智力與創造力時提出的,遠早於 LLM。AI 只是剛好非常適合套這個框架。

**Q:為什麼把「摘要」和「翻譯」分在不同軸區?**
因為方向不同。摘要是**收斂**(多→少,會丟資訊);翻譯是**轉換**(等量,不丟資訊只換語言)。但 OpenAI 把兩者都歸為「須忠於輸入」的一大類——這是它們的共同點,不是同一格。

**Q:這張圖能拿來選模型嗎?**
可以當粗略參考:高階格子(統整、規劃、複雜推理)吃模型的推理能力,值得用更強的模型;低階格子(分類、翻譯)較輕量的模型多半就夠。詳見 [AI 101 - 模型費用與效果比較](../01-fundamentals/model-cost-comparison.md)。

---

## 相關筆記

- [AI 101 - 核心概念](../01-fundamentals/core-concepts.md) —— Agent、RAG、幻覺等基礎詞彙
- [AI 101 - 模型費用與效果比較](../01-fundamentals/model-cost-comparison.md) —— 依任務難度(認知深度)選模型
- [AI 101 - Context Engineering](./context-engineering.md) —— 把對的資訊餵進對的格子

## Sources

- [Divergent Versus Convergent Thinking — Springer(Guilford 1956 起源與定義)](https://link.springer.com/rwe/10.1007/978-1-4614-3858-8_362)
- [Guilford's Divergent and Convergent Thinking — Cogn-IQ](https://www.cogn-iq.org/guilford-divergent-convergent-thinking.php)
- [Best practices for prompt engineering with the OpenAI API — OpenAI Help Center(Transformation 須忠於輸入)](https://help.openai.com/en/articles/6654000-best-practices-for-prompt-engineering-with-the-openai-api)
- [Bloom's Revised Taxonomy — Colorado College](https://www.coloradocollege.edu/other/assessment/how-to-assess-learning/learning-outcomes/blooms-revised-taxonomy.html)
- [Bloom's Taxonomy of Learning — Simply Psychology](https://www.simplypsychology.org/blooms-taxonomy.html)
- [Anderson & Krathwohl 修訂版全表（含 Analyze 的三個子歷程）— Quincy College PDF](https://quincycollege.edu/wp-content/uploads/Anderson-and-Krathwohl_Revised-Blooms-Taxonomy.pdf)
- [The Double Diamond — UK Design Council](https://www.designcouncil.org.uk/our-resources/the-double-diamond/)
