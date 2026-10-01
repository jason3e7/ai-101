---
title: lab01 V06B 權重微調 — N 作為 vote 分量的 data-driven 提案
created: 2026-10-01
updated: 2026-10-01 (套用保守方案, 補實測結果)
---

# V06B 權重微調 — Calibrating V06B Weights

> [!NOTE]
> V06B 公式 `base = B_total × N_sum` 下, N 的意義跟 V06 的 `Σ(B×N)` 不一樣. V06 的 N 是「該 signal 的直接乘數」, V06B 的 N 是「訊號 vote 的分量 + N_sum 的 budget 開關」. 原本 V06B 繼承 V06 的 N (V01=4, strict=2, bq=1, hr=1), 是憑感覺的, 這篇用當前 corpus (15057 篇) 的 contribution + ablation 數據, data-driven 推一組新 N. emoji 的 `distinct_types` 當 anchor 不動.

> **TL;DR (EN):** V06B's `base = B_total × N_sum` means N controls each signal's vote weight on the shared budget, differently than V06's per-signal scalar. Analysis of current 15,057-article contribution shows V01 em dominates at 39.3% of N_sum and 31.8% of total base (via ablation). V05 emoji_types averages only 2.90 per hit article (anchor is relatively weak). Three principled tuning options: conservative (keep em=4, raise strict 2→3, raise hr 1→1.5), **recommended** (lower em 4→3 to match strict, raise strict 2→3, raise hr 1→2), aggressive (further raise strict to 4, lower bq to 0.5). Goal is to balance "vote weight" across strong AI tells (em / emoji variety / strict) rather than let em dominate.

```markdown
# V06B 權重微調
* V06B 下 N 的意義
  * 不是乘數, 是 vote 分量
  * 命中就把整個 B_total 乘以更大的 N_sum
  * 強 vote → N 大, 弱 vote → N 小
* 當前問題
  * V01 em 貢獻 39.3%, 偏強
  * strict 跟 bq 差只 1
  * hr 跟 bq 平, 但 hr 更稀
* 數據基礎
  * N_sum contribution per signal
  * Ablation (設 N=0 的 base 下降)
  * emoji avg types = 2.90
* 推薦方案
  * V01 4→3 (降, 跟 strict 平)
  * strict 2→3 (升, 強 pattern)
  * hr 1→2 (升, AI 愛切章節)
  * bq 1 不動
  * all 0 不動
* 預期效果
  * 強 vote 平衡 (em/strict/hr)
  * JS 核心重構回升
  * 使用gemini 系列不獨佔
```

---

## 當前 V06B 問題 — Current Imbalance

### N_sum expected contribution per avg article

每 signal 對平均 N_sum 的貢獻 = N × coverage:

| Signal | coverage | N | 貢獻 | % |
|:---|---:|---:|---:|---:|
| V01 `——` | 30.2% | 4 | 1.21 | **39.3%** ← 最大 |
| V02 strict | 28.2% | 2 | 0.56 | 18.3% |
| V05 emoji | 16.9% | 2.90 (avg types) | 0.49 | 16.0% |
| V03 bq | 42.9% | 1 | 0.43 | 14.0% |
| V04 hr | 38.2% | 1 | 0.38 | 12.4% |
| V02 all | 68.2% | 0 | 0 | 0% |

全體 avg N_sum = 3.07, median N_sum = 2, max N_sum = 38.

### Ablation 測試

把某 signal N 設 0 (保留其他), 看 base 總和會下降多少 (= 該 signal 對 V06B 全體 base 的實質貢獻):

| Signal | 設 N=0 後 base 下降 | % |
|:---|---:|---:|
| V01 `——` | 610,032 | **31.8%** |
| V05 emoji | 550,119 | 28.6% |
| V02 strict | 358,842 | 18.7% |
| V03 bq | 207,673 | 10.8% |
| V04 hr | 194,344 | 10.1% |

### 問題診斷

