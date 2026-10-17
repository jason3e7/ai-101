# PHM #96 Crypto — slowcipher · 卡住未解

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 200 |

## 題目

> Become burst linker and decrypt the flag faster
>
> 檔案: [`/static/slowcipher.7z`](https://ctf.hackme.quest/static/slowcipher.7z)

解開有 `slowcipher` (x86-64 ELF) + `flag.enc` (324 bytes).

## 現況: 寫了 fast decryptor, 密碼沒猜中

### Binary 逆向

`slowcipher mode password input output`. mode 第一個字 & 0xDF = 'D' 走 decrypt, 否則 encrypt.

Cipher state:
- `rbx` 63-bit LCG state (`rbx = (rbx * 0x777777 + 0x3039) & 0x7FFFFFFFFFFFFFFF`)
- `rbp` 用 feedback 更新

Password mix (**關鍵「slow」處**): 對每 password byte `c`:
1. 做一次 hash update (mult + shift + xor)
2. **forward LCG `c` 次** (ASCII 100+ 就是 100+ 次 modular mult)
3. **再 forward 66 次** (unconditional)
4. 處理到 `\0` 結尾也要跑一次 (含 66 次 LCG)

Per-byte decrypt:
1. forward `rbp` 次 (feedback 讓 rbp 慢慢長到 ~2^N)
2. `plain = cipher XOR (rbx & 0xff)`
3. `rbp = plain XOR (21 * rbp / 10)`

324 bytes * log 次 forward = 設計時故意慢.

### Fast decryptor — Burst Linker 的意圖

Challenge 的 hint「burst linker」就是**把 N 次 LCG 用 closed-form 一步算完**:

```
LCG: x -> a*x + c (mod 2^63)
N steps: x -> a^N * x + c * S_N (mod 2^63)
  where S_N = 1 + a + a^2 + ... + a^{N-1}

Fast via (a, S) composition:
  compose((a1, S1), (a2, S2)) = (a1*a2, S2 + a2*S1)
Then exponent-by-squaring: O(log N).
```

寫成 C 之後, 每 byte 的處理從幾千次 mult 掉到 ~63 次, 整個 decrypt 從幾秒掉到幾 ms. Brute force 速度:

| wordlist | 時間 |
|:---|---:|
| rockyou 1M | 1.1s |
| rockyou 14M | 5s |
| a-z 1-5 char | 10s |
| a-z 6 char (26^6=308M) | ~110s |

Fast decryptor 檔案: `/tmp/phm/crypto93/slow/fast.c` + `brute.c` (已 compile, log-N forward 實作).

### 試過的 password 都 miss

Known plaintext: flag 開頭 `FLAG{` 用 5 byte check filter.

- rockyou 1M, 14M → no match
- a-z 1-5 char (62M) → no match
- a-z 6 char (308M) → 進行中, 耗時長
- Anime / Accel World 相關: burstlink, burstlinker, BrainBurst, AccelWorld, SilverCrow, BlackLotus, Kuroyukihime, Haruyuki 等 (50+ 變體)
- CTF 相關: hackme, inndy, PHM, slowcipher
- 大小寫 / 特殊符號 / 數字後綴 混合變體

### 可能方向 (未試)

- `BurstLinker` 的其他文字 (如全片假名 「バーストリンカー」 的 romaji / utf-8)
- Accel World 中的具體台詞或特殊 keyword
- rockyou 以外的 wordlist (SecLists, Hashmob, HIBP top million)
- 檢查 flag.enc 檔案是否有 metadata / header 洩漏 pwd 長度
- 從 ciphertext 反推 pwd 的 structural attack (LCG state recovery)

## 收斂

Fast decrypt 寫完了 (符合「Become burst linker」字面意思), 但 password 猜測是另一個難題. 看起來 wordlist 要更特別, 或是需要從別處拿 hint. 先存著, 之後再回來.
