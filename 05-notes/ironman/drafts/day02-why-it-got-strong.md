---
title: AI 101 - 鐵人賽 Day 02：不是它突然變強，是它跨過了你的門檻
tags: [ai, 鐵人賽, ironman, llm, 歷史, scaling, agent, 草稿]
created: 2026-09-02
status: draft
---

# Day 02｜不是它突然變強，是它跨過了你的門檻

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> LLM 又不是昨天才有的東西，為什麼「感覺上」是最近才突然爆發？這篇把三個引擎和一條指數曲線攤開來看——你會發現，每一次「突然」，拆開來都不突然。

> **TL;DR (EN):** Capability didn't spike — it has been growing exponentially the whole time. METR measured it: the length of task an agent can finish (at 50% success) doubled every 7 months from 2019, and every 4 months during 2024–2025. Two seconds for GPT-2, fifty minutes for Claude 3.7 Sonnet, nearly two hours for o3. An exponential looks like nothing is happening until it crosses your threshold. Late 2025 is simply when that curve met a mature tool ecosystem, and delegating real work became possible for the first time.

---

## 先修一個前提 — It Crossed a Line, It Didn't Spike

> （jason3e7 的疑問）LLM 以前就有，但好像去年底突然大爆發，為什麼？

答案有點反直覺：**能力沒有突然爆發，它一路都在指數成長。爆發的是「它跨過了對你有用的那條線」。**

研究機構 METR 把這件事量化得很漂亮。他們拿 2019 到 2026 年最強的 agent，測了約 230 個任務（多數是寫程式），問一個很實際的問題：**這個 agent 能穩定完成多長的任務？** 標準訂在成功率 50%。

| 模型世代 | 能撐住的任務長度 |
|---|---|
| GPT-2 | 約 2 秒 |
| Claude 3.7 Sonnet | 約 50 分鐘 |
| o3 | 將近 2 小時 |

這條線在 2019–2025 年間**每 7 個月翻一倍**；而 2024–2025 這一段更快，**每 4 個月就翻一倍**。

指數曲線的特性就是這樣——**在跨過你的門檻之前，看起來什麼都沒發生：**

- 撐 2 秒 → 你連當玩具都不會想到它
- 撐 30 秒 → 好像可以幫忙查個東西
- 撐 5 分鐘 → 可以寫一個小函式
- **撐 50 分鐘到 2 小時 → 可以交辦一件真的工作**

前三格你都不會覺得「爆發」。跨到第四格的那一刻，你的體感就是世界一夕之間變了。**曲線沒有變，是你的門檻被跨過了。**

---

## 三個引擎，先後點火 — Three Engines

那條指數曲線是三件事撐起來的，而且是**先後**點火，不是同時。

### 引擎一：把規模變成可預測的投資（2017–2022）

- **2017** Transformer 架構問世（《Attention Is All You Need》）：可以平行訓練，才有辦法一路放大。
- **2020** Scaling Laws（Kaplan et al.）：模型表現與算力、資料量、參數量呈**可預測**的冪次關係。
- **2022** Chinchilla（Hoffmann et al.）：修正配方——當時的大模型其實「餵太少」，同樣算力下該用更多資料。

第二點才是真正的轉折。它把「這條路走不走得通」變成「**你想花多少錢**」。一件事只要能用錢換，產業就會全力押注。

### 引擎二：讓它聽得懂人話（2022）

GPT-3 在 2020 年就存在了，但很難用——你得用很技巧的方式去「引導」它接話。2022 年的 InstructGPT（Ouyang et al.）用 RLHF（從人類回饋中做強化學習）把模型調成「照指令做事」。

這一步沒有讓模型更聰明，是讓它**變得可用**。同年 11 月 ChatGPT 之所以炸開，是**產品時刻，不是模型突破**——底層模型早就在那裡了。

> [!TIP]
> 這是第一次「感覺上的爆發 ≠ 技術上的突破」。同樣的錯覺，後面還會再發生一次。

### 引擎三：回答的時候多想一下（2024 起）

2024 年 OpenAI 的 o1 系列開了第二根軸：不是把模型訓練得更大，而是**在回答時多給它一點算力去想**。

