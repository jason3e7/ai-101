---
title: AI 101 - Mistral AI
tags: [ai, 工具, mistral, llm, 開源, moe, api, 法國]
created: 2026-06-02
---

# AI 101 - Mistral AI

> [!info]
> 法國 AI 新創，以 **Apache 2.0 開源**為核心策略，主打高效率、多語言、可自架，估值 58 億美元，投資人包含 NVIDIA 與 Cisco。

---

## 快速定位

| 比較維度 | Mistral | OpenAI | Claude | Gemini |
|---|---|---|---|---|
| 開源策略 | 大部分模型開源（Apache 2.0）| 閉源 | 閉源 | 閉源 |
| 特色 | 效率高、多語言、可自架 | 應用生態最豐富 | 隱私優先 | Google 整合 |
| 部署彈性 | 雲端 + 自架 + 本地 | 主要雲端 | 主要雲端 | 主要雲端 |

---

## 模型全覽

### 通用模型

| 模型 | 特色 | 開源 |
|---|---|---|
| **Mistral Medium 3.5** | 128B dense，指令跟隨 + 推理 + 編碼 | ✅ |
| **Mistral Large** | 多模態通用旗艦 | ✅ |
| **Mistral Small 4** | 高效小模型，兼顧指令 + 推理 + 編碼 | ✅ |
| **Ministral** | 文字 + 視覺，輕量最佳化 | ✅ |

### 專業模型

| 模型 | 特色 | 開源 |
|---|---|---|
| **Devstral 2** | 前沿程式碼 Agentic 模型，軟體工程 Agent | ✅ |
| **Codestral** | 程式碼補全專用 | ❌ |
| **Voxtral TTS** | 零樣本語音複製，支援 9 種語言 | ✅ |
| **OCR 3** | 文件 AI 技術棧 | ❌ |
| **Mistral Moderation** | 文字分類 + 有害內容過濾 | ❌ |

### API 定價參考（舊世代，僅供比較）

| 模型 | 輸入 / 輸出（per 1M tokens）|
|---|---|
| Mistral 7B | $0.25 / $0.25 |
| Mixtral 8x7B（MoE）| $0.70 / $0.70 |
| Mistral NeMo | $0.15 / $0.15 |
| Mistral Large | $2.00 / $6.00 |

> [!tip] 最新定價以 [mistral.ai/models](https://mistral.ai/models) 為準

---

## 核心技術：MoE（稀疏混合專家）

Mixtral 系列使用 **Sparse Mixture of Experts**：

```
Mixtral 8x7B
  └── 總參數量：450 億
  └── 每次推理僅啟動：129 億（約 28%）
  └── 效果：效能接近更大模型，運算成本大幅降低
```

每個輸入 token 只路由到最相關的「專家」子網路，其餘閒置——這是 Mistral 在同等硬體上效率更高的核心原因。

---

## 產品線

| 產品 | 定位 |
|---|---|
| **Le Chat** | 聊天機器人，支援文件分析、圖片生成、網路搜尋、協作編輯 |
| **Studio** | 建構、測試、部署 AI Agent 與應用的開發平台 |
| **Vibe** | 自主 Agent，處理長程任務 |
| **Vibe for Code** | 終端機 + IDE + 背景執行的 Coding Agent |
| **Forge** | 客製化模型訓練與對齊 |
| **Compute** | 訓練與推論基礎設施 |

---

## 部署方式

```
Mistral 模型
├── Mistral Cloud（EU 託管）
├── 雲端夥伴：AWS、GCP、Azure、SAP、IBM、Snowflake、NVIDIA
└── 自架
    ├── On-premises（企業內部）
    ├── Edge
    └── 本地（透過 Ollama 執行）
```

### 用 Ollama 跑 Mistral

```bash
# 輕量版（7B，需 ~8GB RAM）
ollama run mistral

# MoE 版（8x7B，需 ~32GB RAM）
ollama run mixtral

# 程式碼專用
ollama run codestral
```

---

## 何時選 Mistral

| 情境 | 建議 |
|---|---|
| 需要自架、不能上雲 | ✅ Mistral 開源，可完全自控 |
| 多語言任務（非英文）| ✅ 多語言支援是強項 |
| 程式碼生成 / Agentic 開發 | ✅ Devstral 2 / Codestral |
| 語音合成 | ✅ Voxtral TTS |
| 需要最強推理 / 複雜對話 | 考慮 Claude Opus 或 GPT-5 |
| 需要豐富第三方整合 | 考慮 OpenAI 生態 |

---

## Sources

- [mistral.ai 官網](https://mistral.ai/)
- [Mistral 模型頁](https://mistral.ai/models)
- [Mistral AI 完整介紹 — solwen.ai](https://solwen.ai/posts/mistral-ai)
