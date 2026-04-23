---
title: AI 101 - ML 演算法精要
tags: [ai, ml, algorithm, isolation-forest, random-forest, xgboost, pelt, lstm, hmm]
created: 2026-04-23
updated: 2026-04-23
---

# ML 演算法精要

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> 六個常見演算法的核心觀念與快速上手範例。
> 安裝：`pip install scikit-learn xgboost ruptures tensorflow hmmlearn`

---

## 先看懂分類

```
六個演算法分三組：

異常偵測（找「奇怪的點」）
  └── Isolation Forest

集成學習（多棵樹投票）
  ├── Random Forest     ← 獨立的樹，票票等值
  └── XGBoost          ← 樹接龍，後面的修前面的錯

時序 / 序列分析（處理「有順序的資料」）
  ├── PELT             ← 找「行為突然改變」的時間點
  ├── LSTM             ← 學習序列的長期規律
  └── HMM              ← 從外部觀測推斷隱藏狀態
```

---

## Isolation Forest（隔離森林）

> **一句話：** 容易被「隔離」的點就是異常。

### 觀念

正常資料點彼此靠近，要切很多刀才能孤立一個點。
異常點孤零零的，幾刀就能隔開。

```
用隨機切割不斷分裂資料：

正常點（密集區）：需要切很多次才能孤立  → 路徑長 → 正常
異常點（稀疏區）：很快就被孤立           → 路徑短 → 異常
```

> [!tip] 直觀比喻
> 想像在人群裡隨機畫線找到某個人。
> 越容易被「圍起來」的人，越像異常值（獨自站在角落）。

### 適合場景

- 信用卡詐騙偵測
- 設備異常監控（感測器數值突然偏離）
- 網路入侵偵測
- 資料清洗（找 outlier）

### 程式碼

```python
from sklearn.ensemble import IsolationForest
import numpy as np

# 模擬資料：大部分正常，少數異常
X = np.random.randn(1000, 2)           # 正常資料
X_anomaly = np.array([[5, 5], [-5, 5], [5, -5]])  # 明顯異常

X_all = np.vstack([X, X_anomaly])

# 訓練（contamination = 預期異常比例）
model = IsolationForest(contamination=0.01, random_state=42)
model.fit(X_all)

# 預測：-1 = 異常，1 = 正常
labels = model.predict(X_all)
print("異常點數量:", (labels == -1).sum())

# 取得異常分數（越負越異常）
scores = model.decision_function(X_all)
```

---

## Random Forest（隨機森林）

> **一句話：** 建很多棵獨立的決策樹，讓它們投票決定結果。

### 觀念

單棵決策樹容易「記死」訓練資料（過擬合）。
解法：種很多棵「刻意不同」的樹，讓它們各自判斷，再取多數決。

```
「隨機」體現在兩個地方：
  1. 每棵樹用不同的資料子集訓練（Bootstrap）
  2. 每次分裂只看部分特徵（Feature Subsampling）

結果：每棵樹都有點不一樣，合起來反而更準
```

> [!tip] 直觀比喻
> 一個人可能判斷錯，但 100 個人投票很難全錯。
> 而且每個人看的資訊不完全相同，避免大家犯同樣的偏見。

### 適合場景

- 分類（垃圾郵件、疾病診斷）
- 回歸（房價預測）
- 特徵重要性分析（哪些變數最重要）
- 資料量中等、特徵數多的問題

### 程式碼

```python
from sklearn.ensemble import RandomForestClassifier
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split

X, y = load_iris(return_X_y=True)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

model = RandomForestClassifier(
    n_estimators=100,   # 樹的數量
    max_depth=5,        # 每棵樹最大深度（防過擬合）
    random_state=42
)
model.fit(X_train, y_train)
print("準確率:", model.score(X_test, y_test))

# 查看哪些特徵最重要
import pandas as pd
feature_importance = pd.Series(
    model.feature_importances_,
    index=load_iris().feature_names
).sort_values(ascending=False)
print(feature_importance)
```

