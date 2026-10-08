---
title: "AI 101 - 鐵人賽 Day 24: 用 Claude Code 刷 Blind 75, 它幫到哪、哪裡得自己來"
tags: [ai, 鐵人賽, ironman, leetcode, blind75, 判斷標準, agentic, 實測, 草稿]
created: 2026-10-08
status: draft
---

# Day 24｜用 Claude Code 刷 Blind 75: AI 幫到哪、哪裡得自己來 — LeetCode Blind 75 With an Agent in the Loop

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 21-23](./day23-zerojudge-cross-tier.md) 的 ZeroJudge 三部曲是「廣度 + 深度 + 救援」, 題庫是台灣 OJ 的入門到五星. 這篇換 LeetCode, 挑 **Blind 75** 這份 FAANG 面試人人都刷過的清單, 看在**訓練資料見過最多次的題庫**上, agent 自己刷能走到哪. 流程跟 ZeroJudge 一致 — Claude 讀題、寫 `.cpp`、本機 g++ 跑範例、Playwright MCP 塞進 Monaco 編輯器送判題、讀 verdict. 我只負責登入.

> **寫在前面** (jason3e7): 這篇是純刷題的 side-by-side 觀察, 不是教程. 想看 AI 在高 pattern density 題庫 (LeetCode 經典題) 上的手感跟盲區. 收尾順便補一句 LC-75 官方 study plan 也跟著掃完.

> **TL;DR (EN):** Agent-driven run of the Blind 75 list. **69 / 75 AC on-judge, 6 remaining are PREMIUM-locked** so I solved them locally with a C++ harness but can't submit — effectively 75 / 75 written. Then rolled the official LC-75 study plan with 10 overlapping, 65 new, all AC — 75 / 75. Blind 75 is the densest pattern bucket in training data, so AC rate is near-ceiling: Hard problems (Merge k Lists, Min Window Substring, Max Path Sum, Word Search II, Median from Stream, Serialize Tree) all first-pass. Where humans still carry weight: writing a local `#ifdef LOCAL` harness for the PREMIUM-locked problems (no external judge to lean on), and the Playwright glue — Monaco `setValue` races the Submit click often enough that inject and click have to be two separate evaluates.

```markdown
# 用 Claude Code 刷 Blind 75
* 為什麼挑 Blind 75
  * FAANG 面試清單
  * 訓練資料見過最多次
  * 覆蓋八大題型
* 流程
  * 本機 cpp + ifdef LOCAL harness
  * g++ 跑樣例
  * Playwright MCP 塞 Monaco + 送判題
* 結果
  * 69 AC + 6 PREMIUM 本地解
  * LC-75 跟著掃 75 全 AC
  * 幾題 Hard 的手感
* AI 幫到哪
  * 經典題 pattern 直出
  * Hard 不卡
  * 批次吃掉 30 題不喘
* 哪裡得自己來
  * PREMIUM 鎖住要寫本地判斷標準
  * Monaco setValue 跟 submit race
  * 平台細節 cookie session
* 收斂
```

---

## 為什麼挑 Blind 75 — Why Blind 75

