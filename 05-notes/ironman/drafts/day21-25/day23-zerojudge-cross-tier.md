---
title: "AI 101 - 鐵人賽 Day 23: ZeroJudge 收尾, 挑難的看看 — 跨難度跨主題 19 題還是全 AC"
tags: [ai, 鐵人賽, ironman, zerojudge, 判准, agentic, 實測, 草稿]
created: 2026-10-08
status: draft
---

# Day 23｜ZeroJudge 收尾: 挑難的看看, AI 自己把 Max Flow + DP 都 AC 了 — Scaling Across Difficulty, Not Just Count

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 22](./day22-zerojudge-at-scale.md) 掃 78 題 96.2% AC 看起來漂亮, 但一個合理質疑是: 這 78 題分佈在 ZeroJudge 的「基礎 / 競賽 / UVa」前幾頁, 以題庫 convention 偏入門. 真正**挑難的、挑冷門主題**, AI 的 self-check loop 還撐得住嗎? 這篇再抽 19 題做一次對照 — 刻意跨主題 (8 類) + 跨難度 (★ 到 ★★★★★), 包含一題 **node-capacity max flow** 跟一題 **找零方案數 DP**. 結果: **全 AC**.

> **寫在前面** (jason3e7): 這是 ZeroJudge 三部曲的收尾 (Day 21 概念 → Day 22 規模 → Day 23 深度). 下一輪開始走向「判准不好做」的場, 不會再抱著 OJ 這種奢侈 oracle 不放.

> **TL;DR (EN):** Day 22 scaled breadth (78 problems) but kept difficulty narrow (easy pages). Day 23 scales **depth**: 19 hand-picked problems across 8 topic categories, ★ to ★★★★★, picked using the [NSYSU rating index](../../../zerojudge/nsysu-uva-ratings.md) built earlier. Includes UVa 10330 (★★★★★, node-capacity max flow — needs the split-node trick) and UVa 147 (★★★★, change-counting DP). **19/19 AC**, same single-pass rules as Day 22. Caveat is the same: ZeroJudge is an unusually clean oracle. The ceiling is high *inside* that bubble; most real work has no such oracle, which is where this series goes next.

```markdown
# ZeroJudge 收尾: 挑難的看看, AI 自己把 Max Flow + DP 都 AC 了
* 為什麼再測一次 (Day 22 只掃前幾頁, 不夠挑戰)
* 選題 (靠 NSYSU 1065 題星等當 index)
  * 跨 8 主題
  * 跨 ★ 到 ★★★★★
  * 19 題 = 10 入門 + 3 中等 + 3 硬題 + 3 冷門
* 結果 (19/19 全 AC)
* 三題硬菜拆解
  * 10330 Max Flow 拆點
  * 147 Dollars DP
  * 122 Tree 建構
* 收斂: ceiling 比預期高, 但只在有 oracle 的場
```

---

## 為什麼再測一次 — Why Scale Depth, Not Just Breadth

Day 22 掃 78 題 96.2% AC, 看起來很強, 但有個誠實的質疑:

- **78 題多是 ZeroJudge 第 1-2 頁** — 題庫 convention 是**入門→進階**排序, 前面幾頁偏簡單 (哈囉、閏年、迴文、GCD 這種)
- **跳過的 4 題 + 失敗的 3 題** 都還落在「中等」區間, 真的「演算法課才會教」的題幾乎沒出現
- 這樣的 pass rate 是「easy baseline 偏強」還是「ceiling 確實高」, 分不清楚

所以這篇**換個抽樣法**: 不再按題庫頁次掃, 而是用上一輪新建的 [NSYSU 1065 題 UVa 星等索引](../../../zerojudge/nsysu-uva-ratings.md) 當 index, 刻意**跨難度跨主題挑**, 看看真正硬的題 AI 自己 self-check 能不能 AC.

---

## 選題 — The 19

總共 **19 題**, 挑選規則:

- **8 個主題各挑 1-3 題** (規則模擬、字串、二維寫真、DP、max flow、樹、排序、進制 ⋯⋯)
- **難度分佈刻意拉開**: ★ 10 題 + ★★ 2 題 + ★★★ 1 題 + ★★★★ 1 題 + ★★★★★ 1 題 + 4 題 NSYSU 沒評 (多為 2023+ 新題)
- 全部已存到 [`zerojudge/selected/`](../../../zerojudge/selected/README.md) 一題一檔

