# PHM #53 Reversing — sha256sum (未解, blackbox 不可能)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 100 |
| 狀態 | 卡住 (需要 Enigma 商業 unpacker) |

## 題目

```
PS C:\> .\sha256sum.exe sha256sum.exe flag.txt
a000e5abe65015d52555c3aab03debc109a9a04351fda9897f10acc5ffe35956  sha256sum.exe
6128d4a4c7e40af66f0bd3347e4d51673823e18a15f6e5299d00d8c562bf01ae  flag.txt
```
> But where is flag?
>
> Binary: [`/static/sha256sum.exe`](https://ctf.hackme.quest/static/sha256sum.exe)

全站只 4 人解.

## 已確認的事實

- 真實 `sha256sum sha256sum.exe` = `a000e5abe65015d5...956` ✓ **跟題目輸出一致** → **exe 真的算 SHA256, 不是騙子**
- 所以 `6128d4a4...1ae` 也是 flag.txt 的**真實 SHA256** → 想從 hash 反推 = 2^256 空間, **做不到**
- Exe sections: `UPX0 + UPX1 + UPX2 + .enigma1 (0x37000B) + .enigma2 (0xad000B)` → **UPX + Enigma Protector x64 雙重套殼**
- Imports: `kernel32`, `oleaut32` (Delphi/BCB), `user32`, `advapi32`, `ole32`, `ntdll` (ZwProtectVirtualMemory), `shlwapi` — Enigma 常見的 import 組合
- UPX 的 `-d` 回 "not packed by UPX" (Enigma 包在外面)
- Enigma 需要**商業 unpacker** (OllyDbg + Enigma plugin, 或 x64dbg + Scylla + 手動找 OEP) 才解得開
- Wine 跑時 Enigma 的 anti-debug stack overflow 直接崩掉 (`virtual_setup_exception stack overflow 2432 bytes addr 0x6fffffca740a`)

## 嘗試過的 shortcut (全失敗)

- `/static/flag` 回 `WRONG_ANSWER{This is not flag}` troll
- 猜 `FLAG{<hash>}`, `FLAG{But where is flag?}`, `FLAG{sha256sum}` 等 → "Not a flag"
- 搜 exe binary 有無 ASCII/UTF-16 的 `FLAG{` 或 `flag` — 零命中 (Enigma 全加密)
- 試 SHA256 preimage of `6128d4a4...` 用常見 CTF 字串 → 全失敗

## 推測正解

- 要**Enigma unpack** 拿到 .text 原始碼, 看 exe 是否有**除了 sha256 之外的隱藏功能** (例如對特定 argv 噴 flag; 或是對自己做某種 hidden check)
- 或是 flag.txt 真的在**作者機器上的檔案系統**, 唯一拿到的途徑是 preimage attack (做不到)
- 全站只 4 人解, 需要特定 Enigma Protector 解包工具, 不是 blackbox 推得出來的題