---

## XGBoost（極限梯度提升）

> **一句話：** 樹接龍——每棵新樹專門修正上一棵的錯誤。

### 觀念

Random Forest 是「並行」的（樹之間互不影響）。
XGBoost 是「串行」的——每一棵新樹盯著前面樹的**殘差**（錯誤），只補那個缺口。

```
第 1 棵樹：做預測，留下殘差（預測值 - 真實值）
第 2 棵樹：學習第 1 棵的殘差
第 3 棵樹：學習第 1+2 棵合起來的殘差
...
最終預測 = 所有樹的輸出加總
```

> [!tip] 直觀比喻
> 一個學生（第 1 棵樹）考試考了 70 分，差 30 分。
> 第 2 個老師專門補那 30 分的不足，又補了 20 分。
> 第 3 個老師再補剩下的 10 分⋯⋯
> 每一輪都在縮小誤差。

> [!warning] Random Forest vs XGBoost
> - 資料小、快速驗證 → Random Forest（超參數少、不容易過擬合）
> - 追求最高準確率、有時間調參 → XGBoost（通常更準，但需要調 learning_rate、depth 等）

### 適合場景

- Kaggle 競賽的標準武器
- 結構化資料（表格資料）幾乎全勝
- 分類、回歸都適合

### 程式碼

```python
import xgboost as xgb
from sklearn.datasets import load_breast_cancer
from sklearn.model_selection import train_test_split

X, y = load_breast_cancer(return_X_y=True)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

model = xgb.XGBClassifier(
    n_estimators=100,
    learning_rate=0.1,    # 每棵樹的貢獻比例（越小越穩，需要更多樹）
    max_depth=4,
    use_label_encoder=False,
    eval_metric='logloss',
    random_state=42
)
model.fit(
    X_train, y_train,
    eval_set=[(X_test, y_test)],
    verbose=False
)
print("準確率:", model.score(X_test, y_test))

# 特徵重要性
xgb.plot_importance(model)
```

---

## PELT（變點偵測）

> **一句話：** 在時間序列裡找「行為突然改變」的時間點。

### 觀念

PELT（Pruned Exact Linear Time）是變點偵測（Changepoint Detection）演算法。
目標：把一段時間序列切成幾段，每段內部的統計特性（均值、方差）盡量一致。

```
輸入：一段時間序列
      [平穩] [突然升高] [再次平穩] [劇烈震盪]

輸出：切割點的位置
      t=120, t=340, t=510  ← 這些是「行為改變」的時刻
```

> [!tip] 直觀比喻
> 你在看一個人的心跳數據。
> 安靜時平均 70，運動後跳到 140，休息後回 75。
> PELT 找的就是「70 → 140 → 75」這幾個轉折點。

### 適合場景

- 感測器異常事件定位（「什麼時候開始出問題」）
- 股價趨勢切分
- 用戶行為模式改變偵測
- 訊號分段

### 程式碼

```python
import ruptures as rpt
import numpy as np
import matplotlib.pyplot as plt

# 模擬有 3 個變點的時間序列
np.random.seed(42)
signal = np.concatenate([
    np.random.normal(0, 1, 100),    # 段 1：均值 0
    np.random.normal(5, 1, 100),    # 段 2：均值 5（突然升高）
    np.random.normal(2, 3, 100),    # 段 3：均值 2，方差變大
])

# PELT 偵測（pen = 懲罰項，越大越少切點）
model = rpt.Pelt(model="rbf").fit(signal)
breakpoints = model.predict(pen=10)

print("偵測到的變點位置:", breakpoints)
# 輸出：[100, 200, 300]（最後一個是序列末端）

# 視覺化
rpt.display(signal, breakpoints)
plt.title("PELT 變點偵測")
plt.show()
```

