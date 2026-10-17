# MCU CPE26 題目選集 — 2027 起 CPE 考試題庫 (MCU, 2026-10 版)

[← 回 ZeroJudge Index](./README.md)

> [!NOTE]
> **CPE** (Collegiate Programming Examination, 大學程式能力檢定) 是銘傳大學 (MCU) 主辦的全國性程式檢定. **CPE26 選集** 是 2027 年起每場 CPE 考試會從中「勾選一題」的官方題庫 — **56 題**, 分 14 個主題各 4 題, 全部對應到 UVa Online Judge 題號, 每題都有解答影片. 這篇筆記把題單抄回來, 並對照 [NSYSU 1065 題星等](nsysu-uva-ratings.md) 標好難度.

> **TL;DR (EN):** The official CPE26 problem collection by Ming Chuan University (MCU), used starting 2027 — each CPE exam picks one problem from this set. 56 UVa problems across 14 topic categories (4 each). 38 problems cross-reference against the NSYSU ratings; distribution skews easy (NSYSU ★ 25 / ★★ 12 / ★★★ 1), consistent with its exam-prep role.

```markdown
# MCU CPE26 選集 — 56 題分 14 主題, 2027 起 CPE 考題庫
* 背景
  * MCU 主辦的 CPE 檢定
  * 2027 起每場從此勾一題
  * 56 題 = 14 主題 x 4 題
* 主題
  * 輸入格式 字串 日期時間 算子
  * 質因倍數 進制 二維寫真
  * 規則模擬 窮舉 二維排數
  * 排列組合 排序 集合映對 堆疊佇列
* 比對
  * 38/56 有 NSYSU 評分
  * 分佈偏易 (★ 25, ★★ 12, ★★★ 1)
  * 跟幸運貓只重 1 題 (275)
* 用法
  * 2027 考 CPE 的直接刷此集
  * 配合解答影片
```

---

## 背景 — Background

**CPE** 由銘傳大學資工系自 2007 年起主辦, 每年舉辦 3 次 (3 月 / 6 月 / 12 月), 檢定大學以上程度的程式解題能力. 每場考 **6 題**, 全國近 **40 校** 的大學生可以到就近考區報考, AC 題數達門檻會頒發對應等級的證書.

這份 **CPE26** 是 **2027 年起** 每場 CPE 考試會「從中勾選一題」的官方題庫, 等於:

- **想考 CPE 的學生 → 這 56 題必刷**
- **想練 UVa、不知道從哪開始 → 這 56 題是 MCU 教授精選的入門路徑, 分主題又有解答影片**

集合內的每題都掛 UVa 原題題號, 直接去 ZeroJudge (前綴 `u`) 或 UVa Online Judge 即可送判. MCU 另外提供 CodingPass「小黑碼場」軟體供離線練習.

---

## 56 題總表 — Full Problem List

照 cpe26 編號排序, 編號前兩位是主題 (01-14), 後一位是該主題內的第幾題 (1-4). **NSYSU** 欄位是 [NSYSU 進階程式設計評分](nsysu-uva-ratings.md), `—` 表示 NSYSU 清單沒收這題 (多為 2022 之後的新題).

