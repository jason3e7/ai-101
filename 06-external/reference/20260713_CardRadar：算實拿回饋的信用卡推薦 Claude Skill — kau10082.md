---
title: "CardRadar：算實拿回饋的信用卡推薦 Claude Skill"
tags: [ai, 外部觀點, 參考, claude-skill, mcp, notion, 信用卡, 工具]
source: https://github.com/kau10082/Card_Radar
author: kau10082
created: 2026-07-13
---

# CardRadar：算實拿回饋的信用卡推薦 Claude Skill — A Real-Cashback Credit Card Picker

[← 回主頁](../../index.md)

> [!NOTE]
> 原文：[kau10082/Card_Radar — GitHub](https://github.com/kau10082/Card_Radar)
> **一句話：** 一個 Claude Skill——你講消費場景，它只看你手上有的卡、算扣掉回饋上限後的**實拿**金額，推薦最划算那張。內建台灣在地排除規則（如全聯體系）。

> **TL;DR (EN):** CardRadar is a Claude Skill: tell it a spending scenario, and it recommends the best card *you actually hold*, computing real cashback after per-card caps (not the headline rate). Includes Taiwan-local exclusion rules; backed by a Notion database + MCP. MIT-licensed.

---

## 這是什麼 — What It Is

多數信用卡比較工具給的是「帳面回饋率」——但真正到手的常常更少，因為有**回饋上限**、有**排除通路**。CardRadar 解決的就是這個落差：

- 只算**你手上真的有的卡**（不推你沒有的）
- 算的是**扣掉上限後的實拿金額**，不是虛高的百分比
- 只計入你**實際會用的支付方式**
- 內建**台灣在地排除規則**（例如全聯體系的排除）
- 會**記住你的消費習慣**（v7 新增），給個人化建議

---

## 怎麼用與技術 — Usage & Stack

**技術棧**：Claude Skill + Notion API（存回饋規則資料庫）+ MCP（Model Context Protocol）+ JSON 設定檔。

**安裝步驟**：

1. Clone 專案
2. 把 `cardradar/` 資料夾放進 Claude 的 skills 目錄
3. 複製並填好 `config.local.json`、`profile.local.json`
4. 在 Notion 建一個「信用卡回饋規則」資料庫
5. 在 Claude 裡輸入 `/card` 開頭的指令查詢

**授權**：MIT（可自由使用、修改、商用，須保留原授權標示）。

> [!TIP]
> 這是個很好的 **Claude Skill 實例**：把「查規則 + 算數字 + 記偏好」這種重複、程序化的事交給 Skill，資料放 Notion、用 MCP 串起來。想學 Skill 怎麼設計的話值得看它的結構。相關概念見 [Claude Code 生態系](../../01-fundamentals/claude-code/ecosystem.md)。

---

## 來源 — Sources

- [kau10082/Card_Radar — GitHub](https://github.com/kau10082/Card_Radar)
