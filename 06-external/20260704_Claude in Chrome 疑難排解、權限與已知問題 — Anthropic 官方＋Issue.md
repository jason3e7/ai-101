---
title: "Claude in Chrome 疑難排解、權限與已知問題"
tags: [ai, 外部觀點, claude-code, chrome, browser-automation, troubleshooting, permissions, bug]
source: https://support.claude.com/en/articles/12902405-claude-in-chrome-troubleshooting
author: Anthropic 官方＋社群 Issue
created: 2026-07-04
---

# Claude in Chrome 疑難排解、權限與已知問題 — Troubleshooting, Permissions & Known Issues

[← 回主頁](../index.md)

> [!NOTE]
> 原文：[Claude in Chrome Troubleshooting](https://support.claude.com/en/articles/12902405-claude-in-chrome-troubleshooting)、[Permissions Guide](https://support.claude.com/en/articles/12902446-claude-in-chrome-permissions-guide)、[Issue #73161](https://github.com/anthropics/claude-code/issues/73161)
> **一句話：** 整理 Claude in Chrome 官方的連線排查步驟、權限模型（兩種模式 + site-level + 禁止行為），以及一個目前無解的重大 bug（`file_upload` / `upload_image` 因 client／extension 協議不同步而全壞）。

> **TL;DR (EN):** A field guide to Claude in Chrome operations from official docs plus a community bug report — connection troubleshooting, the permission model (two modes, site-level grants, hard-prohibited actions), blocked site categories, quota impact, and one currently-unfixable bug: file/image upload tools are 100% broken due to a client/extension protocol mismatch (Issue #73161, still open).

搭配安裝與基本使用請看 [Claude Code chrome 瀏覽器整合](../01-fundamentals/claude-code/chrome-integration.md)；這篇補的是「裝好之後會撞到的問題」。

---

## 一、疑難排解 — Troubleshooting

### 連線問題與對應解法

| 症狀 | 解法 |
|---|---|
| **看不到網頁內容** | 重新整理 + 確認 extension 已啟用 + 確認已授予該站權限；JS 較重的站需多等載入 |
| **extension 裝不了／登不進** | 確認付費方案（Pro/Max/Team/Enterprise）；Team/Enterprise 要管理員「為組織啟用 extension」；清 claude.ai 快取 Cookie；登出再登入 |
| **連不上 Desktop / Claude Code** | 重啟／更新 extension；檢查 Desktop Connector 設定裡 Claude in Chrome 開關；重啟／更新 Claude Code |
| **功能操作異常** | 用最新版 Chrome；停用會干擾網頁互動的其他 extension；重新整理頁面重啟任務 |

### 被預設封鎖的網站類別

某些類別**預設就擋**，不是你設定問題：**金融、銀行、加密貨幣、成人內容**等。Team/Enterprise 的管理員還可能額外限制。

### 效能與配額

> [!WARNING]
> 官方明講：「Browser interactions are more compute-intensive than regular chats」。瀏覽器操作**吃 Max 方案額度**，長時間跑 workflow 會更快燒配額。

---

## 二、權限模型 — Permission Model

### 兩種模式

| 模式 | 行為 |
|---|---|
| **Ask before acting** | Claude 執行前先提計畫給你批准（安全） |
| **Act without asking** | 高風險：Claude 自主運作，只在敏感操作（金融交易、永久刪除等）前才問 |

### Site-level 權限（單站授權）

在特定網站上可選：

- **Allow this action** — 只授權這一次（最安全）
- **Always allow actions on this site** — 持續授權該站（需信任）
- **Decline** — 拒絕

到 **Settings → Permissions** 可管理已批准站點清單、隨時撤銷。

### 無論怎麼設都禁止的行為

> [!IMPORTANT]
> 這些**硬性禁止**，不受權限模式影響：處理信用卡／身份證資料、永久刪除檔案或清空垃圾桶、修改系統檔案或安全權限、執行投資交易。

這套「模式 + site-level + 硬禁止」的分層，和 Claude Code 本身的 [權限模式與設定層級](../01-fundamentals/claude-code/permissions.md) 是同一種設計思路：預設保守、單點授權、最高層有不可覆蓋的紅線。

---

## 三、已知重大問題：檔案／圖片上傳全壞 — Known Bug (#73161)

> [!WARNING]
> **`file_upload` 和 `upload_image` 目前 100% 故障，且無使用者側解法。**（截至 2026-07，Issue 仍未關閉）

**根本原因**：client／extension 版本協議不同步——

- Chrome extension 已改用**基於內容的上傳協議**（期望透過 `files` 參數收檔案 bytes）
- 但 CLI 的 MCP 控制器仍在送**主機檔案系統路徑**

**症狀**：

| 工具 | 錯誤 |
|---|---|
| `file_upload` | `file_upload no longer accepts host filesystem paths...` 但工具 schema 仍只有 `paths`、沒有 `files`，控制器無法照做 |
| `upload_image` | `Unable to access message history to retrieve image`——即使用同一 session 剛截的 `imageId` 也失敗 |

**測試環境**：CLI v2.1.197（最新）、macOS、extension 已更新（2026-06-30）仍無效。標籤 `bug` + `has repro`，**尚無指派人員或修復**。

> [!TIP]
> 實務上：**目前別依賴 `file_upload` / `upload_image`**。要讓網頁拿到檔案，改用其他路徑（例如先把檔案放到可存取的 URL，或用 `computer` 手動操作上傳對話框），直到這個 issue 修復。

---

## 我的重點提煉 — Takeaways

- **「連不上」先查三件事**：extension 啟用、站點權限、帳號方案／組織設定——官方排查九成集中在這三點。
- **權限設計哲學一致**：Chrome 版和 CLI 版都是「保守預設 + 單點授權 + 不可覆蓋的紅線」，理解一邊就懂另一邊。
- **上傳工具現在是地雷**：`file_upload` / `upload_image` 全壞且無解，規劃瀏覽器自動化時要繞開它。
- **瀏覽器操作很貴**：吃 Max 配額，長 workflow 要留意用量。

---

## 相關筆記 — Related

- [Claude Code chrome 瀏覽器整合](../01-fundamentals/claude-code/chrome-integration.md) — 安裝、架構、基本使用與完整 MCP 工具清單
- [Claude Code 權限模式與設定層級](../01-fundamentals/claude-code/permissions.md) — CLI 端的權限模型，與本篇 Chrome 權限同源

## 來源 — Sources

- 官方：[Claude in Chrome Troubleshooting — Claude Help Center](https://support.claude.com/en/articles/12902405-claude-in-chrome-troubleshooting)
- 官方：[Claude in Chrome Permissions Guide — Claude Help Center](https://support.claude.com/en/articles/12902446-claude-in-chrome-permissions-guide)
- Issue：[file_upload and upload_image are 100% broken — protocol mismatch · #73161 — anthropics/claude-code](https://github.com/anthropics/claude-code/issues/73161)
