# PHM #93 Crypto — xor

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 90 |

## 題目

> I've X0Red some file, could you recover it?
>
> 檔案: [`/static/xor.gz`](https://ctf.hackme.quest/static/xor.gz)

## 分析

### 檔案

`xor.gz` 解開是 `xor`, 23200 bytes. hexdump 看起來像 **文字 XOR 短 key** — 很多低字元, 有明顯重複 pattern.

### 找 key length — IoC 掃一遍

對每個 candidate `k`, 把密文每隔 `k` 個 byte 切成 column, 算 column 的 Index of Coincidence 平均, IoC 顯著高過 random 的就是 key 長度:

```python
klen=9: avg_ioc=0.0560
klen=18: 0.0559
klen=27: 0.0559
```

→ key 長度 = 9.

### 猜 key — 空格最高頻

英文 plaintext 最常出現空格 (0x20). 每個 column 的 **mode byte** XOR 0x20 大概就是對應 key byte:

```python
from collections import Counter
cols = [data[i::9] for i in range(9)]
key = bytes(Counter(c).most_common(1)[0][0] ^ 0x20 for c in cols)
# -> b'hackmepls'
```

Key: `hackmepls` ("hack me please").

### 解出 Wikipedia XOR 條目, flag 在裡面

```python
plain = bytes(c ^ b'hackmepls'[i%9] for i, c in enumerate(data))
# "Exclusive or From Wikipedia, the free encyclopedia..."
import re
re.search(rb'FLAG\{[^}]+\}', plain).group()
# -> FLAG{Exclusive oR 15 y0ur fr1end}
```

## Flag

```
FLAG{Exclusive oR 15 y0ur fr1end}
```

## 感想

教科書等級 repeating-key XOR 的拆法: IoC 找長度 → 用「空格最頻」反推 key byte. 作者偷藏在 Wikipedia 文章裡的 flag 要 grep 才找到.
