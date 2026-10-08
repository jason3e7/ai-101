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
| [33](https://leetcode.com/problems/search-in-rotated-sorted-array/) | Search in Rotated Sorted Array | Med | AC |
| [39](https://leetcode.com/problems/combination-sum/) | Combination Sum | Med | AC |
| [48](https://leetcode.com/problems/rotate-image/) | Rotate Image | Med | AC |
| [49](https://leetcode.com/problems/group-anagrams/) | Group Anagrams | Med | AC |
| [53](https://leetcode.com/problems/maximum-subarray/) | Maximum Subarray | Med | AC |
| [54](https://leetcode.com/problems/spiral-matrix/) | Spiral Matrix | Med | AC |
| [55](https://leetcode.com/problems/jump-game/) | Jump Game | Med | AC |
| [56](https://leetcode.com/problems/merge-intervals/) | Merge Intervals | Med | AC |
| [57](https://leetcode.com/problems/insert-interval/) | Insert Interval | Med | AC |
| [62](https://leetcode.com/problems/unique-paths/) | Unique Paths | Med | AC |
| [70](https://leetcode.com/problems/climbing-stairs/) | Climbing Stairs | Easy | AC |
| [73](https://leetcode.com/problems/set-matrix-zeroes/) | Set Matrix Zeroes | Med | AC |
| [76](https://leetcode.com/problems/minimum-window-substring/) | Minimum Window Substring | Hard | AC |
| [79](https://leetcode.com/problems/word-search/) | Word Search | Med | AC |
| [91](https://leetcode.com/problems/decode-ways/) | Decode Ways | Med | AC |
| [98](https://leetcode.com/problems/validate-binary-search-tree/) | Validate Binary Search Tree | Med | AC |
| [100](https://leetcode.com/problems/same-tree/) | Same Tree | Easy | AC |
| [102](https://leetcode.com/problems/binary-tree-level-order-traversal/) | Binary Tree Level Order Traversal | Med | AC |
| [104](https://leetcode.com/problems/maximum-depth-of-binary-tree/) | Maximum Depth of Binary Tree | Easy | AC |
| [105](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/) | Construct Binary Tree from Preorder and Inorder | Med | AC |
| [121](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/) | Best Time to Buy and Sell Stock | Easy | AC |
| [124](https://leetcode.com/problems/binary-tree-maximum-path-sum/) | Binary Tree Maximum Path Sum | Hard | AC |
| [125](https://leetcode.com/problems/valid-palindrome/) | Valid Palindrome | Easy | AC |
| [128](https://leetcode.com/problems/longest-consecutive-sequence/) | Longest Consecutive Sequence | Med | AC |
| [133](https://leetcode.com/problems/clone-graph/) | Clone Graph | Med | AC |
| [139](https://leetcode.com/problems/word-break/) | Word Break | Med | AC |
| [141](https://leetcode.com/problems/linked-list-cycle/) | Linked List Cycle | Easy | AC |
| [143](https://leetcode.com/problems/reorder-list/) | Reorder List | Med | AC |
| [152](https://leetcode.com/problems/maximum-product-subarray/) | Maximum Product Subarray | Med | AC |
| [153](https://leetcode.com/problems/find-minimum-in-rotated-sorted-array/) | Find Minimum in Rotated Sorted Array | Med | AC |
| [190](https://leetcode.com/problems/reverse-bits/) | Reverse Bits | Easy | AC |
| [191](https://leetcode.com/problems/number-of-1-bits/) | Number of 1 Bits | Easy | AC |
| [198](https://leetcode.com/problems/house-robber/) | House Robber | Med | AC |
| [200](https://leetcode.com/problems/number-of-islands/) | Number of Islands | Med | AC |
| [206](https://leetcode.com/problems/reverse-linked-list/) | Reverse Linked List | Easy | AC |
| [207](https://leetcode.com/problems/course-schedule/) | Course Schedule | Med | AC |
| [208](https://leetcode.com/problems/implement-trie-prefix-tree/) | Implement Trie (Prefix Tree) | Med | AC |
| [211](https://leetcode.com/problems/design-add-and-search-words-data-structure/) | Design Add and Search Words Data Structure | Med | AC |
| [212](https://leetcode.com/problems/word-search-ii/) | Word Search II | Hard | AC |
| [213](https://leetcode.com/problems/house-robber-ii/) | House Robber II | Med | AC |
| [217](https://leetcode.com/problems/contains-duplicate/) | Contains Duplicate | Easy | AC |
| [226](https://leetcode.com/problems/invert-binary-tree/) | Invert Binary Tree | Easy | AC |
| [230](https://leetcode.com/problems/kth-smallest-element-in-a-bst/) | Kth Smallest Element in a BST | Med | AC |
| [235](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/) | LCA of a Binary Search Tree | Med | AC |
| [238](https://leetcode.com/problems/product-of-array-except-self/) | Product of Array Except Self | Med | AC |
| [242](https://leetcode.com/problems/valid-anagram/) | Valid Anagram | Easy | AC |
| 252 | Meeting Rooms (PREMIUM) | Easy | 本地解未送判 |
| 253 | Meeting Rooms II (PREMIUM) | Med | 本地解未送判 |
| 261 | Graph Valid Tree (PREMIUM) | Med | 本地解未送判 |
| [268](https://leetcode.com/problems/missing-number/) | Missing Number | Easy | AC |
| 269 | Alien Dictionary (PREMIUM) | Hard | 本地解未送判 |
| 271 | Encode and Decode Strings (PREMIUM) | Med | 本地解未送判 |
| [295](https://leetcode.com/problems/find-median-from-data-stream/) | Find Median from Data Stream | Hard | AC |
| [297](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) | Serialize and Deserialize Binary Tree | Hard | AC |
| [300](https://leetcode.com/problems/longest-increasing-subsequence/) | Longest Increasing Subsequence | Med | AC |
| 323 | Number of Connected Components (PREMIUM) | Med | 本地解未送判 |
| [322](https://leetcode.com/problems/coin-change/) | Coin Change | Med | AC |
| [338](https://leetcode.com/problems/counting-bits/) | Counting Bits | Easy | AC |
| [347](https://leetcode.com/problems/top-k-frequent-elements/) | Top K Frequent Elements | Med | AC |
| [371](https://leetcode.com/problems/sum-of-two-integers/) | Sum of Two Integers | Med | AC |
| [417](https://leetcode.com/problems/pacific-atlantic-water-flow/) | Pacific Atlantic Water Flow | Med | AC |
| [424](https://leetcode.com/problems/longest-repeating-character-replacement/) | Longest Repeating Character Replacement | Med | AC |
| [435](https://leetcode.com/problems/non-overlapping-intervals/) | Non-overlapping Intervals | Med | AC |
| [572](https://leetcode.com/problems/subtree-of-another-tree/) | Subtree of Another Tree | Easy | AC |
| [647](https://leetcode.com/problems/palindromic-substrings/) | Palindromic Substrings | Med | AC |
| [1143](https://leetcode.com/problems/longest-common-subsequence/) | Longest Common Subsequence | Med | AC |

**進度**: 69 / 75 AC + 6 題 PREMIUM 鎖住無法送判 (但 .cpp 寫在本地) = **75/75 寫完**

編譯與本機測試:

```bash
# 開 LOCAL macro 啟動 main() 跑範例
g++ -O2 -std=c++17 -DLOCAL -o two-sum two-sum.cpp && ./two-sum
```

送判題:

- 把檔案裡 `class Solution { ... };` 那段複製到 LeetCode 編輯器 (或讓 Playwright MCP 透過 `monaco.editor.getModels()[0].setValue(code)` 塞進去)
- 按 Submit, 等 verdict
