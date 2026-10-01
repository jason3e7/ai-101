---
title: "AI 101 - 鐵人賽 Day 17: 從 1 個訊號擴到 6 個, 綜合分數再測「AI 味」"
tags: [ai, 鐵人賽, ironman, 文風檢測, 綜合分數, 草稿]
created: 2026-10-01
status: draft
---

# Day 17｜從 1 個訊號擴到 6 個, 綜合分數再測「AI 味」 — From One Signal to Six: A Composite Score

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 16](./day16-ai-fingerprint-scan.md) 只用一個訊號 (雙破折號 `——`) 掃出 15,057 篇, 發現 ChatGPT & Codex 組只 0.11 每千字, 這個指紋對 GPT 生態已經失效. 單一指標會被模型 upstream 壓掉, 這篇把指標疊到**綜合分數** (6 個 signal, 總量加權型, 公式 B 總和 × N 總和), 重跑一次排名, 看多指紋捕捉到的「AI 味」是什麼樣貌.

> **TL;DR (EN):** Day 16 ranked articles by a single signal (`——`); results showed the signal is dying for GPT-based writers because GPT-5.1 can suppress em-dash on request. This post stacks six signals (`——` × 4, emoji × distinct-types, strict-bold-list × 3, all-bold × 0, blockquote × 1, `<hr>` × 1.5) with the formula `base = (ΣB) × (ΣN of hit signals)`, then density per 1k chars. Across the same 15,057 articles, overall density jumps from 0.86 to 55.45. **Top 3 shifts completely**: Day 16's em-dash-heavy articles get displaced by markdown-formatting-heavy ones — 「使用gemini 準備 az-900」series takes #1/#4/#6/#10 (stacks every signal at scale). Different lens: single signal catches em-dash lovers, composite catches markdown-pile-everything users.

```markdown
# 從 1 個訊號擴到 6 個, 綜合分數
* 為什麼擴大 (承 Day 16, 單 signal 被壓就失效)
* 疊了什麼: 6 個 signal
  * 雙破折號, emoji 種類, 嚴格/一般粗體
  * blockquote, <hr>
* 公式
  * B 總和 × N 總和 (總量加權)
  * 一般粗體 N=0 當放大器
* 結果 (全體 density 55.45)
  * top 5 文章: 排版重度派
  * top 5 系列: gemini az-900 奪冠
* 跟 Day 16 (單 ——) 對比
  * top 3 完全不重疊
  * 單指紋 vs 綜合分數捕捉不同的「AI 味」
* 邊界
```

---

## 為什麼擴大 — Why Stack Signals

[Day 16](./day16-ai-fingerprint-scan.md) 的結論之一: **當初挑 `——` 是圖它最粗最好算, 但這個指紋對 GPT 生態已經在失效**. 2025-11 GPT-5.1 開始能遵守「不要用 em-dash」的 custom instruction, 用 ChatGPT/Codex 寫的作者天然沒訊號, 一個 signal 就塌一半.

單指紋還有別的盲區:

- **刻意壓痕跡的作者**: 用 Claude 寫但知道要去掉 `——`, 單一指標完全漏掉
- **愛用 `——` 的老派人類作者**: 單一指標誤判
- **用 AI 但只重排版的作者**: 不碰 `——` 但狂灑 emoji + 粗體 + 分段線, 單一指標完全漏掉

要補這些洞, 做法是**疊更多 signal**. 單一壞掉其他補得起. 這篇疊到**綜合分數** (6 個 signal, 總量加權型), 用同一批 2026-09 corpus (15,057 篇) 重跑排名.

---

## 疊了什麼 — Six Signals Stacked

從單指紋逐步加上五個 signal, 總共六個:

| Signal | 偵測 | 權重 N | 命中率 |
|:---|:---|---:|---:|
| **雙 em dash `——`** | 兩個 em dash 連在一起 (Day 16 已講) | 4 | 30.2% |
| **emoji** | emoji 數 × **distinct 種類** | types (平均 2.90) | 16.9% |
| **嚴格粗體 pattern** | `- **標籤**: 說明` | 3 | 28.2% |
| **一般粗體** | 任何 `**xxx**` | 0 | 68.2% |
| **blockquote** | `> ` 引用塊 | 1 | 42.9% |
| **水平線 `<hr>`** | `---` 分段 | 1.5 | 38.2% |

幾個設計重點:

- **emoji 的乘數是「種類數」不是「個數」**: 5 個相同 emoji (count=5, types=1) 分數 = 5; 5 個不同 emoji (count=5, types=5) 分數 = 25. 人類寫作通常固定幾個常用 emoji, AI 愛展示變化
- **嚴格 pattern 跟一般粗體分開算**: 嚴格 pattern 是一般粗體的子集, 命中嚴格 pattern 同時也算一般粗體 (加倍計, 總權重 3+0=3), 捕捉「`- **標籤**: 說明」這種最像 AI 的樣子
- **一般粗體權重 0**: 人類也常用, 不給 vote, 但 count 會進 B_total 當**痕跡放大器** (下一節解釋)
- **權重是 data-driven 調過的**: 初版 N 憑感覺, 2026-10-01 根據 corpus contribution 調整過 (嚴格 pattern 2→3, `<hr>` 1→1.5)
- **emoji 排除清單**: `○ ✗ ★ ☆ ☐` 這些 checklist / 星等排版符號人類也常用, 不算

---

## 公式: B 總和 × N 總和 — The Composite Formula

```
B_total = em + emoji + strict + all + bq + hr                ← 命中總次數
N_sum   = Σ (N where that signal fires)
        = (4 if em>0) + (types if emoji>0) + (3 if strict>0)
          + (0 if all>0) + (1 if bq>0) + (1.5 if hr>0)
