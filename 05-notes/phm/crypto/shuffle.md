# PHM #89 Crypto — shuffle

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 70 |

## 題目

壓縮包有三個檔案:

```
crypt.py      # 加密 + shuffle 腳本
crypted.txt   # 原文做 substitution 的結果
plain.txt     # 原文「打散」後的結果 (非原始 plaintext!)
```

`crypt.py`:

```python
import random, string
characters = ''.join(map(chr, range(0x20, 0x7f)))

plaintext = open('plain.txt').read()  # 這時候還是原文

mapping = list(characters)
random.shuffle(mapping)
T = str.maketrans(characters, ''.join(mapping))
open('crypted.txt', 'w').write(plaintext.translate(T))

# 再把原文 shuffle (打亂字元順序) 覆蓋回 plain.txt
plain = list(plaintext); random.shuffle(plain)
open('plain.txt', 'w').write(''.join(plain))
```

## 分析

### 關鍵觀察: 字元頻率守恆

- `crypted.txt` 是對原文做 **1-to-1 substitution**, 每個字元的 count 跟原文一樣.
- `plain.txt` 是原文的 **permutation**, 每個字元的 count 也跟原文一樣.

所以 **plain 跟 crypted 的字元 count 完全相同**! 只要 count 是 unique 的字元, 就能直接 1-to-1 配對.

### 用 count 分組建 mapping

```python
from collections import Counter, defaultdict
plain = open('plain.txt').read()
crypted = open('crypted.txt').read()

pg, cg = defaultdict(set), defaultdict(set)
for ch, n in Counter(plain).items():   pg[n].add(ch)
for ch, n in Counter(crypted).items(): cg[n].add(ch)

key = {}  # cipher -> plain
for n, pset in pg.items():
    cset = cg.get(n, set())
    if len(pset) == 1 and len(cset) == 1:
        key[next(iter(cset))] = next(iter(pset))
```

跑完有 63/80 字元唯一對應. 剩下 ambiguous:

```
count 1: {/, Z, {, }} <-> {#, B, K, q}
count 27, 30, 33, 48, 51, 308: 多個互有歧義的字元
```

### 用「稀有字元 + 上下文」補完 flag region

Flag 一定長 `FLAG{...}`, 裡面的 `{` `}` 是 count-1 字元. 把 count-1 的 cipher char (`#, B, K, q`) 的位置拉出來看上下文:

| cipher | pos | 周圍 (已用 unique mapping 解) |
|:---:|---:|:---|
| `#` | 434 | `...Let it go! Let it go!\nFLAG?C01d n3v3r...` |
| `K` | 464 | `...FLAG?C01d n3v3r b0th3r3d m3 @nyw@y?\nTurn...` |
| `q` | 4307 | `...live action?animation biography...` |
| `B` | 7644 | `...Dick ?ondag and Dave Goetz...` |

→ `#=`{`, `K=`}`, `q=/`, `B=Z`. 全對上.

### 唸出原文

套這些 mapping 後整段讀出來: 迪士尼《Frozen》Let It Go 的歌詞. 中間那一行就是 flag.

```
Let it go! Let it go!
FLAG{C01d n3v3r b0th3r3d m3 @nyw@y}
Turn away and slam the door
```

(是「Cold never bothered me anyway」的 1337 寫法, 正好是 Let It Go 最後一句.)

## Flag

```
FLAG{C01d n3v3r b0th3r3d m3 @nyw@y}
```

## 感想

這題不是暴力解 substitution, 題目直接送了 **character count 對照表** (就是打散過的 `plain.txt`). 一旦看懂 shuffle 其實是「保留 Counter」, 整個就塌到頻率對照. 剩下 1-count 字元剛好對應 flag 的括號跟 unique chars, 設計得頗巧.
