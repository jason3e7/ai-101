---
title: "Claude Code 被爆內建隱藏程式碼標記中國用戶"
tags: [ai, 外部觀點, 參考, claude-code, 新聞, 爭議, 透明度]
source: https://www.koc.com.tw/archives/647871
author: Rocky（KOC）
created: 2026-07-06
---

# Claude Code 被爆內建隱藏程式碼標記中國用戶 — Claude Code Accused of Covertly Tagging China Users

[← 回主頁](../../index.md)

> [!NOTE]
> 原文：[Claude Code 被爆內建隱藏程式碼，偷偷標記中國用戶 — Rocky, KOC](https://www.koc.com.tw/archives/647871)
> **一句話：** 有開發者逆向分析後宣稱 Claude Code 內建隱密機制，用肉眼難辨的 Unicode 差異標記疑似中國用戶；Anthropic 工程師回應這是「3 月的反濫用實驗」、目的是防未授權轉售與模型蒸餾，並稱已準備移除。

> **TL;DR (EN):** A developer's reverse-engineering claim (reported by KOC) alleges Claude Code covertly tags suspected China-based users via nearly-invisible Unicode differences. An Anthropic engineer said it was a March anti-abuse experiment (to deter unauthorized resale and model distillation) that they were preparing to remove. Recorded here as a time-sensitive news claim, not independently verified.

> [!WARNING]
> 這是**新聞轉述 + 逆向分析指控**，非本庫查證。以下同時記錄「指控內容」與「官方回應」，判斷請自行斟酌。

---

## 指控內容 — The Claim

文章宣稱，在 **Claude Code 2.1.196** 版本中發現隱密的偵測與標記機制：

- **偵測訊號**：檢查系統時區（`Asia/Shanghai`、`Asia/Urumqi`）、代理網址、以及中國 AI 實驗室的特徵
- **標記手法**：用**肉眼難以察覺的 Unicode 差異**做標記，例如
  - 日期格式從 `2026-06-30` 悄悄變成 `2026/06/30`
  - 用不同編碼的引號替換一般引號
- **發現方式**：對二進位執行檔做逆向分析

---

## Anthropic 官方回應 — The Response

- 工程師 **Thariq Shihipar** 表示：這是「**3 月推出的實驗**」
- **目的**：防止**未授權轉售**與**模型蒸餾（model distillation）**
- **狀態**：稱已**準備移除**此機制

---

## 爭議核心 — Why It Matters

原文的批評點：即使「防濫用」目的可以理解，但在**高權限開發工具**中使用**未公開說明的隱藏標記**，傷害透明度與用戶信任——違反「**權限越大，透明度越該清楚**」的原則。

---

## 記錄用提醒 — Caveats

- **時效性高**：官方稱正在移除；後續版本可能已無此機制，本篇會過期
- **二手來源**：核心是某開發者的逆向發現 + 媒體轉述，本庫未獨立重現
- 若你要引用，建議回頭找**一手來源**（原始逆向分析貼文／Anthropic 正式聲明）再確認

## 來源 — Sources

- 原文：[Claude Code 被爆內建隱藏程式碼，偷偷標記中國用戶 — Rocky, KOC](https://www.koc.com.tw/archives/647871)
