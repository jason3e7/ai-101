# PHM #86 Crypto — classic cipher 2

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 50 |

## 題目

> Solve this vigenere cipher
>
> 檔案: [`/static/classic-cipher-2.txt`](https://ctf.hackme.quest/static/classic-cipher-2.txt) (3135 bytes)

## 分析

### 判斷 Vigenère + 找 key 長度

標題就講了 Vigenère. 先用兩招交叉驗證 key 長度:

- **Index of Coincidence (IoC)**: 把密文每隔 `k` 個字元切成 `k` 條 column, 計算各 column 的 IoC. 單純英文會接近 0.067, 隨機接近 0.038. 找 IoC 平均最高的 `k`.
- **Kasiski examination**: 找重複 3-gram 的距離, 距離的 GCD 就是 key 長度的倍數.

兩招都指向 `key_len = 51`.

### 每個 column 單獨破 Caesar

拆成 51 條 column 後, 每條都是 Caesar shift. 對每個 column 用 **chi-squared** 比對英文字母頻率 (`e=0.127, t=0.091, a=0.082, ...`), 最低分的 shift 就是該 column 的 key 字元.

跑完得到 key:

```
VIGENERECIPHERCANBECRACKEDBYFREQUENCYANALYSISATTACK
```

長度 51. 這其實就是「VIGENERE CIPHER CAN BE CRACKED BY FREQUENCY ANALYSIS ATTACK」拿掉空格.

### 解密後找 flag

套這把 key 解完, 密文解出一段 Caesar salad (維基沙拉) 文字裡藏兩段大寫句子:

```
THE FLAG HAS NINE WORDS AND YOU NEED TO ADD CURLY BRACES TO FLAG
THE FLAG IS VIGENERE CIPHER CAN BE CRACKED BY FREQUENCY ANALYSIS
```

第二句少了 ATTACK 就是 8 個 word, 加上 ATTACK 剛好 9 個. 加 curly braces 就是 flag.

## 解法

```python
with open('classic-cipher-2.txt') as f:
    cipher = f.read()

key = "VIGENERECIPHERCANBECRACKEDBYFREQUENCYANALYSISATTACK"

# 只對 A-Z 做位移, 其他原樣保留
plain = []
k_idx = 0
for c in cipher:
    if c.isalpha():
        shift = ord(key[k_idx % len(key)]) - ord('A')
        base = ord('A') if c.isupper() else ord('a')
        plain.append(chr((ord(c) - base - shift) % 26 + base))
        k_idx += 1
    else:
        plain.append(c)

print(''.join(plain))
```

## Flag

```
FLAG{VIGENERE CIPHER CAN BE CRACKED BY FREQUENCY ANALYSIS ATTACK}
```

## 感想

教科書等級 Vigenère: IoC/Kasiski 找長度 → chi-squared 分欄解 Caesar. Key 本身就是解法, 作者藏得很美.
