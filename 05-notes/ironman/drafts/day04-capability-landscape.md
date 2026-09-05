---
title: AI 101 - 鐵人賽 Day 04：你其實只用了 AI 的兩種能力
tags: [ai, 鐵人賽, ironman, 能力地圖, convergent, divergent, bloom, 草稿]
created: 2026-09-03
status: draft
---

# Day 04｜你其實只用了 AI 的兩種能力：一張全景圖看完它會什麼

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 大家講「AI 會什麼」，習慣列一長串：摘要、翻譯、分類、發想……。清單看不出彼此的關係，也看不出你漏用了哪些。這篇用兩條有學術依據的軸，把 AI 的能力攤成一張二維地圖——**看完你會知道自己一直在用哪兩格，又有哪幾格從來沒碰過。**

> **TL;DR (EN):** Two independent axes place any AI task on one map. Horizontal: does information shrink (convergent), keep its size (transformation), or grow (divergent)? — Guilford, 1956. Vertical: how much thinking does it take? — Bloom's taxonomy. The horizontal axis doubles as a faithfulness gradient, which tells you when hallucination is a bug and when it is the entire point. Most people live in two corners and never touch the calmest column in the middle.

---

## 大家都只用兩個角落 — Two Corners

[Day 03](./day03-what-it-cannot-do.md) 的結論是：鋸齒狀前沿看不見，你不會知道自己什麼時候踩在缺口上。

但有一件事**可以事先知道**：這件任務，我到底該不該擔心它亂加東西。靠的不是經驗，是先看它落在地圖的哪一區。

任何一個 AI 任務，都可以用兩個互相獨立的問題定位：

1. **資訊量往哪走？** 把多變少、等量換個形式，還是把少變多？
2. **要動多少腦？** 照著資料做就好，還是需要推理判斷？

這兩個問題各有來歷，下面分開講。

---

## 橫軸：資訊往哪個方向流 — Axis 1: Information Flow

這條軸的兩端，是心理學家 J.P. Guilford 在 **1956 年**提出的兩種思考模式——比 LLM 早了六十幾年：

| | 在做什麼 | 本質 |
|---|---|---|
| **收斂思考**（Convergent） | 從一堆選項裡評估出**最好的那一個**，通常有正確答案 | 評估 |
| **發散思考**（Divergent） | 開放地生出很多新想法、解法、可能性 | 製造變化 |

Guilford 自己的話很精簡：發散是製造變化（variation），收斂是做評估（evaluation）。

中間還有一段他沒特別命名、但實務上最重要的區帶：**轉換**（Transformation）。OpenAI 官方的 prompt 指南把摘要、改寫、翻譯、抽取歸成同一大類，理由是它們有一個共同性質——**必須忠於輸入內容**（must remain faithful to the input content）。

### 這條軸真正的用處：一條忠實度梯度

把三段連起來，你會得到一條「**你有多需要它忠於原文**」的梯度：

```text
收斂 ←──────────────── 轉換 ────────────────→ 發散
必須忠於輸入、可查證                       要的就是無中生有
幻覺 = bug（最怕它亂加）                   幻覺 = feature（就要它亂想）
──────────────────────────────────────────────────────
摘要 綜合 抽取 分類 排序   翻譯 改寫 解釋 換格式   發想 擴寫 替代方案 腦力激盪
```

這解釋了一個你一定遇過的現象：**發想的時候它亂講你不介意，整理資料的時候你很在意。** 因為這兩端對「忠實度」的要求根本是相反的。

> [!TIP]
> 這就是 Day 03 那個「什麼時候該驗」的第一層答案：**越靠左邊，越要逐項核對**（它應該忠於原文，多加的就是錯）；**越靠右邊，越不用**（你要的就是它發揮）。

**自己試：** 拿同一份會議記錄，給它兩個指令，然後注意你驗收的方式完全不同：

| 指令 | 落在哪 | 你該怎麼驗 |
|---|---|---|
| 「整理成三點重點」 | 收斂 | 每一點都回原文對一次，**多出來的就是錯** |
| 「這場會議還有什麼沒被討論到的風險？」 | 發散 | 不必對原文，要挑的是**有沒有用** |

同一份資料、同一個模型，驗法卻相反——因為它們不在同一區。

