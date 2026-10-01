---
title: "AI 101 - 鐵人賽 Day 17: 從 1 個訊號擴到 6 個, V06B 綜合分數再測「AI 味」"
tags: [ai, 鐵人賽, ironman, 文風檢測, 綜合分數, V06B, 草稿]
created: 2026-10-01
status: draft
---

# Day 17｜從 1 個訊號擴到 6 個, V06B 綜合分數再測「AI 味」 — From One Signal to Six: V06B Composite Score

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 16](./day16-ai-fingerprint-scan.md) 只用一個訊號 (雙破折號 `——`) 掃出 15,057 篇, 發現 ChatGPT & Codex 組只 0.11 每千字, 這個指紋對 GPT 生態已經失效. 單一指標會被模型 upstream 壓掉, 這篇把 lab01 疊到 **V06B 綜合分數** (6 個 signal, B 總和 × N 總和算法), 重跑一次排名, 看多指紋捕捉到的「AI 味」是什麼樣貌.

> **TL;DR (EN):** Day 16 ranked articles by a single signal (`——`); results showed the signal is dying for GPT-based writers because GPT-5.1 can suppress em-dash on request. This post stacks six signals (`——` × 4, emoji × distinct-types, strict-bold-list × 3, all-bold × 0, blockquote × 1, `<hr>` × 1.5) and combines them with V06B formula: `base = (ΣB) × (ΣN of hit signals)`, then density per 1k chars. Across the same 15,057 articles, overall density jumps from 0.86 to 55.45. **Top 3 shifts completely**: Day 16's em-dash-heavy articles get displaced by markdown-formatting-heavy articles — 「使用gemini 準備 az-900」series takes #1/#4/#6/#10 (stacks every signal at scale). Series rankings: JS 核心重構 keeps #1 (overlap with Day 16), gemini-az-900 jumps to #2 (new winner), Phoenix 2026 at #3. Different formula captures different notion of AI-smell: single tell catches em-dash lovers, composite catches markdown-pile-everything users. Both ship as parallel scanners in `lab01/`.

```markdown
# 從 1 個訊號擴到 6 個, V06B 綜合分數
* 為什麼擴大 (承 Day 16, 單 signal 被壓就失效)
* 疊了什麼: 6 個 signal
  * V01 ——, V05 emoji 種類, V02 strict/all 粗體
  * V03 blockquote, V04 <hr>
* V06B 公式
  * B 總和 × N 總和
  * 跟 V06 Σ(B×N) 的差別
* 結果 (全體 density 55.45)
  * top 5 文章: 排版重度派
  * top 5 系列: gemini az-900 奪冠
* 跟 Day 16 (單 ——) 對比
  * top 3 完全不重疊
  * 兩個算法捕捉不同的「AI 味」
* 邊界
```

---

## 為什麼擴大 — Why Stack Signals

[Day 16](./day16-ai-fingerprint-scan.md) 的結論之一: **當初挑 `——` 是圖它最粗最好算, 但這個指紋對 GPT 生態已經在失效**. 2025-11 GPT-5.1 開始能遵守「不要用 em-dash」的 custom instruction, 用 ChatGPT/Codex 寫的作者天然沒訊號, 一個 signal 就塌一半.

單指紋還有別的盲區:

- **刻意壓痕跡的作者**: 用 Claude 寫但知道要去掉 `——`, 單一指標完全漏掉
- **愛用 `——` 的老派人類作者**: 單一指標誤判
- **用 AI 但只重排版的作者**: 不碰 `——` 但狂灑 emoji + 粗體 + 分段線, 單一指標完全漏掉

要補這些洞, 做法是**疊更多 signal**. 單一壞掉其他補得起. 這篇講 lab01 疊到的 **V06B 綜合分數** (6 個 signal, 總量加權型), 用同一批 2026-09 corpus (15,057 篇) 重跑排名.

---

## 疊了什麼 — Six Signals Stacked

從 V01 到 V05, 各加一個 signal:

| Signal | 偵測 | 權重 N (V06B) | 命中率 |
|:---|:---|---:|---:|
| **V01 `——`** | 雙 em dash (Day 16 已講) | 4 | 30.2% |
| **V05 emoji** | emoji 數 × **distinct 種類** | types (平均 2.90) | 16.9% |
| **V02 strict** | `- **標籤**: 說明` 嚴格 pattern | 3 | 28.2% |
| **V02 all** | 一般粗體 `**xxx**` | 0 | 68.2% |
| **V03 blockquote** | `> ` 引用塊 | 1 | 42.9% |
| **V04 `<hr>`** | 水平分段 `---` | 1.5 | 38.2% |

幾個設計重點:

