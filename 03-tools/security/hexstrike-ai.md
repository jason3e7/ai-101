---
title: AI 101 - HexStrike AI
tags: [ai, 工具, 資安, 滲透測試, mcp, claude, claude-code, 開源, python]
created: 2026-06-02
---

# AI 101 - HexStrike AI

> [!info]
> 開源 MCP 伺服器，讓 Claude、GPT、Copilot 等 AI Agent 直接呼叫 150+ 資安工具，涵蓋網路偵察、Web 漏洞、逆向工程、雲端安全、OSINT 等類別。MIT 授權，已內建於 Kali Linux。

---

## 快速定位

| 維度 | HexStrike AI |
|---|---|
| 語言 | Python 3.8+ |
| 整合方式 | MCP Server（接 Claude、GPT、Copilot 等） |
| 工具數量 | 150+ 資安工具 |
| 授權 | MIT |
| 適用場景 | 滲透測試、Bug Bounty、CTF、資安研究 |

---

## 是什麼

HexStrike AI 是一個 **MCP 中間層**，讓 AI 模型能夠呼叫真實的資安工具。架構上是一個 FastMCP Server，綁定所有工具函式，讓 LLM 能用自然語言下指令（例如「掃描這個 IP 的 web 服務」），由 AI 自動選工具、設參數、執行、回傳結果。

```
你（自然語言）
     ↓
Claude / GPT / Copilot
     ↓
HexStrike MCP Server
     ↓
nmap / sqlmap / nuclei / Ghidra ... （150+ 工具）
```

---

## 工具分類

| 類別 | 工具數 | 代表工具 |
|---|---|---|
| 網路偵察 | 25+ | nmap, Rustscan, Amass, Subfinder |
| Web 應用測試 | 40+ | Gobuster, SQLMap, Nuclei, WPScan |
| 二進制分析 | 25+ | Ghidra, Radare2, GDB, Binwalk |
| 雲端安全 | 20+ | Prowler, Trivy, Kube-Hunter |
| CTF / 取證 | 20+ | Volatility, Steghide, ExifTool |
| OSINT | 20+ | Sherlock, SpiderFoot, Maltego |

---

## 安裝

```bash
# 1. Clone repo
git clone https://github.com/0x4m4/hexstrike-ai.git
cd hexstrike-ai

# 2. 建立虛擬環境
python3 -m venv hexstrike-env
source hexstrike-env/bin/activate

# 3. 安裝依賴
pip3 install -r requirements.txt

# 4. 啟動 MCP Server
python3 hexstrike_server.py
```

> [!tip]
> Kali Linux 使用者可直接從套件庫安裝，已預先整合常用工具。

確認服務正常：

```bash
curl http://localhost:8888/health
```

---

## 基本使用

### 接上 Claude Desktop

編輯 `~/.config/Claude/claude_desktop_config.json`：

```json
{
  "mcpServers": {
    "hexstrike-ai": {
      "command": "python3",
      "args": ["/path/to/hexstrike_mcp.py", "--server", "http://localhost:8888"],
      "timeout": 300
    }
  }
}
```

重啟 Claude Desktop 後，即可在對話中直接呼叫資安工具。

### 接上 Claude Code CLI

Claude Code CLI 使用 `claude mcp add` 或 `.mcp.json`，不需要 Desktop。

**前提：先啟動 hexstrike server**

```bash
cd hexstrike-ai
source hexstrike-env/bin/activate
python3 hexstrike_server.py   # 跑在 localhost:8888
```

**方法一：`claude mcp add`（快速一次性設定）**

```bash
claude mcp add --transport stdio hexstrike \
  -- python3 /path/to/hexstrike-ai/hexstrike_mcp.py --server http://localhost:8888
```

驗證：

```bash
claude mcp list    # 看到 hexstrike 即成功
# 或進 session 後執行 /mcp
```

**方法二：`.mcp.json`（推薦，跟著專案目錄走）**

在 HTB / pentest 工作目錄建立 `.mcp.json`：

```json
{
  "mcpServers": {
    "hexstrike": {
      "type": "stdio",
      "command": "python3",
      "args": ["/path/to/hexstrike-ai/hexstrike_mcp.py", "--server", "http://localhost:8888"],
      "timeout": 600000
    }
  }
}
```

> [!tip]
> `timeout` 設 600000ms（10 分鐘）。掃描類工具執行時間長，預設值太短會中途 timeout。

**Desktop 設定 vs CLI 設定對照**

| | Claude Desktop | Claude Code CLI |
|---|---|---|
| 設定位置 | `~/.config/Claude/claude_desktop_config.json` | `claude mcp add` 或 `.mcp.json` |
| Transport | stdio（`command` + `args`） | 同左，加 `--transport stdio` |
| `alwaysAllow` | JSON 陣列（MCP config 內） | `settings.json` 的 `allowedTools`，或 session 內選「Always allow」 |
| Scope | 全域 | `local`（只此 project）或 `user`（全域） |

### 支援的 MCP 客戶端

- Claude Desktop / Claude Code
- VS Code Copilot
- Cursor
- Roo Code
- 其他支援 MCP 協定的 Agent

---

## 進階

### 12 個 AI Agent 分工

HexStrike 內建 12 個專業 Agent，各司其職：

```
偵察 Agent → 漏洞掃描 Agent → 利用 Agent
     ↓               ↓              ↓
OSINT Agent     Cloud Agent    逆向分析 Agent
     ...
```

智慧決策引擎負責挑選最適合的工具與參數，避免重複執行。

### 搭配 CTF 使用

HexStrike 支援 CTF 模式，可串接取證類工具（Volatility、Steghide）和二進制分析（Ghidra、GDB），讓 Claude 直接協助解題。

---

## 資安注意事項

> [!warning]
> HexStrike AI 上線後曾被惡意行為者用於加速零時差漏洞利用（包含 Citrix NetScaler ADC 案例）。
>
> **只在有書面授權的目標上使用。** 本工具對合法的滲透測試、Bug Bounty、CTF、資安研究有很高價值，但未授權使用是違法的，且工具能力足以造成嚴重危害。

---

## Sources

- [GitHub: 0x4m4/hexstrike-ai](https://github.com/0x4m4/hexstrike-ai)
- [hexstrike-ai — Kali Linux Tools](https://www.kali.org/tools/hexstrike-ai/)
- [Hexstrike-AI: LLM Orchestration Driving Real-World Zero-Day Exploits — Check Point](https://blog.checkpoint.com/executive-insights/hexstrike-ai-when-llms-meet-zero-day-exploitation/)
- [I Let AI Pentest my lab for 45 Minutes — InfoSec Write-ups](https://infosecwriteups.com/i-let-ai-pentest-my-lab-for-45-minutes-hexstrike-ai-and-kali-mcp-to-get-a-shell-after-only-20-5ba5857bae10)
