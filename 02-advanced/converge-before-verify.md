---
title: AI 101 - 先收整再展開, 人才驗得動
tags: [ai, prompt-engineering, verification, human-in-the-loop, markdown-list, skeleton-of-thought, 進階]
created: 2026-09-24
---

# 先收整再展開, 人才驗得動 — Digest First, Verify Faster

[← 回主頁](../index.md)

> [!NOTE]
> AI 生成很快但**驗證很慢**, 特別是自由散文, 人要逐字讀又不知道哪裡錯. 這篇拆兩種手法, 目的都一樣: **讓「人」的驗證變便宜**.
>
> - **手法一「濃縮再驗」**: 一堆原始資料先讓 AI 收整成 markdown list, 人 eyeball 快篩
> - **手法二「從收整衍生」**: 從一個已經驗過的 list / 大綱出發, 讓 AI 展開成長篇, 用 list 當 checklist 對照
>
> 方向相反 (壓縮 vs 展開), 但**中間停在同一種形式 — human-readable 的 markdown list**. 這是核心, 因為驗的人是人.

> **TL;DR (EN):** LLMs generate fast but verifying free-form output is slow because humans have to read every word without knowing where errors hide. Two moves collapse verification cost by keeping a compact, human-readable middle layer (usually a markdown list): **(1) Digest-then-verify** — condense raw inputs (transcripts, logs, papers) into a bulleted list so a human can eyeball-check against the source; **(2) Expand-from-digest** — start from an already-verified outline, let the model expand each bullet into prose, then check the prose against the outline as a checklist. Same middle form, opposite directions, one shared purpose: verify at the compact scale before you commit to the expensive scale. Chaining both makes a natural pipeline (raw → digest → verify → expand → check). Bonus tier when applicable: machine-verifiable middle layers (structured JSON, executable code, TDD tests) skip the human step, but the human-eyeball moves above are the base case that always works.

---

## 為什麼「事後全文驗」貴 — Why Reading the Whole Output Is Expensive

當 LLM 產出是自由散文, 驗證瓶頸就是**人的注意力**:

- **對照面積大**: 一句錯藏在千字裡, 找起來像大海撈針
- **partial credit 難算**: 對 8 成錯 2 成, 你不知道哪 2 成錯、能不能救
- **重跑就從頭來**: 錯了通常整篇重生, 前面對的部分沒被利用

換一種做法: **不要在最大尺度上驗**. 把驗證挪到一個小得多、好讀得多的中間層.

---

## 手法一: 濃縮再驗 — Digest Then Verify (壓縮方向)

**輸入端 preprocessing**. 你手上有一大堆原始資料, 直接看眼睛會爆. 讓 AI 先把它濃縮成一個 markdown list, 你 verify **那個 list**.

### 例子

| 情境 | 原始 | AI 濃縮成 | 你驗什麼 |
|:---|:---|:---|:---|
| 開完一個會 | 60 分鐘逐字稿 | 5 條 decisions + 3 條 action items 的 bullet list | 每條 decision 對不對, 有沒有漏 |
| 讀 30 篇論文 | 30 篇 abstract | 每篇 1 句摘要 + 3 個 tag 的表格 | 挑要細讀的那 3 篇 |
| 分析一天的 log | 幾 MB 的 log | 錯誤時間軸, 每個 error 一行 | 有沒有異常模式 |
| Feedback 分類 | 500 則 user feedback | 5 大類 + 每類代表訊息 3 條 | 分類正不正確, 有沒有大類漏 |

### 為什麼好驗

- **人一眼能掃完**: 5 到 20 條 bullet 比 5000 字快 100 倍
- **錯的能定位**: 覺得第 3 條不對, 就回原始資料 grep 第 3 條講的東西, 秒對照
- **AI 在做人的預處理**: 這步 AI 是**你的實習生**, 幫你把該讀的濃縮好, 你只驗結論

### 怎麼寫 prompt

給明確的產出格式 (「用 markdown bullet, 每點 15 字以內, 動詞開頭」) 加來源要求 (「每條後面用 `[來源: 段落編號]` 標」). 前者讓你好讀, 後者讓你**能追**回原文.

---

## 手法二: 從收整衍生 — Expand From a Verified Digest (展開方向)

**輸出端 preprocessing**. 你要 AI 寫一篇長文, 但直接叫它寫, 驗證會爆. 分兩步:

1. 先讓它 (或你自己) 產一個 markdown list / outline
2. **驗這個 list 對不對** (半分鐘)
3. 驗過的 list 才拿去讓 AI 展開成長文
4. 展開後**用 list 當 checklist 對照長文**, 檢查有沒有漏、有沒有無中生有

### 例子

| 情境 | 收整的 list | AI 展開成 | 你驗什麼 |
|:---|:---|:---|:---|
| 寫季度報告 | 8 條重點 bullet | 千字報告 | 8 條都有寫到, 順序對, 沒憑空多第 9 條 |
| 寫 API 文件 | endpoints + params 表 | 完整技術文件 | 表格上的每一個 endpoint 都有段落, 參數描述沒失真 |
| 寫產品說明 | 3 個賣點 + 2 個限制 | marketing 文案 | 3 個賣點都有, 2 個限制沒被漂白掉 |
| 寫技術教學 | 步驟 1 到 7 | 完整教學文 | 每步都到位, 沒跳過, 順序沒亂 |

### 為什麼好驗

- **驗大綱比驗長文快**: list 是規格, 對就對錯就錯
- **展開後能對照回**: 長文的每一段都能標到 list 的哪一條, 沒對到的就是 hallucination
- **錯了只補一段**: 第 5 條展開錯, 只重跑第 5 條, 前 4 條後 3 條不動

### 誰在講這件事

