---
title: AI 101 - Claude Code chrome 進階操作
tags: [ai, claude-code, chrome, browser-automation, mcp, tab-group, browser-batch, 進階]
created: 2026-07-02
---

# Claude Code chrome 進階操作

[← 回主頁](../../index.md)

> [!info]
> 這篇是 [AI 101 - Claude Code chrome 瀏覽器整合](./chrome-integration.md) 的進階篇，專注在「開始執行瀏覽器任務之後」Claude 實際怎麼工作：工具怎麼載入、多瀏覽器怎麼選、Tab Group 怎麼管理、怎麼用 `browser_batch` 加速。

---

## 工具延遲載入（Deferred Tools）

Claude Code 的瀏覽器工具**不是一開始就載入**的——它們是「延遲工具」（deferred tools），必須先用 `ToolSearch` 拉取 schema 才能呼叫，否則會拋出 `InputValidationError`。

### 一次載入所有需要的工具

每次開始瀏覽器任務，在第一次呼叫任何 `mcp__claude-in-chrome__*` 工具之前，用**一次** `ToolSearch` 把這次任務需要的工具全部載入：

```
ToolSearch: select:mcp__claude-in-chrome__tabs_context_mcp,mcp__claude-in-chrome__navigate,mcp__claude-in-chrome__computer,mcp__claude-in-chrome__read_page,mcp__claude-in-chrome__tabs_create_mcp
```

按任務需求加入額外工具：

| 場景 | 需要額外載入 |
|---|---|
| Debug 網頁錯誤 | `read_console_messages`, `read_network_requests` |
| 填表單 | `form_input` |
| 錄製示範 | `gif_creator` |
| 執行 JavaScript | `javascript_tool` |
| 多瀏覽器管理 | `list_connected_browsers`, `select_browser` |

> [!warning]
> 不要一次只載入一個工具——每個 `ToolSearch` call 浪費一個完整 round-trip。把所有需要的工具放在同一個 `select:` query 裡。

---

## 多瀏覽器管理

當環境中有多個 Chrome 實例（例如不同機器的瀏覽器都裝了 extension），可以明確指定要連哪一個。

### 列出所有已連線瀏覽器

```
mcp__claude-in-chrome__list_connected_browsers
```

回傳每個瀏覽器的：

| 欄位 | 說明 |
|---|---|
| `deviceId` | 唯一識別碼，用來選擇此瀏覽器 |
| `name` | 顯示名稱（如 "Browser 1"）|
| `osPlatform` | 作業系統（macOS / Linux / Windows）|
| `connectedAt` | 連線時間戳（毫秒）|
| `isLocal` | 是否為本機瀏覽器 |

### 選擇特定瀏覽器

```
mcp__claude-in-chrome__select_browser
  deviceId: "e40cf948-dd20-4fc2-92a5-0deffb583105"
```

選定之後，後續所有 `tabs_context_mcp`、`navigate`、`computer` 等工具都會作用在這個瀏覽器上。

> [!tip]
> 多瀏覽器場景必須在呼叫 `tabs_context_mcp` **之前**先完成 `select_browser`，否則連線目標可能是上次 session 的殘留值。

---

## Tab Group 生命週期

Claude 在 Chrome 裡透過「Tab Group」來管理分頁，每個 Claude Code session 應該有自己獨立的 tab group。

### 標準啟動流程

```
1. list_connected_browsers  →  選擇目標瀏覽器
2. select_browser           →  綁定瀏覽器
3. tabs_context_mcp         →  取得現有 tab group 狀態
4. tabs_create_mcp          →  建立新分頁（如果需要）
5. navigate                 →  前往目標頁面
```

### `tabs_context_mcp` 的三個關鍵行為

```
# 情況 1：已有 tab group → 直接回傳現有 tab IDs
tabs_context_mcp {}

# 情況 2：沒有 tab group，想自動建立
tabs_context_mcp { createIfEmpty: true }

# 情況 3：不確定狀態，先查再決定
tabs_context_mcp {}
# 如果回傳 "No tab group exists" → 再呼叫 createIfEmpty: true
```

> [!warning]
> **絕對不要重用舊的 Tab ID。** 每個 session 的 Tab ID 是獨立的，跨 session 使用會導致 "tab does not exist" 錯誤。如果工具回傳 tab 不存在，立刻呼叫 `tabs_context_mcp` 取得最新狀態。

### 建立新分頁

```
mcp__claude-in-chrome__tabs_create_mcp
```

這個工具會在目前的 tab group 裡新增一個空白分頁，並回傳新的 `tabId`。

---

## `browser_batch` 效能優化

`browser_batch` 可以把多個瀏覽器操作合併成一個 call，大幅減少 round-trip。

### 什麼時候用

- 連續的截圖 → 等待 → 再截圖
- 點擊一連串元素
- 先 navigate 再立刻 screenshot

### 範例