> [!info] pen 參數（懲罰項）
> `pen` 值越大 → 切點越少（只找很顯著的改變）
> `pen` 值越小 → 切點越多（更敏感，容易過切）
> 沒有固定最佳值，依資料特性調整。

---

## LSTM（長短期記憶網路）

> **一句話：** 能記住「很久以前發生的事」對現在的影響的神經網路。

### 觀念

普通 RNN 理論上能處理序列，但實際上「遠的資訊會被稀釋」（梯度消失問題）。
LSTM 用三個「閘門」解決這個問題：

```
三個閘門：
  遺忘閘（Forget Gate）：決定丟掉哪些過去記憶
  輸入閘（Input Gate） ：決定接收哪些新資訊
  輸出閘（Output Gate）：決定輸出什麼

核心：Cell State（記憶帶）
  像一條傳送帶，重要的資訊可以不被改動地傳遞很遠
```

> [!tip] 直觀比喻
> 讀一篇小說，「第一章出現的角色名字」在第十章仍然重要。
> LSTM 的 cell state 就是一個「長期備忘錄」，不常改動但一直帶著。

### 適合場景

- 時間序列預測（股價、氣溫、用電量）
- 自然語言處理（翻譯、文字生成）
- 異常序列偵測（長期行為模式的偏離）
- 語音識別

### 程式碼

```python
import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense

# 模擬：用前 10 天預測第 11 天
def make_sequences(data, window=10):
    X, y = [], []
    for i in range(len(data) - window):
        X.append(data[i:i+window])
        y.append(data[i+window])
    return np.array(X), np.array(y)

# 造假資料（正弦波 + 噪音）
t = np.linspace(0, 100, 1000)
data = np.sin(t) + np.random.normal(0, 0.1, 1000)

X, y = make_sequences(data, window=10)
X = X.reshape(X.shape[0], X.shape[1], 1)  # (samples, timesteps, features)

# 建立 LSTM 模型
model = Sequential([
    LSTM(64, input_shape=(10, 1), return_sequences=False),
    Dense(1)
])
model.compile(optimizer='adam', loss='mse')

# 訓練
model.fit(X[:800], y[:800], epochs=10, batch_size=32, verbose=1)

# 預測
predictions = model.predict(X[800:])
```

> [!warning] LSTM 的限制
> 訓練慢、需要大量資料。
> 結構化時序預測任務，現在常被 **Transformer 架構** 取代（如 TFT、PatchTST）。
> 但理解 LSTM 仍是學習序列模型的重要基礎。

---

## HMM（隱馬可夫模型）

> **一句話：** 從你能觀測到的現象，推斷背後「看不見的狀態」。

### 觀念

HMM 有兩層：

```
隱藏層（你看不到）：  晴天 → 晴天 → 雨天 → 雨天
                         ↓        ↓       ↓       ↓
觀測層（你能看到）：  帶傘   不帶傘  帶傘   帶傘
```

三個核心參數：
- **轉移機率（A）**：隱藏狀態之間的跳轉機率（晴天 → 雨天的機率）
- **發射機率（B）**：隱藏狀態產生某個觀測值的機率（雨天時帶傘的機率）
- **初始機率（π）**：一開始在哪個狀態的機率

> [!tip] 直觀比喻
> 你在室內無法看窗外，但能觀察同事有沒有帶傘。
> 根據「帶傘序列」去推斷「今天到底是晴是雨」——這就是 HMM 做的事。

### 適合場景

- 語音識別（聲音 → 音素）
- 基因序列分析（觀測 DNA 序列 → 推斷功能區段）
- 金融市場狀態切換（牛市 / 熊市 / 震盪期）
- 用戶行為模式識別

### 程式碼

