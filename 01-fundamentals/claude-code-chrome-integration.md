---
title: AI 101 - Claude Code chrome 瀏覽器整合
tags: [ai, claude-code, chrome, browser-automation, cdp, mcp, 瀏覽器]
created: 2026-06-24
---

# Claude Code chrome 瀏覽器整合

[← 回主頁](../index.md)

> [!info]
> `claude --chrome` 讓 Claude Code CLI 直接控制你的 Chrome 瀏覽器——截圖、點擊、填表、讀 console——而且全程用你自己已登入的帳號，不需要任何 API 金鑰或額外服務。這篇說明怎麼裝、怎麼用、會踩到什麼坑。

---

## 這是什麼

`--chrome` 是 Claude Code 官方（Anthropic 原生）的瀏覽器整合功能（beta），透過 **Chrome Native Messaging Host + Chrome DevTools Protocol（CDP）** 連接 CLI 與瀏覽器。

技術架構：

```
claude CLI  ←→  native messaging host  ←→  Claude in Chrome extension  ←→  Chrome/Edge
```

和第三方 MCP 瀏覽器工具（Puppeteer MCP、Chrome DevTools MCP）的差別：
- `--chrome` 是 Anthropic 官方實作，裝好就用，不需要另起 MCP 伺服器
- 第三方 MCP 工具是社群維護的獨立 server，需要手動設定

---

## 版本需求

| 元件 | 最低版本 |
|---|---|
| Claude Code CLI | v2.0.73 |
| Claude in Chrome extension | v1.0.36 |
| 瀏覽器 | Chrome 或 Edge（最新版）|
| 帳號 | 需要付費方案（Pro / Max / Team / Enterprise）|

> [!warning]
> **Bedrock、Vertex AI、Foundry 用戶不支援。** 只有直接向 Anthropic 訂閱的付費方案可以使用。

---

## 安裝步驟

### Step 1 — 安裝 Chrome extension

前往 Chrome Web Store，搜尋 **Claude** 並安裝「Claude in Chrome」（Anthropic 官方），或直接用連結：

```
https://chromewebstore.google.com/detail/claude/fcoeoabgfenejglbffodgkkbkcdhcgfn
```

### Step 2 — 第一次啟動

```bash
claude --chrome
```

首次執行時，Claude Code 會自動把 native messaging host 設定檔寫入系統：

