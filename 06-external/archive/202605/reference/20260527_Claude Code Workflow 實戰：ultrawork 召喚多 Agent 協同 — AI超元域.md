---
title: "Claude Code Workflow 實戰：ultrawork 召喚多 Agent 協同"
tags: [ai, 外部觀點, claude-code, workflow, ultrawork, multi-agent, harness-engineering, js-script, AI超元域]
source: https://www.youtube.com/watch?v=ozVTJm3n2U4
author: AI超元域
created: 2026-05-27
---

# Claude Code Workflow 實戰：ultrawork 召喚多 Agent 協同

> [!info]
> YouTube：[AI超元域 @AIsuperdomain](https://www.youtube.com/watch?v=ozVTJm3n2U4)
> **一句話：** 示範 Claude Code 的 Workflow 功能與 `/ultrawork` 指令——讓 Claude 自動生成可重複使用的 JS 腳本，召喚多個平行 Agent 協同執行，實現精準可控的工作流，這是 Harness Engineering 的具體落地。

---

## 核心主題

### Workflow 功能

Claude Code 的 Workflow 功能讓 AI 從「問答模式」進化到「流程執行模式」：

- **傳統方式**：每次給 Claude 一段大 Prompt，結果不穩定
- **Workflow 方式**：把流程拆成結構化步驟，每個步驟各司其職，可重複執行

### `/ultrawork` 指令

`/ultrawork` 是 Workflow 功能的核心指令，作用是強制 Claude Code 進入自主執行模式：

```
/ultrawork [任務描述]
```

執行後 Claude 會：
1. 自動分析任務，拆解成多個子任務
2. 生成對應的 JS 腳本（可存檔、重複使用）
3. 召喚多個平行 Agent，各自處理子任務
4. 協調 Agent 結果，整合成最終輸出

---

## 關鍵特色

### 自動生成可重複使用的 JS 腳本

這是 `/ultrawork` 與普通 Prompt 最大的差異：

```javascript
// Claude 自動生成的 workflow 腳本示例
export default {
  name: "market-research-workflow",
  steps: [
    { agent: "researcher", task: "fetch competitor data" },
    { agent: "analyst", task: "summarize findings" },
    { agent: "writer", task: "generate report" }
  ]
}
```

腳本可以：
- 存成 `.js` 檔案，下次直接重用
- 修改參數後重新執行
- 納入版本控制

### 多 Agent 平行召喚

`/ultrawork` 會根據任務結構，自動判斷哪些子任務可以平行執行：

```
任務: 分析競爭對手並生成報告
├── Agent A: 抓取競爭對手 A 的資料（平行）
├── Agent B: 抓取競爭對手 B 的資料（平行）
├── Agent C: 抓取競爭對手 C 的資料（平行）
└── Agent D: 整合 A/B/C 結果，生成報告（等待前三者完成）
```

---

## 與 Harness Engineering 的關係

> [!tip] 這是 Harness Engineering 的具體示範
> 影片的副標題明確點出「這才是 Harness Engineering」。

Harness Engineering 的核心主張：**框架設計決定 AI 系統 70% 的效能**。

`/ultrawork` 正是這個觀念的實作：

| 層次 | 傳統做法 | ultrawork 做法 |
|---|---|---|
| **Prompt 層** | 一個大 Prompt 包含所有邏輯 | 每個 Agent 只負責一件事 |
| **流程層** | 隱含在 Prompt 文字中 | 顯性化為 JS 腳本結構 |
| **執行層** | 單一 Claude 執行 | 多 Agent 平行執行 |
| **重複使用** | 每次重寫 Prompt | 腳本可存檔重用 |

---

## 與 /goal 的比較

| 功能 | `/goal` | `/ultrawork` |
|---|---|---|
| **執行模式** | 長期持續執行單一目標 | 複雜任務的並行分解 |
| **Agent 數量** | 通常單一 Agent | 多個平行 Agent |
| **腳本化** | 否 | 是（自動生成 JS）|
| **適用場景** | 需要連續執行的長任務 | 需要分工協作的複雜任務 |

---

## 實戰啟示

1. **Workflow > Prompt**：對於複雜任務，花時間設計 Workflow 比不斷調整 Prompt 更值得
2. **腳本即資產**：`/ultrawork` 生成的 JS 腳本可以重複使用，是可累積的工作資產
3. **平行化是關鍵**：多 Agent 平行執行大幅縮短複雜任務的完成時間
4. **Harness 優先**：在設計 AI 工作流時，框架設計的優先級高於模型選擇

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — Harness Engineering 完整觀念：框架決定 70% 效能
- [[蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt — 新人類聯盟]] — 企業落地角度的 Workflow 觀念
- [[AI Agent 連續跑 27 小時：Claude Code ／goal 功能解析 — Gary Chen]] — /goal 的深度解析，與 /ultrawork 互補
- [[Claude Code 完整指令與快速鍵速查表 — 數位時代]] — Slash Commands 完整速查
- [[Is Grep All You Need：向量搜尋不如 grep 的 PwC 研究 — Wisely Chen]] — Harness 框架影響效能的實驗佐證

---

## 來源

- YouTube：[AI超元域](https://www.youtube.com/watch?v=ozVTJm3n2U4)
