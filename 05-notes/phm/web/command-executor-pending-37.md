# PHM #37 Web — command-executor (未解)

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 |
| 狀態 | 卡住 |

## 題目

> Here's my useless developer assistant website, try to execute your own command!
>
> 連結: [`https://command-executor.hackme.quest/`](https://command-executor.hackme.quest/)

## 現狀分析

網站有四個功能:

| func | 描述 | 行為 |
|:---|:---|:---|
| `man` | 顯示 man page | 檔名只允許 `[a-zA-Z0-9_-]`, 擋掉 `.`, `/`, 空白 |
| `untar` | 列出 tar 內容 | `tar -tvf` 讀 tmp 檔, 展示用的 filename 可控 (但不影響實際命令) |
| `cmd` | 執行命令 | 嚴格白名單: 只有 `ls` 和 `env` 可以跑, 不接受任何參數 |
| `ls` | 列目錄 | file 參數經 `escapeshellarg` 包起來, 可注入 `-la` / `-R` 等 flag 但不能加 path |

### 已知 flag 位置

```
ls / →
-r--------   1 flag root   37 Jan  9  2018 flag
-rwsr-xr-x   1 flag root 9080 Jan 19  2018 flag-reader
-rw-r--r--   1 root root  653 Jan  9  2018 flag-reader.c
```

- `/flag` mode 400, owner `flag` → 只有 flag user 可讀
- `/flag-reader` setuid flag → 要能 **執行這個 binary** 才能拿 flag

## 嘗試過的失敗路線

1. **直接 cmd exec**: `cmd=flag-reader`, `cmd=/flag-reader`, `cmd=sudo` ...全部回 "command not found", 白名單只有 ls/env
2. **ls 加參數**: `file=-la /` 被 escapeshellarg 包成 `ls '-la /'`, ls 當成 filename, 失敗
3. **cmd 用 `env`**: `env` 單獨跑印 env vars, 不接受 program 參數
4. **cmd 用 `$IFS` 繞空白**: `cmd=ls$IFS-la` 整段當 command name, "command not found"
5. **untar filename injection**: 檔名含 `;id` 被截掉, 真正的 tar 讀的是 PHP 給的 tmp path, 不吃我們的檔名
6. **tar GNU option injection**: `--checkpoint-action=exec=` 要跟 `-x` 配合, `-tvf` 無效
7. **symlink in tar**: tar 只 list 不 extract, symlink 當成 metadata, 不會解開
8. **man `-l path` read file**: 參數過濾擋掉 `/`, 無法 pass 檔案路徑
9. **讀 cmd.php / ls.php / man.php / untar.php source**: 直接 HTTP GET 回 500 (empty), ls 只顯示 metadata 不顯示內容

## 推測正解方向

- 可能是 **tar implementation 的 argv parsing bug** (某個版本 tar 把 filename 當 option 處理)
- 或是 **ls.php / man.php 本身**的某個 edge case 我漏看了
- Flag 直接讀 `.htflag` 的套路 (像 #36 webshell) 這題似乎不適用, 因為 flag 真的在 `/flag` + setuid binary
- 完整 source code 沒公開, 要靠 blackbox probing 把 whitelist 跟 filter 繞開
