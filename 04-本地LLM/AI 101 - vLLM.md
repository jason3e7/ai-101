---
title: AI 101 - vLLM
tags: [ai, vllm, inference, serving, local-llm, openai-compatible, gpu, quantization]
created: 2026-04-23
updated: 2026-04-23
---

# vLLM — 高吞吐量 LLM 推論伺服器

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> GitHub：[vllm-project/vllm](https://github.com/vllm-project/vllm)
> 官網：[docs.vllm.ai](https://docs.vllm.ai)
> **一句話：** 把 HuggingFace 模型變成 OpenAI 相容 API，吞吐量比原生 HF Transformers 高 14–24 倍。

---

## 為什麼用 vLLM？

| 比較項目 | HuggingFace Transformers | vLLM |
|---|---|---|
| **吞吐量** | baseline | **14–24x 更高** |
| **記憶體效率** | KV cache 容易碎片化 | PagedAttention，幾乎零浪費 |
| **API 相容性** | 無內建 server | OpenAI API 格式，直接替換 |
| **批次處理** | 靜態批次 | Continuous batching，自動填滿 GPU |
| **量化支援** | 有限 | AWQ、GPTQ、FP8 原生支援 |

> [!tip] 什麼時候用 vLLM？
> - 需要把本地模型對外提供 API
> - 想用 OpenAI SDK 但換成自己的模型
> - 單張 GPU 要同時服務多個請求
> - 需要量化來省 VRAM

---

## 核心技術：PagedAttention

> [!info] 直覺理解
> 傳統做法：每個 request 預留一整塊連續記憶體 → 大量浪費（平均浪費 60–80%）
> PagedAttention：KV cache 像虛擬記憶體的分頁，按需分配 → 浪費趨近於零
>
> 結果：同樣的 GPU，能同時跑更多 request → 吞吐量大幅提升

---

## 安裝

**系統需求：**
- OS：Linux（Windows 請用 Docker）
- Python：3.10–3.13
- CUDA：12.1+（NVIDIA）

```bash
# 推薦：用 uv 建虛擬環境安裝
uv venv --python 3.12 --seed
source .venv/bin/activate
uv pip install vllm --torch-backend=auto

# 或用 pip 直接安裝
pip install vllm

# AMD ROCm（A100 以外的 AMD GPU）
uv pip install vllm --extra-index-url https://wheels.vllm.ai/rocm/
```

---

## 快速上手

### 方法一：Python 直接推論（不啟動 server）

適合跑批次、腳本自動化：

```python
from vllm import LLM, SamplingParams

# 載入模型（第一次會從 HuggingFace 下載）
llm = LLM(model="Qwen/Qwen2.5-7B-Instruct")

# 設定生成參數
sampling_params = SamplingParams(
    temperature=0.8,
    top_p=0.95,
    max_tokens=256
)

# 批次推論（同時處理多個 prompt，比迴圈快很多）
prompts = [
    "請用一句話解釋什麼是量子力學：",
    "台北最值得去的三個景點是：",
]
outputs = llm.generate(prompts, sampling_params)

for output in outputs:
    print(f"Prompt: {output.prompt!r}")
    print(f"Generated: {output.outputs[0].text!r}\n")
```

### 方法二：啟動 OpenAI 相容 API Server

啟動後，任何支援 OpenAI SDK 的工具都能直接接：

```bash
# 基本啟動（port 8000）
vllm serve Qwen/Qwen2.5-7B-Instruct

# 加上 API key 保護
vllm serve Qwen/Qwen2.5-7B-Instruct --api-key my-secret-key

# 指定 host 讓區網可以連
vllm serve Qwen/Qwen2.5-7B-Instruct --host 0.0.0.0 --port 8000
```

**用 curl 測試：**

```bash
curl http://localhost:8000/v1/chat/completions \
    -H "Content-Type: application/json" \
    -d '{
        "model": "Qwen/Qwen2.5-7B-Instruct",
        "messages": [{"role": "user", "content": "Hello!"}]
    }'
```

**用 Python OpenAI SDK 呼叫（直接換 base_url 就好）：**

```python
from openai import OpenAI

client = OpenAI(
    api_key="my-secret-key",  # 沒設 --api-key 就填任意字串
    base_url="http://localhost:8000/v1"
)

response = client.chat.completions.create(
    model="Qwen/Qwen2.5-7B-Instruct",
    messages=[{"role": "user", "content": "介紹 vLLM 是什麼"}]
)
print(response.choices[0].message.content)
```

---

## 多 GPU：Tensor Parallelism

模型太大放不進單張 GPU 時，用 `--tensor-parallel-size` 切分到多張：

```bash
# 70B 模型用 4 張 GPU 分攤
vllm serve meta-llama/Llama-3.1-70B-Instruct \
    --tensor-parallel-size 4 \
    --gpu-memory-utilization 0.9
```

> [!tip] GPU 數量要求
> `--tensor-parallel-size` 必須是 2 的冪次（2、4、8），且要整除模型的 attention heads 數量。
> 模型頁面通常會標示建議的 TP size。

---

## 量化（省 VRAM）

| 量化方式 | VRAM 節省 | 精度損失 | 適用情境 |
|---|---|---|---|
| **AWQ 4-bit** | ~75% | 極小（<1%） | 推薦首選 |
| **GPTQ 4-bit** | ~75% | 小 | 模型選擇較多 |
| **FP8** | ~50% | 幾乎無 | 需要 H100/H200 |

```bash
# 直接使用已量化的 AWQ 模型（HuggingFace 上有現成的）
vllm serve Qwen/Qwen2.5-7B-Instruct-AWQ --quantization awq

# GPTQ 量化模型
vllm serve TheBloke/Llama-2-13B-GPTQ --quantization gptq
```

> [!tip] 找量化模型
> HuggingFace 搜尋 `模型名稱 AWQ` 或 `GPTQ`，通常有社群預先量化好的版本。
> `TheBloke` 帳號有大量現成量化模型。

---

## Docker 部署（推薦生產環境）

### 單 GPU

```bash
docker run --runtime nvidia --gpus all \
    -v ~/.cache/huggingface:/root/.cache/huggingface \
    -e HF_TOKEN=hf_你的token \
    -p 8000:8000 \
    --ipc=host \
    vllm/vllm-openai:latest \
    --model Qwen/Qwen2.5-7B-Instruct \
    --host 0.0.0.0
```

### 多 GPU

```bash
docker run --runtime nvidia --gpus all \
    -v ~/.cache/huggingface:/root/.cache/huggingface \
    -e HF_TOKEN=hf_你的token \
    -p 8000:8000 \
    --ipc=host \
    vllm/vllm-openai:latest \
    --model meta-llama/Llama-3.1-70B-Instruct \
    --tensor-parallel-size 4 \
    --gpu-memory-utilization 0.9 \
    --host 0.0.0.0
```

### Docker Compose（長期服務）

```yaml
# docker-compose.yml
services:
  vllm:
    image: vllm/vllm-openai:latest
    runtime: nvidia
    environment:
      - HF_TOKEN=${HF_TOKEN}
    volumes:
      - ~/.cache/huggingface:/root/.cache/huggingface
    ports:
      - "8000:8000"
    ipc: host
    command: >
      --model Qwen/Qwen2.5-7B-Instruct
      --host 0.0.0.0
      --api-key ${VLLM_API_KEY}
      --gpu-memory-utilization 0.9
    deploy:
      resources:
        reservations:
          devices:
            - driver: nvidia
              count: all
              capabilities: [gpu]
```

```bash
# 啟動
HF_TOKEN=hf_xxx VLLM_API_KEY=my-key docker compose up -d
```

---

## 常用參數速查

| 參數 | 說明 | 預設值 |
|---|---|---|
| `--model` | HuggingFace 模型路徑或本地路徑 | 必填 |
| `--host` | 監聽位址 | `localhost` |
| `--port` | 監聽埠 | `8000` |
| `--api-key` | API 認證金鑰 | 無（不驗證）|
| `--tensor-parallel-size` | 切分到幾張 GPU | `1` |
| `--gpu-memory-utilization` | GPU 記憶體使用率上限 | `0.9` |
| `--max-model-len` | 最大 context 長度（token）| 模型預設 |
| `--quantization` | 量化方式：`awq`、`gptq`、`fp8` | 無 |
| `--dtype` | 計算精度：`auto`、`float16`、`bfloat16` | `auto` |
| `--max-num-seqs` | 最大並發 request 數 | `256` |

---

## 使用本地已下載的模型

```bash
# 直接指向本地路徑（避免重新下載）
vllm serve /path/to/local/model

# 搭配 HuggingFace cache
vllm serve ~/.cache/huggingface/hub/models--Qwen--Qwen2.5-7B-Instruct/snapshots/最新hash/
```

---

## vLLM vs Ollama

| | Ollama | vLLM |
|---|---|---|
| **安裝難度** | 超簡單（一行）| 需要 CUDA 環境 |
| **吞吐量** | 一般 | 高（生產級）|
| **並發支援** | 有限 | 優秀（設計目標之一）|
| **量化格式** | GGUF | AWQ、GPTQ、FP8 |
| **適用情境** | 個人開發、測試 | 團隊共用、生產部署 |
| **Windows** | ✅ | 需用 Docker |

> [!tip] 選哪個？
> 個人開發 → [[AI 101 - Ollama 指令教學|Ollama]]（更簡單）
> 對外提供 API、需要高吞吐量 → vLLM

---

## 常見問題

> [!warning] CUDA out of memory
> 減少 `--gpu-memory-utilization`（例如改為 `0.8`）或改用量化模型。
> 也可以降低 `--max-model-len` 縮小 KV cache 佔用。

> [!warning] 模型下載慢或被牆
> 設定環境變數切換到 ModelScope：
> ```bash
> export VLLM_USE_MODELSCOPE=True
> vllm serve Qwen/Qwen2.5-7B-Instruct
> ```

> [!warning] Windows 不支援直接安裝
> 請用 Docker 方案，或改用 WSL2（需要 CUDA in WSL2 環境）。

> [!warning] Llama 等 gated 模型需要 HF Token
> ```bash
> export HF_TOKEN=hf_你的token
> # 或加進 Docker -e HF_TOKEN=hf_xxx
> ```
> 先在 HuggingFace 網站申請該模型的存取權限。

---

## Sources

- [vLLM 官方文件 — Quickstart](https://docs.vllm.ai/en/latest/getting_started/quickstart.html)
- [vLLM OpenAI Compatible Server](https://docs.vllm.ai/en/latest/serving/openai_compatible_server.html)
- [vLLM Docker Deployment Guide — inference.net](https://inference.net/content/vllm-docker-deployment/)
- [vLLM Quantization 文件](https://docs.vllm.ai/en/latest/features/quantization/)
- [vLLM GitHub](https://github.com/vllm-project/vllm)

---

## 相關筆記

- [[AI 101 - Ollama 指令教學]] — 更簡單的本地模型方案
- [[AI 101 - 輕量模型推薦]] — 選哪個模型跑在 vLLM 上
- [[AI 101 - 模型費用與效果比較]] — 本地 vs 雲端成本分析
