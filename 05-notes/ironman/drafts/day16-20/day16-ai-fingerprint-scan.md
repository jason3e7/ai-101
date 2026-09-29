---
title: "AI 101 - 鐵人賽 Day 16: 掃當屆所有文章, 量一次「AI 味」有多少"
tags: [ai, 鐵人賽, ironman, 文風檢測, 雙破折號, 實測, 草稿]
created: 2026-09-29
status: draft
---

# Day 16｜掃當屆所有文章, 量一次「AI 味」有多少 — Scanning This Year's Ironman for the AI Fingerprint

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 11](../day11-15/day11-verify-ai-output.md) 到 [Day 15](../day11-15/day15-expand-yourself.md) 講的都是我自己在跟 AI 產出搏鬥時累積出來的心法. 這篇把體感換成數字: 掃當屆 (2026) 鐵人賽已發布的公開文章, 用最粗最粗的機械指標 (雙破折號 `——` 密度) 量一次 AI 味, 給後續更細的 lab 一條基準線.

> **TL;DR (EN):** All 15 days so far have been my own subjective experience editing AI-drafted posts — building mindmap skills so a human can eyeball AI output fast (Day 12) and diagnosing verification fatigue (Day 13) came directly out of that exhaustion. This post turns the anecdote into numbers: scan every public article in the 2026 iThome Ironman contest and measure a single mechanical AI-fingerprint signal — the density of the double em dash `——` per thousand characters, computed over the full article (body + titles + code blocks). Chinese writers almost never use `——`; LLMs (Claude / GPT, unless actively suppressed) love it. Method: enrol via `signup/list`, list articles via each series' RSS, fetch every article page (RSS strips punctuation so it's useless for counting), then `hits / total_chars`. Output is per-article rows (`articles.csv`) plus aggregate stats (`series-summary.csv`, `results.md`). Result is [待實測]. Co-occurrence signal, not a verdict — high density doesn't prove AI, low density doesn't prove human.

```markdown
# 掃當屆鐵人賽, 量一次 AI 味有多少
* 為什麼講這個 (15 天個人厭世 → 想拿數據看)
* 挑最強指紋: 雙破折號 ——
  * 中文幾乎不用, AI 極愛用
  * 是共現訊號, 不是判決
* 方法: 抓、算、排
  * 名單: signup/list + RSS
  * 內文: 一律走文章頁 (RSS 濾標點)
  * 密度 = 命中 / 字數 (算整篇: 正文 + 標題 + code)
  * 輸出: 每篇一列 + 統計匯總
* 結果: 密度分佈與排行 (待實測)
* 邊界: 不是 AI 判定, 只是排序訊號
```

---

## 為什麼講這個 — Why This Matters

原話 (jason3e7):

> 在用 Claude 寫鐵人賽的過程, 一直要修正 AI 產出不通順、不自然的大量內容, 非常厭世, 所以催生了 Day 12 和 Day 13. 接下來也想分析整個鐵人賽到目前為止的文章, AI 明顯的痕跡有多少, 和對未來的推測.

展開來講:

