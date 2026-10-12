# PHM #37 Web — command-executor (未解)

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 |
| 狀態 | 卡住 (已多輪嘗試) |

## 題目

> Here's my useless developer assistant website, try to execute your own command!
>
> 連結: [`https://command-executor.hackme.quest/`](https://command-executor.hackme.quest/)

## 現狀分析

網站有四個功能:

| func | 描述 | 限制 |
|:---|:---|:---|
| `man` | 顯示 man page | 檔名只允許 `[a-zA-Z0-9_-]`, 擋掉 `.`, `/`, 空白 |
| `untar` | 列出 tar 內容 | 跑 `tar -tvf`, 僅 list 不 extract; 上傳檔案 request 後就被清掉 |
| `cmd` | 執行命令 | 嚴格白名單: 只有 `ls` 和 `env` 可以跑, 不接受任何參數 |
| `ls` | 列目錄 | file 參數經 `escapeshellarg` 包起 (`ls '<file>'`), 可注入 `-la`/`-R`/`--help` 等單一 flag |

### 關鍵禁字過濾 (這次新發現)

所有 request param 任何值裡含 `/flag` 子字串 → server 回 `\/flag detected! exit now.` 直接 exit.
Case sensitive — `/FLAG` 不會觸發, 但 Linux FS 分大小寫, `/FLAG-reader` 不存在.
URL encode 無效 (`%2fflag` 解碼後還是 `/flag`).

### 已知 flag 位置

```
ls / →
-r--------   1 flag root   37 Jan  9  2018 flag          ← mode 400, 只有 flag user 可讀
-rwsr-xr-x   1 flag root 9080 Jan 19  2018 flag-reader   ← setuid flag, 跑它拿 flag
-rw-r--r--   1 root root  653 Jan  9  2018 flag-reader.c
```

要跑 setuid `/flag-reader`. **cmd 的過濾擋死 `/flag` 字串, 繞不過**.

## 嘗試過的失敗路線

1. **直接 cmd exec**: `cmd=/flag-reader` → 被 `/flag` 過濾擋; `cmd=flag-reader` → 不在 whitelist, "command not found"
2. **URL encode 繞 `/flag`**: `%2fflag`, `/flag%00-reader`, `/flag%0a-reader` — 都被擋
3. **大小寫繞**: `/FLAG-reader` → 過濾過了但 FS 找不到檔
4. **絕對路徑 whitelist**: `/bin/ls`, `/bin/sh`, `/bin/bash` 全 "command not found" — whitelist 真的只有 `ls`/`env`
5. **ls 加參數**: `file=-la /` 被 escapeshellarg 包成 `ls '-la /'`, ls 當單一 filename, 失敗
6. **ls `+` 當空白**: `file=-la+/etc/passwd` → `ls -la /etc/passwd` 顯示在 h2, 但 output 空 (escapeshellarg 把整段當一個檔名)
7. **ls `file[]=` 陣列**: PHP toString 給 "Array", ls 當 filename
8. **ls 流 wrapper**: `php://filter/convert.base64-encode/resource=flag` — ls 不吃 wrapper
9. **cmd $IFS 繞空白**: `cmd=ls$IFS-la` 整段當 command name
10. **man `-l /path` 讀檔**: 字符過濾擋 `/`
11. **man `-C` 載 config**: 字符過濾擋 `/`
12. **untar filename injection**: 檔名透過 basename + escapeshellarg, 沒 injection; tar 實際讀 tmp_name 不吃我們的 filename
13. **tar `--to-command=` option 注入**: 需要 `-x`, `-tvf` 無效
14. **symlink in tar**: tar 只 list 不 extract
15. **tar 讓檔案 persist**: 上傳後 request 結束就 unlink, 無法 ls 到
16. **讀 PHP source**: `/cmd.php`, `/ls.php` 等直接 GET 回 500 (empty), ls 只 stat 不吐內容
17. **PHP CGI `?-s`**: 不是 CGI mode, 無效
18. **func enumeration**: 試了 20+ 常見名字 (download, exec, cat, system, phpinfo...), 只有原本 4 個 func 存在
19. **ls `--full-time` 等 ls 選項**: 都只是 metadata, ls 無法讀檔內容

## 推測正解方向

- 可能是 **tar binary 的某個 version-specific bug** (特定 argv parsing / option 處理), 我用的 tar version 不同所以沒試出來
- 或是 `/flag` 過濾有**一個特殊字符我沒試到** (unicode normalization / HTTP param pollution 之類)
- 或是 `man` 的 charset filter 其實有邊界 bug
- 所有 func 的 source code 都看不到, blackbox 的解不出來

Flag 直接讀 `.htflag` 的套路 (像 #36 webshell) 這題不適用, 因為 flag 真的在 `/flag` + setuid binary.