| ZeroJudge | UVa | 題名 | NSYSU | 主題 |
|:---|---:|:---|:---|:---|
| [d760](https://zerojudge.tw/ShowProblem?problemid=d760) | 10330 | Power Transmission | ★★★★★ | 節點容量 max flow |
| [d397](https://zerojudge.tw/ShowProblem?problemid=d397) | 147 | Dollars 找零方法數 | ★★★★ | DP |
| [c101](https://zerojudge.tw/ShowProblem?problemid=c101) | 122 | Trees on the level | ★★★ | 二元樹建構 |
| [a539](https://zerojudge.tw/ShowProblem?problemid=a539) | 10327 | Bubble Sort 交換次數 | ★★ | 排序 (逆序數) |
| [e592](https://zerojudge.tw/ShowProblem?problemid=e592) | 10142 | Australian Voting | ★★ | 規則模擬 |
| [a536](https://zerojudge.tw/ShowProblem?problemid=a536) | 11689 | 收集空瓶換汽水 | ★ | 規則模擬 |
| [c007](https://zerojudge.tw/ShowProblem?problemid=c007) | 272 | TeX Quotes | ★ | 字串 |
| [c015](https://zerojudge.tw/ShowProblem?problemid=c015) | 10018 | Reverse and Add | ★ | 規則模擬 |
| [a518](https://zerojudge.tw/ShowProblem?problemid=a518) | 12468 | Zapping | ★ | 算子 |
| [d120](https://zerojudge.tw/ShowProblem?problemid=d120) | 10699 | 相異質因數個數 | ★ | 質因數 |
| [e605](https://zerojudge.tw/ShowProblem?problemid=e605) | 10189 | Minesweeper | ★ | 二維寫真 |
| [j056](https://zerojudge.tw/ShowProblem?problemid=j056) | 11650 | Mirror Clock | ★ | 日期時間 |
| [d096](https://zerojudge.tw/ShowProblem?problemid=d096) | 913 | Joana and the Odd Numbers | ★ | 二維排數 |
| [e706](https://zerojudge.tw/ShowProblem?problemid=e706) | 12820 | Cool Word | ★ | 集合映對 |
| [e155](https://zerojudge.tw/ShowProblem?problemid=e155) | 10935 | Throwing Cards Away | ★ | 堆疊佇列 |
| [d379](https://zerojudge.tw/ShowProblem?problemid=d379) | 446 | Hex 相加減 | — | 進制 (NSYSU 未評) |
| [d094](https://zerojudge.tw/ShowProblem?problemid=d094) | 478 | Point in Figures | — | 窮舉 / 幾何 (NSYSU 未評) |
| [c061](https://zerojudge.tw/ShowProblem?problemid=c061) | 530 | Binomial C(n,m) | — | 排列組合 (NSYSU 未評) |
| [e208](https://zerojudge.tw/ShowProblem?problemid=e208) | — | Run-length Decoding | — | 字串 (ZJ 原生題) |

規則跟 Day 22 一樣: 本機 g++ 跑樣例 → Playwright MCP 送判題 → 單次送出, 不重試.

---

## 結果 — The Numbers

**19 / 19 全 AC**. 包含:

- UVa 10330 (NSYSU ★★★★★) ✅
- UVa 147 (NSYSU ★★★★) ✅
- UVa 122 (NSYSU ★★★) ✅

比起 Day 22 的 75/78 = 96.2%, 這批**反而從 96% 跳到 100%** — 一個反直覺的結果. 原因不是題變簡單 (事實上有一題五星), 而是**抽樣改了**: 這 19 題多是 UVa 經典 (「教科書等級的題」), 跟 Day 22 包含的 ZeroJudge 原生題、校內競賽題比, AI 練過的機率更高, 樣例也更乾淨.

這本身是一個觀察: **AI 的 pass rate 受「題目有多經典」影響很大**, 不只是「難度」.

---

## 三題硬菜 — The Three Harder Ones

### UVa 10330 Power Transmission (★★★★★) — 節點容量 Max Flow

這題題意是 power grid 要算最大供電量, 但**節點本身也有容量上限** — 不是標準的「邊有容量」最大流.

教科書做法: **拆點 (node splitting)**. 把每個節點 v 拆成 v_in 跟 v_out 兩個, 中間連一條容量 = 原節點容量的邊; 原本連到 v 的入邊接到 v_in, 原本從 v 出的邊接到 v_out. 這樣就把「節點容量」問題降階成標準 max flow.

AI 自己想到這個拆點做法, 寫出 Dinic 或 Edmonds-Karp, 本機樣例過, 送判一次 AC. 這題我自己高中讀過時卡了半天, 看它直接寫出來還是有點震撼.

> [!NOTE]
> 一個合理解釋: 拆點 + max flow 是教科書 pattern, 在訓練資料裡大量出現. AI 不是「推理出」這個技巧, 更接近「認出題型後取出配方」. 這就跟 [Day 03 的 LLM 限制分類](https://ithelp.ithome.com.tw/articles/10412787) 說的一致 — **它強在 pattern recognition, 弱在真正新的推理**. OJ 題幾乎全在 pattern 範圍內, 所以 AI 的 ceiling 看起來特別高.

### UVa 147 Dollars (★★★★) — 找零方案數 DP

給一組硬幣面額 (美金的 5¢ / 10¢ / 25¢ / 50¢ / $1 ⋯⋯), 問湊出目標金額有幾種方法. 經典的**找零方案數 DP** — 不是找「最少硬幣」, 是找「有幾種組合」, 外迴圈跑面額、內迴圈跑金額才對 (順序反了會算成「排列」而不是「組合」).

AI 一次寫對, 外內迴圈順序正確, 樣例全綠, 送判 AC.

### UVa 122 Trees on the level — 二元樹建構

輸入是一堆 `(value,path)` 格式的節點 (`path` 是 "LRL" 這種 L/R 字串標示到根的路徑), 要**先建樹再做 level-order traversal (BFS)**. 關鍵在**檢查結構一致性**: 同一個節點不能被定義兩次、也不能有節點缺少 parent.

AI 寫得乾淨, 用 map 存節點 + 旗標偵測重複/缺漏, 一次 AC.

---

## 我的收斂 — Takeaways

- **AI 的 ceiling 在「有 oracle + 題型經典」的場比我以為的高很多**. 一題五星 max flow 自己拆點、找零 DP 內外迴圈順序對, 都不是靠運氣
- **但這高 ceiling 強烈依賴「訓練資料的 pattern 密度」** — ZeroJudge 的 UVa 題大多在 LLM 訓練資料裡出現過. 換成**沒 pattern 的新題** (例如你自己 domain 的業務邏輯), 這個 ceiling 會塌多少, 這系列到此還沒碰
- **ZeroJudge 三部曲到此收尾**:
  - [Day 21](./day21-back-to-verifiable-ground.md): 概念 — 文字難驗, 程式好驗, 判准是分水嶺
  - [Day 22](./day22-zerojudge-at-scale.md): 規模 — 78 題同一種題庫掃過去, 96.2% AC
  - Day 23 (本篇): 深度 — 19 題跨難度跨主題挑, 全 AC
- 下一輪開始走出「有乾淨 oracle」的奢侈場, 看看判准難建的時候, 這套 agentic loop 要怎麼轉

---

## Sources

- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 判准概念鋪陳
- [Day 22: 掃 ZeroJudge 78 題](https://ithelp.ithome.com.tw/articles/10421768) — 規模化 pass rate
- [ZeroJudge selected/ 19 題解答](../../../zerojudge/selected/README.md) — 本篇全部 cpp 原始碼
- [NSYSU 進階程式設計 UVa 星等](../../../zerojudge/nsysu-uva-ratings.md) — 本篇選題用的難度 index
- [MCU CPE26 題目選集](../../../zerojudge/mcu-cpe26-problem-set.md) — 另一份選題參考 (本輪 12/19 屬於 CPE26 選集範圍)
- [ZeroJudge](https://zerojudge.tw/) — 線上解題
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 自動送判題
