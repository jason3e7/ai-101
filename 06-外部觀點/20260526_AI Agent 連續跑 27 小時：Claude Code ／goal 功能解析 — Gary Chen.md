---
title: "AI Agent 連續跑 27 小時：Claude Code /goal 功能解析"
tags: [ai, 外部觀點, claude-code, goal, agent, autonomous, long-running, workflow, gary-chen]
source: https://www.youtube.com/watch?v=PpeCur6fEXc
author: Gary Chen（@garytalksstuff）
created: 2026-05-26
---

# AI Agent 連續跑 27 小時：Claude Code /goal 功能解析

> [!info]
> YouTube：[我的 AI agent 連續跑了 27 個小時，/goal 功能怎麼用？](https://www.youtube.com/watch?v=PpeCur6fEXc)
> 作者：Gary Chen（[@garytalksstuff](https://www.youtube.com/@garytalksstuff)）
> **一句話：** Claude Code 2.1 的 `/goal` 指令讓 Agent 持續自主執行直到目標達成——Gary Chen 實測讓它跑了 27 小時，這篇解析 `/goal` 怎麼運作、怎麼用。

---

## /goal 是什麼

Claude Code 2.1.139（2026-05-12）新增的指令。

**傳統模式**：你輸入一個 prompt，Claude 做一步，你再輸入，Claude 再做一步。
**Goal 模式**：你定義一個「完成條件」，Claude 自主跑、驗證、修正、再跑，直到條件達成才停下來通知你。

> [!tip] 核心差異
> 過去：互動式對話，人在迴圈裡
> `/goal`：Claude 擁有整個執行迴圈，人只在開頭和結尾出現

---

## 語法

```bash
# 基本用法
/goal "你的目標描述"

# 加上限制條件（避免無限跑）
claude /goal "..." --tokens 500K     # token 上限
claude /goal "..." --turns 20        # 最多 N 輪
claude /goal "..." --time 30m        # 時間上限
```

---

## 自主執行流程

```
你下 /goal "目標"
  └── Claude 寫程式 / 執行測試 / Debug / 重跑
        └── 每輪結束後自我評估：目標達成了嗎？
              ├── 否 → 繼續下一輪
              └── 是 → 獨立驗證 session（第二個 Claude 審核）
                          └── 確認完成 → 通知你
```

**獨立驗證**：完成後由一個獨立的 Claude session 審核最終狀態，確認目標確實達成（不是「看起來達成」），才回報完成。

---

## Agent View 儀表板

`/goal` 搭配 Agent View——在 CLI 統一管理多個同時跑的 Agent：

| 狀態 | 說明 |
|---|---|
| **Running** | Agent 正在自主執行中 |
| **Blocked** | 等待人工確認或環境輸入 |
| **Done** | 已完成，等你來 review |

```bash
/bg   # 把當前 agent 送到背景，你可以繼續做其他事
```

---

## 進度追蹤

| 指標 | 說明 |
|---|---|
| **Elapsed time** | 自主執行了多久 |
| **Turn count** | 跑了幾輪 |
| **Token usage** | 累積消耗的 token 數 |

Gary Chen 的實測：連續跑 27 小時，中間不需要人介入。

---

## 什麼任務適合 /goal

**適合：**
- 有明確、可測量的完成條件（「所有 TypeScript 錯誤消除 + 測試全過」）
- 範圍合理的重構任務（「把 auth.ts 改成 dependency injection 架構」）
- 需要反覆試錯的任務（寫、測試、修 bug、再測試的迴圈）
- CI/CD pipeline（`-p` 程式化模式）

**不適合：**
- 開放式創意任務（沒有客觀的成功標準）
- 無法自動驗證結果的任務

> [!warning] 成本注意
> 長時間自主執行會累積大量 token。
> 建議前幾次先加 `--turns` 或 `--tokens` 限制，觀察費用模式再調整。

---

## 可用模式

| 模式 | 說明 |
|---|---|
| Interactive | CLI 直接使用 |
| Programmatic（`-p`）| 腳本化工作流、CI/CD 整合 |
| Remote Control | 雲端協作式 Agent 控制 |
| Claude Code Mobile | 手機端也能啟動和監控 |

---

## 底層設計原則

> 「可靠性來自外層框架（harness），而不是模型本身。」

長時間 Agent 的成敗關鍵不在模型有多聰明，而在於：
- 目標條件定義得夠清楚、可驗證
- context 管理得當（避免長時間跑後 context 品質劣化）
- 有適當的 token / 時間 / 輪次預算限制

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Claude Code Skills、Hooks 整體架構
- [[AI 101 - Subagent 使用與計費]] — 長時間 Agent 的 token 計費與成本分析
- [[AI 101 - Harness Engineering]] — 「可靠性來自框架」的完整觀念
- [[/handoff Skill：把對話濃縮交接給下一個 Agent — Matt Pocock]] — 跑完一個長任務後交接給下一個 Agent

---

## 來源

- YouTube：[Gary Chen @garytalksstuff](https://www.youtube.com/watch?v=PpeCur6fEXc)
- ExplainX：[Claude Code 2.1: Agent View and /goal Command](https://explainx.ai/blog/anthropic-claude-code-agent-view-goal-command)
- ExplainX：[/goal Command — Long-Running Agents](https://explainx.ai/blog/claude-code-goal-command-long-running-agents-2026)
