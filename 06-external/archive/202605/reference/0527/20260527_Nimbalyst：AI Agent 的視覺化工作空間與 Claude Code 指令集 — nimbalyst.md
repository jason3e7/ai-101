---
title: "Nimbalyst：AI Agent 的視覺化工作空間與 Claude Code 指令集"
tags: [ai, 外部觀點, claude-code, visual-editor, electron, ios, wysiwyg, slash-commands, product-manager, developer, nimbalyst]
source: https://github.com/nimbalyst/nimbalyst
author: nimbalyst
created: 2026-05-27
---

# Nimbalyst：AI Agent 的視覺化工作空間與 Claude Code 指令集

> [!info]
> GitHub：[nimbalyst/nimbalyst](https://github.com/nimbalyst/nimbalyst)（623 ★）
> **一句話：** Nimbalyst 是一個免費本地視覺化編輯器，專為與 Claude Code、Codex 等 AI Agent 協作而設計——提供 WYSIWYG diff 審查、Kanban session 管理、iOS 遠端監控，並附帶開發者與 PM 兩套 Claude Code 自訂指令集。

---

## 主產品：Nimbalyst 視覺化工作空間

### 核心定位

> 「針對與 AI Coding Agent 協作最佳化的視覺工作空間，最大化速度、頻寬與 Context。」

解決的核心問題：AI Agent 改了什麼，你怎麼知道？Nimbalyst 把 diff 變成可視化的 WYSIWYG 介面，讓人類審查、編輯、標注後再繼續迭代。

### 主要功能

**視覺化編輯器**

| 支援格式 | 說明 |
|---|---|
| Markdown | WYSIWYG 即時預覽 |
| Mockup | UI 草圖繪製 |
| Mermaid 圖表 | 流程圖、架構圖 |
| Excalidraw | 手繪風格草圖 |
| CSV 資料 | 表格視覺化 |
| 程式碼 | Monaco 編輯器（VS Code 同款）|

**Session 與任務管理**

- Kanban 看板管理多個平行 Agent Session
- 檔案與 Session 綁定
- Git 整合 + AI 輔助 commit 訊息
- 嵌入式 Terminal
- Plan / Bug 任務追蹤

**iOS 行動 App**

在手機上：
- 遠端監控 Agent Session 狀態
- 文字 / 語音回應 Agent 問題
- 視覺化 diff 審查
- 任務排隊
- Push 通知

**可擴充架構**

「任何檔案格式都可插入自訂編輯器」，內建延伸套件支援：
- Astro 網站
- 心智圖（mindmap）
- 投影片
- 3D 物件

### 技術規格

| 項目 | 內容 |
|---|---|
| 架構 | TypeScript / Electron monorepo（npm workspaces）|
| 平台 | macOS（Intel/Silicon）、Windows 10+、Linux |
| 行動端 | iOS（SwiftUI）|
| 隱私 | PostHog 匿名分析，不收集 PII、檔案內容、API Key |

---

## 開發者 Claude Code 指令集（14 個）

> GitHub：[developer-claude-code-commands](https://github.com/nimbalyst/developer-claude-code-commands)（25 ★）

### 規劃與設計

| 指令 | 功能 |
|---|---|
| `/plan` | 帶架構決策的結構化實作計畫 |
| `/mockup` | 根據現有程式碼與設計模式生成 UI 草圖 |

### 實作

| 指令 | 功能 |
|---|---|
| `/implement` | 依最佳實踐實作功能 |
| `/create-command` | 生成新的自訂 Claude Code 指令 |
| `/create-subagent` | 為複雜任務建立專門的 Sub-agent |

### 程式碼品質

| 指令 | 功能 |
|---|---|
| `/code-review` | 涵蓋架構、安全、效能的全面審查 |
| `/analyze-code` | 模式分析與複雜度評估 |
| `/validate-and-fix` | 需求驗證與問題修正 |
| `/write-tests` | 單元測試與整合測試生成 |

### 版本控制與發布

| 指令 | 功能 |
|---|---|
| `/commit` | 格式化 git commit 訊息 |
| `/review-branch` | PR 前的變更審查 |
| `/mychanges` | Standup 格式的 commit 摘要 |
| `/release-internal` | 內部版本發布準備 |
| `/release-public` | 公開 repo 發布流程 |

---

## 產品經理 Claude Code 指令集（19 個）

> GitHub：[product-manager-claude-code-commands](https://github.com/nimbalyst/product-manager-claude-code-commands)（48 ★）

### 分類總覽

| 類別 | 指令 |
|---|---|
| **需求規劃** | `/plan`、`/prd`（PRD 文件）|
| **市場研究** | `/competitive`、`/research`、`/customer-interview`、`/customer-interview-simulate` |
| **設計** | `/mockup` |
| **技術理解** | `/understand-feature`、`/edge-cases` |
| **專案管理** | `/status`（Linear/JIRA 整合）、`/github-status` |
| **Bug 管理** | `/bug-report` |
| **上市與銷售** | `/launch`、`/sales-enablement`、`/documentation` |
| **客戶回饋** | `/triage-requests`、`/feedback-analyze` |
| **策略** | `/strategy` |

---

## 整體生態系

```
nimbalyst 組織（GitHub）
├── nimbalyst          ← 主產品：視覺化 IDE（623★）
├── skills             ← Skills 公開集合，供 PM / 開發者 / 內容創作者（20★）
├── developer-claude-code-commands    ← 14 個開發者指令（25★）
├── product-manager-claude-code-commands  ← 19 個 PM 指令（48★）
└── nimbalyst-mindmap  ← 心智圖延伸套件（1★）
```

---

## 適合誰

- 用 Claude Code 但想要視覺化 diff 審查的開發者
- 非工程師 PM，需要與 AI Agent 協作但不習慣純 Terminal 環境
- 同時跑多個 Agent Session、需要 Kanban 管理的重度用戶
- 想要手機端遠端監控 Agent 的場景

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Nimbalyst 是 Claude Code 生態系的視覺化前端
- [[handoff Skill：把對話濃縮交接給下一個 Agent — Matt Pocock]] — Skills 生態的另一個角度
- [[Claude Code 完整指令與快速鍵速查表 — 數位時代]] — 自訂指令與官方指令的對照

---

## 來源

- 主產品：[nimbalyst/nimbalyst](https://github.com/nimbalyst/nimbalyst)
- 開發者指令：[developer-claude-code-commands](https://github.com/nimbalyst/developer-claude-code-commands)
- PM 指令：[product-manager-claude-code-commands](https://github.com/nimbalyst/product-manager-claude-code-commands)
- Skills 集合：[nimbalyst/skills](https://github.com/nimbalyst/skills)
