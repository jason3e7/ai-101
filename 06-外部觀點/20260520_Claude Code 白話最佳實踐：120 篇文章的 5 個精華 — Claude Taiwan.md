---
title: Claude Code 白話最佳實踐：120 篇文章的 5 個精華
tags: [ai, 外部觀點, claude-code, 最佳實踐, CLAUDE.md, subagent, hooks, prompt-caching, 白話]
source: https://www.facebook.com/groups/1224997379198346/posts/1300786674952749/
slides: https://docs.google.com/presentation/d/1mnjmwNZKsl1afDW2ia_QjWbJ9t45-FNC/edit
author: Claude Taiwan 社群（感謝 Maple Kuo 原始整理）
created: 2026-05-20
---

# Claude Code 白話最佳實踐：120 篇文章的 5 個精華

> [!info]
> 這份筆記整理自一篇整合了 **120+ 篇應用文章、30+ 篇論文**，
> 以及 Anthropic 工程師（Boris、Thariq）社群觀點的中文整理文。
> 作者把所有術語翻成生活比喻，方便沒寫過程式的人理解。
> 完整版為 50 張簡報（.pptx）。

---

## 1. CLAUDE.md ＝ 給 AI 看的「員工手冊」

員工手冊寫越厚，新人反而越不照做。CLAUDE.md 也一樣。

| 長度 | AI 遵從率 |
|---|---|
| 60 行（最佳）| **76%** |
| 超過 200 行 | **52%** |

> [!tip] 最甜長度：60 行
> 剛好夠講「我們這裡怎麼做事」，但不會多到 AI 記不住。
> 超過 200 行，遵從率直接砍掉三分之一。

---

## 2. Skills ＝ 抽屜裡的「專業手冊」

員工不用每天背所有專業知識，要用時拉開抽屜翻就好。

**關鍵在於標籤（description）要夠精準：**
AI 是靠 description 判斷「遇到什麼情境該翻哪本手冊」。
標籤模糊 → 再厚的手冊都用不上。

---

## 3. Subagents ＝ 派助理出去查

你是經理，不該自己跑去搜尋整個 codebase。
派助理出去查，他回來只回報結論。

**實用數字：**
- 一次最多可派 **4 個 subagent** 並行跑
- 最佳組合：**小助理 Haiku 跑檢索 + 顧問 Opus 做決策**
- 這樣的搭配可省下 **85% 成本**

---

## 4. Hooks ＝ 自動守門員

就像餐廳出菜口的品管員，AI 每個動作都先過他這關。

**能做什麼：**
- 擋下危險指令（如 `rm -rf /`）
- 存檔後自動跑 lint、format

> [!warning] 常見踩雷：exit 1 vs exit 2
> - `exit 2` ＝ **真的擋下**（動作不會執行）
> - `exit 1` ＝ **放行**（很多人以為擋下了，其實沒有）
>
> 用錯 exit code，以為安全機制在跑，實際上完全沒守到。

---

## 5. Prompt Caching ＝ 水電費分時段

同樣的對話內容，快取命中時費用大幅降低：

| 狀態 | 費用 |
|---|---|
| 未快取 | $3 / MTok |
| 快取命中 | **$0.30 / MTok（省 90%）**|

> 等於 10 杯咖啡只付 1 杯。

> [!warning] 繁體中文用戶特別注意
> **不要用 LLMLingua 之類的壓縮工具來省 token。**
> 繁中會直接壞掉，衰退 **25%+**。
>
> 要省 token，用 **Caveman Rules**（人工精簡寫法）才安全。

---

## 一句話 Takeaway

> [!quote]
> 員工手冊、抽屜、助理、品管員、水電費分時段——全部都是你日常早就在用的東西。
> 剩下的，只是花一個週末把對照關係建立起來而已。

對沒寫過程式的人：不用先看 120 篇英文文章，
看完一個生活比喻就能開始用 Claude Code。

---

## 資料來源

整合自：
- 120+ 篇 Claude Code 應用文章
- 30+ 篇相關論文
- Anthropic 工程師 Boris、Thariq 的社群觀點

完整 50 張簡報（每章含生活比喻 + 動手任務）：
[Google Slides](https://docs.google.com/presentation/d/1mnjmwNZKsl1afDW2ia_QjWbJ9t45-FNC/edit)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — 六大功能的技術細節
- [[AI 101 - Context Engineering]] — CLAUDE.md 的設計原則
- [[AI 101 - Subagent 使用與計費]] — Subagent 實作與 Haiku/Opus 組合省錢策略
- [[你只用到 Claude Code 六分之一的實力 — Claude Taiwan]] — 同社群的另一篇六大功能說明
