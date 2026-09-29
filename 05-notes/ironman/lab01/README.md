# lab01: AI 文風檢測 — 雙破折號基準線

## 目標

從 2026 iThome 鐵人賽公開文章, 用機械方法檢測 AI 文風的最強指紋: **雙破折號 `——`**. 這是後續更複雜文風檢測 lab 的基準線 (MVP).

## 為什麼從雙破折號開始

- 中文寫作**幾乎不用 `——`**, 但 AI (Claude / GPT 未特別壓制時) 極愛用
- 是「一眼可辨」的 tell, 對照概念見 [ai-writing-style-tells](../../../02-advanced/writing-style/ai-writing-style-tells.md)
- 純字元計數, 不需要 NLP model, 最容易驗證跟複製
- 從最容易的訊號開始, 拿到 pipeline 骨架再堆更複雜的 signal

## 資料來源

- iThome 鐵人賽 2026 首頁: <https://ithelp.ithome.com.tw/2026ironman>
- 抓「**到執行當天為止**」已發布的所有文章
- **版權歸原作者**. `raw.tgz` 保存抓取當下的正文區塊, 只為了讓統計能被重現; 分析結果與筆記只呈現計數與連結, 不引用內文

## 方法

1. 官方報名清單 `/2026ironman/signup/list?group=...` → 全部系列 (每組系列數要等於「報名數」)
2. 每個系列的 RSS `/rss/series/{id}` → 該系列的文章清單
3. 對帳: 文章數對不上的系列, 讀文章頁目錄的「共 N 篇」並補抓 (規則見下方「怎麼確認沒漏抓」)
4. 逐篇抓文章頁, 只存正文區塊 → 計算 `——` 命中次數與正文總字數
5. 以**各篇密度**為主指標, 另附系列匯總; 輸出 CSV + 排行榜

### 為什麼不直接用 RSS 的內文

RSS 雖然附全文, 但會**濾掉部分標點**: 同一篇文章 RSS 裡 `—` 出現 0 次, 文章頁 20 次, 「」、、 也少了一部分. 拿 RSS 算會讓每篇都變 0, 所以 RSS 只用來列文章清單, 內文一律以文章頁為準.

### 怎麼確認沒漏抓

| 層級 | 檢查 |
|:---|:---|
| 系列 | 每組抓到的系列數 = 官方「報名數」 |
| 文章 | 報名清單的進度 `DAY N` 是**連續發文天數**. 挑戰中、已完賽的系列: RSS 篇數必須 = DAY. 斷賽的系列: 作者可能續發, RSS ≥ DAY 屬正常 |
| 上限 | RSS 篇數 ≥ 30 的系列, 另讀文章頁「共 N 篇」, 排除 RSS 有數量上限 |
| 內文 | 每篇文章頁都要下載成功, 缺一篇就算失敗 |

對不上的全部列在 `raw/completeness.json`. 唯一例外是「RSS 空、但進度 DAY > 0」的斷賽系列: 網站上已沒有它們的文章 (實測有作者刪掉斷賽的系列後, 用同名重新報名), 沒有起點可補抓, 單獨列為 `suspected_deleted`.

## 指標: 密度 = 命中次數 / 總字數

```
密度 = 命中特徵次數 / 總字數
```

只看次數會被長文拉高 (寫得多自然命中多), 除以總字數才能比較長短不同的文章.

三個定義先講死, 不然數字沒辦法比:

| 項目 | 定義 |
|:---|:---|
| **命中次數** | 整篇文章 (**正文、標題、程式碼區塊都算**) 裡 `——` 出現幾次. 一個 `——` (兩個 `U+2014` 相連) 算 **1 次**, 不是 2 次 |
| **總字數** | 整篇文章去掉空白後的字元數. 中文字、英數字、標點都各算 1 字. **標題、程式碼區塊都算**, 跟命中次數用**同一段範圍** |
| **呈現單位** | 報表同時列**原始比值**與 × 1,000 後的「**每千字 X 次**」(原始比值常是 0.00x, 乘 1,000 比較好讀) |
| **輸出兩層** | 每篇一列 (`articles.csv`: 命中、字數、密度) ＋ 統計匯總 (`series-summary.csv`: 各系列加總; `results.md`: 全體分佈、quantiles、排行榜) |

**主指標是各篇密度, 排行榜不設字數門檻.** 短文的密度會比較跳 (200 字命中 1 次就是每千字 5), 看排行時要一起看字數欄.

系列 / 組別的匯總則用「加總再除」, 不用「平均各篇密度」:

```
系列密度 = 該系列所有文章命中次數加總 / 該系列所有文章總字數加總
```

如果直接平均各篇密度, 一篇 200 字、剛好命中 1 次的短文 (每千字 5 次) 會跟一篇 5,000 字的長文權重一樣, 把整個系列的數字拉歪. 加總再除等於用字數當權重.

> 參考: 國外研究 (The Last Fingerprint, 2026) 用的是每千「英文字」的次數. 中文沒有空格斷詞, 這裡改用每千「字元」, 兩者數字**不能直接比**, 只能在本 lab 內部互比.

## 檔案結構