```python
from hmmlearn import hmm
import numpy as np

# 範例：市場狀態（牛市/熊市）推斷
# 觀測值為日報酬率（連續型 → 用 GaussianHMM）

np.random.seed(42)

# 模擬資料：
# 狀態 0 = 牛市（平均報酬 +0.5%，低波動）
# 狀態 1 = 熊市（平均報酬 -0.5%，高波動）
observations = np.concatenate([
    np.random.normal(0.5, 0.5, 200),   # 牛市
    np.random.normal(-0.5, 2.0, 100),  # 熊市
    np.random.normal(0.5, 0.5, 150),   # 牛市
]).reshape(-1, 1)

# 訓練（n_components = 隱藏狀態數量）
model = hmm.GaussianHMM(
    n_components=2,
    covariance_type="diag",
    n_iter=100,
    random_state=42
)
model.fit(observations)

# 推斷最可能的隱藏狀態序列（Viterbi 演算法）
states = model.predict(observations)
print("狀態序列（前 20）:", states[:20])
print("狀態均值:", model.means_.flatten())
print("轉移矩陣:\n", model.transmat_)
```

> [!info] GaussianHMM vs MultinomialHMM
> - `GaussianHMM`：觀測值是**連續數值**（股價、感測器讀數）
> - `MultinomialHMM`：觀測值是**離散類別**（文字、符號序列）

---

## 六個演算法一次比較

| 演算法 | 類型 | 輸入 | 輸出 | 最適場景 |
|---|---|---|---|---|
| **Isolation Forest** | 異常偵測 | 特徵向量 | 正常/異常 | 找稀疏的離群點 |
| **Random Forest** | 集成分類/回歸 | 特徵向量 | 分類/數值 | 通用表格資料 |
| **XGBoost** | 集成分類/回歸 | 特徵向量 | 分類/數值 | 追求最高準確率 |
| **PELT** | 時序變點偵測 | 時間序列 | 切割點位置 | 找行為突變時間點 |
| **LSTM** | 深度序列模型 | 時間序列 | 預測值/分類 | 長期依賴序列預測 |
| **HMM** | 機率生成模型 | 觀測序列 | 隱藏狀態 | 推斷不可見的狀態 |

### 同場景應該選哪個？

```
時間序列異常偵測：
  單點異常（某一刻突然異常）   → Isolation Forest
  行為突然改變（整段改變）     → PELT
  長期模式偏離（學過的正常模式突然不對）→ LSTM Autoencoder

時間序列預測：
  有明確的長期記憶依賴         → LSTM
  短期規律、狀態切換           → HMM
  特徵工程後的表格形式         → XGBoost

分類/回歸（表格資料）：
  快速 baseline               → Random Forest
  衝高準確率                  → XGBoost
  找異常樣本                  → Isolation Forest
```

---

## 安裝一覽

```bash
# 全部裝好
pip install scikit-learn xgboost ruptures tensorflow hmmlearn

# 或依需要單獨安裝
pip install scikit-learn    # Isolation Forest, Random Forest
pip install xgboost         # XGBoost
pip install ruptures        # PELT
pip install tensorflow      # LSTM
pip install hmmlearn        # HMM
```

---

## Sources

- [Scikit-learn：IsolationForest](https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.IsolationForest.html)
- [Scikit-learn：RandomForestClassifier](https://scikit-learn.org/stable/modules/generated/sklearn.ensemble.RandomForestClassifier.html)
- [XGBoost 官方文件](https://xgboost.readthedocs.io/)
- [ruptures：PELT 文件](https://centre-borelli.github.io/ruptures-docs/)
- [Understanding LSTM Networks — Colah's Blog](https://colah.github.io/posts/2015-08-Understanding-LSTMs/)
- [hmmlearn 官方文件](https://hmmlearn.readthedocs.io/)
- [原始論文：Isolation Forest（Liu et al., 2008）](https://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/icdm08b.pdf)

---

## 相關筆記

- [[AI 101 - Context Engineering]] — 把這些模型整合進 AI pipeline 時的 context 設計
- [[AI 101 - Harness Engineering]] — 模型外層的執行基礎設施
