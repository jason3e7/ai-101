---
title: AI 101 - OpenAI Privacy Filter
tags: [ai, privacy, pii, openai, nlp, gdpr, huggingface, offline, redaction]
created: 2026-05-13
updated: 2026-05-13
---

# OpenAI Privacy Filter — 本地 PII 偵測與遮蔽

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> HuggingFace：[openai/privacy-filter](https://huggingface.co/openai/privacy-filter)
> GitHub：[openai/privacy-filter](https://github.com/openai/privacy-filter)
> 授權：Apache 2.0（可商用）
> **一句話：** OpenAI 於 2026 年 4 月發布的開源 PII 偵測模型，完全在本地執行，F1 達 97%，支援 128K context。

---

## 是什麼

**Privacy Filter** 是 OpenAI 發布的開放權重（open-weight）PII 偵測模型，設計給需要在把文字送進 LLM **之前**先過濾個資的場景。

**關鍵特色：**

| 特色 | 說明 |
|---|---|
| **完全本地執行** | 個資不離開你的機器 |
| **輕量** | 1.5B 總參數，推論時只用 50M 個 active 參數 |
| **高準確率** | F1 97.43%（修正標注雜訊後）|
| **超長 context** | 128,000 token 上下文視窗 |
| **單次 forward pass** | 非自回歸，速度快、適合高吞吐量 |

> [!tip] 跟 ai4privacy 的差異
> - **ai4privacy**：社群開源資料集 + 套件，支援 50+ 種 PII 類別，主力在資料集
> - **OpenAI Privacy Filter**：OpenAI 官方發布，8 種核心類別，著重部署效能與本地隱私

---

## 偵測的 8 種 PII 類別

| 標籤 | 偵測內容 | 範例 |
|---|---|---|
| `private_person` | 姓名 | "Jane Doe"、"Dr. Smith" |
| `private_address` | 地址、郵遞區號 | "742 Evergreen Terrace" |
| `private_email` | Email | "jane@example.com" |
| `private_phone` | 電話號碼 | "+1-555-867-5309" |
| `private_url` | 含身份識別的 URL | "linkedin.com/in/janedoe" |
| `private_date` | 出生日期、個人日期 | "born March 15, 1990" |
| `account_number` | 身分證、銀行帳號、SSN | "SSN: 123-45-6789" |
| `secret` | API key、密碼、token | "sk-proj-abc123…" |

---

## 安裝

### 方法一：transformers（最簡單）

```bash
pip install transformers torch
```

模型第一次使用時自動從 HuggingFace 下載（約 3GB）。

### 方法二：官方 CLI 工具（`opf`）

```bash
git clone https://github.com/openai/privacy-filter
cd privacy-filter
pip install -e .
```

---

## 快速上手

### Pipeline（三行上手）

```python
from transformers import pipeline

detector = pipeline(
    task="token-classification",
    model="openai/privacy-filter",
    aggregation_strategy="simple"
)

text = "請聯繫 Jane Doe，email：jane@example.com，電話：0912-345-678。"
results = detector(text)

for entity in results:
    print(f"{entity['entity_group']}: {entity['word']!r}  (信心度 {entity['score']:.4f})")
```

**輸出：**

```
private_person: ' Jane Doe'  (信心度 0.9999)
private_email: ' jane@example.com'  (信心度 1.0000)
private_phone: ' 0912-345-678'  (信心度 0.9998)
```

---

## 實用場景：遮蔽 + 還原

> [!tip] 送 LLM 前過濾，回傳後還原

```python
from transformers import pipeline

detector = pipeline(
    task="token-classification",
    model="openai/privacy-filter",
    aggregation_strategy="simple"
)

def mask_with_mapping(text: str) -> tuple[str, dict]:
    """遮蔽 PII，回傳遮蔽後文字與還原對照表"""
    entities = detector(text)
    mapping = {}
    masked = text

    # 從後往前替換，避免 offset 位移
    for i, ent in enumerate(sorted(entities, key=lambda e: e["start"], reverse=True)):
        placeholder = f"[{ent['entity_group'].upper()}_{i}]"
        original = text[ent["start"]:ent["end"]]
        mapping[placeholder] = original
        masked = masked[:ent["start"]] + placeholder + masked[ent["end"]:]

    return masked, mapping

def restore(masked_text: str, mapping: dict) -> str:
    """用對照表還原原始文字"""
    result = masked_text
    for placeholder, original in mapping.items():
        result = result.replace(placeholder, original)
    return result

# 使用範例
original = "請聯繫 Jane Doe，她的 email 是 jane@example.com。"
masked, mapping = mask_with_mapping(original)

print(f"遮蔽後：{masked}")
# → 請聯繫 [PRIVATE_PERSON_1]，她的 email 是 [PRIVATE_EMAIL_0]。

# 送給 LLM 處理
llm_response = f"已轉達給 [PRIVATE_PERSON_1]，將回覆至 [PRIVATE_EMAIL_0]。"

# 還原
final = restore(llm_response, mapping)
print(f"還原後：{final}")
# → 已轉達給 Jane Doe，將回覆至 jane@example.com。
```

---

## CLI 工具（`opf`）

安裝後可以直接在終端使用：

```bash
# 遮蔽單句
opf "Alice was born on 1990-01-02 and lives at 123 Main St."

# 遮蔽整個檔案
opf -f /path/to/document.txt

# 指定用 CPU（不用 GPU）
opf --device cpu "Call me at 0912-345-678"

# Pipeline 用法（grep 過濾 → 遮蔽）
cat document.txt | grep "聯絡" | opf

# 互動模式（無參數啟動，輸出 JSON）
opf
```

---

## 離線使用

```python
# 第一步：連網下載模型（只需一次）
from transformers import pipeline
detector = pipeline("token-classification", model="openai/privacy-filter")
# 模型存在 ~/.cache/huggingface/

# 第二步：之後可完全離線，設環境變數防止嘗試連網
import os
os.environ["HF_HUB_OFFLINE"] = "1"

from transformers import pipeline
detector = pipeline("token-classification", model="openai/privacy-filter")
# 直接讀取本地快取，不會連網
```

---

## 進階：直接用 AutoModel

需要取得 logits 或自訂解碼時使用：

```python
import torch
from transformers import AutoModelForTokenClassification, AutoTokenizer

tokenizer = AutoTokenizer.from_pretrained("openai/privacy-filter")
model = AutoModelForTokenClassification.from_pretrained(
    "openai/privacy-filter",
    device_map="auto"  # 自動選 GPU / CPU
)

text = "My name is Harry Potter and my email is harry.potter@hogwarts.edu."
inputs = tokenizer(text, return_tensors="pt").to(model.device)

with torch.no_grad():
    outputs = model(**inputs)

predicted_ids = outputs.logits.argmax(dim=-1)
predicted_labels = [model.config.id2label[t.item()] for t in predicted_ids[0]]
print(predicted_labels)
```

---

## 資源需求

| 項目 | 需求 |
|---|---|
| **推論 RAM** | ~800MB–1.5GB（50M active 參數）|
| **GPU VRAM** | 非必須，CPU 也可跑 |
| **模型下載大小** | ~3GB（BF16）|
| **Context 上限** | 128,000 tokens |
| **作業系統** | Linux / macOS / Windows |

> [!tip] 跑在瀏覽器也行
> 官方支援 Transformers.js + WebGPU，可以在瀏覽器端本地執行（量化為 q4）。

---

## 效能指標

| 指標 | 數值 |
|---|---|
| F1（PII-Masking-300k）| 96.0% |
| F1（修正標注雜訊後）| 97.43% |
| Precision | 96.79% |
| Recall | 98.08% |
| vs. Regex 方案 | Regex 約 60–80% F1，相差 15–35% |

---

## 常見問題

> [!warning] `secret` 類別召回率較低
> API key、密碼格式太多樣，模型在這個類別的 recall 偏低。
> 建議搭配規則（regex）補強 secrets 的偵測。

> [!warning] 主要訓練語言是英文
> 中文、日文等非英語 PII 偵測效果會下降。
> 中文場景建議先評估準確率，或使用 domain fine-tune。

> [!info] 不是合規認證，是工具
> Privacy Filter 是偵測層，不是 GDPR / CCPA 合規保證。
> 高風險場景仍需人工審查。

---

## Sources

- [Introducing OpenAI Privacy Filter — OpenAI](https://openai.com/index/introducing-openai-privacy-filter/)
- [openai/privacy-filter — HuggingFace](https://huggingface.co/openai/privacy-filter)
- [openai/privacy-filter — GitHub](https://github.com/openai/privacy-filter)
- [OpenAI Privacy Filter Guide — AIMadeTools](https://www.aimadetools.com/blog/openai-privacy-filter-guide/)
- [OpenAI Privacy Filter — MarkTechPost](https://www.marktechpost.com/2026/04/28/openai-releases-privacy-filter-a-1-5b-parameter-open-source-pii-redaction-model-with-50m-active-parameters/)
- [OpenAI Privacy Filter — The New Stack](https://thenewstack.io/openai-privacy-filter-pii/)

---

## 相關筆記

- [[AI 101 - PII Masking（隱私遮蔽）]] — ai4privacy 套件（50+ 類別，社群開源）
- [[AI 101 - Context Engineering]] — 如何設計送 LLM 前的過濾流程
