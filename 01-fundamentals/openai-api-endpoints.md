---
title: AI 101 - OpenAI API 完整端點速查
tags: [ai, api, openai, curl, cheatsheet, embeddings, batch, audio, moderations, responses, 工具, 實戰]
created: 2026-06-25
---

# OpenAI API 完整端點速查

[← 回主頁](../index.md)

> [!info]
> `/v1/chat/completions` 只是冰山一角。OpenAI API 還有 Embeddings（RAG 必備）、Batch（省 50% 費用）、Audio（Whisper STT + TTS）、Responses（新一代有狀態 API）、Moderations（內容安全）等。這篇用 curl 示範每個端點的核心用法，幫你發現你可能沒注意到的功能。

---

## 端點全覽

| 端點 | 用途 | 你可能不知道的是⋯ |
|---|---|---|
| `GET /v1/models` | 列出可用模型 | 可以動態查詢，不用死背 model ID |
| `POST /v1/embeddings` | 文字轉向量 | RAG 的核心，便宜到幾乎可以忽略費用 |
| `POST /v1/batches` | 非同步批次處理 | 比同步便宜 **50%**，24h 內完成 |
| `POST /v1/files` | 上傳檔案 | Batch/Fine-tuning 的輸入用 |
| `POST /v1/audio/transcriptions` | 語音轉文字（Whisper）| 支援 100+ 語言，台語也行 |
| `POST /v1/audio/speech` | 文字轉語音（TTS）| 6 種聲音，每秒 < 200ms |
| `POST /v1/images/generations` | 圖片生成（DALL-E 3）| 可以指定 size、quality、style |
| `POST /v1/moderations` | 內容安全過濾 | **免費**，過濾違規內容 |
| `POST /v1/responses` | 新一代有狀態 API | 2025 起 OpenAI 主力 API，取代 Assistants |
| `POST /v1/fine_tuning/jobs` | 微調模型 | 讓模型學你的風格/格式 |

> [!warning]
> **Responses API（`/v1/responses`）是 OpenAI 2025 年後的主推方向**，Assistants API 已進入 deprecation 路徑。新專案建議直接用 Responses API。

---

## 1. GET /v1/models — 查詢可用模型

不用死背 model ID，直接問 API。

```bash
# 列出所有可用模型
curl https://api.openai.com/v1/models \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  | jq '[.data[] | {id: .id, created: .created}] | sort_by(.created) | reverse | .[0:10]'

# 查詢特定模型詳細資訊
curl https://api.openai.com/v1/models/gpt-4o \
  -H "Authorization: Bearer $OPENAI_API_KEY"
```

**在本地 Ollama 也適用：**

```bash
# 列出本地已下載的模型
curl http://localhost:11434/v1/models \
  | jq '[.data[].id]'
```

---

## 2. POST /v1/embeddings — 文字轉向量（RAG 必備）

把文字轉成向量，用來做語意搜尋、相似度比較、RAG 的知識庫索引。

```bash
# 單筆文字轉向量
curl https://api.openai.com/v1/embeddings \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "text-embedding-3-small",
    "input": "MCP 是 AI 工具整合的標準協議"
  }'
```

**一次送多筆（更有效率）：**

```bash
curl https://api.openai.com/v1/embeddings \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "text-embedding-3-small",
    "input": [
      "Context Engineering 是 2026 年最重要的技能",
      "Prompt Engineering 只是開始",
      "MCP 是 AI 的 USB-C"
    ],
    "encoding_format": "float"
  }'
```

**Response 結構：**

```json
{
  "object": "list",
  "data": [{
    "object": "embedding",
    "index": 0,
    "embedding": [0.0023064255, -0.009327292, ...]
  }],
  "model": "text-embedding-3-small",
  "usage": {"prompt_tokens": 8, "total_tokens": 8}
}
```

**壓縮向量維度（省儲存空間，還是比 ada-002 強）：**

```bash
curl https://api.openai.com/v1/embeddings \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "text-embedding-3-large",
    "input": "你的文字",
    "dimensions": 256
  }'
```

**模型比較：**

| 模型 | 維度 | 特性 |
|---|---|---|
| `text-embedding-3-small` | 1536 | 便宜、夠用，**推薦預設** |
| `text-embedding-3-large` | 3072（可壓縮）| 精度最高，支援 `dimensions` 壓縮 |
| `text-embedding-ada-002` | 1536 | 舊版，別用了 |