base    = B_total × N_sum
density = base / 總字數 × 1000                               ← 排名主指標
```

**一般粗體 N=0 當放大器**: all count 本身不給 vote (N=0), 但 count 進 B_total, 把全盤放大 N_sum 倍. 這是刻意的設計 — 粗體排版重度 (就算每項權重 0) 也會放大其他 signal 的影響.

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

## 結果: 綜合分數排行 — The Rankings

抓取時間 2026-09-30. 同樣掃出 814 系列、15,057 篇文章.

### 整體數字

| 項目 | 數值 |
|:---|---:|
| base 總和 | 2,197,603 |
| 全篇總字數 | 39,634,634 |
| **全體 density** | **55.45** |

Day 16 單 signal 全體 0.86 → 綜合分數 55.45, 數字差 60 倍 (因為加了 6 個 signal 的 B 乘以 N_sum, scale 完全不同). **scale 不是重點, 排名才是**.

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

---

## 跟 Day 16 單 `——` 比: top 3 完全不重疊 — Different Lens, Different Top

| 排名 | Day 16 (單 `——` 指紋) | Day 17 (綜合分數 6 signal) |
|:---:|:---|:---|
| 1 | Day01 Claude 從零打造軟體產品 (17.51) | 使用gemini 準備 az-900 Day 6 (1141.26) |
| 2 | 白稜 Day 12 流程再造 (14.29) | Phoenix 2026 Day 12 (1100.59) |
| 3 | mpv-lazy Day25 (13.49) | JS 核心重構 Day 02 (1077.31) |

**兩套 top 3 完全不重疊**. 不是誰對誰錯, 是**兩個指標在量不同的「AI 味」**:

- **Day 16 top 3 共通點**: 短文 + `——` 用得兇. 單一 tell 強, 其他 signal 平淡
- **Day 17 top 3 共通點**: 排版重度 (每篇都塞 emoji + 粗體 + 分段 + 引用), 不一定愛 `——`
- 「使用gemini 準備 az-900 Day 6」: `——` 只有 3, 但 emoji 49 (30 種)、一般粗體 298、嚴格 pattern 111、`<hr>` 22 — 典型「什麼都疊」. 綜合分數爆分, 單指紋排行外
- 「Day01 Claude 從零打造軟體產品」: `——` 密度 17.51 全屆第 1, 但其他 signal 不特別, 綜合分數排名掉到幾十名後

單一指紋捕捉的是**某種** AI 味 (em-dash 愛好者), 綜合指紋捕捉的是**另一種** (markdown 排版 maximalist). **兩套互補, 不是取代**. Day 16 的工具揭露了 GPT 生態的指紋失效, Day 17 的工具揭露了另一群 upstream 沒壓的「排版重度」作者.

---

## 邊界與限制 — Caveats

- **權重是人工調的**: N 用 data-driven 分析調過 (contribution + ablation), 但終究是人設的, 不是 ground truth
- **「排版重度」不等於 AI 寫的**: 有些作者 (特別是教學類) 本來就愛塞 emoji + 粗體, 不代表 AI 代筆. 這個指標只用來排序, 不做判定
- **系列密度用「加總再除」**: 用字數當權重, 跟平均各篇密度不一樣 (細節同 Day 16)
- 其他邊界 (只算 `——`、算整篇含標題與程式碼、不設字數門檻、不 doxx) 同 [Day 16](./day16-ai-fingerprint-scan.md)

---

## 我的重點 — Takeaways

- 單一指紋會被 upstream 工具壓掉 (Day 16 的 `——` 對 GPT 生態已失效), 疊 signal 是必要的補救
- 綜合分數**跟 Day 16 top 3 完全不重疊** — 不是誰對誰錯, 是在量不同面相的「AI 味」(em-dash 愛好者 vs markdown 排版 maximalist)
- 「排版重度」是另一條訊號線: 看到有人每篇都塞 emoji + 粗體 + 分段, 不是 AI 代筆的鐵證, 但值得多看一眼

---

## Sources

- [Day 16: 掃當屆所有文章, 量一次「AI 味」有多少](https://ithelp.ithome.com.tw/articles/10419414) — 單 `——` 掃全屆
- [Ghostbuster: Detecting Text Ghostwritten by Large Language Models (Verma et al., NAACL 2024)](https://arxiv.org/abs/2305.15047) — 多 feature 綜合計分, 跟本篇的 (ΣB)×(ΣN) 同一思路
- [Spotting LLMs With Binoculars: Zero-Shot Detection of Machine-Generated Text (Hans et al., ICML 2024)](https://arxiv.org/abs/2401.12070) — 不同算法 (perplexity ratio) 但同一題
- [Contrasting Linguistic Patterns in Human and LLM-Generated News Text (Muñoz-Ortiz & Gómez-Rodríguez, 2024)](https://arxiv.org/abs/2308.09067) — 統計上找人 vs LLM 的語言 pattern 差異
