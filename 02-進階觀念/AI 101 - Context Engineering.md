---
title: AI 101 - Context Engineering
tags: [ai, context-engineering, 2026, advanced]
created: 2026-04-22
updated: 2026-04-22
---

# Context Engineering

[[AI 101 - 主頁|← 回主頁]]

> [!quote]
> "Prompt 只是 AI 接收資訊的 5%。Context Engineering 設計的是那另外 95%。"
> — Anthropic Engineering Blog, 2026

---

## 什麼是 Context Engineering

**Context Engineering** 是設計 AI 在回答時「能看到什麼資訊」的整體架構。

不只是寫出好 prompt，而是系統性地管理：
- AI 什麼時候知道什麼
- 哪些資訊要常駐載入
- 哪些資訊按需載入
- 如何避免 context 被無用資訊佔滿

```
Prompt Engineering（舊思維）：
  寫出一個好的指令 → AI 回答

Context Engineering（新思維）：
  CLAUDE.md（常駐規則）
  + Memory（個人記憶）
  + Skills（按需載入的 SOP）
  + MCP（工具）
  + Subagents（隔離的子 context）
  + RAG（相關文件檢索）
  → AI 在正確的資訊環境中回答
```

---

## Context Window 的本質

```
┌─────────────────────────────────────────┐
│            Context Window               │
│                                         │
│  System Prompt / CLAUDE.md   ~10%       │
│  對話歷史                    ~30%       │
│  工具定義（MCP）             ~15%       │
│  檢索到的文件（RAG）         ~30%       │
│  你的 Prompt                  ~5%       │
│  AI 的回答空間               ~10%       │
└─────────────────────────────────────────┘
```

> [!warning] Context 污染
> 當 context 被大量無關資訊填滿，AI 的表現會顯著下降。
> 這就是為什麼 Subagents 和 Progressive Discovery 很重要。

---

## 四個 Context 層次

### 1. 常駐 Context（Always-on）
每次對話都自動載入，適合**幾乎每個任務都需要**的資訊。

- `CLAUDE.md` — 專案規範、架構說明
- System Prompt — AI 的角色定義
- Memory — 個人偏好與歷史

### 2. 按需 Context（On-demand）
使用者主動觸發，適合**特定工作流程**。

- Skills（`/指令`）— 特定任務的 SOP
- 相關程式碼檔案
- 特定文件

### 3. 工具 Context（Tool-based）
AI 自行判斷何時需要，適合**動態資訊**。

- MCP Server 提供的工具
- 搜尋結果
- API 回應

### 4. 隔離 Context（Isolated）
在獨立空間執行，不污染主 context。

- Subagents
- Fork

---

## Progressive Discovery

不一次把所有工具載入 context，**按需載入**。

**舊做法（不好）：**
```
載入 50 個 MCP 工具定義 → context 佔滿 → AI 效能下降
```

**新做法（推薦）：**
```
只載入 3 個最相關的工具 → 需要時再載入更多 → context 乾淨
```

---

## 實際應用：Claude Code 的分層設計

```
┌─────────────────────────────────────────┐
│  CLAUDE.md                              │  ← 常駐規則（每次都載入）
│  Rules: 程式碼規範、禁止事項             │
├─────────────────────────────────────────┤
│  Skills                                 │  ← 按需載入（/指令觸發）
│  Procedures: 特定工作流程的 SOP          │
├─────────────────────────────────────────┤
│  Hooks                                  │  ← 自動執行（不佔 context）
│  Checks: 驗證、格式化、通知              │
├─────────────────────────────────────────┤
│  Subagents / Fork                       │  ← 隔離執行
│  Context Isolation: 獨立子任務           │
└─────────────────────────────────────────┘
```

---

## Context Engineering 原則

> [!tip] 原則 1：最小化常駐 context
> CLAUDE.md 只放「幾乎每個任務都需要」的資訊。
> 不要把所有文件都塞進去。

> [!tip] 原則 2：按需載入
> 用 Skills 代替在 CLAUDE.md 裡寫大量流程說明。
> 流程說明只在需要時（`/指令`）才載入。

> [!tip] 原則 3：隔離雜訊
> 會產生大量中間結果的任務（搜尋、探索程式碼）
> 交給 Subagent，只把結論帶回主 context。

> [!tip] 原則 4：工具不是越多越好
> MCP 工具定義也佔 context。
> 只安裝真正需要的 MCP Server。

---

## 與 Prompt Engineering 的差異

| | Prompt Engineering | Context Engineering |
|---|---|---|
| **關注點** | 如何寫指令 | 如何設計資訊環境 |
| **範疇** | 單一 prompt | 整個 AI 系統架構 |
| **時代** | 2022-2024 | 2025 至今 |
| **技能要求** | 語言表達 | 系統設計思維 |
| **效果上限** | 有限 | 顯著更高 |

---

## Sources

- [Anthropic：Effective context engineering for AI agents](https://www.anthropic.com/engineering/effective-context-engineering-for-ai-agents)
- [Anthropic：Context engineering for long-running agents](https://www.anthropic.com/engineering/harness-design-long-running-apps)
- [LangChain Blog：The rise of Context Engineering](https://blog.langchain.com/the-rise-of-context-engineering/)
- [Shopify Eng Blog：Context Engineering for AI Agents](https://shopify.engineering/context-engineering-ai-agents)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — 工具的實際使用
- [[AI 101 - 實用技巧與最佳實踐]] — 具體技巧