這根軸特別的地方在於，**它可以用時間換聰明**——一個小模型讓它想 10 秒，可以贏過一個大上十幾倍、但秒答的模型。從 o1 到 o3，投在強化學習上的算力增加了超過 10 倍。

到這裡，模型已經很強了。但你去年感覺到的那次爆發，還缺最後一塊。

---

## 你感覺到的那一次：它長出了手 — When the Model Got Hands

前面三個引擎讓模型「會想」；2024 年底開始的這一波，讓它「**能動**」。

- **MCP（Model Context Protocol）**：2024 年 11 月由 Anthropic 提出，讓模型用同一套規格接上檔案、資料庫、API 與外部工具。採用速度很誇張——下載數從 2024 年 11 月的約 10 萬，到 2025 年 4 月超過 800 萬；2025 年底已有 5,800 個以上的 server、300 個以上的 client。RedMonk 說這是他們看過**採用最快的標準**。
- **寫程式工具從「自動補完」變成「交辦任務」**：這個轉折在 2025 年底完成。你不再是一行一行接受建議，而是給一個目標，讓它自己讀檔、修改、跑測試、看錯誤、再修。

把兩件事乘起來，就是答案：

> **（能撐一到兩小時的任務長度）×（能接上你真實的檔案與工具）＝ 第一次可以「交辦一件真事」。**

所以「去年底突然大爆發」的實際內容是：**不是模型在那個月變聰明了，而是模型的能力曲線，剛好在那時候跟工具生態的成熟度撞在一起。**

順帶一提，同一段時間也出現了「尺不夠長」的訊號：SWE-bench Verified 這個寫程式評測 2024 年 8 月才推出，到 2026 年 OpenAI 已公開表示它**不再足以衡量前沿的寫程式能力**。當你的量尺跟不上，你就會覺得到處都在爆發。

---

## 這對我們的意義 — What This Means

三條可以直接拿去用的心法：

**一、「現在不行」的保存期限很短。**
如果曲線 4 到 7 個月翻一倍，那你半年前試過、覺得「AI 做不到」的事，現在很可能做得到。**把失敗過的嘗試記下來，每季重試一次**，比每天追新聞有用得多。

**二、你能施力的地方不是模型。**
爆發來自「模型能力 × 工具接得好不好 × 任務切得夠不夠準」這個乘積。模型你改不了，但另外兩項完全在你手上——那正是 Day 06 要先看的 Loop，以及 Day 12–13 要回頭補的 Context 與 Harness。

**三、那條線的定義是「成功一半」，不是「一定成功」。**
METR 的門檻訂在 50%。換句話說，**在能力邊界上，它有一半機率是錯的。** 這不是叫你別用，是叫你在用之前就先想好怎麼驗——那是 Day 19–21 的主題。

最後留一個提醒：上面講的每一個「突然」，拆開來看都不突然。

> **下次聽到「AI 又爆發了」，先問一句：是它的能力變了，還是我的門檻被跨過了？**

---

## Sources

- [Measuring AI Ability to Complete Long Software Tasks — METR, 2025](https://metr.org/blog/2025-03-19-measuring-ai-ability-to-complete-long-tasks/)
- [A new Moore's Law for AI agents — AI Digest](https://theaidigest.org/time-horizons)
- [Attention Is All You Need — Vaswani et al., 2017](https://arxiv.org/abs/1706.03762)
- [Scaling Laws for Neural Language Models — Kaplan et al., 2020](https://arxiv.org/abs/2001.08361)
- [Training Compute-Optimal Large Language Models（Chinchilla）— Hoffmann et al., 2022](https://arxiv.org/abs/2203.15556)
- [Training language models to follow instructions with human feedback（InstructGPT）— Ouyang et al., 2022](https://arxiv.org/abs/2203.02155)
- [The Model Context Protocol's impact on 2025 — Thoughtworks](https://www.thoughtworks.com/en-us/insights/blog/generative-ai/model-context-protocol-mcp-impact-2025)
- [10 Things Developers Want from their Agentic IDEs in 2025 — RedMonk](https://redmonk.com/kholterhoff/2025/12/22/10-things-developers-want-from-their-agentic-ides-in-2025/)
- [Introducing SWE-bench Verified — OpenAI](https://openai.com/index/introducing-swe-bench-verified/)
