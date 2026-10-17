---
title: "為什麼 Claude 在對話中切換了模型"
tags: [ai, 外部觀點, claude, fable-5, safety, model-switching, classifier, anthropic]
source: https://support.claude.com/zh-TW/articles/15363606-%E7%82%BA%E4%BB%80%E9%BA%BC-claude-%E5%9C%A8%E6%82%A8%E8%88%87-fable-5-%E7%9A%84%E5%B0%8D%E8%A9%B1%E4%B8%AD%E5%88%87%E6%8F%9B%E4%BA%86%E6%A8%A1%E5%9E%8B
author: Anthropic
created: 2026-06-11
---

# 為什麼 Claude 在對話中切換了模型

> [!info]
> 來源：[為什麼 Claude 在您與 Fable 5 的對話中切換了模型](https://support.claude.com/zh-TW/articles/15363606)（Anthropic Help Center）
> 更新：2026-06-10
> **一句話：** Fable 5 內建三類安全分類器（網路安全、生物科學、模型蒸餾），觸發時自動切換到 Opus 4.8 並通知使用者——設計刻意保守，合法工作也可能被擋，但有申請例外的管道。

---

## 為什麼要切換模型

Fable 5 的能力在某些領域（網路安全、生物科學）強到可能造成大規模危害（大型網路攻擊、生化武器）。為了降低風險，特定請求會被攔截並交由 Opus 4.8 執行。

---

## 三類被攔截的請求

| 類別 | 範圍 |
|---|---|
| **攻擊性網路安全技術** | exploit、惡意軟體、攻擊工具 |
| **生物科學與生命科學查詢** | 實驗室方法、分子機制 |
| **模型「摘要思考」的提取** | 防止模型蒸餾（anti-distillation）|

> [!warning] 設計刻意偏保守
> Anthropic 明確承認，安全護欄設計上刻意寬泛，**會攔截合法工作**，包括：
> - 授權的滲透測試
> - 醫療診斷
> - 生物技術文件
> - 教育性生物學內容

---

## 切換機制如何運作

1. 分類器掃描請求（包含整個對話內容：使用者訊息、記憶、connector 內容、網路搜尋結果、上傳檔案）
2. 觸發時，自動用 Opus 4.8 重新執行該請求
3. 通知使用者已切換

**適用平台：** Claude 網頁版、手機版、桌面版、Cowork、Code、Design、Microsoft 365、Teams、Slack

**API：** 不自動啟用，需要 API 使用者主動選擇開啟。

---

## 計費邏輯

| 觸發時機 | 費用 |
|---|---|
| 輸出前被攔截 | 全部以 Opus 4.8 費率計費 |
| 串流輸出中途被攔截 | 攔截前的 input + token 以 Fable 5 費率；其餘以 Opus 4.8 費率 |

---

## 如何關閉自動切換

**設定 → 功能 → 關閉自動切換**

關閉後可以手動選擇重試 Fable 5 或繼續使用 Opus 4.8。

---

## 常態被擋的合法使用者：申請例外

反覆在合法專業工作中被擋的資安人員，可以申請 **Cyber Verification Program（CVP）**。

Anthropic 未來計畫隨著安全系統成熟，逐步開放雙用途防禦性資安和生物研究的配額。

---

## 和 Nathan Lambert 批評的對照

Lambert 批評的是「不通知使用者就讓模型變笨」的設計哲學。從這份文件來看，Fable 5 目前的機制是**有通知的**（符合他「透明」的標準）。他的批評更多是針對這個方向未來可能發展的風險，以及對開源研究的系統性不公平。

---

## 相關筆記

- [[AI 101 - Claude Fable 5 和 Mythos 5 發布公告 — Anthropic]] — 安全架構全貌（包括紅隊測試數據）
- [[AI 101 - 不通知就自動降版等於錯位 AI — Nathan Lambert]] — 對這個機制的外部批評

---

## 來源

- [Anthropic Help Center（繁體中文）](https://support.claude.com/zh-TW/articles/15363606)
