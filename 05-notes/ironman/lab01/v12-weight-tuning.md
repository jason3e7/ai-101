---
title: lab01 V12 權重微調 — data-driven, 以 emoji anchor 不動
created: 2026-10-02
---

# V12 權重微調 — Calibrating V12 Weights

> [!IMPORTANT]
> **已套用 (2026-10-02)**: `analyzeV12.py` 的實際權重為 **v11 5→2、v08c 2→3、v09 2→3**（在「推薦」與「保守」之間：v11 用推薦值、v08c/v09 用保守值）。下面的三方案分析是提案當時的紀錄，`results-v12.md` 已是套用後的結果。

> [!NOTE]
> V12 的 N 權重大多從 V06B 繼承 (em 4, strict 3, bq 1, hr 1.5), v08c/v09 用了 2, v11 剛升到 5. 這篇用 2026-09 corpus (15057 篇) 的 contribution + ablation 數據, data-driven 推一組新 N. emoji 的 `min(distinct_types, 5)` 當 anchor 不動.

> **TL;DR (EN):** V12 inherited N weights from V06B plus three new signals (v08c=2, v09=2, v11=5). Data analysis reveals two imbalances: v11 (title ｜) at 27.8% of N_sum is overweighted for a cosmetic signal, while v09 (「最容易...的」) at 2.2% is underweighted despite being the cleanest AI tell in corpus. Recommended tune (emoji anchor unchanged): v11 5→2, v08c 2→3, v09 2→4. Rationale: clean prose signals (em / strict / v08c / v09) should dominate collectively (~53% of N_sum), cosmetic v11 should drop to typographic-signal range (~11%). Three variants (conservative / recommended / aggressive) listed.

```markdown
# V12 權重微調
* 當前問題
  * v11=5 貢獻 27.8% 太強, cosmetic 不該主宰
  * v09=2 貢獻 2.2% 太弱, 最乾淨 tell 應更高
  * v08c=2 偏低, 強 AI pattern 應跟 strict 同級
* 兩個原則
  * clean prose signals 應同量級
  * cosmetic signal 不該勝過 prose
* 數據基礎
  * emoji cap 後 avg = 3.64 (anchor)
  * 各 signal coverage + N_sum 貢獻 + ablation
* 推薦方案
  * v11 5→2 (降, cosmetic 回到合理區)
  * v08c 2→3 (升, 跟 strict 同級)
  * v09 2→4 (升, 補稀有性, 跟 em 同級)
  * em/strict/emoji/bq/hr 不動
* 預期效果
  * clean prose 合計 ~53% (vs 當前 46%)
  * v11 降到 ~11%
  * v09 升到 ~4%
```

---

## 當前 V12 問題 — Current Imbalance

### N_sum expected contribution per avg article

| Signal | coverage | 當前 N | contribution % | 問題 |
|:---|---:|---:|---:|:---|
| **v11** | 34.8% | 5 | **27.8%** ← 最大 | **過強**, cosmetic 不該主宰 |
| em | 30.2% | 4 | 19.3% | 合理 |
| strict | 28.2% | 3 | 13.5% | 合理 |
| v08c | 35.4% | 2 | 11.3% | 偏低, strong AI tell 應更高 |
| emoji | 16.9% | ~3.64 | 9.8% | anchor, 不動 |
| hr | 38.2% | 1.5 | 9.2% | 合理 |
| bq | 42.9% | 1 | 6.9% | 合理 |
| **v09** | 6.9% | 2 | **2.2%** ← 最小 | **過弱**, 最乾淨 tell 應更高 |
| all | 68.2% | 0 | 0% | 刻意不計 |

avg N_sum per article = 6.26

### Ablation 測試 (N=0 base 總和下降 %)

| Signal | drop | 註 |
|:---|---:|:---|
| em | **21.8%** | 高 B_total 配強 N |
| strict | 19.0% | 強 clean pattern |
| **v11** | **18.2%** | 實質影響大, 但是 cosmetic |
| emoji | 13.6% | anchor |
| hr | 10.4% | |
| v08c | 9.5% | 偏低 |
| bq | 7.5% | |
| **v09** | **2.6%** | 稀有但命中真是 AI tell |

### 問題診斷

1. **v11 N=5 過強**: cosmetic signal (title ｜) 不該跟 em / strict 這類 prose tell 平起平坐
2. **v09 N=2 過弱**: V09「最容易...的」是**所有 ranker 中最乾淨的 AI tell** (coverage 6.9%, top 10 100% AI 教學腔, 無 false positive), 稀有命中但命中就極強, 當前權重讓它幾乎沒影響力
3. **v08c N=2 偏低**: V08C「不是…而是」跟 strict「- **標籤**:」都是 clean AI pattern, 應該同量級
4. **em / strict / emoji / bq / hr 都在合理範圍** (V06B 微調已校過)

