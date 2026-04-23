---
title: AI 101 - PII Masking（隱私遮蔽）
tags: [ai, privacy, pii, dataset, nlp, gdpr, huggingface, offline]
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

## 資源需求

ai4privacy 依模式載入不同模型，資源消耗差異明顯：

| 模式 | 底層模型 | 參數量 | 磁碟大小 | 推論 RAM | CPU 延遲 |
|---|---|---|---|---|---|
| **英文**（預設）| DistilBERT fine-tuned | 66M | ~256 MB | ~700 MB | ~8 ms / 次 |
| **多語言** | mDeBERTa-v3-base | 276M | ~1.1 GB | ~1.5 GB | 慢 2–3x |
| **量化版**（自動）| DistilBERT quantized | 66M | **~43 MB** | ~300 MB | 略慢 |

> [!tip] 一般筆電完全夠用
> 英文模式只需 700 MB RAM，CPU 單次推論約 8ms。
> 不需要 GPU——但若有 GPU 可加速約 5–10x。

> [!info] 為什麼多語言差這麼多？
> mDeBERTa 的詞彙表從 128K 擴展到 250K tokens（支援 100 種語言），
> Embedding 層參數從 98M 暴增到 190M，導致整體大小接近 DistilBERT 的 4 倍。

### GPU 加速（可選）

```python
from ai4privacy import protect

# 預設 CPU
masked = protect(text)

# 指定用 GPU（需安裝 CUDA 版 PyTorch）
masked = protect(text, use_gpu=True)
```

### 實際佔用確認

```python
import os, psutil, time
from ai4privacy import protect

process = psutil.Process(os.getpid())

before = process.memory_info().rss / 1024 / 1024
protect("warm up call to load model")   # 第一次呼叫才載入模型
after = process.memory_info().rss / 1024 / 1024

print(f"模型載入後增加：{after - before:.0f} MB")

# 計算單次推論時間
start = time.perf_counter()
protect("Email me at test@example.com or call +886-912-345-678")
elapsed = (time.perf_counter() - start) * 1000
print(f"推論耗時：{elapsed:.1f} ms")
```

---

## 離線環境使用

> [!info] 為什麼需要這一節？
> `pip install ai4privacy` 只裝 Python 程式碼（幾 KB）。
> **模型本體（幾百 MB）是第一次呼叫 `protect()` 時才從 HuggingFace 下載**，並快取到 `~/.cache/huggingface/`。
> 若部署環境沒有網路，需要事先在有網路的機器預下載。

### Step 1：找出 ai4privacy 使用的模型 ID

```bash
# 找安裝位置，看 source code 裡的 model name
cat $(python3 -c "import ai4privacy; print(ai4privacy.__file__)")

# 或者：在有網路時跑一次，看 cache 裡多了什麼
ls ~/.cache/huggingface/hub/
# 會出現類似 models--ai4privacy--XXXXXXX 的資料夾
```

### Step 2：主動預下載模型（不需要呼叫 protect）

```bash
# 方法 A：CLI（最直接）
pip install huggingface_hub
huggingface-cli download ai4privacy/<model-id>
```

```python
# 方法 B：寫成 setup script，有網路時跑一次
from huggingface_hub import snapshot_download

snapshot_download(repo_id="ai4privacy/<model-id>")
print("模型已下載，可離線使用")
```

### Step 3：離線環境鎖定不連網

```bash
# 設環境變數，強制只讀 cache，完全不嘗試連線
export HF_HUB_OFFLINE=1
python3 your_script.py
```

```python
# 或在程式碼裡設定
import os
os.environ["HF_HUB_OFFLINE"] = "1"

from ai4privacy import protect
print(protect("Email: john@example.com"))
# 直接讀 cache，不聯網
```

> [!tip] 搬移到隔離環境
> `~/.cache/huggingface/` 整個目錄可以直接複製到沒有網路的機器。
> 模型 cache 是可攜的，複製過去後設 `HF_HUB_OFFLINE=1` 即可。

> [!warning] 不設 HF_HUB_OFFLINE 的風險
> 即使 cache 存在，沒設這個變數時 HuggingFace 仍會嘗試連線確認版本。
> 離線環境不設的話可能 timeout 卡住幾秒甚至報錯。

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