- [Day 12](https://ithelp.ithome.com.tw/articles/10417425) 心智清單 skill: 為了讓自己 (人類) 一眼看完 AI 寫的長段落而催生的兩個 skill
- [Day 13](https://ithelp.ithome.com.tw/articles/10417978) 驗證疲勞: 每篇都要驗真的太累, 動筆時已經半崩潰

問題是: 這 15 天全是**我一個人的體感**. 有沒有辦法**拿數據看**大家實際的樣子?

想法很直接: 掃這一屆鐵人賽已發布的所有公開文章, 用最粗的機械指標算一次「AI 味濃度」. 這篇講: **雙破折號 `——` 密度**.

---

## 挑最強指紋: 雙破折號 `——` — The Cleanest Fingerprint

**為什麼從 `——` 開始**:

- 中文寫作**幾乎不用 `——`** (至少不會密集出現), 但 AI (Claude / GPT 未特別壓制時) 極愛用它做插敘、下定義、切節奏
- 純字元計數, 不用 NLP model, 最容易驗證跟複製
- 是「一眼可辨」的 tell, 對照概念見 [ai-writing-style-tells](../../../../02-advanced/writing-style/ai-writing-style-tells.md)
- 從最容易的訊號開始, 拿到 pipeline 骨架再堆更複雜的 signal

> [!IMPORTANT]
> **這是共現訊號, 不是判決**. 高密度不等於 AI 寫的, 低密度也不等於人寫的. 有些老派作者本來就愛用 `——`, 也有人用 AI 但已經把痕跡壓乾淨. 這個指標只用來**排序**, 不用來**判定個案**.

---

## 方法: 抓、算、排 — Fetch, Count, Rank

三步, 拆給看得懂 Python 就會做:

### 1. 列名單: signup/list + RSS

- 官方 `/2026ironman/signup/list?group=...` → 每一組的全部系列, 系列數要對得上「報名數」
- 每個系列的 RSS `/rss/series/{id}` → 該系列的文章清單
- 對帳: RSS 篇數對不上報名頁的進度 `DAY N`, 就讀文章頁的「共 N 篇」補抓, 差多少列在 `raw/completeness.json`

### 2. 抓內文: 一律走文章頁

RSS 雖然附全文, 但**濾掉部分標點**: 同一篇 RSS 裡 `—` 出現 0 次, 文章頁 20 次. 拿 RSS 算會讓每篇密度都變 0. 所以 **RSS 只用來列文章清單, 內文一律以文章頁為準**.

### 3. 算密度: 命中 / 字數

```
密度 = 命中 `——` 次數 / 正文總字數
```

三個定義先講死:

| 項目 | 定義 |
|:---|:---|
| **命中次數** | 整篇文章 (**正文、標題、程式碼區塊都算**) 裡 `——` 出現幾次. 一個 `——` (兩個 `U+2014` 相連) 算 **1 次**, 不是 2 次 |
| **總字數** | 整篇文章去掉空白後的字元數. 中文字、英數字、標點都各算 1 字. **標題、程式碼區塊都算**, 跟命中次數用同一段範圍 |
| **呈現單位** | 同時列**原始比值**與 × 1,000 後的「**每千字 X 次**」 |
| **輸出** | 每篇一列 (`articles.csv`: 命中、字數、密度) ＋ 統計匯總 (`series-summary.csv`: 各系列加總; `results.md`: 全體分佈、quantiles、排行榜) |

**系列匯總用「加總再除」, 不用「平均各篇密度」**:

```
系列密度 = 該系列所有文章命中次數加總 / 該系列所有文章總字數加總
```

平均各篇密度會讓 200 字命中 1 次的短文 (每千字 5 次) 跟 5,000 字的長文權重一樣, 把數字拉歪. 加總再除等於用字數當權重.

> 參考: The Last Fingerprint (2026) 用的是「每千**英文字**」. 中文沒有空格斷詞, 這裡改用「每千**字元**」, 兩者**不能直接比**, 只能在本 lab 內部互比.

**禮貌參數**: fetch 開頭寫死 `SLEEP_SEC = 1.0`、`WORKERS = 2` (每請求後等 1 秒, 同時 2 條連線). 全部組別約 929 系列、1.5 萬篇, 跑完約 2 小時. 抓的是**公開頁面**, 不動任何登入牆或付費內容.

---

## 結果: 密度分佈與排行 — What We Found

> [!WARNING]
> 這一節是 **draft placeholder**. Cut-off 訂在 2026-09-29 之後找一天跑完 fetch + analyze, 再回填實際數字. lab 產出的 `articles.csv`、`series-summary.csv`、`results.md` 會同時 commit 進 repo.

預期看的圖:

- **全體密度分佈**: 中位數 `[待實測]` 每千字次, 前 5% 密度 `[待實測]`
- **各組排行**: Claude AI 組 / Modern Web 組 / DevOps 組 ... 的系列密度中位數比對
- **前 20 高密度文章**: 附連結, 讓讀者自己去看是不是真的很 AI
- **零命中比例**: 全篇沒用過 `——` 的文章佔比, 這一群是「刻意壓乾淨」或「本來就不用」

我對數字的**事前預期** (寫下來給自己打臉用):

1. 中位數大約在 **每千字 0.5-1.5 次**之間 (依 [Last Fingerprint](https://last-fingerprint.example) 的英文語料換算, 但中文寫作對 `——` 更冷淡, 可能再低)
2. 分佈是**長尾**: 大多數人 0 或極少, 少數幾個系列非常高 (每千字 5+)
3. **技術組 > 生活組**: 寫程式的人更早開始拿 AI 幫忙寫文
4. Day 16 之前**已完賽 (30/30)** 的系列, 密度比進行中的系列略高 (存稿多、後段可能更靠 AI 產出)

跑完數字回填時, 對照這四條猜測看誰命中誰打臉.

---

## 邊界與限制 — Caveats

- **只算 `——`** (兩個連在一起的 em dash). 不算單一 `—`, 不算 `--`, 不算 `- -`
- **算整篇**: 正文、標題、程式碼區塊都納入, 命中次數與總字數用同一段範圍
- 排行榜**不設字數門檻**, 短文密度會比較跳, 看排行時一併看字數欄
- 抓的時間點會影響結果, 每次跑數字都不一樣
- **不是 AI 判定**, 只是排序訊號. 不 doxx、不點名, 排行榜只給連結不加評論

---

## 我的重點 — Takeaways

- 15 天寫下來的心法都是**個人厭世**吐出來的, 這篇把體感換數字
- 選 `——` 當第一發, 是因為**中文不用、AI 極愛**的落差最乾淨, 純字元計數最好複製
- **高密度不等於 AI 寫的**, 這個指標只用來排序不做判定
- 密度算整篇 (含標題、程式碼區塊), 系列匯總用**加總再除**, 不用平均各篇, 避免短文拉歪權重
- **輸出兩層**: 每篇一列 (articles.csv) + 統計匯總 (series-summary.csv、results.md 分佈與排行)

---

## Sources

- [lab01: AI 文風檢測 — 雙破折號基準線](../../lab01/README.md)
- [AI 的文風與語氣: 破折號是最強指紋](../../../../02-advanced/writing-style/ai-writing-style-tells.md)
- [AI 的文風與語氣: jason3e7 手筆改寫版](../../../../02-advanced/writing-style/ai-writing-style-tells-jason3e7-voice.md)
- [PG Play writeup 個人文風約束](../../../design-and-guides/pgplay-writeup-style-guide.md)
- [The Last Fingerprint — quantifying LLM stylistic tells (2026)](https://arxiv.org/abs/2603.example)
- [2026 iThome 鐵人賽 首頁](https://ithelp.ithome.com.tw/2026ironman)
