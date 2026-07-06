---
title: "Anthropic 重新部署 Fable 5：安全防護與越獄評估"
tags: [ai, 外部觀點, anthropic, fable-5, 安全, jailbreak, classifier, 官方公告]
source: https://www.anthropic.com/news/redeploying-fable-5
author: Anthropic
created: 2026-07-06
---

# Anthropic 重新部署 Fable 5：安全防護與越獄評估 — Redeploying Fable 5

[← 回主頁](../index.md)

> [!NOTE]
> 原文：[Redeploying Fable 5 — Anthropic（2026-06-30，7-01 更新）](https://www.anthropic.com/news/redeploying-fable-5)
> **一句話：** Fable 5 因一個資安越獄被美國政府出口管制而暫停，Anthropic 強化安全分類器（99%+ 阻擋該技術）後，於 7/1 全球重新上線；公告一併提出「越獄嚴重度」四準則評估框架。

> **TL;DR (EN):** Fable 5 was paused after a jailbreak (found by Amazon) triggered US export controls; Anthropic hardened its safety classifiers (blocking the technique >99% of the time) and redeployed globally on July 1. The post also proposes a 4-criteria framework for scoring jailbreak severity.

---

## 事件背景 — What Happened

- **6/12**：Amazon 研究員找到繞過 Fable 5 安全防護的方法，能誘導模型指認軟體漏洞，某些情況下**產出示範如何利用該漏洞的程式碼**。
- 美國政府隨即發布**出口管制**，要求限制外國國民存取；因無法即時驗證國籍，Anthropic **暫停所有用戶**。
- **6/30**：限制解除；**7/1** Fable 5 全球恢復。

> [!NOTE]
> 測試顯示這**不是 Fable 5 獨有的能力**——Opus 4.8、GPT-5.5、Kimi K2.7，乃至 Haiku 4.5 等較弱模型都能指出同樣漏洞並產出相同利用示範。

---

## 技術重點 — The Technical Core

### 1. 分層防禦（Defense in Depth）

三層疊加，不靠單點：

1. 訓練模型**拒絕**危險請求
2. 事後**分析濫用模式**
3. 互動當下用**分類器（classifier）** 偵測潛在有害的資安任務

### 2. 分類器的安全邊界取捨

> [!WARNING]
> 官方明講的取捨：「為確保所有有害請求都被擋，分類器被設定成**連一些我們知道可能是良性的請求也一起觸發**」——即刻意容忍**假陽性（誤阻合法請求）** 來換更高的攔截率。

### 3. 改進措施

針對 Amazon 報告的**特定行為**訓練了改良分類器，在 **99% 以上**的情況下阻擋該技術；並加派一倍人力、建立 **24/7 監控**關鍵漏洞提交通道的專責團隊。

### 4. Mythos 5 vs Fable 5

| 模型 | 資安進攻能力 | 供應範圍 |
|---|---|---|
| **Mythos 5** | 最強，「比任何其他模型更有效發現與利用軟體漏洞」 | 僅 Project Glasswing 可信夥伴 |
| **Fable 5** | 「未提供此類獨特進攻能力」 | 全球，採最強安全防護 |

---

## 越獄嚴重度四準則 — Jailbreak Severity Framework

Anthropic 與 Amazon、Microsoft、Google 共提，用來評估一個越獄有多嚴重（有持久參考價值）：

1. **能力收益**：越獄比現有工具多給了多少能力
2. **能力廣度**：同一技術能套用到多少種不同攻擊任務
3. **武器化難度**：轉成真實攻擊要多少人力成本
4. **易發現性**：這個技術本身多容易被取得

---

## 部署時間表 — Rollout

- **7/1**：Claude Platform、Claude.ai、Claude Code、Claude Cowork 全球可用
- **7/1–7/7**：Pro/Max/Team/部分 Enterprise 給 50% weekly usage 免費試用
- **7/7 後**：改用 usage credits 購買
- **AWS / Google Cloud / Microsoft Foundry**：重啟時間待定

---

## 我的重點 — Takeaways

- **安全是「機率性攔截 + 刻意假陽性」**：分類器寧可誤擋良性請求，也要壓低漏網率——這解釋了為什麼有時正當的資安/生科任務會被擋。
- **危險能力非某模型獨有**：多個模型都能做同樣的事，凸顯「防護在 harness 層」比「賭模型不會」更務實。
- **四準則框架可自用**：評估任何越獄/濫用風險時，這四題（收益／廣度／武器化難度／易發現性）是好用的檢核表。

## 相關筆記 — Related

- [模型費用與效果比較](../01-fundamentals/model-cost-comparison.md) — Fable 5 在模型陣容中的定位與定價
- [Claude Code 權限模式與設定層級](../01-fundamentals/claude-code/permissions.md) — 「防護在外層」的另一個體現

## 來源 — Sources

- 原文：[Redeploying Fable 5 — Anthropic](https://www.anthropic.com/news/redeploying-fable-5)
