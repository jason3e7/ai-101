# ZeroJudge 解題

[ZeroJudge](https://zerojudge.tw/) (高中生程式解題系統) 的解題紀錄。每題一個原始碼檔,先用本機 g++ 測過範例再上傳判題。

| 題號 | 題名 | 語言 | 結果 |
|:---|:---|:---|:---|
| [a001](https://zerojudge.tw/ShowProblem?problemid=a001) | 哈囉 | CPP | AC (1ms, 3.6MB) |
| [a002](https://zerojudge.tw/ShowProblem?problemid=a002) | 簡易加法 | CPP | AC (1ms, 3.5MB) |
| [a003](https://zerojudge.tw/ShowProblem?problemid=a003) | 兩光法師占卜術 | CPP | AC (2ms, 3.5MB) |
| [a004](https://zerojudge.tw/ShowProblem?problemid=a004) | 文文的求婚 (閏年) | CPP | AC (4ms, 3.5MB) |
| [a005](https://zerojudge.tw/ShowProblem?problemid=a005) | Eva 的回家作業 | CPP | AC (1ms, 3.6MB) |
| [a006](https://zerojudge.tw/ShowProblem?problemid=a006) | 一元二次方程式 | CPP | AC (1ms, 3.6MB) |
| [a009](https://zerojudge.tw/ShowProblem?problemid=a009) | 解碼器 (凱薩密碼) | CPP | AC (2ms, 3.5MB) |
| [a010](https://zerojudge.tw/ShowProblem?problemid=a010) | 因數分解 | CPP | AC (1ms, 3.6MB) |
| [a013](https://zerojudge.tw/ShowProblem?problemid=a013) | 羅馬數字 | CPP | 本機測過, 待上傳 |

編譯與測試:

```bash
g++ -O2 -std=c++17 -o a001 a001.cpp
echo "world" | ./a001   # -> hello, world
```

## 相關筆記

- [用 Playwright 自動操作 ZeroJudge (送出答案的坑)](./playwright-zerojudge-automation.md)
