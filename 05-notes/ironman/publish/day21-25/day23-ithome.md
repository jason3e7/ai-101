title : [Day 23] ZeroJudge 進階測試: 按主題各挑一題, 看 AI 的上限跟自動多輪的能力

> [Day 22](https://ithelp.ithome.com.tw/articles/10421768) 掃 78 題 96.2% AC 是「廣度」, 但那批題分佈在 ZeroJudge 前幾頁, 主題有偏食. 這篇換「深度」: 用 [MCU CPE26 題目選集](https://cpe.mcu.edu.tw/cpelist.php) 的 14 個主題分類當 index, **每主題各挑一題** + 另外**從 ★ 到 ★★★★★ 各挑一題**做難度階梯, 總共 19 題再跑一次. 另外補一段 Day 22 當時卡住的題後來怎麼多輪 AC 救回來, 把「單次送出的 pass rate 低估了上限」這件事講清楚.

寫在前面 (jason3e7):

> 這是 ZeroJudge 三部曲的收尾 (Day 21 概念 → Day 22 規模 → Day 23 深度 + 救援). 下一輪離開 OJ 這種理想的判斷標準, 進入判斷標準難建的場.

## 進階測試測什麼 — What This Advanced Round Tests

Day 22 掃 78 題 96.2% AC 看起來強, 但有兩個合理質疑:

1. **主題有偏食** — 78 題多是 ZeroJudge 第 1-2 頁, 題庫 convention 偏入門, 題型集中在哈囉、閏年、迴文、GCD 這類
2. **真的硬題幾乎沒出現** — 跳過的 4 題、失敗的 3 題都還落在「中等」區間, 教科書等級 (max flow / DP) 的題一題都沒碰到

這篇換個抽樣法, 用 [MCU CPE26 題目選集](https://cpe.mcu.edu.tw/cpelist.php) 當 index — 這 56 題是 MCU 為 2027 起 CPE 檢定分 14 個主題各 4 題挑的題庫, **主題覆蓋面是現成的**. 做法:

- **每主題挑 1 題** → 14 題覆蓋 CPE26 的全部主題分類
- **外加 5 題: 刻意從 NSYSU ★ 到 ★★★★★ 各挑一題** → 看難度階梯對解題成功率跟手感的影響

共 **19 題**.

## 選題 — The 19

### 14 題覆蓋 CPE26 的所有主題

| CPE26 主題 | ZeroJudge | UVa | 題名 | NSYSU |
|:---|:---|---:|:---|:---|
| 輸入格式 | [c007](https://zerojudge.tw/ShowProblem?problemid=c007) | 272 | TeX Quotes | ★ |
| 字串字元 | [e208](https://zerojudge.tw/ShowProblem?problemid=e208) | 11541 | Decoding (Run-length) | ★ |
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

### 5 題: NSYSU ★ 到 ★★★★★ 各一題做難度對照

另外 5 題不是為了覆蓋主題, 是**刻意從 NSYSU 1 星到 5 星各挑一題**, 看難度階梯對 AI 解題成功率跟手感的影響:

| ZeroJudge | UVa | 題名 | NSYSU | 題型 |
|:---|---:|:---|:---|:---|
| [a536](https://zerojudge.tw/ShowProblem?problemid=a536) | 11689 | 收集空瓶換汽水 | ★ | 規則模擬 |
| [e592](https://zerojudge.tw/ShowProblem?problemid=e592) | 10142 | Australian Voting | ★★ | 多輪淘汰模擬 |
| [c101](https://zerojudge.tw/ShowProblem?problemid=c101) | 122 | Trees on the level | ★★★ | 二元樹建構 + BFS |
| [d397](https://zerojudge.tw/ShowProblem?problemid=d397) | 147 | Dollars 找零方法數 | ★★★★ | DP |
| [d760](https://zerojudge.tw/ShowProblem?problemid=d760) | 10330 | Power Transmission | ★★★★★ | 節點容量 max flow |

**難度分佈**: NSYSU 評分的 16 題裡 ★ 11 題 / ★★ 2 題 / ★★★ 1 題 / ★★★★ 1 題 / ★★★★★ 1 題; 另外 3 題 NSYSU 沒收 (皆 2023+ 新題).

送題規則跟 Day 22 完全一樣: 本機 g++ 跑樣例 → Playwright MCP 送判題 → **單次送出, 不重試**.

## 結果 — The Numbers

**19 / 19 全 AC**. 包含:

- UVa 10330 (★★★★★) ✅
- UVa 147 (★★★★) ✅
- UVa 122 (★★★) ✅

從 Day 22 的 96.2% 反而**跳到 100%** — 不是題變簡單, 是**抽樣改變了**: 這批 19 題多是 UVa 經典, 比起 Day 22 包含的 ZeroJudge 原生題與校內競賽題, AI 練過的機率更高, 樣例也更乾淨.

這件事本身就是一個觀察: **AI 的 pass rate 受「題目有多經典」影響很大, 不只是「難度」**. 一題 ★★★★★ 的 max flow 經典題, 比起一題 ★ 但沒出現在訓練資料的校內題, 前者反而容易過.

## 三難題拆解 — Three Hard Problems Walked Through

### UVa 10330 Power Transmission (★★★★★) — 節點容量 Max Flow

題意是 power grid 要算最大供電量, 但**節點本身也有容量上限** — 不是標準「邊有容量」的 max flow.

教科書做法: **拆點 (node splitting)**. 把每個節點 v 拆成 v_in / v_out, 中間連一條容量 = 原節點容量的邊; 原本連到 v 的入邊接到 v_in, 從 v 出的邊接到 v_out. 這樣就把「節點容量」降階成標準 max flow.

AI 自己想到拆點, 寫出 Edmonds-Karp, 本機樣例過, 送判一次 AC.

> **註：** 一個合理解釋是, 拆點 + max flow 是教科書 pattern, 在訓練資料裡大量出現. AI 不是「推理出」這個技巧, 更接近「認出題型後取出配方」. 這跟 [Day 03 的 LLM 限制分類](https://ithelp.ithome.com.tw/articles/10412787) 說的一致 — **它強在 pattern recognition, 弱在真正新的推理**. OJ 題幾乎全在 pattern 範圍內, 所以 AI 的上限看起來特別高.

### UVa 147 Dollars (★★★★) — 找零方案數 DP

給一組硬幣面額, 問湊出目標金額有幾種組合. 經典的**找零方案數 DP** — 不是「最少硬幣」, 是「有幾種組合」, **外迴圈跑面額、內迴圈跑金額**才對 (順序反了會變成算排列).

AI 一次寫對, 迴圈順序正確, 送判 AC.

### UVa 122 Trees on the level — 二元樹建構

輸入是一堆 `(value,path)` 格式的節點 (`path` 是 "LRL" 這種 L/R 字串表示到根的路徑), 要**先建樹再做 level-order traversal (BFS)**. 關鍵在**檢查結構一致性** (節點不能被定義兩次, 也不能缺少 parent).

AI 用 map 存節點 + 旗標偵測重複/缺漏, 一次 AC.

## 自動多輪直到 AC — Agent Retries Until AC

單輪送出的 pass rate 低估上限. 把規則從「單次送出就記錄」放寬成「允許多輪」, 其他完全不變 — **一樣是 agent 自己送、自己讀 verdict、自己改、再送**, 我只指定題目, 沒餵訊息、沒提示方向. 多題就能爬回來.

分兩種救援型態 — **正確性類** (邏輯 / 邊界 / 範圍條件錯) 跟 **效能約束類** (時間 / 記憶體超限).

### A. 正確性類: 邏輯 / 邊界 / 範圍 (基礎題庫)

| 題號 | 當時 | 關鍵 fix |
|:---|:---|:---|
| [a095](https://zerojudge.tw/ShowProblem?problemid=a095) 麥哲倫的陰謀 | Day 22 NA 50% | special-case `M == N` (全紅帽無白帽) |
| [a215](https://zerojudge.tw/ShowProblem?problemid=a215) 明明愛數數 | Day 22 WA line 7 | n/m 可為負數 + `__int128` 防 overflow |
| [b590](https://zerojudge.tw/ShowProblem?problemid=b590) 單位分數分解 | Day 22 原本跳過 | 允許多輪後, agent 改用 DFS + 剪枝重寫 |

### B. 效能約束類: TLE / MLE (ORIGINAL 題庫校內原創題)

這幾題一開始**邏輯都對、樣例也過**, 但判題直接甩 **TLE** 或 **MLE** — 效能約束是另一種「公開樣例看不見」的盲區. 救援的方式不是改邏輯, 是**換資料結構 / 換演算法 / 換 IO**. 跟 A 類一樣, 多輪完全是 agent 自己送判題、讀 TLE/MLE 訊息、自己 refactor.

| 題號 | 當時 | 關鍵 fix | 加速 |
|:---|:---|:---|:---|
| [s142](https://zerojudge.tw/ShowProblem?problemid=s142) 最大正方形 | MLE (10MB 限制) | 2D dp → **滾動 1D dp**, 邊讀邊算, 不存整個矩陣 | 空間 O(nm) → O(m) |
| [s794](https://zerojudge.tw/ShowProblem?problemid=s794) 1A2B | TLE | 關鍵觀察: 猜測各 (A,B) 桶的大小**只取決於數字重數結構**, 用小查表 O(1) 查, 只對「最小桶」的提示建完整 bucket | 單輪 O(N²) → O(表) |
| [s796](https://zerojudge.tw/ShowProblem?problemid=s796) 蜂蜜工廠 | TLE | Matroid 貪心 + **線段樹** 加速區間可達查詢, 鏈式左移/右移快路徑先試, Kuhn's 二分圖匹配當 fallback | 多個 O(N²) 操作各降 log 階 |

這 3 題都是**本機樣例看不出來, 送判題才知道效能不夠**. 判題在這裡扮演兩個角色: (1) 給出 TLE/MLE 的明確信號 (2) 強制 agent 跳出「樣例過了就以為對」的錯覺.

### 共通點: 判斷標準的真正價值是「失敗時給具體 signal」

兩種救援合起來看, pattern 一致:

- **樣例全綠不代表對** — 可能是邊界沒蓋到 (a095)、範圍條件漏讀 (a215)、放棄太早 (b590)、效能不夠 (s142/s794/s796)
- **單輪送出的 pass rate 低估上限** — 只是把規則從「一次就定生死」改成「允許多輪」, agent 自己讀 WA/TLE/MLE 訊息就能爬回來, 不需要人提示
- **判斷標準不是只分「過 / 不過」**, 是**失敗時給具體 signal 讓 agent 自己 refactor** — 這才是 OJ 這類判斷標準真正有價值的地方

### a095 — 邊界條件被忽略

Day 22 的解法沒處理 `M == N` (全部都是紅帽) 這個 edge case. 允許多輪後, agent 從 NA 50% 的訊息自己回推, 修成 `(M == N ? M : M+1)` AC. **主邏輯對、邊界錯**的 WA, 開二輪 loop 最好修.

### a215 — 題目條件被忽略 + 數值 overflow

兩個獨立問題疊在一起: (1) 題目說 n, m 可以是負數, agent 一開始的 while 條件沒處理「`n > m` 時至少數一個」; (2) 累加可能 overflow long long. 多輪後 agent 自己讀 WA line:7 的訊息, 回頭檢查題目範圍條件, 改成 `do-while` + `__int128` AC.

### b590 — Day 22 原本放棄的題

Day 22 agent 自己讀題後說「樣例對不上, 解題模型未定」就跳過了. 允許多輪後它重新讀題, 改以 **DFS 枚舉分母 + 剪枝** 的做法重寫, AC.

### c500 — agent 自己去 Google 答案的那題 (誠實交代)

c500 (AEWE-645 的傷害) 也是 Day 22 卡住的題, 單次送出 NA 0%. 開放多輪後 agent 自己試了幾版還是 NA 0% — **本機樣例全綠, 判題只回「NA 0%」沒給具體 WA 訊息** (這題有 4 個 subtask, 全掛), agent 幾次 refactor 都卡在同一個分數上下, 顯然撞牆了.

**撞牆之後 agent 自己打開瀏覽器去 Google**, 找到作者寫的解題報告讀了一遍, 才搞懂坑在哪.

- 題意表面是: n 個座位排一直線, 兔吉在第 m 格; k 隻滑鼠彼此至少間隔 f 格, 位置 > m 的那隻才造成 `d·t` 傷害 (t = 離 m 的距離)
- **判題用的分佈模型**是「從第 1 格起、每 f 格放一隻」(位置 1, 1+f, 1+2f, ..., 1+(k-1)·f), 不是「找物理最佳的最壞分佈」; 位置 ≤ m 的免計傷害, **包含剛好落在 m 的那隻也算 0**
- 關鍵盲區: 兩個模型**只在 `m ≡ 1 (mod f)` 時結果才會不同**. 兩組公開樣例剛好一組 f=3 沒碰撞、一組 f=1 但 k 太小沒越過 m, 都測不出差

讀懂作者的分佈模型後, agent 一次 refactor (左側數量用 `⌊(m-1)/f⌋+1`, 右側位置直接 `1+(Lcnt+j)·f`), AC.

> **註：** 這題嚴格說算不算 AI 自己解, 看怎麼定義 — agent 不是靠自己推理解出, 是**自己去 Google 搜到作者的解題報告**才解開. 寫在這裡是因為兩個教訓都值得看: (1) **強判斷標準也有盲區, 當盲區剛好蓋住公開樣例時, 自測全綠也會 WA**; agent 自己 brute 對拍只會「跟自己同一個錯誤模型」互相證明對, 要跳出同溫層得找一個**跟題目來源獨立**的參照. (2) **agentic workflow 的「作弊」邊界其實很模糊** — 當 agent 卡住會自己上網找答案, 「AI 自己解」跟「AI 自己 Google」的界線就不清楚了. 這呼應 [Day 21 「綠燈不等於對」](https://ithelp.ithome.com.tw/articles/10421407) — 判斷標準越強, 盲區可能越細.

## 我的收斂 — Takeaways

- **AI 上限在「有判斷標準 + 題型經典」場很高**: 五星 max flow 拆點 + 四星找零 DP 一次 AC, 不是運氣
- **上限強烈依賴 pattern density**: ZeroJudge 多數題在訓練資料出現過, pass rate 看起來特別好看. 換成沒 pattern 的新題, 這個上限會塌多少, 這系列到此還沒碰
- **單輪 pass rate 低估上限**: 開二輪 loop (WA/TLE/MLE 回饋) 全部救回. 判斷標準最關鍵的不是「過或不過」, 是**失敗時提供具體 signal 讓 AI 自己改**
- **救援有兩種型態**: (A) 正確性 — 邊界 / 判題模型 / overflow, 一句 WA 訊息多半能救; (B) 效能 — TLE/MLE 的判斷標準盲區, 本機樣例完全看不出, 要靠判題給的 TLE/MLE 信號強制換演算法

## Sources

- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 判斷標準概念鋪陳
- [Day 22: 掃 ZeroJudge 78 題](https://ithelp.ithome.com.tw/articles/10421768) — 規模化 pass rate
- [MCU CPE26 題目選集](https://cpe.mcu.edu.tw/cpelist.php) — 本篇按主題選題的 index
- [ZeroJudge](https://zerojudge.tw/) — 線上解題
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 自動送判題
