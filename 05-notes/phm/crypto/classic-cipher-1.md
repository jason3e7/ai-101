# PHM #85 Crypto — classic cipher 1

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 50 |

## 題目

> Solve this substitution cipher
>
> `MTHJ{CWTNXRJCUBCGXGUGXWREXIPOYAOEYFIGXWRXCHTKHFCOHCFDUCGTXZOHIXOEOWMEHZO}`

## 分析

### 錨點: `MTHJ` → `FLAG`

Flag 格式一定是 `FLAG{...}`, 所以前綴 `MTHJ` 直接吃出 4 組 cipher→plain:

| cipher | plain |
|:---:|:---:|
| M | F |
| T | L |
| H | A |
| J | G |

試過 Caesar / affine 都不線性 (shift 分別 +7, +8, +7, +3, 不一致), 所以是 **任意的 monoalphabetic substitution**, 要靠 frequency + 字詞對照破解.

### 用 quadgram log-likelihood + 模擬退火 (SA)

Monoalphabetic substitution 的標準解法:

1. 用英文 quadgram log-probability 當 fitness
2. SA 隨機 swap 兩個 cipher 字元的 plain 對應, 以 Metropolis criterion 接受
3. 固定 `M→F T→L H→A J→G` 不動
4. 多次 restart 取最高分

Quadgram 檔案抓 [Practical Cryptography](http://practicalcryptography.com/media/cryptanalysis/files/english_quadgrams.txt.zip).

### SA 跑出近似解, 眼睛補尾段

SA 第一輪輸出:

```
XOLJINGXUSXTITUTIONDIPCKBEKDBYPTIONIXALRAYXKAXYWUXTLIMKAPIKDKOFDAMK
```

看到 `USXTITUTION` 跟 `DIPCKB` 很像 `SUBSTITUTION` 跟 `CIPHER`, 猜開頭是 `SOLVING SUBSTITUTION CIPHER`:

- `C→S`, `W→O`, `N→V`, `X→I`, `R→N`, `G→T`, `U→U`, `B→B`
- `E→C`, `I→P`, `P→H`, `O→E`, `Y→R`

套回去中段 `?ECR?PTION` 補出 `DECRYPTION` → `A→D`, `F→Y`.
最後 `?USTLI?EAPIECEOFCA?E` 補出 `JUST LIKE A PIECE OF CAKE` → `D→J`, `Z→K`, `K→W`.

## 解法

```python
cipher = "CWTNXRJCUBCGXGUGXWREXIPOYAOEYFIGXWRXCHTKHFCOHCFDUCGTXZOHIXOEOWMEHZO"
key = {
    'M':'F','T':'L','H':'A','J':'G',
    'C':'S','W':'O','N':'V','X':'I','R':'N','G':'T','U':'U','B':'B',
    'E':'C','I':'P','P':'H','O':'E','Y':'R',
    'A':'D','F':'Y','K':'W','D':'J','Z':'K',
}
print(''.join(key.get(c, '?') for c in cipher))
# -> SOLVINGSUBSTITUTIONCIPHERDECRYPTIONISALWAYSEASYJUSTLIKEAPIECEOFCAKE
```

## Flag

```
FLAG{SOLVINGSUBSTITUTIONCIPHERDECRYPTIONISALWAYSEASYJUSTLIKEAPIECEOFCAKE}
```

## 感想

4 字錨點 + SA + 眼睛補字, 經典 substitution 流程. SA 第一輪就讓 `SUBSTITUTION / CIPHER` 露頭, 剩下靠英文語感讀出來. 這招對短密文 (50-200 chars) 特別好用.
