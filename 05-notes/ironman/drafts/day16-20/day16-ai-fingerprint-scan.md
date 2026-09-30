---
title: "AI 101 - 鐵人賽 Day 16: 掃當屆所有文章, 量一次「AI 味」有多少"
tags: [ai, 鐵人賽, ironman, 文風檢測, 雙破折號, 實測, 草稿]
created: 2026-09-29
status: draft
---

# Day 16｜掃當屆所有文章, 量一次「AI 味」有多少 — Scanning This Year's Ironman for the AI Fingerprint

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> 過去 15 天寫下來, 都是我自己在跟 AI 產出搏鬥時累積出來的心法 ([Day 11](../day11-15/day11-verify-ai-output.md) 到 [Day 15](../day11-15/day15-expand-yourself.md) 那段最直接寫成方法). 這篇把體感換成數字: 掃當屆 (2026) 鐵人賽已發布的公開文章, 用最粗最粗的機械指標 (雙破折號 `——` 密度) 量一次 AI 味, 給後續更細的 lab 一條基準線.

> **TL;DR (EN):** Scanned all 15,057 public articles across 814 series in the 2026 iThome Ironman contest for one mechanical AI-fingerprint signal: density of `——` (double em dash) per thousand characters, computed over the full article. Chinese writers almost never use it; Claude/Gemini love it. **Results:** 70% of articles have zero hits, overall density 0.86 per 1k. **Biggest surprise:** the ChatGPT & Codex track scores 0.11 per 1k, the lowest of all groups, likely because GPT-5.1 (Nov 2025) started honoring "no em-dash" custom instructions, killing this signal for GPT-based writers. Claude AI track leads at 1.78. Method + CSVs + full ranking in `lab01/`. Co-occurrence signal, not a verdict.

