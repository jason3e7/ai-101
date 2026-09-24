---
title: AI 101 - 先收斂再生成, 驗證才便宜
tags: [ai, prompt-engineering, verification, structured-output, skeleton-of-thought, chain-of-verification, program-of-thought, 進階]
created: 2026-09-24
---

# 先收斂再生成, 驗證才便宜 — Converge Before You Verify

[← 回主頁](../index.md)

> [!NOTE]
> 大部分人的 AI 使用流程是「生成 → 事後驗證」, 生成完丟給人眼或另一顆 LLM 檢查. 這樣驗**貴**, 而且錯了要重跑一遍. 有一條反過來的路: **讓生成本身就長成好驗的形狀**. 這篇拆兩種手法 — 生成前先把資料收斂成結構化中間形式, 或是從已經批准的大綱開始逐項展開. 兩者共同原理是「縮 solution space + 隔離錯誤」, 都有現成技術可以套.

> **TL;DR (EN):** Most people run "generate → verify post-hoc" with LLMs; verifying free-form prose is expensive because the check surface is huge, partial credit is unclear, and errors force full re-runs. Two moves shrink verification cost by shaping the output up front: (1) **Converge first** — force JSON/schema/tuple output so each field is independently checkable (Structured Output, prompt-level or constrained-decoding); (2) **Outline first** — produce a skeleton, verify the skeleton, then expand each bullet independently (Skeleton-of-Thought, Plan-and-Solve). Both narrow the solution space and localize errors. Extreme form: generate executable artifacts (code, tests, SQL) so execution itself is verification (PoT/PAL, TDD-with-agents). Reactive fallback when you can't reshape: Chain-of-Verification (Meta 2023). Rule of thumb: creative/one-off → keep freedom; repeatable/audit-heavy → converge.

---

## 為什麼「事後驗」貴 — Why Post-Hoc Verification Is Expensive

當 LLM 產出是自由散文, 驗證就要人 (或另一顆 LLM) 讀完全文再判斷. 三個成本被藏起來:

- **對照面積大**: 一句錯藏在千字裡, 找起來像大海撈針
- **partial credit 難算**: 對 8 成錯 2 成, 你不知道哪 2 成錯、能不能救
- **重跑就從頭來**: 錯了通常整篇重生, 前面對的部分沒被利用

換一種做法: **在生成階段就把輸出形狀限住**, 讓驗證變成小面積、可 diff、可 partial check.

---

## 手法一: 收斂再生成 — Structured Output First

先讓模型把答案輸出成**可 parse 的結構** (JSON / 表格 / tuple), 再拿這個結構去驗.

例: 「幫我從這份會議記錄抽出行動項」

自由散文版:
> 會後將由小明處理登入頁面的 bug, 期限是週五. 另外, 小華負責更新文件, 沒有明確期限, 但希望下週一之前完成...

結構版 (指定 schema):

```json
[
  {"assignee": "小明", "task": "修登入頁面 bug", "due": "2026-09-26"},
  {"assignee": "小華", "task": "更新文件",       "due": "2026-09-29"}
]
```

**為什麼好驗**:

- 每一欄可以獨立 assert (assignee 是不是有效員工? due 是不是合法日期?)
- 缺欄 / 多欄立刻爆
- 可以跟原文 diff 逐項對照

**兩個技術路線**:

| 層級 | 做法 | 保證強度 |
|:---|:---|:---|
| **prompt-level 收斂** | 在 prompt 裡寫 JSON schema 範例, 靠模型自律 | ~99%, 偶爾漏欄或格式歪 |
| **decode-level 收斂** | Constrained decoding (Outlines、LM Format Enforcer、OpenAI Structured Outputs、Claude tool use), 掩掉不合 schema 的 token | 100% parseable, 有 constraint tax (可能壓縮到少量推理空間) |

要 audit / 進 pipeline 就上 decode-level; 一次性人看用 prompt-level 就夠.

---

## 手法二: 從大綱衍生 — Outline First (Skeleton-of-Thought)

不是輸出一次寫完, **分兩步**:

1. **先產大綱** (skeleton): 5 到 10 個 bullet, 每個 5 到 10 字
2. **對每個 bullet 獨立展開**: 可以平行跑

例: 一份技術 review 報告

- **Step 1 產大綱**: 「1. 背景 / 2. 現況痛點 / 3. 修法 A 的優劣 / 4. 修法 B 的優劣 / 5. 推薦 / 6. 風險」
- **Step 2 對每個標題各叫模型寫一段** (可以平行 6 個 API call)

**為什麼好驗**:

- 大綱本身**短且結構化**, 你可以**先驗大綱對不對再展開** (省重跑)
- 每段獨立展開, 錯一段只重跑那一段
- 大綱是自然的**規格**, 展開的內容跟大綱對照就能檢查有沒有跑題

這個技巧有正式名字: **Skeleton-of-Thought** (Ning et al., ICLR 2024). 原本是為了加速 (parallel decoding), 但副作用剛好是**易驗證**. 近親: **Plan-and-Solve** (Wang et al., 2023) — 先生 plan 再 solve, 同一種形狀.

---

## 為什麼兩種都在降驗證成本 — The Common Mechanism

兩種手法, 同一個結構:

