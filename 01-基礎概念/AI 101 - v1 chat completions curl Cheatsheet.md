---
title: AI 101 - /v1/chat/completions curl Cheatsheet
tags: [ai, api, openai, curl, cheatsheet, http, rest, 工具, 實戰, openai-compat]
created: 2026-06-25
---

# /v1/chat/completions curl Cheatsheet

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> `/v1/chat/completions` 是業界最廣泛採用的 LLM API 格式——OpenAI 首創，Ollama、vLLM、Groq、Mistral 等幾十個提供者都相容，換個 URL 就能切換模型。這篇用 curl 示範所有主要用法，不需要任何 SDK，複製貼上就能跑。

---

## 各大提供者 Endpoint 速查

| 提供者 | Base URL | Auth 方式 |
|---|---|---|
| OpenAI | `https://api.openai.com/v1` | `Authorization: Bearer $OPENAI_API_KEY` |
| Ollama（本地）| `http://localhost:11434/v1` | 不需要 key |
| vLLM（本地）| `http://localhost:8000/v1` | `Authorization: Bearer $VLLM_TOKEN` |
| LM Studio（本地）| `http://localhost:1234/v1` | 不需要 key |
| Groq | `https://api.groq.com/openai/v1` | `Authorization: Bearer $GROQ_API_KEY` |
| Together AI | `https://api.together.xyz/v1` | `Authorization: Bearer $TOGETHER_API_KEY` |
| Mistral AI | `https://api.mistral.ai/v1` | `Authorization: Bearer $MISTRAL_API_KEY` |
| Perplexity | `https://api.perplexity.ai` | `Authorization: Bearer $PPLX_API_KEY` |
| Fireworks AI | `https://api.fireworks.ai/inference/v1` | `Authorization: Bearer $FIREWORKS_API_KEY` |

> [!warning]
> Anthropic Claude 原生 API 用 `/v1/messages`，格式完全不同。要用 Claude 請直接查 [[AI 101 - 核心概念]]。
> Anthropic 沒有官方的 `/v1/chat/completions` endpoint；若需要在 OpenAI-compat 框架下用 Claude，透過 LiteLLM 或 Ollama 代理是常見做法。

---

## Response 基本結構

理解 response 格式，後面每個範例都用這個結構取值。

```json
{
  "id": "chatcmpl-abc123",
  "object": "chat.completion",
  "created": 1750000000,
  "model": "gpt-4o-2024-11-20",
  "choices": [{
    "index": 0,
    "message": {
      "role": "assistant",
      "content": "回覆文字在這"
    },
    "finish_reason": "stop"
  }],
  "usage": {
    "prompt_tokens": 25,
    "completion_tokens": 150,
    "total_tokens": 175
  }
}
```

**用 jq 取出回覆文字：**

```bash
curl ... | jq -r '.choices[0].message.content'
```

---

## 1. 最基本請求

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "user", "content": "用一句話解釋什麼是 RAG？"}
    ],
    "max_tokens": 256
  }'
```

---

## 2. System Prompt

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "system", "content": "你是一個專業的 Python 程式師，回覆要簡潔，只給程式碼和必要說明。"},
      {"role": "user", "content": "怎麼用 requests 讀取 JSON API？"}
    ]
  }'
```

---

## 3. 多輪對話（Multi-turn）

API 是無狀態的。每次都要把**完整對話歷史**一起送出去，模型才「記得」之前說了什麼。

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "system", "content": "你是一位耐心的老師。"},
      {"role": "user", "content": "我叫 Alice，我想學 Python。"},
      {"role": "assistant", "content": "你好 Alice！Python 很適合初學者，我們從哪裡開始？"},
      {"role": "user", "content": "我叫什麼名字？"}
    ]
  }'
```

> [!tip]
> 實作多輪對話時，把每次的 user 訊息和 assistant 回覆都 append 到 messages array，再送下一輪。

---

## 4. Streaming（串流輸出）

加 `"stream": true`，response 改為 SSE（Server-Sent Events）格式，字一個一個出現，不需要等完整回覆。

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [{"role": "user", "content": "寫一首關於 AI 的詩"}],
    "stream": true
  }'
```

