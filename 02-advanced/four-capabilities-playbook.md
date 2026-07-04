---
title: AI 101 - 四種能力執行手冊：省 token vs 最有成效
tags: [ai, 能力地圖, token優化, 模型選型, prompt, cost, 進階]
created: 2026-07-05
---

# 四種能力執行手冊：省 token vs 最有成效 — Four Capabilities: Cheapest vs Best Execution

[← 回主頁](../index.md)

> [!NOTE]
> 承接 [AI 能力全景圖](./ai-capability-landscape.md)，這篇把四種高頻能力——**Summarization（摘要）、Explanation（解釋）、Ideation（發想）、Refactor Note（重構筆記）**——各給兩套打法：**① 最省 token**、**② 最有成效**。每種都附模型選型表與可直接複製的 prompt / 工作流程。

> **TL;DR (EN):** For four common capabilities (summarize / explain / ideate / refactor a note), this gives two execution modes each — cheapest-token and highest-quality — with a model-selection table and copy-paste prompts. Core rules: match the model to task difficulty, cap output aggressively for cheap mode, and add thinking/diversity only where the task's cognitive depth needs it.

---

## 先定位：四種能力在全景圖的哪裡 — Where They Sit

| 能力 | 資訊流向 | 認知深度 | 忠實度要求 |
|---|---|---|---|
| **Summarization 摘要** | 收斂（多→少） | 低～中階 | 高（怕亂加） |
| **Explanation 解釋** | 轉換（等量換形式） | 高階 | 高（須忠於原意） |
| **Ideation 發想** | 發散（少→多） | 高階 | 低（要它亂想） |
| **Refactor Note 重構筆記** | 轉換為主 + 少量收斂 | 高階 | **最高**（不可漏內容） |

> [!IMPORTANT]
> 定位決定打法。**忠實度高**的三個（摘要/解釋/重構）→ 低 temperature、要驗證；**忠實度低**的發想 → 高 temperature、不必查證。**認知深度高**的（解釋/發想/重構）→ 值得給思考空間；低階的摘要 → 不必。

---

## 兩條通用槓桿 — The Two Universal Levers

執行任何能力，「省 token」和「拉成效」各有一組固定手段，先記這張總表，後面四節只是套用。

### 省 token 的四個槓桿（有數據）

| 槓桿 | 做法 | 省多少 |
|---|---|---|
| **降模型** | 用「能做好這件事的最便宜模型」，別預設 Opus | Haiku vs Opus 輸入省 80% |
| **限輸出** | `max_tokens` + 要求精簡格式（bullet / JSON） | 輸出省 30–50% |
| **Chain of Draft（CoD）** | 要模型「用最少字做中間推理」，取代冗長 CoT | 推理 token 省 68–92%，準確度相當 |
| **Batch / 快取** | 量大用 Batch API（約 5 折）；同一份長文重複問就開 prompt caching | Batch 省 ~50%；快取按命中率 |

### 拉成效的四個槓桿

| 槓桿 | 做法 | 適用 |
|---|---|---|
| **升模型** | 難的任務換 Sonnet→Opus→Fable | 高階任務 |
| **給思考空間** | 開 extended thinking / 要求先列思路再產出 | 解釋、重構、發想結構 |
| **給標準** | 明講受眾、格式、取捨準則、風格指南 | 全部 |
| **多樣化**（發散專用） | 高 temperature、強制不同角度、多次獨立呼叫 | 發想 |

> [!WARNING]
> **發想有個反直覺陷阱**：研究發現用 LLM 發想會**收窄**想法多樣性——一項實驗中 94% 的點子共享同一核心概念。所以「最有成效的發想」不是把 prompt 寫更細，而是**刻意製造分歧**（見下方 Ideation 節）。

---

## 模型選型總表 — Model Selection Cheat Sheet

價格為每 1M token（輸入/輸出）。詳見 [模型費用與效果比較](../01-fundamentals/model-cost-comparison.md)。

| 能力 | ①最省 token 選 | ②最有成效選 | 理由 |
|---|---|---|---|
| **Summarization** | **Haiku 4.5**（$1/$5） | **Sonnet 4.6**（$3/$15），密集學術材料才上 Opus | 摘要是高量低階任務，Haiku 官方點名適用 |
| **Explanation** | **Sonnet 4.6** | **Opus 4.8**（$5/$25） | 解釋要重組+類比，吃推理；便宜端 Sonnet 已夠 |
| **Ideation** | **Sonnet 4.6**（高 temp） | **Opus 4.8 / Fable 5**（$10/$50），多次獨立呼叫 | 發想品質看模型天花板與多樣性 |
| **Refactor Note** | **Sonnet 4.6** | **Opus 4.8** | 結構判斷是高階，且不可漏內容，可靠度優先 |