| | 手法一 (收斂再生成) | 手法二 (從大綱衍生) |
|:---|:---|:---|
| 收斂的東西 | **輸入 → 結構** | **想法 → 大綱** |
| 中間形式 | JSON / table / tuple | bullet skeleton |
| 驗證發生在 | 結構化資料上 (per field) | 大綱 + 每段展開 (per section) |
| 錯了怎麼救 | 只補錯的 field | 只重跑錯的 section |

共同原理: **把「一次生成一大團」拆成「先收斂到一個好驗的中間層, 再由這個中間層驅動下一步」**. 中間層越窄、越結構化, 驗證面積越小, 錯了越好隔離.

---

## 進一步: 讓執行本身就是驗證 — When Generation Is Verification

延伸: 如果生成的東西**天生可執行**, 執行結果就是驗證.

- **Program-of-Thought / PAL** (Chen 2022, Gao 2023): 讓模型產 Python code 而不是文字算式, 給 interpreter 跑. 錯了直接 exception, 對了跑出數字. PoT 在 GSM8K / MATH 上**平均比 CoT 高 12%**
- **TDD 的 AI 版** (見 [Day 09 Loop Engineering](../05-notes/ironman/drafts/day09-loop-engineering.md)): 先寫測試 (規格 = 收斂形式), 讓 agent 改 code 到綠. 測試綠 = 驗證通過
- **SQL / API call 生成**: 產可執行 query, 執行成功且結果符合預期 = 驗證通過

這一類是「收斂再生成」的極端形式: 中間結構 (code / test / query) 本身就是可執行的驗證器.

---

## Chain-of-Verification: 反向的補丁 — CoVe as a Reactive Fallback

如果來不及在生成前收斂 (例如上游 API 只吐自由文本), 還有一個補救: **Chain-of-Verification** (Meta, 2023).

四步:

1. 生成 baseline 回答
2. 讓模型自己生一組 verification 問題來檢查自己
3. 逐題**獨立**回答 (獨立這一步很重要, 避免它偷看第 1 步)
4. 用檢查結果修正原答案

CoVe 屬於「生成後再結構化驗」, 效果比純事後 review 好, 但**還是輸給一開始就結構化生成**. 適合場景: 你只能拿到自由文本輸出、無法改 prompt 讓它結構化時.

---

## 什麼時候別用 — When Not To

- **創意寫作、開放式對話**: 硬套 schema 會扼殺流暢度
- **需求本身就模糊**: 你都不知道要什麼欄位, 硬定 schema 只是把「想清楚需求」的成本推給你 (但這也可能是件好事)
- **一次性任務**: 建 schema 的 overhead 可能比多驗兩次還高

**大原則**: 反覆做、要交付、需要 audit 的任務值得收斂; 一次性、探索性、給人看的內容值得留自由度.

---

## 我的重點 — Takeaways

- 「事後驗」是預設但不是最省, **在生成前縮 solution space 才是** 
- 兩條路: 手法一收斂**資料** (schema / tuple / JSON), 手法二收斂**結構** (skeleton / plan)
- 有現成技術可套: JSON schema、Constrained decoding、Skeleton-of-Thought、Plan-and-Solve、PoT/PAL、CoVe
- 極端形式: 讓生成的東西**本身可執行**, 執行結果就是驗證 (TDD、PAL、SQL)
- 判斷: **一次性/創意留自由; 反覆/交付/audit 用結構**

---

## 相關筆記 — Related

- [Context Engineering 進階](./context-engineering-in-depth.md), 結構化 context 讓模型更準的機制解釋
- [Prompt 到 Loop 五個時代](./prompt-engineering-evolution.md), 這篇的手法貫穿多代 (Prompt 時代的 SoT / Context 時代的 tool schema / Loop 時代的 goal-with-verifier)
- [Day 06 Prompt Engineering](../05-notes/ironman/drafts/day06-prompt-engineering.md), 通用四件事範本裡的 `<format>` 就是「收斂 output」
- [Day 09 Loop Engineering](../05-notes/ironman/drafts/day09-loop-engineering.md), TDD 版就是「收斂 + 執行 = 驗證」的極端

---

## Sources

- [Skeleton-of-Thought: LLMs Can Do Parallel Decoding — Ning et al., ICLR 2024](https://arxiv.org/pdf/2307.15337)
- [Chain-of-Verification Reduces Hallucination in Large Language Models — Meta, 2023](https://arxiv.org/pdf/2309.11495)
- [Program of Thoughts Prompting: Disentangling Computation from Reasoning — Chen et al., 2022](https://arxiv.org/pdf/2211.12588)
- [PAL: Program-aided Language Models — Gao et al., 2023](https://arxiv.org/abs/2211.10435)
- [Plan-and-Solve Prompting — Wang et al., 2023](https://arxiv.org/abs/2305.04091)
- [Structured Output Generation in LLMs: JSON Schema and Grammar-Based Decoding — Medium](https://medium.com/@emrekaratas-ai/structured-output-generation-in-llms-json-schema-and-grammar-based-decoding-6a5c58b698a6)
- [LLM Structured Output in 2026 — DevToolLab](https://devtoollab.com/blog/llm-structured-outputs-guide-2026)
- [Outlines: constrained decoding library](https://github.com/dottxt-ai/outlines)
