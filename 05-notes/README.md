# 個人筆記

這裡放我自己對 AI 的想法、觀察、和實驗記錄。

## 資料夾 — Folders

| 資料夾 | 放什麼 |
|:---|:---|
| [`01-refactorPlan/`](01-refactorPlan) | 結構重構計劃書（refactor-note skill 的 plan 產出，commit 前的設計稿） |
| [`02-project/`](02-project) | 專案集，每個子資料夾是一個獨立的實測或系列 |
| &nbsp;&nbsp;└ [`2026ironman/`](02-project/2026ironman) | iThome 鐵人賽 2026 的規劃、草稿與上稿版（Day 1-30 全部完賽） |
| &nbsp;&nbsp;└ [`zerojudge/`](02-project/zerojudge) | ZeroJudge 自動化刷題（Playwright + agentic loop） |
| &nbsp;&nbsp;└ [`leetcode/`](02-project/leetcode) | LeetCode Blind 75 / LC-75 / NC150 刷題紀錄 |
| &nbsp;&nbsp;└ [`phm/`](02-project/phm) | Please Hack Me CTF writeup（Top 10 完賽） |
| &nbsp;&nbsp;└ [`htb/`](02-project/htb) | HTB 靶機案例（Cap / Touch / Management + 目標綁架三案例） |
| [`experiments/`](experiments) | 單次實測與實驗記錄（LLM 極限三輪實測、Claude Code 檔案上傳、Codex 多 agent） |
| [`essays/`](essays) | 自己的論述與方法（AI 與知識壁壘、驗證再拓展、meta-prompting） |
| [`design-and-guides/`](design-and-guides) | 設計稿與寫作指南（設定拆分、心智清單 skill、pgplay 寫作風格） |
| [`assets/`](assets) | 跨資料夾共用的截圖與結果檔 |

## 命名規則

依 [CLAUDE.md](../CLAUDE.md) 規則：

- 一般內容資料夾 / 檔案：**kebab-case**（`essays/`、`core-concepts.md`）
- 數字前綴 meta 目錄：**camelCase 可**（`01-refactorPlan/`、`02-project/`）—— 結構 index 跟內容區隔
- 日期型目錄：不套 `refactor-note` 的 ≤ 10 上限（`06-external/archive/yyyymm/`、`2026ironman/drafts/dayNN-*.md`）
