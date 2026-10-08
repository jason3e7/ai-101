# LeetCode 解題 — Index

[← 回主頁](../../index.md)

[LeetCode](https://leetcode.com/) 的解題紀錄。每題一個 `.cpp` 檔, 內含 `class Solution` + 本機 harness (包在 `#ifdef LOCAL` 下)。本機測過範例再透過 Playwright MCP 自動把 `class Solution` 塞進網頁編輯器送判。

## Blind 75 進度

來自 LeetCode 的 [Blind 75 list](https://leetcode.com/problem-list/oizxjoit/), 持續補題中.

| 題號 | 題名 | 難度 | 結果 |
|---:|:---|:---|:---|
| [1](https://leetcode.com/problems/two-sum/) | Two Sum | Easy | AC |
| [3](https://leetcode.com/problems/longest-substring-without-repeating-characters/) | Longest Substring Without Repeating Characters | Med | AC |
| [5](https://leetcode.com/problems/longest-palindromic-substring/) | Longest Palindromic Substring | Med | AC |
| [11](https://leetcode.com/problems/container-with-most-water/) | Container With Most Water | Med | AC |
| [15](https://leetcode.com/problems/3sum/) | 3Sum | Med | AC |
| [19](https://leetcode.com/problems/remove-nth-node-from-end-of-list/) | Remove Nth Node From End of List | Med | AC |
| [20](https://leetcode.com/problems/valid-parentheses/) | Valid Parentheses | Easy | AC |
| [21](https://leetcode.com/problems/merge-two-sorted-lists/) | Merge Two Sorted Lists | Easy | AC |
| [23](https://leetcode.com/problems/merge-k-sorted-lists/) | Merge k Sorted Lists | Hard | AC |
| [128](https://leetcode.com/problems/longest-consecutive-sequence/) | Longest Consecutive Sequence | Med | AC |
| [133](https://leetcode.com/problems/clone-graph/) | Clone Graph | Med | AC |
| [139](https://leetcode.com/problems/word-break/) | Word Break | Med | AC |
| [141](https://leetcode.com/problems/linked-list-cycle/) | Linked List Cycle | Easy | AC |
| [143](https://leetcode.com/problems/reorder-list/) | Reorder List | Med | AC |
| [268](https://leetcode.com/problems/missing-number/) | Missing Number | Easy | AC |
| [647](https://leetcode.com/problems/palindromic-substrings/) | Palindromic Substrings | Med | AC |

**進度**: 16 / 75

編譯與本機測試:

```bash
# 開 LOCAL macro 啟動 main() 跑範例
g++ -O2 -std=c++17 -DLOCAL -o two-sum two-sum.cpp && ./two-sum
```

送判題:

- 把檔案裡 `class Solution { ... };` 那段複製到 LeetCode 編輯器 (或讓 Playwright MCP 透過 `monaco.editor.getModels()[0].setValue(code)` 塞進去)
- 按 Submit, 等 verdict
