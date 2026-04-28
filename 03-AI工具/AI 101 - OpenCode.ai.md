---
title: AI 101 - OpenCode.ai
tags: [ai, coding-agent, terminal, open-source, ollama, model-agnostic]
created: 2026-04-28
updated: 2026-04-28
---

# OpenCode.ai — 開源、模型無關的 AI 程式碼助理

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> GitHub：[opencode-ai/opencode](https://github.com/opencode-ai/opencode)
> 官網：[opencode.ai](https://opencode.ai)
> 授權：開源（MIT）
> **一句話：** Claude Code 的開源替代方案，支援 75+ LLM 提供商，可完全免費搭配本地模型使用。

---

## 與 Claude Code 的核心差異

| | Claude Code | OpenCode |
|---|---|---|
| **模型** | 綁定 Claude | 75+ 提供商，可換模型 |
| **開源** | 否 | ✅ 是 |
| **本地模型** | 不支援 | ✅ 支援 Ollama |
| **費用** | 需 Anthropic API | 自帶 API key 或接本地模型 |
| **LSP 整合** | 是 | ✅ 是 |
| **專案記憶** | `CLAUDE.md` | `AGENTS.md` |
| **MCP 支援** | ✅ | ✅ |

> [!tip] 什麼時候選 OpenCode？
> - 想用 Gemini / GPT / DeepSeek 等不同模型寫程式
> - 想接 Ollama 完全免費、離線開發
> - 不想被綁定在 Anthropic 生態

---

## 安裝

```bash
# 方法一：一行安裝（推薦）
curl -fsSL https://opencode.ai/install | bash

# 方法二：npm
npm i -g opencode-ai@latest

# 方法三：Homebrew（macOS / Linux）
brew install opencode

# 方法四：Scoop（Windows）
scoop install opencode

# 升級
opencode upgrade
```

---

## 快速上手

```bash
# 1. 進入你的專案目錄
cd ~/my-project

# 2. 啟動（TUI 介面）
opencode

# 3. 第一次使用：連接 LLM 提供商
/connect   # 在 TUI 內輸入，引導設定 API key

# 4. 初始化專案記憶
/init      # 產生 AGENTS.md（相當於 Claude Code 的 CLAUDE.md）
```

---

## 兩種模式

| 模式 | 切換方式 | 能做什麼 |
|---|---|---|
| **Build 模式**（預設）| — | 讀寫檔案、執行指令、完整開發 |
| **Plan 模式** | `Tab` 鍵 | 唯讀分析，不修改任何檔案，適合先討論方案 |

> [!tip] 先 Plan 再 Build
> 複雜任務先切 Plan 模式讓 AI 規劃，確認方向對了再切回 Build 執行。

---

## 常用快捷鍵

| 快捷鍵 | 功能 |
|---|---|
| `Ctrl+S` / `Enter` | 送出訊息 |
| `Tab` | 切換 Plan / Build 模式 |
| `Ctrl+N` | 開新 Session |
| `Ctrl+A` | 切換 Session |
| `Ctrl+K` | 指令選單 |
| `Ctrl+O` | 選擇模型 |
| `Ctrl+E` | 外部編輯器編輯訊息 |
| `Ctrl+X` | 取消目前操作 |
| `Ctrl+L` | 查看 logs |
| `Ctrl+C` | 離開 |

## Slash 指令

| 指令 | 功能 |
|---|---|
| `/init` | 產生 `AGENTS.md` 專案記憶 |
| `/connect` | 設定 LLM 提供商 API key |
| `/undo` | 還原上一個 AI 操作（含檔案變更）|
| `/redo` | 重做還原的操作 |

---

## 支援的 LLM 提供商（75+）

### 雲端主流

| 提供商 | 模型範例 |
|---|---|
| **OpenAI** | GPT-4.1、GPT-4o、o3 |
| **Google** | Gemini 2.5 Pro / Flash |
| **DeepSeek** | DeepSeek V3.2、R1 |
| **xAI** | Grok 4 |
| **Groq** | Llama 4、DeepSeek R1 |
| **GitHub Copilot** | 用 Copilot 訂閱免費用 Claude / GPT |
| **OpenRouter** | 統一接入所有主流模型 |

> [!warning] Anthropic Claude Pro/Max 不可用
> Anthropic 政策限制，**Claude Pro / Max 訂閱無法用於 OpenCode**。
> 若要用 Claude，需使用 Anthropic API（pay-as-you-go）。

### 本地模型（免費、離線）

```bash
# 先確認 Ollama 在跑
ollama list
```

在 `~/.config/opencode/opencode.jsonc` 加入：

```jsonc
{
  "provider": {
    "ollama": {
      "npm": "@ai-sdk/openai-compatible",
      "name": "Ollama (local)",
      "baseURL": "http://localhost:11434/v1",
      "models": {
        "qwen3:14b": { "name": "Qwen3 14B" },
        "deepseek-r1:14b": { "name": "DeepSeek R1 14B" },
        "gemma3:4b": { "name": "Gemma 3 4B" }
      }
    }
  }
}
```

> [!warning] Ollama context 預設只有 4K
> Ollama 預設 `num_ctx=4096`，agentic 工作流需要至少 16K–32K。
> 建議建立 Modelfile 設定更大的 context（參考 [[AI 101 - Ollama 指令教學]]）：
> ```bash
> cat > Modelfile << 'EOF'
> FROM qwen3:14b
> PARAMETER num_ctx 32768
> EOF
> ollama create qwen3-14b-32k -f Modelfile
> ```
> 然後在 opencode.jsonc 的 models 裡用 `"qwen3-14b-32k"` 這個名稱。

---

## 設定檔結構

```
~/.config/opencode/opencode.jsonc   ← 主設定（providers、models、agents）
~/.local/share/opencode/auth.json   ← API keys（敏感，不要 commit）
```

### 設定範例

```jsonc
{
  // 預設使用的 Agent
  "agents": {
    "coder": {
      "model": "google/gemini-2.5-pro",
      "maxTokens": 32000
    }
  },

  // 自訂 provider（可加入 proxy 或本地服務）
  "provider": {
    "ollama": {
      "npm": "@ai-sdk/openai-compatible",
      "name": "Ollama",
      "baseURL": "http://localhost:11434/v1",
      "models": {
        "qwen3-14b-32k": { "name": "Qwen3 14B (32K ctx)" }
      }
    }
  }
}
```

---

## AGENTS.md — 專案記憶

相當於 Claude Code 的 `CLAUDE.md`，讓 AI 每次都了解專案脈絡。

```bash
# 讓 OpenCode 自動分析專案並產生
/init
```

建議 commit 到 Git：

```markdown
# 專案說明（AGENTS.md 範例）

## 技術棧
- Python 3.12 + FastAPI
- PostgreSQL 15
- Docker Compose

## 開發規範
- 所有 API 都要有 type hints
- 測試用 pytest，放在 tests/ 目錄

## 常用指令
- 啟動：`docker compose up`
- 測試：`pytest tests/`

## 注意事項
- 不要直接修改 migration 檔案
- 環境變數放 .env，不要 commit
```

---

## 非互動模式（自動化 / CI）

```bash
# 直接執行一個任務，不開 TUI
opencode run "幫我把 utils.py 裡的所有函式加上 type hints"

# 指定模型
opencode run --model google/gemini-2.5-flash "寫一個 README.md"

# 輸出 JSON（適合腳本解析）
opencode run --format json "列出這個專案的所有 API endpoint"

# 從檔案讀取 prompt
opencode run --file task.md

# 繼續上一個 session
opencode run --continue "繼續剛才的工作"
```

---

## MCP 整合

```bash
# 新增 MCP server
opencode mcp add github -- npx @modelcontextprotocol/server-github

# 列出已安裝的 MCP
opencode mcp list
```

設定方式與 Claude Code 相同，詳見 [[AI 101 - Claude Code 生態系]]。

---

## 分享 Session

```bash
# 產生可分享的 session 連結（方便 debug 或協作）
opencode run --share "分析這段程式碼的效能問題"
# 輸出：https://opencode.ai/s/abc123
```

---

## 查看 Token 用量

```bash
opencode stats
```

---

## 常見問題

> [!warning] Tool calling 不動作（接 Ollama）
> 先確認模型支援 function calling，再把 Ollama 的 `num_ctx` 調到 16K 以上。
> 不是所有模型都支援 tool calling，建議用 Qwen3、Llama 3.1+ 等有明確支援的。

> [!warning] 用 GitHub Copilot 訂閱
> 可以用 Copilot 帳號免費呼叫 Claude 3.5 Sonnet / GPT-4o 等模型，
> 但用量受 Copilot 方案限制，大量使用可能觸發 rate limit。

> [!tip] 用 OpenRouter 一個 key 搞定所有模型
> 註冊 OpenRouter，充值後用單一 API key 切換所有主流模型，
> 不需要分別管理 Anthropic / OpenAI / Google 的 key。

---

## Sources

- [OpenCode 官網](https://opencode.ai)
- [opencode-ai/opencode GitHub](https://github.com/opencode-ai/opencode)
- [OpenCode 官方文件](https://opencode.ai/docs/)
- [OpenCode Providers 文件](https://opencode.ai/docs/providers/)
- [OpenCode CLI 文件](https://opencode.ai/docs/cli/)
- [OpenCode + Ollama 整合 — Ollama 官方文件](https://docs.ollama.com/integrations/opencode)
- [OpenCode vs Claude Code 比較 — builder.io](https://www.builder.io/blog/opencode-vs-claude-code)
- [OpenCode vs Claude Code — DataCamp](https://www.datacamp.com/blog/opencode-vs-claude-code)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 的對應功能
- [[AI 101 - 輕量模型推薦]] — 本地 Ollama 模型選型
- [[AI 101 - Ollama 指令教學]] — 設定 num_ctx 給 OpenCode 用
- [[AI 101 - 模型費用與效果比較]] — 選哪個 provider 最划算
