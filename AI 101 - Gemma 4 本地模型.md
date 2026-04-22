---
title: AI 101 - Gemma 4 本地模型
tags: [ai, gemma4, ollama, local-model, google]
created: 2026-04-22
updated: 2026-04-22
---

# Gemma 4 本地模型

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> Gemma 4 發佈日期：2026 年 4 月 2 日
> 授權：Apache 2.0（可商用）
> 特色：原生 Function Calling、多模態輸入、benchmark 媲美 20 倍大的模型

---

## 什麼是 Gemma 4

Google 開源的輕量語言模型系列，設計目標是**在本機硬體上跑出接近雲端大模型的效果**。
透過 [Ollama](https://ollama.com) 即可一行安裝，完全本地執行，資料不離機。

---

## 選擇哪個版本？

| 版本 | 參數量 | VRAM 需求 | 適合場景 |
|---|---|---|---|
| `gemma4:e4b` | 4B | 8 GB（或 Apple Silicon）| 筆電、輕量任務 |
| `gemma4:26b` | 26B MoE* | 18 GB+ | 桌機、工作站 |

> [!tip] 26B MoE 的特別之處
> 26B 是 Mixture of Experts（MoE）架構，每次推理只激活 3.8B 參數，
> 跑起來像 4B 的速度，品質卻接近 13B 模型。
> - τ2-bench（agentic tool use）：**85.5%**
> - MMLU：**82.6%**
> 大多數人建議選 26B MoE。

---

## 安裝 Ollama + Gemma 4

```bash
# 1. 安裝 Ollama
curl -fsSL https://ollama.com/install.sh | sh

# 2. 拉取 Gemma 4（選一個版本）
ollama pull gemma4:e4b    # 筆電版（8GB VRAM）
ollama pull gemma4:26b    # 桌機版（18GB+ VRAM）

# 3. 測試是否正常運作
ollama run gemma4:e4b
# 或
ollama run gemma4:26b
```

Ollama 會在背景啟動 server，預設監聽 `http://localhost:11434`

---

## 接上 OpenClaw

### 方法：修改 `~/.openclaw/openclaw.json`

```json
{
  "model": {
    "provider": "ollama",
    "baseUrl": "http://127.0.0.1:11434",
    "model": "gemma4:26b",
    "contextWindow": 131072
  }
}
```

> [!warning] 注意 URL 格式
> OpenClaw 的 Ollama URL 用 `http://127.0.0.1:11434`，**不要加 `/v1`**。
> 加了 `/v1` 會導致 tool calling 失效。

### 驗證設定

```bash
openclaw doctor
```

---

## 接上 Hermes Agent

### 方法一：透過設定精靈（推薦新手）

```bash
hermes setup
# 選擇 Ollama 作為 provider
# 輸入 base URL：http://localhost:11434/v1
# 輸入 model：gemma4:26b
```

### 方法二：直接執行時指定 context size

```bash
# Hermes 要求最少 64K context
ollama run gemma4:26b --ctx-size 65536
```

### 方法三：建立 Modelfile（穩定性最佳）

```bash
cat > ~/Modelfile.gemma4 << 'EOF'
FROM gemma4:26b
PARAMETER num_ctx 32768
EOF

ollama create gemma4-hermes -f ~/Modelfile.gemma4
```

然後在 `hermes setup` 選模型時填 `gemma4-hermes`。

> [!tip] 為何用 Modelfile？
> 預設 context 設定在消費級硬體上可能不穩定。
> Modelfile 把 context 鎖在 32K，跑起來更穩定，速度也更快。

---

## WSL2 用戶注意事項

如果 Ollama 裝在 Windows 本機，Hermes/OpenClaw 在 WSL2 裡面，URL 要改成：

```bash
# 查詢 Windows host IP
ip route show | grep default | awk '{print $3}'
# 通常是 172.x.x.1

# 設定 Ollama URL
http://172.x.x.1:11434
```

或設定環境變數讓 Ollama 對外開放：

```powershell
# Windows PowerShell
$env:OLLAMA_HOST = "0.0.0.0"
ollama serve
```

---

## 三者比較（雲端 vs 本地）

| | 雲端（Claude/GPT） | Gemma 4 本地 |
|---|---|---|
| **費用** | 按用量收費 | 免費（硬體電費）|
| **隱私** | 資料上傳雲端 | 資料不離機 |
| **速度** | 取決於網路 | 取決於本機硬體 |
| **能力上限** | 較高 | 略低 |
| **離線使用** | 不行 | 可以 |

---

## 相關筆記

- [[AI 101 - OpenClaw]] — OpenClaw 完整說明
- [[AI 101 - Hermes Agent]] — Hermes Agent 完整說明
- [[AI 101 - 核心概念]] — LLM 基礎概念