這個手法有學術名字: **Skeleton-of-Thought** (Ning et al., ICLR 2024). 原本論文動機是**加速** (skeleton 產完各 bullet 平行展開), 但副作用剛好就是易驗證. 近親: **Plan-and-Solve** (Wang et al., 2023), 一樣是「先規劃再執行」的形狀.

---

## 兩者共同機制 — Same Middle Layer, Opposite Directions

| | 手法一 濃縮再驗 | 手法二 從收整衍生 |
|:---|:---|:---|
| 方向 | 大 → 小 | 小 → 大 |
| AI 在做 | preprocessing 給人看 | 從人 approved 的 spec 展開 |
| 中間層 | markdown list | markdown list |
| 誰在驗、驗什麼 | 人看 list vs 原始資料 | 人看長文 vs list |
| 錯了怎麼救 | 挑錯的資料點重跑 | 挑漏掉的段落重補 |

**共同原理**: 都在一個 human-readable 的**收整層**停住, 讓驗證發生在這一層, 不是原始資料或最終長文的那個大尺度.

**為什麼用 markdown list**: 對人夠好讀 (bullet 直接掃), 對 AI 夠結構化 (穩定產出、可 diff、可 count). 兩邊都吃, 是最省事的中間層.

---

## 兩個接起來就是一個 pipeline — Chain Them

最強的用法是**把兩個接起來**:

```
原始資料 (大)
    │  手法一: AI 濃縮
    ▼
markdown list (小)  ← 你在這裡驗第一次 (半分鐘)
    │  手法二: AI 展開
    ▼
長文 (大)  ← 你在這裡驗第二次 (兩分鐘, 拿 list 當 checklist)
```

省掉的是「全文精讀」的半小時. 驗兩次小的比驗一次大的快很多, 而且中間可攔截.

實例: 30 篇論文 → 每篇 1 句摘要 (你驗) → 精選 5 篇擴寫成完整讀書會分享 (你用摘要當 checklist 驗). 從 30 篇到成稿, 全程沒精讀過原文, 但你**確認過每一個中間輸出**.

---

## 什麼時候別用 — When Not To

- **資料量小 (500 字內)**: 濃縮的 overhead 比直接讀還高
- **需要意外性、創意、發散**: 收整會扼殺, 例如寫詩、發想 side project idea
- **需求極度模糊**: 你都不知道要什麼, 硬定 list 只是把「想清楚需求」的成本推給你 (但這也可能是好事)
- **一次性 throwaway**: 建 list 的思考成本可能比多驗兩次還高

**大原則**: 交付、audit、給別人看的東西值得先收整; 自己隨手用、探索、發散的內容值得留自由度.

---

## 進階: 讓機器代替人驗 — Bonus: Machine-Verifiable Middle Layers

如果收整層可以升級成**機器可驗**的形式, 連人都可以省掉:

- **JSON schema / structured output** (Outlines、OpenAI Structured Outputs、Claude tool use): 每欄可 assert
- **Program-of-Thought / PAL** (Chen 2022, Gao 2023): 生成 Python 讓 interpreter 跑, 錯了 exception, 對了跑出數字. GSM8K/MATH 平均比 CoT 高 12%
- **TDD 的 AI 版** (見 [Day 09 Loop Engineering](../05-notes/ironman/drafts/day09-loop-engineering.md)): 生成測試 (規格 = 收整), agent 改 code 到綠

但這是「進階版, 且要看情境能不能適用」. 上面兩個 markdown list 手法是**永遠適用的 base case** — 只要人得參與, 這條就有效. 先把 base case 練熟, 遇到適合的情境再升級.

---

## 我的重點 — Takeaways

- 事後全文驗貴, 因為**人的注意力是 bottleneck**, 不是模型速度
- 兩個手法都在做「preprocessing for human eyeball」, 只是方向不同: 壓縮 vs 展開
- **markdown list 是靈魂中間層** — 對人夠好讀, 對 AI 夠結構化, 兩邊都吃
- 兩個手法能接起來成 pipeline (原始 → list → 驗 → 長文 → 對照驗), 全程沒精讀過原文但**每一步都驗過**
- 遇到情境允許時, 可以把收整層升級成 machine-verifiable, 但那是 bonus, 不是預設

---

## 相關筆記 — Related

- [Context Engineering 進階](./context-engineering-in-depth.md), 為什麼結構化的中間層讓後續每一步都更準
- [Prompt 到 Loop 五個時代](./prompt-engineering-evolution.md), 「先收整再展開」的思路在多個時代都出現
- [Day 06 Prompt Engineering](../05-notes/ironman/drafts/day06-prompt-engineering.md), 通用四件事範本裡的 `<format>` 就是要求收整
- [Day 09 Loop Engineering](../05-notes/ironman/drafts/day09-loop-engineering.md), TDD 版是「機器代替人驗」的極端形式

---

## Sources

- [Skeleton-of-Thought: LLMs Can Do Parallel Decoding — Ning et al., ICLR 2024](https://arxiv.org/pdf/2307.15337)
- [Plan-and-Solve Prompting — Wang et al., 2023](https://arxiv.org/abs/2305.04091)
- [Program of Thoughts Prompting — Chen et al., 2022](https://arxiv.org/pdf/2211.12588)
- [PAL: Program-aided Language Models — Gao et al., 2023](https://arxiv.org/abs/2211.10435)
- [Chain-of-Verification Reduces Hallucination in LLMs — Meta, 2023](https://arxiv.org/pdf/2309.11495) (相關: 事後補驗的一種做法)
- [LLM Structured Output in 2026 — DevToolLab](https://devtoollab.com/blog/llm-structured-outputs-guide-2026) (進階段落用)
- [Outlines: constrained decoding library](https://github.com/dottxt-ai/outlines)
