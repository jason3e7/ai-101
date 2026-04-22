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

| 版本 | 參數量 | VRAM 需求 | 速度 | 適合場景 |
|---|---|---|---|---|
| `gemma4:e4b` | 4B | 8 GB（或 Apple Silicon）| 快 | 筆電、硬體較弱、一般任務 |
| `gemma4:26b` | 26B MoE* | 18 GB+ | 慢 | 桌機、工作站、複雜推理 |

> [!tip] 26B 跑很慢？改用 e4b
> 26B 需要 18GB+ VRAM，硬體不夠強時推理速度會很慢。
> `gemma4:e4b` 只需 8GB，速度快很多。
> 一般任務（問答、程式碼、摘要）e4b 已經夠用。

> [!info] 26B MoE 架構說明
> 26B 是 Mixture of Experts（MoE）架構，每次推理只激活 3.8B 參數。
> 但仍需將整個模型載入 VRAM，所以記憶體需求不會因此降低。

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

## Step 1：確認遠端 Ollama 對外開放

Ollama 預設只監聽 `127.0.0.1`，跑在另一台機器時需先開放對外連線。

**在 Gemma 4 那台機器執行：**

```bash
# Linux（systemd）
sudo systemctl edit ollama
```

加入以下內容後存檔：

```ini
[Service]
Environment="OLLAMA_HOST=0.0.0.0:11434"
```

```bash
sudo systemctl daemon-reload
sudo systemctl restart ollama
```

確認防火牆放行 port 11434：

```bash
sudo ufw allow 11434
```

---

## Step 2：確認 Context 大小是否達到 64K

Hermes Agent 要求模型至少支援 **64K（65536）tokens context**。
在**你自己的機器**用以下方法遠端查詢（把 `gemma4:26b` 換成你實際用的版本）：

### 方法一：curl 查詢模型資訊（推薦）

```bash
# 26b 版
curl http://<遠端IP>:11434/api/show \
  -d '{"name": "gemma4:26b"}' | python3 -m json.tool | grep num_ctx

# e4b 版
curl http://<遠端IP>:11434/api/show \
  -d '{"name": "gemma4:e4b"}' | python3 -m json.tool | grep num_ctx
```

看到 `num_ctx` 的值，若 **≥ 65536** 就達標。

### 方法二：curl 發一次 chat 看回傳的 context 資訊

```bash
curl http://<遠端IP>:11434/api/chat \
  -d '{
    "model": "gemma4:e4b",
    "messages": [{"role":"user","content":"hi"}],
    "stream": false
  }' | python3 -m json.tool | grep -E "eval_count|prompt_eval"
```

### 方法三：在遠端機器直接跑 `ollama ps`

```bash
ssh user@<遠端IP> "ollama ps"
# 輸出的 CONTEXT 欄位顯示目前運行的 context 大小
```

### 方法四：在遠端機器跑 `ollama show`

```bash
ssh user@<遠端IP> "ollama show gemma4:e4b --modelfile"
# 找 PARAMETER num_ctx 那行
```

> [!tip] 預設值問題
> Ollama 預設 `num_ctx` 只有 **2048**，遠低於 64K。
> 就算模型架構支援更大的 context，如果沒有明確設定，實際能用的只有 2048。

---

## Step 3：若 Context 不足，在遠端機器調整

**在 Gemma 4 那台機器建立 Modelfile（以 e4b 為例）：**

```bash
cat > ~/Modelfile.gemma4e4b << 'EOF'
FROM gemma4:e4b
PARAMETER num_ctx 65536
EOF

ollama create gemma4-e4b-64k -f ~/Modelfile.gemma4e4b
```

26b 版本同理：

```bash
cat > ~/Modelfile.gemma4 << 'EOF'
FROM gemma4:26b
PARAMETER num_ctx 65536
EOF

ollama create gemma4-hermes -f ~/Modelfile.gemma4
```

驗證是否生效：

```bash
ollama show gemma4-e4b-64k --modelfile | grep num_ctx
# 應顯示：PARAMETER num_ctx 65536
```

---

## Step 4：接上 OpenClaw（遠端 Ollama）

修改 `~/.openclaw/openclaw.json`，把 `baseUrl` 改成遠端 IP：

```json
{
  "model": {
    "provider": "ollama",
    "baseUrl": "http://<遠端IP>:11434",
    "model": "gemma4:e4b",
    "contextWindow": 65536
  }
}
```

> [!warning] 注意 URL 格式
> OpenClaw 的 Ollama URL **不加 `/v1`**，否則 tool calling 會失效。

