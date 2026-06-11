---
title: "停止對 Claude 下 Prompt，改用 Karpathy 的方法"
tags: [ai, 外部觀點, claude-code, karpathy, claude-md, behavioral-rules, prompt-engineering, workflow]
source: https://www.youtube.com/watch?v=7zZy1QTvokM
author: YouTube
created: 2026-06-11
---

# 停止對 Claude 下 Prompt，改用 Karpathy 的方法

> [!info]
> 來源：[Stop Prompting Claude. Use Karpathy's Method Instead.](https://www.youtube.com/watch?v=7zZy1QTvokM)
> 發表：2026-06-09
> **一句話：** AI 編程 Agent 的真正問題不是能力，而是行為——Karpathy 方法透過 CLAUDE.md 定義「AI 怎麼思考和行動」，比每次微管理 prompt 更有效，這個 65 行的檔案在數個月內累積了 22 萬顆 GitHub 星。

---

## 核心論點

AI coding agent 卡住的原因通常不是模型不夠聰明，而是**行為設計的缺失**：

- Agent 不會主動釐清需求，直接假設並執行
- Agent 不會標記不確定性
- 每次靠 prompt 微管理，但 prompt 不跨 session 延續

Karpathy 方法的解答：**不要管理每個 prompt，而是定義 agent 的行為規則。**

---

## CLAUDE.md 的四個行為原則

由開發者 Forrest Chang（Jiayuan Zhang）在 2026-01-27 實作，靈感來自 Karpathy 的想法：

| 原則 | 說明 |
|---|---|
| **Think Before Coding** | 動手前先思考，不要直接就開始寫 |
| **Simplicity First** | 優先選最簡單的可行方案 |
| **Surgical Changes** | 只改需要改的地方，不要大幅重構 |
| **Goal-Driven Execution** | 每個動作都要服務於明確的目標 |

這份 65 行的 CLAUDE.md 在 2026 年成為 GitHub 史上成長最快的 repo 之一：數個月內超過 **10 萬顆星**，相關 fork 合計超過 **22 萬顆星**。

---

## Boris（Claude Code 負責人）的觀點

> **「我已經不再對 Claude 下 prompt 了。我讓 loops 在跑，是 loops 在對 Claude 下 prompt、搞清楚要做什麼。我的工作是寫 loops。」**

這句話揭示了一個方向轉移：
- 舊思維：你是 prompt 工程師，負責每次告訴 AI 怎麼做
- 新思維：你是行為架構師，負責設計讓 AI 自主運作的系統

---

## 方法對比

| 做法 | 本質 | 問題 |
|---|---|---|
| 傳統 prompting | 每次告訴 AI 要做什麼 | 不跨 session，需要持續人工介入 |
| Karpathy 方法 | 定義 AI 的思考和行動方式 | 一次設定，持久有效 |

---

## 相關筆記

- [[AI 101 - Claude Code 行為結構設計]] — CLAUDE.md、sub-agents、skills、hooks 的完整實作
- [[AI 101 - Harness Engineering]] — 「可靠性來自外層框架」的底層概念

---

## 來源

- YouTube：[Stop Prompting Claude. Use Karpathy's Method Instead.](https://www.youtube.com/watch?v=7zZy1QTvokM)
- Miraflow：[Karpathy's CLAUDE.md — 100K GitHub Stars](https://miraflow.ai/blog/karpathy-claude-md-100k-github-stars-ai-coding-2026)