> [!TIP]
> 通則（Anthropic 2026 建議）：**Sonnet 4.6 當預設**，量大又簡單就掉到 **Haiku 4.5**，任務真的難才升 **Opus 4.8**。Sonnet 以約 6 折成本達到 Opus 97–99% 的品質，是省錢與成效的最佳平衡點。

---

## 1. Summarization 摘要

**定位**：收斂、低～中階、忠實度高。目標是丟資訊但不丟重點、不加料。

### ① 最省 token

- 模型 **Haiku 4.5**；長文用 **map-reduce**（分段各自摘要再合併），量大走 **Batch API**
- 硬限輸出長度，指定精簡格式

```
用繁體中文，把以下內容濃縮成最多 5 條 bullet，每條 ≤ 20 字。
只保留「結論、數字、行動項」，其餘一律略去。不要前言與結語。

<在此貼上原文>
```

### ② 最有成效

- 模型 **Sonnet 4.6**（密集論文/財報才上 Opus）
- 給**取捨準則**（要為誰摘、保留什麼維度），並要求**忠於原文、不得補充**

```
你是為「趕時間的決策者」做摘要。閱讀下文後輸出：

1. 一句話總結（≤ 30 字）
2. 3–5 個關鍵重點（每點附原文依據，若原文沒說就不要寫）
3. 一個「這對讀者的意義 / 該做什麼」

規則：只用原文有的資訊，不得推測或補充外部知識。
數字與專有名詞逐字保留。

<在此貼上原文>
```

> [!TIP]
> 摘要在忠實度高的一端，**低 temperature（0–0.3）**、產出後快速核對數字。長文優先 map-reduce 而非硬塞進 context，省 token 又避免中段被忽略。

---

## 2. Explanation 解釋

**定位**：轉換、高階、忠實度高。把難的說成好懂的，但意思不能跑掉。

### ① 最省 token

- 模型 **Sonnet 4.6**；用 **Chain of Draft** 讓它精簡推理
- 綁定受眾與長度，避免它長篇大論

```
用一個高中生能懂的比喻解釋「<概念>」，限 3 句話。
先想再答，但思考過程只用關鍵詞、不要寫成完整句子。
```

### ② 最有成效

- 模型 **Opus 4.8**；開 extended thinking / 要求先建結構
- 明確指定**受眾、深度、類比、以及「先前概念」**

```
向「<受眾，例：完全沒寫過程式的產品經理>」解釋「<概念>」。

請依此結構：
1. 一句話直覺（用日常類比）
2. 為什麼需要它 / 它解決什麼問題
3. 運作方式（拆 2–3 步，每步配一個具體例子）
4. 常見誤解一則

要求：忠於技術正確性，類比不可誤導；先在心裡建好結構再寫。
```

> [!TIP]
> 解釋雖在轉換端，但認知深度高——**給思考空間**（先結構後內容）比直接要答案品質好很多。省 token 模式改用 CoD（只用關鍵詞推理）能保住品質又砍掉冗長。

---

## 3. Ideation 發想

**定位**：發散、高階、忠實度低。要的就是多、新、不同，幻覺是 feature。

### ① 最省 token

- 模型 **Sonnet 4.6**，temperature 拉高（0.9–1.0）
- 要點子清單而非長篇論述，輸出天然就短

```
針對「<主題>」，快速給 15 個點子。
每個點子一行、≤ 15 字，只要方向不要解釋。求數量與差異，不求完整。
```

### ② 最有成效

- 模型 **Opus 4.8 / Fable 5**；**多次獨立呼叫**再彙整（對抗多樣性收窄）
- 強制不同角度 / 角色，避免它全部收斂到同一核心

```
針對「<主題>」發想，分成三批，每批用不同視角，各給 5 個點子：

批次 A：從「成本最低 / 最懶」的視角
批次 B：從「反直覺 / 故意打破常規」的視角
批次 C：從「十年後回看，現在很蠢」的視角

規則：三批之間點子不可重複核心概念；越出乎意料越好，可行性先不管。
```

> [!WARNING]
> 別把發想和其他三個一樣「寫更嚴格的 prompt」——過度約束會讓它更收斂。**製造分歧的手段**才是關鍵：高 temperature、多角色、多次獨立 session、最後再人工收斂評估。想法的「收斂評估」是另一個能力（見全景圖左上收斂區），別和發散混在同一次呼叫。