---

## 兩個原則 — Two Principles

**原則 1: Clean prose signals 應在同量級**
- em (「——」), strict (「- **標籤**:」), v08c (「不是…而是」), v09 (「最容易...的」) 都是強 clean AI tell
- 應該 N 落在 3-4 區間, 不該讓單一 signal 主宰
- 一起加起來佔 N_sum 50%+ 比較合理

**原則 2: Cosmetic signal 不該勝過 prose**
- v11 (title ｜) 是 cosmetic 排版 signal, 強度不如 prose cliche
- bq (markdown blockquote) 跟 hr (切章節) 也是 cosmetic, 都在 N=1-1.5
- v11 應該落在這個區間, 不該升到 N=5

---

## 推薦方案與理由 — Recommendation

### 三個方案

| Signal | 當前 N | 保守 | **推薦** | 激進 |
|:---|:---:|:---:|:---:|:---:|
| em | 4 | 4 | **4** | 4 |
| emoji | min(types,5) | anchor | **anchor** | anchor |
| strict | 3 | 3 | **3** | 4 |
| all | 0 | 0 | **0** | 0 |
| bq | 1 | 1 | **1** | 1 |
| hr | 1.5 | 1.5 | **1.5** | 1.5 |
| **v08c** | 2 | 3 | **3** | 4 |
| **v09** | 2 | 3 | **4** | 5 |
| **v11** | 5 | 3 | **2** | 1 |

### 逐項理由 (推薦方案)

- **v11 5 → 2**: 當前 27.8% N_sum 貢獻遠超合理. 降到 2 讓它跟原本的 v08c (2) 同級, 位於 cosmetic / 弱訊號區 (bq 1 / hr 1.5 / v11 2)
- **v08c 2 → 3**: 強 clean AI pattern, 升到跟 strict (3) 同級. 兩個最強的 prose cliche 分量相等
- **v09 2 → 4**: 最乾淨的 AI tell (本 lab 試過的 9 個 ranker 裡 top 10 誤判率最低). 稀有命中但命中就該重算分, N=4 跟 em 同級
- **em / strict / emoji / bq / hr 不動**: V06B 微調已校過的設定

### 跟 V06B / V06B 推薦為什麼不同

V06B 只處理 em / emoji / strict / all / bq / hr 六個訊號. V12 加 v08c / v09 / v11 三個新訊號, 這篇是**新訊號怎麼校**的提案:

- V06B 權重保留 (已校過)
- 新 3 個訊號跟 V06B 原有訊號的相對關係要校正
- 推薦讓 clean prose tell 群 (em, strict, v08c, v09) 落在 N=3-4 區間, 一致量級

---

## 預期效果 — Expected Impact

套用推薦後預估 N_sum 貢獻:

| Signal | 當前 % | 推薦 N | 預估新 % |
|:---|---:|:---:|---:|
| em | 19.3% | 4 | ~19% |
| strict | 13.5% | 3 | ~13% |
| **v08c** | 11.3% | 3 | **~17%** |
| emoji | 9.8% | anchor | ~10% |
| hr | 9.2% | 1.5 | ~9% |
| bq | 6.9% | 1 | ~7% |
| **v09** | 2.2% | 4 | **~4%** |
| **v11** | 27.8% | 2 | **~11%** |

**Clean prose 加總 (em + strict + v08c + v09)**: 當前 46% → 預估 **53%**, 更貼近「文字層 AI 味主宰」設計哲學.

**排名變化預期**:
- **使用gemini az-900 系列**可能更不利 (無 prose signal + cosmetic v11 降權)
- **教老人 Google AI / Phoenix 2026 / OpenClaw** 這類「對立句密集 + 教學警告」文章往上
- **純標題 ｜ 但 prose 乾淨**的文章從 top 掉下
- **真 AI 教學警告腔**(v09 命中)系列會明顯浮上

但預期只是預期, 跑完才知道.

---

## 下一步 — Next Steps

1. 選方案 (推薦 / 保守 / 激進)
2. 改 `analyzeV12.py` 的 `compute_v12()` 的 N 值
3. 重跑, 比較新舊 V12 top 20
4. 迭代

---

## Related

- [composite-score-v06.md](./composite-score-v06.md) — V06 原始設計
- [v06b-weight-tuning.md](./v06b-weight-tuning.md) — V06B 權重微調 (套用後 strict 2→3, hr 1→1.5)
- [analyzeV12.py](./analyzeV12.py) — V12 code, 要改的檔案
- [results-v12.md](./results-v12.md) — 當前結果, 比較基準
- [four-ranker-compare.md](./four-ranker-compare.md) — V06B / V07 / V08C / V09 對照 (建立 V12 加 3 signal 的動機)
