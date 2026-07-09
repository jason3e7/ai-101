---
title: 用 Prompt 生成好 Prompt（Meta-Prompting）
tags: [ai, 個人筆記, prompt, meta-prompting, dspy, ape, 草稿]
created: 2026-07-08
status: draft
---

# 用 Prompt 生成好 Prompt（Meta-Prompting）— Prompting to Generate Better Prompts

[← 回主頁](../index.md)

> [!NOTE]
> 草稿 · 研究底稿。整理「怎麼用一個 prompt 去產生／改進另一個 prompt」的方法與可複製範本，等 jason3e7 加入自己的想法後定稿。

> **TL;DR (EN):** Meta-prompting = using an LLM to write or improve prompts. Three tiers: (1) hand-written meta-prompts you paste in, (2) official prompt generators (Anthropic Console, OpenAI Playground), (3) automated optimizers (APE, DSPy) that search prompt space against an eval set. Copy-paste templates below; use tier 3 only once you have 15+ prompts and a real eval harness.

---

## 是什麼 — What It Is

**Meta-prompt = 「產生指令的指令」**——用一段高階 prompt，讓 LLM 幫你寫出／改好真正要用的 prompt。

為什麼有效：模型產出的 prompt 通常**比人手寫的更結構化**，會主動把「模糊的指示」換成「具體、明確的要求」。Anthropic 和 OpenAI 都提供官方工具，可見這招夠實用。

---

## 好 prompt 的特性 — What Makes a Prompt Good

> [!IMPORTANT]
> Anthropic 官方的第一課不是技巧，而是前提：**先有「成功標準」和「能測試的方法」，再談 prompt。** 沒有「怎樣算好」的定義，就無從判斷一個 prompt 到底好不好——這也是為什麼自動優化要有 eval（見下方）。

一個好 prompt 通常同時具備這些特性，可當檢查表用：

| 特性 | 說明 | 壞 → 好 |
|---|---|---|
| **明確無歧義** | 只有一種解讀 | 「幫我看看這個」→「指出這段程式碼的 3 個效能瓶頸」|
| **具體可驗證** | 限制能被檢查，不用模糊詞 | 「詳細一點」→「每點至少含一個程式碼範例」|
| **有脈絡** | 給角色、受眾、背景 | 「解釋 API」→「向沒寫過程式的 PM 解釋這個 REST API」|
| **指定輸出格式** | 格式、長度、語氣 | 「列一下」→「markdown 表格，≤ 5 列，每格 ≤ 20 字」|
| **有邊界** | 說明不要做什麼、缺資料怎麼辦 | （無）→「原文沒提到就別臆測，標記『未知』」|
| **有示範**（需要時） | few-shot 範例，格式類任務特別有效 | （無）→ 附 1–2 個輸入／輸出範例 |
| **給思考空間**（難任務） | 要求先推理再結論 | 「答案是？」→「先列步驟，再給結論」|
| **對齊成功標準** | 你知道怎樣算好、而且測得出來 | 憑感覺 → 有可檢查的驗收條件 |

### 最小骨架 — The Minimal Skeleton

多數指南同意一個 prompt 的四個核心元件：**指令（instruction）＋脈絡（context）＋輸入資料（data）＋輸出格式（output format）**。把這四塊分清楚、用分隔符或 XML 標籤隔開，就已經勝過大多數隨手寫的 prompt。

> [!NOTE]
> **一句話總結：好 prompt = 把「模糊」換成「具體可驗證」，並對齊一個你測得出來的成功標準。**
> 其餘技巧（XML 標籤分區、system prompt 設角色、few-shot 示範、prefill、prompt chaining、chain-of-thought）都只是**達成上面特性的手段**，不是目的。

---

## 三個層次 — Three Tiers

