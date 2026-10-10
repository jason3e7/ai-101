# PHM #97 Crypto — ffa (finite field arithmetic)

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 270 |

## 題目

> finite field arithmetic
>
> 檔案: [`/static/ffa.tar.xz`](https://ctf.hackme.quest/static/ffa.tar.xz)

```python
import sympy
m = sympy.randprime(2**257, 2**258)
M = sympy.randprime(2**257, 2**258)
a, b, c = [(sympy.randprime(2**256, 2**257) % m) for _ in range(3)]

x = (a + b * 3) % m
y = (b - c * 5) % m
z = (a + c * 8) % m

flag = int(open('flag', 'rb').read().strip().hex(), 16)
p = pow(flag, a, M)
q = pow(flag, b, M)
json.dump({ k: globals()[k] for k in "Mmxyzpq" }, open('crypted', 'w'))
```

輸出給 `x, y, z, m, M, p, q`.

## 分析

### Step 1 — 從 linear system 解 (a, b, c) mod m

3 條線性等式 3 個未知數, 在 mod m 下直接解:

- `x = a + 3b (mod m)`
- `y = b - 5c (mod m)`
- `z = a + 8c (mod m)`

消元:
```
b = (5x - 8y - 5z) * inv(7) mod m
c = (b - y) * inv(5) mod m
a = (x - 3b) mod m
```

7 跟 5 都跟 m (257-258 bit prime) 互質, 乘法逆元存在.

### Step 2 — 從 (p, q, a, b) 還原 flag

- `p = flag^a mod M`
- `q = flag^b mod M`

因為 `a, b` 是獨立選的大 prime, 幾乎必然 `gcd(a, b) = 1`. 用 **extended Euclidean** 找 u, v 讓 `u*a + v*b = 1`.

```
flag = flag^1 = flag^(u*a + v*b) = (flag^a)^u * (flag^b)^v = p^u * q^v (mod M)
```

即使 u 或 v 是負數 (Python `pow(x, -1, M)` 處理負指數就是 modular inverse) 也 OK.

## 解法

```python
import json
from math import gcd
d = json.load(open('crypted'))
M, m, x, y, z, p, q = (d[k] for k in "Mmxyzpq")

b = ((5*x - 8*y - 5*z) * pow(7, -1, m)) % m
c = ((b - y) * pow(5, -1, m)) % m
a = (x - 3*b) % m

assert gcd(a, b) == 1
# ext Euclidean: u*a + v*b = 1
def eea(a, b):
    old_r, r = a, b; old_s, s = 1, 0; old_t, t = 0, 1
    while r:
        q = old_r // r
        old_r, r = r, old_r - q*r
        old_s, s = s, old_s - q*s
        old_t, t = t, old_t - q*t
    return old_r, old_s, old_t
_, u, v = eea(a, b)

flag_int = (pow(p, u, M) * pow(q, v, M)) % M
h = hex(flag_int)[2:]
if len(h) % 2: h = '0' + h
print(bytes.fromhex(h))
# -> b'FLAG{Math is simple, right? OwO}'
```

## Flag

```
FLAG{Math is simple, right? OwO}
```

## 感想

兩個獨立弱點**湊在一起**才拆得開:

1. x, y, z 幫 a, b, c 洩漏到可解的 linear system
2. gcd(a, b) = 1 + extended Euclidean 讓 flag 直接從 p, q 倒推

270 分題目但只要認出「3 條等式 3 未知 + 兩個 modexp」, 完全不用 factor n, 15 行 Python 搞定. 這題名字 "finite field arithmetic" 其實說明了全部.
