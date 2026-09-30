---
title: lab01 綜合訊號分數設計 — Composite AI-Writing Score
created: 2026-09-30
---

# 綜合訊號分數設計 — Composite AI-Writing Score

> [!NOTE]
> V01–V05 每個訊號各自能排序, 但單看一個容易冤枉人或漏掉. 這篇提出一個把多個訊號**加權疊加**成一個 0–N 分「AI 味濃度」的做法, 是 lab04「綜合指標」的施工圖.

> **TL;DR (EN):** Convert each signal to percentile rank across the corpus (auto-normalizes scale differences), apply a soft threshold at p75 to get a 0–1 "hit", weight-sum into a composite score. Weights encoded by prior belief about tell strength (`——` weight 3, emoji weight 2, layout signals 1–1.5). Score is a **ranking tool + explanation**, not a verdict — always show per-signal breakdown so you can see WHY an article ranked high.

```markdown
# AI 味濃度 = 多訊號加權
* 主張
  * 單一訊號容易冤枉或漏
  * 多訊號一致 → 才強
  * 分數要能解釋, 不能只出一個總分
* 三步驟
  * 每個訊號各自算 per_1k (V01-V05 已有)
  * 轉 percentile rank (自動去 scale)
  * soft 門檻 → 0-1 分 → 加權相加
* 建議權重
  * V01 —— w=3 (最乾淨的 tell)
  * V05 emoji w=2 (純度高)
  * V02 bold w=1.5 (排版習慣)
  * V04 hr w=1.5 (排版分隔)
  * V03 bq w=1 (有正當引言用途)
* 邊界
  * 短文放大 → 加最小字數門檻
  * 不是判決 是排序
  * 迭代: 看 top 30 是否符合直覺
```

---

## 為什麼要多訊號 — Why Combine Signals

- **單一訊號都有偏差**: V01 (`——`) 有人翻譯外文文章會用、V03 (blockquote) 有人真的在引言、V05 emoji 有人本來就活潑
- **多訊號一致才強**: 一篇文章同時 `——` 密、粗體多、hr 切段、emoji 灑, 幾乎不可能是純人手
- **可解釋**: 分數配上「命中哪幾個訊號」, 讀的人能看到 WHY, 不是黑箱

---

## 三步驟公式 — The Formula in Three Steps

### 步驟 1: 各自算 per_1k

沿用 V01–V05 已有的 `per_1k` (或 V02 那種 ratio × 1000). 這一步不變, 每個訊號就是原始的密度值.

### 步驟 2: 轉 percentile rank (自動去 scale)

各訊號 per_1k 的絕對值範圍差很大 (V04 hr 頂端 34, V05 emoji 頂端 21, V01 大概個位數). 直接相加會被大的訊號淹掉. 解法: **轉成 percentile rank**.

```
rank_i(article) = (該文章 signal_i 排在前幾 %)
                = P(signal_i(其他文章) < signal_i(article))
                ∈ [0, 100]
```

**副作用是「自動 baseline」**: 不需要決定「per_1k 幾以上算多」, corpus 自己決定. 換一份 corpus (例如去年鐵人賽當對照組), rank 會自動重算, 不用改任何參數.

### 步驟 3: soft 門檻 + 加權相加

要「命中才計分」而不是「連續加」, 因為底層假設是**罕見才是 tell** — 一篇文章 per_1k 是 corpus 中位數, 一點意義都沒有.

```
hit_i(article) = clamp((rank_i - 75) / 25, 0, 1)
```

- rank ≤ p75 → hit = 0 (太普通, 不計)
- rank = p87.5 → hit = 0.5 (半分)
- rank = p100 → hit = 1 (滿分)

比起硬門檻 (`hit = 1 if rank > 75`), 這種 soft 版本讓分數連續, 不會在門檻附近有階梯感.

```
composite(article) = Σᵢ wᵢ × hit_i(article)
```

滿分 = Σ wᵢ. 用建議權重 (下表) 滿分是 9.

---

## 建議權重 — Suggested Weights

先估一組, 跑完看結果再校準. 權重反映「這個訊號多可信是 AI tell」的先驗信念:

| # | 訊號 | 權重 | 理由 |
|:---|:---|:---:|:---|
| V01 | `——` (em dash × 2) | **3.0** | 中文寫作幾乎不用, 最乾淨的 tell |
| V05 | emoji | **2.0** | 純度高 (排除完 checklist 符號後只剩真 emoji), corpus 只 17% 用過, 用了就顯眼 |
| V02 | 粗體加權 (V02e) | **1.5** | `- **標籤**: 說明` pattern 特別像 AI, 但一般 markdown 排版也會用 |
| V04 | `<hr>` | **1.5** | AI 愛分段, 但長文人類也用, 需要跟其他訊號一起看 |
| V03 | blockquote | **1.0** | 有正當引言用途, 訊號較弱 |

