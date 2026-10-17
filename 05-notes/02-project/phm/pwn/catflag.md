# PHM #58 Pwn — catflag

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 10 |

## 題目

> `nc ctf.hackme.quest 7709`
>
> Try using `nc` connect to server!
>
> 檔案: [`/static/bash`](https://ctf.hackme.quest/static/bash) (GNU bash 4.3.42)

## 分析

連進去會先 countdown 5 秒:

```
plz capture the flag after 5 seconds...
plz capture the flag after 4 seconds...
...
```

倒數結束後就打開一個 **restricted shell**: 丟任何指令都回 `Invalid command`, 只有特定命令會過. 題目名字 `catflag` 直接提示 → 送 `cat flag`.

試過 `/bin/cat flag` 會被擋成 `Invalid command`, 所以 whitelist 應該是**字串完全比對** `cat flag` (跟 `ls` 也 pass).

## 解法

```bash
# sleep 7 給倒數跑完, 再送 cat flag
(sleep 7; echo "cat flag") | nc ctf.hackme.quest 7709
```

輸出:

```
plz capture the flag after 5 seconds...
...
plz capture the flag after 1 seconds...
FLAG{cat flag? dog flag!}
```

## Flag

```
FLAG{cat flag? dog flag!}
```

## 感想

Pwn 類別 10 分熱身題, 就是**字面上照題目做**: 要你 cat flag, 你就 cat flag. 沒有真的 pwn (buffer overflow / ROP), 只是看懂題目的 whitelist 機制.
