---
title: AI 101 - 鐵人賽 Day 01：它只是在猜下一個字
tags: [ai, 鐵人賽, ironman, llm, 原理, 統計, 草稿]
created: 2026-09-02
status: draft
---

# Day 01｜它只是在猜下一個字：LLM 的原理，決定了後面 29 天的所有心法

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 這是三十天的第一篇。在講任何「怎麼用」之前，先把地基講清楚：**LLM 到底在做什麼**。後面 29 天的每一條心法，都是從這一篇推出來的。

> **TL;DR (EN):** An LLM does one thing: read everything so far, output a probability distribution over the next token, sample one, repeat. Shannon was already doing this by hand in 1948. That single fact explains why it sounds fluent, why it is confidently wrong, and why every technique in this series exists. But "it can't do what it never learned" is only half true — it recombines rather than recites, and it builds internal models of the process behind the data.

---

## 它在做的，是 1948 年就有人做過的事 — An Old Idea

> （jason3e7 的直覺）LLM 就是統計學。

這句話是對的，而且比多數人以為的更「字面」。

1948 年，Claude Shannon 在《A Mathematical Theory of Communication》裡做了一個實驗：他想知道英文到底有多可預測。做法很土——翻書、數字母，統計哪兩個字母常黏在一起，然後照這個機率一個一個抽出來。抽到第三層（每個字接前一個字的機率）時，他得到這樣一串東西：

> THE HEAD AND IN FRONTAL ATTACK ON AN ENGLISH WRITER THAT THE CHARACTER OF THIS POINT IS THEREFORE ANOTHER METHOD FOR THE LETTERS THAT THE TIME OF WHO EVER TOLD THE PROBLEM FOR AN UNEXPECTED

它不是英文，但已經很像英文。Shannon 沒有教它任何文法，他只統計了「哪個字後面常出現哪個字」。那是 1948 年，他只能靠手翻書，所以停在這裡。

今天的大型語言模型（LLM）做的是同一件事，只是把三個地方推到極端：**能看多長的前文、統計得多細、吃了多少資料。**

用一句話說完模型的工作：

> **看完前面所有的字 → 輸出「下一個 token 各自有多少機率」 → 抽一個出來 → 把它接到後面，再做一次。**

沒有第二個動作了。你看到的那些長篇大論、程式碼、分析報告，都是這個動作重複幾千次的結果。

> [!NOTE]
> **Token** 是模型眼中的最小單位，不完全等於「字」。大約 0.75 個英文單字 ≈ 1 token，中文 1 個字 ≈ 1–2 tokens。

理解了這件事，兩個最常見的疑問就有答案了：

- **它為什麼講得那麼順？** 因為「順」就是它被訓練的目標本身。流暢不是能力的證明，流暢是它唯一保證會做到的事。
- **它為什麼會一本正經地講錯？** 因為在機率上，一個「看起來很合理的答案」常常比「我不知道」更高分。

第二點有實證。OpenAI 在 2025 年的論文《Why Language Models Hallucinate》講得很直白：幻覺的起點只是二元分類的錯誤；而它為什麼一直存在，是因為主流評測用準確率打分——**猜對加分，猜錯扣的分跟老實說「不知道」一樣多，那當然要猜。** 他們開的處方也很直接：對「有自信的錯」要罰得比「承認不確定」更重。

換句話說：**幻覺不是壞掉了，是統計目標加上評分方式的必然產物。**

---

## 「沒學過的就不會」——對了一半 — Half Right

> （jason3e7 的直覺）沒學過的，是不會的。

**對的那一半：** 訓練資料裡沒有的事實，它真的不知道。知識截止日（knowledge cutoff）是一道硬邊界——問它昨天發生的事，它只能編。這也是 RAG（檢索增強生成）存在的唯一理由：把它沒學過的東西，塞進當下的脈絡裡給它看。

**要修的那一半：** 「沒學過」不等於「沒背過那句話」。它會的比背誦多，有兩個實驗可以說明。