```markdown
# 掃當屆鐵人賽, 量一次 AI 味有多少
* 為什麼講這個 (15 天個人厭世, 想拿數據看)
* 挑最強指紋: 雙破折號
  * 中文幾乎不用, AI 極愛用
  * 是共現訊號, 不是判決
* 方法: 抓、算、排
  * 名單 + RSS 列文章清單
  * 內文一律走文章頁
  * 密度 = 命中 / 字數
* 結果 (2026-09 實測)
  * 15057 篇, 全體每千字 0.86
  * 70% 零命中
  * ChatGPT 組反常低 0.11
* 前 20 篇排行 (不設字數門檻, max 17.51)
* 邊界: 不是判決, 是排序訊號
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

RSS 雖然附全文, 但**濾掉部分標點**: 同一篇文章對照, RSS 版本 `—` 常常是 0, 文章頁卻可能有數十. 拿 RSS 算會讓每篇密度都變 0. 所以 **RSS 只用來列文章清單, 內文一律以文章頁為準**.

### 3. 算密度: 命中 / 字數

```
密度 = 命中 `——` 次數 / 整篇文章總字數
```

三個定義先講死:

| 項目 | 定義 |
|:---|:---|
| **命中次數** | 整篇文章 (**正文、標題、程式碼區塊都算**) 裡 `——` 出現幾次. 一個 `——` (兩個 `U+2014` 相連) 算 **1 次**, 不是 2 次 |
| **總字數** | 整篇文章去掉空白後的字元數. 中文字、英數字、標點都各算 1 字. **標題、程式碼區塊都算**, 跟命中次數用同一段範圍 |
| **呈現單位** | 同時列**原始比值**與 × 1,000 後的「**每千字 X 次**」 |

**系列匯總用「加總再除」, 不用「平均各篇密度」**:

```
系列密度 = 該系列所有文章命中次數加總 / 該系列所有文章總字數加總
```

平均各篇密度會讓 200 字命中 1 次的短文 (每千字 5 次) 跟 5,000 字的長文權重一樣, 把數字拉歪. 加總再除等於用字數當權重.

> 參考: The Last Fingerprint (2026) 用的是「每千**英文字**」. 中文沒有空格斷詞, 這裡改用「每千**字元**」, 兩者**不能直接比**, 只能在本 lab 內部互比.

**禮貌參數**: fetch 開頭寫死 `SLEEP_SEC = 0.1`、`WORKERS = 7` (每請求 0.1 秒, 同時 7 條連線). 全部組別約 929 系列、1.5 萬篇, 跑完約 30 分鐘, 全程 0 失敗沒被 Cloudflare 擋. 初版設 1 秒 / 2 workers 保守, 實測可以更快. 抓的是公開頁面, 不動任何登入牆或付費內容.

---

## 結果: 密度分佈與排行 — What We Found

抓取時間 2026-09-30. 掃出 814 個系列、15,057 篇文章.

### 整體數字

| 項目 | 數值 |
|:---|---:|
| 系列 / 文章 | 814 / 15,057 |
| 有 `——` 的文章 | 4,547 (30.2%) |
| **零命中** | **10,510 (69.8%)** |
| 命中總次數 | 34,185 |
| 總字數 | 39,634,634 |
| **全體每千字** | **0.86** 次 |

### 各組排行 (加總再除)

| 組別 | 每千字 | 命中 | 篇數 |
|:---|---:|---:|---:|
| Claude AI | **1.78** | 4654 | 990 |
| 佛心分享-IT 人自學之術 | 1.69 | 1708 | 469 |
| Kubernetes | 1.46 | 1913 | 384 |
| AI Engineering | 1.35 | 9339 | 2146 |
| JavaScript | 0.95 | 1094 | 438 |
| 佛心分享-IT 人職涯歷練 | 0.92 | 364 | 295 |
| AI 自動化 | 0.77 | 1347 | 843 |
| AI Security | 0.70 | 1456 | 666 |
| Modern Web | 0.68 | 1614 | 748 |
| Vibe Coding | 0.62 | 1300 | 900 |
| Software Development | 0.60 | 4141 | 2289 |
| Build on Google AI | 0.58 | 1478 | 1028 |
| 自我挑戰 | 0.57 | 1417 | 1459 |
| Security | 0.52 | 1060 | 889 |
| 佛心分享-SideProject30 | 0.51 | 383 | 291 |
| IT Operation | 0.41 | 742 | 612 |
| 佛心分享-IT 人技術創業 | 0.35 | 33 | 36 |
| **ChatGPT & Codex** | **0.11** | 142 | 574 |

**ChatGPT & Codex 組全體最低, 0.11 每千字**. 用 ChatGPT 或 Codex 寫的作者, 密度不到全體 0.86 的八分之一. Claude AI 組 1.78 是它的十六倍.

怎麼解釋? OpenAI 2025-11 讓 GPT-5.1 開始能遵守 custom instruction 的「不要用 em-dash」, 用該工具寫的作者天然沒訊號. Claude 跟 Gemini 目前壓不掉, Claude AI 組還是最高. 當初挑 `——` 是圖它**最粗最好算**, 現在看下來, 這個指紋對 GPT 生態**已經在失效**. 是這次掃描最有價值的意外收穫.

完整排行見 [`lab01/results.md`](../../lab01/results.md).

---

## 原始比值最高的 20 篇（不設字數門檻）

| # | 原始比值 | 每千字 | 命中 | 字數 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 0.017510 | 17.51 | 27 | 1542 | [Day01用 Claude 從零打造一個軟體產品：30 天你也做得到](https://ithelp.ithome.com.tw/articles/10411391) | 零基礎也能當產品長：30 天用 Claude 身兼數職，從零打造軟體產品 | Claude AI |
| 2 | 0.014291 | 14.29 | 42 | 2939 | [Day 12｜第 6 章：流程再造（下）](https://ithelp.ithome.com.tw/articles/10416320) | 白稜 | 佛心分享-IT 人自學之術 |
| 3 | 0.013489 | 13.49 | 15 | 1112 | [Day25：另一套 mpv 懶人包：dyphire/mpv-config 介紹](https://ithelp.ithome.com.tw/articles/10402332) | 一天一招，快速上手 mpv-lazy | 自我挑戰 |
| 4 | 0.013474 | 13.47 | 32 | 2375 | [Day 12：治理了半年，治理系統自己也胖了兩千個 Markdown 的肥大與漂移](https://ithelp.ithome.com.tw/articles/10401999) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 5 | 0.013451 | 13.45 | 32 | 2379 | [Day 10｜第 5 章：三角成形（下）](https://ithelp.ithome.com.tw/articles/10415289) | 白稜 | 佛心分享-IT 人自學之術 |
| 6 | 0.013239 | 13.24 | 15 | 1133 | [Day 23｜量測而不規訓：三個讀數，零個 KPI](https://ithelp.ithome.com.tw/articles/10408284) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 7 | 0.013072 | 13.07 | 14 | 1071 | [Day 12｜四個頻率聚焦：日週月季年，每層看什麼](https://ithelp.ithome.com.tw/articles/10405411) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 8 | 0.012983 | 12.98 | 38 | 2927 | [Day 09｜第 5 章：三角成形（上）](https://ithelp.ithome.com.tw/articles/10414677) | 白稜 | 佛心分享-IT 人自學之術 |
| 9 | 0.012951 | 12.95 | 14 | 1081 | [Day 21｜帳本就是介面：AI 不跟 AI 聊天](https://ithelp.ithome.com.tw/articles/10407741) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 10 | 0.012752 | 12.75 | 19 | 1490 | [Day 9｜董總監：組織需要第三個治理功能](https://ithelp.ithome.com.tw/articles/10404754) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 11 | 0.012721 | 12.72 | 56 | 4402 | [Day 07｜第 3 章：董事長的承諾（下）](https://ithelp.ithome.com.tw/articles/10413569) | 白稜 | 佛心分享-IT 人自學之術 |
| 12 | 0.012126 | 12.13 | 15 | 1237 | [Day 6｜兩本帳：向內的帳全綠，向外的帳沒人記](https://ithelp.ithome.com.tw/articles/10404140) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 13 | 0.012090 | 12.09 | 14 | 1158 | [Day 20｜AI 是手，不是腦：認知卸載的紅線](https://ithelp.ithome.com.tw/articles/10407481) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 14 | 0.011518 | 11.52 | 11 | 955 | [Day 28｜加了這堆治理，真的有比較好嗎](https://ithelp.ithome.com.tw/articles/10406510) | 我做了一個幫你寫鐵人賽的 Agent Skill，然後讓它寫自己 | 佛心分享-SideProject30 |
| 15 | 0.011511 | 11.51 | 16 | 1390 | [Day 18｜解釋頻寬：系統不只做不動，還會看不懂](https://ithelp.ithome.com.tw/articles/10406854) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 16 | 0.011398 | 11.40 | 15 | 1316 | [Day 17｜兵法三部曲：餘裕重定義落地](https://ithelp.ithome.com.tw/articles/10406603) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 17 | 0.011377 | 11.38 | 10 | 879 | [Day 22｜Day 1 初稿不是成品，是等著被改的半成品](https://ithelp.ithome.com.tw/articles/10405151) | 我做了一個幫你寫鐵人賽的 Agent Skill，然後讓它寫自己 | 佛心分享-SideProject30 |
| 18 | 0.011376 | 11.38 | 31 | 2725 | [Day 03｜第 1 章：微軟博物館（下）](https://ithelp.ithome.com.tw/articles/10410755) | 白稜 | 佛心分享-IT 人自學之術 |
| 19 | 0.011345 | 11.35 | 14 | 1234 | [Day 27｜管現在的人：節奏承載力，與不掉球的日常](https://ithelp.ithome.com.tw/articles/10409424) | 轉型之後：IT 領導者的第二座山 | IT Operation |
| 20 | 0.011334 | 11.33 | 13 | 1147 | [Day 16｜裂變定向突變組牌：讓團隊長大的三個動詞](https://ithelp.ithome.com.tw/articles/10406339) | 轉型之後：IT 領導者的第二座山 | IT Operation |

---

## 邊界與限制 — Caveats

- **只算 `——`** (兩個連在一起的 em dash). 不算單一 `—`, 不算 `--`, 不算 `- -`
- **算整篇**: 正文、標題、程式碼區塊都納入, 命中次數與總字數用同一段範圍
- 排行榜**不設字數門檻**, 短文密度會比較跳, 看排行時一併看字數欄
- 抓的時間點會影響結果, 每次跑數字都不一樣
- **不是 AI 判定**, 只是排序訊號. 不 doxx、不點名, 排行榜只給連結不加評論

---

## 我的重點 — Takeaways

- 15 天的心法都是個人厭世吐出來的, 這篇把體感換數字
- 選 `——` 當第一發沒錯, 用「中文不用、AI 極愛」的落差最乾淨, 純字元計數最好複製
- 最意外的一件事: **ChatGPT & Codex 組全體最低 (0.11)**, 對照 2025-11 GPT-5.1 起可壓 em-dash 的變化, 這個指紋對 GPT 生態已經在失效. 訊號選集要考慮不同模型的實際輸出, 這行為還會隨版本變
- **七成文章零命中**, 這件事本身也是強觀察. 大多數作者根本不用這個符號
- 高密度不等於 AI 寫的, 這個指標只做排序不做判定

---

## Sources

- [lab01: AI 文風檢測 — 雙破折號基準線](../../lab01/README.md), [完整排行 `results.md`](../../lab01/results.md), [每篇 `articles.csv`](../../lab01/articles.csv), [每系列 `series-summary.csv`](../../lab01/series-summary.csv)
- [lab01 V07 preview: 中文冗詞候選 + Kobak 英文權威清單](../../lab01/v07-preview.md)
- [AI 的文風與語氣: 破折號是最強指紋](../../../../02-advanced/writing-style/ai-writing-style-tells.md)
- [AI 的文風與語氣: jason3e7 手筆改寫版](../../../../02-advanced/writing-style/ai-writing-style-tells-jason3e7-voice.md)
- [AI 生成內容浮水印: 2026 現況 + em-dash 政治化](../../../../01-fundamentals/ai-content-watermark.md)
- [Kobak et al. (2025) Science Advances: excess vocabulary 統計研究](https://www.science.org/doi/10.1126/sciadv.adt3813)
- [2026 iThome 鐵人賽 首頁](https://ithelp.ithome.com.tw/2026ironman)