1. **V01 em 權重 4 讓它主宰 N_sum (39.3%) 與 base (31.8%)**. 在 V06 下 em×4 乘以自己的 count 合理, 但 V06B 下 em 命中就 +4 整個 N_sum, 相對過強.
2. **V02 strict (N=2) 跟 V03 bq (N=1) 差只 1**. strict 是 clean AI tell, bq 有正當引言用途, 差距可以拉開.
3. **V04 hr (N=1) 跟 V03 bq (N=1) 平手**, 但 hr 比較明顯是 AI 排版 (切章節), coverage 也比 bq 稀 (38.2% vs 42.9%), 應該升.
4. **emoji avg types 只 2.90**, variety-driven 的 anchor 相對弱 (contribution 16%). 其他 N 不過度放大的話, emoji 自適應能發揮.

---

## V06B 下 N 的意義 — N as Vote Weight

V06B 跟 V06 的 N 意義完全不同, 這點是調參前要先搞清楚的:

| | V06 | V06B |
|:---|:---|:---|
| 公式 | `Σ(Bᵢ × Nᵢ)` | `B_total × N_sum` |
| N 的角色 | 該 signal count 的直接乘數 | 訊號 vote 的分量 + N_sum budget 開關 |
| N 大的意義 | 該 signal 的 B 多 → 爆分 | 該訊號命中 → 整個 B_total 被乘得更大 |
| 不同 signal 的 N 關係 | 線性 (各自獨立貢獻) | 相乘 (大家一起放大 B_total) |

從 vote 哲學看各 signal:

| Signal | Vote 強度 | 理由 |
|:---|:---|:---|
| V01 `——` | **強** | 中文幾乎不用, 強 AI tell |
| V05 emoji variety | **強** (自適應) | types 多樣 → vote 分量大 |
| V02 strict | **強** | AI 典型 pattern `- **label**:` |
| V04 hr | **中** | AI 愛切章節, 但長文人類也用 |
| V03 bq | 弱 | 有正當引言用途 |
| V02 all | **零** | 太 generic, 68% 文章都用 |

強 vote 應該 N 大, 弱 vote 應該 N 小. 零 vote (all) 已經是 0.

---

## 推薦方案與理由 — Recommendation

### 三個方案

| 方案 | V01 | V05 | V02 strict | V02 all | V03 bq | V04 hr |
|:---|:---:|:---:|:---:|:---:|:---:|:---:|
| 當前 (憑感覺) | 4 | types | 2 | 0 | 1 | 1 |
| 保守微調 | 4 | types | 3 | 0 | 1 | 1.5 |
| **推薦** | **3** | **types** | **3** | **0** | **1** | **2** |
| 激進 | 3 | types | 4 | 0 | 0.5 | 2 |

### 逐項理由

- **V01 em 4 → 3**: V06B 下 em 命中 +4 N_sum 偏強 (占 39.3%), 降到 3 跟 strict 平. 強 tell 之間不應該有一個獨霸, 這樣其他 vote 才有存在感
- **V02 strict 2 → 3**: 強 AI pattern vote, 升到跟 em 平. 兩個最強的 clean tell 分量相等
- **V04 hr 1 → 2**: AI 愛切章節是明確中強 tell. 比 bq 稀 (38.2% vs 42.9%), 升到 2 讓 hr 的 vote 比 bq 大
- **V03 bq 1 不動**: 弱 tell (有正當用途), 保持弱 vote
- **V02 all 0 不動**: generic 排版, 不給 vote
- **V05 emoji anchor (types) 不動**: variety 自適應強 tell. avg 2.90 已經偏弱, 其他 N 不要過度 inflate 讓 emoji 有存在空間

### 跟 V06 的推薦為什麼不同

[v06-weight-tuning.md](./v06-weight-tuning.md) 已經 obsolete (基於 all N=1 的錯誤前提). 但假設 V06 要重做, V06 推薦可能是「V01 不動 4, strict 2→3, hr 1→2」(保持 V01 強勢因為 V06 下 em×4 乘自己的 B 合理). V06B 這裡 V01 降到 3 的差別, 核心原因:

