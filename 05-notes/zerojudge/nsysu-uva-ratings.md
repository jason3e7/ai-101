# 中山資工 UVa 星等評分 — NSYSU Advanced Programming UVa Ratings

[← 回 ZeroJudge Index](./README.md)

> [!NOTE]
> 國立中山大學 (NSYSU) 資工系「進階程式設計」(`par.cse.nsysu.edu.tw/~advprog/`) 替 UVa 題目評了 **★–★★★★★** 五個等級, 2011/05/25 ~ 2022/12/13 期間還吃進了歷屆 **CPE** (大學程式能力檢定) 試題. 原頁已離線, 這篇從 [Wayback Machine 2024-11-02 快照](https://web.archive.org/web/20241102135335/https://par.cse.nsysu.edu.tw/~advprog/star.php) 抄回來, **1065 題**, 下 batch 解題時可以照星等挑難度.

> **TL;DR (EN):** NSYSU CS Department rated 1,065 UVa problems with ★–★★★★★ (incl. historical Taiwan CPE exam problems through 2022-12). Original site offline; archived via Wayback. Full `(problem_id, stars)` table in [nsysu-uva-ratings.csv](nsysu-uva-ratings.csv). Distribution skews mid: ★★★ 334 題 (31%), ★★ 304 題 (29%), ★★★★ 167 題 (16%), ★ 234 題 (22%), ★★★★★ 只有 25 題 (2%).

```markdown
# 中山資工 UVa 星等評分 — 1065 題照 ★1-5 分級, 原站離線靠 archive
* 背景
  * NSYSU 進階程式設計評分
  * 吃 2011-2022 CPE 試題
  * 原頁離線, Wayback 保存
* 分佈
  * ★★★ 居多 (334 題)
  * ★★★★★ 稀有 (25 題)
  * ★ 入門多 (234 題)
* 用法
  * batch 掃題照星等挑
  * ★★★★★ 25 題當挑戰集
  * 完整表看 csv
```

---

## 背景 — Background

NSYSU 這份清單原本掛在 **進階程式設計** (Advanced Programming) 課程網站, 由助教/教師把每題 UVa 評上一個 **★ 1–5 的難度等級**, 幫學生照順序刷. 2011 年起還一路把每屆 **CPE (Collegiate Programming Examination, 大學程式能力檢定)** 的試題也收進來一併評分, 累積到 2022/12 已有 **1065 題**.

網站本體 (`par.cse.nsysu.edu.tw/~advprog/star.php`) 已經連不上, 這份紀錄是 Wayback Machine 2024-11-02 的最後快照.

> [!TIP]
> 跟 [幸運貓 UVa 選題](luckycat-uva-selection.md) 的差別: 幸運貓是手挑 32 題「值得練」; 這份 NSYSU 是**覆蓋式評分**, 1000+ 題一題不漏標星等, 適合當 batch 自動解題時的難度 index.

---

## 星等分佈 — Star Distribution

| 星等 | 題數 | 占比 | 體感 |
|:---|---:|---:|:---|
| ★★★★★ | 25 | 2.3% | 挑戰題, 單獨處理 (競賽等級) |
| ★★★★ | 167 | 15.7% | 需要正確演算法選型 |
| ★★★ | 334 | 31.4% | 中規中矩, 數量最多 |
| ★★ | 304 | 28.5% | 標準題, 熟手 batch 掃 |
| ★ | 234 | 22.0% | 入門暖身 |
| 0 (未評) | 1 | 0.1% | UVa 11799, 評分表上留 0 分 |
| **合計** | **1065** | 100% | — |

**整體偏中後段** (★★★ + ★★★★ 合計 47%), 不是純入門清單. 想從簡單開始的 → 先掃 ★ 234 題; 想找硬菜 → 看下面 ★★★★★ 名單.

---

## 五星題 (25 題) — The Hard Set

這 25 題是這份清單裡**只有的 ★★★★★**, 具 reference 價值, 直接列出來:

| 題號 | ZeroJudge | 題號 | ZeroJudge |
|---:|:---|---:|:---|
| 10021 | [u10021](https://zerojudge.tw/ShowProblem?problemid=u10021) | 10904 | [u10904](https://zerojudge.tw/ShowProblem?problemid=u10904) |
| 10059 | [u10059](https://zerojudge.tw/ShowProblem?problemid=u10059) | 11007 | [u11007](https://zerojudge.tw/ShowProblem?problemid=u11007) |
| 10084 | [u10084](https://zerojudge.tw/ShowProblem?problemid=u10084) | 11014 | [u11014](https://zerojudge.tw/ShowProblem?problemid=u11014) |
| 10119 | [u10119](https://zerojudge.tw/ShowProblem?problemid=u10119) | 11019 | [u11019](https://zerojudge.tw/ShowProblem?problemid=u11019) |
| 10120 | [u10120](https://zerojudge.tw/ShowProblem?problemid=u10120) | 11046 | [u11046](https://zerojudge.tw/ShowProblem?problemid=u11046) |
| 10149 | [u10149](https://zerojudge.tw/ShowProblem?problemid=u10149) | 11050 | [u11050](https://zerojudge.tw/ShowProblem?problemid=u11050) |
| 10206 | [u10206](https://zerojudge.tw/ShowProblem?problemid=u10206) | 11081 | [u11081](https://zerojudge.tw/ShowProblem?problemid=u11081) |
| 10270 | [u10270](https://zerojudge.tw/ShowProblem?problemid=u10270) | 11098 | [u11098](https://zerojudge.tw/ShowProblem?problemid=u11098) |
| 10330 | [u10330](https://zerojudge.tw/ShowProblem?problemid=u10330) | 11419 | [u11419](https://zerojudge.tw/ShowProblem?problemid=u11419) |
| 10418 | [u10418](https://zerojudge.tw/ShowProblem?problemid=u10418) | 11467 | [u11467](https://zerojudge.tw/ShowProblem?problemid=u11467) |
| 10615 | [u10615](https://zerojudge.tw/ShowProblem?problemid=u10615) | 11922 | [u11922](https://zerojudge.tw/ShowProblem?problemid=u11922) |
|  |  | 12030 | [u12030](https://zerojudge.tw/ShowProblem?problemid=u12030) |
|  |  | 12075 | [u12075](https://zerojudge.tw/ShowProblem?problemid=u12075) |
|  |  | 12092 | [u12092](https://zerojudge.tw/ShowProblem?problemid=u12092) |

幾個熟面孔 (若有在刷 UVa 的話): **10120 Gallery Guard**、**10120 Damage Assessment**、**11081 Strings** (DP)、**11467** 都是知名硬題.

---

## 完整列表 — Full Table

全部 1065 題的 `(題號, 星等)` 存成 CSV, 方便 grep / 做 batch 自動化 input:

[`nsysu-uva-ratings.csv`](./nsysu-uva-ratings.csv) · 格式: `problem_id,stars`

常用查詢範例:

```bash
# 查某題的星等
grep '^11081,' nsysu-uva-ratings.csv
# → 11081,5

# 列出所有 ★ 題目 (234 題)
awk -F, '$2==1 {print $1}' nsysu-uva-ratings.csv

# 隨機抽 10 題 ★★★ 來今天掃
awk -F, '$2==3 {print $1}' nsysu-uva-ratings.csv | shuf -n 10

# 跟手上已 AC 的題目對差集, 挑沒做過的 ★★★★
comm -23 <(awk -F, '$2==4 {print $1}' nsysu-uva-ratings.csv | sort) <(ls uva/ | grep -oP 'u\K\d+' | sort)
```

---

## 用法 — How to Use

這份清單對應 ZeroJudge 的 UVa 題庫, URL pattern:

```
https://zerojudge.tw/ShowProblem?problemid=u<題號>
```

挑題策略 (個人用):

- **batch 自動解**: 挑 ★★ 或 ★★★ 區間 — 數量多 (合計 638 題)、難度中規中矩, AI 一次掃一批通過率高
- **壓力測試 AI**: 挑 ★★★★ 或 ★★★★★ — 看 AI 在需要正確演算法選型的題目上是不是也能自己判出來
- **按主題補強**: 這份清單**沒有 tag**, 要搭 [幸運貓 UVa 選題](luckycat-uva-selection.md) 的 tag 一起看 (DP / graph / math / string ...)

---

## Sources

- [NSYSU 進階程式設計 UVa 星等 (archive.org 2024-11-02 快照)](https://web.archive.org/web/20241102135335/https://par.cse.nsysu.edu.tw/~advprog/star.php) — 本文資料原始來源
- [ZeroJudge](https://zerojudge.tw/) — 可直接按題號送判
- [UVa Online Judge](https://onlinejudge.org/) — 題目原站
- [CPE 大學程式能力檢定](https://cpe.cse.nsysu.edu.tw/) — NSYSU 主辦的全國性檢定, 這份清單吸收了 2011–2022 歷屆試題
