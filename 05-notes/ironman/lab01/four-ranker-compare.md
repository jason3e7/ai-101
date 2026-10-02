---
title: lab01 四 ranker 對照 — V06B / V07 / V08C / V09 捕捉不同維度的 AI 味
created: 2026-10-02
---

# 四 Ranker 對照 — V06B / V07 / V08C / V09

> [!NOTE]
> V06B (排版綜合分數), V07 (「最...的」加權), V08C (「不是/不只是…而是」), V09 (「最容易...的」) 這四個 ranker 都在排 15,057 篇鐵人賽文章的「AI 味」. 直覺上以為它們多少會重疊 — 排名靠前的應該是「大家公認最 AI」. 跑完才知道**幾乎完全正交**: top 20 聯集 79/80 (近乎沒重疊), 四 ranker 都在 top 1000 的交集是 0 篇. 這篇用數據固化這個發現, 討論意涵.

> **TL;DR (EN):** Compared four AI-smell rankers on the same 15,057-article Ironman corpus. V06B measures markdown-formatting load (bold / hr / blockquote / emoji variety). V07 ranks by weighted "most X" superlatives. V08C ranks by "不是/不只是 X 而是 Y" dichotomy. V09 ranks by "最容易 X 的" teaching-warning phrase family. **The four rankers almost never agree**: top-20 union is 79/80 (so top 20s are nearly disjoint), four-way intersection at top 1000 is zero, even three prose-focused rankers (V07+V08C+V09) have zero top-50 overlap. Spearman cross-correlations range 0.10–0.45. This means AI "smell" isn't a single dimension — different authors/models/topics express different patterns, and a composite needs OR aggregation not AND filtering.

```markdown
# 四 ranker 捕捉不同維度
* 四 ranker 回顧
  * V06B 排版綜合 (B×N)
  * V07 「最...的」加權
  * V08C 「不是/不只是…而是」
  * V09 「最容易...的」
* 全 corpus Spearman
  * 彼此 0.10 - 0.45 中低相關
  * V08C vs V09 = 0.45 最高
  * V06B vs V08C = 0.10 最低
* Top N 交集近乎 0
  * top 20 聯集 79/80
  * 四 ranker 都中 top 1000 = 0 篇
  * 需要 top 2000 才有 20 篇
* 意涵
  * AI 味不是單一向度
  * 不同 ranker 抓不同作者/pattern
  * 複合用 OR 不能用 AND
```

---

## 四個 Ranker 回顧

| Ranker | 捕捉的 pattern | 公式 | 全體 density | 覆蓋率 |
|:---:|:---|:---|---:|---:|
| **V06B** | markdown 排版重度 (bold / hr / bq / emoji variety) | B_total × N_sum (微調後) | 55.45 | ~100% |
| **V07** | 「最...的」加權 superlative (4-7 字, 排除 3 字自然詞) | hit_sum × distinct | 2.74 | 67% |
| **V08C** | 「不是/不只是…而是」對立句 | per_1k 直接頻率 | 0.28 | 35% |
| **V09** | 「最容易...的」教學警告腔 | per_1k 直接頻率 | 0.03 | 7% |

- V06B 是**排版 level** AI 味
- V07/V08C/V09 都是**文字 level** AI tell, 稀有度一個比一個高

---

## 全 corpus Spearman 相關

| | V06B | V07 | V08C | V09 |
|:---|---:|---:|---:|---:|
| V06B | — | 0.19 | **0.10** | 0.13 |
| V07 | 0.19 | — | 0.25 | 0.31 |
| V08C | 0.10 | 0.25 | — | **0.45** |
| V09 | 0.13 | 0.31 | 0.45 | — |

- 最高: V08C vs V09 = **0.45** (都 clean prose AI tell)
- 最低: V06B vs V08C = **0.10** (排版 vs 文字 完全不同)
- V07 vs 其他 = 0.19-0.31 (中度, 因為「最...的」中性詞偏多)