| 系統 | 設定檔位置 |
|---|---|
| macOS（Chrome）| `~/Library/Application Support/Google/Chrome/NativeMessagingHosts/com.anthropic.claude_code_browser_extension.json` |
| Linux（Chrome）| `~/.config/google-chrome/NativeMessagingHosts/com.anthropic.claude_code_browser_extension.json` |
| Windows（Chrome）| 登錄檔 `HKCU\Software\Google\Chrome\NativeMessagingHosts\` |

### Step 3 — 重啟 Chrome

第一次安裝後**必須重啟 Chrome**，讓它載入新的 native messaging host 設定。

### 驗證連線

進入 session 後執行：

```
/chrome
```

顯示 "Browser extension is connected" 表示成功。

---

## 怎麼使用

### 每次啟動時開啟

```bash
claude --chrome
```

### 設定成預設（每個 session 自動開啟）

在 session 內執行：

```
/chrome
```

選擇 "Enabled by default"。

> [!warning]
> 預設開啟會增加 context window 消耗，因為瀏覽器工具一直掛在 context 裡。如果 context 吃緊，改成需要時才加 `--chrome`。

### session 內管理

`/chrome` 指令在 session 中可以：
- 查看連線狀態
- 管理工具權限
- 切換要連接的瀏覽器
- 重新連線（Reconnect extension）
- 選擇哪個 Chrome/Edge profile

---

## 可以做什麼（18 個 MCP 工具）

extension 透過 MCP 協議對 Claude 暴露 18 個工具，涵蓋：

| 類別 | 能力 |
|---|---|
| **視覺** | 截目前頁面截圖；錄製互動為 GIF |
| **互動** | 點擊元素；鍵盤輸入；填表單 |
| **瀏覽** | 開/關/切換分頁；前往 URL |
| **讀取** | 讀 DOM 狀態；讀 console 輸出 |
| **執行** | 在 console 跑 JavaScript |
| **狀態** | 使用你的已登入 session，不需要另設 cookie 或 token |

用 `/mcp` → 選 `claude-in-chrome` 可以看完整工具清單。

---

## 實際使用場景

### Debug 網頁錯誤

```
# 在瀏覽器打開出問題的頁面
claude --chrome
"幫我截一張網頁截圖，讀 console 的 error，然後在 src/app.js 裡找到問題修掉"
```

### 驗證 UI 是否和設計稿吻合

```
"把首頁截圖，跟 design.png 比較，列出差異"
```

### 自動化填表

```
"幫我把 report.csv 裡每一筆資料依序填進 https://app.example.com/form 並送出"
```

### 已登入服務的資料擷取

```
"在 Gmail 裡找這個月所有含 invoice 關鍵字的信，整理成 invoices.md"
```

（直接用你的 Gmail 登入狀態，不需要 OAuth 設定）

---

## 注意事項和限制

### 支援的瀏覽器

| 瀏覽器 | 支援 |
|---|---|
| Google Chrome | ✅ |
| Microsoft Edge | ✅ |
| Brave | ❌ |
| Arc | ❌ |
| WSL 環境 | ❌ |

### 需要手動介入的情況

- **Modal dialogs**（alert / confirm / prompt）：會阻塞頁面，Claude 會暫停並請你手動關閉
- **CAPTCHA 和登入頁**：Claude 會暫停，等你手動完成後再繼續
- **OAuth 流程**：Claude 不能代替你完成 OAuth 授權

### 帳號必須一致

Claude Code CLI 的登入帳號和 Chrome extension 的帳號必須相同，不一致會導致 extension 無聲無息地連線失敗。

---

## 常見問題排查

### "Browser extension is not connected"

最常見原因有三個：

**1. 同時安裝了 Claude Desktop 和 Claude Code**

兩者都有 native messaging host，Chrome extension 只能連接一個。Claude Desktop 的 host 優先級可能更高，導致 Claude Code 永遠連不上。

暫時解法：關閉 Claude Desktop，重啟 Chrome，再啟動 `claude --chrome`。

**2. Native messaging host 尚未安裝**

確認設定檔存在（見上方安裝路徑），或重跑一次 `claude --chrome` 讓它重新安裝。

**3. Chrome 沒有重啟**

第一次安裝後沒有完整重啟 Chrome。強制關閉所有 Chrome 視窗再重開。

### EADDRINUSE（Windows named pipe 衝突）

重啟 Claude Code，確認沒有其他 Claude Code session 在跑。

### Service worker 閒置超時

長時間沒互動後 extension 的 service worker 被系統暫停。在 session 裡跑 `/chrome` → "Reconnect extension" 可以重新喚醒。

---

## 和其他工具的比較

| | `claude --chrome` | MCP Puppeteer | MCP Chrome DevTools |
|---|---|---|---|
| 來源 | Anthropic 官方 | 社群 | 社群 |
| 安裝難度 | 裝 extension，一個指令 | 需要自行設定 MCP | 需要自行設定 MCP |
| 使用已登入 session | ✅ | ❌（headless browser）| 部分 |
| 需要另起 server | ❌ | ✅ | ✅ |
| Headless 模式 | ❌（必須有視窗）| ✅ | ✅ |

---

## 相關筆記

- [AI 101 - Claude Code 生態系](./claude-code-ecosystem.md) — Skills、Hooks、MCP、Plugins、Subagents 的關係
- [AI 101 - Claude Code 行為結構設計](./claude-code-behavior-design.md) — Hook 等行為結構設計入門

## Sources

- [Use Claude Code with Chrome (beta) — 官方文件](https://code.claude.com/docs/en/chrome)
- [Getting Started with Claude in Chrome — Anthropic Support](https://support.claude.com/en/articles/12012173-get-started-with-claude-in-chrome)
- [Using Claude in Chrome Safely — Anthropic Support](https://support.claude.com/en/articles/12902428-using-claude-in-chrome-safely)
- [Claude Code CLI Reference](https://code.claude.com/docs/en/cli-reference)
