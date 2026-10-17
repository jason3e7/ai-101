---
title: "AI 101 - 鐵人賽 Day 22: 掃 ZeroJudge 78 題, 只下 prompt, AI 自己解到 AC"
tags: [ai, 鐵人賽, ironman, zerojudge, 判准, agentic, 實測, 草稿]
created: 2026-10-07
status: draft
---

# Day 22｜掃 ZeroJudge 78 題: 只下 prompt, AI 自己解到 AC — Scale-Testing AI Autonomous Solving

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 21](./day21-back-to-verifiable-ground.md) 用 3 題 (a001-a003) 示範「有判准, AI 就能自己判有沒有做對」. 這篇擴到 78 題, 分佈在 ZeroJudge 的三個題庫 (基礎 42 / 競賽 16 / UVa 20). 規則很簡單: **除了登入, 人只下 prompt, 完全不介入解題**. 看看 agentic coding + 判准這套在真實規模下 pass rate 多少、哪種題會卡.

> **寫在前面** (jason3e7): 原本想解更多題、再寫一輪比較完整的統整, 但這週 token 已經到頂了. 這篇先是**現階段的 snapshot**, 不是最終版; 下一輪會調整測試方式 (開 2-pass 讀 WA 訊息再送、擴到更難的題庫、或換不同模型跑同一題), 之後再回來補一次更完整的對比.

> **TL;DR (EN):** Day 21 proved "with a judge, AI can self-check" using 3 trivial problems. This post scales to 78: 42 beginner, 16 contest, 20 UVa. Rule: human only logs in; everything else — read problem page, write .cpp, local g++ test, submit via Playwright MCP, read verdict, retry on fail — Claude does unattended. **Overall AC rate: 96.2% (75/78 attempted, 91.5% if counting 4 skipped)**. All UVa 20/20 passed; 3 failures are exactly the "green doesn't mean correct" scenario from Day 21: local samples passed, hidden test cases didn't. 4 skipped problems were AI's own call ("problem statement unclear" or "I can't solve this") — unexpectedly useful behavior: not forcing a wrong answer beats a confident wrong one. Caveat: ZeroJudge is an unusually clean verifier (hidden test cases from the author); most real work has no such ground truth, which is why Day 23 moves on to hooks.

```markdown
# 掃 ZeroJudge 78 題: 只下 prompt, AI 自己解到 AC
* 為什麼擴大 (Day 21 的 3 題太少, 看規模化)
* 測試方法
  * 除了登入, 人只下 prompt
  * 流程: 讀題 → 寫 cpp → 本機 g++ 測 → MCP 送判 → 讀 AC/WA → 不過自改
* 結果 (78 攻 + 4 跳 = 82)
  * 整體 AC 75 / 78 = 96.2%
  * UVa 20/20 全 AC
  * 失敗 3 題全是「樣例過但隱藏資沒過」
* 失敗 / 跳過 分析
  * 失敗 = 判准顯威 (Day 21 綠燈不等於對的實證)
  * 跳過 = AI 自己止損 (意外好)
* 下一篇: 判准不夠強的時候
```

---

## 為什麼擴大 — Why Scale Up

[Day 21](./day21-back-to-verifiable-ground.md) 用 3 題證明 "AI 有判准就能自判". 但 3 題太少, 可能是運氣. 這篇擴到 82 題 (實際答 78 題 + 跳過 4 題), 想一次回答三件事:

1. **AI + 判准的 agentic loop 到底能走多遠?** 整體通過率是什麼量級
2. **失敗卡在哪?** 是題目難, 還是判准本身的限制 (回應 Day 21 的「綠燈不等於對」)
3. **什麼題 AI 會自己放棄?** 跟真的做錯有本質差別

規則很簡單: **登入是人做的, 其他全部 AI**. Claude 讀題網頁、寫 cpp、本機 g++ 編 (`-std=c++17`) 跑樣例自測, 過了用 Playwright MCP 自動送到 ZeroJudge, 讀回判題結果 (AC/WA/CE/TLE). 不過的話自己讀訊息改, 再送.

---

## 測試方法 — The Loop

```
題號清單 (prompt 給 AI 唯一一次)
  ↓
AI 讀題網頁 → 寫 .cpp → g++ 本機測樣例
  ↓ (本機樣例不過: 自己改, 回上一步)
  ↓ (本機樣例過了)
Playwright MCP 自動送 → 讀 AC/WA/CE/TLE → 記錄結果
  ↓
下一題 (不論判題結果, 不重試)
```

**重點**: 這是**一次性 loop**, 不是「不過就再改再送」的多輪 loop. 本機樣例可以反覆改到過, 但**一旦送到 ZeroJudge 判題**, 不論回什麼 (AC/WA/NA/CE), 記錄下來就下一題. 下面的 pass rate 都是**單次送出**的結果, 如果開放重試這個數字會更高.

這條迴圈的三個關鍵:

- **本機先過樣例再送**: 送出去前先在本機用同一版編譯器跑過範例, 等於先過一道內建的驗證. 省掉大量「送上去才發現 CE」的無效 round-trip.
- **MCP 送判題全自動**: Playwright MCP 做 navigate + 貼碼 + 送出 + 等判題結果. 細節踩坑見 [批次上傳 67 題遇到的坑](../../../zerojudge/batch-upload-lessons.md), 這裡不展開.
- **人**: 登入是手動的 (session cookie 到期要重新登), 其他沒有介入. 連「這題看起來不對, 你再想想」都沒說.

