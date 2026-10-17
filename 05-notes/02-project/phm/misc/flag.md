# PHM #1 Misc — flag

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 10 |
| 解題數 | 2918 (最熱門入門題) |

## 題目

> All flags are in this format:
> `FLAG{This is flag's format}`

## 解法

這題是整站的 **flag 格式說明暨教學題**. 題目字面上就告訴你 flag 的格式長這樣, 整個字串 `FLAG{This is flag's format}` 本身就是答案 — 不是提示, 不是範例, 是真的 flag.

把這個字串送到 PHM 的 submit 表單就 AC:

```http
POST /scoreboard/?capture=the_flag
Content-Type: application/x-www-form-urlencoded

name=json3e74101&flag=FLAG{This is flag's format}
```

## Flag

```
FLAG{This is flag's format}
```

## 感想

CTF 常見的「暖身題」, 教新手兩件事: (1) flag 的格式, (2) 怎麼送 flag. 送完這題總分從 0 跳到 10, 代表 submit 流程 pipeline 整個通了, 之後其他題只要有 flag 就能直接 POST.
