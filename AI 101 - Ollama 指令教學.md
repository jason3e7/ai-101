---
title: AI 101 - Ollama 指令教學
tags: [ai, ollama, cli, commands, local-model]
created: 2026-04-22
updated: 2026-04-22
---

# Ollama 指令教學

[[AI 101 - 主頁|← 回主頁]]

> [!info] 你目前的環境
> 已安裝：`gemma4:latest`（9.6 GB）
> Ollama server：另一台機器

---

## 指令總覽

| 指令 | 說明 |
|---|---|
| `ollama list` | 列出所有已下載的模型 |
| `ollama ps` | 列出目前正在運行的模型 |
| `ollama show <model>` | 查看模型詳細資訊 |
| `ollama run <model>` | 啟動模型（互動模式）|
| `ollama pull <model>` | 下載模型 |
| `ollama rm <model>` | 刪除模型 |
| `ollama cp <src> <dst>` | 複製模型（改名）|
| `ollama create <name>` | 從 Modelfile 建立客製模型 |
| `ollama serve` | 手動啟動 Ollama server |
| `ollama stop <model>` | 停止正在運行的模型 |

---

## 查看模型資訊

### 列出已安裝的模型

```bash
ollama list
# NAME              ID              SIZE      MODIFIED
# gemma4:latest     c6eb396dbd59    9.6 GB    4 days ago
```

### 查看目前正在跑的模型

```bash
ollama ps
# NAME           ID      SIZE    PROCESSOR    UNTIL
# gemma4:latest  ...     ...     100% GPU     4 minutes from now
# CONTEXT 欄位顯示目前實際使用的 context 大小
```

### 查看模型詳細參數（**確認 context 大小用這個**）

```bash
ollama show gemma4:latest
```

輸出範例：
```
  Model
    architecture        gemma3
    parameters          27.2B
    context length      131072    ← 模型架構支援的最大值
    embedding length    5120
    quantization        Q4_K_M

  Parameters
    num_ctx    2048              ← 實際使用的 context（預設很小！）

  ...
```

> [!warning] 注意兩個不同的數字
> - `context length 131072`：模型**架構**支援的上限
> - `num_ctx 2048`：Ollama **實際**分配給這次推理的大小
>
> Ollama 預設 `num_ctx` 只有 **2048**，遠低於架構上限。
> 必須手動設定才能用到完整的 context。

### 只看 Modelfile 設定

```bash
ollama show gemma4:latest --modelfile
```

---

## 確認 gemma4:latest 的 Context 大小

在 Ollama 那台機器執行：

```bash
# 方法一：看 show 輸出的 num_ctx
ollama show gemma4:latest | grep num_ctx

# 方法二：透過 API 查
curl http://localhost:11434/api/show \
  -d '{"name": "gemma4:latest"}' \
  | python3 -m json.tool | grep num_ctx
```

**結果判斷：**

| `num_ctx` 值 | 狀態 |
|---|---|
| 沒出現 / 2048 | 使用預設值，對 Hermes 不夠用 |
| ≥ 65536 | 達到 Hermes Agent 最低需求 |
| ≥ 131072 | 完整 context，最佳狀態 |

---

## 調整 Context 大小

### 臨時調整（只影響這次對話）

```bash
ollama run gemma4:latest
# 進入互動模式後輸入：
/set parameter num_ctx 65536
```

### 永久調整（建立 Modelfile）

```bash
# 1. 複製現有模型（避免改壞原版）
ollama cp gemma4:latest gemma4-64k

# 2. 查看現有 Modelfile 內容
ollama show gemma4-64k --modelfile > /tmp/Modelfile.gemma4

# 3. 加入 num_ctx 設定
echo "PARAMETER num_ctx 65536" >> /tmp/Modelfile.gemma4

# 4. 重新建立模型
ollama create gemma4-64k -f /tmp/Modelfile.gemma4

# 5. 驗證
ollama show gemma4-64k | grep num_ctx
```

---

## 執行與互動

### 基本啟動

```bash
ollama run gemma4:latest
# 進入互動 REPL，直接打字對話
```

### 互動模式內的指令

```
/set parameter num_ctx 65536   設定 context 大小
/set parameter temperature 0   降低隨機性（輸出更穩定）
/show info                     顯示模型資訊
/show modelfile                顯示 Modelfile
/bye                           離開
```

### 單次問答（不進入互動模式）

```bash
echo "用一句話解釋 MCP" | ollama run gemma4:latest
```

### 從檔案輸入

```bash
ollama run gemma4:latest < prompt.txt
```

---

## 模型管理

### 下載新模型

```bash
ollama pull gemma4:e4b         # 下載輕量版
ollama pull llama3.2:latest    # 下載其他模型
```

### 刪除模型

```bash
ollama rm gemma4:e4b
```

### 複製 / 重命名

```bash
# cp 不會重新下載，只是建立一個新的參照
ollama cp gemma4:latest gemma4-custom
```

---

## 遠端存取

### 從另一台機器查詢

```bash
# 列出遠端所有模型
curl http://<遠端IP>:11434/api/tags | python3 -m json.tool

# 查遠端特定模型資訊
curl http://<遠端IP>:11434/api/show \
  -d '{"name": "gemma4:latest"}' | python3 -m json.tool

# 確認遠端 Ollama 是否存活
curl http://<遠端IP>:11434/
# 回傳 "Ollama is running" 就正常
```

### 使用環境變數指定遠端主機

```bash
export OLLAMA_HOST=http://<遠端IP>:11434

# 之後的 ollama 指令都會打到遠端
ollama list
ollama ps
ollama show gemma4:latest
```

---

## 常見問題診斷

| 問題 | 指令 |
|---|---|
| 確認 server 是否在跑 | `curl http://localhost:11434/` |
| 看模型 context 實際大小 | `ollama ps` |
| 模型回答品質差、忘記前文 | `ollama show <model>` 確認 `num_ctx` |
| 確認 GPU 有沒有在用 | `ollama ps`（看 PROCESSOR 欄位）|
| 手動重啟 server | `ollama serve` |

---

## 相關筆記

- [[AI 101 - Gemma 4 本地模型]] — Gemma 4 完整設定，含遠端 Ollama 連線
- [[AI 101 - OpenClaw]] — 接上 OpenClaw
- [[AI 101 - Hermes Agent]] — 接上 Hermes Agent
