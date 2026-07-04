---
title: "Code with Claude London 2026 全場逐字稿（CC0）"
tags: [ai, 外部觀點, claude-code, anthropic, 研討會, 逐字稿, gemini, 謝昆霖]
source: https://www.facebook.com/keanu.hsieh/posts/pfbid029XqeKEcmE6hZHZ3N3mftxxCKXxsgidhvmhVw3dsCCvxVJN2NDZZ37QgFRkK7LMc8l
author: 謝昆霖（Keanu Hsieh）
created: 2026-05-27
---

# Code with Claude London 2026 全場逐字稿（CC0）

> [!info]
> 原文：[謝昆霖 Facebook 貼文](https://www.facebook.com/keanu.hsieh/posts/pfbid029XqeKEcmE6hZHZ3N3mftxxCKXxsgidhvmhVw3dsCCvxVJN2NDZZ37QgFRkK7LMc8l)
> **一句話：** 謝昆霖用 Gemini Meeting Recorder 把 Code with Claude London 2026 的 24 場議程全部轉成逐字稿，CC0 免費釋出，並整理出三個大趨勢。

---

## 資源下載

> 📄 24 場逐字稿（CC0）：[bookai.com.tw/XSxfh](https://bookai.com.tw/XSxfh)

每份文件包含：完整逐字稿、重點整理、Action Items、摘要。
可直接餵給 AI 快速查閱，不用看完整場錄影。

---

## Code with Claude London 2026 是什麼

Anthropic 2026 年全球開發者巡迴活動（舊金山 → 倫敦 → 東京），
London 場次：**2026-05-19**

### 九場議程

| # | 議程標題 | 講者 |
|---|---|---|
| 1 | Opening Keynote | Angela Jiang, Boris Cherny, Cat Wu, Katelyn Lesse, Lisa Crofoot |
| 2 | What's New in Claude Code | Ralph Ramos |
| 3 | Memory and Dreaming for Self-Learning Agents | Ravi Trivedi |
| 4 | Picking the Right Model | Lucas Smedley |
| 5 | Coding is No Longer the Constraint: Scaling DevEx at Spotify | Niklas Gustavsson |
| 6 | Designing with Claude: From Prompt to Production | Dan Cary |
| 7 | Beyond the Basics with Claude Code | Daisy Hollman |
| 8 | How to Get to Production Faster with Claude Managed Agents | Michael Cohen, Harrison Stall |
| 9 | From One Person to 80: Scaling a Hypergrowth Engineering Org with Claude Code | Gabriel Grinberg, Yoav Orlev（monday.com）|

---

## 本屆大會三個趨勢（謝昆霖整理）

1. **系統設計成為瓶頸**：模型進步後，限制不再是模型本身，而是系統設計能力
2. **簡化 System Prompt + 建立評估標準**：這兩件事是提升 AI 效能的關鍵槓桿
3. **從 Code Review 開始導入**：組織要開始 AI 整合，Code Review 是阻力最小的切入點

---

## 大會主要公告（綜合整理）

### Claude Code 強化
- **速率限制加倍**：Pro、Max、Enterprise 用戶的五小時用量上限翻倍
- **SpaceX Colossus 資料中心合作**：API 量年成長 17 倍的基礎設施支援
- **Desktop App**：全螢幕 GUI，與既有 CLI 和 IDE 整合並行
- **Code Review**：Anthropic 內部已全面採用
- **Remote Agents**：雲端異步執行
- **CI 自動修復**：PR 自動偵測錯誤並送出修正
- **Security Reviews**：安全掃描整合

### Claude Managed Agents
- **Multi-agent orchestration**：複雜任務的多 Agent 協作
- **Outcomes**：定義成功條件（與 `/goal` 同一個方向）
- **Dreaming（研究預覽）**：Agent 分析自己過去的 session 進行自我優化

### 策略方向
> 「讓開發者設定非同步自動化流程，早上醒來 PR 已經準備好可以 merge。」— Boris Cherny

重心從「即時互動」轉向「非同步自主工作流程」。

---

## 轉錄工具：Gemini Meeting Recorder

謝昆霖使用 [Gemini Meeting Recorder](https://meeting.hfcc.com.tw/) 處理 YouTube 影片，
使用的模型是 **Gemini 3.5 Flash**。

### 工具測試發現

| 測試項目 | 結果 |
|---|---|
| 直接處理 YouTube 連結 | 成功率約 1/3 |
| 先轉成 MP3 再處理 | 成功率 100% |
| Gemini 3.5 Flash 指令遵循 | 出乎意料地強，CP 值高 |
| Google 免費配額 | 大幅縮減（付費用戶也受影響）|

> [!tip] 實用技巧
> YouTube 影片轉錄成功率不穩定時，先把影片下載轉成 MP3 再丟給 Gemini，成功率從 1/3 提升到 100%。

---

## 適合誰用這份逐字稿

| 對象 | 建議關注的議程 |
|---|---|
| 開發者 | #2 What's New、#7 Beyond the Basics、#8 Managed Agents |
| 工程主管 | #5 Spotify 案例、#9 monday.com 案例 |
| 導入 AI 的 IT 部門 | #3 Memory & Dreaming、#6 Prompt to Production |
| 對模型選型有疑問的 | #4 Picking the Right Model |

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 整體架構與功能
- [[AI Agent 連續跑 27 小時：Claude Code /goal 功能解析 — Gary Chen]] — /goal 即是大會 Outcomes 功能的對應
- [[AI 101 - Subagent 使用與計費]] — Managed Agents 與 Subagent 的計費邏輯
- [[AI 101 - Harness Engineering]] — 大會「系統設計成為瓶頸」這個趨勢的完整觀念

---

## 來源

- 原文：[謝昆霖 Facebook 貼文](https://www.facebook.com/keanu.hsieh/posts/pfbid029XqeKEcmE6hZHZ3N3mftxxCKXxsgidhvmhVw3dsCCvxVJN2NDZZ37QgFRkK7LMc8l)
- 逐字稿下載：[bookai.com.tw/XSxfh](https://bookai.com.tw/XSxfh)
- 活動官網：[claude.com/code-with-claude/london](https://claude.com/code-with-claude/london)
- Simon Willison Live Blog：[simonwillison.net/2026/May/6/code-w-claude-2026](https://simonwillison.net/2026/May/6/code-w-claude-2026/)