---

## 4. Refactor Note 重構筆記

**定位**：以轉換為主（重排結構、改寫）+ 少量收斂（去重）、高階、**忠實度最高**——重構最大的風險是「悄悄弄丟內容」。

### ① 最省 token

- 模型 **Sonnet 4.6**；**不要每次重送整篇再整篇重寫**
- 開 **prompt caching**（同一篇反覆調），或要求**只回傳改動的段落 / diff**

```
以下筆記結構鬆散。請只做「結構重排」：重新分節、加小標、調順序。
不要改寫任何句子的用字。
輸出格式：只列出「新的章節大綱 + 每節該放原文哪幾段」，不要重印全文。

<貼上筆記>
```

### ② 最有成效

- 模型 **Opus 4.8**；**兩段式工作流**（先診斷、後重寫），並餵**風格指南**
- 明確要求「內容零遺失」並自我檢查

```
第一步（先只做這步，等我確認）：
閱讀下面筆記，指出結構問題：重複、順序不合理、缺少的過渡、可合併的段落。
輸出一份「重構計畫」，不要動筆改。

<貼上筆記 + 你的風格指南，例如 CLAUDE.md 的寫作規範>
```

確認計畫後，第二步：

```
依剛才的重構計畫改寫。硬性要求：
- 原文每個「事實、數字、範例」都必須保留，只能搬位置或改措辭
- 改寫後附一個「內容對照檢查表」：列出原文有哪些要點、逐一標記在新版哪一節
- 符合提供的風格指南（callout 語法、標題格式等）
```

> [!IMPORTANT]
> 重構是四者中**最該用強模型 + 最該驗證**的。它同時要求「大改結構」與「零內容遺失」，這對忠實度是矛盾壓力——用**兩段式**（診斷／執行分開）+ **內容對照檢查表**把風險壓下來。省 token 版切忌讓它整篇重印，改用「大綱 + 段落對應」或 diff。

---

## 常見問題 — FAQ

**Q：為什麼摘要用 Haiku，重構卻要 Opus？兩者都是「處理既有文字」。**
差在認知深度與風險。摘要是低階濃縮，Haiku 夠；重構要判斷整體結構又不能漏內容（高階 + 最高忠實度），值得用 Opus + 驗證。

**Q：省 token 和最有成效一定衝突嗎？**
不一定。**Chain of Draft**（精簡推理）和**降模型到剛好夠用**能同時省錢又保品質；真正衝突的是「難任務硬要用便宜模型」或「簡單任務浪費 Opus」。

**Q：發想為什麼不能靠更好的 prompt 拉成效？**
因為 LLM 發想會自然收窄多樣性（94% 點子同核心）。成效來自**多樣性**，得靠高 temperature、多角度、多次獨立呼叫，而非把單次 prompt 寫更細。

---

## 相關筆記 — Related

- [AI 能力全景圖](./ai-capability-landscape.md) — 這四種能力的座標與理論依據
- [模型費用與效果比較](../01-fundamentals/model-cost-comparison.md) — 各模型定價與 benchmark
- [Context Engineering](./context-engineering.md) — 餵對資訊本身就是最大的省 token 槓桿

## Sources

- [Models overview — Claude Platform Docs](https://platform.claude.com/docs/en/about-claude/models/overview)
- [Claude Model Selection Guide 2026: Sonnet vs Opus vs Haiku — Zenken AI](https://ai.zenken.co.jp/en/post/claude-model-selection-guide/)
- [LLM Cost Optimization: 8 Strategies That Cut API Spend by 80% (2026) — PremAI](https://blog.premai.io/llm-cost-optimization-8-strategies-that-cut-api-spend-by-80-2026-guide/)
- [Prompt Compression and Cache Tuning: Cut Your LLM API Costs by 60% — SitePoint](https://www.sitepoint.com/prompt-compression-cache-tuning-llm-api-costs/)
- [Token-Efficient Prompting Patterns: Chain of Draft — Token Optimize](https://www.tokenoptimize.dev/guides/token-efficient-prompting-patterns)
- [Move Beyond Chain-of-Thought with Chain-of-Draft — AWS ML Blog](https://aws.amazon.com/blogs/machine-learning/move-beyond-chain-of-thought-with-chain-of-draft-on-amazon-bedrock/)
- [AI-Augmented Brainwriting: LLMs in group ideation — arXiv](https://arxiv.org/pdf/2402.14978)
