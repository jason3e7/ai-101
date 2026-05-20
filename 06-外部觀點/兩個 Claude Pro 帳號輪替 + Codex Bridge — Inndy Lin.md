---
title: 兩個 Claude Pro 帳號輪替 + Codex Bridge
tags: [ai, 外部觀點, claude-code, codex, 帳號策略, openai-compatible, golang]
source: https://www.facebook.com/inndy.tw/posts/pfbid0KcDgpahwyEB6gENimvPn1DHXb4aSqidvyxG471kKdR4JJfh9rCWDKMBB5L4djaJel
author: Inndy Lin
created: 2026-05-20
---

# 兩個 Claude Pro 帳號輪替 + Codex Bridge — Inndy Lin

> [!info]
> 一個工程師的真實使用策略：用兩個 Claude Pro 帳號輪替、Codex 外包 code review、
> 自建工具降低成本，同時維持供應鏈安全。

---

## 使用策略

**兩個 Claude Pro 帳號輪替：**
- 不固定用 Opus（最貴的模型），視任務選擇模型
- 避免過度並行 Agent（防止超出配額）
- 目前夠用，預期之後升 Max 方案

**Code Review 外包給 Codex：**
- Claude 負責開發，Codex 負責審查
- 兩個工具各司其職，互補使用

---

## 自建工具

### 1. Claude Code Launcher
為了解決帳號切換麻煩而做，整合了 **ccxray** 的功能。

- **ccxray**：[github.com/lis186/ccxray](https://github.com/lis186/ccxray) — Claude Code 使用量監控工具

### 2. Codex Bridge（核心工具）

> GitHub：[github.com/Inndy/codex-bridge](https://github.com/Inndy/codex-bridge)

**目的：** 把 Codex 包裝成 **OpenAI 相容的 API endpoint**，
讓任何支援 OpenAI API 的 chatbot 工具都能直接用 Codex，
不需要 pay-as-you-go 的 token 費用。

**技術細節：**
- 用 Golang 實作，**零外部依賴**
- 由 AI 協助開發，但特別注意供應鏈安全（避免引入不可信的第三方套件）
- 作為 OpenAI-compatible server，可接入各種現有工具

**使用情境：**

```
原本：chatbot 工具 → OpenAI API（按 token 計費）

用了 Codex Bridge：
chatbot 工具 → Codex Bridge → Codex（訂閱制，不額外計費）
```

---

## 費用數據

實際 API 費用：**$119.15 / 月**（從 dashboard 截圖可見）

> [!tip] 這個策略的邏輯
> 訂閱制（Claude Pro + Codex）處理主要工作，
> API 費用只用在訂閱無法覆蓋的部分，
> 自建 bridge 讓 chatbot 工具也能吃到訂閱額度。

---

## 延伸思考

- 「兩個帳號輪替」是在現有定價下最大化利用率的土炮方案，也反映訂閱制 rate limit 的壓力
- Codex Bridge 的零依賴設計，示範了「AI 輔助開發 + 人工把關供應鏈安全」的實踐
- Codex 做 code review、Claude 做開發——工具分工比單一工具全包更划算

---

## 相關筆記

- [[AI 101 - Subagent 使用與計費]] — 訂閱 vs API 計費的詳細分析
- [[在 Claude Code 裡呼叫 OpenAI Codex：codex-plugin-cc — Will 保哥]] — 另一種 Claude + Codex 整合方式
- [[實驗：Claude 指揮 Codex 與 Codex Multi-Agent 測試]] — Claude 指揮 Codex 的實測數據
