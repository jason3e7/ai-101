# PHM #31 Web — dafuq-manager 1

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 50 |

## 題目

> Login as guest and find flag 1
>
> 連結: [`https://dafuq-manager.hackme.quest/`](https://dafuq-manager.hackme.quest/)

## 分析

Login `guest/guest` → 進入檔案管理器. 看得到的檔案:

- `dafuqManager.7z` — source code backup
- `index.html`
- `see-me-if-you-need-tips.txt` — tips
- `source-code-in-dafuqManager.7z-please-download-it.txt`

讀 `see-me-if-you-need-tips.txt`:
```
Do you know cookie? Create a cookie named `help` with value `me`!
```

設 `help=me` cookie 後任意 request → alert "Very good. You know how to create cookie. How about tamper a cookie?"

→ 下一步: tamper cookie `show_hidden=no` → `yes`.

有 `show_hidden=yes` cookie 後, 列表多出:
- `.good.job.here.is.your.hidden.flag-1.txt` ← flag1
- `.where-is-flag-2-please-tell-me.txt` ← hint for flag2

## 解法

```bash
# 1. Login
curl -c cookies.txt -X POST 'https://dafuq-manager.hackme.quest/index.php?action=login' \
  --data-urlencode 'p_user=guest' --data-urlencode 'p_pass=guest' --data-urlencode 'lang=en'

# 2. Add show_hidden=yes to cookies.txt
# 3. Fetch flag file
curl -b cookies.txt 'https://dafuq-manager.hackme.quest/data/guest/.good.job.here.is.your.hidden.flag-1.txt'
```

## Flag

```
FLAG{Wow, how did you found me? I was hidden!}
```

## 感想

兩步 cookie tampering 教學: 先 `help=me` (程式提示路徑), 再改 `show_hidden` 看隱藏檔. 原始碼 (`fun_extra.php::get_show_item`) 的 dotfile 過濾只檢查 `$_COOKIE['show_hidden'] != 'yes'`, 完全信任 client cookie.

後續 [#32 dafuq-manager 2](dafuq-manager-2.md) + [#33 dafuq-manager 3](dafuq-manager-3.md) 都要靠 `dafuqManager.7z` 的 source review 找 vuln.