驗證：

```bash
openclaw doctor
```

---

## Step 5：接上 Hermes Agent（遠端 Ollama）

```bash
hermes setup
# provider: ollama
# base URL: http://<遠端IP>:11434/v1      ← Hermes 要加 /v1
# model:    gemma4-e4b-64k               ← e4b 用這個
#           或 gemma4-hermes             ← 26b 用這個
```

或直接設定環境變數：

```bash
export OLLAMA_HOST=http://<遠端IP>:11434
hermes setup
```

---

## 快速診斷指令一覽

| 目的 | 指令（在你的機器執行）|
|---|---|
| 查遠端模型 context 大小 | `curl http://<IP>:11434/api/show -d '{"name":"gemma4:26b"}'` |
| 確認遠端 Ollama 有在跑 | `curl http://<IP>:11434/api/tags` |
| 列出遠端所有模型 | `curl http://<IP>:11434/api/tags \| python3 -m json.tool` |
| 驗證 OpenClaw 設定 | `openclaw doctor` |
| 重跑 Hermes 設定 | `hermes setup` |

---

## WSL2 用戶注意事項

如果 Ollama 裝在 Windows 本機，Hermes/OpenClaw 在 WSL2 裡面，URL 要改成：

```bash
# 查詢 Windows host IP
ip route show | grep default | awk '{print $3}'
# 通常是 172.x.x.1

# 設定 Ollama URL（WSL2 內）
http://172.x.x.1:11434
```

---

## 進階：Gemma-4-31B-JANG（MLX 版，Apple Silicon 專用）

> [!info] 模型來源
> HuggingFace：[dealignai/Gemma-4-31B-JANG_4M-CRACK](https://huggingface.co/dealignai/Gemma-4-31B-JANG_4M-CRACK)
> 格式：JANG v2（MLX-native safetensors）
> 這是 **abliterated** 版本——移除了原始模型的安全限制，僅供研究用途，使用者自行負責法律合規。

### 規格

| 項目 | 詳情 |
|---|---|
| 參數量 | 31B（Dense 架構）|
| 量化 | 平均 5.1 bits |
| 模型大小 | 21 GB |
| 下載大小 | 22.7 GB |
| 最低硬體 | Apple Silicon Mac，**32GB+ 統一記憶體** |

> [!warning] Ollama 不支援此模型
> 此模型為 MLX 原生格式，**無法用 Ollama 跑**。
> 需要 vMLX 或 mlx_lm，且只能在 Apple Silicon Mac 上執行。

### 方法一：vMLX（推薦，最簡單）

```bash
# 1. 安裝 vMLX（到官網下載 App）
# https://vmlx.net

# 2. 下載模型
pip install huggingface_hub
python3 -c "
from huggingface_hub import snapshot_download
snapshot_download('dealignai/Gemma-4-31B-JANG_4M-CRACK')
"

# 3. 在 vMLX 中載入模型路徑即可
```

vMLX 原生支援：視覺模式（多模態）、思考模式（Chain of Thought）、所有推理參數設定。

### 方法二：mlx_lm（命令列）

> [!warning]
> 標準 `mlx_lm` 截至 v0.31.2 尚未支援 Gemma 4，請先確認版本。

```bash
# 安裝
pip install mlx-lm

# 執行
python3 -c "
from mlx_lm import load, generate

model, tokenizer = load('dealignai/Gemma-4-31B-JANG_4M-CRACK')
response = generate(model, tokenizer, prompt='你好', max_tokens=1024)
print(response)
"
```

### 推薦推理參數

| 模式 | Temperature | Repetition Penalty | Top P |
|---|---|---|---|
| 思考模式 OFF | 0.0 – 1.0 | 1.00 | 0.95 |
| 思考模式 ON | **0.3 – 0.7** | **1.15 – 1.25** | 0.95 |

> [!tip] 思考模式注意事項
> - 避免 temperature=0 搭配思考模式（會增加迴圈風險）
> - 建議 repetition penalty 1.2 防止規劃迴圈

### 與標準 Gemma 4 的差異

| | `gemma4:26b`（Ollama）| JANG 31B（MLX）|
|---|---|---|
| 執行平台 | Linux / Windows / macOS | Apple Silicon Mac 限定 |
| 工具 | Ollama | vMLX / mlx_lm |
| 安全限制 | 保留 | 移除（abliterated）|
| 硬體需求 | 18GB VRAM | 32GB 統一記憶體 |
| 接 OpenClaw / Hermes | 可以 | 需要額外轉接層 |

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