**全部都在 0.10-0.45 的中低相關區間**, 沒有一對高相關.

---

## Top N 交集 (兩兩 + 全 4)

### Top 20 交集 (兩兩)

| | V06B | V07 | V08C | V09 |
|:---:|---:|---:|---:|---:|
| V06B | 20 | 0 | 0 | 0 |
| V07 | | 20 | 1 | 0 |
| V08C | | | 20 | 0 |
| V09 | | | | 20 |

**只有 V07 vs V08C 有 1 篇交集** — 其他全是 0.

### Top N 四 ranker 交集 (都在 top)

| N | 四 ranker 都在 | 三 prose ranker 都在 (V07+V08C+V09) |
|---:|---:|---:|
| 10 | 0 | 0 |
| 20 | 0 | 0 |
| 50 | 0 | 0 |
| 100 | 0 | 0 |
| 500 | 0 | 5 |
| 1000 | 0 | 33 |
| **2000** | **20** | 111 |

要擴到 **top 2000** 才有 20 篇文章四 ranker 都中.

### Top 20 聯集

四 ranker 的 top 20 聯集 = **79 / 80** (若完全無重疊 = 80, 完全重疊 = 20). 幾乎所有進榜文章都是**獨家進榜**.

---

## 各 Ranker Top 10 (獨家無交集確認)

### V06B top 10 (排版型, Azure cert + JS 核心重構)
1. 使用gemini 準備 az-900 DAY 6
2. Day 12 你敢不敢承認 (Phoenix 2026)
3. Day 02 清點魔法物資 (JS 核心重構)
4. 使用gemini 準備 az-900 DAY 1
5. Day 03 箭頭函式 (JS 核心重構)
6. 使用gemini 準備AZ-900 Day21
7. Day 27 戰略指揮 (JS 核心重構)
8. 我想像中的未來小豬 (前端三分鐘)
9. Day 26 效能神兵 (JS 核心重構)
10. 驗證使用 AI 助教

### V07 top 10 (「最...的」加權, 教學總結腔)
1. Day 4 為什麼復原要從最後一步開始 (資料結構)
2. Day 4 取消訂閱的道別藝術 (UI 心理學)
3. Day 29 如果今天全部重來 (AI 管理健康)
4. MySQL/PostgreSQL 弱密碼攻擊
5. Day 11 一句 Prompt 改 UI (咖啡 Wi-Fi AI)
6. Day 20 IT 診斷模型多大
7. Day 4 鑄劍 (OSCP)
8. Day 2 FUDAT 框架
9. Day 30 教老人用 Google AI (← **跟 V08C top 5 重複!**)
10. Day 21 量化交易

