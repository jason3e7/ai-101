---
title: "quote-shield：報價文件敏感資料遮蔽工具"
tags: [ai, 外部觀點, 資安, pii, 報價單, xlsx, pdf, 瀏覽器工具, python]
source: https://github.com/kerr20801/quote-shield
author: kerr20801
created: 2026-05-20
---

# quote-shield：報價文件敏感資料遮蔽工具

> [!info]
> GitHub：[kerr20801/quote-shield](https://github.com/kerr20801/quote-shield)
> **一句話：** 在瀏覽器裡把 XLSX / CSV / PDF 報價單的敏感欄位遮掉再寄出，資料完全不離開你的電腦。

---

## 是什麼

**quote-shield** 是一個純瀏覽器工具，
解決的問題是：報價單裡通常同時含有**對外資訊**（給客戶看的）和**對內資訊**（成本、利潤、內部備註），
寄給客戶前必須手動刪欄位，既麻煩又容易漏。

quote-shield 讓你上傳檔案、選好遮蔽設定、預覽，然後下載乾淨版本。

**技術核心：單一 HTML 檔，無後端、無伺服器、資料不上傳。**

---

## 支援格式

| 格式 | 說明 |
|---|---|
| **XLSX** | 試算表報價單，支援工作表密碼保護 |
| **CSV** | 純文字逗號分隔檔 |
| **PDF** | 可標記「機密文件」浮水印 |

---

## 六種遮蔽類型

| 類型 | 說明 |
|---|---|
| 客戶名稱 | Client names |
| 報價金額 | Quote amounts |
| 成本／利潤 | Cost / profit data |
| 內部備註 | Internal remarks |
| 供應商資訊 | Supplier details |
| 業務員資訊 | Salesperson information |

---

## 四種收件人預設

不同對象需要隱藏不同資訊，quote-shield 內建四組快速設定：

| 預設 | 適合情境 |
|---|---|
| **Customer（客戶）** | 隱藏成本、利潤、內部備註、供應商 |
| **Internal（內部共享）** | 保留所有資訊 |
| **Partner（合作夥伴）** | 介於客戶與內部之間 |
| **Custom（自訂）** | 逐項手動選擇 |

---

## 使用流程

```
1. 用瀏覽器開啟 quote-shield（單一 HTML 檔）
2. 上傳 XLSX / CSV / PDF
3. 選擇收件人預設（或自訂遮蔽項目）
4. 設定選項：
   - 檔名格式（加日期、時間、收件人姓名、自訂文字）
   - XLSX：可加工作表密碼保護
   - PDF：可標記機密浮水印
5. 預覽（遮蔽版 / 原始版 / 並排比較）
6. 下載乾淨檔案
```

---

## 技術實作

| 元件 | 說明 |
|---|---|
| [SheetJS](https://sheetjs.com/) | XLSX / CSV 處理 |
| [PDF.js](https://mozilla.github.io/pdf.js/) | PDF 預覽 |
| [pdf-lib](https://pdf-lib.js.org/) | PDF 修改與標記 |

三個函式庫皆透過 CDN 載入，整個應用就是一個 HTML 檔。

---

## 與 auto-sanitize 的關係

同一個作者（kerr20801）的兩個互補工具：

| | [auto-sanitize](https://github.com/kerr20801/auto-sanitize) | quote-shield |
|---|---|---|
| **目標** | 程式碼、設定檔 | 商業報價文件 |
| **格式** | .py、.env、任意文字檔 | XLSX、CSV、PDF |
| **觸發方式** | Git pre-commit hook / CLI | 瀏覽器手動操作 |
| **遮蔽邏輯** | 格式辨識（IP、token prefix） | 欄位類型（成本、備註） |
| **使用場景** | 開發者防止 token 洩漏 | 業務人員寄報價單前清理 |

---

## 相關筆記

- [[auto-sanitize：Git commit 前自動遮蔽敏感資料 — kerr20801]] — 同作者，針對程式碼的 PII 遮蔽
- [[AI 101 - PII Masking（隱私遮蔽）]] — ai4privacy 套件，自然語言 PII 偵測
- [[AI 101 - OpenAI Privacy Filter]] — OpenAI 官方 PII 模型

---

## 來源

- GitHub：[kerr20801/quote-shield](https://github.com/kerr20801/quote-shield)
