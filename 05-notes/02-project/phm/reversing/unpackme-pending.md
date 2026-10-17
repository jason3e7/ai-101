# PHM #51 Reversing — unpackme (未解, 進度 ~70%)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 200 |
| 狀態 | 卡住 (已 unpack, 剩 MD5 preimage) |

## 題目

> Let's learning some Windows stuff! Here's a packed Windows executable binary, try to unpack and reverse it!
>
> Binary: [`/static/unpackme.exe`](https://ctf.hackme.quest/static/unpackme.exe)

## 已確認的事實

**實際不是 Enigma, 是改名過的 UPX**. Section 名稱被改成 `CTF0`/`CTF1`/`CTF2` (正常是 `UPX0`/`UPX1`/`UPX2`), 另外 `UPX!` 魔術字跟 L_INFO 結構也被改掉, 導致 UPX 原生 `-d` 拒絕解包 ("CantUnpackException: file is possibly modified/hacked/protected").

- PE32 32-bit, 3 sections
- Imports: `CryptHashData` + `VirtualProtect` + `GetProcAddress` + `LoadLibraryA` + `ExitProcess` + `GetStockObject` + `GetParent`
- Entry point 0x417830 (在 CTF1 區)

## 做到的進度

1. **unicorn emulator unpack**: 寫 Python+Unicorn script 從 entry point `0x417830` emulate unpacker stub, 自動 map 缺的頁, dump CTF0 region ~33KB
2. **找到 password check 邏輯** (在 unpacked CTF0 的 0x40bc45-0x40bcff):
   ```
   CryptAcquireContext(0, 0, 0, PROV_RSA_FULL=1, 0xf0000000)
   CryptCreateHash(h, CALG_MD5=0x8003, 0, 0, &hash)
   CryptHashData(hash, user_input, len, 0)
   CryptGetHashParam(hash, HP_HASHVAL=2, out_buf, 16, 0)
   compare 16 bytes with 固定常數
   ```
3. **目標 MD5**: `34af0d074b17f44d1bb939765b02776f`
4. **流程**: match → 印 `"Flag is " + user_input` (user 輸入**就是 flag**); mismatch → 印 `"Wrong answer"`
5. **找到 troll flag**: strings 裡反轉的 `}ysae oot s'ti ,galf ruoy ton si siht oN{GALF` = `FLAG{No this is not your flag, it's too easy}` (是 "Flag is NOT" 分支印的假 flag, 誘導攻擊者)

## 卡住點

**MD5 preimage of `34af0d074b17f44d1bb939765b02776f`**:
- 不在 rockyou (14M 全掃無 hit)
- 試遍 ~500 CTF-context 字串 (`unpackme`, `UPX`, `HACKMECTF`, `Richy`, `FLAG{...}` 格式, pi variations 等)
- 線上 hash DB (hashes.com, crackstation, md5decrypt) 查無結果

## 可能方向

- 密碼可能是**非英文字母**組合, 或**特定 CTF 社群梗** (NTUST 課程 trivia?)
- 完成 unpacker emulation 直到 OEP, 看最終 unpacked `.text` 是否有更多 hint (比如 debug print, 註解)
- 用 hashcat + 大字典 (rockyou + mangle rules) 跑長時間
- 用 Windows VM + x32dbg + Scylla 做真實的 unpack 看 memory 內有無 plaintext leak
