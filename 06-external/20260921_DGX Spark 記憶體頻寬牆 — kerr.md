---
title: "DGX Spark 記憶體頻寬牆: 4 台不夠服務 30 個 RD"
tags: [ai, 外部觀點, 地端 LLM, dgx-spark, 記憶體頻寬, 併發, capacity-planning, gemma, qwen]
source: https://github.com/kerr20801/kerr-notes/blob/main/ai-systems/2026-09-04-dgx-spark-memory-bandwidth-wall.md
author: kerr (@kerr20801)
created: 2026-09-21
---

# DGX Spark 記憶體頻寬牆 — The Memory Bandwidth Wall

[← 回主頁](../index.md)

> [!NOTE]
> 原文: [kerr-notes: 4 台 DGX Spark 想服務 RD？記憶體頻寬這關過不了](https://github.com/kerr20801/kerr-notes/blob/main/ai-systems/2026-09-04-dgx-spark-memory-bandwidth-wall.md) (2026-09-04). kerr 寫的實測筆記, 短、密, 每一句都對應一組數字. **這篇特別值得推的地方是: 它不是講一個經驗, 是把「便宜硬體加起來 = 便宜服務」這個直覺乾淨地打死, 而且用兩顆同尺寸不同架構的模型示範同一個瓶頸的兩種撞法.**

> **TL;DR (EN):** Real deployment note. Plan was to serve 30 RD with 4 desktop DGX Spark (GB10, 128GB unified memory) as a cheap on-prem alternative. Bandwidth math kills it. LPDDR5X sustains 200 to 240 GB/s vs H200 HBM3e ~4,800 GB/s (17x gap), and 4 units can't be merged because the 200GbE interconnect is 1/10 of local memory bandwidth. Stress test with 20K coding context: Qwen3.8:27b (full attention) fully serializes — 12 parallel users → first-token wait 179 seconds; gemma4:26b (sliding window) batches but tanks from 48 to 14 tok/s the moment user 2 joins. Aggregate on 4 units maxes ~320 tok/s (gemma-class) — enough for 10 people at 30 tok/s, not 30 people at 30 tok/s. Real conclusion: single RTX Pro 6000 beats 12 DGX Spark for shared serving; DGX Spark's actual fit is single-team helper / dev / CI / edge.

---

## 原文重點 — Post's Key Points

**背景**: kerr 團隊想找便宜的地端方案, 讓 30+ 個 RD 用自架 AI 做 agentic 開發. DGX Spark 桌機 (GB10, 128GB 統一記憶體) 一台不到伺服器等級的價格, 4 台串起來聽起來剛好. 一開始想的是「容量夠不夠、算力夠不夠」, 都想錯了. 真正的瓶頸是**記憶體頻寬**.

**紙上算下來**:

| 硬體 | 記憶體 | 頻寬 (GB/s) | 相對 DGX Spark |
|:---|:---|:---|:---|
| **DGX Spark GB10** | LPDDR5X | 200 to 240 (實測) | 1× |
| RTX Pro 6000 | GDDR7 | ~1,800 | 8× |
| H200 | HBM3e | ~4,800 | 20× |

還有一個致命細節: **4 台不能合起來當一台用.** 單元間互連只有 200GbE (25 GB/s), 是本機頻寬的 1/10. 4 台實際上是 4 座獨立孤島, 各跑一份 replica.

**實測 (Ollama, 20K coding context, 每題生 400 tokens)**:

| 並發 | Qwen3.8:27b (full attention) | gemma4:26b (sliding window) |
|:---|:---|:---|
| 1 | 25 tok/s | 48 tok/s |
| 2 | 25 | **14** |
| 6 | 25 (首 token 等 82 秒) | 13.5 |
| 12 | 25 (首 token 等 **179 秒**) | 13.7 |

- **Qwen3.8 完全序列化.** full-attention KV cache 在 20K context 太重, Ollama 實際只開 1 個 slot. 每人固定 25 tok/s, 請求排隊做. 整台 decode 卡在 24 tok/s.
- **gemma4 會批次, 但每人腰斬.** sliding-window 讓 KV 夠小, 批次有效, 整台總量能爬到 ~80 tok/s. 但頻寬就 220 GB/s, 第 2 人進來每人立刻從 48 掉到 14, 之後平在那.

**4 台的實際容量**:

- gemma 級模型: 4 台聚合約 320 tok/s → 10 人各 30 tok/s, 或 30 人各 11 tok/s
- 要 30 人同時各 30 tok/s (~900 tok/s): 需要 11 到 12 台 gemma 級 Spark, **或一張 RTX Pro 6000**

**收線句**: 「便宜的硬體省的是採購單上的數字. 頻寬省不了.」

---

## 為什麼有趣 — Why This Is Actually Interesting

kerr 自己講短、講平, 沒有多說. 但這篇值得單獨收, 有四層:

### 一、把三個常見直覺乾淨反駁

大多數人買 AI 硬體的思考順序是**容量 → 算力 → 頻寬**. kerr 這篇證明真實順序是**倒過來的**:

- 「容量夠不夠」: 128 GB × 4 = 512 GB, 隨便塞 27B 模型都夠. 沒用
- 「算力夠不夠」: GB10 有一定算力, 但 decode 是**記憶體 bound 不是算力 bound**. 沒用
- 「頻寬夠不夠」: 這才是實際卡的地方

這個順序反轉本身就值得記下來. 下次評估地端 LLM, 先看頻寬再說.

### 二、示範同一個瓶頸的兩種撞法

Qwen3.8:27b 跟 gemma4:26b 參數量幾乎一樣, 撞牆結果**完全不同**:

- **Full attention 的撞法**: KV cache 大到只能開 1 個 slot, 於是排隊. 總吞吐固定在單流的 25, 加人只讓大家等更久 (12 人時首 token 等 3 分鐘)
- **Sliding window 的撞法**: KV cache 小, 能批次, 總吞吐爬到 80. 但每人的速率被頻寬瓜分, 從 48 掉到 14 就平了

一個是**延遲爆炸**, 一個是**單人速率崩潰**. 兩者都是同一個頻寬牆, 但體感完全不同. 這種「同瓶頸不同症狀」的並列示範是這篇最珍貴的地方.

### 三、互連頻寬是隱形殺手

200GbE 聽起來很快. 但拿來跟 GB10 本機的 ~273 GB/s 比只有 1/10. **這代表 4 台不能拿去跑一個大模型**, 只能各跑各的. 這是很多人買 cluster 才發現的教訓, kerr 用一句話解釋完.

一般化的原則: **凡是「多台合起來變一台」的方案, 都要先看互連頻寬跟本機頻寬的比例.** 差一個數量級就別想.

### 四、「單流數字會騙人」有量化證據

gemma4:26b 單人 48 tok/s, 看起來很棒. 第 2 人進來就變 14. **這是 71% 的速率損失, 只加了 1 個人.** 大部分產品評測、部落格 benchmark 都只跑單流 (好看好寫), 讀者容易誤判. 這篇 4 行表格說服力比 40 段文字強.

---

## 跟同一天整理的 H200 那篇並排 — Cross-Reference

同一天收的 [8× H200 跑 Qwen3.8-Flash-Next 併發實測](./20260921_8×%20H200%20跑%20Qwen3.8-Flash-Next%20併發實測：顯存不是瓶頸,%20速度才是%20—%20FB%20貼文.md) 講的是**高端**硬體的併發塌陷. kerr 這篇是**桌上型**硬體. 兩篇拼起來看, 一條完整的曲線就出來了:

| 級別 | 頻寬 | Decode 每人 tok/s | 服務尺度 |
|:---|:---|:---|:---|
| **DGX Spark GB10** | ~220 GB/s | 14 to 25 | 單一團隊助手 (< 10 人) |
| RTX Pro 6000 | ~1,800 GB/s | (推估) 中檔 | 小型部門 (~30 人) |
| **8× H200** | 4,800 GB/s × 8 卡 | 20+ | 大部門 (32K 場景 27 人) |

**共通點**: 都是頻寬 bound, 都不能靠加人變快, 都必須限流排隊維持 SLA. **差別**: 只是位置在頻寬光譜的哪一段.

如果 H200 那篇教會你「H200 也不能無限併發」, kerr 這篇補上「桌機更是」, 加起來的心智模型就穩了.

---

## 我的重點 — Takeaways

- **買 AI 硬體先問「持續頻寬多少 GB/s」, 再問其他**. 容量最好回答但最不重要, 頻寬最不好回答但最卡瓶頸
- **互連頻寬 vs 本機頻寬要看比例**. < 1/5 基本上就別想合成一台大機器用
- **模型架構決定撞牆的形狀**. full-attention 會排隊 (延遲爆), sliding-window 會分蛋糕 (單人速率崩). 選模型不能只看參數量
- **單流 benchmark 沒有意義**, 一定要壓 concurrency 才知道真實表現
- **Ollama 的 `OLLAMA_NUM_PARALLEL` 是硬限制**. 多人 serving 要換 vLLM 的連續批次 (continuous batching)
- **DGX Spark 的正確定位是 dev / CI / 邊緣節點 / 單一團隊助手**, 不是共享 serving fabric. 想服務 30 人 → 一張 RTX Pro 6000 比 12 台 Spark 便宜也簡單

---

## 相關筆記 — Related

- [8× H200 跑 Qwen3.8-Flash-Next 併發實測](./20260921_8×%20H200%20跑%20Qwen3.8-Flash-Next%20併發實測：顯存不是瓶頸,%20速度才是%20—%20FB%20貼文.md), 同樣是頻寬 bound 的故事, 高端硬體版
- [vLLM](../04-local-llm/vllm.md), 連續批次是 Ollama 那個排隊問題的答案
- [輕量模型推薦](../04-local-llm/lightweight-models.md), 桌機硬體選型
- [Ollama 指令教學](../04-local-llm/ollama-guide.md), Ollama 適用邊界

## Sources — Sources

- 原文: [kerr-notes - dgx-spark-memory-bandwidth-wall](https://github.com/kerr20801/kerr-notes/blob/main/ai-systems/2026-09-04-dgx-spark-memory-bandwidth-wall.md)
- [NVIDIA DGX Spark 官方頁 (GB10 spec)](https://www.nvidia.com/en-us/products/workstations/dgx-spark/)
- [Ollama OLLAMA_NUM_PARALLEL 文件](https://github.com/ollama/ollama/blob/main/docs/faq.md)
- [vLLM continuous batching / PagedAttention 原始 blog](https://blog.vllm.ai/2023/06/20/vllm.html)
