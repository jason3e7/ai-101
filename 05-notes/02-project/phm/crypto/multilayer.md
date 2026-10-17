# PHM #95 Crypto — multilayer

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 150 |

## 題目

> Decrypt multi-layer encryption and capture the flag
>
> 檔案: [`/static/multilayer.7z`](https://ctf.hackme.quest/static/multilayer.7z)

解開有:
- `encrypt.py`: 加密腳本
- `encrypted`: 加密後輸出 (RSA 參數 + base64)

## 分析

### 加密四層

```python
# layer1: 大寫 A-Z 隨機 substitution (非字母原樣)
# layer2: byte * 17 mod 251
# layer3: XOR with LCG keystream (key seed = random 128-byte)
# layer4: hex-encode 後 4 bytes 一組 RSA-encrypt, chain XOR, base64
```

### 逆序拆

#### Layer 4 - RSA 的致命弱點

`e = number.getPrime(24)` 24-bit prime, `n` 2048-bit, **plaintext 每 block 只有 4 bytes** = 32-bit = 範圍 `0x30303030-0x66666666` (hex chars).

Plaintext space 只有 **16^4 = 65536** 種可能. 直接**預計算 lookup table**:

```python
lut = {}
for combo in itertools.product(b'0123456789abcdef', repeat=4):
    pt = bytes(combo)
    v = int.from_bytes(pt, 'big')
    lut[pow(v, e, n)] = pt
```

Chain XOR 反轉: `block_i = chunk_i XOR chunk_{i-1}`, 第一個 chunk 是 IV. 每個 block 查 LUT 得 4-byte hex chars. 全部串起來 unhex 得 layer3 output.

#### Layer 3 - LCG 的 256 candidate

```python
key = (key * 0xc8763 + 9487) % 2**64
output[i] = plaintext[i] ^ (key & 0xff)
```

Key seed 是 128-byte 隨機數 (unknown), 但 `(key_i mod 256)` 完全只依賴 `(key_1 mod 256)` → 只有 **256 個 keystream candidate**.

對每個 candidate 試 XOR, 檢查 output 是否全部落在 **layer2 的合法 byte set** (layer2 input 是 26 大寫 + ' .{}\n' = 31 chars, 分別 * 17 mod 251 得 31 個合法 output).

#### Layer 2 - 乘法逆元

17 * 192 ≡ 1 (mod 251), 所以 inv = 192. 每個 byte * 192 mod 251 還原.

#### Layer 1 - 已知 plaintext 校驗 + 猜語句

layer1 output 是 `FLAG{...}\n` 形式, 某 substitution 作用在大寫字母上. 透過 SHA256 比對 (題目有附 `flag` 的 sha256), 加 **大寫字母 substitution 用英文語句還原**:

- 題目英文語境 + 字長樣式: `1,5,5,3,5,4,3,4,3,2,3,6,3,7,4` → 猜 **「A QUICK BROWN FOX JUMPS OVER THE LAZY DOG...」** 的 pangram
- 補 `IN THE CLOUDY DAY WITHOUT RAIN` 剛好對上, 15 words / 73 chars

對 SHA256 驗證通過 → `FLAG{A QUICK BROWN FOX JUMPS OVER THE LAZY DOG IN THE CLOUDY DAY WITHOUT RAIN.}\n` ✓

## 解法 (核心)

```python
# layer4 reversal
lut = { pow(int.from_bytes(bytes(c), 'big'), e, n): bytes(c) 
        for c in itertools.product(b'0123456789abcdef', repeat=4) }
blocks = [bytes(a^b for a,b in zip(chunks[i], chunks[i-1])) 
          for i in range(1, len(chunks))]
hex_data = b''.join(lut[int.from_bytes(blk, 'big')] for blk in blocks)
l3 = binascii.unhexlify(hex_data)

# layer3 reversal
valid = set((c*17) % 251 for c in b'ABCDEFGHIJKLMNOPQRSTUVWXYZ .{}\n')
for seed in range(256):
    key = seed; out = []
    for cb in l3:
        key = (key * 0xc8763 + 9487) % (1<<64)
        p = cb ^ (key & 0xff)
        if p not in valid: break
        out.append(p)
    else:
        # layer2 reversal
        inv17 = pow(17, -1, 251)
        l1 = bytes((b * inv17) % 251 for b in out)
        print(l1)  # substitution cipher output
        break

# layer1: pangram guess + SHA256 verify
```

## Flag

```
FLAG{A QUICK BROWN FOX JUMPS OVER THE LAZY DOG IN THE CLOUDY DAY WITHOUT RAIN.}
```

## 感想

4 層看似複雜, 每一層都有單獨的弱點: RSA 32-bit plaintext (brute), LCG 8-bit state (256), 乘法 mod 251 (inverse), 字母 substitution (pangram pattern 匹配). 把弱點排好一層層吃就 OK.

學到: **substitution cipher 遇到名言 / pangram 直接比對字長 pattern** 是最快的攻擊面, 比盲 SA 快很多.
