# PHM #50 Reversing — what-the-hell

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 190 |

## 題目

> Tips: modinv, Something is slow there in my code, make it faster.
>
> 檔案: [`/static/what-the-hell`](https://ctf.hackme.quest/static/what-the-hell)
> ELF 32-bit, not stripped.

## 分析

main 讀 scanf `"%u-%u"` 兩個 uint, 呼叫 `calc_key3(a, b)`:

```c
int calc_key3(uint32_t a, uint32_t b) {
    if (a * b != 0xddc34132) return 0;           // 32-bit multiply
    if ((a ^ 0x7e) * (b + 0x10) != 0x732092be) return 0;
    if (((a - b) & 0xfff) != 0xcdf) return 0;
    if (!is_prime(a)) return 0;
    for (int i = 1; i <= 10000000; i++) {
        if (what(i) == a) return i * b + 1;      // 遞迴 Fibonacci!! 慢爆
    }
    return 0;
}
```

`what(n)` 是**遞迴 Fib** (指數時間), i=887 就跑不完. 但同檔案有 `fast_what()` 迭代版 — hint "make it faster" 就是指**把 call 從 what 改到 fast_what**.

### 推 a, b

直接用 Fibonacci + 條件篩選:

```python
# a 必須是 32-bit Fibonacci 數字 (fast_what(i) for i < 10M)
# 必須是 prime
# a * b ≡ 0xddc34132 (mod 2^32) → b = 0xddc34132 * modinv(a, 2^32) & 0xffffffff
#   (這就是 Tip "modinv")
# 再驗 (a^0x7e)*(b+0x10) ≡ 0x732092be AND (a-b)&0xfff == 0xcdf

fibs = [None, 1, 1]
while len(fibs) < 10000000:
    fibs.append((fibs[-1] + fibs[-2]) & 0xffffffff)

for i, a in enumerate(fibs):
    if not a or a % 2 == 0: continue
    b = (0xddc34132 * mod_inverse(a, 2**32)) & 0xffffffff
    if ((a ^ 0x7e) * (b + 0x10)) & 0xffffffff != 0x732092be: continue
    if (a - b) & 0xfff != 0xcdf: continue
    if not isprime(a): continue
    print(f"i={i}, a={a}, b={b}")
# -> i=887, a=4284256177, b=1234567890
```

### 跑 binary

```bash
# patch: 把 call what(i) 改成 call fast_what(i), 把 i 起始從 1 改成 2 (fast_what(1) 會 hang)
echo "4284256177-1234567890" | ./what-the-hell-patched
# -> Input the key: Flag is FLAG{modules inverse can help you..}
```

## Flag

```
FLAG{modules inverse can help you..}
```

## 感想

兩個 reversing 梗一起用: 
1. **modinv** 把乘法方程式反解 (a*b ≡ target mod 2^32)
2. 看懂 "make it faster" = 把 **遞迴 Fib 換掉**, 用迭代版

Hint 很誠懇就直接寫 "modinv" 跟 "slow", 剩下照指示做.
