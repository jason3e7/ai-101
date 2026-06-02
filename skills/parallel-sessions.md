---
name: parallel-sessions
description: 把今天的工作拆成可以平行跑的獨立 Claude Session，複製 Boris Cherny 的多 Session 工作模式
tags: [workflow, productivity, multi-session, parallelization]
source: 06-外部觀點/20260513_寫程式已被 AI 解完了 — Boris Cherny（Claude Code 之父）.md
---

# /parallel-sessions

把我今天的任務清單拆解成可以**平行執行的獨立 Session**。

## 步驟

1. **列出所有待辦任務**
   請我把今天想完成的事情全部列出來（或從 PENDING.md 讀取）。

2. **識別依賴關係**
   分析哪些任務之間有前後依賴，哪些完全獨立。

3. **分組成獨立 Session**
   把獨立任務分組，每組輸出一張 Session 卡片：

   ```
   Session A — [任務名稱]
   目標：一句話說清楚這個 session 要達成什麼
   範圍：只做這件事，不碰其他
   完成條件：怎樣算做完
   預計時間：X 分鐘
   ```

4. **標出可以用 `/loop` 自動化的任務**
   如果有重複性工作（CI 監控、PR 維護、資料收集），標出來並建議用 `/loop` 處理。

5. **輸出今日 Session 地圖**
   用簡單的文字圖顯示哪些 Session 可以同時開，哪些要等前一個完成。

## 原則

- 一個 Session 只做一件事，範圍越窄越好
- Session 之間不共享狀態，各自獨立
- 有明確完成條件才算一個合格的 Session
- 重複性任務優先考慮 `/loop` 自動化，不要佔用人的注意力