```
lab01/
├── README.md          規劃 doc (這一份)
├── fetch.py           抓文 + 對帳 (輸出到 raw/)
├── analyze.py         分析 (讀 raw/ 或 raw.tgz, 產 CSV + results.md)
├── raw.tgz            raw/ 打包 (進 git, 保留可重現性)
├── raw/               解壓後的原始資料 (gitignore, 只在本機用)
│   ├── index.json         系列與文章清單
│   ├── completeness.json  對帳結果
│   ├── rss/{系列}.xml     各系列 RSS (列清單用)
│   ├── pages/{文章}.html  各篇正文區塊 (分析用)
│   └── articles/{文章}.html  對帳補抓時存的完整文章頁 (少量)
├── articles.csv       每篇: 命中次數、總字數、原始比值、每千字
├── series-summary.csv 每系列: 加總後比值、各篇中位數
└── results.md         原始比值排行 (文章、系列)
```

**為什麼原始資料打包成 tgz 才進 git**:

- 一屆鐵人賽上萬篇, 散開放進 git 會產生上萬個小 blob, git history 難看
- 打包成一個 `raw.tgz` 對 git 友善 (一個 blob), 未來要 diff / restore 也一樣清楚
- 只存每篇的正文區塊, 不存整頁 HTML (整頁約 57KB, 正文區塊約 8KB), 壓縮包小很多
- 分析結果 (CSV、results.md) 少且高價值, 散開進 git 沒問題

## 執行

只需要 Python 3 (標準函式庫, 不用裝套件)。三步, 每步可以單獨跑:

```bash
cd 05-notes/ironman/lab01

# 1. 抓文 → raw/ (全部組別約 929 系列、1.5 萬篇, 要 2 小時上下)
python3 -u fetch.py 2>&1 | tee fetch.log

# 2. 確認沒漏抓, 再打包
python3 -c "import json; print(json.load(open('raw/completeness.json'))['summary'])"
#    要看到 unresolved: 0、pages_missing: 0 才打包
tar czf raw.tgz raw/
ls -lh raw.tgz            # GitHub 單檔上限 100MB

# 3. 分析 → articles.csv、series-summary.csv、results.md
python3 analyze.py        # 排行榜預設 20 名; 要 30 名: --top 30
```

**常用選項**:

| 想做的事 | 指令 |
|:---|:---|
| 先試一組就好 (幾分鐘) | `python3 fetch.py --groups claude-ai` |
| 系列有新文章, 重抓 RSS | `python3 fetch.py --refresh` |
| 別台機器只想重現分析 | `git clone` 後直接 `python3 analyze.py` (沒有 `raw/` 會自動讀 `raw.tgz`) |

**怎麼看 fetch 有沒有成功**:

- 結束碼 0 = 全部對上; 結束碼 1 = `completeness.json` 裡 `unresolved` 或 `pages_missing` 不是空的
- `suspected_deleted` 是「報名頁說有進度、RSS 卻 0 篇」的系列, 多半是作者刪文, 不算漏抓, 但會列出來
- 中斷沒關係: 重跑會跳過 `raw/` 裡已有的檔案, 只補沒抓到的。不過步驟 1–4 (點名、對帳) 每次都會重做, 約 10 分鐘

**速度與禮貌**: `fetch.py` 開頭的 `SLEEP_SEC = 0.1` (每個請求後等 0.1 秒)、`WORKERS = 7` (同時 7 條連線)。實測這個組合能穩定跑完不被 Cloudflare 擋; 想更保守可改回 0.5 秒 / 2 workers。

## 邊界與限制

- **只算 `——`** (兩個 em dash 連在一起, `U+2014 U+2014`). 不算單一 `—`, 不算 `--`, 不算 `- -`
- **算整篇**: 正文、標題、程式碼區塊都納入 (命中次數跟總字數都用同一段範圍)
- 排行榜不設字數門檻, 短文密度會比較跳, 看排行時一併看字數
- 抓的時間點會影響結果, 執行當天為 cut-off, 每次跑數字都不一樣
- 不做 AI 判定, **不是說 `——` 多就一定是 AI 寫的**. 這是**共現訊號**, 用來排序, 不是判決
- 抓公開資料, 不動任何登入牆或付費內容

## 後續 lab 可能延伸

- **lab02**: 三段式結構偵測 (「首先/接著/最後」節奏)
- **lab03**: AI 常見冗詞掃描 (「值得注意的是」「更在於」「換句話說」)
- **lab04**: 綜合指標 (多 signal 疊加, 給每篇一個「AI 味濃度分數」)
- **lab05**: 對照組 vs 實驗組 (拿去年 pre-ChatGPT 時代的鐵人文章當 baseline, 看 `——` 密度的變化)

## 相關筆記

- [AI 的文風與語氣](../../../02-advanced/writing-style/ai-writing-style-tells.md), 破折號是「最強指紋」的原因與量化證據
- [AI 的文風與語氣: jason3e7 手筆改寫版](../../../02-advanced/writing-style/ai-writing-style-tells-jason3e7-voice.md), 同一份內容的 voice 對照
- [PG Play writeup 個人文風約束](../../design-and-guides/pgplay-writeup-style-guide.md), Do/Don't 清單
- [jason3e7-writing-voice skill](../../../skills/jason3e7-writing-voice.md), 抽出來給 Claude 用的 skill
