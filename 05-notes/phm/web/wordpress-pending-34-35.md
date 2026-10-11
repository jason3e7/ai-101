# PHM #34-35 Web — wordpress 1 / wordpress 2 (未解)

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 + 100 |
| 狀態 | 卡住 |

## 題目

- #34 wordpress 1 — "Something strange is hidding in the source code, find it."
- #35 wordpress 2 — "Find another strange thing in the source code."
- 連結: [`https://wp.hackme.quest/`](https://wp.hackme.quest/)
- 題目頁 (`/sample-page`) 明示: *dig something from source code, you don't need to exploit any thing. please don't try to bruteforce ANYTHING.*

## 嘗試過程

### 已拿到的線索

- WordPress 4.6.1 (舊版, 可能有 CVE 但題目禁止爆破)
- RSS feed (`/feed`) 列出 post 96 "Backup File", 連到 Dropbox 的 `web-security-course-game2.7z`
- 下載後有完整 WP install + 一個自訂 plugin `wp-content/plugins/core.php`:

```php
Plugin Name: Super f14g Printer
Description: This plugin can print f14g1 for you if you know the password!

function print_f14g() {
    $h = 'm'.sprintf('%s%d','d',-4+9e0);  // "md5"
    if($h($_GET['passw0rd']) === '5ada11fd9c69c78ea65c832dd7f9bbde') {
        if(wp_get_user_ip() === '127.0.0.1') {
            eval(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, $h($_GET['passw0rd'].AUTH_KEY),
                base64_decode('zEFnGVANrtEUTMLVy...'), MCRYPT_MODE_CBC,
                $h($_GET['passw0rd'].AUTH_SALT)));
        } else die('Sorry, Only admin from localhost can get flag');
    }
}
add_action('wp_head', 'print_f14g');
```

- `wp_get_user_ip()` 讀 `HTTP_CLIENT_IP` / `X-Forwarded-For` → 容易偽造 127.0.0.1
- 題目做 GET `?passw0rd=XXX`, md5 要等於 `5ada11fd9c69c78ea65c832dd7f9bbde`

- 另有 post 78 "FLAG2" 是**密碼保護文章** (需要輸入密碼才看得到內容)

### 卡住點

**password 猜不到**:
- MD5 `5ada11fd9c69c78ea65c832dd7f9bbde` 不在 rockyou (14M entries) 中
- 猜了所有 context 相關的字 (f14g, inndy, hackme, pi, 3.1415926, Astrid, 等 ~50 個) 都沒 hit
- 題目禁止爆破, 線索一定在 source code 裡找得到

目前推測密碼可能:
- 藏在 plugin / theme 某處 (comment / 隱藏字串)
- 藏在 HTML / JS 某個 nonce / data attribute
- 來自 WP 某個預設值 (AUTH_SALT, SECURE_AUTH_KEY 之類)

Post 78 的密碼也是同樣問題 — 題目禁爆破, 線索未找到.

## 後續方向

- 把 `dafuqManager.7z` + wp backup 都丟 LLM 做 dead code / strange constant review
- 檢查 WP admin 的 wp-login.php nonce 行為
- 看 wp-content/mu-plugins/ 或其他 wordpress 預設擴充的差異
- `wp.hackme.quest` 的 nginx `/wp-content/uploads/` 回 `nginx/1.10.3` (backend) → 可能有 path confusion / proxy misconfig
