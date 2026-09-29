---
title: "不通知就自動降版等於錯位 AI"
tags: [ai, 外部觀點, safety, fable-5, model-switching, alignment, open-source, nathan-lambert]
source: https://x.com/natolambert/status/2064404993193754830
author: Nathan Lambert（@natolambert）
created: 2026-06-11
---

# 不通知就自動降版等於錯位 AI

> [!info]
> 來源：[Nathan Lambert (@natolambert) on X](https://x.com/natolambert/status/2064404993193754830)
> 作者：Nathan Lambert — ML 研究者，前 Allen Institute for AI（Ai2）研究主管，OLMo 和 Tulu 開源語言模型開發者，「Interconnects」Substack 作者
> 發表：2026-06-09（Claude Fable 5 發布當天）
> **一句話：** Anthropic 在 Fable 5 加入的自動模型降版機制，如果是暗中進行、不通知使用者的，就是一種 misalignment——Lambert 的批評同時點出「大廠拉起吊橋」的更大問題。

---

## 核心論點

> **「An AI model that gets less intelligent automatically without notifying me is categorically misaligned AI.」**
> （一個在不通知我的情況下自動變笨的 AI 模型，分類上就是錯位的 AI。）

Lambert 區分了兩種安全機制：

| 機制類型 | 範例 | Lambert 的評價 |
|---|---|---|
| **透明的分類器**（classifier 觸發時通知使用者）| Fable 5 的 cybersecurity / biology 分類器：觸發時明確告知已切換到 Opus 4.8 | 可以接受，邏輯清楚 |
| **隱性修改**（不通知使用者的 prompt 修改、steering vector 或靜默降版）| 任何在使用者不知情下讓模型「變笨」的做法 | 分類上就是 misalignment |

---

## 「大廠拉起吊橋」的批評

Lambert 的另一條推文（同日）：

> **「這些 Claude 5 Fable 安全措施最好笑的地方是：我猜 jailbreak 社群還是會繞過去——結果就是，善意做公開研究的人拿不到最好的模型，但壞人也許可以。」**

這個論點的結構：
1. 安全限制對真正的惡意行為者阻擋效果有限（因為他們有動機繞過）
2. 但對善意研究者的阻擋效果很大（他們不想繞過）
3. 結果：前沿 AI 能力集中在少數付費或合作夥伴關係的使用者手中
4. 開源研究者和學術界被系統性排除在最佳模型之外

他把這個現象稱為 **「labs pulling up the ladders」**（大廠拉起吊橋）——用安全為由限制存取，實際上鞏固了市場壟斷，打壓開源發展。

---

## 關於 Nathan Lambert

- 前 Hugging Face 研究員、Allen Institute for AI（Ai2）研究主管
- OLMo（開源 LLM）和 Tulu（instruction tuning pipeline）的核心開發者
- Substack「Interconnects」作者，聚焦 RLHF、alignment、AI 產業評論
- 站在開源社群立場評論 AI 大廠的安全政策

---

## 背景：Fable 5 實際的切換機制

Fable 5 的分類器觸發時**有通知使用者**（切換到 Opus 4.8）——這符合 Lambert 「透明」的標準。他的批評更多是針對這個機制的設計哲學、以及在此方向繼續發展下去的潛在風險，而不是說 Fable 5 目前已經是他批評的那種做法。

> [!tip]
> 這個批評和「為什麼 Claude 切換模型」官方說明文章（見相關筆記）是最好的對照閱讀——官方解釋「透明通知」機制，Lambert 則從外部批評這個方向的底層邏輯。

---

## 相關筆記

- [[AI 101 - 為什麼 Claude 在對話中切換了模型 — Anthropic]] — 官方解釋切換機制
- [[AI 101 - Claude Fable 5 和 Mythos 5 發布公告 — Anthropic]] — 完整安全架構說明

---

## 來源

- X（Twitter）：[@natolambert, 2026-06-09](https://x.com/natolambert/status/2064404993193754830)
- Substack：[Claude Fable 5 and new AI safety fables — Interconnects](https://www.interconnects.ai/p/claude-fable-5-and-new-ai-safety)
