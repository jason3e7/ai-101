---
title: "AI 101 - 鐵人賽 Day 23: ZeroJudge 收尾, 跨主題各挑一題 + 多輪 AC 的題怎麼救回來"
tags: [ai, 鐵人賽, ironman, zerojudge, 判准, agentic, 實測, 草稿]
created: 2026-10-08
status: draft
---

# Day 23｜ZeroJudge 收尾: 按主題各挑一題, 看 AI 的 ceiling 跟「救回」能力 — Topics × Difficulty × Retries

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 22](./day22-zerojudge-at-scale.md) 掃 78 題 96.2% AC 是「廣度」, 但那批題分佈在 ZeroJudge 前幾頁, 主題有偏食. 這篇換「深度」: 用 [MCU CPE26 題目選集](../../../zerojudge/mcu-cpe26-problem-set.md) 的 14 個主題分類當 index, **每主題各挑一題 + 外加 5 題超綱硬菜**, 總共 19 題再跑一次. 另外補一段 Day 22 當時卡住的 4 題後來怎麼多輪 AC 救回來, 把「單次送出的 pass rate 低估了 ceiling」這件事講清楚.

> **寫在前面** (jason3e7): 這是 ZeroJudge 三部曲的收尾 (Day 21 概念 → Day 22 規模 → Day 23 深度 + 救援). 下一輪離開 OJ 這種奢侈 oracle, 走到判准不好做的場.

> **TL;DR (EN):** Day 22 scaled breadth with narrow topics. Day 23 scales depth: picked 14 problems one per CPE26 topic category (input/string/datetime/arithmetic/factors/base/2D-grid/simulation/brute-force/combinatorics/sorting/set/stack-queue) + 5 harder off-topic (max flow, DP, tree, extra simulation). All 19 single-pass AC, including UVa 10330 (★★★★★ node-capacity max flow — AI applied the split-node trick) and UVa 147 (★★★★ change-counting DP). Also documents 4 problems from Day 22's fail/skip set that *did* AC on a later round once the human intervened minimally (feed back the WA message, read the author's solution writeup) — single-pass numbers underreport the ceiling.

```markdown
# ZeroJudge 收尾: 按 CPE26 主題挑 + 補 5 題超綱, 另外救回 4 題舊帳
* 為什麼再測一次 (Day 22 主題有偏食)
* 選題
  * CPE26 14 主題各挑一題
  * 外加 5 題硬菜 (DP / Max Flow / 樹)
  * 19 題 = 14 覆蓋 + 5 超綱
* 結果 (19 題全一次 AC)
* 三題硬菜拆解
  * 10330 Max Flow 拆點
  * 147 Dollars DP
  * 122 Tree 建構
* 多輪 AC 救回 (舊帳)
  * A 類: 正確性 (a095 / a215 / c500 / b590)
  * B 類: 效能 (s142 MLE, s794 TLE, s796 TLE)
* 收斂
```

---

## 為什麼再測一次 — Why Scale Depth, Not Just Breadth

Day 22 掃 78 題 96.2% AC 看起來強, 但有兩個合理質疑:

1. **主題有偏食** — 78 題多是 ZeroJudge 第 1-2 頁, 題庫 convention 偏入門, 題型集中在哈囉、閏年、迴文、GCD 這類
2. **真的硬題幾乎沒出現** — 跳過的 4 題、失敗的 3 題都還落在「中等」區間, 教科書等級 (max flow / DP) 的題一題都沒碰到

這篇換個抽樣法, 用上一輪新建的 [MCU CPE26 題目選集](../../../zerojudge/mcu-cpe26-problem-set.md) 當 index — 這 56 題是 MCU 為 2027 起 CPE 檢定分 14 個主題各 4 題挑的題庫, **主題覆蓋面是現成的**. 做法:

- **每主題挑 1 題** → 14 題覆蓋 CPE26 的全部主題分類
- **外加 5 題「CPE26 不收但更難」的題** → 補 NSYSU ★★★ 以上、CPE26 選集沒有的方向 (DP、max flow、樹)

