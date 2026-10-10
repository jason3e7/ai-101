# PHM #45 Reversing — accumulator

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 120 |

## 題目

> Reverse this for the flag
>
> 檔案: [`/static/accumulator`](https://ctf.hackme.quest/static/accumulator)
> ELF 64-bit, dynamically linked (libcrypto.so.1.0.0), stripped.

## 分析

main 流程:

```c
read(input, 0x400 bytes until '\xff')
SHA512(input, len, sha_buf)
accumulator(sha_buf, 0x40)      // 累積 SHA512 的 64 bytes
accumulator(input, len)         // 累積 input bytes
puts("ok" / "fail")
```

`accumulator` 函式有兩個全域變數: 索引 `idx` (起 0) 跟累積和 `acc` (起 0). 對每個 byte:

```
acc += byte
if acc != table[idx*4]: fail
idx += 1
```

兩次呼叫**共用 idx 跟 acc**. 所以 table 的 diff 就是原本 bytes:
- `table[0..0x3f]` = SHA512(input) 的 64 bytes 的累積和
- `table[0x40..]` = input bytes 的累積和

直接讀 `table[i] - table[i-1]` 就還原每個 byte.

## 解法

```python
import struct, hashlib
with open('accumulator', 'rb') as f: data = f.read()
# table 在 virt 0x601080 (file offset 0x1080)
offset = data.find(bytes.fromhex('c3000000ff000000ed010000'))
entries = []
prev = 0
i = offset
while True:
    val = struct.unpack('<I', data[i:i+4])[0]
    diff = val - prev
    if not (0 <= diff <= 255): break
    entries.append(val); prev = val; i += 4
# diffs
prev = 0; bytes_ = []
for e in entries:
    bytes_.append(e - prev); prev = e
flag_input = bytes(bytes_[64:])
assert hashlib.sha512(flag_input).digest() == bytes(bytes_[:64])
print(flag_input)
# -> FLAG{051339467306...}
```

## Flag

```
FLAG{051339467306f9769350136b41c330840eebcac337f1b8b0dc03e58be14fe690b123f61b0c0b35fc93ccc72100459369ef8531a1e8a7b4299e7b9d970b9a23aa}
```

## 感想

Table-based verification 很常見 — 這題獨特是**共用 state 連結兩段 (SHA512 + input)**. diff 直接還原每個 byte 比暴力試單 byte 快多了.
