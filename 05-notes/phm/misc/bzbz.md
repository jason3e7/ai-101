# PHM Misc #13 — BZBZ (50 pts) · 卡住未解

[← 返回索引](../README.md)

## 題目

> Do you like ACGN? Do you know bilibili source code leak?

連結: [`/bzbz/`](https://ctf.hackme.quest/bzbz/) — 一份山寨 bilibili 登入頁

## 現況: 找到 `login.php` 但觸發 alert

### 頁面結構

`/bzbz/` 回一份高度仿真的 bilibili 首頁 (大量 base64 inline icon). 唯一 form:

```html
<form id="form1" action="login.php" method="GET">
  <input name="username" placeholder="你的手機號/郵箱">
  <input name="password" type="password" placeholder="密碼">
</form>
```

JavaScript 綁 `btn-login` 點擊 → `form1.submit()`.

### `login.php` 的行為

任何方法 (GET / POST / PUT / 任意 params / 任意 headers / 任意 cookies) 送到 `/bzbz/login.php`, server 回原始頁面 + 一行:

```html
<script>alert('You must be a employee from bilibili!')</script>
```

Base `/bzbz/` 回應 **沒有** 這個 alert; `login.php` 無條件加這段. 推測是 server-side 檢查某個條件, 條件達成就不加 alert.

### 已試過但失敗的 bypass

- Credentials: `admin/admin`, `inndy/inndy`, `inndy/cnm`, `cnm/cnm`, `bilibili/bilibili`, `root/toor`, 洩漏 appkey/secret `1d8b6e7d45233436/560c52ccd288fed045859ed18bffd973` 等
- Headers: `User-Agent: Bilibili/bilibili-employee/go-common`, `Referer: bilibili.com/github.com/openbilibili/go-common`, `Cookie: SESSDATA/DedeUserID/employee=1`
- IP spoofing: `X-Forwarded-For: 127.0.0.1/10.0.0.1`, `X-Real-IP`, `Client-IP`
- Host header: `inner.bilibili.co`, `bilibili.com`
- Basic auth: 多組常見組合
- HTTP methods: PUT / PATCH / DELETE / OPTIONS 都一樣加 alert
- Email domain: `admin@bilibili.com`, `inndy@bilibili.com` 等
- Literal strings from message: `"a employee from bilibili"` 當 username
- 路徑探測: `/bzbz/admin`, `/x/internal`, `.git/`, `source.zip`, `login.php.bak`, `.phps`, `view-source` 全部 404

## 可能方向 (未試)

- 2018 洩漏的 go-common repo 裡**特定 Chinese 字串 / 註解內容** (`cnmcnm` 等) 當 username/password 的其他組合
- `/bzbz/` 可能有特定的 **PHP wrapper 路徑** (`php://input` + POST body)
- 洩漏源碼裡**內部用的 `appkey`** 跟特定 URL hash 配對
- 看看是否要**特定路徑結尾** (`/bzbz/login.php/x`) 之類的 trailing path
- 試 `Content-Length: 0` 或 chunked transfer encoding

## 收斂

看出來 `login.php` 的 alert 是條件觸發的, 推測需要「模擬自己是 bilibili 內部員工」才能跳過. 但不知道確切的 check 條件, 暴力過一輪 header / cookie / credential 沒命中. 先存著, 之後找洩漏資料再試.