- **emoji 的乘數是「種類數」不是「個數」**: 5 個相同 emoji (count=5, types=1) 分數 = 5; 5 個不同 emoji (count=5, types=5) 分數 = 25. 人類寫作通常固定幾個常用 emoji, AI 愛展示變化
- **V02 strict 跟 all 分開**: strict 是 all 的子集, 命中 strict 同時也算 all. strict 加倍計 (權重 3+0=3), 捕捉「`- **標籤**: 說明」這種最像 AI 的 pattern
- **V02 all 權重 0**: 一般粗體人類也常用, 不給 vote, 但 count 會進 B_total 當**痕跡放大器** (下一節解釋)
- **權重是 data-driven 調過的**: 原本 V06 的 N 是憑感覺, 2026-10-01 根據 corpus contribution 調整過 (strict 2→3, hr 1→1.5), 詳見 [v06b-weight-tuning.md](../../lab01/v06b-weight-tuning.md)
- **emoji 排除清單**: `○ ✗ ★ ☆ ☐` 這些 checklist / 星等排版符號人類也常用, 不算

---

## V06B 公式: B 總和 × N 總和 — The V06B Formula

```
B_total = em + emoji + strict + all + bq + hr                ← 命中總次數
N_sum   = Σ (N where that signal fires)
        = (4 if em>0) + (types if emoji>0) + (3 if strict>0)
          + (0 if all>0) + (1 if bq>0) + (1.5 if hr>0)
