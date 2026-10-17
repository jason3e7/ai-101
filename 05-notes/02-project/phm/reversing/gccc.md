# PHM #46 Reversing — GCCC

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 140 |

## 題目

> Maybe you should try some z3 magic.
>
> 檔案: [`/static/gccc.exe`](https://ctf.hackme.quest/static/gccc.exe)
> PE32 Mono/.NET assembly.

## 分析

`monodis gccc.exe` 解 IL. Class 名稱 `GrayCCC` 已經劇透了 **Gray Code**. Main 邏輯:

```csharp
uint V_0 = uint.Parse(input)
string V_1 = ""
string valid = "ABCDEFGHIJKLMNOPQRSTUVWXYZ{} "
int i = 0
byte V_5 = 0
byte[32] arr = {...}   // 32-byte 固定陣列

while (V_0 != 0) {
    char c = arr[i] ^ (byte)V_0 ^ V_5
    if (c not in valid) exit "Invalid"
    V_1 += c
    V_5 ^= arr[i]
    i++
    V_0 >>= 1
}
if (V_1[0..5] != "FLAG{" || V_1[31] != '}') exit "Invalid"
puts("Your flag is: " + V_1)
```

要找 32-bit uint `V_0` 使得:
- 迴圈剛好跑 32 次 (V_0 高位必為 1)
- 每個 iter 的輸出 char 落在 `[A-Z{} ]`
- V_1[0..4] = `"FLAG{"`, V_1[31] = `"}"`

### Bit 推導

對 iter i: `out[i] = arr[i] ^ ((V_0 >> i) & 0xff) ^ (arr[0] ^ ... ^ arr[i-1])`

每個已知 `out[i]` → 已知 `(V_0 >> i) & 0xff` → 鎖定 V_0 的 bit `[i, i+7]`.

有 6 個已知: F L A G { } (位置 0, 1, 2, 3, 4, 31) → 鎖定 V_0 的 bit 0-11 跟 bit 31.

剩 bit 12-30 (19 bits) 窮舉 2^19 = 524288, 檢查每個 iter 的輸出 char 都在 valid charset.

## 解法

```python
V_4 = bytes.fromhex('a41904827e9e5bc7adfcef8f96fb7e27686892d0f909dbd065b63e5c061b052e')
charset = set("ABCDEFGHIJKLMNOPQRSTUVWXYZ{} ")

def compute(v):
    out = []; V_5 = 0
    for i in range(32):
        c = V_4[i] ^ (v & 0xff) ^ V_5
        if c not in map(ord, charset): return None
        out.append(chr(c)); V_5 ^= V_4[i]; v >>= 1
    return ''.join(out)

# 從 FLAG{ + } 鎖定 bit 0-11, 31
V5_at = [0]
for i in range(32): V5_at.append(V5_at[-1] ^ V_4[i])
known = {0:'F', 1:'L', 2:'A', 3:'G', 4:'{', 31:'}'}
bits = {}
for i, ch in known.items():
    byte_val = V_4[i] ^ V5_at[i] ^ ord(ch)
    for b in range(8):
        if i + b < 32: bits[i+b] = (byte_val >> b) & 1

import itertools
unknown = [b for b in range(32) if b not in bits]
for combo in itertools.product([0,1], repeat=len(unknown)):
    v = 0
    for b in range(32):
        bit = bits.get(b, combo[unknown.index(b)] if b in unknown else 0)
        if bit: v |= 1 << b
    out = compute(v)
    if out and out.startswith('FLAG{') and out.endswith('}'):
        print(v, out); break
# -> 0xda0ab3e2 = 3658134498 → FLAG{DO YOU KNOW GRAY CODE QAQQ}
```

## Flag

```
FLAG{DO YOU KNOW GRAY CODE QAQQ}
```

## 感想

z3 用來解確實是正統解, 但這題 bit 空間只有 19-bit 窮舉 (525K), 直接 brute force 比裝 z3 快多了.
