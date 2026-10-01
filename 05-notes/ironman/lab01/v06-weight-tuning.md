---
title: lab01 V06 權重微調 — 從憑感覺到有依據
created: 2026-09-30
status: obsolete
---

# V06 權重微調 — Calibrating V06 Weights

> [!CAUTION]
> **本份已 obsolete, 需要重寫**. 2026-09-30 的分析基於「V02 all N=1」的**錯誤前提**, 事實上原設計意圖是 V02 all N=0 (不計分, 只記錄命中次數). 修正後 V02 all 根本不吃權重, 「V02 all 吃 30.7%」這個驅動 recommendation 的 observation 不成立, 整份 analysis 失去意義.
>
> 修正後的實際 code 已更新 (`analyzeV06.py` V02 all N=0, 其他保持原本 V01=4, strict=2, bq=1, hr=1), 但這篇 note 的 recommendation 不再適用. 要重做 principled 微調的話, 需要用修正後的 contribution 分佈重新分析.
>
> 以下內容保留作紀錄, **不要照抄**.

---

> [!NOTE]
> V06 的原本 N 權重 (V01 4 / V05 types / V02 strict 2 / V02 all 1 / V03 1 / V04 1) 是憑感覺寫的. 這篇用當前 corpus (2026-09 鐵人賽 15057 篇) 的 coverage + contribution 數據, principled 推一組新 N, 並列出理由. emoji 的 `count × distinct_types` 當 anchor 不動.

> **TL;DR (EN):** Analyzed current V06 weight contributions on 15,057 articles. Problem: V02 all-bold eats 30.7% of base despite 68% coverage (too generic to deserve that much weight), while V02 strict pattern (the actual AI tell) only gets 11.8%. Proposed re-calibration keeping emoji × distinct_types as anchor: raise V02 strict 2→3 and V04 hr 1→2, drop V02 all 1→0.5. New contribution distribution: strong tells (V01/emoji/strict) each ~18-28%, weak tells (V03) at ~6%. Three variants (conservative / recommended / aggressive) listed so user can pick.

```markdown
# V06 權重微調
* 問題
  * 舊 N 憑感覺, V02 all 吃 30.7% 權
  * V02 strict 更強但只 11.8%
* 兩個 principled 原則
  * 稀有度 rarity = 1/coverage
  * 強度 prior (強 tell vs 弱 tell)
* 數據基礎
  * emoji avg effective = 6.51 (anchor)
  * 各 signal coverage + contribution
* 推薦方案
  * V01 4 不動
  * V02 strict 2→3
  * V02 all 1→0.5
  * V04 hr 1→2
  * V03 bq 1 不動
* 三個選項
  * 保守 / 推薦 / 激進
```

---

## 當前問題 — The Problem

舊 N 是憑感覺寫的, 跑完 15057 篇 corpus 後看 contribution 分佈, 問題明顯:

| Signal | coverage | 當前 contribution % | 問題 |
|:---|---:|---:|:---|
| V01 `——` | 30.2% | **27.6%** | 合理, 強 tell |
| V05 emoji | 16.9% | 17.7% | anchor, N = distinct types |
| V02 strict | 28.2% | 11.8% | **過低**, 強 AI pattern 應該更吃分 |
| V02 all | 68.2% | **30.7%** ← 最大 | **過高**, 一般粗體太 generic, 68% 文章都用 |
| V03 bq | 42.9% | 5.5% | 合理, 弱訊號 (有正當引言用途) |
| V04 hr | 38.2% | 6.7% | **過低**, AI 愛切章節, 比 bq 稀 |

**emoji 平均每次命中 effective weight = 6.51** (= total_contrib / total_B). 這是 anchor baseline, 其他 signal 的 N 要跟這個量級對齊才合理.

---

## 兩個 principled 原則 — Two Principles

要有依據地推 N, 不再憑感覺. 用兩條原則交叉決定:

**原則 1: 稀有度 (rarity-based)**
`rarity = 1 / coverage`. coverage 越低 → 越稀有 → 命中權重該越大.

- V05 emoji: 1 / 0.169 = 5.92 (最稀有)
- V02 strict: 1 / 0.282 = 3.55
- V01 ——: 1 / 0.302 = 3.31
- V04 hr: 1 / 0.382 = 2.62
- V03 bq: 1 / 0.429 = 2.33
- V02 all: 1 / 0.682 = 1.47 (最 generic)

**原則 2: 訊號強度 prior (tell strength prior)**
基於 AI writing detection 文獻跟直覺:

