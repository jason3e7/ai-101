---
title: "AI 101 - Context Engineering 進階: 9 塊 4 操作 3 原理"
tags: [ai, context-engineering, 2026, 進階, 研究筆記]
created: 2026-09-21
---

# Context Engineering 進階 — Curating What the Model Sees

[← 回主頁](../index.md)

> [!NOTE]
> Prompt Engineering 早就不夠了. 2025 年 6 月, Tobi Lütke (Shopify CEO) 和 Andrej Karpathy 幾乎同時把「Context Engineering」推到公眾視野, 到 2026 年 Anthropic 官方 engineering blog 發〈Effective context engineering for AI agents〉, 這門學科正式站穩. 這篇跟 [Context Engineering (Claude Code 使用者角度)](./context-engineering.md) 那篇不同: 換一個角度, 拆**這門學科的 9 個組成、4 種操作、3 個核心原理**. 主要參考 ihower 的中文入門 + Anthropic engineering blog + LangChain 的實作觀察.

> **TL;DR (EN):** Context Engineering is the discipline of dynamically assembling the smallest possible set of high-signal tokens the model needs for the next step. Coined publicly by Tobi Lütke and Karpathy in mid-2025, mainstream by 2026, it replaces "Prompt Engineering" as the load-bearing skill in serious LLM apps. Break it down into nine components (system prompt, user input, short/long-term memory, RAG, tools, tool responses, structured output, workflow state), four operations (write, select, compress, isolate — Lance Martin), and three architectural constraints (finite attention budget, n² context rot, distraction). Practical takeaway: don't preload; retrieve just-in-time; compact aggressively; delegate messy exploration to sub-agents; never optimize a prompt when the real problem is that the model is looking at the wrong thing.

**兩個推波助瀾的引言**, 值得抄一次:

> Tobi Lütke (2025-06): 「the art of providing all the context for the task to be plausibly solvable by the LLM.」
>
> Andrej Karpathy (2025-06): 「+1 for 'context engineering' over 'prompt engineering'. ... in every industrial-strength LLM app, context engineering is the delicate art and science of filling the context window with just the right information for the next step.」

Harrison Chase (LangChain) 一句話說完差別: **「prompt engineering is a subset of context engineering.」** Prompt 只是模型看到的其中一部分, Context Engineering 管的是全部.

---

## 拆開來看有 9 塊 — The Nine Components

ihower 的整理是我看過最清楚的分類, 直接列:

| # | 元件 | 是什麼 | 誰決定 |
|:---|:---|:---|:---|
| 1 | **Instructions / System Prompt** | 模型的行為規範、角色、風格 | 開發者 |
| 2 | **User Input** | 使用者當下打字給你的 | 使用者 |
| 3 | **短期記憶** (state/history) | 這場對話到目前為止的訊息 | 系統 |
| 4 | **長期記憶** (long-term memory) | 跨對話保留的偏好、事實 | 系統/使用者 |
| 5 | **RAG 檢索資料** | 向量庫、搜尋、外部知識 | 系統動態抓 |
| 6 | **可用工具** (tools) | function calling / MCP 定義 | 開發者 |
| 7 | **工具回傳** (tool responses) | 執行後的結果 | 執行環境 |
| 8 | **結構化輸出** (structured output) | JSON schema / grammar | 開發者 |
| 9 | **全域狀態** (workflow context) | multi-step agent 的變數、進度 | 開發者/框架 |

每一塊都佔 context window 空間, 都會影響下一步的推理. Prompt 只是第 2 塊, 其他 8 塊你以前可能沒認真在管.

> [!IMPORTANT]
> **不是每個應用都用到全部 9 塊.** 一個 chatbot 可能只有 1、2、3; 一個 code agent 可能全部都用; 一個 RAG 應用特別倚重 5、7. 判斷你的應用缺哪塊、多哪塊, 才是設計的起點.

---

## 四種操作 — Four Operations (Write / Select / Compress / Isolate)

Lance Martin (LangChain) 整理的動作分類, 現在被廣泛引用. 這四個是**每個 context engineer 都要學會的動詞**:

| 操作 | 做什麼 | 實例 |
|:---|:---|:---|
| **Write** | 把資訊寫到 context window **外面**, 之後再取 | Skills 檔、long-term memory 存到向量庫、agent 主動記筆記到檔案 |
| **Select** | 從外面**挑**相關的塞進來 | RAG 檢索、記憶召回、動態載入相關工具定義 |
| **Compress** | 把塞進來的東西**壓**到 token 預算內 | 對話摘要、tool response 截斷、Claude Code 95% 時的 auto-compact |
| **Isolate** | 把某段工作**隔離**在別的 context window 跑 | sub-agent 只把結論回傳、fork 子任務、Anthropic 的 Pokémon agent 用小 agent 追局部狀態 |

四個動作全部指向同一個目標函數: **讓主要那顆模型看到的東西, 越少越乾淨越好.** Anthropic 官方原話「the smallest possible set of high-signal tokens」就是這句.

