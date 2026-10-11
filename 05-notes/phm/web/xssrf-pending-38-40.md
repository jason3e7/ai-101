# PHM #38-40 Web — xssme / xssrf leak / xssrf redis (未解, 進度 ~60%)

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 100 + 150 + 200 |
| 狀態 | 進行中 (基礎已打通, XSS payload 還差一步) |

## 題目

同一個站 [`https://xssrf.hackme.quest/`](https://xssrf.hackme.quest/):
- #38 xssme — "XSS admin to steal flag"
- #39 xssrf leak — "Steal flag from source code file"
- #40 xssrf redis — "Steal flag from redis"

## 已完成的偵查

### 站點結構

Corgi mail 系統. 註冊/登入後有:
- `sendmail.php?to=<user>` — 寄信 (需過 PoW: `md5(salt+nonce)[:5]==="00000"`, salt 每次刷新)
- `mailbox.php` — 收信匣
- `read.php?id=<id>` — 讀信 (HTML 不過濾, 直接 render)
- `sentmail.php` — 寄件備份

Admin 會自動讀所有 user 寄來的 mail — 這就是 XSS trigger.

### robots.txt 透漏的檔

```
Disallow: /config.php
Disallow: /you/cant/read/config.php/can/you?
Disallow: /backup.zip     ← 這個真的在!
```

`/backup.zip` **用 ZIP legacy encryption 加密**, 內有:
- `imgs/corgi-1.jpg` (網站公開, 可下載到明文)
- `config.php`

→ **known-plaintext attack** 用 `bkcrack`:

```bash
# 下載公開的 corgi-1.jpg, 用 zip -9 壓成相同 compression level
zip -9 -j plain.zip corgi-1.jpg
bkcrack -C backup.zip -c imgs/corgi-1.jpg -P plain.zip -p corgi-1.jpg
# → Keys: 2a723811 7ed741bd 4147417d
bkcrack -C backup.zip -c config.php -k 2a723811 7ed741bd 4147417d -d config.php.raw
python3 -c "import zlib; print(zlib.decompress(open('config.php.raw','rb').read(), -15).decode())"
```

拿到 config.php:

```php
define('DB_USER', '..............');  // 真的被改成點點
define('DB_HOST', 'host=localhost');
define('REDIS_HOST', 'localhost');
define('REDIS_PORT', 25566);
define('FLAG', 'Sorry you have to read config.php from file system to obtain this flag');
$c_hardness = 5;
```

→ 線上 server 的 `FLAG` 常數是真的 flag (備份版是 troll), 要透過 admin 的 XSS 讀. redis 在 localhost:25566, 第 #40 題就是從那邊撈.

### 內容 filter

寄信 content 擋了:
- `<script>` 整個 tag
- `on*=` 跟 `(` 在同個 tag 內
- `function_name(...)` 這類函式呼叫 (parens)
- 單字 `eval`

**沒擋的**:
- backticks (tagged template literal): `` alert`1` ``
- 零參數的 attribute: `onerror=`, `ontoggle=`
- `<details open ontoggle=...>` — details 的 `open` 屬性會**自動展開** → `ontoggle` 自動觸發!
- JS 中的 property assignment (無 parens): `location=...`, `document.title=...`

→ 可行的 XSS 起手:
```html
<details open ontoggle=location='javascript:xxx'>
```

或直接
```html
<details open ontoggle=document.title=document.cookie>
```

(但 `document.body.innerHTML=` 被擋, 因為 `.innerHTML` 的字串 pattern 疑似被 WAF 抓)

### Admin 瀏覽器環境推測

- 看起來是 headless browser 在 server 本機跑 (所以 fetch `localhost:25566` 可以直通 redis — 這就是 "SSRF via XSS")
- Admin 讀信會自動 render HTML, 觸發 `details` 的 ontoggle

## 卡住的點

**從 XSS 把資料 exfil 回來的 payload 寫不出來**:
- 無 parens → fetch(url) 不能直接寫
- 可以用 tagged template: `fetch\`url\`` 但 `fetch(["url"])` 不合法
- `new Image;x.src='url'` 不用 parens 但只能發 GET 沒法讀 response
- 想用 `location=url` 把資料送出去, 但要先把 sendmail PoW 算出來才能透過 mail 回傳

**目前推進策略**:
1. 用 `<details open ontoggle=...>` + 分號分隔多個 statement, 跑一段 inline JS
2. 這段 JS 要:
   - fetch 想拿的東西 (config.php via localhost, 或 redis via `fetch('http://localhost:25566',...)`)
   - 算 sendmail PoW (JS md5 + loop)
   - POST 回 sendmail.php 寄給自己
3. **所有 parens 都要繞**: 可能需要把 payload 分段放在多個 attributes, 或靠 template literal 技巧

還需要克服的具體問題:
- `fetch` 需要 options object → 用 template literal 加 config object 的話字串拼接複雜
- `btoa` / `encodeURIComponent` 都是 function call

## 送出的 flag

目前尚未送出任何 flag 給 #38/#39/#40.
