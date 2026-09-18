---
title: AI 101 - 鐵人賽 Day 05：AI 怎麼知道該用哪種能力
tags: [ai, 鐵人賽, ironman, in-context-learning, instruction-tuning, 機制, 草稿]
created: 2026-09-18
status: draft
---

# Day 05｜AI 怎麼知道該用哪種能力

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> [Day 04](./day04-capability-landscape.md) 把能力攤成一張圖。但圖上那麼多格，**你說一句話，它怎麼知道要用哪一格？** 今天往裡面看一層：那個選擇不是誰設定的，是模型從你的話裡「算」出來的 - 而且這件事有可以動手驗證的機制證據。

> **TL;DR (EN):** Nobody configures which capability the model uses. Pretraining makes it infer a latent task from context, and that inferred task provably exists inside the model as a single extractable direction - you can pull it out of one prompt and paste it into another. Instruction tuning is what binds everyday phrasing to those tasks. Three practical consequences: examples beat adjectives, capability belongs in the request rather than the persona, and the model's own account of its steps is not evidence.

---

## 沒有人幫它選 — Nobody Picked For It

你說「幫我摘要」它就摘要，說「幫我想幾個點子」它就發想。中間沒有人按按鈕。

最自然的猜測是：背後有一張表，或有一段設定，寫著「遇到什麼要用什麼」。

這個猜測有一個很乾脆的反證：**同一個模型，把所有設定清空、直接送一句話進去，它照樣分得出來。** 沒有任何地方寫著「摘要請走這條」。

所以真正的答案是別的：**那個選擇是推論出來的，而推論這件事是訓練出來的。**

分兩層。第一層讓它「認得出你要什麼」，第二層讓它「聽得懂你怎麼說」。

---

## 第一層：它在算你要的是什麼 — It Infers the Task

2021 年 Xie 等人提出一個解釋：模型在預訓練時，為了把下一個字猜準，不得不一直做一件事 - **推論這份文件的潛在主題是什麼**。看到「親愛的客戶您好」，它得先推出「這是一封客服信」，後面的字才猜得準。

到了你使用它的時候，它把完全相同的動作套到你的 prompt 上：**推論「這段對話的潛在任務是什麼」。**

這不是打比方。2023 年有兩組研究各自找到了直接的證據，做法大致是這樣：

1. 給模型幾個示範例子（例如「法國→巴黎、日本→東京」）
2. 把模型內部的狀態抽出來，發現那個任務被壓縮成**單一一個向量**
3. 換一個**完全沒有示範**的新場景，把那個向量貼回去
4. 模型執行了那個任務 - 效果跟看過全部示範差不多

> [!IMPORTANT]
> 這代表模型裡真的存在一個「我現在要做什麼」的東西，而且它可以被**抽出來、搬走、貼到別的地方**。不是隱喻，是一個可以拿在手上的物件。

換成白話：

> **它不是查表決定「要用摘要能力」，而是從你的話裡算出一個座標。**

這一句就能推出今天最實用的一條心法，等一下會回來講。

---

## 第二層：讓「用講的」就能指定 — Instruction Tuning

第一層解決了「從語境認出任務」，但沒解決「聽得懂命令」。光靠第一層，你得給範例它才知道要幹嘛。

補上這一塊的是**指令微調**（instruction tuning）。

2021 年的 FLAN 是這條路線的代表：拿一個模型，在 60 多個任務（後來擴到 1,800 多個）上微調，每一個任務都用**自然語言指令**包起來。結果是 - 在**訓練時沒見過的任務類型**上，它的 zero-shot 表現大幅提升，25 個評測任務裡有 20 個贏過規模大得多的 GPT-3。

消融實驗指出三個要素缺一不可：任務數量、模型規模、**以及有沒有用自然語言指令包裝**。最後那項最說明問題 - **同樣的資料，不包成指令，效果就掉下來。**

所以這一層教會的不是「怎麼摘要」（第一層就會了），而是：

> **聽到「摘要一下」這四個字，要對應到哪一個任務座標。**

---

## 所以呢？三條心法 — What This Changes

### 一、給範例，比給形容詞有效

既然那個座標是從你的話裡算出來的，**話越模糊，算出來的位置越糊。**

而且注意上面那個實驗：任務向量是從**示範例子**裡抽出來的。範例不是「多給一點資料」，範例是你操作那個座標最直接的介面。

**自己試：** 同一份會議記錄，比較這兩種說法拿到的東西差多少：

| 說法 | 它要猜什麼 |
|:---|:---|
| 「幫我整理得專業一點」 | 「專業」是什麼？座標大概落在中間某處 |
| 「像這樣：`決議：X ｜ 負責人：Y ｜ 期限：Z`，把全部列出來」 | 座標被範例釘死了 |

### 二、能力寫進請求，不是寫進人設

人設（「你是一位資深分析師」）影響的是語氣和風格。**任務座標是從你實際要求的動作算出來的。**

所以與其寫「你是一位資深分析師」，不如直接寫「先把資料裡的數字抽出來，再判斷哪些重要」 - 後者才是在動那個座標。用 [Day 04](./day04-capability-landscape.md) 的圖來看，你是在指定要走哪幾格。

### 三、它說它做了什麼，不能當證據

這條最重要，也最容易被忽略。

你叫它「先抽取再統整」，它回答裡寫「我先抽取了以下要點……」 - **那段話不構成它真的這樣做了的證據。**

2025 年 Anthropic 的一篇研究測了這件事：給模型一個提示、確認它確實用了那個提示，再看它的思考過程有沒有承認。結果是**承認率通常低於 20%**。用強化學習訓練會在早期改善，但很快就停在一個高原，不會繼續往上。

> [!WARNING]
> 所以 [Day 03](./day03-what-it-cannot-do.md) 那條「驗證必須來自外部」，在這裡有了第二個理由：**不只它不會發現自己錯了，連它對自己步驟的描述也不可靠。**
>
> 要確認它有沒有做某一步 - **看產出對不對，不要看它說它做了什麼。**

---

今天這篇往裡面看了一層，結論其實很短：

> **你不是在下指令，你是在給它足夠的線索，讓它推論出你要什麼。**

推論會推錯。你以為你在要「統整」，它推成「摘要」，於是你拿到一份忠實但沒有新判斷的整理。修法不是把同一句話再講一次大聲一點，是換成它推不歪的說法。

明天開始講怎麼把話說到它推不歪。

---

## Sources

- [An Explanation of In-context Learning as Implicit Bayesian Inference — Xie, Raghunathan, Liang & Ma, ICLR 2022](https://arxiv.org/abs/2111.02080)
- [In-Context Learning Creates Task Vectors — Hendel, Geva & Globerson, Findings of EMNLP 2023](https://aclanthology.org/2023.findings-emnlp.624/)
- [Function Vectors in Large Language Models — Todd et al., ICLR 2024](https://functions.baulab.info/)
- [Finetuned Language Models Are Zero-Shot Learners（FLAN）— Wei et al., 2021](https://arxiv.org/abs/2109.01652)
- [Reasoning Models Don't Always Say What They Think — Chen et al., Anthropic, 2025](https://arxiv.org/abs/2505.05410)