base    = B_total × N_sum
density = base / 總字數 × 1000                               ← 排名主指標
```

**跟 V06 (`Σ(Bᵢ × Nᵢ)` 每 signal 各自 B×N 再加) 的差別**:

- V02 all 的 N=0 → V06 下 all 完全消失, V06B 下 all 的 count 進 B_total 當放大器
- 直覺差別: V06 是「選擇性加權」(弱 signal 消音), V06B 是「總量加權」(弱 signal 當放大器)
- 兩個 Spearman 排名相關 0.95 (高相關), 但 top 10 交集只 3/10, 細節洗牌劇烈
- **這篇用 V06B**, V06 作姊妹算法並存. 對照細節見 [v06-vs-v06b.md](../../lab01/v06-vs-v06b.md)

舉例: 「使用gemini 準備 az-900 Day 21」:

```
em=20, emoji=101(25 種), strict=170, all=248, bq=15, hr=21
B_total = 575
N_sum   = 4 + 25 + 3 + 0 + 1 + 1.5 = 34.5
base    = 575 × 34.5 = 19,837.5
density = 19,837.5 / 19,693 × 1000 ≈ 1007 (全體 top 6)
```

all=248 本身 N=0 不給 vote, 但 248 這個數字進 B_total 把全盤放大 34.5 倍.

---

## 結果: V06B 排行 — The V06B Rankings

抓取時間 2026-09-30. 同樣掃出 814 系列、15,057 篇文章.

### 整體數字

| 項目 | 數值 |
|:---|---:|
| base 總和 | 2,197,603 |
| 全篇總字數 | 39,634,634 |
| **全體 density** | **55.45** |

Day 16 單 signal 全體 0.86 → V06B 55.45, 數字差 60 倍 (因為加了 6 個 signal 的 B 乘以 N_sum, scale 完全不同). **scale 不是重點, 排名才是**.

### density 最高的 5 篇

| # | density | B_total | N_sum | 文章 | 系列 |
|---:|---:|---:|---:|:---|:---|
| 1 | 1141.26 | 496 | 39.5 | [Azure Advisor & Service Health Day 6](https://ithelp.ithome.com.tw/articles/10405969) | 使用gemini 準備 az-900 |
| 2 | 1100.59 | 149 | 27.5 | [Day 12: 你敢不敢承認你只優化了自己那一段?](https://ithelp.ithome.com.tw/articles/10401963) | Phoenix 2026 (DevOps RPG) |
| 3 | 1077.31 | 97 | 25.5 | [Day 02: 清點魔法物資](https://ithelp.ithome.com.tw/articles/10401261) | JS 核心重構: 勇者轉職傳說 |
| 4 | 1008.95 | 287 | 26.5 | [使用gemini 準備 az-900 Day 1](https://ithelp.ithome.com.tw/articles/10404896) | 使用gemini 準備 az-900 |
| 5 | 1008.15 | 106 | 24.5 | [Day 03: 極速短咒 箭頭函式](https://ithelp.ithome.com.tw/articles/10401425) | JS 核心重構: 勇者轉職傳說 |

### density 最高的 5 個系列 (加總再除)

| # | density | 篇數 | 系列 |
|---:|---:|---:|:---|
| 1 | 765.79 | 31 | JS 核心重構: 勇者轉職傳說 |
| 2 | 656.47 | 30 | 使用gemini 準備 az-900 |
| 3 | 489.14 | 30 | Phoenix 2026 (DevOps RPG) |
| 4 | 487.02 | 30 | 槍林彈雨下的資安防守 |
| 5 | 408.43 | 29 | OpenShift AI 簡易入門 30 天 |

完整排行 (20 篇文章、20 系列) 見 [results-v06b.md](../../lab01/results-v06b.md).

---

## 跟 Day 16 單 `——` 比: top 3 完全不重疊 — Different Lens, Different Top

| 排名 | Day 16 (V01 單 `——`) | Day 17 (V06B 6 signal) |
|:---:|:---|:---|
| 1 | Day01 Claude 從零打造軟體產品 (17.51) | 使用gemini 準備 az-900 Day 6 (1141.26) |
| 2 | 白稜 Day 12 流程再造 (14.29) | Phoenix 2026 Day 12 (1100.59) |
| 3 | mpv-lazy Day25 (13.49) | JS 核心重構 Day 02 (1077.31) |

**兩套 top 3 完全不重疊**. 不是誰對誰錯, 是**兩套算法在量不同的「AI 味」**:

- **Day 16 top 3 共通點**: 短文 + `——` 用得兇. 單一 tell 強, 其他 signal 平淡
- **Day 17 top 3 共通點**: 排版重度 (每篇都塞 emoji + 粗體 + 分段 + 引用), 不一定愛 `——`
- 「使用gemini 準備 az-900 Day 6」: `——` 只有 3, 但 emoji 49 (30 種)、all 粗體 298、strict 111、hr 22 — 典型「什麼都疊」. V06B 爆分, V01 排行外
- 「Day01 Claude 從零打造軟體產品」: `——` 密度 17.51 全屆第 1, 但其他 signal 不特別, V06B 排名掉到幾十名後

單一指紋捕捉的是**某種** AI 味 (em-dash 愛好者), 綜合指紋捕捉的是**另一種** (markdown 排版 maximalist). **兩套互補, 不是取代**. Day 16 的工具揭露了 GPT 生態的指紋失效, Day 17 的工具揭露了另一群 upstream 沒壓的「排版重度」作者.

---

## 邊界與限制 — Caveats

- **權重是人工調的**: V06B 的 N 用 data-driven 分析調過 (contribution + ablation), 但終究是人設的, 不是 ground truth
- **「排版重度」不等於 AI 寫的**: 有些作者 (特別是教學類) 本來就愛塞 emoji + 粗體, 不代表 AI 代筆. 這個指標只用來排序, 不做判定
- **系列密度用「加總再除」**: 用字數當權重, 跟平均各篇密度不一樣 (細節同 Day 16)
- **V06 vs V06B 選用**: 想抓**乾淨 AI tell** 用 V06; 想抓**排版 maximalist** 用 V06B. 細節見 [v06-vs-v06b.md](../../lab01/v06-vs-v06b.md)
- 其他邊界 (只算 `——`、算整篇含標題與程式碼、不設字數門檻、不 doxx) 同 [Day 16](./day16-ai-fingerprint-scan.md)

---

## 我的重點 — Takeaways

- 單一指紋會被 upstream 工具壓掉 (Day 16 的 `——` 對 GPT 生態已失效), 疊 signal 是必要的補救
- V06B 的 `(ΣB) × (ΣN)` 公式讓**連弱 signal (V02 all N=0) 都當放大器**, 跟 V06 選擇性加權是不同哲學
- 兩套算法**跟 Day 16 top 3 完全不重疊** — 不是誰對誰錯, 是在量不同面相的「AI 味」(em-dash 愛好者 vs markdown 排版 maximalist)
- 系列層級更穩: JS 核心重構 兩套算法都 top 3 (強重疊), 說明有穩定的「排版 pattern」
- 「使用gemini 準備 az-900」整系列從 V06 排 58 直衝 V06B 排名 1/4/6/10, 顯示**連綴性排版習慣**會系列性重現 — 一次校準的作者, 整系列都在同一個峰值

---

## Sources

- [results-v06b.md](../../lab01/results-v06b.md) — V06B 完整排行 (20 篇文章 + 20 系列)
- [v06-vs-v06b.md](../../lab01/v06-vs-v06b.md) — V06 vs V06B 兩算法對照
- [composite-score-v06.md](../../lab01/composite-score-v06.md) — V06 原始設計 (姊妹算法)
- [v06b-weight-tuning.md](../../lab01/v06b-weight-tuning.md) — V06B N 權重 data-driven 微調
- [signal-experiments.md](../../lab01/signal-experiments.md) — V01-V05 各 signal 的實驗紀錄
- [Day 16: 掃當屆所有文章, 量一次「AI 味」有多少](https://ithelp.ithome.com.tw/articles/10419414) — 單 `——` 掃全屆
