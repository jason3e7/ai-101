---
title: "Is Grep All You Need？向量搜尋不如 grep 的 PwC 研究"
tags: [ai, 外部觀點, rag, vector, grep, retrieval, agent, harness, 研究論文, wisely-chen]
source: https://www.facebook.com/thegiivee/posts/pfbid027yrGtEiUFTwABhyfTS2Xs73uazDVAHVD4SGbFZgMr4SrKRSESjHDcH1pJU5wJ6v6l
author: Wisely Chen
created: 2026-05-27
---

# Is Grep All You Need？向量搜尋不如 grep 的 PwC 研究

> [!info]
> Facebook 原文：[Wisely Chen](https://www.facebook.com/thegiivee/posts/pfbid027yrGtEiUFTwABhyfTS2Xs73uazDVAHVD4SGbFZgMr4SrKRSESjHDcH1pJU5wJ6v6l)
> 論文：[Is Grep All You Need? How Agent Harnesses Reshape Agentic Search](https://arxiv.org/abs/2605.15184)（PwC，2026-05-14）
> **一句話：** PwC 用 116 道題測了四個 Agent 框架，發現 grep 在 Agentic RAG 中全面勝過向量搜尋——更震撼的發現是：框架（Harness）本身的影響力不亞於搜尋方法。

---

## 核心問題

建 RAG 系統時，大家預設向量資料庫（Vector DB）是標配。
這篇論文問了一個簡單卻刺耳的問題：

> **「Grep 就夠了嗎？」**

---

## 實驗設計

| 項目 | 內容 |
|---|---|
| **測試集** | LongMemEval 116 道題（從長對話歷史中回答特定事實）|
| **四個 Agent 框架** | Chronos（自研）、Claude Code、Codex、Gemini CLI |
| **搜尋方法** | Grep（字面搜尋）vs 向量搜尋 |
| **輸出模式** | Inline（直接回傳內容）vs Programmatic（存檔再讀）|

---

## 結果一：Grep 全面勝出

在 Inline 模式下，Grep 在所有測試組合中都優於向量搜尋：

| 組合 | Grep 準確率 | 向量搜尋準確率 | 差距 |
|---|---|---|---|
| Chronos + Gemini 3.1 Flash-Lite | **86.2%** | 62.9% | +23.3 pp |
| Chronos + GPT-5.4 | **93.1%** | 83.6% | +9.5 pp |
| Claude Code + Claude Opus 4.6 | **76.7%** | 75.0% | +1.7 pp（最小）|

### 為什麼 Grep 贏？

LongMemEval 的題目需要「找到精確的文字片段」：確切日期、計數、ID、偏好設定、錯誤訊息。

> 「這些字串在 tokenization 後通常保持穩定，Grep 可以直接找到，不需要經過 Embedding 這層有損壓縮。」

向量搜尋的問題：
- **Vocabulary mismatch**：Agent 查詢時用了錯誤的詞，什麼都找不到
- **Embedding 是有損壓縮**：精確細節（日期、ID）在高維空間裡可能被稀釋

---

## 結果二：Harness 框架的影響不輸搜尋方法本身

**同樣的模型（Claude Opus 4.6）、同樣的資料、同樣的搜尋方法（Grep）：**

| Harness | 準確率 |
|---|---|
| Chronos（自研）| **93.1%** |
| Claude Code | 76.7% |
| **差距** | **16.4 個百分點** |

> [!warning] 這是論文最重要的發現
> 光是換一個 Agent 框架，在完全相同的其他條件下，準確率差了 16.4%。
> **Harness 框架的重要性，不亞於你選擇哪種搜尋方法。**

---

## 結果三：Programmatic 模式的災難性退化

把工具結果存成檔案再讓 Agent 讀（而非直接 Inline 回傳），效能崩潰：

| 模式 | Codex + GPT-5.4 準確率 |
|---|---|
| Inline Grep | **93.1%** |
| Programmatic Grep | 55.2% |
| **退化幅度** | **−37.9 個百分點** |

論文稱此為「端到端的脆弱性（End-to-End Brittleness）」——一個看似無關的實作細節，幾乎讓系統準確率腰斬。

---

## 重要限制

> [!tip] 這個研究的適用範圍
> LongMemEval 測試的是**每位使用者的對話歷史記憶**，不是真實企業的文件庫。
>
> - 對話歷史的字元是「精確、穩定、不重複」的
> - 企業文件庫的語意往往「模糊、同義詞多、需要概念理解」
>
> **結論**：Grep 在「精確事實查找」場景確實有優勢，但向量搜尋在「語意相似查找」的場景（自然語言查詢、跨語言、同義詞）仍有其不可取代的價值。

---

## 對 RAG 系統設計的啟示

1. **不要預設向量 = 必要**：先評估你的查詢類型是「精確匹配」還是「語意相似」
2. **Harness 框架的選擇和調校，可能比搜尋方法更關鍵**
3. **Inline vs Programmatic 是細節，但影響巨大**：工具輸出的呈現方式直接影響 LLM 能讀到什麼
4. **Hybrid 搜尋**：Grep 作為精確層、向量作為語意層，組合使用可能是實務上最穩的做法

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — Harness 框架影響 70% 效能的完整觀念，這篇論文是實證
- [[AI 101 - Context Engineering]] — 工具輸出如何呈現給 LLM，正是 Context 設計的一環
- [[蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt — 新人類聯盟]] — 框架設計決定 AI 系統品質的另一個角度

---

## 來源

- Facebook：[Wisely Chen](https://www.facebook.com/thegiivee/posts/pfbid027yrGtEiUFTwABhyfTS2Xs73uazDVAHVD4SGbFZgMr4SrKRSESjHDcH1pJU5wJ6v6l)
- 論文：[arXiv:2605.15184](https://arxiv.org/abs/2605.15184)
- HTML 版：[arxiv.org/html/2605.15184v1](https://arxiv.org/html/2605.15184v1)
