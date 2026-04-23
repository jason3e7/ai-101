---
title: AI 101 - Better Agent Terminal
tags: [ai, claude-code, terminal, electron, workspace, agent]
created: 2026-04-22
updated: 2026-04-22
---

# Better Agent Terminal

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> GitHub：[tony1223/better-agent-terminal](https://github.com/tony1223/better-agent-terminal)
> 版本：v2.2.16
> 技術棧：Electron 28 + React 18 + TypeScript + Anthropic Claude SDK
> 授權：MIT

---

## 什麼是 Better Agent Terminal

一個 Electron 桌面應用程式，把**終端機管理 + Claude Code AI + 開發工具**整合在同一個視窗。

**解決的痛點：** 同時跑多個專案、多個 Claude agent session 時，視窗和終端機會亂成一片。

---

## 安裝

```bash
# macOS（Homebrew）
brew install --cask tonyq-org/tap/better-agent-terminal
```

其他平台（Windows / Linux）到 [GitHub Releases](https://github.com/tony1223/better-agent-terminal/releases) 下載預編譯版本。

---

## 核心功能

### 1. 多工作區管理

每個 workspace 綁定一個專案目錄。

- 分群組、拖拉排序
- 儲存成 profile 快速切換（本地或遠端）
- 每個 workspace 有獨立的環境變數設定
- 可 pop 出獨立視窗

### 2. 內建 Claude Code Agent

不需要另開終端機跑 `claude`，直接在 app 裡的 Agent 面板操作。

| 功能 | 說明 |
|---|---|
| **Git worktree 隔離** | Claude agent 在獨立 worktree 執行，不影響 main branch |
| **Session 持久化** | 關掉 app 重開，`/resume` 繼續上次對話 |
| **Session fork** | 在對話任意點分叉，開新分支試不同方向 |
| **Auto-compact** | context 快滿時自動壓縮，不中斷任務 |
| **Permission 模式** | 逐一確認 → 接受編輯 → 計劃模式 → 全自動（Bypass）|
| **Extended thinking** | 思考區塊可折疊，版面更乾淨 |
| **多帳號切換** | 可切換不同 Anthropic 帳號 |

#### /auto-continue — 長任務神器

```
/auto-continue 5 keep going
```

自動重複送 prompt 最多 5 次，讓大型重構、長時間任務自己跑完，不需要守在旁邊一直按繼續。

### 3. 終端機

- 70% 主面板 + 30% 縮圖列，同時監看多個終端機狀態
- xterm.js 驅動，完整 Unicode / 中文支援
- 可點擊的 file path，直接開啟預覽

### 4. Procfile Worker 面板

類似 Overmind，用 Procfile 格式同時管理多個 process：

```
web: npm run dev
worker: node worker.js
redis: redis-server
```

集中查看 log，可個別 start / stop / restart。

### 5. 整合工具

| 工具 | 功能 |
|---|---|
| **File browser** | 側邊欄瀏覽，markdown 即時預覽，語法高亮 |
| **Git viewer** | commit history、diff、branch、GitHub PR/Issue 瀏覽 |
| **Snippet manager** | SQLite 儲存程式碼片段，分類加星號 |
| **Image attachment** | 拖拉上傳圖片給 Claude（最多 5 張）|

### 6. Status Line

可自訂的狀態列，共 15 個欄位分三區顯示：

```
[Session ID] [Git Branch] [Token 用量] [Cache 效率] [API Rate Limit] ...
```

透過拖拉模板編輯器自訂顯示項目。

---

## 遠端存取

### WebSocket 連線

在遠端機器或手機上連入操作，輸入 host IP、port、token。

### Tailscale 整合

不需要手動開 port，直接 P2P 連線，跨網路也能安全存取。

### Headless Server 模式

無 GUI 的 server 版本，適合跑在 VPS 或背景服務：

```bash
bat-server --bind tailscale --port 3000 --token mytoken
```

| 參數 | 說明 |
|---|---|
| `--bind` | `localhost` / `tailscale` / `all` |
| `--port` | 自訂 port |
| `--token` | 驗證 token |

---

## 常用快捷鍵

| 快捷鍵 | 功能 |
|---|---|
| `Ctrl+\`` / `Cmd+\`` | 切換 Agent 面板與終端機 |
| `Ctrl+P` | 開啟檔案選擇器 |
| `Shift+Enter` | 多行輸入換行 |
| `Shift+Tab` | 切換模式 |

## Slash 指令

| 指令 | 功能 |
|---|---|
| `/resume` | 恢復上次 session |
| `/new` | 重置 session |
| `/model` | 切換 Claude 模型 |
| `/auto-continue N prompt` | 自動重複 N 次 |

---

## 架構

```
主程序（Node.js / Electron）
  ├── main.ts              應用程式入口、IPC、視窗管理
  ├── pty-manager.ts       PTY 程序生命週期
  ├── claude-agent-manager.ts  Claude SDK session 管理
  └── remote/              WebSocket server/client

渲染程序（React）
  ├── App.tsx              根元件、佈局、profile 協調
  ├── WorkspacePanel       工作區管理
  ├── TerminalPanel        終端機
  ├── ClaudeAgentPanel     AI Agent 面板
  └── workspace-store.ts / settings-store.ts  狀態管理
```

---

## 一句話定位

> 比單純的終端機多了 AI 整合，比純 Claude Code 多了完整的開發環境管理。
> 核心價值在 **Git worktree 隔離** 和 **session 管理**——讓多個 Claude agent 並行工作時不互相干擾。

---

## Sources

- [tony1223/better-agent-terminal GitHub](https://github.com/tony1223/better-agent-terminal)
- [GitHub Releases（各平台下載）](https://github.com/tony1223/better-agent-terminal/releases)
- [專案 README（功能、快捷鍵、slash 指令）](https://github.com/tony1223/better-agent-terminal/blob/main/README.md)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 核心概念
- [[AI 101 - Harness Engineering]] — Agent 執行基礎設施
- [[AI 101 - Context Engineering]] — Context / session 管理原理
