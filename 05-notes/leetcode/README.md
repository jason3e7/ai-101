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

## LeetCode-75 (官方 study plan) 進度

LC-75 跟 Blind 75 有 **10 題重疊** (Container, Product, Reverse Linked List, Max Depth, House Robber, Unique Paths, LCS, Counting Bits, Trie, Non-overlapping Intervals), 其餘 65 題在補.

### LC-75 新增 AC

| 題號 | 題名 | 難度 | 結果 |
|---:|:---|:---|:---|
| [151](https://leetcode.com/problems/reverse-words-in-a-string/) | Reverse Words in a String | Med | AC |
| [283](https://leetcode.com/problems/move-zeroes/) | Move Zeroes | Easy | AC |
| [334](https://leetcode.com/problems/increasing-triplet-subsequence/) | Increasing Triplet Subsequence | Med | AC |
| [345](https://leetcode.com/problems/reverse-vowels-of-a-string/) | Reverse Vowels of a String | Easy | AC |
| [392](https://leetcode.com/problems/is-subsequence/) | Is Subsequence | Easy | AC |
| [443](https://leetcode.com/problems/string-compression/) | String Compression | Med | AC |
| [605](https://leetcode.com/problems/can-place-flowers/) | Can Place Flowers | Easy | AC |
| [643](https://leetcode.com/problems/maximum-average-subarray-i/) | Maximum Average Subarray I | Easy | AC |
| [1071](https://leetcode.com/problems/greatest-common-divisor-of-strings/) | Greatest Common Divisor of Strings | Easy | AC |
| [1431](https://leetcode.com/problems/kids-with-the-greatest-number-of-candies/) | Kids With the Greatest Number of Candies | Easy | AC |
| [1456](https://leetcode.com/problems/maximum-number-of-vowels-in-a-substring-of-given-length/) | Maximum Number of Vowels in a Substring | Med | AC |
| [1679](https://leetcode.com/problems/max-number-of-k-sum-pairs/) | Max Number of K-Sum Pairs | Med | AC |
| [1768](https://leetcode.com/problems/merge-strings-alternately/) | Merge Strings Alternately | Easy | AC |
| [328](https://leetcode.com/problems/odd-even-linked-list/) | Odd Even Linked List | Med | AC |
| [394](https://leetcode.com/problems/decode-string/) | Decode String | Med | AC |
| [649](https://leetcode.com/problems/dota2-senate/) | Dota2 Senate | Med | AC |
| [724](https://leetcode.com/problems/find-pivot-index/) | Find Pivot Index | Easy | AC |
| [735](https://leetcode.com/problems/asteroid-collision/) | Asteroid Collision | Med | AC |
| [933](https://leetcode.com/problems/number-of-recent-calls/) | Number of Recent Calls | Easy | AC |
| [1004](https://leetcode.com/problems/max-consecutive-ones-iii/) | Max Consecutive Ones III | Med | AC |
| [1207](https://leetcode.com/problems/unique-number-of-occurrences/) | Unique Number of Occurrences | Easy | AC |
| [1493](https://leetcode.com/problems/longest-subarray-of-1s-after-deleting-one-element/) | Longest Subarray of 1s After Deleting One | Med | AC |
| [1657](https://leetcode.com/problems/determine-if-two-strings-are-close/) | Determine if Two Strings Are Close | Med | AC |
| [1732](https://leetcode.com/problems/find-the-highest-altitude/) | Find the Highest Altitude | Easy | AC |
| [2095](https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/) | Delete the Middle Node of a Linked List | Med | AC |
| [2130](https://leetcode.com/problems/maximum-twin-sum-of-a-linked-list/) | Maximum Twin Sum of a Linked List | Med | AC |
| [2215](https://leetcode.com/problems/find-the-difference-of-two-arrays/) | Find the Difference of Two Arrays | Easy | AC |
| [2352](https://leetcode.com/problems/equal-row-and-column-pairs/) | Equal Row and Column Pairs | Med | AC |
| [2390](https://leetcode.com/problems/removing-stars-from-a-string/) | Removing Stars From a String | Med | AC |
| [199](https://leetcode.com/problems/binary-tree-right-side-view/) | Binary Tree Right Side View | Med | AC |
| [215](https://leetcode.com/problems/kth-largest-element-in-an-array/) | Kth Largest Element in an Array | Med | AC |
| [236](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-tree/) | LCA of Binary Tree | Med | AC |
| [399](https://leetcode.com/problems/evaluate-division/) | Evaluate Division | Med | AC |
| [437](https://leetcode.com/problems/path-sum-iii/) | Path Sum III | Med | AC |
| [450](https://leetcode.com/problems/delete-node-in-a-bst/) | Delete Node in a BST | Med | AC |
| [547](https://leetcode.com/problems/number-of-provinces/) | Number of Provinces | Med | AC |
| [700](https://leetcode.com/problems/search-in-a-binary-search-tree/) | Search in a Binary Search Tree | Easy | AC |
| [841](https://leetcode.com/problems/keys-and-rooms/) | Keys and Rooms | Med | AC |
| [872](https://leetcode.com/problems/leaf-similar-trees/) | Leaf-Similar Trees | Easy | AC |
| [994](https://leetcode.com/problems/rotting-oranges/) | Rotting Oranges | Med | AC |
| [1161](https://leetcode.com/problems/maximum-level-sum-of-a-binary-tree/) | Maximum Level Sum of a Binary Tree | Med | AC |
| [1372](https://leetcode.com/problems/longest-zigzag-path-in-a-binary-tree/) | Longest ZigZag Path in a Binary Tree | Med | AC |
| [1448](https://leetcode.com/problems/count-good-nodes-in-binary-tree/) | Count Good Nodes in Binary Tree | Med | AC |
| [1466](https://leetcode.com/problems/reorder-routes-to-make-all-paths-lead-to-the-city-zero/) | Reorder Routes | Med | AC |
| [1926](https://leetcode.com/problems/nearest-exit-from-entrance-in-maze/) | Nearest Exit from Entrance in Maze | Med | AC |
| [17](https://leetcode.com/problems/letter-combinations-of-a-phone-number/) | Letter Combinations of a Phone Number | Med | AC |
| [72](https://leetcode.com/problems/edit-distance/) | Edit Distance | Med | AC |
| [136](https://leetcode.com/problems/single-number/) | Single Number | Easy | AC |
| [162](https://leetcode.com/problems/find-peak-element/) | Find Peak Element | Med | AC |
| [216](https://leetcode.com/problems/combination-sum-iii/) | Combination Sum III | Med | AC |
| [374](https://leetcode.com/problems/guess-number-higher-or-lower/) | Guess Number Higher or Lower | Easy | AC |
| [452](https://leetcode.com/problems/minimum-number-of-arrows-to-burst-balloons/) | Minimum Number of Arrows to Burst Balloons | Med | AC |
| [714](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/) | Best Time to Buy and Sell Stock with Fee | Med | AC |
| [739](https://leetcode.com/problems/daily-temperatures/) | Daily Temperatures | Med | AC |
| [746](https://leetcode.com/problems/min-cost-climbing-stairs/) | Min Cost Climbing Stairs | Easy | AC |
| [790](https://leetcode.com/problems/domino-and-tromino-tiling/) | Domino and Tromino Tiling | Med | AC |
| [875](https://leetcode.com/problems/koko-eating-bananas/) | Koko Eating Bananas | Med | AC |
| [901](https://leetcode.com/problems/online-stock-span/) | Online Stock Span | Med | AC |
| [1137](https://leetcode.com/problems/n-th-tribonacci-number/) | N-th Tribonacci Number | Easy | AC |
| [1268](https://leetcode.com/problems/search-suggestions-system/) | Search Suggestions System | Med | AC |
| [1318](https://leetcode.com/problems/minimum-flips-to-make-a-or-b-equal-to-c/) | Minimum Flips to Make a OR b Equal to c | Med | AC |
| [2300](https://leetcode.com/problems/successful-pairs-of-spells-and-potions/) | Successful Pairs of Spells and Potions | Med | AC |
| [2336](https://leetcode.com/problems/smallest-number-in-infinite-set/) | Smallest Number in Infinite Set | Med | AC |
| [2462](https://leetcode.com/problems/total-cost-to-hire-k-workers/) | Total Cost to Hire K Workers | Med | AC |
| [2542](https://leetcode.com/problems/maximum-subsequence-score/) | Maximum Subsequence Score | Med | AC |

**LC-75 進度**: 10 (重疊) + 65 (新 AC) = **75 / 75** 全完成

## NeetCode 150 進度

[NeetCode 150](https://leetcode.com/problem-list/plakya4j/) 跟前兩套合計重疊 79 題, 這一輪新增 **64 AC + 7 PREMIUM 本地解 = 71 題寫完**.

### NC150 新增 AC

| 題號 | 題名 | 難度 | 結果 |
|---:|:---|:---|:---|
| [2](https://leetcode.com/problems/add-two-numbers/) | Add Two Numbers | Med | AC |
| [4](https://leetcode.com/problems/median-of-two-sorted-arrays/) | Median of Two Sorted Arrays | Hard | AC |
| [7](https://leetcode.com/problems/reverse-integer/) | Reverse Integer | Med | AC |
| [10](https://leetcode.com/problems/regular-expression-matching/) | Regular Expression Matching | Hard | AC |
| [22](https://leetcode.com/problems/generate-parentheses/) | Generate Parentheses | Med | AC |
| [25](https://leetcode.com/problems/reverse-nodes-in-k-group/) | Reverse Nodes in k-Group | Hard | AC |
| [36](https://leetcode.com/problems/valid-sudoku/) | Valid Sudoku | Med | AC |
| [40](https://leetcode.com/problems/combination-sum-ii/) | Combination Sum II | Med | AC |
| [42](https://leetcode.com/problems/trapping-rain-water/) | Trapping Rain Water | Hard | AC |
| [43](https://leetcode.com/problems/multiply-strings/) | Multiply Strings | Med | AC |
| [45](https://leetcode.com/problems/jump-game-ii/) | Jump Game II | Med | AC |
| [46](https://leetcode.com/problems/permutations/) | Permutations | Med | AC |
| [50](https://leetcode.com/problems/powx-n/) | Pow(x, n) | Med | AC |
| [51](https://leetcode.com/problems/n-queens/) | N-Queens | Hard | AC |
| [66](https://leetcode.com/problems/plus-one/) | Plus One | Easy | AC |
| [72](https://leetcode.com/problems/edit-distance/) | Edit Distance (已於 LC-75 AC) | Med | AC |
| [74](https://leetcode.com/problems/search-a-2d-matrix/) | Search a 2D Matrix | Med | AC |
| [78](https://leetcode.com/problems/subsets/) | Subsets | Med | AC |
| [84](https://leetcode.com/problems/largest-rectangle-in-histogram/) | Largest Rectangle in Histogram | Hard | AC |
| [90](https://leetcode.com/problems/subsets-ii/) | Subsets II | Med | AC |
| [97](https://leetcode.com/problems/interleaving-string/) | Interleaving String | Med | AC |
| [110](https://leetcode.com/problems/balanced-binary-tree/) | Balanced Binary Tree | Easy | AC |
| [115](https://leetcode.com/problems/distinct-subsequences/) | Distinct Subsequences | Hard | AC |
| [127](https://leetcode.com/problems/word-ladder/) | Word Ladder | Hard | AC |
| [130](https://leetcode.com/problems/surrounded-regions/) | Surrounded Regions | Med | AC |
| [131](https://leetcode.com/problems/palindrome-partitioning/) | Palindrome Partitioning | Med | AC |
| [134](https://leetcode.com/problems/gas-station/) | Gas Station | Med | AC |
| [138](https://leetcode.com/problems/copy-list-with-random-pointer/) | Copy List with Random Pointer | Med | AC |
| [146](https://leetcode.com/problems/lru-cache/) | LRU Cache | Med | AC |
| [150](https://leetcode.com/problems/evaluate-reverse-polish-notation/) | Evaluate Reverse Polish Notation | Med | AC |
| [155](https://leetcode.com/problems/min-stack/) | Min Stack | Med | AC |
| [167](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/) | Two Sum II | Med | AC |
| [202](https://leetcode.com/problems/happy-number/) | Happy Number | Easy | AC |
| [210](https://leetcode.com/problems/course-schedule-ii/) | Course Schedule II | Med | AC |
| [239](https://leetcode.com/problems/sliding-window-maximum/) | Sliding Window Maximum | Hard | AC |
| 286 | Walls and Gates (PREMIUM) | Med | 本地解未送判 |
| [287](https://leetcode.com/problems/find-the-duplicate-number/) | Find the Duplicate Number | Med | AC |
| [309](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/) | Best Time to Buy and Sell Stock with Cooldown | Med | AC |
| [312](https://leetcode.com/problems/burst-balloons/) | Burst Balloons | Hard | AC |
| [329](https://leetcode.com/problems/longest-increasing-path-in-a-matrix/) | Longest Increasing Path in a Matrix | Hard | AC |
| [332](https://leetcode.com/problems/reconstruct-itinerary/) | Reconstruct Itinerary | Hard | AC |
| [355](https://leetcode.com/problems/design-twitter/) | Design Twitter | Med | AC |
| [416](https://leetcode.com/problems/partition-equal-subset-sum/) | Partition Equal Subset Sum | Med | AC |
| [494](https://leetcode.com/problems/target-sum/) | Target Sum | Med | AC |
| [518](https://leetcode.com/problems/coin-change-ii/) | Coin Change II | Med | AC |
| [543](https://leetcode.com/problems/diameter-of-binary-tree/) | Diameter of Binary Tree | Easy | AC |
| [567](https://leetcode.com/problems/permutation-in-string/) | Permutation in String | Med | AC |
| [621](https://leetcode.com/problems/task-scheduler/) | Task Scheduler | Med | AC |
| [678](https://leetcode.com/problems/valid-parenthesis-string/) | Valid Parenthesis String | Med | AC |
| [684](https://leetcode.com/problems/redundant-connection/) | Redundant Connection | Med | AC |
| [695](https://leetcode.com/problems/max-area-of-island/) | Max Area of Island | Med | AC |
| [703](https://leetcode.com/problems/kth-largest-element-in-a-stream/) | Kth Largest Element in a Stream | Easy | AC |
| [704](https://leetcode.com/problems/binary-search/) | Binary Search | Easy | AC |
| [743](https://leetcode.com/problems/network-delay-time/) | Network Delay Time | Med | AC |
| [763](https://leetcode.com/problems/partition-labels/) | Partition Labels | Med | AC |
| [778](https://leetcode.com/problems/swim-in-rising-water/) | Swim in Rising Water | Hard | AC |
| [787](https://leetcode.com/problems/cheapest-flights-within-k-stops/) | Cheapest Flights Within K Stops | Med | AC |
| [846](https://leetcode.com/problems/hand-of-straights/) | Hand of Straights | Med | AC |
| [853](https://leetcode.com/problems/car-fleet/) | Car Fleet | Med | AC |
| [973](https://leetcode.com/problems/k-closest-points-to-origin/) | K Closest Points to Origin | Med | AC |
| [981](https://leetcode.com/problems/time-based-key-value-store/) | Time Based Key-Value Store | Med | AC |
| [1046](https://leetcode.com/problems/last-stone-weight/) | Last Stone Weight | Easy | AC |
| [1584](https://leetcode.com/problems/min-cost-to-connect-all-points/) | Min Cost to Connect All Points | Med | AC |
| [1851](https://leetcode.com/problems/minimum-interval-to-include-each-query/) | Minimum Interval to Include Each Query | Hard | AC |
| [1899](https://leetcode.com/problems/merge-triplets-to-form-target-triplet/) | Merge Triplets to Form Target Triplet | Med | AC |
| [2013](https://leetcode.com/problems/detect-squares/) | Detect Squares | Med | AC |

**NC150 進度**: 79 (前兩套重疊) + 64 (新 AC) + 7 PREMIUM 本地解 = **150 / 150** 全完成
(PREMIUM 共 7: 252/253/261/269/271/323 已於 Blind 75 本地解, 新增 286 Walls and Gates)

編譯與本機測試:

```bash
# 開 LOCAL macro 啟動 main() 跑範例
g++ -O2 -std=c++17 -DLOCAL -o two-sum two-sum.cpp && ./two-sum
```

送判題:

- 把檔案裡 `class Solution { ... };` 那段複製到 LeetCode 編輯器 (或讓 Playwright MCP 透過 `monaco.editor.getModels()[0].setValue(code)` 塞進去)
- 按 Submit, 等 verdict