共 **19 題**, 全部已存到 [`zerojudge/selected/`](../../../zerojudge/selected/README.md) 一題一檔.

---

## 選題 — The 19

### 14 題覆蓋 CPE26 的所有主題

| CPE26 主題 | ZeroJudge | UVa | 題名 | NSYSU |
|:---|:---|---:|:---|:---|
| 輸入格式 | [c007](https://zerojudge.tw/ShowProblem?problemid=c007) | 272 | TeX Quotes | ★ |
| 字串字元 | [e208](https://zerojudge.tw/ShowProblem?problemid=e208) | — | Run-length Decoding (ZJ 原生) | — |
| 日期時間 | [j056](https://zerojudge.tw/ShowProblem?problemid=j056) | 11650 | Mirror Clock | ★ |
| 算子運算 | [a518](https://zerojudge.tw/ShowProblem?problemid=a518) | 12468 | Zapping | ★ |
| 質因倍數 | [d120](https://zerojudge.tw/ShowProblem?problemid=d120) | 10699 | Count the factors | ★ |
| 進制基底 | [d379](https://zerojudge.tw/ShowProblem?problemid=d379) | 446 | Kibbles n Bits (Hex) | — |
| 二維寫真 | [e605](https://zerojudge.tw/ShowProblem?problemid=e605) | 10189 | Minesweeper | ★ |
| 規則模擬 | [c015](https://zerojudge.tw/ShowProblem?problemid=c015) | 10018 | Reverse and Add | ★ |
| 正暴窮舉 | [d094](https://zerojudge.tw/ShowProblem?problemid=d094) | 478 | Point in Figures | — |
| 二維排數 | [d096](https://zerojudge.tw/ShowProblem?problemid=d096) | 913 | Joana and the Odd Numbers | ★ |
| 排列組合 | [c061](https://zerojudge.tw/ShowProblem?problemid=c061) | 530 | Binomial C(n,m) | — |
| 排序處秩 | [a539](https://zerojudge.tw/ShowProblem?problemid=a539) | 10327 | Flip Sort (逆序數) | ★★ |
| 集合映對 | [e706](https://zerojudge.tw/ShowProblem?problemid=e706) | 12820 | Cool Word | ★ |
| 堆疊佇列 | [e155](https://zerojudge.tw/ShowProblem?problemid=e155) | 10935 | Throwing Cards Away | ★ |

### 5 題「CPE26 不收但更有鑑別度」的硬菜

| ZeroJudge | UVa | 題名 | NSYSU | 補的方向 |
|:---|---:|:---|:---|:---|
| [d760](https://zerojudge.tw/ShowProblem?problemid=d760) | 10330 | Power Transmission | ★★★★★ | 節點容量 max flow |
| [d397](https://zerojudge.tw/ShowProblem?problemid=d397) | 147 | Dollars 找零方法數 | ★★★★ | DP |
| [c101](https://zerojudge.tw/ShowProblem?problemid=c101) | 122 | Trees on the level | ★★★ | 二元樹建構 + BFS |
| [a536](https://zerojudge.tw/ShowProblem?problemid=a536) | 11689 | 收集空瓶換汽水 | ★ | 多一題規則模擬當對照 |
| [e592](https://zerojudge.tw/ShowProblem?problemid=e592) | 10142 | Australian Voting | ★★ | 多輪淘汰的規則模擬 |

**難度分佈**: NSYSU 評分的 15 題裡 ★ 10 題 / ★★ 2 題 / ★★★ 1 題 / ★★★★ 1 題 / ★★★★★ 1 題; 另外 4 題 NSYSU 沒收 (2023+ 新題或 ZJ 原生題).

送題規則跟 Day 22 完全一樣: 本機 g++ 跑樣例 → Playwright MCP 送判題 → **單次送出, 不重試**.

---

## 結果 — The Numbers

**19 / 19 全 AC**. 包含:

- UVa 10330 (★★★★★) ✅
- UVa 147 (★★★★) ✅
- UVa 122 (★★★) ✅

從 Day 22 的 96.2% 反而**跳到 100%** — 不是題變簡單, 是**抽樣改變了**: 這批 19 題多是 UVa 經典, 比起 Day 22 包含的 ZeroJudge 原生題與校內競賽題, AI 練過的機率更高, 樣例也更乾淨.

這件事本身就是一個觀察: **AI 的 pass rate 受「題目有多經典」影響很大, 不只是「難度」**. 一題 ★★★★★ 的 max flow 經典題, 比起一題 ★ 但沒出現在訓練資料的校內題, 前者反而容易過.

---

## 三題硬菜 — The Three Harder Ones

### UVa 10330 Power Transmission (★★★★★) — 節點容量 Max Flow

題意是 power grid 要算最大供電量, 但**節點本身也有容量上限** — 不是標準「邊有容量」的 max flow.

教科書做法: **拆點 (node splitting)**. 把每個節點 v 拆成 v_in / v_out, 中間連一條容量 = 原節點容量的邊; 原本連到 v 的入邊接到 v_in, 從 v 出的邊接到 v_out. 這樣就把「節點容量」降階成標準 max flow.

AI 自己想到拆點, 寫出 Edmonds-Karp, 本機樣例過, 送判一次 AC.

> [!NOTE]
> 一個合理解釋: 拆點 + max flow 是教科書 pattern, 在訓練資料裡大量出現. AI 不是「推理出」這個技巧, 更接近「認出題型後取出配方」. 這跟 [Day 03 的 LLM 限制分類](https://ithelp.ithome.com.tw/articles/10412787) 說的一致 — **它強在 pattern recognition, 弱在真正新的推理**. OJ 題幾乎全在 pattern 範圍內, 所以 AI 的 ceiling 看起來特別高.

### UVa 147 Dollars (★★★★) — 找零方案數 DP

給一組硬幣面額, 問湊出目標金額有幾種組合. 經典的**找零方案數 DP** — 不是「最少硬幣」, 是「有幾種組合」, **外迴圈跑面額、內迴圈跑金額**才對 (順序反了會變成算排列).

AI 一次寫對, 迴圈順序正確, 送判 AC.

### UVa 122 Trees on the level — 二元樹建構

輸入是一堆 `(value,path)` 格式的節點 (`path` 是 "LRL" 這種 L/R 字串表示到根的路徑), 要**先建樹再做 level-order traversal (BFS)**. 關鍵在**檢查結構一致性** (節點不能被定義兩次, 也不能缺少 parent).

AI 用 map 存節點 + 旗標偵測重複/缺漏, 一次 AC.

---

## 不是一次過的: 舊帳救回來 — Multi-Round AC

單輪送出的 pass rate 低估 ceiling. 開「WA/TLE/MLE 訊息餵回去」或「人提示一句」的二輪 loop, 多題能爬回來. 分兩種救援型態 — **正確性類** (邏輯 / 邊界 / 判題模型錯) 跟 **效能約束類** (時間 / 記憶體超限).

### A. 正確性類: 邏輯 / 邊界 / 判題模型 (基礎 + 競賽題庫)

| 題號 | 當時 | 幾輪 AC | 關鍵 fix |
|:---|:---|:---|:---|
| [a095](https://zerojudge.tw/ShowProblem?problemid=a095) 麥哲倫的陰謀 | Day 22 NA 50% | **2 輪** | special-case `M == N` (全紅帽無白帽) |
| [a215](https://zerojudge.tw/ShowProblem?problemid=a215) 明明愛數數 | Day 22 WA line 7 | **2 輪** | n/m 可為負數 + `__int128` 防 overflow |
| [c500](https://zerojudge.tw/ShowProblem?problemid=c500) AEWE-645 的傷害 | Day 22 NA 0% | **3 輪 (靠作者解題報告)** | 判題模型跟物理最佳解不同 |
| [b590](https://zerojudge.tw/ShowProblem?problemid=b590) 單位分數分解 | Day 22 原本跳過 | **1 次重試** | DFS 搜尋 + 剪枝框架 |

### B. 效能約束類: TLE / MLE (ORIGINAL 題庫校內原創題)

這幾題一開始**邏輯都對、樣例也過**, 但判題直接甩 **TLE** 或 **MLE** — 效能約束是另一種「公開樣例看不見」的盲區. 救援的方式不是改邏輯, 是**換資料結構 / 換演算法 / 換 IO**.

| 題號 | 當時 | 關鍵 fix | 加速 |
|:---|:---|:---|:---|
| [s142](https://zerojudge.tw/ShowProblem?problemid=s142) 最大正方形 | MLE (10MB 限制) | 2D dp → **滾動 1D dp**, 邊讀邊算, 不存整個矩陣 | 空間 O(nm) → O(m) |
| [s794](https://zerojudge.tw/ShowProblem?problemid=s794) 1A2B | TLE | 關鍵觀察: 猜測各 (A,B) 桶的大小**只取決於數字重數結構**, 用小查表 O(1) 查, 只對「最小桶」的提示建完整 bucket | 單輪 O(N²) → O(表) |
| [s796](https://zerojudge.tw/ShowProblem?problemid=s796) 蜂蜜工廠 | TLE | Matroid 貪心 + **線段樹** 加速區間可達查詢, 鏈式左移/右移快路徑先試, Kuhn's 二分圖匹配當 fallback | 多個 O(N²) 操作各降 log 階 |

這 3 題都是**本機樣例看不出來, 送判題才知道效能不夠**. 判題在這裡扮演兩個角色: (1) 給出 TLE/MLE 的明確信號 (2) 強制 AI 跳出「樣例過了就以為對」的錯覺.

### 共通點: 判准的真正價值是「失敗時給具體 signal」

兩種救援合起來看, pattern 一致:

- **樣例全綠不代表對** — 可能是邊界沒蓋到 (a095)、資料範圍漏讀 (a215)、判題模型不同 (c500)、效能不夠 (s142/s794/s796)
- **單輪送出的 pass rate 低估 ceiling** — 一個「WA/TLE/MLE + 錯誤訊息」丟回 AI, 它多半能自己改; 真的卡住的 (c500, b590) 一句方向性提示就救回來
- **判准不是只分「過 / 不過」**, 是**失敗時給具體 signal 讓 AI 自己爬** — 這才是 OJ 這類 oracle 真正有價值的地方

### 救援後的總帳

| 範圍 | AC 數 | AC 率 |
|:---|---:|---:|
| Day 22 單次送出 (78 題) | 75 / 78 | 96.2% |
| Day 22 + A 類二輪救援 | 78 / 78 | 100% |
| 加 ORIGINAL B 類效能救援 (s142/s794/s796) | 全部 AC | 100% |
| 加 Day 23 新 19 題 | 全部 AC | 100% |

### a095 — 邊界條件被忽略

Day 22 的解法沒處理 `M == N` (全部都是紅帽) 這個 edge case. 把 WA 訊息給 AI 看, 它馬上發現「如果沒有白帽就不用等那一天」, 修正為 `(M == N ? M : M+1)` 一次 AC. 這類 **"主邏輯對, 邊界錯"** 的 WA, 開二輪 loop 最好修.

### a215 — 題目條件被忽略 + 數值 overflow

兩個獨立問題疊在一起: (1) 題目說 n, m 可以是負數, 但 AI 一開始的 while 條件沒處理「`n > m` 時至少數一個」(2) 累加可能 overflow long long. 把 WA 訊息 + 「仔細看範圍條件」一句提示給它, 第二輪就改成 `do-while` + `__int128`, AC.

### c500 — 判題模型跟物理最佳解不一樣 (3 輪才通)

這題是整系列最有啟發的一題. **公開範例全綠, 判題 NA 0%**, 樣例都測不出差.

坑在**判題用的模型跟物理最佳解不是同一個**:
- 題目描述讓人以為滑鼠位置是「用物理 argmin 算」的最壞分佈
- 但判題腳本實際上是**「從第 1 格起每 f 格放一隻」固定模型** (位置 1, 1+f, 1+2f...), 位置 ≤ m 的免計傷害 (包含剛好落在 m 的也算 0)
- 兩個做法**只在 `m ≡ 1 (mod f)` 時結果不同**, 兩組公開範例剛好都不滿足這條件, 所以本機怎麼測都綠

AI 自己撞牆兩輪都在改實作沒用. 第三輪我把**作者的解題報告貼進 context**, 它立刻看出來「原來判題用這個模型」, 修 `⌊(m-1)/f⌋+1` 當左側數量, AC.

> [!IMPORTANT]
> 這題的教訓: **公開範例全綠 + WA, 要先懷疑「對判題模型的理解」而不是實作**. AI 自己 brute 對拍只會「跟自己同一個錯誤模型」互相證明對 — 需要一個**獨立來源** (作者解題報告 / 題目來源的 reference solution) 才能跳出同溫層. 這呼應 [Day 21 「綠燈不等於對」](https://ithelp.ithome.com.tw/articles/10421407) — 強的 oracle 也會有盲區, 當盲區剛好蓋住公開樣例, 自測就無效了.

### b590 — Day 22 原本放棄的題

Day 22 AI 自己讀題後說「樣例對不上, 解題模型未定」就跳過了. 事後我讀題意後給 AI 一個明確的「用 DFS 分母非遞減」提示, 它立刻寫出來一次 AC. **一個「原本放棄」的題, 一句話的方向就能救回**.

### 救援後的總帳

把 4 題二輪 AC 併回去:

| 範圍 | AC 數 | AC 率 |
|:---|---:|---:|
| Day 22 單次送出 | 75 / 78 | 96.2% |
| Day 22 + 二輪救援 (本篇) | 78 / 78 | **100%** (剩下的 3 跳過也補到 AC) |
| Day 22 + 二輪 + Day 23 新 19 題 | 97 / 97 | 100% |

**單輪 pass rate 系統性低估 AI 的 ceiling**. 判准不只用來「過或不過」, 更關鍵的是**失敗時提供錯的具體 signal**, AI 就能自己或靠一句提示爬回來.

---

## 我的收斂 — Takeaways

- **AI ceiling 在「有 oracle + 題型經典」場很高**: 五星 max flow 拆點 + 四星找零 DP 一次 AC, 不是運氣
- **ceiling 強烈依賴 pattern density**: ZeroJudge 多數題在訓練資料出現過, pass rate 看起來特別好看. 換成沒 pattern 的新題, 這個 ceiling 會塌多少, 這系列到此還沒碰
- **單輪 pass rate 低估 ceiling**: 開二輪 loop (WA/TLE/MLE 回饋) 全部救回. 判准最關鍵的不是「過或不過」, 是**失敗時提供具體 signal 讓 AI 自己改**
- **救援有兩種型態**: (A) 正確性 — 邊界 / 判題模型 / overflow, 一句 WA 訊息多半能救; (B) 效能 — TLE/MLE 的 oracle 盲區, 本機樣例完全看不出, 要靠判題給的 TLE/MLE 信號強制換演算法
- **c500 的教訓**: 強 oracle 也有盲區. 當盲區蓋住公開樣例, 自測全綠也會 WA — 要獨立 reference 才跳得出來
- **ZeroJudge 三部曲到此收尾**: Day 21 (概念) → Day 22 (規模) → Day 23 (深度 + 救援). 下一輪離開 OJ, 看判准難建的場怎麼辦

---

## Sources

- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 判准概念鋪陳
- [Day 22: 掃 ZeroJudge 78 題](https://ithelp.ithome.com.tw/articles/10421768) — 規模化 pass rate
- [ZeroJudge selected/ 19 題解答](../../../zerojudge/selected/README.md) — 本篇全部 cpp 原始碼
- [MCU CPE26 題目選集](../../../zerojudge/mcu-cpe26-problem-set.md) — 本篇按主題選題的 index
- [NSYSU 進階程式設計 UVa 星等](../../../zerojudge/nsysu-uva-ratings.md) — 難度標籤來源
- [ZeroJudge 競賽題庫 (含 c500 補 AC 筆記)](../../../zerojudge/contest/README.md) — c500 判題模型案例詳細 writeup
- [ZeroJudge](https://zerojudge.tw/) — 線上解題
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 自動送判題
