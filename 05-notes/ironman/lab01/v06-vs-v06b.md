---
title: lab01 V06 vs V06B — 兩種綜合分數算法對照
created: 2026-10-01
---

# V06 vs V06B — Comparing Two Composite Score Formulas

> [!NOTE]
> V06 原本實作成「每個 signal 的 B × 自己的 N, 再加總」(乘積和). 討論中發現原始設計意圖是「所有 B 加總, 乘上所有 N 加總」(和的乘積). 兩種算法對 V02 all N=0 的處理差很多: V06 讓 V02 all 完全消失, V06B 讓它作為「痕跡放大器」. 這篇用同一份 2026-09 corpus (15057 篇) 跑兩個算法, 比對排名差異, 討論兩者各自的語意與用途.

> **TL;DR (EN):** Two composite-score formulas for V06. V06 is `Σ(Bᵢ × Nᵢ)` where V02 all (N=0) contributes nothing. V06B is `B_total × N_sum` where V02 all still inflates B_total even though its own weight is zero. Both run on same 15,057 articles. Spearman rank correlation 0.95 (strongly correlated overall) but top-10 overlap is only 3/10. V06 ranks em-dash + emoji-variety + strict-pattern heavy articles highest; V06B promotes "markdown-formatting-heavy" articles that pile on all bold + hr + bq together. 使用gemini 準備 az-900 series jumps from rank 58 (V06) to rank 1 (V06B) because it stacks all signals; JS 核心重構 series stays top in both but shuffles. Different formulas measure different notions of "AI-smell": V06 catches cleanest AI tells, V06B catches markdown-formatting load.

```markdown
# V06 vs V06B 兩種綜合分數
* 公式差別
  * V06  Σ(Bi × Ni) 每 signal 各自 B×N 再加
  * V06B (ΣBi) × (ΣNi) 總 B × 總 N
  * N=0 對 all 的影響不同
* 全體數字
  * V06 density 8.64
  * V06B density 48.47 (約 5.6x)
* 排名相關性
  * Spearman 0.95 高相關
  * top 10 交集只 3
  * top 20 交集 10
  * top 100 交集 76
* 誰上誰下
  * V06B 把「使用gemini 準備 az-900」系列拉上 top
  * V06 的 JS 核心重構 系列有一些掉名次
* 哲學差異
  * V06 選擇性加權, 弱 signal 消音
  * V06B 總量加權, 弱 signal 當放大器
```

---

## 公式對比 — The Two Formulas

**V06 (current)**:

```
base = Σᵢ (Bᵢ × Nᵢ)
     = 4×em + emoji_types×emoji + 2×strict + 0×all + 1×bq + 1×hr
```
每 signal 用自己的 N 乘自己的 B, 再全部加. V02 all 的 N=0 → all 命中再多, 貢獻恆 0.

**V06B (user intent)**:

```
B_total = em + emoji + strict + all + bq + hr                    ← 命中總次數 (含 all)
N_sum   = Σ (Nᵢ where Bᵢ > 0)                                     ← 只對命中的 signal 加 N
        = (4 if em>0) + (types if emoji>0) + (2 if strict>0)
          + (0 if all>0) + (1 if bq>0) + (1 if hr>0)
base    = B_total × N_sum
```

all 的 count 也進 B_total, 等於「痕跡放大器」. 但單獨命中 all → N_sum=0 → base=0 (放大器也沒東西可放大).

兩個都用 `density = base / chars × 1000` 當排名指標.

---

## 全體數字 — Corpus-Level Numbers

| 指標 | V06 | V06B | 比值 |
|:---|---:|---:|---:|
| 系列數 | 814 | 814 | — |
| 文章數 | 15,057 | 15,057 | — |
| base 總和 | 342,543 | 1,921,010 | **×5.6** |
| 全體 density | 8.64 | 48.47 | **×5.6** |

V06B 的 base 普遍是 V06 的 5.6 倍 (平均而言). 但 scale 不是重點, 排名才是.

---

## 排名重疊分析 — Rank Overlap

| Top N | V06 ∩ V06B | 只在 V06 | 只在 V06B |
|---:|---:|---:|---:|
| 10 | **3** | 7 | 7 |
| 20 | 10 | 10 | 10 |
| 50 | 34 | 16 | 16 |
| 100 | 76 | 24 | 24 |

