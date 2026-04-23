---
title: AI 101 - PII Masking（隱私遮蔽）
tags: [ai, privacy, pii, dataset, nlp, gdpr, huggingface]
created: 2026-04-23
updated: 2026-04-23
---

# PII Masking — 自動偵測與遮蔽個人資料

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> **pii-masking-300k** 是目前最大的開源 PII（個人識別資訊）遮蔽資料集。
> 同名的 Python 套件可以讓你**三行程式碼**就對任意文字做隱私遮蔽。

**PII（Personally Identifiable Information）個人識別資訊**：姓名、電話、email、身分證字號、地址等一切能識別特定個人的資料。

---

## 兩種使用方式

| 方式 | 適合誰 | 難度 |
|---|---|---|
| **① 直接用 Python 套件** | 想立刻遮蔽文字、不想自己訓練模型 | ⭐ 簡單 |
| **② 載入資料集自行訓練** | 想訓練自己的 NER / PII 偵測模型 | ⭐⭐⭐ 進階 |

---

## ① 快速上手：ai4privacy Python 套件

### 安裝

```bash
pip install ai4privacy
```

### 基本用法：遮蔽 PII

```python
from ai4privacy import protect

text = "請聯絡 alice@example.com 或撥打 +886-912-345-678"
masked = protect(text)
print(masked)
# 輸出：請聯絡 [PII_1] 或撥打 [PII_2]
```

### 觀察模式：分析但不改動文字

```python
from ai4privacy import observe
import json

text = "My name is Alice and I live in Berlin."
report = observe(text, classify_pii=True)
print(json.dumps(report, indent=2))
```

輸出範例：
```json
{
  "text_count": 1,
  "pii_entity_count": 2,
  "privacy_mask": [
    {
      "label": "GIVENNAME",
      "value": "Alice",
      "start": 11,
      "end": 16,
      "score": 0.98
    },
    {
      "label": "CITY",
      "value": "Berlin",
      "start": 31,
      "end": 37,
      "score": 0.95
    }
  ]
}
```

### 顯示 PII 類別標籤

```python
from ai4privacy import protect

text = "My name is John Smith, email: john@corp.com"
result = protect(text, classify_pii=True, verbose=True)

# 取得替換明細
for r in result['replacements']:
    print(r['label'], r['value'])
# GIVENNAME John
# LASTNAME  Smith
# EMAIL     john@corp.com
```

### 多語言支援

```python
from ai4privacy import protect

# 法文
text = "Je m'appelle Pierre et j'habite à Paris."
masked = protect(text, multilingual=True)
print(masked)
# Je m'appelle [PII_1] et j'habite à [PII_2]
```

支援語言：英文（英/美）、法文、德文、義大利文、荷蘭文、西班牙文

### 調整靈敏度

```python
text = "Contact John for details."

# 高精確（少報）：score_threshold 調高
masked_precise = protect(text, score_threshold=0.5)

# 高靈敏（多報）：score_threshold 調低
masked_sensitive = protect(text, score_threshold=0.001)
```

> [!tip] 預設值 `score_threshold=0.01`
> 值越低，越多文字會被判定為 PII（寧可錯殺）。
> 值越高，只有非常確定的才會遮蔽（準確但可能漏掉）。
> 隱私需求高的場景建議降低 threshold。

---

## ② 載入資料集：用於訓練模型

### 安裝 datasets

```bash
pip install datasets
```

### 載入資料集

```python
from datasets import load_dataset

# 載入完整資料集（約 225K 筆）
ds = load_dataset("ai4privacy/pii-masking-300k")

# 只看訓練集
train = ds["train"]
print(train[0])
```

### 資料集欄位說明

```python
# 每一筆資料包含：
{
  "source_text":       # 原始文字（含 PII）
  "target_text":       # 遮蔽後文字（PII 換成 [LABEL]）
  "privacy_mask":      # PII 實體清單（含位置、標籤）
  "span_labels":       # 座標標籤 [[start, end, "LABEL"], ...]
  "mbert_text_tokens": # 分詞後的 token 序列（給 mBERT 用）
  "mbert_bio_labels":  # BIO 格式標籤（B-開始、I-內部、O-非PII）
  "language":          # 語言代碼（en, fr, de, it, nl, es）
  "id":                # 唯一識別碼
}
```

### 實際資料長什麼樣

```python
example = train[0]
print("原文:", example["source_text"][:100])
print("遮蔽:", example["target_text"][:100])
print("PII清單:", example["privacy_mask"])
```

