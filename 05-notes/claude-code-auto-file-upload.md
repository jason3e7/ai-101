---
title: Claude Code 自動上傳檔案到網頁（Playwright MCP + CDP）
tags: [ai, 個人筆記, claude-code, playwright, mcp, file-upload, automation]
created: 2026-07-10
---

# Claude Code 自動上傳檔案到網頁（Playwright MCP + CDP）— Auto File Upload with Playwright MCP

[← 回主頁](../index.md)

> [!NOTE]
> 想讓 Claude Code 在 skill 裡自動把本機圖檔傳到第三方網頁的檔案輸入欄，`claude --chrome` 這條路走不通（見末尾說明）；改用 **Playwright MCP** 接一顆已登入的 Chrome，就能直接上傳。這篇是我實測過的**開箱即用步驟**，其餘背景與限制放後面。

> **TL;DR (EN):** Automating file uploads to third-party web forms via `claude --chrome` is broken (the `file_upload` tool isn't exposed through MCP). Workaround: Playwright MCP with `--cdp-endpoint`, attached to a real Chrome you launched inside an RDP desktop and logged in manually. Quick-start steps below.

---

## 開箱即用步驟 — Quick Start

前提：你有一個看得見的桌面環境（RDP、本機 X11、或其他），能手動開一顆 Chrome 並登入目標網站。

### Linux / X11 桌面

1. **在桌面手動開一顆帶 debugging port 的 Chrome**（保持這視窗開著）

    ```bash
    google-chrome \
      --remote-debugging-port=9222 \
      --user-data-dir="$HOME/chrome-cdp-profile"
    # 有些發行版是 chromium-browser / chromium
    ```

2. **在這顆 Chrome 手動登入目標網站** —— SSO / 2FA 全部走完，登入態會存進 `chrome-cdp-profile`。**登入這步不自動化**。

3. **確認 debugging port 活著**

    ```bash
    curl http://localhost:9222/json/version
    # 應該回傳 JSON，含 browser 版本與 webSocketDebuggerUrl
    ```

4. **把 Playwright MCP 接到這顆 Chrome**

    ```bash
    claude mcp remove playwright
    # 注意 -- 分隔符是給 MCP server 的 flag
    claude mcp add playwright npx @playwright/mcp@latest -- \
      --cdp-endpoint=http://localhost:9222 \
      --allow-unrestricted-file-access
    ```

    `--allow-unrestricted-file-access`：允許讀 workspace 根目錄以外的檔案（否則可能撞路徑限制）。

5. **重開 Claude Code，確認工具在** —— 應該看得到 `browser_navigate`、`browser_snapshot`、`browser_file_upload`。

6. **空跑驗證** —— 叫 Claude Code 導到目標網站首頁並 snapshot；若顯示**登入後**內容，代表 session 沾成功。

7. **完整上傳流程**

    1. `browser_navigate` → 上傳頁
    2. `browser_snapshot` → 找上傳按鈕的 `ref`
    3. `browser_click` → 點按鈕（觸發檔案選擇器）
    4. `browser_file_upload` → `paths: ["/abs/path/to/img.png"]`（**絕對路徑**）
    5. `browser_click` → 送出

### Windows RDP

只有第 1 步指令換成 Windows 版，其餘 3–7 相同：

```bat
REM 在 RDP 桌面 session 內執行
"C:\Program Files\Google\Chrome\Application\chrome.exe" ^
  --remote-debugging-port=9222 ^
  --user-data-dir="%USERPROFILE%\chrome-cdp-profile"
```

---

## 為什麼要走這條路 — Why This Setup

### 為什麼不用 `claude --chrome`

我原本想用 `claude --chrome`（Claude in Chrome 擴充功能），但卡在：

- 能吃**本機檔案路徑**的工具 `file_upload`，只存在 Chrome 側邊欄本體
- 透過 MCP 橋接給 Claude Code 時，只暴露 17 個工具，`file_upload` **被濾掉、沒暴露**
- 剩下的 `upload_image` 只吃截圖產生的 `imageId`、不吃路徑，還常報「Unable to access message history to retrieve image」

**結論**：這條路做不到，不是設定問題。相關 issue [anthropics/claude-code #31210](https://github.com/anthropics/claude-code/issues/31210)（以 duplicate 關閉合併，非明確「不修」）。

### 為什麼選「CDP 接真 Chrome」

Playwright MCP 有兩種模式：

- **A：持久 profile**：MCP 自己開瀏覽器並登入一次存進 profile。**預設 headed（要圖形環境）**。
- **B：CDP 接真 Chrome**（本篇採用）：接一顆自己手動開好、已登入的 Chrome。

我用 B 的原因：

- Claude Code 從**沒有圖形環境**的 shell 跑，沒有 `DISPLAY`，A 的 headed 瀏覽器起不來（headless 可繞但又是另一回事）
- B 的瀏覽器是**我自己在 RDP 桌面手動開的**，那個桌面有正常 `DISPLAY`，Chrome 開得起來；MCP 只透過 CDP（`http://localhost:9222`）接上去，**MCP 這端不需要 display**

### 上傳機制

Playwright 的 `browser_file_upload` 是**先點「選擇檔案」按鈕觸發原生檔案選擇器，再把路徑餵給選擇器**——路徑和真人操作一致，第三方前端驗證會正常觸發。定位元素靠 accessibility tree（頁面的無障礙結構），不是靠截圖猜座標。

---

## 限制與注意事項 — Caveats

方法 B 有兩個逃不掉的限制（不是壞掉）：

1. **手動開的那顆 Chrome 要一直活著** —— 被關掉、RDP session 斷掉，自動化就接不到 endpoint，得重開
2. **登入 session 會過期** —— 重新登入（尤其 2FA）仍需人工回那顆 Chrome 登一次。這是第三方自動化的天花板，B 也不例外

### 未來優化方向

若要放進「很常跑」的自動化 skill，微軟建議 coding agent 用 **Playwright CLI + SKILLS**（`microsoft/playwright-cli`）而非 MCP，較省 token。先用 MCP 版驗證能通，之後再評估搬過去。

---

## Sources

- [anthropics/claude-code #31210 — `file_upload` 未透過 MCP 暴露](https://github.com/anthropics/claude-code/issues/31210)
- [Playwright MCP 官方 repo — `--cdp-endpoint`、headed 預設、file access 限制](https://github.com/microsoft/playwright-mcp)
- [browser_file_upload 工具文件](https://playwright.dev/mcp/tools/file-upload)
- 相關：[Claude in Chrome 疑難排解、權限與已知問題（含 #73161 上傳工具全壞）](../06-external/20260704_Claude%20in%20Chrome%20%E7%96%91%E9%9B%A3%E6%8E%92%E8%A7%A3%E3%80%81%E6%AC%8A%E9%99%90%E8%88%87%E5%B7%B2%E7%9F%A5%E5%95%8F%E9%A1%8C%20%E2%80%94%20Anthropic%20%E5%AE%98%E6%96%B9%EF%BC%8BIssue.md)