**一、Othello-GPT（Li et al., 2022）。** 研究者只餵給一個小模型「合法的黑白棋棋步序列」，從頭到尾沒告訴它棋盤長什麼樣、規則是什麼，任務就只有猜下一步。結果把模型拆開來看，裡面竟然長出了**當前棋盤狀態的表徵**——而且研究者直接去修改那個內部表徵，模型的輸出就跟著改變。從一個純統計的目標，長出了「產生這些資料的那個過程」的模型。（後續 Neel Nanda 的研究進一步指出，那個表徵是線性的、更容易讀出來。）

**二、Anthropic 的《On the Biology of a Large Language Model》（2025）。** 他們用電路追蹤的方法直接看 Claude 內部在做什麼：

- **寫詩會先想好韻腳。** 開始寫一行之前，模型就已經選好了行尾要押的字，再回頭鋪陳前面——不是一個字一個字硬猜到最後剛好押上。
- **兩跳推理是真的在跳。** 問「Dallas 所在的那一州，首府是哪裡」，模型內部會先浮現「Texas」，再導出「Austin」。研究者把中間那個「Texas」改掉，答案就跟著變。
- **心算有專門的平行電路**，而且跟它嘴上講的計算步驟並不一樣。

所以更準確的講法是：

> **它會的是「重組與內插」，不是「背誦」。沒見過的組合，它拼得出來；沒有依據的事實，它拼不出來，只會編一個像樣的。**

這也是為什麼 2021 年那篇〈On the Dangers of Stochastic Parrots〉（隨機鸚鵡）的比喻，今天要分兩半看：關於偏見與過度炒作的批評，仍然成立；但強版本——「它只學到表面統計、內部沒有任何模型」——已經被上面兩個實驗推翻了。

反方向也要留一手。〈Are Emergent Abilities of Large Language Models a Mirage?〉（Schaeffer et al., 2023）指出：很多「能力在某個規模突然湧現」的漂亮曲線，換一個評分指標就變回平滑的直線。**兩邊都別信滿**——這本身就是後面要反覆用到的心法。

---

## 為什麼這篇要放 Day 01 — Why This Comes First

因為接下來 29 天的每一條心法，都是從這個原理直接推出來的：

| 因為它…… | 所以…… | 在哪幾天 |
|---|---|---|
| 目標是「看起來合理」，不是「正確」 | 你必須有一套自己的驗證方法 | Day 18–20 |
| 下一個字的機率由前面**全部**內容決定 | 餵什麼，比怎麼問更重要 | Day 12–14 |
| 沒有內建「該停了」的概念，只會一直往下接 | 要給它可驗證的終點與護欄 | Day 15–17 |
| 擅長重組，不擅長無中生有 | 能帶你跨出自己的舒適圈，但跨不出人類知識的邊界 | Day 21–23 |
| 模糊的問題只會得到模糊分布下的抽樣 | 提問本身就要能被檢查 | Day 08–11 |
| 產出會留下可偵測的統計痕跡 | 標記與辨識做得到，但都不算證據 | Day 24 |

今天只要記住一句話：

> **你不是在跟一個知道答案的人講話，你是在跟一個非常會接話的機率機器講話。**

接下來的 29 天，講的都是怎麼跟這種東西合作。

---

## Sources

- [A Mathematical Theory of Communication — Claude E. Shannon, 1948（PDF）](https://monoskop.org/images/a/ae/Shannon_Claude_E_A_Mathematical_Theory_of_Communication_1957.pdf)
- [Why Language Models Hallucinate — Kalai, Nachum, Vempala & Zhang, OpenAI, 2025](https://arxiv.org/abs/2509.04664)
- [Emergent World Representations: Exploring a Sequence Model Trained on a Synthetic Task（Othello-GPT）— Li et al., 2022](https://arxiv.org/abs/2210.13382)
- [Actually, Othello-GPT Has A Linear Emergent World Representation — Neel Nanda](https://www.neelnanda.io/mechanistic-interpretability/othello)
- [On the Biology of a Large Language Model — Anthropic, 2025](https://transformer-circuits.pub/2025/attribution-graphs/biology.html)
- [On the Dangers of Stochastic Parrots — Bender, Gebru, McMillan-Major & Shmitchell, 2021](https://dl.acm.org/doi/10.1145/3442188.3445922)
- [Are Emergent Abilities of Large Language Models a Mirage? — Schaeffer, Miranda & Koyejo, 2023](https://arxiv.org/abs/2304.15004)
