# PHM #57 Reversing — rc87cipher (未解, 已深入分析)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 500 |
| 狀態 | 卡在**客製化 PRGA 需深度反組譯** + 不知道 40-char 密碼 |

## 題目

> Tips: password length is 40
>
> `rc87.txz` 解壓 → `encryptor/` 含三個檔:
> - `rc87` (326448 bytes): 加密器, static ELF 64-bit, no section headers
> - `rc87.enc` (326456 bytes): encryptor 用自己加密自己 (**known plaintext!**)
> - `flag.enc` (1282 bytes): 要解的目標

PHM 最高分題 (500), 全站很少人解.

## 密碼器行為 — Cipher Observations

### 介面

```bash
./rc87 enc <password> <input> <output>
```

- 從 `/dev/urandom` 讀 **8 bytes IV**
- `output = IV(8 bytes) + ciphertext (same len as input)`
- 加密會**印出 S-box** (16×16 hex 格): `Here is your sbox:\n...`
- `dec` 選項 → 直接印 "Too lazy to implement" (作者放棄實作)
- (password, IV) → ciphertext 是 deterministic

### 關鍵確認

| 問題 | 答案 |
|:---|:---|
| 同 password 同 IV 兩次 enc → 一樣? | ✓ |
| 同 password 不同 IV → S-box 一樣? | ✗ (IV 混進 state) |
| 標準 RC4 KSA? | ✗ (binary 印的 S-box 跟 `KSA(pw+IV)` 全不符) |
| 標準 RC4 PRGA? | ✗ (給它正確 S-box 跑 PRGA, keystream 也不符) |

結論: **KSA 跟 PRGA 都是客製化的**, 不是純 RC4.

## 可用 primitives

### IV 控制: gdb catchpoint + 直接改 buffer

Binary static 無 dynamic linker, LD_PRELOAD 失效. 改 binary 字串 `/dev/urandom` → `/tmp/iv_src` 可以替代檔案, 但會把其他字串的 format 弄壞 (附近 bytes 被用來解其他字串). 乾淨方法:

```gdb
# /tmp/phm-rev57/gdb_cmd
set pagination off
set confirm off
catch syscall read
commands
silent
if ($rax == 8)
  set *(unsigned long long*)$rsi = 0xb48514a5483d85e3  # 想要的 IV, little-endian
  delete 1
  continue
end
continue
end
run
```

```bash
timeout 60 gdb -q -batch -x gdb_cmd --args ./rc87 enc TESTPW rc87 /tmp/out.enc
```

每次 run ~1 秒 (gdb overhead), 可以當 encryption oracle.

### Known plaintext → keystream

```python
rc87 = open('encryptor/rc87','rb').read()
rc87_enc = open('encryptor/rc87.enc','rb').read()
ks_rc87 = bytes(a ^ b for a, b in zip(rc87_enc[8:], rc87))
# 326448 bytes of keystream, 用 (unknown_pw, rc87.enc 的 IV = 7fcb78345591f7e7) 產
```

## 卡住的點

1. **要恢復 flag**: 需要 keystream for `(unknown_pw, flag.enc 的 IV = e3853d48a51485b4)`
2. **轉換不過去**: IV 影響最終 S-box, 無法把 rc87.enc 的 keystream 直接套到 flag.enc
3. **純反組譯 PRGA 很累**: binary 無 section headers, 要靠 capstone + 自己 mapping
4. **密碼反推**: 40 字元, 字典打過幾個常見組合 (`A*40`, `0123...`, `sha1(各檔)`, `please_hack_me*n`...) 全不中

## 可能攻擊路線 — Attack Directions

1. **純反組譯 PRGA + KSA**: 用 gdb 在 S-box 讀寫位置下 watchpoint, 看每 iteration 的操作. 寫 Python emulator. 之後把 keystream 反推 S-box 初始 state, 再反推 (pw, IV).
2. **RC4 state recovery from 326KB keystream**: 經典 RC4 攻擊 (Knudsen 等) 用於這個客製變體, 困難度 unknown.
3. **Password 字典攻擊**: gdb 固定 IV = rc87.enc 的 IV, encrypt rc87 用候選 password, 比對 rc87.enc. 每次 ~1 sec. rockyou 15M 行跑不完, 要有針對性 wordlist.
4. **PHM 作者 OSINT**: 看作者其他題有沒有愛用 password / 字串 pattern.

## 試過但沒 work 的路

- LD_PRELOAD 攔 `/dev/urandom` → static ELF 失效
- bwrap `--dev-bind` → 本機沒 user namespace 權限
- SHA-1(rc87), SHA-1(rc87.enc), SHA-1(flag.enc), SHA-1 of common strings
- 常見 40-char pattern: `A*40`, `0*40`, `0123456789*4`, `ABC...*n`, PHM 的 hack me 相關字
- 標準 RC4 KSA(pw+IV), KSA(IV+pw), KSA(pw*256)

## Flag

未取得.

## Sources

- [PHM scoreboard — rc87cipher](https://ctf.hackme.quest/scoreboard/?id=57)
- `/static/rc87.txz`
