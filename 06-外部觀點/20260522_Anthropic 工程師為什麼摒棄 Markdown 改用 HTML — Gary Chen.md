---
title: "Anthropic 工程師為什麼摒棄 Markdown 改用 HTML 跟 AI 工作？"
tags: [ai, 外部觀點, claude-code, html, markdown, workflow, thariq, 輸出格式, 影片]
source: https://www.youtube.com/watch?v=BhHMGRcbPkQ
author: Gary Chen（@garytalksstuff）
created: 2026-05-22
---

# Anthropic 工程師為什麼摒棄 Markdown 改用 HTML 跟 AI 工作？

> [!info]
> YouTube：[Anthropic 工程師為什麼摒棄 Markdown 改用 HTML 跟 AI 工作？](https://www.youtube.com/watch?v=BhHMGRcbPkQ)
> 作者：Gary Chen（[@garytalksstuff](https://www.youtube.com/@garytalksstuff)）
> **一句話：** 解析 Anthropic 工程師 Thariq 的論點：為什麼在 Claude Code 時代，HTML 已取代 Markdown 成為 AI 協作的最佳輸出格式。

---

## 核心主張

Anthropic Claude Code 工程師 **Thariq Shihipar** 在 2026 年 5 月發表文章 *"Using Claude Code: The Unreasonable Effectiveness of HTML"*，宣稱自己「幾乎已停止寫 Markdown」，改讓 Claude Code 直接產出 HTML。

這支影片是 Gary Chen 對這個主張的深度解析。

---

## 為什麼 HTML 比 Markdown 更適合 AI 輸出

### 1. 表達能力

> 「幾乎沒有任何 Claude 能理解的資訊，無法用 HTML 有效呈現。」— Thariq

| 內容類型 | Markdown | HTML |
|---|---|---|
| 表格 | 基本支援 | 可加顏色、排序、篩選 |
| 圖表 | 不支援 | SVG、互動圖表 |
| 嚴重程度標示 | 只能純文字 | 色碼（紅/橙/綠）|
| 頁內導覽 | 不支援 | 錨點、目錄 |
| 互動元件 | 不支援 | 按鈕、下拉選單、核取方塊 |

**實際範例**：分析 Linux 安全漏洞時，Markdown 輸出是縮排純文字，HTML 版本可以是深色背景 + 色碼嚴重程度 + 比較表格，可讀性與可用性截然不同。

### 2. Token 悖論

常見反對意見：HTML 標籤比 Markdown 多，浪費 token。

Thariq 的反駁：
- Markdown 時代是因為 GPT-4 只有 8K context，必須省 token
- Claude 進入 200K–1M context 時代，**token 成本不再是瓶頸**
- 一份清楚的 HTML 介面減少來回釐清次數，長期反而更省 token

### 3. 可分享性

Markdown 文件要透過 GitHub、Notion、或特定編輯器才能看，HTML 直接用瀏覽器開，可以放 S3 或靜態網站分享 URL——**大幅提升同事實際去讀的機率**。

### 4. 互動決策流程

最有說服力的使用場景：**審核 / 確認流程**。

與其讓使用者讀一長串 Markdown 規格文件再用文字描述修改意見，不如用 HTML 提供可點擊的選項介面，大幅降低認知負擔，也讓決策品質更好。

---

## Markdown 還沒死

> [!tip]
> 這不是要「消滅 Markdown」，而是重新分工：
> - **Markdown** 繼續適合版本控制、跨平台相容性（git diff 友善）
> - **HTML** 適合最終輸出、報告、需要人讀的文件、互動介面

---

## 社群反應

- Thariq 相關貼文在 X 累計超過 **15,000 讚**
- Andrej Karpathy 也公開表態支持 HTML-first 方向
- 台灣媒體廣泛報導：T 客邦、數位時代、INSIDE、ABMedia

---

## 相關筆記

- [[Claude Code 邊寫邊記施工日誌：implementation-notes.html — Thariq（Anthropic）]] — 同一個作者（Thariq）的具體 Prompt 技巧，就是用 HTML 寫施工日誌
- [[AI 101 - 實用技巧與最佳實踐]] — Claude Code 工作流程整理
- [[AI 101 - Context Engineering]] — Context 設計的完整觀念

---

## 來源

- YouTube：[Gary Chen @garytalksstuff](https://www.youtube.com/watch?v=BhHMGRcbPkQ)
- T 客邦：[Markdown 已過時？Anthropic 工程師建議：改用 HTML](https://www.techbang.com/posts/129267-anthropic-html-ai-output-format)
- 數位時代：[Markdown 語法跟 HTML 有什麼不同？](https://www.bnext.com.tw/article/90893/when-humans-stop-editing-ai-outputs-why-anthropic-engineers-are-switching-from-markdown-to-html)
- INSIDE：[HTML 才是 AI 時代的原生語言？](https://www.inside.com.tw/article/41251-from-md-to-html)
- ABMedia：[Anthropic 工程師：HTML 才是 Claude Code 最佳輸出格式](https://abmedia.io/anthropic-engineer-html-claude-code-output-format-may-2026)
