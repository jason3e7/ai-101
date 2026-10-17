# PHM #90 Crypto — login as admin 2

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 50 |

## 題目

> Please login as admin.
>
> Tips: length extension attack
>
> 連結: [`/login2/`](https://ctf.hackme.quest/login2/)

## 分析

`?show_source=1` 看 source:

```php
function set_user($user_data) {
    global $user, $secret;
    $user = ['name' => $user_data['name'], 'admin' => $user_data['admin']];
    $serialized = http_build_query($user);           // "name=guest&admin=0"
    $sig = md5(md5($secret) . $serialized);           // ← key = md5(secret), 32 bytes
    $all = base64_encode("{$sig}#{$serialized}");
    setcookie('user', $all, time()+3600);
}
```

Cookie 格式: `base64(md5(md5(secret) . serialized) . "#" . serialized)`.

### MD5 length extension

MD5 的 Merkle-Damgård 結構 → 給 `md5(prefix . data)` 跟 `len(prefix)`, 可以**不知道 prefix** 算出 `md5(prefix . data . glue_pad . extra)`.

prefix = `md5(secret)` → 固定 32 bytes.
data = `"name=guest&admin=0"` (18 bytes).
total = 50 bytes.

### Extension payload

append `"&admin=1"` → `parse_str` 時後面的 key 覆蓋前面, 結果 `admin=1`.

攻擊 payload:
```
new_data = "name=guest&admin=0" + glue_pad(50 bytes total) + "&admin=1"
new_sig  = md5_from_state(orig_sig, "&admin=1", 64)   # 64 = 一個 block (50+14 pad)
cookie   = base64(new_sig + "#" + new_data)
```

## 解法

```python
import struct, math, base64

# ... implement md5 from scratch (compress + padding) ...

orig_sig = '6bcb9c9155975a53e951b0b50f137480'  # from guest/guest login
orig_data = b'name=guest&admin=0'
prefix_len = 32  # len(md5(secret) hex)

glue = md5_padding(prefix_len + len(orig_data))  # pad for 50 bytes
append = b'&admin=1'
new_sig = md5_from_state(orig_sig, append, 64)   # 64 bytes already processed
new_data = orig_data + glue + append
cookie = base64.b64encode((new_sig + '#' + new_data.decode('latin-1')).encode('latin-1')).decode()
```

送出 cookie → "You are admin!" + flag.

## Flag

```
FLAG{H3110, 4dm1n1576a70r... 1f y0u kn0w my 53cR37}
```

## 感想

MD5 length extension 教科書題. 關鍵: `md5(md5(secret).data)` 把 `md5(secret)` 當 prefix, length 固定 32 → 可被 extend. 修法用 HMAC-MD5.