| 層次 | 做法 | 適合 |
|---|---|---|
| **1. 手寫 meta-prompt** | 自己貼一段「請你當 prompt 工程師，幫我把 X 寫成好 prompt」 | 隨手、單次、少量 |
| **2. 官方生成器** | [Anthropic Console 的 Prompt Generator](https://docs.anthropic.com/en/docs/build-with-claude/prompt-engineering/prompt-generator)、OpenAI Playground 的 system instructions generator | 要針對特定模型調校 |
| **3. 自動化優化器** | APE、DSPy——對著**評估資料集**搜尋最佳 prompt | 15+ prompts + 有 eval harness 的生產環境 |

> [!TIP]
> 2026 年的實務通則：**維護少於 15 個 prompt → 手工**；**15 個以上且有評估資料 → 用優化器**，工程師只負責準備資料與指標。

---

## 可直接用的 Meta-Prompt 範本 — Copy-Paste Templates

### 範本 A：一句話 → 完整結構化 prompt

```
你是資深 prompt 工程師。我會給你一個模糊的需求，請把它改寫成一個高品質 prompt。

我的需求：<在此寫一句話，例如「幫我摘要會議記錄」>

請輸出一個 prompt，包含：
1. 角色設定（要 AI 扮演誰）
2. 明確任務與步驟
3. 輸出格式與長度限制
4. 邊界條件（不要做什麼、遇到缺資料怎麼辦）
把所有模糊詞換成具體、可驗證的要求。只輸出改好的 prompt，不要解釋。
```

### 範本 B：先訪談再產出（品質最高）

```
我想要一個關於「<主題>」的 prompt，但還沒想清楚。
請不要馬上寫，先問我 5 個最關鍵的釐清問題（受眾、目標、格式、限制、成功標準）。
等我回答後，再產出最終 prompt。
```

> [!TIP]
> 範本 B 通常品質最好——因為它逼你（和模型）先把「問題本身」界定清楚，而不是急著生成。這正是 [四種能力執行手冊](../02-advanced/four-capabilities-playbook.md) 裡「給標準」的體現。

### 範本 C：反思式改寫（generate → critique → improve）

```
這是我目前的 prompt：
---
<貼上現有 prompt>
---
請分三步：
1. 指出這個 prompt 的 3 個具體弱點（模糊、缺限制、易誤解之處）
2. 說明每個弱點會導致什麼壞輸出
3. 輸出改進版

改進版要保留原意，只補強明確度與可控性。
```

---

## 背後的技法 — The Techniques

- **Reflection（反思）**：讓模型先**批判自己的分析**，再定稿改進版（範本 C 就是）
- **Prompt gradients（文字梯度）**：針對每個失敗案例，產生「往哪個方向改」的具體建議，像梯度下降
- **Few-shot bootstrapping**：讓模型自己生成好的示範例，塞回 prompt

---

## 進階：自動化優化 — Automated Optimization

當 prompt 多到手工顧不來，改用「對評估集搜尋」的優化器：

- **APE（Automatic Prompt Engineer）**：把「找指令」當成黑箱優化，生成大量候選再挑最好——曾自動找出「Let's think step by step to be sure we have the right answer」，比人手寫的 CoT 好 3%
- **DSPy（Stanford, Omar Khattab）**：**不寫 prompt，寫程式**，讓優化器（BootstrapFewShot、MIPROv2、GEPA…）對著 eval 資料集找出勝過手寫的 prompt；2026 的生產標準

> [!WARNING]
> 自動化優化**需要評估資料與指標**才有意義——沒有 eval，優化器無從判斷「哪個 prompt 比較好」。沒到那個規模就別急著上 DSPy，手寫 meta-prompt 更快。

---

## 常見陷阱 — Pitfalls

- **沒有評估就談優化**：手感不是指標；要能量測才談自動優化
- **一次到位的迷思**：好 prompt 幾乎都是「生成 → 測 → 改」迭代出來的，範本 C 的反思循環要反覆跑
- **過度結構化**：meta-prompt 產出的 prompt 有時過長過細，對簡單任務反而累贅——用完要人工瘦身
- **跨模型不通用**：為 Claude 優化的 prompt 不保證在別的模型一樣好，換模型要重測

---

## jason3e7 的想法（待補）— My Take (TODO)

> 這一節留給 jason3e7。可以想的角度：
> - 你自己最常用哪一種（手寫 meta-prompt？官方生成器？）？效果如何？
> - 範本 A/B/C 哪個對你最有用？有沒有你自己的第四種？
> - 「先訪談再產出」是不是你偏好的模式？

---

## Sources

- [Prompt engineering overview — Anthropic 官方（先有成功標準與 eval）](https://platform.claude.com/docs/en/build-with-claude/prompt-engineering/overview)
- [General Tips for Designing Prompts — Prompt Engineering Guide](https://www.promptingguide.ai/introduction/tips)
- [Understanding Prompt Structure: Key Parts of a Prompt — Learn Prompting](https://learnprompting.org/docs/basics/prompt_structure)
- [A Complete Guide to Meta Prompting — PromptHub](https://www.prompthub.us/blog/a-complete-guide-to-meta-prompting)
- [Meta-Prompting: LLMs Crafting Their Own Prompts — IntuitionLabs](https://intuitionlabs.ai/articles/meta-prompting-llm-self-optimization)
- [Exploring Prompt Optimization（reflection、text gradients）— LangChain](https://www.langchain.com/blog/exploring-prompt-optimization)
- [Anthropic Prompt Generator — 官方文件](https://docs.anthropic.com/en/docs/build-with-claude/prompt-engineering/prompt-generator)
- [Automatic Prompt Engineer (APE) — Prompt Engineering Guide](https://www.promptingguide.ai/techniques/ape)
- [Systematic LLM Prompt Engineering Using DSPy — Towards Data Science](https://towardsdatascience.com/systematic-llm-prompt-engineering-using-dspy-optimization/)
- [Prompt Optimization with DSPy — Haystack](https://haystack.deepset.ai/cookbook/prompt_optimization_with_dspy)