> [!tip]
> 語意搜尋流程：把所有文件用 embeddings 轉向量存到向量資料庫（pgvector、ChromaDB、Pinecone）→ 查詢時把問題也轉向量 → 找 cosine similarity 最近的幾筆 → 塞進 chat/completions context。

---

## 3. Batch API — 非同步批次處理（省 50% 費用）

不需要即時回覆的任務（資料標注、大量摘要、離線分析），用 Batch API 可以省一半費用。

### 流程：準備 → 上傳 → 建立批次 → 等待 → 下載結果

**Step 1 — 準備輸入檔案（JSONL 格式）**

每行是一個獨立請求，`custom_id` 用來對應輸出：

```bash
cat > batchinput.jsonl << 'EOF'
{"custom_id": "req-001", "method": "POST", "url": "/v1/chat/completions", "body": {"model": "gpt-4o-mini", "messages": [{"role": "user", "content": "用一句話描述 RAG"}], "max_tokens": 100}}
{"custom_id": "req-002", "method": "POST", "url": "/v1/chat/completions", "body": {"model": "gpt-4o-mini", "messages": [{"role": "user", "content": "用一句話描述 MCP"}], "max_tokens": 100}}
{"custom_id": "req-003", "method": "POST", "url": "/v1/chat/completions", "body": {"model": "gpt-4o-mini", "messages": [{"role": "user", "content": "用一句話描述 Context Engineering"}], "max_tokens": 100}}
EOF
```

**Step 2 — 上傳檔案**

```bash
FILE_ID=$(curl -s https://api.openai.com/v1/files \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -F purpose="batch" \
  -F file="@batchinput.jsonl" \
  | jq -r '.id')

echo "File ID: $FILE_ID"
```

**Step 3 — 建立批次任務**

```bash
BATCH_ID=$(curl -s https://api.openai.com/v1/batches \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d "{
    \"input_file_id\": \"$FILE_ID\",
    \"endpoint\": \"/v1/chat/completions\",
    \"completion_window\": \"24h\"
  }" | jq -r '.id')

echo "Batch ID: $BATCH_ID"
```

**Step 4 — 查詢狀態**

```bash
curl https://api.openai.com/v1/batches/$BATCH_ID \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  | jq '{status: .status, completed: .request_counts.completed, total: .request_counts.total}'
```

狀態：`validating` → `in_progress` → `completed`（或 `failed`）

**Step 5 — 下載結果**

```bash
OUTPUT_FILE_ID=$(curl -s https://api.openai.com/v1/batches/$BATCH_ID \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  | jq -r '.output_file_id')

curl https://api.openai.com/v1/files/$OUTPUT_FILE_ID/content \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  > batch_output.jsonl

# 從輸出取出每個回覆
cat batch_output.jsonl | jq -r '.custom_id + ": " + .response.body.choices[0].message.content'
```

**限制：**

| 項目 | 限制 |
|---|---|
| 每批次最多請求數 | 50,000 筆 |
| 輸入檔案大小 | 200 MB |
| 完成時限 | 24 小時 |
| 費用折扣 | **50% off** 同步價格 |

---

## 4. Audio — 語音轉文字（Whisper）

### `/v1/audio/transcriptions` — 語音轉文字

```bash
# 上傳音訊檔轉文字（支援 mp3、mp4、m4a、wav、webm 等）
curl https://api.openai.com/v1/audio/transcriptions \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -F model="whisper-1" \
  -F file="@meeting.mp3"
```

**指定語言（更準確）：**

```bash
curl https://api.openai.com/v1/audio/transcriptions \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -F model="whisper-1" \
  -F file="@meeting.mp3" \
  -F language="zh" \
  -F response_format="verbose_json"
```

**`response_format` 選項：**

| 值 | 輸出 |
|---|---|
| `json`（預設）| `{"text": "..."}` |
| `text` | 純文字 |
| `srt` | 字幕格式 |
| `vtt` | WebVTT 字幕格式 |
| `verbose_json` | 含時間戳、語言、每段 token |

### `/v1/audio/translations` — 翻譯成英文

```bash
# 任何語言的音訊直接翻成英文
curl https://api.openai.com/v1/audio/translations \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -F model="whisper-1" \
  -F file="@chinese_speech.mp3"
```

---

