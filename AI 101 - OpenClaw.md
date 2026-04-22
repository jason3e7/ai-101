---
title: AI 101 - OpenClaw
tags: [ai, openclaw, self-hosted, agent, gateway]
created: 2026-04-22
updated: 2026-04-22
---

# OpenClaw

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> 目前版本：**2026.4.15**（活躍維護中）
> GitHub：[openclaw/openclaw](https://github.com/openclaw/openclaw)

---

## 什麼是 OpenClaw

OpenClaw 是一個**自架式 AI Agent 閘道器（Gateway）**，讓你把強大的語言模型（Claude、GPT 等）連接到本地系統和各種通訊平台。

**一句話定位：** 你自己跑在自己機器上的個人 AI 助理，在你已經在用的通訊軟體上回應你。

> [!tip] 與 Claude Code 的差異
> Claude Code 專注於程式碼開發與 Repository 操作。
> OpenClaw 定位更廣：跨平台通訊、語音、排程任務、個人助理。
> 兩者可以搭配使用。

---

## 安裝

### 系統需求

- Node.js + npm
- macOS / Linux / Windows（WSL2）

### 安裝步驟

```bash
# 1. 安裝 Node.js 與 npm（Linux）
sudo apt update
sudo apt install nodejs npm -y

# 2. 安裝 OpenClaw
sudo npm install -g openclaw@latest

# 3. 引導設定（設定 gateway、workspace、channels）
openclaw onboard --install-daemon
```

預設控制介面：`http://localhost:18789`
Workspace 位置：`~/.openclaw/`

---

## 核心概念

### Gateway（閘道器）

OpenClaw 的核心。負責在 AI 模型與各通訊管道之間橋接。

```
你的訊息（Telegram/Discord/...）
  → Gateway
    → AI 模型（Claude / GPT / ...）
      → 工具執行（搜尋、瀏覽器、排程...）
        → 回覆
```

### Channels（通訊管道）

支援的平台：

| 類別 | 平台 |
|---|---|
| **即時通訊** | Telegram、WhatsApp、Signal、LINE、WeChat |
| **工作協作** | Slack、Discord、Microsoft Teams、Mattermost |
| **其他** | iMessage、Matrix、IRC、Twitch、Nostr |

### Skills（技能）

OpenClaw 的 Skills 概念與 Claude Code 類似，可擴充 AI 的能力。

---

## 內建工具

| 工具 | 說明 |
|---|---|
| `browser` | 控制瀏覽器，抓取網頁內容 |
| `canvas` | 即時渲染可互動的畫布 |
| `nodes` | 自訂處理節點 |
| `cron` | 排程定時任務 |
| `sessions` | 管理對話 session |

---

## 常用 CLI 指令

```bash
# 查看所有指令（100+ 個子指令）
openclaw --help

# 診斷工具（設定變更後、升級後、有問題時都跑這個）
openclaw doctor

# 啟動 gateway
openclaw start

# 設定管理
openclaw configure

# 查看 channels 狀態
openclaw channels list
```

> [!tip] 最重要的指令
> `openclaw doctor` — 任何問題先跑這個，會自動檢查設定是否正確。

---

## 設定檔

所有設定集中在單一檔案：

```
~/.openclaw/openclaw.json
```

格式為 **JSON5**（支援註解的 JSON）。

---

## 模型支援

| 提供商 | 說明 |
|---|---|
| Anthropic | Claude 系列（Opus、Sonnet、Haiku）|
| OpenAI | GPT-4o、o1 系列 |
| 其他 | 可透過相容 API 接入 |

---

## 相關筆記

- [[AI 101 - 核心概念]] — Agent 概念說明
- [[AI 101 - Claude Code 生態系]] — Claude Code 的 MCP / Skills 對比
