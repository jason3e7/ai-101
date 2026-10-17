# PHM #20 Web — scoreboard

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 30 |

## 題目

> **DO NOT** ATTACK or SCAN scoreboard, you don't need to do that.
>
> 連結: [`/scoreboard`](https://ctf.hackme.quest/scoreboard)

## 分析

題目直接說「別攻擊」, 所以答案不在打 server, 而在**正常的 HTTP response 本身**. 直接看 response headers:

```bash
curl -I https://ctf.hackme.quest/scoreboard/
```

```
HTTP/2 200
server: nginx/1.24.0 (Ubuntu)
date: ...
content-type: text/html; charset=UTF-8
x-flag: FLAG{Header can hide some data aswell.}      ← 這裡
set-cookie: ...
```

## 解法

```bash
curl -I https://ctf.hackme.quest/scoreboard/ | grep -i x-flag
```

## Flag

```
FLAG{Header can hide some data aswell.}
```

## 感想

30 分的梗題: 平常用 browser DevTools 大家只看 response body, header 不太看. 題目講「別掃」是誠懇提示 flag 就在**瀏覽器已經送回的頭上**.
