---
title: "非工程師的 Claude Code Memory 整理術：1075 行瘦成 57 行"
tags: [ai, 外部觀點, claude-code, memory, pending, context, multi-project, 非工程師, 跨機器同步, Claude Taiwan 社群]
source: https://www.facebook.com/groups/1224997379198346/posts/1307072610990822/
author: Claude Taiwan 社群成員
created: 2026-05-27
---

# 非工程師的 Claude Code Memory 整理術：1075 行瘦成 57 行

> [!info]
> 原文：[Claude Taiwan 社群貼文](https://www.facebook.com/groups/1224997379198346/posts/1307072610990822/)
> **一句話：** 一位非工程師同時管六個專案、跑三台機器，把累積兩個月、1,075 行的 PENDING.md 重整成 57 行——靠的是分層架構、目錄即上下文、以及把規則寫進 CLAUDE.md。

---

## 問題背景

作者特徵：
- **不會寫程式**
- 同時管六個專案（A/B/C/D/E/F）
- 三台機器（桌機 + 筆電 + Mac）
- 每天切換不同主題

### 待辦清單的腫脹問題

`PENDING.md` 累積兩個月後：**1,075 行**

- 95% 是已完成的歷史紀錄
- 真正還在跑的只有約 50 條
- 這個檔每次對話開頭都會載入 → Claude 要在千行中找重點，速度變慢、判斷變鈍

更嚴重的是：**所有專案的待辦混在一起**，六個專案的進度全擠在同一個檔，每次處理專案 A 要先過濾掉 B/C/D/E/F 的雜訊。

---

## 解法：一個下午的整頓

### 第一步：三層架構

```
主 PENDING.md（每次對話必讀，只當摘要層）
├─ 專案 A：14 個待辦 / 2026-05-27 13:25
├─ 專案 B：10 個待辦 / 2026-05-22
├─ 專案 C：8 個待辦 / 2026-05-18
└─ 全域：3 個待辦

各專案專屬 PENDING.md（進那個專案目錄才讀）
→ 專案 A 的全部細節搬到 C:\Projects\project_A\PENDING.md
→ 每個專案各有獨立待辦檔
```

**主檔只看摘要**：哪個專案有幾個待辦、最後更新時間。要處理專案 A，就 `cd` 進對應目錄再開 Claude，它自動讀到那個專案的詳細 PENDING。

### 第二步：實際瘦身結果

| 檔案 | 整理前 | 整理後 | 降幅 |
|---|---|---|---|
| 主 PENDING.md | 1,075 行 | 57 行 | **95%** |
| 專案 A 專屬 PENDING | — | 50 行（立即/本週/下輪/技術債）| — |

**歷史紀錄全砍**。理由：完成的事 `git log` 看得到，不需要每次對話載入。

### 第三步：規則寫進 CLAUDE.md

```markdown
1. 每次對話開頭讀主 PENDING
2. 進專案目錄時，額外讀那個專案的 PENDING
3. 我每次更新任何專案 PENDING，
   自動同步更新主檔的摘要行（重算數字 + 蓋時間戳）
```

> [!tip] 第三條最關鍵
> 以前數字會失準（改了子檔但主檔沒更新）。現在改一處就自動同步另一處。

### 第四步：三台機器同步

踩到的坑：
- 筆電的 repo 放錯位置（家目錄 vs Documents）
- Mac 有一個過期的舊 clone 沒清掉
- 三台路徑慣例不統一（`C:\Projects` vs `C:\dev` vs `~/Projects`）

花了一小時把三台對齊到同一路徑，驗證所有機器的主 PENDING 都是 57–60 行。

---

## 三個帶走啟發

### 啟發 1：Memory 不是越多越好，是越精準越好

> 「每次對話強制載入的東西要精簡到極致，深度資訊放延遲載入（reference 目錄）或專案專屬檔。」

之前以為 Memory 寫越詳細 Claude 越懂，結果是反過來的——強制載入的 context 越龐雜，Claude 越難聚焦。

### 啟發 2：用「目錄 = 上下文」分流多專案

Claude Code 會根據 `cd` 進哪個目錄，自動讀該目錄下的 `CLAUDE.md` 與對應的 memory。

**多專案人的 SOP：開工前先進對的目錄，讓 Claude 自動套用對的上下文。** 不要從家目錄開所有東西。

### 啟發 3：跨機器同步要早點建好機制

作者踩過自動同步 hook 覆寫資料的災難，後來改成手動指令：

| 指令 | 動作 |
|---|---|
| `/claude-push` | 推上 GitHub |
| `/claude-pull` | 從 GitHub 拉下來 |

手動的好處：推之前一定 diff 過，知道改了什麼才推。

---

## 核心洞見

> [!quote]
> 「關鍵不是技術，是**把規則寫下來讓 Claude 自己遵守**。紀律寫得越清楚，AI 越好用。」

這是整篇最重要的一句話——也是 CLAUDE.md 存在的意義。

---

## 相關筆記

- [[AI 101 - Context Engineering]] — Context 精簡的理論基礎：主動載入 vs 延遲載入
- [[AI 101 - Harness Engineering]] — CLAUDE.md 就是 Harness 框架的一部分
- [[蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt — 新人類聯盟]] — 把規則顯性化的另一個角度

---

## 來源

- 原文：[Claude Taiwan 社群](https://www.facebook.com/groups/1224997379198346/posts/1307072610990822/)
