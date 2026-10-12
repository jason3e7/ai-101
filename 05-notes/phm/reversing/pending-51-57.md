# PHM Reversing #51–#57 · 卡住未解

| 題號 | 題名 | 分 | 狀態 |
|---:|:---|---:|:---|
| 51 | unpackme | 200 | packed PE + Enigma Protector, 商業套殼 |
| 52 | mov | 200 | M/o/Vfuscator 編譯, 只有 MOV 指令, 5.9MB binary |
| 53 | sha256sum | 100 | UPX + Enigma 雙重套殼, 只 4 人解, 吃 preimage / hash collision |
| 54 | a-maze | 200 | 2MB map file + VM runner, 迷宮搜尋 |
| 55 | esrever-mv | 240 | VM-based, static ELF 849KB |
| 56 | termvis | 230 | 終端畫 PNG 的工具, 需要 reverse 畫圖邏輯 |
| 57 | rc87cipher | 500 | password length 40, 自製 cipher |

這 7 題都是 PHM reversing 的硬題 (平均 15-50 人解, 相對 #42-50 的 50-500 人), 都需要**特定 VM / 商業套殼 / 自製加密**的深度 reverse. 一天內做不完, 先存著.

## 可能方向 (未試)

### #51 unpackme (modified UPX, 不是 Enigma)

**實際不是 Enigma, 是改名過的 UPX**. Section 名稱被改成 `CTF0`/`CTF1`/`CTF2` (正常是 `UPX0`/`UPX1`), 另外 `UPX!` 魔術字跟 L_INFO 結構也被改掉, 導致 UPX 原生 `-d` 拒絕解包.

**已做到的進度**:

1. **識別**: PE32 32-bit, 3 sections, imports `CryptHashData`+`VirtualProtect`+`GetProcAddress`+`LoadLibraryA` → 明顯 UPX 樣式 + 自解包 unpacker
2. **unicorn 跑 unpacker**: 寫了 Python+Unicorn script, 從 entry point `0x417830` emulate unpacker stub, 自動 map 缺的頁, dump CTF0 region
3. **找到 password check**:
   - CryptCreateHash with `CALG_MD5 (0x8003)` → MD5 hash
   - CryptHashData 吃 user 輸入
   - 跟一個 16-byte 常數比對 (`34af0d074b17f44d1bb939765b02776f`)
   - match → 印 "Flag is <input>" (user 輸入**就是 flag**)
   - mismatch → 印 "Wrong answer"
4. **找到 troll flag**: 反轉 `}ysae oot s'ti ,galf ruoy ton si siht oN{GALF` = `FLAG{No this is not your flag, it's too easy}` (這是 "Flag is NOT" 分支印的假 flag)

**卡住**: MD5 preimage of `34af0d074b17f44d1bb939765b02776f`
- 不在 rockyou (14M)
- 試遍 ~500 CTF-context 字串 (unpackme / UPX / HACKMECTF / FLAG{...} 格式等)
- hashes.com / crackstation / md5decrypt 都沒結果 (hash DB 查不到)

**可能方向**:
- 密碼可能是**非英文字母**組合, 或**特定 CTF 社群梗**
- 完成 unpacker emulation 直到 OEP, 看最終 unpacked `.text` 是否有更多 hint
- 用 hashcat + 大字典 + rule

### #52 mov (M/o/Vfuscator)
- The movfuscator 編譯的 code 可以被 **demovfuscator** 工具自動還原 (Chris Domas)
- 嘗試 demovfuscator / r2 的 mov deobfuscate

### #53 sha256sum (UPX + Enigma)

**題目**:
```
PS C:\> .\sha256sum.exe sha256sum.exe flag.txt
a000e5abe65015d52555c3aab03debc109a9a04351fda9897f10acc5ffe35956  sha256sum.exe
6128d4a4c7e40af66f0bd3347e4d51673823e18a15f6e5299d00d8c562bf01ae  flag.txt
```
> But where is flag?

**已確認**:
- 真實 `sha256sum /tmp/sha256sum.exe` = `a000e5abe65015d5...956` ✓ **跟題目一致** → exe 真的算 SHA256, 不是騙子
- 所以 `6128d4a4...1ae` 也是 flag.txt 的真實 SHA256 → 想從 hash 反推 = 2^256 空間, 做不到
- Exe sections: UPX0 + UPX1 + UPX2 + **.enigma1 (0x37000 bytes)** + **.enigma2 (0xad000 bytes)** → UPX + Enigma Protector x64 雙重套殼
- UPX 的 `-d` 回 "not packed by UPX" (Enigma 包在外面)
- Enigma 需要商業 unpacker (OllyDbg + Enigma plugin) 才解得開
- Wine 跑時 Enigma 的 anti-debug stack overflow 直接崩掉 (`virtual_setup_exception stack overflow 2432 bytes`)

**嘗試過的 shortcut (全失敗)**:
- `/static/flag` 回 `WRONG_ANSWER{This is not flag}` troll
- 猜 `FLAG{<hash>}`, `FLAG{But where is flag?}`, `FLAG{sha256sum}` 之類 → "Not a flag"
- 搜 exe binary 有無 ASCII/UTF-16 的 FLAG{ 或 flag — 都被 Enigma 加密了, 零命中
- 試 SHA256 preimage of `6128d4a4...` 用常見 CTF 字串 → 全失敗

**真正解法 (推測)**:
- 要**Enigma unpack** 拿到 .text 原始碼, 看 exe 是否有**除了 sha256 之外的隱藏功能** (例如對特定 argv 噴 flag)
- 或是 flag.txt 真的在**作者機器上的檔案系統**, 唯一拿到的途徑是 preimage attack (做不到)
- 全站只 4 人解, 需要特定 Enigma Protector 解包工具, 不是 blackbox 推得出來的題

### #54 a-maze
- 讀懂 maze binary 的 input format, 找出 2MB map 的格式
- 可能是 BFS/DFS 找出可達路徑, path 編碼 flag

### #55 esrever-mv (VM)
- VM 題標準流程: dump bytecode, 分析 opcode handler table, 寫 disassembler
- static ELF 省很多力 (不用解 dynamic linker)

### #56 termvis
- 讀懂終端畫 PNG 的程式, 找出 flag.png 的位置
- 可能用 ANSI escape 畫圖, 需要讀出 escape sequence 還原 flag

### #57 rc87cipher
- Password 40 chars → 窮舉空間太大, 肯定有結構 (RC4-like with weakness?)
- 看 rc87 encryptor + flag.enc + rc87.enc (RC87 作者自我加密), 可能是 known plaintext attack