**SSE 格式長這樣：**

```
data: {"id":"chatcmpl-abc","choices":[{"delta":{"role":"assistant","content":""},"index":0}]}
data: {"id":"chatcmpl-abc","choices":[{"delta":{"content":"在"},"index":0}]}
data: {"id":"chatcmpl-abc","choices":[{"delta":{"content":"矽"},"index":0}]}
data: {"id":"chatcmpl-abc","choices":[{"delta":{"content":"基板上"}}]}
data: [DONE]
```

**即時印出文字（bash 一行）：**

```bash
curl -sN https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{"model":"gpt-4o","messages":[{"role":"user","content":"說個笑話"}],"stream":true}' \
  | grep -o '"content":"[^"]*"' \
  | sed 's/"content":"//;s/"$//'
```

---

## 5. Tool Use（Function Calling）

分兩步：先告訴模型有哪些工具 → 模型決定呼叫哪個 → 你執行工具 → 把結果送回模型 → 模型給最終回覆。

### Step 1 — 定義工具，送出第一次請求

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "user", "content": "台北現在幾度？"}
    ],
    "tools": [{
      "type": "function",
      "function": {
        "name": "get_weather",
        "description": "取得指定城市目前的天氣資訊",
        "parameters": {
          "type": "object",
          "properties": {
            "location": {
              "type": "string",
              "description": "城市名稱，例如 台北"
            },
            "unit": {
              "type": "string",
              "enum": ["celsius", "fahrenheit"],
              "description": "溫度單位"
            }
          },
          "required": ["location"]
        }
      }
    }],
    "tool_choice": "auto"
  }'
```

**模型回傳 tool call（`finish_reason: "tool_calls"`）：**

```json
{
  "choices": [{
    "message": {
      "role": "assistant",
      "content": null,
      "tool_calls": [{
        "id": "call_abc123",
        "type": "function",
        "function": {
          "name": "get_weather",
          "arguments": "{\"location\": \"台北\", \"unit\": \"celsius\"}"
        }
      }]
    },
    "finish_reason": "tool_calls"
  }]
}
```

### Step 2 — 執行工具後，送回結果

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "user", "content": "台北現在幾度？"},
      {
        "role": "assistant",
        "content": null,
        "tool_calls": [{
          "id": "call_abc123",
          "type": "function",
          "function": {
            "name": "get_weather",
            "arguments": "{\"location\": \"台北\"}"
          }
        }]
      },
      {
        "role": "tool",
        "tool_call_id": "call_abc123",
        "content": "{\"temperature\": 29, \"unit\": \"celsius\", \"condition\": \"晴天\"}"
      }
    ],
    "tools": [{
      "type": "function",
      "function": {
        "name": "get_weather",
        "description": "取得指定城市目前的天氣資訊",
        "parameters": {
          "type": "object",
          "properties": {
            "location": {"type": "string"}
          },
          "required": ["location"]
        }
      }
    }]
  }'
```

**`tool_choice` 選項：**

| 值 | 行為 |
|---|---|
| `"auto"` | 模型自行決定是否要呼叫工具（預設）|
| `"none"` | 禁止呼叫工具，強制純文字回覆 |
| `"required"` | 強制一定要呼叫工具 |
| `{"type":"function","function":{"name":"get_weather"}}` | 強制呼叫特定工具 |

---

## 6. Vision（圖片輸入）

支援 URL 或 Base64 兩種方式。

### URL 方式

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [{
      "role": "user",
      "content": [
        {"type": "text", "text": "這張圖片裡有什麼？"},
        {
          "type": "image_url",
          "image_url": {
            "url": "https://upload.wikimedia.org/wikipedia/commons/thumb/1/1e/Stonehenge.jpg/640px-Stonehenge.jpg",
            "detail": "auto"
          }
        }
      ]
    }]
  }'
```

### Base64 方式（本地圖片）

```bash
IMAGE_B64=$(base64 -w 0 screenshot.png)

curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d "{
    \"model\": \"gpt-4o\",
    \"messages\": [{
      \"role\": \"user\",
      \"content\": [
        {\"type\": \"text\", \"text\": \"描述這個截圖的 UI 問題\"},
        {
          \"type\": \"image_url\",
          \"image_url\": {
            \"url\": \"data:image/png;base64,${IMAGE_B64}\"
          }
        }
      ]
    }]
  }"
```

**`detail` 參數：**

| 值 | 效果 |
|---|---|
| `"auto"` | 模型自行決定（推薦）|
| `"low"` | 省 token，85 tokens，不分 tile |
| `"high"` | 高解析度，每 512px 切 tile，更貴但更準 |

---

## 7. JSON Mode（強制輸出 JSON）

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {
        "role": "system",
        "content": "你必須回傳 JSON 格式，不要有其他文字。"
      },
      {
        "role": "user",
        "content": "列出三個台灣城市，包含人口（萬人）。格式：{\"cities\": [{\"name\": ..., \"population\": ...}]}"
      }
    ],
    "response_format": {"type": "json_object"}
  }'
```

> [!warning]
> `json_object` 模式下，system prompt 裡一定要明確提到 JSON，否則 API 會報錯 `400 Bad Request`。

---

## 8. Structured Output（JSON Schema 嚴格約束）

比 JSON mode 更強：用 JSON Schema 定義精確格式，模型**保證**輸出符合 schema。

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o-2024-08-06",
    "messages": [
      {
        "role": "user",
        "content": "從這段話提取人名和職稱：Alice 是 CTO，Bob 是工程師，Carol 是設計師"
      }
    ],
    "response_format": {
      "type": "json_schema",
      "json_schema": {
        "name": "people_extraction",
        "strict": true,
        "schema": {
          "type": "object",
          "properties": {
            "people": {
              "type": "array",
              "items": {
                "type": "object",
                "properties": {
                  "name": {"type": "string"},
                  "title": {"type": "string"}
                },
                "required": ["name", "title"],
                "additionalProperties": false
              }
            }
          },
          "required": ["people"],
          "additionalProperties": false
        }
      }
    }
  }'
```

> [!tip]
> `strict: true` + `additionalProperties: false` + 所有欄位加進 `required` — 這三個條件都滿足才能啟用嚴格模式。

---

## 9. Logprobs（Token 機率）

回傳每個 token 的機率，用於不確定性估計、分類信心分數等。

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [{"role": "user", "content": "這句話是正面還是負面的？今天天氣很好。直接回答正面或負面。"}],
    "logprobs": true,
    "top_logprobs": 3,
    "max_tokens": 5
  }'
```

---

## 10. 一次產生多個回覆（n）

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [{"role": "user", "content": "幫我想一個 SaaS 產品名稱"}],
    "n": 3,
    "temperature": 1.2
  }'
```

> [!warning]
> `n` 個回覆都算 token 費用，`n=3` 就是 3 倍 output token 費用。

---

## 11. 可重現輸出（seed）

相同的 `seed` + 相同的參數 → 極高機率得到相同回覆（不保證 100%）。

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [{"role": "user", "content": "給我一個隨機數字（1-100）"}],
    "seed": 42,
    "temperature": 0
  }'
```

---

## 12. 切換到不同提供者

換 URL 和 `model` 名稱，其餘格式不變。

### Ollama（本地，完全免費）

```bash
curl http://localhost:11434/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "llama3.2",
    "messages": [{"role": "user", "content": "你好"}]
  }'
```

### vLLM（本地高效能推論）

```bash
curl http://localhost:8000/v1/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "meta-llama/Llama-3.2-8B-Instruct",
    "messages": [{"role": "user", "content": "你好"}]
  }'
```

### Groq（超快，latency < 300ms）

```bash
curl https://api.groq.com/openai/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $GROQ_API_KEY" \
  -d '{
    "model": "llama-3.3-70b-versatile",
    "messages": [{"role": "user", "content": "你好"}]
  }'
```

