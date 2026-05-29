---
title: "AI System Design Guide：Staff 工程師的 AI 系統設計完全指南"
tags: [ai, 外部觀點, system-design, rag, agent, mcp, a2a, evals, mlops, 面試, staff-engineer, ombharatiya]
source: https://github.com/ombharatiya/ai-system-design-guide
author: Om Bharatiya（ombharatiya）
created: 2026-05-29
---

# AI System Design Guide：Staff 工程師的 AI 系統設計完全指南

> [!info]
> GitHub：[ombharatiya/ai-system-design-guide](https://github.com/ombharatiya/ai-system-design-guide)（974 ★ · 236 forks · MIT）
> **一句話：** 持續更新至 2026 年 5 月的 AI 系統設計活文件，18 個章節涵蓋從 LLM 原理到生產部署的完整知識，附 110+ 道真實面試題，適合準備 Staff / Principal 面試或打造生產級 AI 系統。

---

## 為什麼值得收藏

與一般書籍不同，這份指南是**活文件**：
- **即時追蹤**新模型發布（截至 2026 年 5 月，含 Claude Opus 4.7、GPT-5.5、Gemini 3.1 Pro、DeepSeek V4、Llama 4）
- **即時更新**演進中的協議（MCP 2.0、A2A protocol）
- **直接點出 tradeoff**，不刻意保持廠商中立

---

## 18 個章節架構

```
Foundations（基礎）
  ├── LLM 原理與 Transformer 架構
  ├── 模型選擇指南（含 2026 年 5 月最新定價）
  └── Prompting 策略

Building（建構）
  ├── RAG 系統（Chunking、向量資料庫、Reranking、ColBERT）
  ├── Agentic 架構
  └── 工具使用協議（MCP 2.0、A2A）

Operations（運維）
  ├── 推理優化
  ├── 記憶體管理
  ├── 框架（LangGraph、DSPy）
  └── 基礎設施

Governance（治理）
  ├── 安全性與存取控制
  ├── 可靠性
  ├── 安全護欄
  └── Evaluation pipeline

Application（應用）
  ├── 設計模式
  ├── 20+ 生產案例
  └── 面試準備
```

---

## 110+ 面試題涵蓋範圍

| 主題 | 代表問題方向 |
|---|---|
| RAG 設計 | Chunking 策略選擇、向量資料庫比較 |
| Agent 除錯 | 多步驟 Agent 的 failure mode |
| 多租戶隔離 | 企業 SaaS 的資料隔離架構 |
| 成本 / 延遲 tradeoff | 如何在精準度與速度之間取捨 |

> [!tip]
> 面試題持續更新到 2026 年 5 月，含答題框架與白板練習，適合 Staff / Principal 等級職位。

---

## 20 個生產案例

涵蓋場景：
- 即時搜尋
- 自主 Coding Agent
- 多租戶 SaaS
- 文件智慧處理
- 推薦引擎
- 語音醫療
- 詐欺偵測
- 跨舊系統 UI 的 Computer-Use 自動化

---

## 進階資源

| 資源 | 說明 |
|---|---|
| **AI Evals 深度指南** | 兩份 3,000+ 行指南，涵蓋 Phoenix/Langfuse 與 LangWatch 整合 |
| **課程推薦** | 精選學習路徑 |
| **職涯轉換指南** | 從後端 / QA / PM 轉 AI 工程師的路徑 |
| **術語表** | 所有 AI 系統設計術語定義 |

---

## 安裝方式

```bash
git clone https://github.com/ombharatiya/ai-system-design-guide.git
```

或直接在 GitHub 上閱讀，持續追蹤更新。

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — Agent 架構設計的核心觀念，本指南的 Agentic 章節對應
- [[Is Grep All You Need：向量搜尋不如 grep 的 PwC 研究 — Wisely Chen]] — RAG 章節的批判性補充
- [[Anthropic Cybersecurity Skills：754 個 AI 資安技能庫 — ThreatVector]] — 安全性章節的實務延伸

---

## 來源

- GitHub：[ombharatiya/ai-system-design-guide](https://github.com/ombharatiya/ai-system-design-guide)