> [!TIP]
> **這四個動作可以疊用.** 一個成熟的 agent 通常是: 平常 write 到外部 memory → 每一步 select 相關的 → 用不完的 compress 掉 → 燒 context 的探索 isolate 到 sub-agent. 少了任何一個都會在某種規模下爆掉.

---

## 為什麼要這麼講究 — Three Constraints

三個原理, 全部是 transformer 架構特性, 不是可以靠訓練解決的:

**一、Attention budget 有限.** LLM 每產一個 token, 都會回頭看整個 context. 內容越長, 每個位置分到的注意力越薄. Anthropic 明說: 「LLMs have finite attention budgets that deplete with each new token.」

**二、Context rot.** 準確率會隨 token 累積下降. 原因是 transformer 的 n² 兩兩注意力關係; 越長的序列, 模型維持這些關係的能力越差. 加上訓練資料以短序列為主, 長 context 的表現不成比例地變糟.

**三、Distraction.** 模型看到不相關的東西, 會被引導往不相關的方向去. 這比「看不到」還糟, 因為它會**自信地**答錯.

所以 Context Engineering 的核心不是「塞更多」, 是「塞剛好」. 這跟姊妹篇 [Context Engineering (Claude Code 使用者角度)](./context-engineering.md) 講的「不要把所有 MCP 都裝上」是同一件事的兩個講法.

> [!WARNING]
> **「窗越大越好塞更多」是 2023 年的直覺.** 早期上下文窗小, 大家在爭誰能塞更多; 現在窗夠大了 (200k+), 但**能不能用好塞進去的東西**才是瓶頸. 200k 窗塞到 180k, 效能通常比塞 30k 差, 不是好.

---

## 常見失敗與對策 — Common Failures & Fixes

Harrison Chase 把失敗歸兩類 (missing context, poor formatting), Anthropic 補了長跑任務會遇到的另外幾種. 湊成一張速查表:

| 失敗 | 症狀 | 對策 |
|:---|:---|:---|
| **Missing context** | 模型答錯, 因為根本沒看到關鍵資訊 | 檢查 RAG 有沒有漏檢、tool 有沒有被呼叫 |
| **Poor formatting** | 資訊都在 context 裡但模型抓不到 | 加 XML 標籤或明確標題分區; 見 [Day 06](../05-notes/ironman/drafts/day06-prompt-engineering.md) 的通用範本 |
| **Context rot** | 對話越長答案越糊 | Compaction (階段性摘要)、note-taking (寫檔案)、sub-agent 隔離 |
| **Tool overload** | 工具太多, 模型選錯或該用的沒用 | 收斂到最相關的 3 到 5 個; MCP 分開安裝 |
| **Preloading everything** | 每輪都吃掉一半 window 在載無關資料 | Just-in-time retrieval: 只給 identifier, 需要時才 load |
| **Middle-of-context loss** | 塞在中間的關鍵資訊被忽略 | 最重要的放頭尾; 短的話重複一次 |

Anthropic 那篇有一條很反直覺的實作建議: **「Do the simplest thing that works.」** 不要一開始就上 sub-agent ＋ long-term memory ＋ RAG, 先做最簡單能 work 的, 出問題再加. 從第一天上四層架構, 出問題會 debug 到懷疑人生.

---

## 相關筆記 — Related

- [Context Engineering (Claude Code 使用者角度)](./context-engineering.md), 常駐/按需/工具/隔離 四層, CLAUDE.md/Skills/Hooks/Subagents 的分工
- [Harness Engineering](./harness-engineering.md), 70% AI 效能來自外層框架, Context Engineering 是其中一環
- [Loop Engineering](./loop-engineering.md), 從單輪擴大到多輪, Context 就變成跨輪的變數
- [Day 06 Prompt Engineering](../05-notes/ironman/drafts/day06-prompt-engineering.md), Prompt 只是 Context 的第 2 塊
- [Claude 顯示 thinking 是什麼機制](./how-claude-shows-thinking.md), thinking blocks 也計入 context window

## Sources

- [ihower - 什麼是 Context Engineering?](https://ihower.tw/blog/12817-context-engineering)
- [Anthropic - Effective context engineering for AI agents](https://www.anthropic.com/engineering/effective-context-engineering-for-ai-agents)
- [LangChain - The rise of Context Engineering (Harrison Chase)](https://www.langchain.com/blog/the-rise-of-context-engineering)
- [LangChain - Context Engineering for Agents (Lance Martin, Write/Select/Compress/Isolate)](https://blog.langchain.com/context-engineering-for-agents/)
- [Andrej Karpathy - "+1 for context engineering over prompt engineering"](https://x.com/karpathy/status/1937902205765607626)
- [Philipp Schmid - The New Skill in AI is Not Prompting, It's Context Engineering](https://www.philschmid.de/context-engineering)
- [Dex Horthy - 12-Factor Agents](https://github.com/humanlayer/12-factor-agents)
