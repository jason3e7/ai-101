# PHM #34-35 Web — wordpress 1 / wordpress 2 (未解)

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 + 100 |
| 狀態 | 卡住 (已多次挑戰) |

## 題目

- #34 wordpress 1 — "Something strange is hidding in the source code, find it."
- #35 wordpress 2 — "Find another strange thing in the source code."
- 連結: [`https://wp.hackme.quest/`](https://wp.hackme.quest/)
- 題目頁 (`/sample-page`) 明示: *dig something from source code, you don't need to exploit any thing. please don't try to bruteforce ANYTHING.*

## 已拿到的線索

- WordPress 4.6.1 (舊版, 2016 年發布, 寫死「site is up-to-date」是梗)
- RSS feed (`/feed`) 列出 post 96 "Backup File", 連到 Dropbox 的 `web-security-course-game2.7z`
- 下載後是完整 WP install + **自訂 plugin `wp-content/plugins/core.php`**:

```php
/**
 * @package f14gPrinter
 * @version 3.1415926
 */
/*
Plugin Name: Super f14g Printer
Plugin URI: https://game2.security.ntu.st
Description: This plugin can print f14g1 for you if you know the password!
Author: Inndy Lin
Version: 3.1415926
Author URI: https://inndy.tw
*/

function print_f14g() {
    $h = 'm'.sprintf('%s%d','d',-4+9e0);  // => 'md5'
    if($h($_GET['passw0rd']) === '5ada11fd9c69c78ea65c832dd7f9bbde') {
        if(wp_get_user_ip() === '127.0.0.1') {
            eval(mcrypt_decrypt(MCRYPT_RIJNDAEL_256,
                $h($_GET['passw0rd'].AUTH_KEY),
                base64_decode('zEFnGVANrtEUTMLVy...很長...JUpqIw=='),
                MCRYPT_MODE_CBC,
                $h($_GET['passw0rd'].AUTH_SALT)));
        } else {
            die('</head><body><h1>Sorry, Only admin from localhost can get flag');
        }
    }
}
add_action('wp_head', 'print_f14g');
```

- `wp_get_user_ip()` 讀 `HTTP_CLIENT_IP` / `X-Forwarded-For` → 可偽造 127.0.0.1
- 密文 base64 解開是 160 bytes = 5 個 Rijndael-256 block, CBC mode, key/iv 都是 md5(password . AUTH_KEY/SALT)
- 另有 **post 78 "FLAG2"** 是密碼保護文章 (需要 WP 內建的 post password) — 多半是 #35

## 卡住點: 找不到 md5(密碼) = `5ada11fd9c69c78ea65c832dd7f9bbde` 的 preimage

### 試過的方向 (全失敗)

1. **rockyou 14M wordlist** — 不在列
2. **rockyou 100k × 10 種 mangle rule** (prefix/suffix 加 `f14g`/`super`/`!`/`123` 等) — 無 hit
3. **~300 個 context-related guesses**:
   - Plugin header 的每個字段: `f14gPrinter`, `Super f14g Printer`, `3.1415926`, `Inndy Lin`, `inndy.tw`, `game2.security.ntu.st`
   - 2-piece combo `piece1[sep]piece2` × 7 separator ≈ 8574 組
   - Pi 數字截前 N 位
   - WP context: `wordpress`, `admin`, `pleasehackme`, `PHM`
   - 課程 context: `ntust`, `web-security-course`, `game2`
4. **把整個 WP backup 的檔內 token 都 hash 一遍** (regex `[A-Za-z0-9_@\-.]{3,50}` + 整行) — 無 hit
5. **系統字典** (`/usr/share/dict/words`, `american-english`) — 無 hit

### 可能漏掉的點

- 密碼可能是**非 ASCII** (日文 / 繁中), 字典沒掃
- 可能需要**特殊 encode / escape 後的字串** (URL encoded / htmlentities 等)
- 可能跟 WP 本身的某個 default salt / key 連動 (但沒 wp-config 可查)
- 題目 `game2.security.ntu.st` 可能是 NTUST 某門資安課的材料, 密碼可能是**課程特有梗**, 不是 agent 能從外部推出的

### 現有的驗證方式 (Oracle)

送 `?passw0rd=XXX` 到 live site, 看 HTML <head> 有無 `Sorry, Only admin from localhost can get flag`.
- 有 → md5(XXX) == `5ada11fd...`, 可以把 X-Forwarded-For 設成 127.0.0.1 進入 eval 分支
- 無 → 密碼錯

→ 有 oracle 但密碼空間太大

### 推測正解方向

- 密碼可能寫在 NTUST 的某個講義 / Inndy 的某個 blog post / 當時課程公告
- 或者是 **Please Hack Me 的 Discord / 社群梗** (CTF ad-hoc trivia, 跟 Misc slow / slowcipher 類似死法)
- #35 wordpress 2 = post 78 的 post password, 可能跟 #34 共用密碼
