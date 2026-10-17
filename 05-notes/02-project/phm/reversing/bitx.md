# PHM #48 Reversing — bitx

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 150 |

## 題目

> bits?
>
> 檔案: [`/static/bitx`](https://ctf.hackme.quest/static/bitx)
> ELF 32-bit.

## 分析

`main → verify(argv[1])`. verify 逐 byte 檢查:

```c
for (i = 0; input[i] != 0 && table[i] != 0; i++) {
    uint8_t swapped = ((table[i] & 0xaa) >> 1) | ((table[i] & 0x55) << 1);
    if ((input[i] + 9) != swapped) return 0;
}
return 1;
```

**bit_swap** 把相鄰 bit pair 對調 (0xaa = 10101010 / 0x55 = 01010101). 反轉: `input[i] = bit_swap(table[i]) - 9`.

Table 在 `0x804a040`:

```
8faa85a0 48ac4095 b616be40 b41697b1 bebc16b1 bc169d95 bc411636 42959516 40b1beb2 1636423d 3d4900
```

## 解法

```python
table = bytes.fromhex('8faa85a048ac4095b616be40b41697b1bebc16b1bc169d95bc41163642959516' + '40b1beb21636423d3d49')
def bit_swap(x): return ((x & 0xaa) >> 1) | ((x & 0x55) << 1)
flag = bytes((bit_swap(b) - 9) & 0xff for b in table)
print(flag)
# -> b'FLAG{Swap two bits is easy 0xaa with 0x55}'
```

## Flag

```
FLAG{Swap two bits is easy 0xaa with 0x55}
```
