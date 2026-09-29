---
title: "Claude Code 完整指令與快速鍵速查表"
tags: [ai, 外部觀點, claude-code, slash-commands, keyboard-shortcuts, 速查表, 數位時代]
source: https://www.bnext.com.tw/article/90925/claude-code-slash-commands-shortcuts-complete-guide
author: 數位時代
created: 2026-05-27
---

# Claude Code 完整指令與快速鍵速查表

> [!info]
> 原文：[數位時代 — Claude Code 完整指令速查](https://www.bnext.com.tw/article/90925/claude-code-slash-commands-shortcuts-complete-guide)
> **一句話：** Claude Code 60+ 個 Slash Command 與 19 個快速鍵的完整速查，按功能分成 13 大類，從對話管理到多 Agent 協同一覽無遺。

---

## Slash Commands 分類速查

### 對話管理

| 指令 | 功能 |
|---|---|
| `/btw` | 傳遞背景資訊，不觸發行動 |
| `/compact` | 壓縮對話以省 token |
| `/context` | 查看目前的 context 使用量 |
| `/clear` | 清除對話歷史 |
| `/recap` | 重新整理當前 session 的摘要 |
| `/export` | 匯出對話記錄 |
| `/copy` | 複製最後一個 AI 回應 |

### 程式碼操作

| 指令 | 功能 |
|---|---|
| `/diff` | 查看最近的程式碼變更差異 |
| `/rewind` | 回退到前一個狀態 |
| `/branch` | 建立 git branch |
| `/add-dir` | 新增工作目錄 |
| `/security-review` | 針對現有程式碼進行安全審查 |
| `/review` | 請 AI 審查程式碼 |

### 規劃與自動化

| 指令 | 功能 |
|---|---|
| `/plan` | 啟動計畫模式（先規劃後執行）|
| `/ultraplan` | 深度規劃模式，適合複雜任務 |
| `/goal` | 設定長期目標，讓 Agent 持續執行 |
| `/schedule` | 排程執行任務 |
| `/autofix-pr` | 自動修復 PR 上的 CI 錯誤 |

### 多 Agent 協同

| 指令 | 功能 |
|---|---|
| `/agents` | 查看目前所有 Agent 的狀態 |
| `/tasks` | 查看所有任務列表 |
| `/background` | 將任務移到背景執行 |
| `/stop` | 停止指定的 Agent 或任務 |

### Session 管理

| 指令 | 功能 |
|---|---|
| `/rename` | 重新命名目前 session |
| `/resume` | 恢復上一個 session |
| `/desktop` | 切換到桌面模式 |
| `/teleport` | 傳送到另一個 session |
| `/remote-control` | 允許遠端控制 |
| `/sandbox` | 在沙盒環境中執行 |

### 模型設定

| 指令 | 功能 |
|---|---|
| `/model` | 切換 AI 模型 |
| `/effort` | 調整思考深度 |
| `/fast` | 切換 Fast mode（Opus 4.6，更快輸出）|

### 介面設定

| 指令 | 功能 |
|---|---|
| `/config` | 開啟設定 |
| `/theme` | 切換主題 |
| `/color` | 調整顏色 |
| `/tui` | 切換 TUI 模式 |
| `/focus` | 聚焦模式 |
| `/statusline` | 設定狀態列 |
| `/keybindings` | 查看或設定快速鍵 |

### 記憶與權限

| 指令 | 功能 |
|---|---|
| `/memory` | 管理 AI 記憶 |
| `/permissions` | 管理工具使用權限 |
| `/init` | 初始化 CLAUDE.md 專案設定 |

### 外部整合

| 指令 | 功能 |
|---|---|
| `/mcp` | 管理 MCP 伺服器連線 |
| `/ide` | 整合 IDE（VS Code / JetBrains）|
| `/chrome` | 連接 Chrome 瀏覽器 |
| `/install-github-app` | 安裝 GitHub App 整合 |
| `/install-slack-app` | 安裝 Slack App 整合 |
| `/voice` | 開啟語音輸入 |
| `/setup-bedrock` | 設定 AWS Bedrock |
| `/setup-vertex` | 設定 Google Vertex AI |

### Skills 與外掛

| 指令 | 功能 |
|---|---|
| `/skills` | 管理已安裝的 Skills |
| `/plugin` | 管理外掛 |
| `/reload-plugins` | 重新載入所有外掛 |

### 診斷與用量

| 指令 | 功能 |
|---|---|
| `/doctor` | 診斷環境設定 |
| `/usage` | 查看 token 用量 |
| `/extra-usage` | 查看進階用量統計 |
| `/status` | 查看系統狀態 |
| `/insights` | 查看使用洞察 |
| `/login` / `/logout` | 帳號登入 / 登出 |
| `/passes` | 查看訂閱方案 |

### 團隊

| 指令 | 功能 |
|---|---|
| `/team-onboarding` | 建立團隊 onboarding 文件 |

### 其他

| 指令 | 功能 |
|---|---|
| `/help` | 顯示說明 |
| `/powerup` | 升級功能 |
| `/release-notes` | 查看版本更新記錄 |
| `/feedback` | 送出意見回饋 |
| `/radio` | 背景音樂（lo-fi）|
| `/stickers` | 顯示貼圖 |
| `/exit` | 退出 Claude Code |

---

## 鍵盤快速鍵（19 個）

| 快速鍵 | 功能 |
|---|---|
| `Ctrl+C` | 中斷目前執行 |
| `Ctrl+D` | 退出 Claude Code |
| `Ctrl+T` | 開啟 Todos 面板 |
| `Ctrl+O` | 查看 transcript（對話記錄）|
| `Ctrl+J` | 在輸入框插入換行 |
| `Escape` | 取消目前動作 |
| `Ctrl+G` | 開啟外部編輯器 |
| `Ctrl+S` | Stash（暫存）目前工作 |
| `Shift+Tab` | 循環切換輸入模式 |
| `Meta+P`（Alt+P）| 開啟模型選擇器 |
| `Meta+O`（Alt+O）| 切換 Fast mode |
| `Meta+T`（Alt+T）| 切換 Thinking（深度思考）模式 |
| `Ctrl+X Ctrl+K` | 強制停止所有 Agent |
| `Ctrl+R` | 搜尋歷史指令 |

---

## 使用建議

1. **高頻指令**：`/compact`、`/context`、`/plan` 是日常工作最常用的三個
2. **多 Agent 工作流**：`/goal` + `/agents` + `/tasks` 組合使用，適合長時間自主執行
3. **快速鍵優先**：`Ctrl+T`（Todos）、`Meta+T`（Thinking）、`Ctrl+S`（Stash）能大幅加速工作節奏
4. **模型切換**：`Meta+O` 直接切 Fast mode，`/fast` 也能做到同樣效果

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 的完整功能架構
- [[AI Claude Code Workflow 實戰：ultrawork 召喚多 Agent 協同 — AI超元域]] — /ultrawork 實戰應用
- [[AI Agent 連續跑 27 小時：Claude Code ／goal 功能解析 — Gary Chen]] — /goal 的深度解析
- [[handoff Skill：把對話濃縮交接給下一個 Agent — Matt Pocock]] — Skills 系統的延伸應用

---

## 來源

- 原文：[數位時代](https://www.bnext.com.tw/article/90925/claude-code-slash-commands-shortcuts-complete-guide)
