---
title: AI 不只是 ChatGPT：25 個核心演算法地圖
tags: [ai, 外部觀點, 機器學習, 演算法, 深度學習, 強化學習, 參考]
source: https://www.facebook.com/smartrobotcom/posts/pfbid0trYC5yJddr8btSJfaDcyA7ExQZZti77q48QuryXZNjEtKyjAv5npPkU2kAc1srT4l
author: Smart Robots 智慧機器人
created: 2026-05-14
---

# AI 不只是 ChatGPT：25 個核心演算法地圖

> [!quote]
> "AI 不只是 ChatGPT，推薦系統、自駕車、影像辨識、人形機器人背後，各有不同的演算法在運作。"

---

## 為什麼這張地圖有用

大多數人對「AI」的認識停在 LLM（大型語言模型），
但真實世界的 AI 應用橫跨 25 種以上的核心演算法，
分屬四個不同的技術家族。

---

## 一、迴歸與分類（1–10）

| # | 演算法 | 典型應用 |
|---|---|---|
| 1 | **Linear Regression** 線性迴歸 | 房價預測、銷售預測 |
| 2 | **Logistic Regression** 邏輯迴歸 | 信用評估、疾病診斷 |
| 3 | **Decision Tree** 決策樹 | 客戶分類、風險評估 |
| 4 | **Random Forest** 隨機森林 | 金融詐欺偵測、醫療診斷 |
| 5 | **SVM** 支援向量機 | 文字分類、圖像辨識 |
| 6 | **K-Nearest Neighbors** K 近鄰 | 推薦系統、異常偵測 |
| 7 | **Naive Bayes** 樸素貝氏 | 垃圾郵件過濾、情感分析 |
| 8 | **GBDT** 梯度提升決策樹 | 廣告點擊率預測、排名 |
| 9 | **AdaBoost** 自適應提升 | 人臉偵測、物件辨識 |
| 10 | **XGBoost** | 競賽冠軍常客、結構化資料預測 |

---

## 二、分群與降維（11–15）

| # | 演算法 | 典型應用 |
|---|---|---|
| 11 | **K-Means** | 客群分析、影像壓縮 |
| 12 | **Hierarchical Clustering** 層次分群 | 基因資料分析、文件分類 |
| 13 | **DBSCAN** | 地理資料分析、異常偵測 |
| 14 | **PCA** 主成分分析 | 特徵壓縮、資料視覺化 |
| 15 | **t-SNE** | 高維資料視覺化、探索性分析 |

---

## 三、強化學習（16–20）

| # | 演算法 | 典型應用 |
|---|---|---|
| 16 | **Q-Learning** | 遊戲 AI、簡單機器人控制 |
| 17 | **SARSA** | 保守策略的機器人導航 |
| 18 | **Deep Q-Network (DQN)** | Atari 遊戲、複雜遊戲 AI |
| 19 | **Policy Gradient** | 連續動作控制、機器人手臂 |
| 20 | **Actor-Critic** | 自駕車、人形機器人 |

---

## 四、深度學習（21–25）

| # | 演算法 | 典型應用 |
|---|---|---|
| 21 | **ANN** 人工神經網路 | 基礎分類與迴歸任務 |
| 22 | **CNN** 卷積神經網路 | 影像辨識、臉部識別、醫療影像 |
| 23 | **RNN** 循環神經網路 | 時間序列、早期語言模型 |
| 24 | **LSTM** 長短期記憶網路 | 語音辨識、機器翻譯、股價預測 |
| 25 | **Transformer** | GPT、BERT、所有現代 LLM 的基礎 |

---

## 一張圖看懂選哪個

```
你的資料是結構化表格？
  → 迴歸/分類（1–10）：XGBoost 通常是首選

你需要找資料裡的群組或模式？
  → 分群/降維（11–15）：K-Means 或 DBSCAN

你的任務是「讓 AI 學會做決策」？
  → 強化學習（16–20）：從 DQN 入手

你的資料是圖片/文字/語音？
  → 深度學習（21–25）：圖片用 CNN、文字用 Transformer
```

---

## 延伸思考

- Transformer 架構（#25）統治了 NLP，現在也開始入侵 CV（Vision Transformer）——邊界正在模糊
- 現實中的系統常常是混合的：推薦系統可能同時用 Embedding + GBDT + 強化學習
- LLM 是 Transformer 的一種，但 Transformer 不等於 LLM

---

## 相關筆記

- [[AI 101 - ML 演算法精要]] — Random Forest、XGBoost、LSTM、HMM 的深入說明與程式碼
- [[AI 101 - 核心概念]] — LLM、Agent、RAG 等基礎詞彙
