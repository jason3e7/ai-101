---
title: "蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt"
tags: [ai, 外部觀點, claude-code, workflow, agent, 企業落地, prompt, 新人類聯盟]
source: https://www.facebook.com/groups/1224997379198346/posts/1305637254467691/
author: 新人類聯盟（Claude Taiwan 社群）
created: 2026-05-27
---

# 蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt

> [!info]
> 原文：[Claude Taiwan 社群貼文](https://www.facebook.com/groups/1224997379198346/posts/1305637254467691/)
> 系列：蜂群 Agent 系列第四篇
> **一句話：** 企業 AI 落地最大的痛點是「2,000 字大 Prompt 今天好明天壞」，解法是改用 Workflow 工作流編排——把流程拆成結構化的階段，每個階段各司其職。

---

## 核心問題

> 「我們都知道 AI 很有用，但要怎麼把它寫成一個穩定、不會隨機出錯、又可以隨時升級的系統？」

企業環境（HR 審批、財務稽核等）對「合規與精準」的要求是硬性的，不能接受隨機表現。

### 大 Prompt 的失敗模式

過去的常見做法：把所有邏輯塞進一段 2,000 字的 Prompt 處理所有情況。
結果往往是：

- 今天表現好，明天表現差
- 改了一個字，別的地方崩潰
- 無法追蹤「是哪個部分出錯」
- 難以升級或維護

---

## 解法：Workflow 工作流編排

Anthropic 在 Claude Code 中釋出的功能，核心概念是**把流程顯性化**：

| 大 Prompt 方式 | Workflow 方式 |
|---|---|
| 一個 Prompt 解決所有事 | 拆成多個明確的步驟 |
| 黑盒，出錯難以追蹤 | 每個節點可獨立觀察與測試 |
| 牽一髮動全身 | 每個節點可獨立升級 |
| 適合簡單任務 | 適合企業級複雜流程 |

Workflow 不是另一個「視覺化工具」——而是讓 AI 系統從「問答模式」進化到「流程執行模式」。

---

## 為什麼這對企業落地重要

企業場景的三個關鍵需求：

1. **穩定性**：相同輸入要有一致的輸出，不能「看心情」
2. **可稽核性**：HR 審批、財務稽核需要知道每一步是怎麼決策的
3. **可升級性**：業務規則改變時，只需更新對應的節點，不用重寫整個系統

Workflow 編排讓這三件事都變得可能。

---

## 蜂群 Agent 系列脈絡

這是「蜂群 Agent」系列的第四篇，系列聚焦在如何讓多個 AI Agent 協作完成複雜任務。

| 概念層次 | 說明 |
|---|---|
| 單一 Agent | 一個 Claude 處理一件事 |
| 蜂群 Agent | 多個 Agent 分工協作 |
| **Workflow 編排**（本篇）| 把多個 Agent 的協作流程顯性化、結構化 |

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — Workflow 編排的更深層觀念：70% 效能來自外層框架
- [[AI 101 - Subagent 使用與計費]] — 多 Agent 協作的架構與計費
- [[AI Agent 連續跑 27 小時：Claude Code /goal 功能解析 — Gary Chen]] — /goal 是 Workflow 的另一種實現方式
- [[用 Claude Code 週末建出台股回測系統：Playbook 方法論 — 羅達]] — 實戰中把工作拆成有驗收標準的步驟，是同一個思路

---

## 來源

- 原文：[Claude Taiwan 社群](https://www.facebook.com/groups/1224997379198346/posts/1305637254467691/)
