# PHM #91 Crypto — passcode

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 70 |

## 題目

> Can you break AES?
>
> `nc ctf.hackme.quest 7700`
>
> 檔案: [`/static/ais3passcode.py`](https://ctf.hackme.quest/static/ais3passcode.py)

Source:

```python
key = "XXXXXXXXXXXXXXXX"
iv = ''.join(random.choice(string.hexdigits) for _ in range(16))

def encrypt(p):
    return AES.new(key, AES.MODE_OFB, iv).encrypt(p)

print encrypt(flag).encode("hex")        # 開場: flag 用這個 keystream 加密
while True:
    raw_input()
    p = ''.join(random.choice(string.lowercase) for _ in range(32))
    print encrypt(p).encode("hex")        # 每按 enter: 32 隨機小寫字加密
```

## 分析

### 關鍵: AES-OFB 的 keystream 跟 plaintext 無關

- 同一條連線裡 `key` 跟 `iv` **都固定**
- 每次 `AES.new(key, MODE_OFB, iv)` 的 keystream 都一樣

Cipher = plaintext XOR keystream. 多樣本彼此的 keystream 相同, 只有 plaintext 不同.

### 不知道 plaintext, 怎麼反推 keystream?

random 的 32 bytes **全是小寫** (`[a-z]` = `0x61-0x7a`). 對每個 byte 位置 i, keystream[i] 滿足:

```
∀ sample j: 0x61 <= cipher_j[i] ^ keystream[i] <= 0x7a
```

每個 random sample 排除 ~230 個 candidate (剩 ~26). 50 個 sample 之後幾乎每個 position 只剩 1 個 valid candidate.

### 剩下的 ambiguous position 用語義補

跑完 50 sample 還剩 5 個 position 有 2-3 個 candidate. 看出來 flag 大概是:

```
FLAG{You?_?la??for_breaking?OFB}
       8 10 13-14              27
```

英文直讀 → `FLAG{Your_flag_for_breaking_OFB}`.

## 解法

```python
import socket, re
s = socket.create_connection(("ctf.hackme.quest", 7700))
# 收 banner, 抓 flag_cipher
...
# 送 50 次 enter, 收 50 個 32-byte random cipher
...
# 每 position 窮舉 256 candidate, 挑能把所有樣本都解成小寫的
for i in range(32):
    valid = [c for c in range(256) if all(0x61 <= b[i]^c <= 0x7a for b in samples)]
# 解 flag_cipher XOR keystream
```

## Flag

```
FLAG{Your_flag_for_breaking_OFB}
```

## 感想

Stream cipher nonce reuse 經典. 不需要破 AES, 只要**同 keystream 加密多個訊息** = 可以互相用來 crib. 這題只給 cipher 不給 plaintext, 但靠 "plaintext 是全小寫字母" 這個限制也能收斂到唯一解.