滿分 3 + 2 + 1.5 + 1.5 + 1 = **9**.

---

## 邊界與陷阱 — Edge Cases

### 短文超敏感

200 字命中 1 顆 emoji → per_1k = 5, 大概率衝到 p95 以上. 跟 5000 字命中 25 顆 emoji 的 per_1k 一樣, 但後者訊號強得多.

**建議**: 排行時加**最小字數門檻**, 例如 `chars >= 500` 才進榜. 短於 500 字的文章單獨列一個「短文榜」.

### 引用他人的長段

有人整段引用官方文件 → V03 blockquote ratio 爆高. 這不是 AI 味.

**緩解**: composite 有多訊號一致的要求, 單一 V03 爆高但其他訊號都低 → composite 不高, 天然壓下去. 這也是為什麼要用綜合分而不是單看 V03.

### V01 短文剛好命中一次

500 字命中 1 個 `——` → per_1k = 2, 在 corpus 裡不算低. 單一次命中可能只是巧合 (作者引用 AI 文章的一段).

**緩解**: 一樣靠 composite 多訊號共現. 一次 `——` + 其他訊號都低 → composite 不高.

### rank 被同分很多稀釋

如果 corpus 裡幾百篇 V05 emoji_count = 0, rank 都會併到 p50 以下, 他們不會有 hit. 這是 feature 不是 bug — 沒 emoji 的文章確實不該從 emoji 訊號拿分.

---

## 實作提示 — Implementation Notes

打算另寫一支 `composeV01.py`:

1. **讀 5 份 CSV** — `articles.csv` (V01), `articles-v02.csv` … `articles-v05.csv`. 用 `article_id` join.
2. **各自算 rank** — `scipy.stats.rankdata` 或手寫 (sort + index). 只用 stdlib 就手寫.
3. **算 hit_i, composite** — 依上面公式.
4. **輸出 `articles-composite.csv`**:
   - 每列: article_id, url, title, series_title, chars, composite, 以及每個訊號的 `_per_1k` 和 `_hit`
   - 讓讀的人一眼看到「這篇分高是因為哪幾個訊號」
5. **`results-composite.md`**: top 30 排行, 每列顯示 composite + 5 個 hit 的 bar (例如 `V01:1.0 V02:0.8 V03:0 V04:0.5 V05:1.0`)

輸出結構刻意讓讀者能自己回頭看單一訊號. 這是**排序工具 + 解釋工具**, 不是判決.

---

## 校準流程 — Calibration Loop

初版跑完不會直接對, 要迭代:

1. **跑 composite**, 看 top 30
2. **人工看 5–10 篇**: 覺得像 AI 嗎? 為什麼像 / 不像?
3. **找 outlier**:
   - 「明顯 AI 味但分數不高」→ 是不是漏了某個訊號? (可能就是 lab04+ 要加的新 signal)
   - 「明顯人手但分數高」→ 是不是某個訊號權重太大? 或需要 exclusion (例如 V05 已經排除 ○ ✗ ★ ☆ ☐)
4. **調權重或門檻**, 重跑
5. **收斂條件**: top 30 有 25 個以上讓多數人覺得「這篇有 AI 味」

**別追求絕對正確**. 這是相對排序工具, 不是判決器. 反過來說, 一篇 composite 分數低也不代表「這是人寫的」— 可能是 AI 技巧比較好, 或訊號選集沒抓到那位作者的習慣.

---

## 為什麼不用 z-score / min-max / logistic regression

- **z-score**: 需要假設分布近常態, V01–V05 都是重度偏態 (絕大多數文章 = 0, 尾部拉很長), z-score 會被離群值扭曲
- **min-max**: 對離群值超敏感, corpus 出一篇超高分, 所有其他文章都被壓到接近 0
- **邏輯迴歸 / 分類器**: 需要**標註資料** (哪些是 AI 寫的), 目前沒有. 未來有的話可以走這條, 但那是 lab05+ 的事

**percentile rank 對重度偏態穩健**, 不需要標註, 不需要調分布假設參數. 這是 corpus 分析裡常見的 conservative 選擇.

---

## 相關筆記 — Related

- lab01 [README.md](./README.md) — 整個 lab 的定位跟資料流
- lab01 各 vNN 的 `results-vNN.md` — 各單一訊號的排行

## Sources

沒有外部來源, 純 lab 內部設計提案.
