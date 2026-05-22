---
title: "HTML is the New Markdown — Thariq Shihipar"
tags: [ai, 外部觀點, claude-code, html, markdown, workflow, thariq, 輸出格式, 原文]
source: https://www.lennysnewsletter.com/p/how-i-ai-html-is-the-new-markdown
author: Thariq Shihipar（Anthropic Claude Code 團隊）
created: 2026-05-22
---

# HTML is the New Markdown — Thariq Shihipar（Anthropic）

> [!info]
> 原文：[How I AI: HTML is the New Markdown](https://www.lennysnewsletter.com/p/how-i-ai-html-is-the-new-markdown)（Lenny's Newsletter，2026-05-18）
> 作者：Thariq Shihipar，Anthropic Claude Code 團隊工程師
> **一句話：** Thariq 的原文——為什麼他已停止寫 Markdown，改用 HTML 與 Claude 協作，以及三個具體工作流程：活 Design System、拋棄式 Micro-UI、互動專案計畫。

---

## 背景

這是 Lenny's Newsletter「How I AI」專欄的一篇，
源自 Anthropic 的 *Code with Claude* 活動。

這篇文章是 2026 年 5 月 **HTML > Markdown** 這波討論的源頭：
Andrej Karpathy 轉發、超過 15,000 讚、台灣各大媒體跟進報導。

---

## 核心論點

### 1. Markdown 的邊界

Markdown 的優勢是「人和機器都能讀」，但當規格文件長到幾千行，
你根本不會真的逐行讀——只是快速滑過。

HTML 讓計畫變成**可互動的視覺介面**：可捲動、可折疊、有 mockup、有色塊，
人才真的願意去讀、去批評、去修改。

> 「閱讀體驗好到值得花時間參與」才是關鍵，不是格式本身。

### 2. 工程師變成「算力分配者」（Compute Allocator）

> 「重要技能不再是寫程式，而是決定什麼值得做，以及在 AI 執行期間維持同步。」

當 Claude 可以自主工作幾小時，工程師的角色轉移了：
- 過去：寫程式
- 現在：定義邊界、審核決策、分配哪些任務值得讓 AI 去做

### 3. 拋棄式 Micro-UI（Disposable Micro-UIs）

針對計畫的某個區塊，臨時生成一個**專屬的編輯介面**，用完即丟。

> 「Micro software on top of micro software」——每個問題都有最適合它的工具，
> 用完之後丟掉，因為生成成本幾乎為零。

### 4. 活的 Design System（Living HTML Design System）

把整個設計系統（顏色、字型、間距、元件）壓縮進一個 HTML 檔案：
- 機器可以直接讀並套用
- 人也可以在瀏覽器預覽
- 可以跨專案攜帶，也可以從現有 codebase 萃取出來
- 不依賴 Figma 或 GitHub repository

### 5. Token 經濟的現實

> 「約 99% 的生成 token 不是 production code，
> 而是 dashboard、狀態更新、規劃工具。」

這個數字改變了成本計算：既然大多數 token 都在「協作過程」裡，
就應該把這個過程做得漂亮、好用，而不是省 token。

---

## 有效的 Prompt 範本

Thariq 分享他常用的起手式，刻意**給方向但留空間給 Claude 發揮**：

```
Create an HTML file with a plan.
Help me visualize.
Include excerpts, mockups, code, whatever is needed.
```

不強制規定結構，讓 Claude 自行判斷哪種視覺化最適合這個任務。

---

## 三個具體工作流程

| 工作流程 | 說明 |
|---|---|
| **互動專案計畫** | HTML 計畫檔包含視覺化、mockup、程式碼片段，整個規格一頁可讀 |
| **活的 Design System** | 壓縮版 HTML 設計系統，可機器讀取 + 人類瀏覽器預覽 |
| **拋棄式 Micro-App** | 針對特定計畫區塊生成的一次性編輯介面，用完即棄 |

---

## Markdown 還適合做什麼

> [!tip] 不是要消滅 Markdown
> - **版本控制**：git diff 友善，純文字好 review
> - **跨平台相容**：到處都能 render
> - **短篇筆記**：不需要視覺化的簡單內容

分工原則：**Markdown 給版控流程用，HTML 給人讀的輸出用。**

---

## 相關筆記

- [[Claude Code 邊寫邊記施工日誌：implementation-notes.html — Thariq（Anthropic）]] — 同一個作者的另一個 HTML 應用：邊實作邊寫施工日誌
- [[Anthropic 工程師為什麼摒棄 Markdown 改用 HTML — Gary Chen]] — Gary Chen 對這篇文章的影片解析

---

## 來源

- 原文：[How I AI: HTML is the New Markdown](https://www.lennysnewsletter.com/p/how-i-ai-html-is-the-new-markdown)（Lenny's Newsletter）
- T 客邦：[Markdown 已過時？Anthropic 工程師建議：改用 HTML](https://www.techbang.com/posts/129267-anthropic-html-ai-output-format)
- ABMedia：[Anthropic 工程師：HTML 才是 Claude Code 最佳輸出格式](https://abmedia.io/anthropic-engineer-html-claude-code-output-format-may-2026)