## 5. Audio — 文字轉語音（TTS）

### `/v1/audio/speech`

```bash
curl https://api.openai.com/v1/audio/speech \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "tts-1",
    "input": "歡迎使用 AI 101 知識庫，今天我們來聊聊 Context Engineering。",
    "voice": "alloy",
    "response_format": "mp3"
  }' \
  --output speech.mp3
```

**`voice` 選項：** `alloy`、`echo`、`fable`、`onyx`、`nova`、`shimmer`

**`model` 選項：**

| 模型 | 特性 |
|---|---|
| `tts-1` | 即時，latency 低，品質一般 |
| `tts-1-hd` | 高品質，latency 稍高 |

---

## 6. `/v1/images/generations` — 圖片生成（DALL-E 3）

```bash
curl https://api.openai.com/v1/images/generations \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "dall-e-3",
    "prompt": "一隻戴著太陽眼鏡的機器人坐在台北 101 前面喝珍奶，賽博龐克風格",
    "n": 1,
    "size": "1024x1024",
    "quality": "standard",
    "response_format": "url"
  }'
```

**參數：**

| 參數 | 選項 |
|---|---|
| `size` | `1024x1024`、`1792x1024`（橫）、`1024x1792`（直）|
| `quality` | `standard`（快/便宜）、`hd`（高細節）|
| `style` | `vivid`（鮮豔預設）、`natural`（更自然真實）|
| `response_format` | `url`（有效期限 60 分鐘）、`b64_json` |

---

## 7. `/v1/moderations` — 內容安全過濾（免費）

送出任何文字，API 判斷是否違規。**完全免費。**

```bash
curl https://api.openai.com/v1/moderations \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "omni-moderation-latest",
    "input": "I want to hurt someone"
  }'
```

**Response：**

```json
{
  "id": "modr-abc123",
  "results": [{
    "flagged": true,
    "categories": {
      "violence": true,
      "hate": false,
      "self-harm": false,
      "sexual": false,
      "harassment": false
    },
    "category_scores": {
      "violence": 0.9823,
      "hate": 0.0012
    }
  }]
}
```

> [!tip]
> 在讓使用者的輸入進入 LLM 之前先過 moderations，可以擋掉明顯違規，避免觸發模型的安全過濾器或浪費 token。

---

## 8. Responses API — 新一代有狀態 API

> [!info]
> OpenAI 2025 年推出 `/v1/responses`，是 Chat Completions + Assistants 的繼承者。核心差異：**伺服器端維護對話狀態**、**內建工具**（web search、code interpreter、file search）。新專案建議優先考慮。

### 基本請求

```bash
curl https://api.openai.com/v1/responses \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "input": "台灣最高的山是哪座？"
  }'
```

### 有狀態對話（`store: true`，伺服器記憶）

```bash
# 第一輪
RESPONSE=$(curl -s https://api.openai.com/v1/responses \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "store": true,
    "input": "我叫 Alice，我想學 Python。"
  }')

RESPONSE_ID=$(echo $RESPONSE | jq -r '.id')

# 第二輪：只送新訊息，不用重送全部歷史
curl https://api.openai.com/v1/responses \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d "{
    \"model\": \"gpt-4o\",
    \"previous_response_id\": \"$RESPONSE_ID\",
    \"input\": \"我叫什麼名字？\"
  }"
```

### 內建 Web Search 工具

```bash
curl https://api.openai.com/v1/responses \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "tools": [{"type": "web_search_preview"}],
    "input": "2026 年 Claude 最新的模型是什麼？"
  }'
```

### Responses vs Chat Completions 比較

| | `/v1/chat/completions` | `/v1/responses` |
|---|---|---|
| 對話狀態 | 你自己維護，每次送全部歷史 | `store:true` → 伺服器幫你記 |
| 內建工具 | 無 | web search、code interpreter、file search |
| 成本 | 基準 | 快取利用率提升 40-80%，通常更便宜 |
| 推理保留 | 每輪清掉 | 推理跨輪保留，推理模型效能更好 |
| 相容性 | 幾乎所有提供者都支援 | 僅 OpenAI 原生 |

---

## 9. Rate Limit Headers — 你用了多少配額

每次 API 回應都帶有 rate limit 資訊，用 `-v` 或 `-D` 看 headers：