**Spearman rank correlation (全 corpus) = 0.9501**

- 全 corpus 層級高相關 (0.95), 兩個算法大致同意「誰 AI 味高誰低」
- Top 10 層級差異巨大 (交集只 3), 細節排名完全不同
- Top 20 一半交集, Top 100 四分之三交集 → 「大致同一群人, 但局部洗牌劇烈」

這很符合直覺: 當分數高到一定程度, 兩個算法的「選擇哲學」差異會放大, 讓極端排名差很多.

---

## Top 10 對照 — Head-to-Head

| 排名 | V06 | V06B |
|:---:|:---|:---|
| 1 | 驗證使用 AI 助教 (246.58) | **使用gemini 準備 az-900 DAY 6** (1097.92) |
| 2 | Day 02 清點魔法物資 (JS 核心重構, 233.88) | **Day 12 你敢不敢承認** (Phoenix 2026, 1040.56) |
| 3 | Day 12 邏輯開關 (JS 核心重構, 209.86) | Day 02 清點魔法物資 (JS 核心重構, 1013.94) |
| 4 | Day 20 Promise (JS 核心重構, 204.59) | **使用gemini 準備AZ-900 Day21** (963.54) |
| 5 | Day 15 事件委派 (JS 核心重構, 200.12) | **使用gemini 準備 az-900 DAY 1** (951.84) |
| 6 | Day 03 箭頭函式 (JS 核心重構, 189.83) | Day 03 箭頭函式 (JS 核心重構, 946.43) |
| 7 | Day 10 this (JS 核心重構, 187.86) | Day 27 戰略指揮 (JS 核心重構, 941.71) |
| 8 | 我想像中的未來小豬 (187.27) | 我想像中的未來小豬 (912.09) |
| 9 | Day 17 Timeout (JS 核心重構, 180.01) | Day 26 效能神兵 (JS 核心重構, 904.31) |
| 10 | 小豬的健康讓我來守護 (179.72) | **使用gemini 準備 az-900 Day 4** (876.55) |

**V06B 新進榜 top 10 (粗體)**:

- 「使用gemini 準備 az-900」系列 4 篇 (V06 排 35-152 → V06B 1-10)
- 「Phoenix 2026」1 篇 (V06 排 63 → V06B 2)
- 掉榜: V06 的 JS 核心重構 Day 12/15/20/10/17 掉到 V06B 排 20-30 外

---

## 誰上誰下 — Who Jumps / Falls

### V06B top 20 新進榜 (V06 排名較低) — 10 篇

| V06B | V06 | 文章 | 系列 |
|---:|---:|:---|:---|
| 1 | 58 | 使用gemini 準備 az-900 DAY 6 | 使用gemini 準備 az-900 |
| 4 | 28 | 使用gemini 準備AZ-900 Day21 | 使用gemini 準備 az-900 |
| 5 | 35 | 使用gemini 準備 az-900 DAY 1 | 使用gemini 準備 az-900 |
| 10 | 93 | 使用gemini 準備 az-900 Day 4 | 使用gemini 準備 az-900 |
| 12 | 63 | Day 10: 你敢不敢承認 | Phoenix 2026 |
| 16 | 37 | 使用gemini 準備AZ-900 Day28 | 使用gemini 準備 az-900 |
| 17 | 32 | Day 06 影印術 | JS 核心重構 |
| 18 | 104 | 使用gemini 準備 az-900 Day 5 | 使用gemini 準備 az-900 |
| 19 | **152** | 使用gemini 準備 az-900 Day 8 | 使用gemini 準備 az-900 |
| 20 | 22 | Day 07 記憶膠囊 | JS 核心重構 |

**最大跳躍: 「使用gemini 準備 az-900 Day 8」V06 排 152 → V06B 排 19** (跳 133 名). 這個系列有特殊的排版 pattern 讓 V06B 爆分.

### V06 top 20 掉出 V06B top 20 — 10 篇

