# PHM #55 Reversing — esrever-mv

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 240 |
| Flag | `FLAG{F1r5t_t1m3_t0_r3v3r53_a_VM_1s_FuN__jZJYJd3yY5hDMl7q}` |

## 題目

> VM is fun
>
> Binary: 849KB **static** ELF 64-bit stripped. 題名「esrever-mv」是「vm-reverse」反過來.

## 關鍵觀察 — Key Finding

程式用 `read(0, buf, 1)` **一個 byte 一個 byte** 讀 stdin, 讀對一個馬上去讀下一個, 讀錯直接印 "Bad flag" 退出. 用 strace 就能看到 `read(0, ...)` 的次數變化.

```
echo A      → 1 次 read (第 0 byte 'A' 錯)
echo F      → 2 次 read (第 0 byte 'F' 對, 第 1 byte EOF 錯)
echo FLAG{  → 6 次 read (前 5 byte 全對)
```

**這就是 oracle**: 固定前綴, 掃第 N byte 的 256 個值, read 次數變成 N+2 的就是正解. 寫 Python 包 strace, 每 byte 試 ~95 個 printable ASCII, 10 秒跑完一位, 總共 57 位 flag 不到 1 分鐘.

> [!NOTE]
> 這題是 side-channel 救命 — binary 內部是個完整 VM (`.text` ~ 640KB 幾乎都 static libc + VM dispatch), 要正面硬 reverse VM opcode handler 很累. 程式一次只檢查一個 byte 的設計把難度直接送到 easy.

## 解法 — Exploit

```python
import subprocess

def count_reads(input_bytes):
    p = subprocess.Popen(
        ['strace','-e','trace=read','./esrever-mv'],
        stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    _, err = p.communicate(input=input_bytes, timeout=3)
    return err.count(b'read(0,')

flag = b''
for pos in range(100):
    baseline = len(flag) + 1
    for c in list(range(0x20, 0x7f)) + list(range(0x20)) + list(range(0x7f, 0x100)):
        n = count_reads(flag + bytes([c]) + b'A' * 60)
        if n > baseline:
            flag += bytes([c])
            print(f'pos {pos}: {chr(c)} ({c:#x}), reads={n}')
            break
    else:
        print(f'done: {flag}')
        break
    if c == ord('}'): break
print(f'FLAG: {flag.decode()}')
```

跑完:

```
FLAG{F1r5t_t1m3_t0_r3v3r53_a_VM_1s_FuN__jZJYJd3yY5hDMl7q}
```

本地驗證 `echo 'FLAG{...}' | ./esrever-mv` → `Good flag` ✓.

## Sources

- [PHM scoreboard — esrever-mv](https://ctf.hackme.quest/scoreboard/?id=55)
- `/static/esrever-mv.xz`
