---
title: "Camofox：AI Agent 專用的隱形瀏覽器"
tags: [ai, 外部觀點, browser, headless, playwright, ai-agent, scraping, anti-detection, 陳盟升]
source: https://www.facebook.com/chenshaoyun0603/posts/pfbid02XBeRkUg6iAUxywnjd3XgHg11RSTsy2YaypLgt5pwYckHFQ3d2hyfDcKhE7eQ1zmXl
author: 陳盟升
created: 2026-05-27
---

# Camofox：AI Agent 專用的隱形瀏覽器

> [!info]
> Facebook 原文：[陳盟升](https://www.facebook.com/chenshaoyun0603/posts/pfbid02XBeRkUg6iAUxywnjd3XgHg11RSTsy2YaypLgt5pwYckHFQ3d2hyfDcKhE7eQ1zmXl)
> GitHub：[jo-inc/camofox-browser](https://github.com/jo-inc/camofox-browser)（5.9k ★）
> **一句話：** 在 C++ 引擎層偽裝硬體特徵的 Firefox fork，讓 AI Agent 瀏覽真實網頁時不被 Cloudflare 擋掉，並把 HTML 壓縮 90% 轉成 AI 友好格式。

---

## 問題背景

開發網路爬蟲或 AI Agent 時，最常遇到的障礙是反爬蟲系統（Cloudflare、PerimeterX 等）。

傳統工具的問題：
- **Playwright**：JS 層的特徵偽裝，容易被 fingerprinting 偵測
- **Headless Chrome**：特徵過於明顯，被識別率高

> 「AI Agent 需要瀏覽真實的網頁。Playwright 被擋掉，Headless Chrome 被 fingerprinting。」

---

## Camofox 的解法

**Camoufox**（Firefox fork）的 REST API 包裝器——反偵測能力建在 **C++ 層**，而非 JavaScript shim。

### 為什麼 C++ 層更難被偵測

JavaScript shim 是在 JS 執行後再覆蓋屬性，進階的反爬蟲系統可以偵測這個覆蓋行為。
C++ 層偽裝是在 JS 執行之前就完成，無法從 JS 端看出差異。

### 偽裝的內容

| 特徵 | 說明 |
|---|---|
| Navigator properties | 瀏覽器識別資訊 |
| WebGL renderer | GPU 型號 |
| AudioContext | 音訊硬體指紋 |
| Screen geometry | 螢幕解析度與尺寸 |

---

## AI Agent 友好設計

### Accessibility Snapshot（關鍵特色）

把原始 HTML 轉成壓縮格式，**縮小約 90%**，並自動標記每個可互動元素：

```
原始 HTML：<button class="btn-primary" id="submit-order" ...>送出訂單</button>
Snapshot：e1: 送出訂單 [button]
```

AI 只需要說「點 e1」，不需要 selector 或 XPath，減少 token 消耗、提升操作精準度。

### 內建搜尋捷徑

```
@google_search   → 直接搜尋 Google
@reddit_subreddit → 進入指定 subreddit
@youtube_search  → 搜尋 YouTube
```

---

## 技術規格

| 項目 | 數值 |
|---|---|
| 閒置記憶體 | ~40MB（懶惰啟動，按需開啟瀏覽器）|
| Session 隔離 | 每個使用者獨立 cookie / storage，自動持久化 |
| 部署方式 | Node.js（port 9377）、Docker、Railway、Fly.io |
| 認證 | Bearer token（`CAMOFOX_ACCESS_KEY`），機密走環境變數 |

---

## 適合的使用場景

- AI Agent 瀏覽需要登入或有反爬機制的網站
- 電商價格監控
- 自動化測試（需要真實瀏覽器行為）
- 需要大量網路爬取但不想被封鎖的研究用途

> [!warning] 合法使用
> 工具本身是中性的，但繞過反爬蟲機制可能違反目標網站的 ToS，使用前請確認合法性。

---

## 相關筆記

- [[Nuclei 結合 LLM Agents 完整指南 — BASHCAT]] — AI Agent 搭配資安工具的另一個面向
- [[AI 101 - Subagent 使用與計費]] — AI Agent 自主瀏覽網路的架構設計

---

## 來源

- Facebook：[陳盟升](https://www.facebook.com/chenshaoyun0603/posts/pfbid02XBeRkUg6iAUxywnjd3XgHg11RSTsy2YaypLgt5pwYckHFQ3d2hyfDcKhE7eQ1zmXl)
- GitHub：[jo-inc/camofox-browser](https://github.com/jo-inc/camofox-browser)
