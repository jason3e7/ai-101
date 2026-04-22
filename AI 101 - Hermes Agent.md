---
title: AI 101 - Hermes Agent
tags: [ai, hermes-agent, self-hosted, agent, nous-research]
created: 2026-04-22
updated: 2026-04-22
---

# Hermes Agent

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> 開發者：[Nous Research](https://nousresearch.com)
> GitHub：[NousResearch/hermes-agent](https://github.com/nousresearch/hermes-agent)
> 官方文件：[hermes-agent.nousresearch.com/docs](https://hermes-agent.nousresearch.com/docs/)
> 定位：**"The agent that grows with you"**

---

## 什麼是 Hermes Agent

開源、自架的個人 AI Agent。特色是**持久記憶 + 自我進化**——它會在完成複雜任務後自動建立新的 Skill 留存，下次遇到類似任務就直接用。

**與 Claude Code / OpenClaw 的差異：**

| | Claude Code | OpenClaw | Hermes Agent |
|---|---|---|---|
| **定位** | 程式碼開發 | 通訊平台閘道 | 通用個人助理 |
| **記憶** | Session / 專案 | Session | 跨 session 持久 |
| **自我進化** | 無 | 無 | 自動建立 Skills |
| **開源** | 否 | 是 | 是 |
| **模型綁定** | Claude | 不綁定 | 不綁定 |

---

## 快速安裝（一行搞定）

> [!tip] 支援平台
> Linux、macOS、WSL2、Android（Termux）
> Windows 請先安裝 WSL2

```bash
curl -fsSL https://raw.githubusercontent.com/NousResearch/hermes-agent/main/scripts/install.sh | bash
```

**安裝程式自動處理：**
- Python、Node.js、ripgrep、ffmpeg 等相依套件
- Clone repo + 建立虛擬環境
- 設定全域 `hermes` 指令
- 引導設定 LLM 提供商

---

## 安裝後第一步

```bash
# 1. 啟動設定精靈（選 LLM 提供商、API key）
hermes setup

# 2. 啟動（推薦用 TUI 介面）
hermes --tui

# 或使用傳統 CLI
hermes
```

> [!warning] 最佳實踐
> 先讓一個乾淨的對話正常運作，再慢慢加 gateway、cron、skills、voice。
> 不要一次全開。

---

## 核心功能

### 持久記憶

跨 session 記住你說過的事，並定期整理。

- FTS5 全文搜尋歷史對話
- LLM 自動摘要跨 session 內容
- 不需要每次重新說明背景

### 自我進化 Skills

完成複雜任務後，Hermes 會自動把這個方法存成 Skill，下次直接用。

```
你：幫我每週一早上 9 點整理 GitHub Issues 並發到 Telegram
Hermes：完成。已建立 Skill「weekly-github-report」
下次：直接呼叫這個 Skill，不需要重新說明
```

### 40+ 內建工具

| 類別 | 工具 |
|---|---|
| **網路** | 網頁搜尋、瀏覽器自動化、視覺分析 |
| **系統** | 終端指令、檔案操作、程式碼執行 |
| **AI** | 圖片生成、語音合成（TTS）、多模型推理 |
| **自動化** | Cron 排程、Subagent 委派、任務規劃 |
| **記憶** | 記憶管理、Session 搜尋 |

### 多平台 Gateway

一個 gateway 同時串接：

```bash
hermes gateway
```

支援：Telegram、Discord、Slack、WhatsApp、Signal、CLI

- 語音備忘轉文字（voice memo transcription）
- 跨平台對話連續性

### 排程任務（Cron）

用自然語言設定定時任務：

```
「每天早上 8 點給我今日天氣和行事曆摘要，發到 Telegram」
「每週一晚上備份所有筆記到 Google Drive」
```

---

## 常用 CLI 指令

```bash
hermes              # 啟動（CLI 模式）
hermes --tui        # 啟動（TUI 介面，推薦）
hermes setup        # 設定精靈
hermes model        # 切換 LLM 提供商
hermes tools        # 管理啟用的工具
hermes gateway      # 啟動通訊平台閘道
hermes skills       # 管理 Skills
```

---

## 支援的 LLM 模型

不綁定特定模型，可自由切換：

| 提供商 | 說明 |
|---|---|
| Anthropic | Claude 系列 |
| OpenAI | GPT-4o、o1 系列 |
| Ollama | 本地模型（Gemma 4、Qwen 等，完全離線）|
| OpenRouter | 統一接入多家模型 |

---

## 接上 Gemma 4（本地模型）

> 詳細安裝步驟請見 [[AI 101 - Gemma 4 本地模型]]

**快速設定：**

```bash
# 1. 啟動設定精靈，選 Ollama
hermes setup
# base URL: http://localhost:11434/v1
# model:    gemma4:26b

# 2. 或建立穩定版 Modelfile（推薦）
cat > ~/Modelfile.gemma4 << 'EOF'
FROM gemma4:26b
PARAMETER num_ctx 32768
EOF
ollama create gemma4-hermes -f ~/Modelfile.gemma4
# 然後在 hermes setup 填入 model: gemma4-hermes
```

> [!warning] Context 最低需求
> Hermes 要求模型至少支援 **64K context**。
> 若使用 Modelfile 將 context 設為 32K，啟動時需加 `--ctx-size 65536`。

---

## 相關資源

- [官方文件](https://hermes-agent.nousresearch.com/docs/)
- [Quickstart](https://hermes-agent.nousresearch.com/docs/getting-started/quickstart/)
- [Skills Hub](https://hermes-agent.nousresearch.com/docs/skills)
- [awesome-hermes-agent](https://github.com/0xNyk/awesome-hermes-agent)（社群整理的 Skills 與資源）

---

## 相關筆記

- [[AI 101 - 核心概念]] — Agent 概念說明
- [[AI 101 - OpenClaw]] — 類似定位的 AI Gateway 比較
- [[AI 101 - Claude Code 生態系]] — Skills / Memory 概念對照
- [[AI 101 - Gemma 4 本地模型]] — 本地模型完整設定指南
