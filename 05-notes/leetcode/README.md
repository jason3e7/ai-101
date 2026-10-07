# LeetCode 解題 — Index

[← 回主頁](../../index.md)

[LeetCode](https://leetcode.com/) 的解題紀錄。每題一個 `.cpp` 檔, 內含 `class Solution` + 本機 harness (包在 `#ifdef LOCAL` 下)。本機測過範例再透過 Playwright MCP 自動把 `class Solution` 塞進網頁編輯器送判。

| 題號 | 題名 | 難度 | 語言 | 結果 |
|---:|:---|:---|:---|:---|
| [1](https://leetcode.com/problems/two-sum/) | Two Sum | Easy | CPP | AC (3 ms / 14.96 MB) |

編譯與本機測試:

```bash
# 開 LOCAL macro 啟動 main() 跑範例
g++ -O2 -std=c++17 -DLOCAL -o two-sum two-sum.cpp && ./two-sum
```

送判題:

- 把檔案裡 `class Solution { ... };` 那段複製到 LeetCode 編輯器 (或讓 Playwright MCP 透過 `monaco.editor.getModels()[0].setValue(code)` 塞進去)
- 按 Submit, 等 verdict
