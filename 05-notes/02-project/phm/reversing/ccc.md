# PHM #47 Reversing — ccc

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 150 |

## 題目

> ccc cc
>
> 檔案: [`/static/ccc`](https://ctf.hackme.quest/static/ccc)
> ELF 32-bit.

## 分析

main 讀 input (max 0x40 bytes), 送到 `verify(input, len-1)`.

```c
int verify(char *s, int len) {
    if (len != 42) return 0;
    int counter = 0;
    for (int i = 3; i <= 42; i += 3) {
        uint32_t crc = crc32(0, s, i);     // CRC32 of first i bytes
        if (crc != targets[counter]) return 0;
        counter++;
    }
    return 1;
}
```

**input 是 42 bytes, 每 3 bytes 的 CRC32 都要對上 14 個 target.**

這等於 "sequential CRC32 constraint": 已知前綴 CRC + 下個目標 CRC + 3 bytes extend → 可以**窮舉 3 bytes** (假設 printable ASCII, 95^3 = 857K per block). 14 區塊乾淨地往前推.

## 解法

```python
import zlib, itertools
targets = [0xd641596f, 0x80a3e990, 0xc98d5c9b, 0x0d05afaf, 0x1372a12d, 0x5d5f117b,
           0x4001fbfd, 0xa7d2d56b, 0x7d04fb7e, 0x2e42895e, 0x61c97eb3, 0x84ab43c3,
           0x9fc129dd, 0xf4592f4d]

chars = [bytes([c]) for c in range(0x20, 0x7f)]
prefix = b''
for tgt in targets:
    for a, b, c in itertools.product(chars, repeat=3):
        if zlib.crc32(prefix + a + b + c) == tgt:
            prefix += a + b + c; break
print(prefix.decode())
# -> FLAG{CRC32 is fun, but brute force is not}
```

跑約 30 秒 (printable ASCII 每區塊 ~857K candidate).

## Flag

```
FLAG{CRC32 is fun, but brute force is not}
```

## 感想

CRC32 不是碰撞-abuse-safe hash. 它線性 + 可以本機算 → 加上**遞增 CRC** 這個設計, 每區塊只有 3 bytes 要猜, 甚至可以搞 CRC32 的 inverse 直接解. Brute force 爛歸爛, 這題剛好夠簡單直接吃.