| 主題 | cpe26 編號 | UVa | 題名 | NSYSU |
|:---|:---|---:|:---|:---|
| 輸入格式 | cpe26011 | [272](https://zerojudge.tw/ShowProblem?problemid=u272) | TEX Quotes | ★ |
| 輸入格式 | cpe26012 | [11356](https://zerojudge.tw/ShowProblem?problemid=u11356) | Dates | — |
| 輸入格式 | cpe26013 | [1586](https://zerojudge.tw/ShowProblem?problemid=u1586) | Molar mass | — |
| 輸入格式 | cpe26014 | [392](https://zerojudge.tw/ShowProblem?problemid=u392) | Polynomial Showdown | — |
| 字串字元 | cpe26021 | [12289](https://zerojudge.tw/ShowProblem?problemid=u12289) | OneTwo Three | ★ |
| 字串字元 | cpe26022 | [11541](https://zerojudge.tw/ShowProblem?problemid=u11541) | Decoding | ★ |
| 字串字元 | cpe26023 | [11576](https://zerojudge.tw/ShowProblem?problemid=u11576) | Scrolling Sign | — |
| 字串字元 | cpe26024 | [10921](https://zerojudge.tw/ShowProblem?problemid=u10921) | Find The Telephone | ★ |
| 日期時間 | cpe26031 | [11650](https://zerojudge.tw/ShowProblem?problemid=u11650) | Mirror Clock | ★ |
| 日期時間 | cpe26032 | [11947](https://zerojudge.tw/ShowProblem?problemid=u11947) | Cancer or Scorpio | — |
| 日期時間 | cpe26033 | [11219](https://zerojudge.tw/ShowProblem?problemid=u11219) | How old are you? | ★ |
| 日期時間 | cpe26034 | [11309](https://zerojudge.tw/ShowProblem?problemid=u11309) | COUNTING CHAOS | ★ |
| 算子運算 | cpe26041 | [12468](https://zerojudge.tw/ShowProblem?problemid=u12468) | Zapping | ★ |
| 算子運算 | cpe26042 | [10035](https://zerojudge.tw/ShowProblem?problemid=u10035) | Primary Arithmetic | ★ |
| 算子運算 | cpe26043 | [13178](https://zerojudge.tw/ShowProblem?problemid=u13178) | Is it multiple of 3? | — |
| 算子運算 | cpe26044 | [275](https://zerojudge.tw/ShowProblem?problemid=u275) | Expanding Fractions | — |
| 質因倍數 | cpe26051 | [10699](https://zerojudge.tw/ShowProblem?problemid=u10699) | Count the factors | ★ |
| 質因倍數 | cpe26052 | [412](https://zerojudge.tw/ShowProblem?problemid=u412) | Pi | — |
| 質因倍數 | cpe26053 | [993](https://zerojudge.tw/ShowProblem?problemid=u993) | Product of digits | ★★ |
| 質因倍數 | cpe26054 | [516](https://zerojudge.tw/ShowProblem?problemid=u516) | Prime Land | ★★ |
| 進制基底 | cpe26061 | [446](https://zerojudge.tw/ShowProblem?problemid=u446) | Kibbles n Bits n Bits n Bits | — |
| 進制基底 | cpe26062 | [11398](https://zerojudge.tw/ShowProblem?problemid=u11398) | The Base-1 Number System | — |
| 進制基底 | cpe26063 | [11121](https://zerojudge.tw/ShowProblem?problemid=u11121) | Base -2 | ★★ |
| 進制基底 | cpe26064 | [948](https://zerojudge.tw/ShowProblem?problemid=u948) | Fibonaccimal Base | ★ |
| 二維寫真 | cpe26071 | [10189](https://zerojudge.tw/ShowProblem?problemid=u10189) | Minesweeper | ★ |
| 二維寫真 | cpe26072 | [10010](https://zerojudge.tw/ShowProblem?problemid=u10010) | Where's Waldorf? | ★★ |
| 二維寫真 | cpe26073 | [706](https://zerojudge.tw/ShowProblem?problemid=u706) | LC-Display | ★ |
| 二維寫真 | cpe26074 | [10800](https://zerojudge.tw/ShowProblem?problemid=u10800) | Not That Kind of Graph | ★★ |
| 規則模擬 | cpe26081 | [10018](https://zerojudge.tw/ShowProblem?problemid=u10018) | Reverse and Add | ★ |
| 規則模擬 | cpe26082 | [10409](https://zerojudge.tw/ShowProblem?problemid=u10409) | Die Game | ★ |
| 規則模擬 | cpe26083 | [11743](https://zerojudge.tw/ShowProblem?problemid=u11743) | Credit Check | ★ |
| 規則模擬 | cpe26084 | [10258](https://zerojudge.tw/ShowProblem?problemid=u10258) | Contest Scoreboard | ★ |
| 正暴窮舉 | cpe26091 | [478](https://zerojudge.tw/ShowProblem?problemid=u478) | Points in Figures | — |
| 正暴窮舉 | cpe26092 | [10365](https://zerojudge.tw/ShowProblem?problemid=u10365) | Blocks | — |
| 正暴窮舉 | cpe26093 | [1225](https://zerojudge.tw/ShowProblem?problemid=u1225) | Digit Counting | ★ |
| 正暴窮舉 | cpe26094 | [11728](https://zerojudge.tw/ShowProblem?problemid=u11728) | Alternate Task | ★ |
| 二維排數 | cpe26101 | [880](https://zerojudge.tw/ShowProblem?problemid=u880) | Cantor Fractions | — |
| 二維排數 | cpe26102 | [913](https://zerojudge.tw/ShowProblem?problemid=u913) | Joana and the Odd Numbers | ★ |
| 二維排數 | cpe26103 | [10161](https://zerojudge.tw/ShowProblem?problemid=u10161) | Ant on a Chessboard | ★★ |
| 二維排數 | cpe26104 | [10920](https://zerojudge.tw/ShowProblem?problemid=u10920) | Spiral Tap | ★★ |
| 排列組合 | cpe26111 | [530](https://zerojudge.tw/ShowProblem?problemid=u530) | Binomial Showdown | — |
| 排列組合 | cpe26112 | [10943](https://zerojudge.tw/ShowProblem?problemid=u10943) | How do you add? | — |
| 排列組合 | cpe26113 | [729](https://zerojudge.tw/ShowProblem?problemid=u729) | The Hamming Distance Problem | — |
| 排列組合 | cpe26114 | [10098](https://zerojudge.tw/ShowProblem?problemid=u10098) | Generating Fast | ★★ |
| 排序處秩 | cpe26121 | [10327](https://zerojudge.tw/ShowProblem?problemid=u10327) | Flip Sort | ★★ |
| 排序處秩 | cpe26122 | [11369](https://zerojudge.tw/ShowProblem?problemid=u11369) | Shopaholic | ★★ |
| 排序處秩 | cpe26123 | [11942](https://zerojudge.tw/ShowProblem?problemid=u11942) | Lumberjack Sequencing | ★ |
| 排序處秩 | cpe26124 | [11039](https://zerojudge.tw/ShowProblem?problemid=u11039) | Building designing | ★★ |
| 集合映對 | cpe26131 | [642](https://zerojudge.tw/ShowProblem?problemid=u642) | Word Amalgamation | — |
| 集合映對 | cpe26132 | [12820](https://zerojudge.tw/ShowProblem?problemid=u12820) | Cool Word | ★ |
| 集合映對 | cpe26133 | [496](https://zerojudge.tw/ShowProblem?problemid=u496) | Simply Subsets | ★ |
| 集合映對 | cpe26134 | [10730](https://zerojudge.tw/ShowProblem?problemid=u10730) | Antiarithmetic | ★★ |
| 堆疊佇列 | cpe26141 | [10935](https://zerojudge.tw/ShowProblem?problemid=u10935) | Throwing cards away I | ★ |
| 堆疊佇列 | cpe26142 | [13190](https://zerojudge.tw/ShowProblem?problemid=u13190) | Rockabye Tobby | ★ |
| 堆疊佇列 | cpe26143 | [673](https://zerojudge.tw/ShowProblem?problemid=u673) | Parentheses Balance | — |
| 堆疊佇列 | cpe26144 | [11352](https://zerojudge.tw/ShowProblem?problemid=u11352) | Crazy King | ★★★ |

---

## 跟 NSYSU 評分對照 — Cross-Reference with NSYSU

| NSYSU 星等 | CPE26 題數 | 占比 |
|:---|---:|---:|
| ★★★ | 1 | 1.8% |
| ★★ | 12 | 21.4% |
| ★ | 25 | 44.6% |
| 未收 (NSYSU 沒評) | 18 | 32.1% |
| **合計** | **56** | 100% |

**觀察**:

- 整個選集**完全沒有 ★★★★ 或 ★★★★★**, 最難的只有一題 **11352 Crazy King (★★★)** — 符合 CPE 作為入門檢定的性格: 考題不會離譜地難
- **18 題 NSYSU 沒收** (約 1/3), 多數是 UVa 11300+ / 12000+ / 13000+ 的近期題; NSYSU 清單只收到 2022/12
- 跟 [幸運貓 UVa 選題](luckycat-uva-selection.md) 32 題只重疊 **1 題** — [275 Expanding Fractions](https://zerojudge.tw/ShowProblem?problemid=u275); 幸運貓專注 2007-2010 的經典題, CPE26 偏新題

### 唯一 ★★★ 題: 11352 Crazy King

這題在**堆疊佇列**主題下, NSYSU 評 ★★★, 用途應該是壓集合裡的「封頂題」 — 如果你 CPE 要考, 這題大概是 CPE 考試會從這 56 題挑出來、相對有鑑別度的那題.

---

## 用法 — How to Use

### 準備 CPE 考試

CPE 考試每場 6 題, 從 2027 起會有 **1 題** 來自這個選集. 刷題路徑建議:

1. 先掃 **NSYSU ★** 的 25 題 (入門級), 熟悉 CPE 風格的輸入輸出
2. 再掃 **NSYSU ★★** 的 12 題 (中等)
3. 把 **18 題 NSYSU 沒評的新題** 當挑戰補完 — 這些是近年加入, 考試出現的機率可能更高
4. **11352 Crazy King** 單獨處理, 看懂題目再動手

### 當 LLM batch 自動解題的題庫

這批難度低、題意清晰、有 UVa 原文 + MCU 解答影片的補強 — 很適合當 AI batch 自動解題的練習集, 預期通過率應該在 90%+ (比我前面 ZeroJudge 78 題那輪 96.2% 的分佈還乾淨, 因為沒有 UVa 題庫以外的雜訊).

### ZeroJudge URL pattern

```
https://zerojudge.tw/ShowProblem?problemid=u<UVa 題號>
```

---

## Sources

- [MCU CPE26 題目選集 (cpe.mcu.edu.tw/cpelist.php)](https://cpe.mcu.edu.tw/cpelist.php) — 本文資料原始來源 (2026-10 版)
- [CPE 大學程式能力檢定官網](https://cpe.cse.nsysu.edu.tw/) — 考試資訊 (報名 / 考區 / 等級)
- [ZeroJudge](https://zerojudge.tw/) — 可直接按題號送判, UVa 題庫前綴 `u`
- [UVa Online Judge](https://onlinejudge.org/) — 題目原站
- [NSYSU 進階程式設計 UVa 星等 (本 repo)](nsysu-uva-ratings.md) — 難度對照來源
