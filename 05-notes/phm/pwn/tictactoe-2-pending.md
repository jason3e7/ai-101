# PHM #79 Pwn — tictactoe-2 (未解, 已深入分析)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 250 |
| 保護 | Non-PIE 32-bit / Canary ON / NX on / Partial RELRO (GOT 可寫) |
| 狀態 | OOB write primitive 已確認, 卡在**無 libc leak + 無 system/binsh** 的 1-byte write 升級 |

## 題目

> `nc ctf.hackme.quest 7714`, "Get a shell pls"
>
> 跟 [#78 tictactoe-1](tictactoe-1.md) 同 binary (`/static/tictactoe.xz`), 需要**開 shell** 讀第二個 flag.

## OOB write primitive (繼承 #78)

Move handler `0x8048a4b`:

```c
int move = atoi(input);
if (move == 9) {
    read(0, &flavor, 4);
    board_char = flavor[0];      // 控 player char
    recursive_move_handler();
}
// Write first, validate later!
*(char*)(0x804b056 + move) = board_char;   // ← 1-byte OOB (signed move)
if (is_valid(move)) taken_map[move] = 0xff;
```

兩個都可控:
- **target address**: `0x804b056 + signed_move` (負數可以寫到 GOT / .got.plt)
- **byte value**: 透過 `move=9` 送 4 bytes, byte 0 變新 `board_char`

### 可達 GOT entries

| Symbol | Addr | Offset from 0x804b056 |
|:---|:---|---:|
| GOT[setbuf] | 0x804b00c | -74 |
| GOT[read] | 0x804b010 | -70 |
| GOT[printf] | 0x804b014 | -66 |
| GOT[fclose] | 0x804b018 | -62 |
| GOT[__stack_chk_fail] | 0x804b01c | -58 |
| GOT[fwrite] | 0x804b020 | -54 |
| GOT[puts] | 0x804b024 | -50 |
| GOT[exit] | 0x804b028 | -46 |
| GOT[open] | 0x804b02c | -42 |
| GOT[memset] | 0x804b034 | -34 |
| GOT[putchar] | 0x804b038 | -30 |
| GOT[atoi] | 0x804b03c | -26 |

**沒有 `system` 被 import!** 要 shell 必須靠 libc redirect.

## 卡住的點

1. **無 libc leak primitive**: binary 的 printf 全是 fixed 字串, 沒有 user-controlled format. OOB **write** 不給 read. Game 的 board print 用 `%c` 不是 `%s`.
2. **無 `/bin/sh` 字串** 跟 **無 `system` import**: 要走 execve 的話還要 argv/envp 設 `NULL` ptr, 單純 1-byte write 做不到.
3. **1-byte-at-a-time 的 partial GOT overwrite**:
   - libc 2.23 i386: `system @ 0x3AD80`, `execve @ 0xB0700`, `puts @ 0x5FB80`
   - puts → system / execve 不同的 top 2 bytes, 2-byte overwrite 跨不過去
   - 3-byte overwrite 可以, 但要知道 libc_base 的 byte 2 (有 carry 問題), 沒 leak 做不到

## 試過但走不下去的路

- Partial GOT overwrite GOT[atoi] / GOT[puts] 的 low 2 bytes → 停在同 64KB 內, 跳不到 system
- 覆蓋 `.rodata` 的 `"flag_simple"` 讓 `open` 讀別的檔 → .rodata 是 R-only, 寫會 SEGV
- 用 OOB 寫 saved return 做 ROP → canary ON + 沒 stack leak
- 用 OOB 寫 `GOT[open] → execve` 讓 `open("flag_simple")` 變 `execve("flag_simple")` → 要 argv/envp = NULL + flag_simple 不是 valid ELF

## 下一步 — Resume

- **Brute force 3-byte partial overwrite**: 每次連線 ASLR 不同, 猜 byte 2. 256 次試 1 次中機率不錯, 但要寫自動化腳本
- **FSOP**: 蓋 `stderr->_vtable` (COPY reloc 在 .bss `0x804b060`) → file 操作時 call vtable 指標. 蓋成 house of orange 類攻擊. 複雜但有希望
- **ret2csu / main loop**: 蓋 `GOT[exit]` = main 讓 game 重啟, 累積多次 OOB write 把 .bss 填滿 shellcode, 然後 mprotect 自己... 需要先解 libc leak
- 等真的解完的參考 writeup 看別人怎麼做

## Flag

未取得. OOB write primitive 跟 #78 一樣穩, 但 1-byte write 在這個保護組合下很難開 shell.

## Sources

- [PHM scoreboard — tictactoe-2](https://ctf.hackme.quest/scoreboard/?id=79)
- `/static/tictactoe.xz` (跟 #78 同 binary)
- [#78 tictactoe-1](tictactoe-1.md) — 同 OOB write primitive 的 easy 版
