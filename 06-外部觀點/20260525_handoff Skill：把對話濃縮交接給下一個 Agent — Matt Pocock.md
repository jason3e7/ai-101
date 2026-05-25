---
title: "/handoff Skill：把對話濃縮交接給下一個 Agent"
tags: [ai, 外部觀點, claude-code, skill, handoff, context, agent, workflow, matt-pocock]
source: https://www.youtube.com/watch?v=dtAJ2dOd3ko
author: Matt Pocock（@mattpocockuk）
created: 2026-05-25
---

# /handoff Skill：把對話濃縮交接給下一個 Agent

> [!info]
> YouTube：[/handoff is my new favourite skill](https://www.youtube.com/watch?v=dtAJ2dOd3ko)
> GitHub：[mattpocock/skills](https://github.com/mattpocock/skills)（104k ★）
> **一句話：** `/handoff` 把目前的 Claude Code 對話壓縮成一份交接文件，讓下一個 Agent 或下一個 session 能無縫接手，是解決「AI 沒有記憶、每次重開都得重新解釋」問題的務實方案。

---

## 問題：AI Agent 沒有記憶

> 「AI Agent 就像一個奇怪的實習工程師：沒有記憶，每個 session 都是全新複製出來的。」— Matt Pocock

長對話累積了大量 context（決策、架構取捨、已試過的方法、現在卡在哪），
但 context window 有上限，而且換 session 就全部消失。

---

## /handoff 做什麼

把**當前對話**壓縮成一份交接文件（handoff document），讓另一個 Agent 能從這份文件理解脈絡並繼續工作。

### 文件儲存位置

存到 **OS 暫存目錄**（`/tmp`），不存進工作區——避免污染 git，也不會被 commit 進去。

### 文件包含什麼

| 內容 | 說明 |
|---|---|
| 進行到哪裡 | 當前任務狀態與已完成的事 |
| 重要決策 | 已做過的取捨與原因 |
| 還沒解決的問題 | 卡住的地方、open questions |
| **Suggested Skills** | 建議下一個 Agent 啟用哪些 skill |
| 參照（不複製）| PRD、計畫文件、ADR、issues、commits、diffs——只記路徑或 URL，不重複貼內容 |

> [!tip] 精簡設計
> `/handoff` 刻意不把既有文件內容全部貼進去，而是「指標化」——
> 下一個 Agent 自己去讀那些文件，handoff document 只補充「文件之外的 context」。

### 隱私處理

自動遮蔽 API key、密碼、個人識別資訊（PII）——handoff document 可以安全分享或傳入新 session。

### 支援參數

```
/handoff 下一步重點放在 API 效能優化
```

傳入描述後，文件會針對下一個 session 的焦點調整內容，而不是平均交代所有事。

---

## 使用場景

| 場景 | 說明 |
|---|---|
| Context 快用完 | 開新 session 前先 `/handoff`，讓新 session 快速進入狀況 |
| 換電腦繼續 | 把 handoff doc 帶走，新環境直接繼續 |
| 轉交給隊友 | 讓別人的 Claude Code session 接手你的工作 |
| 任務切換 | 去做別件事，回來時直接把 handoff doc 餵進新 session |
| 多 Agent 編排 | Orchestrator 把任務狀態打包後傳給下一個 subagent |

---

## 安裝（Matt Pocock 的整個 Skills 套件）

```bash
npx skills@latest add mattpocock/skills
```

安裝後 `/handoff` 就會出現在你的 Claude Code skills 清單裡。

---

## Matt Pocock 的 Skills 套件概覽

`/handoff` 是其中一個，完整套件（104k ★）還包含：

| 分類 | Skill | 用途 |
|---|---|---|
| **工程** | `/grill-me` | 開始前密集提問，確認需求（最高 ROI）|
| **工程** | `/grill-with-docs` | 同上 + 維護 `CONTEXT.md` 專案詞彙表 |
| **工程** | `/to-prd` | 把討論轉成 PRD |
| **工程** | `/to-issues` | 把規格拆成獨立可交付的 issue |
| **工程** | `/tdd` | 強制 red-green-refactor 循環 |
| **工程** | `/diagnose` | 結構化 debug |
| **工程** | `/improve-codebase-architecture` | 架構改善 |
| **生產力** | `/handoff` | 本文主題，對話交接 |
| **生產力** | `/caveman` | 壓縮溝通，用簡短語言代替長句 |
| **生產力** | `/write-a-skill` | 讓 Claude Code 幫你寫新 skill |

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Skills 的架構與安裝說明
- [[AI 101 - Subagent 使用與計費]] — Multi-agent 場景下交接 context 的需求
- [[AI 101 - Context Engineering]] — Context 管理是 2026 年最關鍵的技能

---

## 來源

- YouTube：[/handoff is my new favourite skill](https://www.youtube.com/watch?v=dtAJ2dOd3ko)
- GitHub：[mattpocock/skills](https://github.com/mattpocock/skills)
- Skill 原始碼：[skills/productivity/handoff/SKILL.md](https://github.com/mattpocock/skills/blob/main/skills/productivity/handoff/SKILL.md)
- Matt Pocock on X：[/handoff might be my new favourite skill](https://x.com/mattpocockuk/status/2052489881088049407)