### V08C top 10 (對立句, 短文作者 register)
1-2. 菜雞學習資料結構 Day 5 (兩系列重複)
3. Day 1 教老人用 Google AI
4. Day1 2026鐵人賽
5. Day 30 教老人用 Google AI (← **跟 V07 #9 重複!**)
6. Day 4 架構原則
7. Day 5 好的架構共同特質
8. 有 AI 菜有辦法 Day 25
9. Day 04 VoCare
10. Day 06 網管黑手到資安長

### V09 top 10 (「最容易...的」教學警告腔)
1. Day 2 左手不能打字 (教老人用 Google AI)
2. Day 28 Claude AI 新手入門
3. 關於我開始踏入資安 Day28
4. Day 8 JSX ≠ HTML
5. Day 14 五種設定各管什麼 (文科生 Claude Code)
6. Day 10 事實查核 (用 AI 打鐵人賽)
7. 執行計劃 AI 做 CI/CD
8. Day2 哪款 AI 適合孩子
9. Day 7 AI 找論文先確認
10. Day 12 React Props/State

**唯一一篇 top 10 交集**: 「Day 30 從不敢打開，到能陪另一個人按一次」(教老人用 Google AI 系列), V07 排 9, V08C 排 5. 該篇同時用大量 superlative + 大量對立句 — 真正的雙 signal 命中.

**V09 top 1「Day 2 左手不能打字」也是「教老人用 Google AI」系列**. 這系列是 V08C + V09 都愛的「教學總結腔」典型.

---

## 關鍵觀察 — 為什麼幾乎零 overlap

1. **四 ranker 衡量的「AI 味」真的不是同一件事**
   - V06B: 排版重度 (堆料式 markdown)
   - V07: 愛用 superlative 強調
   - V08C: 愛用對立句修正誤解
   - V09: 愛用「最容易...的」教學警告
   - 這些是**不同作者 / 不同主題 / 不同 AI 模型**的輸出偏好, 不是同一群人的共同特徵

2. **單一 AI 文章只會強烈踩中其中一兩個 pattern**
   - Azure cert 系列重排版但文字不堆對立句 → V06B 爆榜, 其他 0
   - 教老人用 AI 系列重教學總結但排版不花俏 → V08C/V09 爆榜, V06B 普通
   - UI 心理學文章重 superlative 但不切章節 → V07 爆榜, V06B 普通

3. **AND filter 幾乎必失效**: 若要「四 ranker 都中 top 100 才算高信心 AI」會抓到 0 篇. 要 **top 2000 才 20 篇**, 這個門檻幾乎等於「全 corpus 都有可能」

4. **OR aggregation 比較合理**: 「某 ranker top 100 就算 high density」, union 起來能涵蓋多種 AI 風格, 不至於漏掉只踩單一 pattern 的作者

---

## 使用建議

### 定位

| Ranker | 最適合用來找 | 不適合用來找 |
|:---:|:---|:---|
| V06B | 「markdown 堆料」的教材/cert 類文章 | 文字層面的 AI slop |
| V07 | 愛用強調語氣的教學/總結腔 | 排版粗糙但文字乾淨的 AI |
| V08C | 愛用對立句修正誤解的 framing 腔 | 不用對立句的 AI |
| V09 | 「最容易出錯的 / 最容易被忽略的」典型警告腔 | 其他風格的 AI tell |

### 複合用法

不要用 AND (「四 ranker 都 top N」) → 太嚴格, 幾乎零命中.

**用 OR 擴大召回, 用「中幾個 ranker」做分級**:
- 中 1 ranker: 單一 pattern 可疑
- 中 2 ranker: 高信心
- 中 3+ ranker: 幾乎確定

或**用加權總分**: 把每個 ranker 的 percentile rank 乘上信念權重 (V08C/V09 權重較高 — pattern 更乾淨) 再加總.

### 下一步可能

- **寫 V10 composite ranker**: 讀 4 份 CSV, 算每篇在 4 個 ranker 的 percentile, 合成總分. 跟 V06 那種單 corpus 綜合不同, 這是跨 ranker 的 meta-composite.
- **深看「教老人用 Google AI」系列**: V08C/V09 都夾擊, V07 也進榜. 可能是乾淨的「AI 教學腔」範本系列
- **深看 V06B 的 Azure cert 系列**: 為什麼排版爆分但文字無 cliche? 可能是「cert 教材」有特殊 register, 不代表 AI
- **做負面對照**: 2020-2021 pre-ChatGPT 的鐵人賽文章, 這四 ranker 的分佈應該要比 2026 低很多

---

## Related

- [analyzeV06B.py](./analyzeV06B.py) + [results-v06b.md](./results-v06b.md) — 排版 ranker
- [analyzeV07.py](./analyzeV07.py) + [results-v07.md](./results-v07.md) — 「最...的」加權
- [analyzeV08C.py](./analyzeV08C.py) + [results-v08c.md](./results-v08c.md) — 「不是/不只是…而是」
- [analyzeV09.py](./analyzeV09.py) + [results-v09.md](./results-v09.md) — 「最容易...的」
- [v06-vs-v06b.md](./v06-vs-v06b.md) — V06 vs V06B 兩種綜合算法對照
- [composite-score.md](./composite-score.md) — percentile rank 複合分數的設計 (未實作, 這篇可當它的證據基礎)