| V06 | V06B | 文章 | 系列 |
|---:|---:|:---|:---|
| 3 | 25 | Day 12 邏輯開關 | JS 核心重構 |
| 5 | 28 | Day 15 事件委派 | JS 核心重構 |
| 7 | 29 | Day 10 this | JS 核心重構 |
| 9 | 22 | Day 17 Timeout | JS 核心重構 |
| **10** | **72** | 小豬的健康讓我來守護 | 前端三分鐘 X 要轉職養豬 |
| 11 | 32 | Day 14 事件監聽 | JS 核心重構 |
| 12 | 31 | Day 3 身份與網路安全 IAM | 30 天的 SAA 學習筆記 |
| 13 | 37 | Day 11 類別 Class | JS 核心重構 |
| 16 | 21 | Day 08 物件導航 | JS 核心重構 |
| 19 | 45 | Day 13 DOM 樹大探險 | JS 核心重構 |

**最大掉落: 「小豬的健康讓我來守護」V06 排 10 → V06B 排 72** (掉 62 名). V06 給它高分因為 emoji variety 強, V06B 把它壓下因為其他 signal 的 count 都不高, B_total 不夠大.

---

## 為什麼差異 — Why the Divergence

### V06B 把這些拉上來的原因

「使用gemini 準備 az-900」系列每篇**所有 signal 都命中, 而且 count 都很大**. 以 Day 21 為例:

```
em=20, emoji=101(25 種), strict=170, all=248, bq=15, hr=21
B_total = 575
N_sum = 4 + 25 + 2 + 0 + 1 + 1 = 33
V06B base = 575 × 33 = 18,975
V06  base = 20×4 + 101×25 + 170×2 + 15 + 21 = 80 + 2525 + 340 + 36 = 2,981
```

V06B 比 V06 高 6.4 倍, 因為 all=248 進了 B_total, strict=170 進了 B_total, 兩個 big number 乘上 33 的 N_sum. V06 下 all 完全消失, strict 只貢獻 340, 遠低於 B_total×N_sum 的放大效應.

### V06 把這些拉上來的原因

「小豬的健康讓我來守護」:

```
em=1, emoji=31(9 種), strict=0, all=14, bq=5, hr=8, chars=1647
V06  base = 1×4 + 31×9 + 0 + 5 + 8 = 296, density = 179.72
V06B B_total = 1+31+0+14+5+8 = 59
V06B N_sum = 4 + 9 + 0 (strict=0) + 0 (all is 0 anyway) + 1 + 1 = 15
V06B base = 59 × 15 = 885, density = 537.34, 排名 72
```

V06 排 10, V06B 排 72. V06 給它高分是因為 emoji_count × emoji_types (31 × 9 = 279) 吃了大部分分. V06B 下 N_sum 只有 15 (沒有 strict), B_total 只有 59 (沒有大量 all), 乘起來不如那些「什麼都塞」的系列.

---

## 建議 — Which to Use

**不是「哪個對」, 是「要測什麼」**:

| 情境 | 推薦 | 理由 |
|:---|:---|:---|
| 想抓**乾淨的 AI tell** (em dash, emoji variety, strict pattern) | **V06** | 選擇性加權, 強 signal 吃分, 弱 signal (all) 消音 |
| 想抓**markdown 排版重度使用者** | **V06B** | 總量加權, 整篇排版「堆料」就會爆分 |
| 當**初篩** (寬網抓可疑文章) | V06B | 覆蓋廣, 排版重的都進榜 |
| 當**細篩** (確認是強 AI 排版) | V06 | 排名的 top 都是真的用了強 tell |
| 做 lab04 綜合指標的 input | **兩個都可以當 feature** | 不同哲學, 可能給 classifier 不同的 signal |

**保留兩份並存**, 不互相取代. 日後跑實驗選適合當下任務的那個就好.

---

## Related

- [analyzeV06.py](./analyzeV06.py) — V06 current (Σ(B×N))
- [analyzeV06B.py](./analyzeV06B.py) — V06B (B_total × N_sum)
- [results-v06.md](./results-v06.md) / [results-v06b.md](./results-v06b.md) — 各自排行
- [composite-score-v06.md](./composite-score-v06.md) — V06 原始設計
- [composite-score.md](./composite-score.md) — percentile rank 版 (另一條路)