單獨呼叫（慢）：
```
navigate { url: "https://example.com", tabId: 123 }
→ 等待
computer { action: "wait", duration: 2, tabId: 123 }
→ 等待
computer { action: "screenshot", tabId: 123 }
```

用 `browser_batch`（快）：
```
browser_batch [
  { tool: "navigate", params: { url: "https://example.com", tabId: 123 } },
  { tool: "computer", params: { action: "wait", duration: 2, tabId: 123 } },
  { tool: "computer", params: { action: "screenshot", tabId: 123 } }
]
```

> [!tip]
> 截圖前後各加一個 `wait` frame，確保頁面完全渲染，GIF 播放也會更順暢。

---

## 完整 MCP 工具清單

用 `/mcp` → 選 `claude-in-chrome` 可在 session 內查看。以下是所有已知工具：

| 工具 | 功能 |
|---|---|
| `tabs_context_mcp` | 取得目前 tab group 狀態，必須最先呼叫 |
| `tabs_create_mcp` | 在 tab group 裡建立新分頁 |
| `tabs_close_mcp` | 關閉指定分頁 |
| `navigate` | 導航到 URL，或前進／後退 |
| `computer` | 滑鼠點擊、鍵盤輸入、截圖、滾動、拖曳等 |
| `read_page` | 讀取頁面 accessibility tree（比截圖更快取得文字內容）|
| `get_page_text` | 取得頁面純文字 |
| `find` | 在頁面中尋找元素 |
| `form_input` | 填寫表單欄位 |
| `javascript_tool` | 在 console 執行 JavaScript |
| `read_console_messages` | 讀取 console 輸出（支援 regex pattern 過濾）|
| `read_network_requests` | 讀取網路請求 |
| `gif_creator` | 錄製互動過程為 GIF |
| `list_connected_browsers` | 列出所有已連線的瀏覽器實例 |
| `select_browser` | 選擇特定瀏覽器（by deviceId）|
| `switch_browser` | 廣播配對請求，讓使用者在 extension 介面手動選擇 |
| `resize_window` | 調整視窗大小 |
| `upload_image` | 上傳圖片到頁面 |
| `file_upload` | 觸發檔案上傳 |
| `browser_batch` | 批次執行多個瀏覽器操作 |
| `shortcuts_list` | 列出已儲存的 workflow shortcuts |
| `shortcuts_execute` | 執行已儲存的 workflow shortcut |

---

## 實戰踩坑記錄

### 1. Modal Dialog 會讓一切停下來

JavaScript `alert()`、`confirm()`、`prompt()` 會阻塞所有瀏覽器事件，Claude 再也收不到任何回應。

避免辦法：
- 不要點擊會觸發 dialog 的按鈕（如刪除確認）
- 如果必須點，先告知使用者可能中斷
- 用 `javascript_tool` 偵測並清除現有 dialog

### 2. 帳號不一致時 extension 靜默失敗

Claude Code CLI 的登入帳號和 Chrome extension 的帳號不同時，extension 不會報錯，只是靜默連線失敗。症狀：`/chrome` 顯示已連，但工具呼叫沒有反應。解法：確認兩邊登入同一個帳號。

### 3. Service Worker 閒置超時

長時間沒有互動後，Chrome extension 的 service worker 會被系統暫停。症狀：工具呼叫超時或沒有回應。解法：`/chrome` → "Reconnect extension"。

### 4. `read_page` 比截圖更適合讀文字

截圖需要 vision model 解析，`read_page` 直接回傳 accessibility tree（純文字，帶 ref ID），速度更快、消耗更少 token。只有需要視覺確認版面或圖片內容時才用截圖。

### 5. 用 `computer zoom` 看清楚小元素

```
computer { action: "zoom", region: [x0, y0, x1, y1], tabId: ... }
```

放大指定區域截圖，用來確認 icon、按鈕、小字體的精確位置，再決定點哪裡。

---

## 方案與模型限制

| 方案 | 可用模型 |
|---|---|
| Pro | Haiku 4.5 |
| Max / Team / Enterprise | Opus 4.7、Sonnet 4.6、Haiku 4.5 |

---

## 相關筆記

- [AI 101 - Claude Code chrome 瀏覽器整合](./chrome-integration.md) — 基礎安裝、架構、初始設定
- [AI 101 - Claude Code 生態系](./ecosystem.md) — Skills、Hooks、MCP 的關係

## Sources

- [Use Claude Code with Chrome (beta) — 官方文件](https://code.claude.com/docs/en/chrome)
- [Get started with Claude in Chrome — Anthropic Support](https://support.claude.com/en/articles/12012173-get-started-with-claude-in-chrome)
- [Parallel Browser Agents: How to Run Multiple Claude Code Instances Simultaneously — MindStudio](https://www.mindstudio.ai/blog/parallel-browser-agents-claude-code)
- [Claude --chrome connects to wrong browser when multiple Chrome instances on LAN — GitHub Issue #25551](https://github.com/anthropics/claude-code/issues/25551)
