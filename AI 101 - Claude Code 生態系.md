---
title: AI 101 - Claude Code 生態系
tags: [ai, claude-code, mcp, skills, hooks, plugins]
created: 2026-04-22
updated: 2026-04-22
---

# Claude Code 生態系

[[AI 101 - 主頁|← 回主頁]]

---

## 架構總覽

```
Plugin（容器）
  ├── Skills    → 可呼叫的 /指令
  ├── Hooks     → 自動觸發的行為
  └── MCP 設定  → 工具擴充

CLAUDE.md        → 專案常駐記憶
Memory           → 跨對話個人記憶
Subagents        → 獨立 context 的子 AI
```

---

## MCP（Model Context Protocol）

**工具層** — 給 AI 新的「手」

Anthropic 在 2024 年 11 月推出，已被 OpenAI、Google DeepMind 採用。
被業界稱為 **「AI 的 USB-C」**——統一的工具連接標準。

### MCP 的三種原語（Primitives）

| 原語 | 說明 |
|---|---|
| **Resources** | AI 可以讀取的資料（檔案、資料庫） |
| **Tools** | AI 可以呼叫的函式（搜尋、API） |
| **Prompts** | 預設的指令模板 |

### 常用 MCP Servers

| MCP | 用途 |
|---|---|
| `github` | 操作 PR、Issues、Repo |
| `playwright` | 瀏覽器自動化 |
| `context7` | 自動抓最新技術文件 |
| `linear` | 專案管理整合 |
| `serena` | LSP 語言伺服器（程式碼分析）|
| `filesystem` | 讀寫本地檔案 |

### 設定範例（settings.json）

```json
{
  "mcpServers": {
    "github": {
      "command": "npx",
      "args": ["-y", "@modelcontextprotocol/server-github"],
      "env": {
        "GITHUB_PERSONAL_ACCESS_TOKEN": "your_token"
      }
    }
  }
}
```

---

## Skills

**行為層** — 教 AI 怎麼做一件事

本質是一段定義好流程的 prompt/指令集，透過 `/skill-name` 呼叫。

### 內建 Skills

| 指令 | 功能 |
|---|---|
| `/review` | 自動 code review |
| `/init` | 產生 CLAUDE.md |
| `/security-review` | 安全性審查 |
| `/commit` | 智能 commit message |
| `/pr` | 建立 Pull Request |

### 第三方 Skills（來自 Plugins）

| 指令 | Plugin | 功能 |
|---|---|---|
| `/feature-dev` | feature-dev | 完整功能開發流程（含多 agent）|
| `/hookify` | hookify | 從對話自動建立 hooks |
| `/skill-creator` | skill-creator | 建立新 skill |

> [!tip] 如何呼叫
> 在對話中直接輸入 `/指令名稱` 即可觸發。

---

## Hooks

**自動化層** — 在特定事件自動執行 shell 指令

不依賴 AI 判斷，**保證執行**。

### Hook 事件列表

| 事件 | 觸發時機 |
|---|---|
| `SessionStart` | 對話開始時 |
| `UserPromptSubmit` | 使用者送出訊息時 |
| `PreToolUse` | 工具執行前（可阻擋） |
| `PostToolUse` | 工具執行後 |
| `PermissionRequest` | AI 請求權限時 |
| `SubagentStart` | 子 agent 啟動 |
| `SubagentStop` | 子 agent 結束 |
| `TaskCreated` | 任務建立 |
| `TaskCompleted` | 任務完成 |
| `Stop` | 對話結束時 |

### Hook 設定範例

```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Write",
        "command": "prettier --write $CLAUDE_TOOL_INPUT_PATH"
      }
    ],
    "Stop": [
      {
        "command": "notify-send 'Claude 完成了！'"
      }
    ]
  }
}
```

### 常見 Hook 用途

- 寫完檔案後自動格式化（prettier、black）
- git push 前自動跑測試
- 對話結束時發送通知
- 阻止危險指令執行（如 `rm -rf`）

---

## Plugins

**打包層** — 容器，一次安裝多項功能

從 Marketplace 安裝，包含 Skills + Hooks + MCP 設定。

### 推薦 Plugins

| Plugin | 亮點 |
|---|---|
| `commit-commands` | 簡化 git 流程 |
| `code-review` | 多 agent 並行 code review |
| `feature-dev` | 完整功能開發 workflow |
| `security-guidance` | 自動提示安全風險 |
| `hookify` | 從對話學習建立 hooks |
| `superpowers` | TDD、subagent 開發、brainstorming |
| `claude-md-management` | 管理 CLAUDE.md |

---

## Subagents

**隔離層** — 在獨立 context 執行子任務

> [!info] 核心概念
> 每個 subagent 有自己的 context window，不會污染主對話。
> 完成後只回傳摘要給主對話。

**適合用 subagent 的情況：**
- 搜尋任務會產生大量不需要保留的結果
- 探索程式碼庫（讀了很多檔案但只需要結論）
- 平行執行多個獨立任務
- 專業分工（一個 agent 寫程式，一個 review）

```
主對話 context（乾淨）
  └── Subagent A：搜尋文件 → 回傳摘要
  └── Subagent B：分析程式碼 → 回傳結論
  └── Subagent C：跑測試 → 回傳結果
```

---

## CLAUDE.md

**專案記憶** — 每次對話都會自動載入

放在專案根目錄，告訴 Claude：
- 專案架構與技術棧
- 程式碼風格規範
- 常用指令（如何跑測試、如何部署）
- 不要做的事（禁止事項）

```markdown
# 專案說明
這是一個 FastAPI + PostgreSQL 的後端服務。

# 技術棧
- Python 3.11
- FastAPI
- PostgreSQL 15
- pytest

# 常用指令
- 跑測試：`pytest tests/`
- 啟動服務：`uvicorn main:app --reload`

# 規範
- 所有 API 必須有 type hints
- 不要直接修改 migration 檔案
```

---

## Memory 系統

**個人跨對話記憶** — 記住使用習慣與偏好

儲存在 `~/.claude/projects/*/memory/`，跨對話持續存在。

| 記憶類型 | 儲存什麼 |
|---|---|
| `user` | 使用者角色、技術背景、偏好 |
| `feedback` | 你給 AI 的修正與確認 |
| `project` | 專案目標、決策、截止日期 |
| `reference` | 外部資源位置（Linear、Slack 頻道）|

---

## 相關筆記

- [[AI 101 - Context Engineering]] — 理解為何這些工具這麼重要
- [[AI 101 - 實用技巧與最佳實踐]] — 具體的使用技巧