- **強 tell**: V01 `——` (中文幾乎不用), V05 emoji (variety 天然懲罰), V02 strict (`- **label**:` 典型 AI pattern)
- **中性 tell**: V04 hr (AI 愛切章節但長文人類也用)
- **弱 tell**: V02 all bold (一般排版都有), V03 blockquote (有正當引言用途)

兩原則**交叉**: 稀有 + 強 tell → N 提高; 高覆蓋 + 弱 tell → N 壓低.

---

## 推薦方案 — Recommended Weights

| Signal | 舊 N | **新 N** | 新 contribution % | 理由 |
|:---|:---:|:---:|---:|:---|
| V01 `——` | 4 | **4** (不動) | 28.4% | 跟 emoji avg (6.51) 同量級, 不需調 |
| V05 emoji | types | **types** (anchor) | 18.2% | 不動 |
| V02 strict | 2 | **3** | 18.2% | 強 AI pattern, 從被 all 淹沒拉回來 |
| V02 all | 1 | **0.5** | 15.8% | 68% 覆蓋 = 太 generic, 權重壓一半 |
| V03 bq | 1 | **1** (不動) | 5.6% | 弱訊號 (有引言用途), 保持低 |
| V04 hr | 1 | **2** | 13.7% | AI 愛切章節, 升一倍 |

**新 contribution 分佈**: top 3 (V01 / emoji / V02 strict) 各佔 ~18-28%, 強 tell 群. V02 all + V04 hr 各佔 ~14-16%, 中性. V03 bq 5.6%, 弱.

### 單項調整理由

- **V01 `——` 不動**: 已經是跟 emoji avg (6.51) 同量級的強 tell, 看數字 27.6% contribution 合理
- **V02 strict ↑ 2→3**: 現在被 all (太大) 淹沒. 升到 3 後 strict 命中 effective 權重 = 3 + 0.5 = 3.5 (strict ⊂ all, 兩個都算), 弱 pattern (只中 all 非 strict) = 0.5. 差 7 倍, 符合「strict 是真 AI」的直覺
- **V02 all ↓ 1→0.5**: 68.2% 文章都用, 根本不稀有, 不配拿 30% 權. 壓一半讓它變成「small boost」而非主宰
- **V04 hr ↑ 1→2**: coverage 38.2% 比 bq 42.9% 稀, 且 hr 更明顯是 AI 排版 (切章節). bq 有正當引言用途, hr 幾乎只有 AI 愛用, 應該更吃分
- **V03 bq 不動**: 弱訊號 (真的有引言用途), 權重保持低. corpus 中有些系列整段引用官方文件, 不是 AI tell

---

## 三個選項 — Three Variants

讓 user 挑. 保守 = 動最少, 激進 = 最大化強弱差距.

| 方案 | V01 | emoji | V02 strict | V02 all | V03 bq | V04 hr |
|:---|:---:|:---:|:---:|:---:|:---:|:---:|
| 原本 (憑感覺) | 4 | types | 2 | 1 | 1 | 1 |
| 保守微調 | 4 | types | 3 | 0.75 | 1 | 1.5 |
| **推薦** | **4** | **types** | **3** | **0.5** | **1** | **2** |
| 激進 | 5 | types | 4 | 0.3 | 0.5 | 2 |

- **保守**: 修 V02 strict/all 失衡 + 微調 V04. 動最少, 安全
- **推薦**: 補上 V04 hr 升到 2, V02 all 壓到一半, 平衡 contribution
- **激進**: V01 升到 5, V02 strict 到 4, V03 bq 砍半. 最大化強弱差距但偏主觀

---

## 下一步 — Next Steps

1. 選方案 (推薦為預設)
2. 改 `analyzeV06.py` 的 `score_base()`
3. 重跑, 比較新舊 top 20
4. 看新排名是否符合直覺. 若不符合:
   - 排進榜的「不像 AI」 → 可能某訊號權重還太大
   - 掉出榜的「明顯 AI」 → 某訊號太低, 補
5. 迭代

**不追求絕對正確**. 這是相對排序工具, 不是判決器. 「換一組權重排名會變」本身就是這類方法的 feature, 不是 bug.

---

## 相關 — Related

- [composite-score-v06.md](./composite-score-v06.md) — V06 的原始設計 (絕對值加權型)
- [composite-score.md](./composite-score.md) — 姊妹方案 (percentile rank 型)
- [analyzeV06.py](./analyzeV06.py) — 現行 code, 要改的檔案
- [results-v06.md](./results-v06.md) — 現行結果, 比較基準
