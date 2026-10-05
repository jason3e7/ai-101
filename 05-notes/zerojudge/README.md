# ZeroJudge 解題

[ZeroJudge](https://zerojudge.tw/) (高中生程式解題系統) 的解題紀錄。每題一個原始碼檔,先用本機 g++ 測過範例再上傳判題。

| 題號 | 題名 | 語言 | 結果 |
|:---|:---|:---|:---|
| [a001](https://zerojudge.tw/ShowProblem?problemid=a001) | 哈囉 | CPP | AC (1ms, 3.6MB) |
| [a002](https://zerojudge.tw/ShowProblem?problemid=a002) | 簡易加法 | CPP | AC (1ms, 3.5MB) |

編譯與測試:

```bash
g++ -O2 -std=c++17 -o a001 a001.cpp
echo "world" | ./a001   # -> hello, world
```

## 相關筆記

- [用 Playwright 自動操作 ZeroJudge (送出答案的坑)](./playwright-zerojudge-automation.md)