[Blind 75](https://leetcode.com/problem-list/oizxjoit/) 是 2020 年一個叫 Yangshun 的工程師在 Teamblind 匿名論壇整理的 75 題清單, 這幾年變成**最被刷爆的面試題庫**. 挑它的三個理由:

1. **覆蓋面廣** — 陣列 / 字串 / 連結串列 / 二叉樹 / 圖 / DP / 位元 / 堆 / Trie 都有, 八大類題型全覆蓋到
2. **訓練資料裡的 pattern density 爆表** — 這 75 題在 StackOverflow / 教學部落格 / GitHub 題解 repo 出現過**無數次**, 幾乎是 LLM 刷題能力的「上限場」. 這裡的 pass rate 塌下來, 其他地方就不用期待了
3. **題目分級剛好** — 15 Easy / 50 Med / 10 Hard 的分佈, 能一次測三個難度段

對照 Day 22-23 的 ZeroJudge, 兩邊都是 OJ 題, 差別在「英文題幹 + 美式演算法傳統」vs「中文題幹 + 台灣 OJ convention」, 中英文題庫的手感差異也能順便測一下.

---

## 流程 — The Loop

跟 ZeroJudge 一樣的 agentic loop, 只換送題目的 target:

```
Claude 讀 LeetCode 題頁
  ↓
寫 class Solution + 本機 harness (包在 #ifdef LOCAL)
  ↓
g++ -O2 -std=c++17 -DLOCAL 編, 跑範例自測
  ↓ (樣例不過: 自己改)
  ↓ (樣例過)
Playwright MCP 把 class Solution 塞進 Monaco 編輯器
  ↓
點 Submit, 讀 "Accepted" / "Wrong Answer" / "Runtime Error"
  ↓
下一題
```

### 本機 cpp 架構

每題一個 `.cpp` 檔, 內含 `class Solution` + `#ifdef LOCAL` 區的 `main()` harness:

```cpp
// LeetCode #1 Two Sum (Easy)
// Hash map 一遍過, O(n)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int,int> seen;
        for (int i = 0; i < (int)nums.size(); i++) {
            auto it = seen.find(target - nums[i]);
            if (it != seen.end()) return {it->second, i};
            seen[nums[i]] = i;
        }
        return {};
    }
};

#ifdef LOCAL
int main() {
    vector<int> nums = {2,7,11,15};
    auto r = Solution().twoSum(nums, 9);
    for (int x : r) cout << x << ' ';
    cout << '\n';
}
#endif
```

```bash
# 本機跑: 開 LOCAL macro 啟動 main
g++ -O2 -std=c++17 -DLOCAL -o two-sum two-sum.cpp && ./two-sum
```

送出時只貼 `class Solution { ... };`, LeetCode 自己接 harness.

### Playwright MCP 送判題

關鍵兩步 (踩過坑才知道要分開):

```js
// 第一步: 把 class Solution 塞進 Monaco 編輯器
monaco.editor.getModels()[0].setValue(code);

// 第二步 (獨立一次 evaluate, 不能合併): 點 Submit
document.querySelector('button[data-e2e-locator="console-submit-button"]').click();
```

**為什麼要分開**: setValue 跟 click 寫在同一個 `evaluate()` 裡, 偶爾會送出前一版 (預設 stub) 的程式碼, 判題回 Compile Error. 原因大概是 Monaco 的 React state 跟 DOM 更新沒同步 — 分成兩個 evaluate 讓 React 跑完一輪 reconciliation 就穩了.

讀 verdict 用 `document.querySelector('[data-e2e-locator="submission-result"]')?.innerText`, 回來的字串是 `"Accepted"` 或 `"Wrong Answer"` 之類.

---

## 結果 — The Numbers

### Blind 75

**69 / 75 AC**(可送判題的全過), 其餘 6 題被 PREMIUM 鎖住無法送:

| 鎖住的題 | 類型 |
|:---|:---|
| 252 / 253 Meeting Rooms I & II | 區間排程 |
| 261 Graph Valid Tree | 圖/UF |
| 269 Alien Dictionary | 拓撲排序 |
| 271 Encode / Decode Strings | 字串設計 |
| 323 Number of Connected Components | UF |

這 6 題 Claude 都寫出 `class Solution` + 本機 harness 跑過樣例, **只是沒 LeetCode 帳號能送**. 等於**全 75 題寫完, 69 題拿到 AC 標籤**.

### LC-75 (官方 study plan)

[LeetCode 官方 study plan](https://leetcode.com/studyplan/leetcode-75/) 也跟著掃一遍. 它跟 Blind 75 重疊 10 題 (Container With Most Water / Product Except Self / Reverse Linked List / Max Depth / House Robber / Unique Paths / LCS / Counting Bits / Trie / Non-overlapping Intervals), 其餘 65 題新題全送上去 **65/65 AC**. 加上重疊的 10, **75 / 75 全 AC**.

兩套清單加起來: **去重後 140 題, 送判題成功 134, PREMIUM 鎖住 6, pass rate ≈ 95.7%** — 鎖住的那 6 題不是 AI 解不出, 是沒帳號.

### 幾題 Hard 的手感

Blind 75 有 10 題 Hard, 挑三題講:

**#297 Serialize and Deserialize Binary Tree** — 需要設計自己的序列化格式. Claude 選 BFS + `#` 代表 null, 用逗號分隔. 分析完直接寫, 一次 AC. 這題在 StackOverflow / LeetCode 題解 repo 的解法極為一致, 幾乎變公版.

**#295 Find Median from Data Stream** — 兩個 heap (max-heap 存左半、min-heap 存右半), 保持大小差 ≤ 1. 教科書結構, 一次 AC. 這題在 C++ STL `priority_queue` 用法上有個小坑: 預設是 max-heap, min-heap 要寫 `priority_queue<int, vector<int>, greater<int>>`, 這段 agent 寫對了.

**#212 Word Search II** — Trie + DFS backtracking. 單字一多暴力搜很慢, 要先建 Trie 讓多個單字共享前綴. 這題我看過不少工程師第一次寫會卡在**從 Trie 節點回收已匹配單字避免重複輸出**那段. Claude 直接寫對了 — 解完的 word 從 Trie 標成 null, 不是加 set 去重. 這個做法在熱門題解裡很常見, 應該是從訓練資料裡複製下來的手感.

Hard 題幾乎沒有卡住的, pattern density 高到讓刷題變批量作業.

---

## AI 幫到哪 — Where It Carries

**一、經典題 pattern 直出.** Two Sum / Valid Parentheses / Merge Two Sorted Lists 這類題, agent 從讀題到提交 AC 大約**半分鐘**. 連想都不太想 — 題幹讀完 pattern 就認出來了.

**二、Hard 不卡.** 10 Hard 全部一次 AC, 沒有一題需要我介入. 這跟 [Day 23 的 ★★★★★ UVa 10330 max flow 一次 AC](./day23-zerojudge-cross-tier.md#uva-10330-power-transmission--節點容量-max-flow) 是同一件事: **AI 在 OJ 上強, 不是因為它會推理, 是因為題目剛好全在它的 pattern 範圍裡**.

**三、批次吃掉 30 題不喘.** 整個 Blind 75, 從開始到 commit 到 repo, 大概兩個 session (約 3-4 小時). 一題平均 5-7 分鐘含送判題延遲. 比起我自己手刷快一個量級 — 不是因為我思考比較慢, 是「切頁面 → 讀題 → 寫 → 編 → 送 → 讀 verdict → 存檔 → 下一題」這個流程 agent 一次吃完, 中間沒 context switch 的 overhead.

**四、進度看板順便維護.** [leetcode/README.md](../../../leetcode/README.md) 的進度表格 (題號 / 題名 / 難度 / AC 狀態) agent 每做完一批就自己更新, 不需要我回頭 copy-paste.

---

## 哪裡得自己來 — Where I Still Carry

**一、PREMIUM 鎖住就要寫本地判斷標準.** Blind 75 的 6 題 PREMIUM 題 (Meeting Rooms I/II, Graph Valid Tree, Alien Dictionary, Encode/Decode Strings, Connected Components) **沒有外部驗證**. agent 寫出 `class Solution` 之後, 本機 harness 要**我幫忙想幾組邊界測資** — 這其實就是 [Day 21 「有判斷標準」](./day21-back-to-verifiable-ground.md) 的反面: 一旦沒有外部判斷標準, agent 自測全綠也不保證對, 我得補上「這個測資 agent 自己可能沒想到」的那幾個邊界. 這 6 題沒上判題所以嚴格說**不算 AC**, 只能算「寫出一個看起來對的解」.

**二、Monaco setValue 跟 submit race.** 第一輪刷題有連續三題遇到 Compile Error, 看了半天才發現 agent 把 `setValue + click` 合在同一個 `evaluate()` 送 — 以為 Monaco 值已經更新, 其實 React 還沒 reconcile 完就 submit 了, 送出的是前一版 stub. 分開成兩個 evaluate 之後完全消失. 這種**跟平台 DOM 時序磨合**的坑, agent 本身不會意識到 — 除非它看過一樣的 bug report. 我介入告訴它要分開, 之後它才改成兩步流程.

**三、平台細節.** cookie session 到期要重新登入 (手動). Playwright MCP server 偶爾斷線, 要用 ToolSearch 重新 load tool schema. LeetCode 偶爾彈 Cloudflare 的 "Just a moment..." 風控頁, 要等幾秒. 這些都是**人在管基礎建設, agent 在做解題**的分工.

**四、進度判斷.** 「這題跳過還是重試」「這批先 commit 還是繼續」這類**排程判斷**, agent 不會主動做. 我會每 15 題左右喊一次 "batch commit", 不然它會一路刷到 context 塞爆才停.

---

## 我的收斂 — Takeaways

- **Blind 75 不是測難度, 是測 pattern density 的上限**: 75 題全寫完 (69 AC + 6 PREMIUM 本地解), 加 LC-75 的 65 新題全 AC, 說到底就是「訓練資料裡出現幾千次的題目, LLM 幾乎零摩擦吃掉」. 這個結果**不代表** AI 真能推理, 只代表這批題剛好在它舒適圈裡
- **Hard 不等於難**: Blind 75 的 10 Hard 全一次過, 因為它們全是教科書 pattern (BFS序列化 / 雙 heap / Trie 回收). 真正「難」的題不在 OJ 上, 在 Day 23 的 c500 那種「公開樣例剛好蓋住判題模型盲區」的情境裡
- **判斷標準有無差很多**: 69 題有 LeetCode 判題, 全掛看得見; 6 題 PREMIUM 鎖住只能本地測, 本機綠不代表對 — [Day 21 的「綠燈不等於對」](./day21-back-to-verifiable-ground.md) 在這裡又驗證一次
- **人在管基建, agent 在解題**: 登入 / session / 平台風控 / submit race 這些**跟題目無關的環境層**還是我的事. 這條分工在接下來的 Day 25 CTF 實戰會更明顯 — CTF 比 LeetCode 又多了「要主動找 attack surface」這一層
- **ZeroJudge 到 LeetCode 的對照**: 中文 OJ / 英文 OJ, 兩邊 pass rate 都在 95% 以上. 這是**判斷標準明確 + 題型經典**兩個條件疊加的結果, 下一篇 (Day 25) 進 CTF, 判斷標準變成「拿到 flag」, 題型從沒看過, 看它還撐得住多少

---

## Sources

- [Blind 75 清單 (LeetCode)](https://leetcode.com/problem-list/oizxjoit/) — 本篇刷的 75 題
- [LeetCode-75 官方 study plan](https://leetcode.com/studyplan/leetcode-75/) — 收尾順便掃完的 75 題
- [leetcode/ 全部解答](../../../leetcode/README.md) — 本篇所有 cpp 原始碼與進度表
- [Day 21: 回到好驗證的主場](https://ithelp.ithome.com.tw/articles/10421407) — 判斷標準概念鋪陳
- [Day 22: 掃 ZeroJudge 78 題](https://ithelp.ithome.com.tw/articles/10421768) — 規模化 pass rate (中文 OJ)
- [Day 23: ZeroJudge 進階測試](./day23-zerojudge-cross-tier.md) — 主題覆蓋 + 難度階梯 + 多輪救援
- [Playwright MCP](https://github.com/microsoft/playwright-mcp) — 自動送判題的工具
