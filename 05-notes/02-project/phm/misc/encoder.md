# PHM #6 Misc — encoder

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 50 |
| 解題數 | 260 |
| 附檔 | [`/static/encoder.tar.xz`](https://ctf.hackme.quest/static/encoder.tar.xz) |

## 題目

> Can you decode this?

解壓後拿到 `encoder.py` + `flag.enc` (29 MB).

```python
#!/usr/bin/env python2
import random, string

def rot13(s): return s.translate(...)
def base64(s): return s.encode('base64').strip()
def hex(s): return s.encode('hex')
def upsidedown(s): return s.swapcase()

flag = 'FLAG{.....................}'
E = (rot13, base64, hex, upsidedown)

for i in range(random.randint(30, 50)):
    c = random.randint(0, 3)
    flag = '%d%s' % (c, E[c](flag))

open('flag.enc', 'w').write(flag)
```

每輪隨機挑一個 encoder (rot13 / base64 / hex / upsidedown), 把編號**前綴到字串前面**. 迭代 30-50 輪.

## 分析

每輪前綴 1 個字元 (0-3) 標示用了哪個 encoder → **解開第一個字元就知道怎麼反推**. 這四個 encoder 的反運算:

- `0 rot13` → rot13 (自反)
- `1 base64` → base64 decode
- `2 hex` → unhex
- `3 upsidedown` → swapcase (自反)

一直剝到字串開頭變成 `FLAG{` 就停.

## 解法

```python
import base64, codecs, string
tr = str.maketrans(string.ascii_uppercase + string.ascii_lowercase,
                   string.ascii_uppercase[13:] + string.ascii_uppercase[:13] +
                   string.ascii_lowercase[13:] + string.ascii_lowercase[:13])
INV = {
    '0': lambda s: s.translate(tr),
    '1': lambda s: base64.b64decode(s).decode(),
    '2': lambda s: codecs.decode(s, 'hex').decode(),
    '3': lambda s: s.swapcase(),
}
enc = open('flag.enc').read().strip()
while not enc.startswith('FLAG{'):
    enc = INV[enc[0]](enc[1:])
print(enc)
```

跑 47 輪就 AC. 從 29 MB 一路解到 48 bytes.

## Flag

```
FLAG{KEEP CLAM AND DECODE!}
```

("CLAM" 是作者刻意的 typo 梗, 應該是 "CALM")

## 感想

- 這題關鍵在「**作者留了明文的 opcode 給你反推**」. 真實場景大多沒這麼好心 — 密文沒標示用了什麼演算法, 得先 fingerprint
- hex / base64 的解法很好分辨 (字元集), rot13 跟 upsidedown 需要字母表觀察. 本題直接給編號, 把 reverse engineering 變成純 decode loop
- 29 MB → 47 bytes 的壓縮倍率很驚人, 可見 hex + base64 每輪膨脹 2x / 1.33x, 幾十輪疊起來尺寸爆炸
