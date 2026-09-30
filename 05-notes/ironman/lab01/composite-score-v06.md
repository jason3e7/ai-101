---
title: lab01 V06 — 絕對值加權型綜合分數
created: 2026-09-30
---

# V06 綜合分數 — Absolute-Value Weighted Composite

> [!NOTE]
> 跟 [composite-score.md](./composite-score.md) 的 percentile rank 版是**兩條並行的路線**. V06 走「命中次數 × 權重, 除總字數」, 更貼近直覺, 不需要跑完 corpus 才能算, 缺點是各訊號的絕對尺度差異沒自動去掉, 得靠人工調權重.

> **TL;DR (EN):** Per article: score = (Σ signal_count × weight) / chars × 1000. Weights: em-dash × 4 (strongest tell), emoji count × distinct types (variety penalty), strict-bold × 2, all-bold × 1, blockquote × 1, hr × 1. Ranks by density (per_1k), so short articles that hit lots of signals still surface. Simpler and more direct than the percentile-rank composite; complements rather than replaces it.

```markdown
# V06 = Σ(count × weight) / chars × 1000
* 對比 composite-score.md 的 percentile 版
  * V06 絕對值 · percentile 相對排名
  * V06 貼直覺 · percentile 自動去 scale
  * 並行不取代
* 六個訊號 + 權重
  * V01 —— × 4 (最強)
  * V05 emoji × distinct_types (種類懲罰)
  * V02 strict × 2 (子集加倍計)
  * V02 all × 1
  * V03 bq × 1
  * V04 hr × 1
* 正規化
  * 除總字數 → per_1k
  * 解決短文絕對值偏低問題
  * 但短文密度會爆
* 校準
  * 看 top 30 是否符合直覺
  * 調權重, 重跑
```

---

## 公式 — Formula

```
base(article)
    = em_count       × 4                     # V01 ——
    + emoji_count    × emoji_types           # V05 emoji (種類數當 multiplier)
    + strict_bold    × 2                     # V02 嚴格 `- **標籤**: xxx`
    + all_bold       × 1                     # V02 一般 `**標籤**`
    + bq_count       × 1                     # V03 blockquote
    + hr_count       × 1                     # V04 <hr>

density(article) = base / chars × 1000       ← 排名主指標
```

**strict 是 all 的子集**: 一個「嚴格 pattern」命中會被 strict 與 all 各計一次, 等於總權重 × 3. 這是刻意 (延續 V02e 的 strict 加算精神).

**emoji 排除清單** 沿用 V05: `○ ✗ ★ ☆ ☐`, 這些 checklist / 星等 / 圈叉 排版符號人類寫作也常用, 排除掉才乾淨.

---

## 為什麼這樣設 — Why These Weights

| 訊號 | 權重 | 理由 |
|:---|:---:|:---|
| V01 `——` | 4 | 中文寫作幾乎不用, 最乾淨的 tell |
| V05 emoji × types | count × types | 一種 emoji 灑十次比較弱, 十種各一次強得多. AI 愛「種類齊全」的排版, 用 distinct types 當 multiplier 直接放大這件事 |
| V02 strict | 2 | `- **標籤**: 說明` 是最像 AI 的 pattern (V02e 已驗證) |
| V02 all | 1 | 一般粗體排版習慣, 弱訊號 |
| V03 blockquote | 1 | 有正當引言用途, 訊號弱 |
| V04 `<hr>` | 1 | AI 愛分段, 但長文人類也用 |

**emoji 的 count × types 是 V06 的特色**:

- 5 個相同 emoji 😀×5 → count = 5, types = 1, 分數 = 5
- 5 個不同 emoji 😀🎉✅💡🚀 → count = 5, types = 5, 分數 = 25
- 10 個 = 5 種各 2 → count = 10, types = 5, 分數 = 50

「種類多樣性」是很強的 AI tell (人類寫作通常固定幾個常用 emoji, AI 會刻意展示變化).

---

## 對比 [composite-score.md](./composite-score.md) — Comparison

|  | V06 (這份) | composite-score.md (percentile 版) |
|:---|:---|:---|
| 正規化方式 | 除總字數 (per_1k) | 各訊號轉 percentile rank |
| 依賴 corpus | 不用, 一篇也能算 | 要, 一篇不能算 |
| 直覺易懂 | ✅ count × weight | 需先解釋 percentile |
| 尺度自動處理 | ❌ 要手調權重 | ✅ 自動去 scale |
| 對重度偏態 | 密度分布可能仍偏 | 對偏態穩健 |
| 短文問題 | 密度會爆 | 一樣會爆 (rank 也高) |
| 校準難度 | 靠人工調權重 | 靠人工調權重 + 門檻 |

**建議兩條路都跑, 對照結果**:
- 交集 (兩個排名都靠前) → 高信心 AI 味
- 差異大 → 值得看單一訊號是哪裡不一樣
- V06 分數低但 percentile 版高 → 可能該篇每項訊號都「普通高」, V06 因為絕對值不夠拉不上來
- V06 分數高但 percentile 版低 → 可能該篇單一訊號超極端, 但其他訊號都平

---

## 邊界與陷阱 — Edge Cases

### 短文密度爆

200 字命中 10 個 emoji (5 種) → density = (10 × 5) / 200 × 1000 = 250. 5000 字命中 100 個 emoji (10 種) → density = 1000 / 5000 × 1000 = 200. 短文分數比長文高.

**這是刻意**: 除總字數就是為了做密度比較, 短文密度高本來就顯眼. 讀者看排行要一併看「總字」欄, 判斷短文是「真濃度高」還是「樣本太少」.

### 引用他人長段

blockquote 灌爆分數 → 但 V03 權重只有 1, 單一訊號拉不了多少. 除非同時 emoji 很多 + 粗體多, 否則不會爆.

### 空 emoji 陷阱

如果一篇 emoji_count = 0, types = 0, emoji 貢獻 = 0. 沒問題.

如果 emoji_count = 3 但都是同一種 (types = 1), 貢獻 = 3. 弱訊號, 符合直覺.

---

## 校準流程 — Calibration Loop

同 [composite-score.md](./composite-score.md):

1. 跑 V06, 看 top 30
2. 人工看 5-10 篇, 判斷「像 AI 嗎」
3. 找 outlier 調權重
4. 收斂條件: top 30 有 25 個以上覺得像 AI

**先跑一次不校準**, 看初版結果, 再決定要動哪個權重.

---

## 相關 — Related

- [composite-score.md](./composite-score.md) — percentile rank 版, V06 的姊妹方案
- [README.md](./README.md) — lab01 整體
- 各 vNN 的 `results-vNN.md` — 各單一訊號排行

## Sources

沒有外部來源, 純 lab 內部設計.