---

## 縱軸：要動多少腦 — Axis 2: Cognitive Depth

同樣叫「摘要」，把三則新聞濃縮成一句，跟讀完十篇論文提煉出一個沒人講過的觀點，難度差很遠。差在**認知深度**。

依據是教育學的 Bloom's Taxonomy（布魯姆分類法）修訂版，由淺到深六層：

| 層級 | 在做什麼 | |
|---|---|---|
| 記憶 Remember | 回想事實、定義 | 低階 |
| 理解 Understand | 用自己的話講一遍 | 低階 |
| 應用 Apply | 把學過的套到新情境 | 低階 |
| 分析 Analyze | 拆解，找出組成與因果 | **高階** |
| 評估 Evaluate | 下判斷，而且給得出理由 | **高階** |
| 創造 Create | 把零件組成一個原創的整體 | **高階** |

關鍵在於：**這條軸跟橫軸無關。** 「摘要」可以很淺（單純濃縮），也可以很深（綜合多來源 ＋ 判斷哪些重要 ＋ 講出沒人講過的角度）。所以它是獨立的第二個維度，不是同一條線的延伸。

> [!IMPORTANT]
> 實務差別：**高階任務要給它思考空間。** 綜合、評估、發想這類，讓它先列思路再給結論，品質差很多；低階任務（翻譯、分類）直接要答案就好，囉嗦反而是浪費 token。這一條 Day 07 展開。

---

## 交叉起來：你的盲區在哪 — The Map

兩條軸一交叉，常見能力就各就各位：

| | **收斂**（多→少） | **轉換**（等量換形式） | **發散**（少→多） |
|---|---|---|---|
| **低階**（照著資料做） | 摘要、抽取、分類 | 翻譯、改寫、換格式 | 擴寫、續寫 |
| **高階**（要推理判斷） | 綜合、排序、評估 | 解釋、簡化、換風格 | 發想、腦力激盪、替代方案 |

有些能力不落在單一格，而是把好幾格串起來：

- **分析** ＝ 抽取 → 綜合 → 評估
- **規劃** ＝ 分析現況 ＋ 發想選項 ＋ 收斂成步驟
- **跟它「討論一個主題」** 看起來是一個動作，其實是在快速切格子：先抽取你話裡的重點（左上）、分析你的邏輯（左下）、再發想反例（右下）。**一次對話用掉半張圖。**

現在回答標題那個問題。多數人重度使用的其實只有**兩個角落**：左上的**摘要**，和右下的**發想**。

而中間那一整欄「轉換」——翻譯、改寫、換格式、**把難懂的東西解釋成簡單的**——是全圖**最穩、幻覺最少**的一區，因為它的定義本身就是「不准加東西」。它偏偏也是最常被忽略的一區。

> [!TIP]
> （jason3e7 的狀況）我一開始就只用兩件事：把一大堆資料整理出重點、還有發想。剛好就是左上跟右下那兩個角落。中間整欄幾乎沒碰過。

**看著這張圖問自己：哪幾格我幾乎沒用過？** 那通常不是因為 AI 做不好，而是你沒想到可以這樣用。

接下來兩天：明天先盤點 prompt engineering 到底哪些技巧真的有效（Day 05）；之後把圖上最常用的四格——摘要、解釋、發想、重構——各給一套配方（Day 07）。

---

## Sources

- [Divergent Versus Convergent Thinking — Springer（Guilford 1956 起源與定義）](https://link.springer.com/rwe/10.1007/978-1-4614-3858-8_362)
- [Guilford's Divergent and Convergent Thinking — Cogn-IQ](https://www.cogn-iq.org/guilford-divergent-convergent-thinking.php)
- [Best practices for prompt engineering with the OpenAI API — OpenAI（Transformation 須忠於輸入）](https://help.openai.com/en/articles/6654000-best-practices-for-prompt-engineering-with-the-openai-api)
- [Bloom's Revised Taxonomy — Colorado College](https://www.coloradocollege.edu/other/assessment/how-to-assess-learning/learning-outcomes/blooms-revised-taxonomy.html)
- [Bloom's Taxonomy of Learning — Simply Psychology](https://www.simplypsychology.org/blooms-taxonomy.html)
