---
title: AI 101 - 鐵人賽 Day 26：自架本地 LLM，什麼時候該把 AI 搬回自己機器上
tags: [ai, 鐵人賽, ironman, local-llm, ollama, vllm, 隱私, 自架, 草稿]
created: 2026-09-15
status: draft
---

# Day 26｜自架本地 LLM：什麼時候該把 AI 搬回自己機器上

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 自架的本質不是省錢，是**把一個你控制不了的變數收回來**。這篇講三個真正值得自架的理由、什麼時候不該自架，以及三行指令怎麼開始。

> **TL;DR (EN):** Self-hosting is not about cost. It is about taking back a variable you do not control. Three reasons actually justify it: data that must never leave the building, a model version that nobody can silently change under you, and freedom from someone else's rate limits and policies. Everything else — frontier capability, convenience, low-volume cost — still favours the cloud. Start with Ollama and one model sized to your VRAM; switch to vLLM only when you need to serve an API.

---

## 三個真正該自架的理由 — Three Real Reasons

**一、資料不能出門。**
客戶名單、合約、病歷、內部原始碼——有些東西不是「遮一遮就好」。遮蔽（PII masking）是一層防線，但最徹底的那層是：**根本不送出去**。

這一條通常不是技術決定，是法遵或合約決定的。一旦它成立，其他理由都不用討論了。

**二、版本不會被人動。**
這一條最少人講，但對某些工作是決定性的。

雲端模型會在你不知道的時候換版本、調 system prompt、改預設參數。你昨天測過的行為，今天可能已經不一樣了——而你不會收到通知。

**自架的權重躺在你自己的硬碟裡，今天跟明天是同一個。** 需要可重現的場景，這是唯一的解：

- 做評測、做研究，結果要能被別人重跑
- 合規稽核，要說得出「當時用的到底是什麼」
- 你自己的長期實驗，不希望變數偷偷換掉

> [!IMPORTANT]
> 這條接得回 [Day 03](./day03-what-it-cannot-do.md)：鋸齒狀前沿看不見，所以你得靠實測找出自己的邊界。**但如果模型會偷偷換版，你昨天測出來的邊界今天就作廢了。** 可重現性是驗證的前提。

**三、沒有別人的限制。**
速率限制、地區限制、內容政策、服務中斷。自架換來的是：跑多少是你的事、什麼時候跑是你的事。

延伸的一種用法是無審查（abliterated）模型——那個只能在本地跑，而且**代價不小**，Day 27 專門講。

---

## 什麼時候不該自架 — When Not To

誠實講：**多數時候，雲端還是比較好。**

| 你在意的 | 誰贏 | 為什麼 |
|---|---|---|
| 最強的能力 | **雲端** | [Day 02](./day02-why-it-got-strong.md) 那條曲線，前沿一直在雲端。本地模型永遠落後一段 |
| 低用量的成本 | **雲端** | 按用量計費。你一個月用不到幾十美元，硬體錢要回本很久 |
| 不想維運 | **雲端** | 自架你要顧硬體、電費、驅動、模型更新、磁碟空間 |
| 長文件、超長脈絡 | **雲端** | 本地模型的脈絡長度與利用率通常更吃緊 |
| 高用量、固定負載 | **自架** | 用量夠大時，一次性硬體成本才划得來 |

> [!TIP]
> 判斷法則很簡單：**先問「有沒有一條非自架不可的理由」**（資料不能出門、版本要凍結、被限制卡死）。**沒有的話，就用雲端。** 為了省錢而自架，多數人算完電費會後悔。

---

## 怎麼開始：看你的 VRAM — Getting Started

Ollama 一行裝好，三行就能跑。挑模型只看一件事：**你的顯示卡有多少 VRAM。**

| VRAM | 推薦起手 | 大小 | 適合 |
|---|---|---|---|
| **8 GB 以下** | `ollama pull qwen3:4b` | ~4 GB | 輕量任務、低延遲 |
| **16 GB** | `ollama pull qwen3:14b` | ~10 GB | 綜合 CP 值最高 |
| **32 GB** | `ollama pull qwen3:32b` | ~20 GB | 推理、程式碼、中文全能 |

三行起步：

```bash
ollama pull qwen3:14b     # 下載
ollama run qwen3:14b      # 開始對話
ollama ps                 # 看它有沒有真的跑在 GPU 上
```

`ollama ps` 那行很重要——如果 `PROCESSOR` 顯示的不是 100% GPU，代表模型塞不進顯示卡、正在用 CPU 跑，速度會慢一個量級。**看到這個就換小一號的模型。**

---

## 要對外提供 API 就換 vLLM — When You Need to Serve

Ollama 適合自己用。當你要把本地模型變成一個**服務**（多人用、接進自己的應用、跑批次），換 vLLM：

| | Ollama | vLLM |
|---|---|---|
| 定位 | 自己機器上跑一跑 | 生產級推論伺服器 |
| 吞吐量 | 夠個人用 | 比原生 HF Transformers 高 **14–24 倍** |
| 記憶體 | 一般 | PagedAttention，KV cache 幾乎零浪費 |
| API | 自有格式（也相容） | **OpenAI 相容**，既有程式碼幾乎不用改 |

最後這一格是關鍵：**OpenAI 相容代表你可以只換一個 base URL，就把整套應用從雲端切到自己的機器上**——要切回去也一樣快。

---

## 心法 — The Takeaway

把整個系列串起來看：

- [Day 02](./day02-why-it-got-strong.md)：能力曲線一直往上，**前沿永遠在雲端**
- [Day 03](./day03-what-it-cannot-do.md)：邊界看不見，所以要靠自己實測
- 今天：**實測需要一個不會偷偷改變的東西**

所以結論不是「自架 vs 雲端」二選一，而是分工：

> **前沿的活交給雲端；要可重現、要隱私、要不被別人動版本的活，留在自己的機器上。**

明天講一個只有自架才做得到、但代價比想像中大的東西：把模型的「拒絕」拿掉，會發生什麼事。

---

## Sources

- [Ollama 指令教學](../../../04-local-llm/ollama-guide.md)
- [輕量模型推薦（依 VRAM 分級）](../../../04-local-llm/lightweight-models.md)
- [vLLM — 高吞吐量推論伺服器](../../../04-local-llm/vllm.md)
- [PII Masking（隱私遮蔽）](../../../03-tools/pii-masking.md)
- [Qwen3.6 27B 原版 vs Uncensored](../../../04-local-llm/qwen3-6-27b-uncensored.md)
- [vLLM 官方文件](https://docs.vllm.ai)
- [Ollama 官網](https://ollama.com)
