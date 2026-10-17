# PHM #44 Reversing — pyyy

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 110 |

## 題目

> Can you pass the challenage?
>
> 檔案: [`/static/pyyy.pyc`](https://ctf.hackme.quest/static/pyyy.pyc)
> Python 2.7 bytecode.

## 分析

`uncompyle6 pyyy.pyc` 拿到源碼. 核心邏輯:

```python
F = (a, b, c, d, e)   # 5 個大 prime
m, g = <big primes>

z = []
for i, f in enumerate(F):
    n = pow(f, m, g)
    l = factorial(g % 27777) mod n   # Y-combinator 包裝的 factorial
    c = raw_input('Channenge #%d:' % i)
    if int(c) != l: exit()
    z.append(l)

z.sort()
flag_fmt = ''.join(gg[fib(i+2)] for i in range(16))   # 從 gg 挑 16 chars → "flag is FLAG{%s}"
flag_body = ''.join(data[pow((gcd(z[i%5], z[(i+1)%5])*2+1) * g, F[i%5]*(i*2+1), len(data))] for i in range(32))
print(flag_fmt % flag_body)
```

兩步:
1. 用 Python 3 (配 `math.gcd` 代替 `fractions.gcd`) 把 5 個 `l_i` 算出來 (每個是 `factorial(g%27777) mod pow(f_i, m, g)`, 大約秒級完成)
2. z 排序後直接套最後的公式

## 解法

```python
from math import gcd
# 把 a..e, m, g, data, gg 從 decompile 搬進來
F = (a, b, c, d, e)
z = []
for i, f in enumerate(F):
    n = pow(f, m, g)
    k = g % 27777
    fac = 1
    for j in range(1, k+1): fac = fac * j % n
    z.append(fac)
z.sort()
inner = ''.join(data[pow((gcd(z[i%5], z[(i+1)%5])*2+1)*g, F[i%5]*(i*2+1), len(data))] for i in range(32))
print(f'FLAG{{{inner}}}')
```

## Flag

```
FLAG{VBXDVV4jkVVS4hVVj7NVV1heVVX1jVVh}
```

## 感想

Python 2 pyc 用 `uncompyle6` 直接解, Y-Combinator 包裝 factorial / fib 是炫技但實際算起來就是普通遞迴. 關鍵是看出 `factorial(g%27777)` 只是「大 factorial mod n」, 不需要用 lambda. 15 行 Python 3 搞定.
