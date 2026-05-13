---
title: 寫程式已被 AI 解完了 — Boris Cherny（Claude Code 之父）
tags: [ai, 外部觀點, claude-code, vibe-coding, 未來工作, 跨域團隊, engineering]
author: Boris Cherny（Anthropic，Claude Code 創辦人）
created: 2026-05-13
---

# 寫程式已被 AI 解完了 — Boris Cherny

> [!quote]
> "對我來說，coding 已經解完了。" — Boris Cherny，2026

---

## 核心主張

**手動寫程式正在成為過去式。**
工程師的角色從「寫程式的人」轉變為「指揮 AI Agent 的人」。
跨域團隊（人人都能寫程式）將成為業界主流。

---

## Boris Cherny 是誰

Anthropic 內部 Claude Code 的創辦人（founder），也是現在最被廣泛使用的 AI 程式開發工具的設計者。

---

## 他現在怎麼工作

- **2026 年起，不再手動寫程式**，所有程式碼由模型生成
- 每天同時管理 **5–10 個 Claude session**
- 每天有數百到數千個 AI Agent 在幫他工作
- 某天單日推了 **150 個 PR**
- 主要工具之一：`/loop` 指令 — 把重複性任務（CI 監控、PR 維護、用戶回饋收集）自動化

---

## 跨域團隊是未來

Cherny 以 Claude Code 團隊為例：
- PM、設計師、資料科學家、財務人員**都在寫程式**
- 橫向專才（iOS + Web + Backend）→ **跨域通才**（產品 + 工程 + 資料科學）

> [!warning] 不學程式的風險
> 設計、PM、研究等非技術職位，若不學習用 AI 寫程式，可能成為「最後一代純職能人才」。

---

## Anthropic 內部現況

- 內部已無純手寫程式碼，**所有 SQL 皆由模型生成**
- 員工的 Claude 實例透過 Slack 互相溝通，形成人機混合 Agent 網路
- 真正的競爭優勢：**組織流程**，而不是模型本身

---

## 重要但書

> [!info] 「Coding 解完了」的邊界
> 這個說法主要適用於 **TypeScript + React** 這類主流技術棧。
> 以下情境仍未解決：
> - 老舊系統（legacy codebase）
> - 小眾程式語言
> - 高度客製化的平台

> [!warning] 安全問題尚未解決
> 現有系統仍需要「Harness（安全外殼）」處理：
> - Prompt injection 防護
> - 權限控制
> - 人工審查機制

---

## 延伸思考

- 如果「寫程式」不再是稀缺技能，**什麼才是？**（判斷力、系統思維、商業理解）
- 跨域團隊的崛起，會讓現有的垂直分工組織結構怎麼演化？
- 這對軟體工程教育意味著什麼？

---

## Sources

- [「寫程式已經被AI解完了！」Claude Code之父 — 數位時代 BusinessNext](https://www.bnext.com.tw/article/90865/when-engineers-stop-typing-ai-driven-coding-new-teams)
- [Anthropic's Boris Cherny: Why Coding Is Solved, and What Comes Next — YouTube](https://www.youtube.com/watch?v=SlGRN8jh2RI)
- [Facebook Reel — 相關影片](https://www.facebook.com/reel/1649823712971073)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code 的工具生態
- [[AI 101 - Harness Engineering]] — 他提到的「安全外殼」概念
- [[AI 101 - Context Engineering]] — Agent 指揮的核心技能
- [[AI 的隱形勞工與知識工作 Uberization — 矽谷輕鬆談]] — 另一個角度：這場轉型對勞工的衝擊