- V06 下 em count 跟 em N=4 的乘積是「em 自己的事」
- V06B 下 em 命中就 +4 整個 N_sum, 影響所有 B_total 的放大

**同樣的 N 值在 V06 跟 V06B 下有不同效果**, 不能照搬.

---

## 預期效果 — Expected Impact

套用推薦後預期:

1. **強 vote (em / strict / hr) 達到平衡**: N 落在 2-3 之間, 加上 emoji 自適應 (avg 2.9), 四個強 vote 大致平均
2. **使用gemini 準備 az-900 系列不會獨佔 top 10**: 這系列靠「所有 signal 命中 + 大量 all_bold」上榜, N 平衡後 em 不再是 kingmaker, 系列對 B_total 的強勢會被其他 signal 的 vote 稀釋
3. **JS 核心重構 系列可能回升**: 這系列 em 少 (但 em=0), strict/emoji variety 強. V01 降權 + strict 升權對它們有利
4. **「小豬的健康讓我來守護」可能也回升**: emoji variety 強 (9 types), hr 升權對它有一些幫助

但預期只是預期, 跑完才知道.

---

## 保守微調實測結果 — Conservative Tuning Applied (2026-10-01)

選了**保守方案**, 已套用到 `analyzeV06B.py`:

- V02 strict N: 2 → 3 ✓
- V04 hr N: 1 → 1.5 ✓
- 其他不動 (V01=4, V05=types, V02 all=0, V03 bq=1)

### 微調前後對比

| 指標 | 微調前 V06B | 微調後 V06B |
|:---|---:|---:|
| 全體 base 總和 | 1,921,010 | 2,197,603 (×1.14) |
| 全體 density | 48.47 | 55.45 (×1.14) |
| Spearman (前後) | — | **0.9984** |
| Top 10 交集 | — | 9/10 |
| **Top 20 交集** | — | **20/20** (完全一樣) |
| Top 50 交集 | — | 49/50 |

### 排名變化

Top 10 唯一變動: 「驗證使用 [AI 助教] 後的結果」從舊 top 11 → 新 top 10, 擠掉「使用gemini 準備 az-900 Day 4」(舊 top 10 → 新 top 12).

Top 20 完全一樣, 只有順序微變動.

### 為什麼排名變動這麼小

- 微調前 top 20 的文章幾乎都「所有 signal 都命中」, N_sum 的變化對他們影響一致 (都 +1 for strict, +0.5 for hr)
- V06B 下 top 排名由 **B_total 大 + 多 signal 命中** 主導, 單個 N ±1-2 的調整只是 scale shift
- 要大洗牌 top 排名可能需要**改變 N 的相對比例**, 例如激進方案的「V01 em 4→3 + strict 2→4」會讓 em-only vs strict-only 的差異反轉

### 下一步 (如果繼續)

- 保守微調已足夠作為「有依據」的 baseline, 不急著迭代
- 若未來要讓 V06B 排名跟 V06 更分化 → 試激進方案 (V01 4→3, strict 2→4, bq 1→0.5), 看能不能讓 V06B 自成一派
- 真正要影響排名的是 N 的**相對比例**, 不是絕對值
- V06B 跟 V06 新一輪對照見 [v06-vs-v06b.md](./v06-vs-v06b.md) (已更新)

---

## Related

- [analyzeV06B.py](./analyzeV06B.py) — V06B code, 要改的檔案
- [results-v06b.md](./results-v06b.md) — 當前結果, 比較基準
- [v06-vs-v06b.md](./v06-vs-v06b.md) — V06 vs V06B 對照 note (調完要更新)
- [v06-weight-tuning.md](./v06-weight-tuning.md) — V06 版本的權重分析 (obsolete, 因 all N=1 錯誤前提)
- [composite-score-v06.md](./composite-score-v06.md) — V06 原始設計