### Token / 時間規模

[待填] 這部分需要你補實測數字. 建議的 layout:

| 難度分組 | 每題平均 token | 每題平均時間 | 送判題 MCP 平均 token |
|:---|---:|---:|---:|
| 入門 (基礎前段) | [待填] | [待填] | [待填] |
| 中難 (基礎後段 + 競賽) | [待填] | [待填] | [待填] |
| 難 (UVa + 競賽後段) | [待填] | [待填] | [待填] |

(先把表格框架留好, 填入後這段才能完整)

---

## 結果 — The Numbers

| 題庫 | 答題 | AC | 待修 | 跳過 | 題目感 |
|:---|---:|---:|---:|---:|:---|
| [基礎](../../../zerojudge/basic/README.md) | 42 | 40 | 2 | 0 | 哈囉、閏年、羅馬數字、GCD、迴文、二進位、排序、小字串處理 |
| [競賽](../../../zerojudge/contest/README.md) | 16 | 15 | 1 | 4 | 博弈 DP、剝殼最大子集、凸包頂點、Move-to-Front、Jump Game |
| [UVa](../../../zerojudge/uva/README.md) | 20 | 20 | 0 | 0 | 3n+1、Blocks、Skyline、Trees on the level、Accordian Patience、Krypton Factor |
| **合計** | **78** | **75** | **3** | **4** | — |

- **整體 AC 率 75 / 78 = 96.2%** (不計跳過的)
- 計入跳過也只掉到 91.5% (75 / 82)
- UVa 題庫 **20/20 全 AC** — 經典題目 AI 練過的機率高, 幾乎是零摩擦
- 基礎 42 題只掉 2 題 (95.2%), 競賽 16 題掉 1 題 (93.8%)

這個數字比我預期 (抓 60-70% AC) 高很多.

---

## 失敗跟跳過的差別 — Fails vs Skips

兩件事看起來都是「沒 AC」, 性質完全不同.

### 失敗 (3 題) — 綠燈不等於對的實證

- [`a095`](https://zerojudge.tw/ShowProblem?problemid=a095) 麥哲倫的陰謀 — NA 50% (判題過一半樣例)
- [`a215`](https://zerojudge.tw/ShowProblem?problemid=a215) 明明愛數數 — WA line 7 (第 7 筆輸出不對)
- [`c500`](https://zerojudge.tw/ShowProblem?problemid=c500) AEWE-645 的傷害 — NA 0% (完全沒過)

共通點: **AI 本機樣例全綠, 以為自己對了**. 但 ZeroJudge 有出題者寫的**隱藏測資**, 抓到 AI 沒想到的 edge case. 這正是 [Day 21](./day21-back-to-verifiable-ground.md) 「綠燈不等於對」的具體顯現 — 本機樣例過 ≠ 判題全過. 判准越強, 這類失敗越能被精準捕捉.

這 3 題之所以停在失敗, 是因為這次是**單次送出**的 loop (見 [測試方法](#測試方法--the-loop)): 判題一回 WA/NA 就記錄下來, 不給 AI 看判題訊息再改一輪. 如果開一個「WA 回去讀訊息、改、再送」的二輪 loop, 這 3 題有機會被救回來 — 那會是另一個實驗.

### 跳過 (4 題) — AI 自己止損

- [`m930`](https://zerojudge.tw/ShowProblem?problemid=m930) 正方型池塘水深 — 題意不明, 樣例對不上
- [`b579`](https://zerojudge.tw/ShowProblem?problemid=b579) 恢復分數 — 整數線性系統, 難
- [`b590`](https://zerojudge.tw/ShowProblem?problemid=b590) 單位分數分解 — 樣例對不上, 解題模型未定
- [`i236`](https://zerojudge.tw/ShowProblem?problemid=i236) 邊緣人 (NPSC2020) — 除數分塊數論, 難

共通點: AI 讀題後**自己判斷「做不出來」或「題意不明」**, 主動停下不送. 這是意外的好: **不會硬塞一個隨便的答案送上去**. 一個「自認解不出而不交」比一個「自信交錯」乾淨多了.

這條線其實跟 Day 21 失語症的那條線同根: AI 願不願意說「我不知道」, 比它說「我覺得這樣」還重要.

---

## 我的重點 — Takeaways

- **AC 96%** 的前提是「**有乾淨 ground truth**」. ZeroJudge 是異常強的判准 (出題者寫的隱藏測資), 一般真實專案的測試幾乎沒這麼完整
- **失敗全是「樣例過但隱藏資沒過」** — [Day 21](./day21-back-to-verifiable-ground.md) 的綠燈 ≠ 對, 這 3 題是直接的實證. 當你自己寫測試時, 你就是出題者, 想不到的 case 就測不到
- **跳過 4 題是意外的好 feature**: AI 遇到「讀不懂」或「做不出來」會主動止損, 比硬產一個錯答案乾淨. 這個行為對應 [Day 13](https://ithelp.ithome.com.tw/articles/10417978) 講的「願意說不知道」

---

## Sources

- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 這系列的鋪陳篇 (概念: 有判准就能自判)
- [ZeroJudge](https://zerojudge.tw/) — 文中用的線上解題系統
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 讓 AI 用 MCP 操作瀏覽器
