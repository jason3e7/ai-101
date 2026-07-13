---
title: "JT Doc Tools 去識別化實作解析：regex、PDF 座標、LLM 補偵測"
tags: [ai, 外部觀點, 資安, pii, pdf, regex, pymupdf, llm, 去識別化, 源碼分析]
source: https://github.com/jasoncheng7115/jt-doc-tools
author: jasoncheng7115
created: 2026-05-21
---

# JT Doc Tools 去識別化實作解析：regex、PDF 座標、LLM 補偵測

> [!info]
> 這篇是 [[JT Doc Tools：自架文件處理平台（36 工具 + 本地 AI）— jasoncheng7115]] 的技術深潛，
> 直接讀原始碼（`doc_deident/` 和 `text_deident/`），分析兩個去識別化工具的實作細節。

---

## 兩個工具的定位

| 工具 | 輸入 | 核心處理 |
|---|---|---|
| **文件去識別化**（`doc_deident`）| PDF / Office 文件 | PDF 座標系，bbox 定位後黑塊或文字替換 |
| **文字去識別化**（`text_deident`）| 純文字 / .txt / .md | 字元索引 span 追蹤，支援多種輸出模式 |

兩者共用同一套 `patterns.py` 偵測規則，但後處理完全不同。

---

## patterns.py：40+ 個 regex 分六類

### 六大偵測類別

| 類別 | 涵蓋內容 |
|---|---|
| **個人身分** | 身分證、居留證、護照、駕照、健保卡、生日 |
| **聯絡方式** | 手機、市話、Email、郵寄地址 |
| **金融資訊** | 信用卡、銀行帳號、分行代碼、帳戶名稱 |
| **企業資料** | 統編、公司名稱、電子發票號碼、採購單號 |
| **其他** | 姓名、車牌、VIN、GPS 座標、航班號、PNR 訂位碼 |
| **IT 資料** | Hostname、MAC、AD DN、Windows 網域帳號、UUID、URL、API Token、SSH key、PEM block、密碼欄位、檔案路徑 |

> [!warning] IT 類別預設關閉
> IT 相關 pattern 預設不啟用，因為商業文件裡大量出現 IP、路徑、UUID 反而會造成大量誤判。
> 使用者需手動開啟。

### 格式比對 + 校驗雙層設計

regex 過了還要通過 validator 才算命中：

| 資料類型 | 校驗方式 |
|---|---|
| 台灣身分證 | 字母轉數字後加權模 10 驗證 |
| 信用卡 | Luhn 演算法 |
| 統編（8 碼）| 位權加總規則，第 7 碼有特殊處理 |
| 其他大多數 | `_always()` — 只做格式比對，不額外校驗 |

---

## 文件去識別化（`doc_deident`）深潛

### PDF 解析：PyMuPDF 字元座標

用 PyMuPDF（`fitz`）把 PDF 解成帶座標的字典格式，保留每個字元對應的 span 和 bounding box。

偵測流程（`_build_findings_for_page()`）：

```
每一頁
  └── 取出所有 line / span / char
        └── 把 spans 拼成完整字串，同時記錄每個字元的 span 索引
              └── 跑 patterns.py 的 regex
                    └── 命中後反查 span，聯集所有相關 span 的 bbox
                          └── 單 span 命中的 bbox 做字元寬度修正（避免框太大）
                                └── 記錄 {type, value, masked, bbox, text}
```

### Redact vs Mask：兩種不同處理策略

| | Redact（刪除）| Mask（遮蔽）|
|---|---|---|
| **PyMuPDF 呼叫** | `page.add_redact_annot(bbox, fill=(0,0,0))` | 透明 redact annot + 重新插入文字 |
| **結果** | 黑色實心塊，不可還原 | 視覺上替換為遮蔽字串 |
| **字型選擇** | — | 偵測到 CJK 用 `china-t`，否則用 `helv` |
| **可還原？** | 否，從 content stream 永久移除 | 否，原始文字已移除，插入的是替代字串 |

### Mask 的替代字串設計（保留語意結構）

| 資料類型 | 遮蔽後格式 |
|---|---|
| 身分證 | 前 2 碼 + `****` + 後 2 碼 |
| 手機 | 前 4 碼 + `***` + 後 3 碼 |
| Email | `***@domain.com`（保留 domain）|
| 地址 | `OO市OO區OO路OOO號` |
| SSH key / PEM block | 整段替換 |