```bash
curl -s -D - https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{"model":"gpt-4o-mini","messages":[{"role":"user","content":"hi"}]}' \
  | grep -i "x-ratelimit"
```

**重要 Headers：**

| Header | 意義 |
|---|---|
| `x-ratelimit-limit-requests` | 每分鐘最大請求數 |
| `x-ratelimit-remaining-requests` | 這分鐘還剩幾個請求 |
| `x-ratelimit-limit-tokens` | 每分鐘最大 token 數 |
| `x-ratelimit-remaining-tokens` | 這分鐘還剩多少 token |
| `x-ratelimit-reset-requests` | 幾秒後重置請求配額 |
| `x-ratelimit-reset-tokens` | 幾秒後重置 token 配額 |

---

## 10. 錯誤代碼速查

| HTTP Code | 意義 | 怎麼處理 |
|---|---|---|
| `400` | 請求格式錯誤 | 檢查 JSON body、必填欄位 |
| `401` | API key 無效或過期 | 確認 key 是否正確 |
| `403` | 沒有這個功能的權限 | 確認帳號 tier 是否支援 |
| `404` | 模型不存在 | 確認 model ID |
| `429` | Rate limit 或 quota 超限 | 等待 `retry-after` header 指定的秒數 |
| `500` | OpenAI 伺服器錯誤 | 重試，並考慮 exponential backoff |
| `503` | 服務暫時不可用 | 重試 |

**自動重試 shell 函式（帶 exponential backoff）：**

```bash
call_with_retry() {
  local max_retries=3
  local delay=1
  for i in $(seq 1 $max_retries); do
    response=$(curl -s -w "\n%{http_code}" "$@")
    http_code=$(echo "$response" | tail -1)
    body=$(echo "$response" | head -n -1)
    
    if [ "$http_code" = "200" ]; then
      echo "$body"
      return 0
    elif [ "$http_code" = "429" ] || [ "$http_code" = "503" ]; then
      echo "Retry $i/$max_retries after ${delay}s (HTTP $http_code)..." >&2
      sleep $delay
      delay=$((delay * 2))
    else
      echo "Error HTTP $http_code: $body" >&2
      return 1
    fi
  done
  echo "Max retries exceeded" >&2
  return 1
}

# 用法：
call_with_retry https://api.openai.com/v1/chat/completions \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -H "Content-Type: application/json" \
  -d '{"model":"gpt-4o","messages":[{"role":"user","content":"hi"}]}'
```

---

## 11. 快速 Token 計算（送出前估費用）

`/v1/chat/completions` 送出前可以先數 token，不會被計費：

```bash
curl https://api.openai.com/v1/chat/completions \
  -H "Content-Type: application/json" \
  -H "Authorization: Bearer $OPENAI_API_KEY" \
  -d '{
    "model": "gpt-4o",
    "messages": [
      {"role": "system", "content": "你是一個助理"},
      {"role": "user", "content": "解釋一下什麼是 RAG"}
    ]
  }' \
  | jq '.usage'
```

> [!tip]
> 回傳的 `usage.prompt_tokens` 就是這個 context 的 token 數。Anthropic 也有獨立的 `POST /v1/messages/count_tokens` endpoint 可以在送出前計算，不需要真的執行。

---

## 相關筆記

- [AI 101 - v1 chat completions curl Cheatsheet](./chat-completions-curl-cheatsheet.md) — `/v1/chat/completions` 完整用法（streaming、tool use、vision）
- [AI 101 - Ollama 指令教學](../04-local-llm/ollama-guide.md) — 本地模型，部分端點相容 OpenAI API
- [AI 101 - vLLM](../04-local-llm/vllm.md) — 高效能推論 server，相容 `/v1/chat/completions` 和 `/v1/embeddings`
- [AI 101 - 模型費用與效果比較](./model-cost-comparison.md) — 選哪個模型最划算

## Sources

- [OpenAI API Reference Overview](https://developers.openai.com/api/reference/overview)
- [OpenAI Embeddings Guide](https://developers.openai.com/api/docs/guides/embeddings)
- [OpenAI Batch API Guide](https://developers.openai.com/api/docs/guides/batch)
- [OpenAI Audio Guide](https://platform.openai.com/docs/guides/audio)
- [Why we built the Responses API](https://developers.openai.com/blog/responses-api)
- [Migrate to the Responses API](https://developers.openai.com/api/docs/guides/migrate-to-responses)
