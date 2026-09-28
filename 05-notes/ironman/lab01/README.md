# lab01: AI 文風檢測 — 雙破折號基準線

## 目標

從 2026 iThome 鐵人賽公開文章, 用機械方法檢測 AI 文風的最強指紋: **雙破折號 `——`**. 這是後續更複雜文風檢測 lab 的基準線 (MVP).

## 為什麼從雙破折號開始

- 中文寫作**幾乎不用 `——`**, 但 AI (Claude / GPT 未特別壓制時) 極愛用
- 是「一眼可辨」的 tell, 對照概念見 [ai-writing-style-tells](../../../02-advanced/ai-writing-style-tells.md)
- 純字元計數, 不需要 NLP model, 最容易驗證跟複製
- 從最容易的訊號開始, 拿到 pipeline 骨架再堆更複雜的 signal

## 資料來源

- iThome 鐵人賽 2026 首頁: <https://ithelp.ithome.com.tw/2026ironman>
- 抓「**到執行當天為止**」已發布的所有文章
- **版權歸原作者**, 本 lab 只做統計計數, 不重現內文, 只保留 URL、標題、作者、系列、次數

## 方法

1. 拿 2026 鐵人賽全部系列清單 (跨組)
2. 每個系列展開, 抓已發布文章的 URL
3. 每篇 fetch → 抽正文 → 計算 `——` 出現次數
4. 匯總: per-article、per-series、per-group
5. 輸出 CSV + 排行榜

## 產出

- `data/articles.csv`: 每篇的 url、title、author、series、group、`——` 次數
- `data/series-summary.csv`: 每系列平均 / 中位數 / 總次數
- `results/README.md`: Top N 系列 (最多雙破折號) 排行 + 觀察

## 執行

TBD. 大概會有一個 `run.py` 或 `fetch.sh`, 執行需求:

- 能繞過 Cloudflare (curl + User-Agent header 目前可以, 見 Day 13 檢查文的做法)
- Rate limit: 每篇之間 sleep 幾秒, 別打爆對方
- 增量抓取: 已抓過的不重抓 (存 URL 到 seen list)

## 邊界與限制

- **只算 `——`** (兩個 em dash 連在一起, `U+2014 U+2014`). 不算單一 `—`, 不算 `--`, 不算 `- -`
- 只算正文, **不算標題、程式碼區塊**
- 抓的時間點會影響結果, 執行當天為 cut-off, 每次跑數字都不一樣
- 不做 AI 判定, **不是說 `——` 多就一定是 AI 寫的**. 這是**共現訊號**, 用來排序, 不是判決
- 抓公開資料, 不動任何登入牆或付費內容

## 後續 lab 可能延伸

- **lab02**: 三段式結構偵測 (「首先/接著/最後」節奏)
- **lab03**: AI 常見冗詞掃描 (「值得注意的是」「更在於」「換句話說」)
- **lab04**: 綜合指標 (多 signal 疊加, 給每篇一個「AI 味濃度分數」)
- **lab05**: 對照組 vs 實驗組 (拿去年 pre-ChatGPT 時代的鐵人文章當 baseline, 看 `——` 密度的變化)

## 相關筆記

- [AI 的文風與語氣](../../../02-advanced/ai-writing-style-tells.md), 破折號是「最強指紋」的原因與量化證據
- [AI 的文風與語氣: jason3e7 手筆改寫版](../../../02-advanced/ai-writing-style-tells-jason3e7-voice.md), 同一份內容的 voice 對照
- [PG Play writeup 個人文風約束](../../pgplay-writeup-style-guide.md), Do/Don't 清單
- [jason3e7-writing-voice skill](../../../skills/jason3e7-writing-voice.md), 抽出來給 Claude 用的 skill
