---
title: AI 101 - 鐵人賽修正紀錄（fixButNotPublish）
tags: [ai, 鐵人賽, ironman, 修正, 出版, 流程]
created: 2026-09-16
---

# 修正紀錄 — Fix Log (fixButNotPublish)

[← 回主頁](../../index.md)｜[參賽規劃](./plan.md)｜[口語化紀錄](./plain-language-log.md)

> [!IMPORTANT]
> jason3e7 的訊息若以 **`[fixButNotPublish]`** 開頭，代表：**這是修正，但不要更新到 iThome 上。** 只改 repo，留作日後修訂與正式出版使用。賽期中的 iThome 文章維持原樣。

> **TL;DR (EN):** Messages prefixed `[fixButNotPublish]` carry corrections that must land in the repo only — never pushed back to the live iThome article. `drafts/` is the canonical, always-best version and the basis for any future book; `publish/` is a frozen snapshot of what is actually live. This file records every divergence so nothing gets lost.

---

## 三份檔案的分工 — Who Owns What

| 檔案 | 角色 | 什麼時候改 |
|---|---|---|
| `drafts/dayNN-*.md` | **正典。** 結構完整（導言、TL;DR、心智圖、系列對照表），文字永遠採用目前最好的版本。**未來出版以這份為底稿。** | 隨時。`[fixButNotPublish]` 一律改這裡 |
| `publish/dayNN-ithome.md` | **線上快照。** 精簡結構，必須跟 iThome 上看到的一字不差 | **只有真的去改了 iThome 才動** |
| `fix-log.md`（本檔） | 記錄兩者的落差與原因 | 每次 `[fixButNotPublish]` 都補一列 |

> [!WARNING]
> **不要把 `[fixButNotPublish]` 的內容同步到 `publish/`。** 那份檔的唯一價值是「忠實反映線上」——一旦摻進沒發布的修正，之後就再也無法判斷線上到底長什麼樣。

---

## 為什麼不即時更新 iThome — Why Not Push Live

- 賽制規定**每日發文當天才能修改**，評審看的是當日快照；事後大改意義不大
- 賽後**刪文會取消完賽資格**，文章要原樣留著
- 但內容的品質不該被凍結在賽期——**出版版本應該是修訂過的版本**

所以：**線上保持原樣，repo 持續長大。** 兩條線分開走。

---

## 修正清單 — The Log

| 日期 | Day | 修正內容 | 原因 | 線上狀態 |
|---|---|---|---|---|
| 2026-09-16 | 01 | 「『沒學過』不等於『沒背過那句話』」→ 改成「沒看過一樣的題目，它也常常答得出來，就像學生沒寫過這題，也能用學過的觀念解」 | 雙重否定繞圈，改成正面敘述＋生活比喻 | **已同步**（賽期內完成的口語化） |
| 2026-09-16 | 01 | 結論句「沒見過的組合，它拼得出來」→「它把學過的東西重新組起來，所以能解沒見過的題」 | 原句與底下兩個實驗實際證明的東西對不上 | **已同步** |

> [!NOTE]
> 上面兩列標「已同步」，是因為它們發生在賽期第一天、當天就改上去了。從這裡之後的 `[fixButNotPublish]` 項目，線上狀態一律是**未同步**。

---

## 相關 — Related

- [口語化修訂紀錄](./plain-language-log.md) —— 改寫的判準與檢查清單
- [上稿版本與轉換規則](./publish/README.md) —— repo 版轉 iThome 版要改什麼
