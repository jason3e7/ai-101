# PHM #92 Crypto — login as admin 5

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 80 |

## 題目

> Tips: stream cipher
>
> 連結: [`/login5/`](https://ctf.hackme.quest/login5/)

## 分析

Source:

```php
$cipher = new RC4($flag);                             // RC4 的 key 是 flag 本身!
function set_session($user) {
    $cookie = base64_encode($cipher->encrypt(json_encode($user)));
    setcookie('user5', $cookie, ...);
}
function restore_session() {
    $data = $cipher->decrypt(base64_decode($_COOKIE['user5']));
    $user = json_decode($data, true);
}
```

### Stream cipher nonce reuse 的致命弱點

RC4 是 stream cipher: `cipher = plaintext XOR keystream`. 整個 session 都用同一個 keystream (因為每次 request 都 new 一個 RC4($flag) 從頭開始).

### Guest 登入給我們 known plaintext

- Login as `guest/guest` → cookie = base64(RC4(`{"name":"guest","admin":false}`))
- 我們**知道 plaintext** → `keystream[:30] = cookie XOR {"name":"guest","admin":false}`

### Forge admin cookie

用拿到的 keystream, 加密**新的 plaintext**:
- `{"name":"guest","admin":true}` (29 bytes, 剛好塞進 keystream[:30])
- `fake_cookie = base64(keystream[:29] XOR new_plain)`

## 解法

```python
import base64, urllib.parse

# Login as guest, 拿 cookie
val = urllib.parse.unquote('U%2FosUbnY8nSrWz4WPwKSwWPzKq9tOIQ9eCWnN5E%2B')
cipher = base64.b64decode(val)                                   # 30 bytes
plain = b'{"name":"guest","admin":false}'                        # 30 bytes

ks = bytes(c ^ p for c, p in zip(cipher, plain))                 # keystream

admin_plain = b'{"name":"guest","admin":true}'                   # 29 bytes
admin_cipher = bytes(k ^ p for k, p in zip(ks, admin_plain))
fake_cookie = base64.b64encode(admin_cipher).decode()
```

送 cookie → "Hello, guest" + flag.

## Flag

```
FLAG{Every hacker should know RC4 cipher}
```

## 感想

跟 #91 passcode 同系: stream cipher 的 key/nonce 重用 → XOR known plaintext 就拿到 keystream, 可以**自由改寫 plaintext**. 這題還加碼: RC4 key 就是 flag 本身, 但其實我們**不需要 recover key**, 只要 forge cookie 進得去就會把 flag 印給我們看.
