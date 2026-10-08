---
title: "AI 101 - 鐵人賽 Day 25: Blind 75 不夠, 再加碼 LC-75 + NC150"
tags: [ai, 鐵人賽, ironman, leetcode, lc75, neetcode150, 判斷標準, agentic, 實測, 草稿]
created: 2026-10-09
status: draft
---

# Day 25｜Blind 75 不夠, 再加碼 LC-75 + NC150 — Scaling the Pattern-Density Test Further

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 24](./day24-leetcode-blind75.md) 用 Blind 75 測 LeetCode pattern density 的上限, 69 AC + 6 PREMIUM 本地解, Hard 7 題全一次 AC. 問題來了 — 75 題會不會剛好是**pattern 最厚**那區, 結論站不住? 這篇把實驗再加兩套: **[LC-75 官方 study plan](https://leetcode.com/studyplan/leetcode-75/)** (65 新題) 跟 **[NeetCode 150](https://leetcode.com/problem-list/plakya4j/)** (64 新題 + 1 新 PREMIUM), 總共再加 **130 題**, 看結論撐不撐得住.

> **寫在前面** (jason3e7): 這篇是 Day 24 的放大版實驗, 想看第一輪結論是不是運氣. 擴到三套、去重後 **134 題送判題 + 7 PREMIUM 本地解**, 結論沒改但有一個小意外: 唯一一次 WA 不是解法錯, 是浮點誤差.

> **TL;DR (EN):** Scaled the Blind 75 test with the official LC-75 (65 new problems) and NeetCode 150 (64 new + 1 new PREMIUM). Combined fresh additions: **129 submitted + 1 locally solved = 130 problems**. Result: **128 / 129 first-pass AC (99.2 %)**. The single WA was LeetCode #50 Pow(x, n) — classic fast-exponentiation accumulated floating-point error when x ≈ -1 and |n| ~ 1.7e9; swapping to `std::pow` passed immediately. Hard problems covered broader territory this round: Dijkstra, Bellman-Ford, Hierholzer Euler path, interval DP (Burst Balloons), monotonic deque sliding window — all first-pass. The Day 24 claim ("AI is strong on OJ because the problems live inside the pattern bucket, not because of reasoning") survives a 2.7× scaling.

```markdown
# Blind 75 不夠, 再加碼 LC-75 + NC150
* 為什麼要加碼
  * Blind 75 可能是 pattern 最厚那區
  * 三套差一個數量級, 結論才敢固化
* 資料
  * LC-75 65 新題 全 AC
  * NC150 64 新題 + 1 新 PREMIUM
  * 加總 129 送判 + 1 本地
  * 唯一 WA Pow x n
* Pow x n 的 FP 救援
  * fast-pow 累積 7 位誤差
  * std::pow 單步到位
  * 這不是推理錯是數值錯
* NC150 擴到的題型
  * Dijkstra Bellman-Ford
  * Hierholzer 歐拉路徑
  * 區間 DP Burst Balloons
  * 單調 deque sliding max
* 收斂
```

---

## 為什麼要加碼 — Why Scale Up Again

Day 24 的 75 題結論看起來乾淨: **AI 強是因為題目在 pattern 範圍, 不是因為它會推理**. 但有一個邏輯漏洞 — Blind 75 **本身就是** StackOverflow / 教學部落格 / GitHub 題解出現最多次的那 75 題, 它可能是 pattern 密度最厚的那一塊, 樣本偏差太大, 結論不能外推.

要修這個漏洞, 加碼才有說服力. 挑兩套標準:

1. **跟 Blind 75 重疊少** — 多測幾題「以前沒出現過」的
2. **各有不同的題型偏好** — 覆蓋面要補到 Blind 75 沒碰到的角落

兩套最合適:

- **[LC-75 官方 study plan](https://leetcode.com/studyplan/leetcode-75/)** — LeetCode 2023 推出的官方 75 題, 跟 Blind 75 重疊 10 題, 新題 65 題. 題型偏「現代面試」, DP / 位元 / 設計題比例高
- **[NeetCode 150](https://leetcode.com/problem-list/plakya4j/)** — Blind 75 作者 Navi (NeetCode) 擴充的 150 題, 跟 Blind 75 + LC-75 兩套合計重疊 79 題, 新題 64 + 7 PREMIUM. 題型補「教科書演算法」, Dijkstra / Bellman-Ford / 區間 DP / 歐拉路徑都有

加起來新增 **130 題** (129 可送判 + 1 新 PREMIUM 本地解). 三套合在一起去重後 **200 題 LeetCode 清單**, 寫過了 198 (134 送判 AC + 7 新 PREMIUM 本地解 + 57 跟 Blind 75 重疊的 AC 標籤已拿). 樣本從 75 翻到 2.7 倍.

---

## 資料 — The Numbers

### LC-75 (65 新題)

**65 / 65 全 AC**. 挑三題講題型:

- **#2 Add Two Numbers** — 鏈結串列逐位加帶 carry, dummy head 建結果. 教科書等級, 幾秒內 AC
- **#2300 Successful Pairs of Spells and Potions** — potions 排序 + 二分找 ceil(success / spell) 下界. 現代面試題典型
- **#2542 Maximum Subsequence Score** — 按 nums2 降序, min-heap 保留 k 個最大 nums1, 當前 nums2 當 min 乘上去. 這題在 StackOverflow 的 pattern 直出沒有別的做法, agent 一次寫對

### NC150 (64 新題 + 1 新 PREMIUM)

**64 / 64 送判 AC, 1 本地解**. 跟 Blind 75 / LC-75 重疊的 79 題不重複, 完全是「新資料點」. 挑四題講題型:

- **#743 Network Delay Time** — Dijkstra. 標準 priority_queue + dist 陣列實作, 寫對了
- **#787 Cheapest Flights Within K Stops** — Bellman-Ford k+1 輪, 用 tmp 複本避免同輪污染. 這個「tmp 複本」細節是常見 WA 點 (在同一輪污染會變成「允許走 >k 跳」), agent 一次就寫對
- **#312 Burst Balloons** — 區間 DP, 外填 1 當邊界, 枚舉**最後**戳的 k. 不是「第一個戳的」是這題的關鍵, agent 寫對
- **#332 Reconstruct Itinerary** — Hierholzer 歐拉路徑, 後序 push 再反轉. 不是普通 DFS, agent 直接認出模板

**新 PREMIUM**: #286 Walls and Gates — 多源 BFS, 全部門一次性入 queue 一波波擴散. 經典題, 本地 harness 跑過, 送不出去.

### 加總 pass rate

| 清單 | 新題 | 送判 | 一次 AC | pass rate |
|:---|---:|---:|---:|---:|
| LC-75 | 65 | 65 | 65 | 100 % |
| NC150 | 64 | 64 | 63 | 98.4 % |
| **合計** | **129** | **129** | **128** | **99.2 %** |

唯一的 WA 是 #50 Pow(x, n), 下一節講細節.

---

## 唯一的失敗: Pow(x,n) 的 FP 救援 — The One WA

Agent 第一版寫的是標準 fast exponentiation:

```cpp
double myPow(double x, int n) {
    long long m = n;
    if (m < 0) { x = 1 / x; m = -m; }
    double res = 1;
    while (m) {
        if (m & 1) res *= x;
        x *= x;
        m >>= 1;
    }
    return res;
}
```

邏輯對、本機樣例全綠, 送上去 **Wrong Answer**. LeetCode 直接甩失敗測資:

```
Input:  x = -0.9999999968539456,  n = -1669585506
Output:    191.06373
Expected:  191.06370
```

差 3e-5. 不是演算法錯, 是**浮點精度在 ~30 層平方累積後飄了**. x 接近 -1、|n| 接近 2^31 這組邊界是 fast-pow 算法的經典死角.

Agent 自己讀 WA 訊息就改成 `return pow(x, (double)n);`, 一次 AC. **這題嚴格說不是「解法錯」, 是「選錯實作方式」** — 跟 Day 23 的 c500 「判題盲區」是同類: 樣例看不出來, 判題給具體 signal 之後一次補救.

> [!NOTE]
> **為什麼 `std::pow` 就過**: glibc 的 `pow(x, double)` 在內部用 `log(|x|) * y` 再 `exp`, 搭配 FMA (fused multiply-add) 跟更高位的中間計算, 整體誤差比「乘 30 次」低得多. 不是「演算法比較高級」, 是「用了更精細的實作一次把乘法做完」. 這再次呼應 Day 24 的結論 — AI 不是推理出哪種好, 它只是**換一個訓練資料裡出現更多的實作** (`pow()` 是 C 幾十年的標配).

---

## NC150 擴到的題型 — What NC150 Covers That Blind 75 Doesn't

NC150 加進 Blind 75 沒碰到的教科書演算法, 這批題寫對的話, Day 24 的 pattern-density 說法才算真的站穩. 一次點名:

| 類別 | NC150 代表題 | 題型 |
|:---|:---|:---|
| 最短路 | #743 Network Delay Time | Dijkstra |
| 有邊數限制最短路 | #787 Cheapest Flights | Bellman-Ford |
| 歐拉路徑 | #332 Reconstruct Itinerary | Hierholzer |
| 區間 DP | #312 Burst Balloons | 從裡往外枚舉最後動作 |
| 鏈序列 DP | #115 Distinct Subsequences | 子序列匹配計數 |
| 單調 deque | #239 Sliding Window Maximum | O(n) 滑動窗最大 |
| MST | #1584 Min Cost Connect Points | Prim's O(n²) |
| 多源 BFS (PREMIUM) | #286 Walls and Gates | 多源同時擴散 |
| Union-Find | #684 Redundant Connection | 第一條成環邊 |
| 迴圈偵測 | #287 Find Duplicate Number | Floyd 龜兔跑 (nums 當隱式鏈) |

這些題在教科書 (CLRS / 算競入門 / NeetCode 影片) 都有乾淨的對應模板, agent 認出題型後幾乎零摩擦寫出來. **沒有一題需要「推理出」解法, 全是「取出配方」**.

---

## 真正會卡的是什麼 — Where It Still Breaks

三套刷下來, 真正卡住或 agent 自己踩坑的只有:

1. **Pow(x, n) 的 FP 誤差** — 解法對, 數值精度不夠. 判題給具體 WA 一次救
2. **幾題 Playwright 送判題 race** — Monaco setValue + click 寫在同一 evaluate() 偶爾送出前一版 stub, 已在 Day 24 講過; 這 130 題還是會偶發, 繼續用「分兩次 evaluate」做 workaround
3. **PREMIUM 鎖住** — 7 題 (6 從 Blind 75 延續 + 1 新的 Walls and Gates) 沒有外部判斷標準. agent 自測全綠不代表對, 這批跟 Day 24 一樣**嚴格說不算 AC**

這三個都不是「推理不夠」. 第 1 是數值, 第 2 是平台, 第 3 是沒 reference. 三種失敗模式都在 **AI 推理能力範圍之外**.

---

## 我的收斂 — Takeaways

- **Blind 75 不是 cherry-picked, LC-75 + NC150 加碼確認結論**: 加 130 題後 pass rate 99.2%, 跟 Day 24 的「Blind 75 Hard 全 AC」是同一條曲線. 結論從 75 題放大到 ~200 題不塌
- **唯一 WA 不是推理, 是 FP 精度**: #50 Pow 的失敗是「演算法對、實作選錯」, agent 讀 WA 一次救回. 這再次證明 AI 在 OJ 上的「強」是 pattern 認得 + 配方齊全, 不是會推理
- **教科書演算法全在 pattern bucket 內**: Dijkstra / Bellman-Ford / Hierholzer / 區間 DP / 多源 BFS / Union-Find 這些算法課的招式, 一題一題全一次 AC. 這跟 Day 24 的 BFS 序列化 / 雙 heap / Trie 回收是同件事 — **教科書在訓練資料裡出現得太多次, 它變成 autocomplete**
- **真正會卡的三種情境都在推理之外**: FP 精度 / 平台 race / 無判斷標準 (PREMIUM). AI 的「不能」在這三個地方特別清楚
- **200 題寫完的意義**: 這份 repo 的 [leetcode/](../../../leetcode/) 現在是 **134 送判 AC + 7 PREMIUM 本地解 = 141 題 .cpp**, 加上三套 study plan 的進度表. 不是為了拿 badge, 是為了把 Day 24-25 這個「pattern density = AI 上限」結論用數據鎖死, 下一輪離開 OJ 判斷標準明確的場, 這個結論才派得上用場

---

## Sources

- [LC-75 官方 study plan (LeetCode)](https://leetcode.com/studyplan/leetcode-75/) — 加碼的第一套
- [NeetCode 150 清單 (LeetCode)](https://leetcode.com/problem-list/plakya4j/) — 加碼的第二套
- [leetcode/ 全部解答](../../../leetcode/README.md) — 本篇所有 cpp 原始碼與進度表
- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 判斷標準概念鋪陳
- [Day 23: ZeroJudge 進階測試](./day23-zerojudge-cross-tier.md) — 中文 OJ 同一條觀察
- [Day 24: 用 Claude Code 全自動刷 LeetCode Blind 75](./day24-leetcode-blind75.md) — 本篇的前作
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 自動送判題