### Office 文件

先透過 `office_convert` 模組（呼叫 LibreOffice/OxOffice）轉成 PDF，後續流程與 PDF 相同。

---

## 文字去識別化（`text_deident`）深潛

### 輸入處理

| 格式 | 處理方式 |
|---|---|
| `.txt` / `.md` | 依序嘗試 UTF-8 → UTF-8-sig → Big5 → CP950 → Latin-1 |
| `.pdf` | PyMuPDF 逐頁抽文字 |
| `.docx` / `.odt` 等 | `soffice` 轉純文字 |

### 字元 Span 追蹤

不是 PDF 座標，而是記 `[start, end)` 字元索引。

**去重邏輯**：同類型命中中，如果一個 span 完全被另一個包住，丟掉短的，只保留較長的那個。

### 三種輸出模式

| 模式 | 效果 |
|---|---|
| **Redact** | 用 `█` 字元填滿原始字元數 |
| **Mask** | 用 `*` 遮蔽 |
| **Fake** | 插入格式正確的合成假資料（非真實值）|

> [!tip] Fake 模式的用途
> 適合需要「維持文件格式可讀性」但又不能留真實資料的場景，
> 例如：測試環境的資料匿名化、示範用文件。

---

## LLM 補偵測（兩個工具都有，預設關閉）

### 目的

regex 只能抓**已知格式**，但語意型敏感詞（職稱後的姓名、內部代號、合約編號）它抓不到。

LLM 補偵測負責找：
- 職稱 + 姓名（「主管 王經理」）
- 客戶代號（「A-2024-0815」）
- 採購單 / 合約編號
- 內部地點名稱
- 公司名稱
- 航班號

### 流程

```
截取前 8,000 字元（避免 timeout / 費用過高）
  └── 送 LLM：找「regex 容易漏掉的語意型敏感詞」
        └── 告知「這些已被 regex 找到，不要重複」
              └── 要求回傳純 JSON，不加 markdown 包裝
                    └── 格式：[{"text": "...", "type": "category"}]
```

### 兩個工具的 span 解析差異

| | doc_deident | text_deident |
|---|---|---|
| **拿到詞後** | `document.search_for()` 在全文找 bbox（PDF 座標）| `_attach_llm_spans()` 轉字元索引 |
| **重複處理** | 標記 `source: "llm"` 避免與 regex 結果衝突 | 跳過已被 regex 命中的範圍 |

### 容錯設計

LLM 呼叫失敗時**靜默 fallback** 到純 regex 結果，只寫 log，不向使用者報錯。

---

## 整體架構小結

```
patterns.py（共用）
  ├── 40+ regex patterns（6 類）
  └── 雙層校驗（格式 + checksum）

doc_deident
  ├── PyMuPDF 解 PDF → 字元 + bbox
  ├── regex 命中 → 反查 span → bounding box
  ├── LLM 補偵測 → search_for() 找 bbox
  └── Redact（黑色實心）or Mask（替代字串插回）

text_deident
  ├── 多編碼輸入（UTF-8 / Big5 / CP950）
  ├── regex 命中 → [start, end) 字元索引
  ├── LLM 補偵測 → 字元索引 span
  └── Redact（█）/ Mask（*）/ Fake（合成假資料）
```

> [!tip] 兩者的核心差異
> `doc_deident` 難的地方在**座標系**——要知道文字在 PDF 哪個位置才能畫黑塊或插文字。
> `text_deident` 難的地方在**編碼和 span 管理**——純文字沒有座標，靠字元索引追蹤，多一種 Fake 模式。

---

## 相關筆記

- [[JT Doc Tools：自架文件處理平台（36 工具 + 本地 AI）— jasoncheng7115]] — 工具總覽
- [[AI 101 - PII Masking（隱私遮蔽）]] — ai4privacy：NLP 模型偵測 PII
- [[AI 101 - OpenAI Privacy Filter]] — OpenAI 官方 PII 模型
- [[auto-sanitize：Git commit 前自動遮蔽敏感資料 — kerr20801]] — 程式碼的去識別化

---

## 來源

- 原始碼：[github.com/jasoncheng7115/jt-doc-tools](https://github.com/jasoncheng7115/jt-doc-tools)
- `app/tools/doc_deident/patterns.py`
- `app/tools/doc_deident/router.py`
- `app/tools/text_deident/router.py`