輸出：
```
原文: Hi wynqvrh053, meeting at 10:20am on 2024-03-15
遮蔽: Hi [USERNAME], meeting at [TIME] on [DATE]
PII清單: [
  {"value": "wynqvrh053", "start": 3, "end": 13, "label": "USERNAME"},
  {"value": "10:20am",    "start": 26, "end": 33, "label": "TIME"},
  {"value": "2024-03-15", "start": 37, "end": 47, "label": "DATE"}
]
```

### 篩選特定語言

```python
# 只取英文資料
en_data = train.filter(lambda x: x["language"] == "en")
print(f"英文筆數：{len(en_data)}")
```

### 用 BIO 標籤做 NER 訓練

```python
from transformers import AutoTokenizer, AutoModelForTokenClassification

# 資料集附有 mBERT tokenizer 對齊的 BIO 標籤
# 可以直接拿來 fine-tune token classification 模型
tokenizer = AutoTokenizer.from_pretrained("bert-base-multilingual-cased")

example = train[0]
print("tokens:", example["mbert_text_tokens"][:10])
print("labels:", example["mbert_bio_labels"][:10])
# B-USERNAME, I-USERNAME, O, O, B-TIME, ...
```

---

## 資料集規格

| 項目 | 數值 |
|---|---|
| 總筆數 | 約 225,000 筆 |
| 訓練集 / 驗證集 | 178K / 47.7K（79% / 21%）|
| 總 tokens | 3,040 萬 |
| PII tokens | 760 萬 |
| PII 類別數 | 27 類（OpenPII）+ 約 20 類（FinPII）|
| 支援語言 | 6 種 |
| 資料品質 | 人工驗證，標籤準確率 ~98.3% |
| 資料性質 | **合成資料**（非真實個資，無隱私疑慮）|

### 27 種 PII 類別（OpenPII）

`USERNAME` `EMAIL` `PHONE` `DATE` `TIME` `GIVENNAME` `LASTNAME`
`CITY` `STREET` `ZIPCODE` `STATE` `COUNTRY` `IP` `URL`
`PASSPORT` `ID_CARD` `SSN` `CREDIT_CARD` `IBAN` `AGE`
`GENDER` `ORGANIZATION` `JOBTYPE` `NATIONALITY` `RELIGION`
`HEALTH_RELATED` `MARITAL_STATUS`

---

## 常見應用場景

> [!tip] 最快的路
> 不需要自己訓練模型時，直接用 `ai4privacy` 套件就好。
> 資料集主要是給「想訓練自己 PII 偵測模型」的人用的。

| 應用 | 建議做法 |
|---|---|
| 把 log / email 裡的個資遮掉再送給 LLM | `protect()` 套件 |
| GDPR 合規——上線前過濾使用者輸入 | `observe()` + 條件攔截 |
| 訓練自己的 PII NER 模型 | 載入資料集，fine-tune BERT |
| 評估現有 PII 偵測系統 | 用資料集的 validation split 當 benchmark |
| 多語言客服系統個資過濾 | `protect(text, multilingual=True)` |

---

## 常見問題

> [!warning] 套件 vs 資料集
> `pip install ai4privacy` 裝的是**推論套件**（已訓練好的模型）。
> `load_dataset("ai4privacy/pii-masking-300k")` 載入的是**訓練資料**。
> 兩者是不同的東西，依需求選擇。

> [!warning] 準確率不是 100%
> 套件文件標示準確率 ~98.3%，仍有漏偵測的風險。
> 高風險場景（醫療、金融）建議人工二次確認，或降低 `score_threshold`。

> [!info] FinPII 子資料集
> 金融 / 保險專用的 FinPII-80k 需要聯絡 `licensing@ai4privacy.com` 取得授權。
> 一般場景用 OpenPII 即可。

---

## Sources

- [ai4privacy/pii-masking-300k — Hugging Face](https://huggingface.co/datasets/ai4privacy/pii-masking-300k)
- [ai4privacy Python 套件 — PyPI](https://pypi.org/project/ai4privacy/)
- [PII Masking with Python Using ai4privacy — Medium](https://heshanhfernando.medium.com/pii-masking-with-python-using-ai4privacy-12279bf7312a)
- [Unmasking the Reality of PII Masking Models — arXiv](https://arxiv.org/pdf/2504.12308)

---

## 相關筆記

- [[AI 101 - 核心概念]] — LLM 與 AI 基礎
- [[AI 101 - Harness Engineering]] — 把 PII 過濾整合進 agent 的護欄層
