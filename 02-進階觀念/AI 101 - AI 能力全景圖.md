---
title: AI 101 - AI 能力全景圖
tags: [ai, 能力地圖, 認知框架, convergent, divergent, bloom, 進階]
created: 2026-07-04
---

# AI 能力全景圖

[[AI 101 - 主頁|← 回主頁]]

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
摘要 綜合 抽取 分類 排序   翻譯 改寫 解釋 格式轉換   發想 擴寫 替代方案 腦力激盪
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

**關鍵:這條軸和橫軸無關(正交)。** 「摘要」可以是低階(單純濃縮 = Understand),也可以是高階(綜合多來源 + 評估重要性 = Analyze + Evaluate + Create)。認知深度是獨立於資訊流向的第二個維度。

---

## 全景圖:兩軸交叉

把兩條軸交叉,AI 的常見能力就各就各位:

| | **收斂**(多→少) | **轉換**(等量換形式) | **發散**(少→多) |
|---|---|---|---|
| **低階**(照著資料做) | 摘要 Summarization<br>抽取 Extraction<br>分類 Classification | 翻譯 Translation<br>改寫 Rewriting<br>格式轉換 Reformatting | 擴寫 Elaboration<br>續寫 Continuation |
| **高階**(需要推理) | 綜合 Synthesis<br>排序 Prioritization<br>評估 Evaluation | 解釋 Explanation<br>簡化 Simplification<br>風格轉換 Style transfer | 發想 Ideation<br>腦力激盪 Brainstorming<br>替代方案 Alternatives |

### 跨全圖的「複合能力」

有些能力不落在單一格子,而是把多格組合起來完成一個目標:

- **分析 Analysis** —— 通常是「抽取 → 綜合 → 評估」的組合
- **推論 Reasoning** —— 從已知推未知,橫跨理解到創造
- **規劃 Planning** —— 分析現況 + 發想選項 + 收斂成步驟
- **問答 QA** —— 依問題可能落在任何一格

> [!info]
> 你跟 AI「討論一個主題」看似單一動作,其實是在快速切換多個格子:它先**抽取**你話裡的重點(收斂低階)、**分析**你的邏輯(高階)、再**發想**反例(發散高階)。一次對話用掉半張圖。

---

## 對實務的三個意義

### 1. 先定位,再決定要不要查證

用 AI 前先問「這在哪一區」。**越靠收斂/轉換端,越該逐項核對**(它應該忠於原文,加料就是錯);**越靠發散端,越不必**(你要的就是它發揮)。

### 2. 高階任務要給它「思考空間」

落在**高階列**的任務(綜合、評估、發想),用 chain-of-thought、讓它先列思路再給結論,品質差很多。低階任務(翻譯、分類)通常直接給答案即可,不必囉嗦。

### 3. 檢查自己的使用盲區

大多數人只重度使用**兩個角落**:左上(摘要)和右下(發想)。中間那整欄**轉換**(翻譯、改寫、換格式、把難的解釋成簡單的)最穩、最少幻覺,卻常被忽略。看看這張圖,你有哪幾格幾乎沒用過?

---

## 常見問題

**Q:收斂 / 發散 這組詞是 AI 圈發明的嗎?**
不是。是心理學家 Guilford 在 1956 年研究智力與創造力時提出的,遠早於 LLM。AI 只是剛好非常適合套這個框架。

**Q:為什麼把「摘要」和「翻譯」分在不同軸區?**
因為方向不同。摘要是**收斂**(多→少,會丟資訊);翻譯是**轉換**(等量,不丟資訊只換語言)。但 OpenAI 把兩者都歸為「須忠於輸入」的一大類——這是它們的共同點,不是同一格。

**Q:這張圖能拿來選模型嗎?**
可以當粗略參考:高階格子(綜合、規劃、複雜推理)吃模型的推理能力,值得用更強的模型;低階格子(分類、翻譯)較輕量的模型多半就夠。詳見 [[AI 101 - 模型費用與效果比較]]。

---

## 相關筆記

- [[AI 101 - 核心概念]] —— Agent、RAG、幻覺等基礎詞彙
- [[AI 101 - 模型費用與效果比較]] —— 依任務難度(認知深度)選模型
- [[AI 101 - Context Engineering]] —— 把對的資訊餵進對的格子

## Sources

- [Divergent Versus Convergent Thinking — Springer(Guilford 1956 起源與定義)](https://link.springer.com/rwe/10.1007/978-1-4614-3858-8_362)
- [Guilford's Divergent and Convergent Thinking — Cogn-IQ](https://www.cogn-iq.org/guilford-divergent-convergent-thinking.php)
- [Best practices for prompt engineering with the OpenAI API — OpenAI Help Center(Transformation 須忠於輸入)](https://help.openai.com/en/articles/6654000-best-practices-for-prompt-engineering-with-the-openai-api)
- [Bloom's Revised Taxonomy — Colorado College](https://www.coloradocollege.edu/other/assessment/how-to-assess-learning/learning-outcomes/blooms-revised-taxonomy.html)
- [Bloom's Taxonomy of Learning — Simply Psychology](https://www.simplypsychology.org/blooms-taxonomy.html)
