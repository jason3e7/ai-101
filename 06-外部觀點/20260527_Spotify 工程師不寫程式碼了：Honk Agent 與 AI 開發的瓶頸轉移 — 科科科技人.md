---
title: "Spotify 工程師不寫程式碼了：Honk Agent 與 AI 開發的瓶頸轉移"
tags: [ai, 外部觀點, spotify, honk, claude, agent-sdk, mcp, backstage, 企業落地, 瓶頸轉移, Code-with-Claude, 科科科技人]
source: https://www.facebook.com/permalink.php?story_fbid=pfbid02qpwHCoDz1xEPjHhEtA5hxevAi5rQPkjdzQ1BmEyq6DbenU7PKM3b4kntZJvxA1i7l&id=100092499602680
author: 科科 科技人 Tech-Technology
created: 2026-05-27
---

# Spotify 工程師不寫程式碼了：Honk Agent 與 AI 開發的瓶頸轉移

> [!info]
> 原文：[科科 科技人 Tech-Technology](https://www.facebook.com/permalink.php?story_fbid=pfbid02qpwHCoDz1xEPjHhEtA5hxevAi5rQPkjdzQ1BmEyq6DbenU7PKM3b4kntZJvxA1i7l&id=100092499602680)
> 原始場合：Code with Claude London 2026，Spotify 首席架構師 Niklas Gustavsson（17 年 Spotify 資歷）27 分鐘演講
> **一句話：** Spotify 99% 工程師每週用 AI 編碼，PR 頻率暴漲 76%，自研 Agent「Honk」可自主完成跨 39 個 repo 的升級——最大的轉變不是效率，而是瓶頸從「寫程式碼」移到了「做決策」。

---

## 核心數據

| 指標 | 數值 |
|---|---|
| 每週使用 AI 編碼的工程師比例 | **99%** |
| 自評「AI 讓我更高效」 | **94%** |
| PR 頻率增幅 | **+76%**（且每兩週還在漲）|
| 自動化維護 PR 已合併數量 | **250 萬個**（大部分零人工介入）|

> [!quote]
> "By now, most of the PRs we ship are authored by an AI agent together with the developer."
> — Niklas Gustavsson, Spotify 首席架構師

---

## Honk：Spotify 的自研後台 Agent

Honk 是 Spotify 基於 **Claude + Agent SDK** 自研的工程 Agent，在 Slack 上呼叫：

```
@Honk 幫我把這 39 個 Repository 的 gRPC 升級
```

Honk 的自動化流程：

```
讀取 Backstage 元數據
  └── 修改程式碼
        └── 推分支
              └── 跑 CI
                    └── 跨 OS 驗證
                          └── 失敗 → 自己讀錯誤 → 自己修正
```

全程無需人工介入。

### 實際成效

| 任務 | 以前 | 現在 |
|---|---|---|
| Java 大版本遷移（協調幾百個團隊）| 幾個月 | **3 天** |
| Prototype 驗證 | 數天 | **幾分鐘** |

### Honk V2 新功能

- **多人協作 Agent Session**：像 Google Docs，但裡面坐著 Claude
- **Agent 編譯器 Chirp**：結構化 Agent 生成
- **App 快速生成**：任何人可在程式碼倉庫裡幾分鐘生成可安裝 App

---

## 三個企業落地洞見

### 洞見 1：標準化是 Agent 的燃料

> 「程式碼越一致，Claude 越強。碎片化程式碼庫裡 Claude 表現明顯變差。」

Spotify 做法：把規範寫進 lint，Claude 寫完程式碼後：
1. 觸發 lint
2. 自己讀取錯誤
3. 自己修正

→ 這是讓 Agent 能「自我校正」的關鍵基礎設施。

### 洞見 2：Backstage 變成 Agent 操作台

Backstage 原本是給人類用的開發者門戶，現在：

**所有功能暴露成 MCP（Model Context Protocol）工具**，Claude 可直接調用：
- 查詢組件歸屬
- 給團隊發 Slack 通知
- 觸發部署流程

→ 既有的內部工具不用重建，只需加 MCP 介面讓 Agent 能操作。

### 洞見 3：瓶頸轉移才是真正的洞見

> [!warning] 這才是最重要的訊號
> "Coding used to be the bottleneck. Now it's shifting to human decision making."
> — Niklas Gustavsson

| 過去的瓶頸 | 現在的瓶頸 |
|---|---|
| 寫程式碼的速度 | 做什麼的**決策力** |
| | 審哪些 PR 的**判斷力** |
| | Agent 調用前後的**品味** |

---

## 對工程職位的影響

作者觀察：

> 「很多公司連 Senior 缺都不太開了，只開架構師、Staff 工程師等主要負責『規劃』而不是『寫 code』的職位。」

Spotify 案例正是這個趨勢的具體實證——資深工程師的價值轉移到架構規劃與系統設計，AI Agent 承接大量實作工作。

---

## 相關筆記

- [[Code with Claude London 2026 全場逐字稿（CC0）— 謝昆霖]] — 這場演講的完整逐字稿來源
- [[AI 101 - Harness Engineering]] — Honk 的標準化策略正是 Harness Engineering 的實踐
- [[蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt — 新人類聯盟]] — 企業 Agent 編排的思路
- [[AI 101 - Subagent 使用與計費]] — Agent SDK 的架構基礎

---

## 來源

- 原文：[科科 科技人 Tech-Technology](https://www.facebook.com/permalink.php?story_fbid=pfbid02qpwHCoDz1xEPjHhEtA5hxevAi5rQPkjdzQ1BmEyq6DbenU7PKM3b4kntZJvxA1i7l&id=100092499602680)
- 演講場合：[Code with Claude London 2026](https://www.youtube.com/live/Dz4JhsZMCyU)