---

## 13. 參數速查表

| 參數 | 型別 | 預設 | 說明 |
|---|---|---|---|
| `model` | string | **必填** | 模型 ID，例如 `gpt-4o`、`llama3.2` |
| `messages` | array | **必填** | 對話歷史，至少一筆 |
| `max_tokens` | int | 依模型 | 最多產生幾個 token |
| `temperature` | float | 1.0 | 0–2，越高越有創意，越低越穩定 |
| `top_p` | float | 1.0 | 0–1，nucleus sampling，與 temperature 擇一調整 |
| `n` | int | 1 | 一次產生幾個回覆 |
| `stream` | bool | false | SSE 串流輸出 |
| `stop` | string/array | null | 遇到這個字串立刻停止 |
| `presence_penalty` | float | 0 | -2 到 2，鼓勵談新話題（正值減少重複主題）|
| `frequency_penalty` | float | 0 | -2 到 2，減少逐字重複（正值降低重複 token）|
| `seed` | int | null | 固定 seed 讓輸出可重現 |
| `user` | string | null | 終端使用者 ID（計費追蹤/安全）|
| `tools` | array | null | 定義可用的工具（函式）|
| `tool_choice` | string/object | `"auto"` | 控制工具呼叫行為 |
| `response_format` | object | null | `json_object` 或 `json_schema` |
| `logprobs` | bool | false | 回傳 token 機率 |
| `top_logprobs` | int | null | 每個位置回傳前 N 個 token 的機率（最多 20）|

---

## 14. Finish Reason 速查

`choices[0].finish_reason` — 模型為什麼停下來：

| 值 | 意義 |
|---|---|
| `"stop"` | 正常結束（遇到 EOS 或 stop sequence）|
| `"length"` | 達到 `max_tokens` 上限，回覆可能不完整 |
| `"tool_calls"` | 模型想呼叫工具，等你送回結果 |
| `"content_filter"` | 被安全過濾擋下 |
| `null` | 串流中（尚未結束）|

---

## 15. 實用 Shell 函式

加到 `~/.bashrc` 或 `~/.zshrc`：

```bash
# 快速問 OpenAI
ask() {
  curl -s https://api.openai.com/v1/chat/completions \
    -H "Content-Type: application/json" \
    -H "Authorization: Bearer $OPENAI_API_KEY" \
    -d "{\"model\":\"gpt-4o-mini\",\"messages\":[{\"role\":\"user\",\"content\":\"$*\"}]}" \
  | jq -r '.choices[0].message.content'
}
# 用法：ask 什麼是 RAG？

# 快速問本地 Ollama
ollama_ask() {
  local model="${OLLAMA_MODEL:-llama3.2}"
  curl -s http://localhost:11434/v1/chat/completions \
    -H "Content-Type: application/json" \
    -d "{\"model\":\"$model\",\"messages\":[{\"role\":\"user\",\"content\":\"$*\"}]}" \
  | jq -r '.choices[0].message.content'
}
# 用法：ollama_ask 解釋 MCP 協議
# 用法：OLLAMA_MODEL=gemma3:12b ollama_ask 你好
```

---

## 相關筆記

- [[AI 101 - Ollama 指令教學]] — 本地跑模型，完全離線，也支援這個格式
- [[AI 101 - vLLM]] — 高效能推論 server，OpenAI 相容 API
- [[AI 101 - 模型費用與效果比較]] — 選哪個模型最划算

## Sources

- [OpenAI API Reference - Chat Completions](https://platform.openai.com/docs/api-reference/chat)
- [OpenAI Guide - Function Calling](https://platform.openai.com/docs/guides/function-calling)
- [OpenAI Guide - Vision](https://platform.openai.com/docs/guides/vision)
- [OpenAI Guide - Structured Outputs](https://platform.openai.com/docs/guides/structured-outputs)
- [OpenAI Guide - Streaming](https://platform.openai.com/docs/api-reference/streaming)
- [Ollama OpenAI Compatibility](https://ollama.com/blog/openai-compatibility)
