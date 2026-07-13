---
title: "Claude Code 邊寫邊記施工日誌：implementation-notes.html"
tags: [ai, 外部觀點, claude-code, prompt, workflow, html, 施工日誌, thariq]
source: https://www.threads.com/@cooljerrett/post/DYf62y-jzyf/
author: cooljerrett（轉述 Thariq Shihipar, Anthropic）
created: 2026-05-20
---

# Claude Code 邊寫邊記施工日誌：implementation-notes.html

> [!info]
> 原帖：[cooljerrett on Threads](https://www.threads.com/@cooljerrett/post/DYf62y-jzyf/)
> **一句話：** 讓 Claude Code 邊實作邊寫 HTML 施工日誌，你不用盯著它做，回來翻日誌就知道所有決策。

---

## 是什麼

Anthropic 工程師 **Thariq Shihipar** 分享的 Prompt 技巧：
在叫 Claude Code 實作功能的同時，要它維護一份 `implementation-notes.html`，
把所有「規格以外的決策」都記錄下來。

---

## Prompt 範本

```
Implement <SPEC>.

As you work maintain a running implementation-notes.html file that captures
anything I should know about how the implementation diverges from or
interprets the spec, including:
- Design decisions
- Deviations
- Tradeoffs
- Open questions
```

把 `<SPEC>` 換成你的任務描述即可。

---

## 為什麼用 HTML 而不是 Markdown

Thariq 本人也提倡在 Claude Code 工作流程裡優先用 HTML 而不是 Markdown：

| | HTML | Markdown |
|---|---|---|
| **結構** | 可巢狀、語意化標籤 | 純文字，限制多 |
| **渲染** | 瀏覽器直接開，格式豐富 | 需要解析器 |
| **Claude 熟悉度** | 訓練資料大量 HTML | 也很熟，但 HTML 更精確 |
| **施工日誌用途** | 表格、章節、折疊，一目了然 | 夠用但較平 |

> [!tip]
> 直接在終端機 `open implementation-notes.html` 或用瀏覽器開，
> 就能看到排版整齊的施工日誌，不需要 Obsidian 或任何額外工具。

---

## 解決了什麼問題

你叫 Claude Code 做一件複雜的事，通常有兩個選擇：

1. **盯著它做** — 看輸出、確認每一步，累且耗時
2. **放著不管** — 回來發現它做了一堆決策，你完全不知道為什麼這樣設計

**implementation-notes.html 是第三條路：**
你去做別的事，Claude 自己邊做邊記，你回來直接讀日誌，
不需要反推「它為什麼這樣做」。

---

## 適合的場景

- 規格不夠完整，需要 Claude 自行詮釋的任務
- 重構、架構調整等「決策密集」的工作
- 你不在場但想事後 review 的長時間任務
- 多人協作：讓 AI 的決策過程對團隊透明

---

## 使用範例

```
Implement a rate limiter middleware for our Express API.
Rate limit by IP: 100 req/min for anonymous, 1000 req/min for authenticated.

As you work maintain a running implementation-notes.html file that captures
anything I should know about how the implementation diverges from or
interprets the spec, including:
- Design decisions
- Deviations
- Tradeoffs
- Open questions
```

Claude 完成後，`implementation-notes.html` 可能包含：
- 「選用 sliding window 而非 fixed window，因為 fixed window 有邊界爆量問題」
- 「Redis key 格式設計為 `ratelimit:{ip}:{minute}`」
- 「Open question：authenticated user 是用 JWT sub 還是 API key 作為識別？」

---

## 相關筆記

- [[AI 101 - 實用技巧與最佳實踐]] — Claude Code 工作流程整理
- [[AI 101 - Context Engineering]] — Context 設計的完整觀念

---

## 來源

- Threads 原帖：[@cooljerrett](https://www.threads.com/@cooljerrett/post/DYf62y-jzyf/)
- Thariq Shihipar：Anthropic Claude Code 工程師
