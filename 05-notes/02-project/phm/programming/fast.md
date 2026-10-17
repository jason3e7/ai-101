# PHM #98 Programming — fast

| 項目 | 值 |
|:---|:---|
| 類別 | Programming |
| 分數 | 40 |

## 題目

> How fast could you be?
>
> `nc ctf.hackme.quest 7707`

連進去 server 吐:

```
You have to be fast!
Calculation 10000 times simple math expression and I will give you flag~
Send 'Yes I know' to start the game.
```

送 `Yes I know\n`, 之後 server 連丟 **10000 條** 四則運算:

```
2011052585 - -37814276 = ?
-4991610 + 2146938710 = ?
-134875690 * -589968548 = ?
-771228 / -1075951039 = ?
```

每條要回正確答案. 這題考**用程式自動化**, 不是 nc 手刷能贏的.

## 分析

### 兩個要注意的點

1. **整數語意**: 數字落在 32-bit int 範圍 (±2^31). 乘法會爆 32-bit, 要**模 2^32 + 轉有號**才對得上 server 的 C 行為.
2. **除法方向**: C 的 `/` 是**往 0 截斷** (`-7 / 2 = -3`), Python 的 `//` 是**往下取整** (`-7 // 2 = -4`). 兩者在負數會差 1, 要自己實作.

### Python 用 socket + regex 解析

`subprocess.popen(nc)` 不好用 (buffer 問題). 直接 `socket.create_connection()` 收 TCP line 最乾淨.

## 解法

```python
import socket, re, time

HOST, PORT = "ctf.hackme.quest", 7707

def to_i32(x):
    x &= 0xFFFFFFFF
    return x - 0x100000000 if x >= 0x80000000 else x

def ctrunc_div(a, b):
    q = abs(a) // abs(b)
    return -q if (a < 0) ^ (b < 0) else q

OPS = {
    '+': lambda a,b: to_i32(a + b),
    '-': lambda a,b: to_i32(a - b),
    '*': lambda a,b: to_i32(a * b),
    '/': lambda a,b: to_i32(ctrunc_div(a, b)),
}

s = socket.create_connection((HOST, PORT), timeout=30)
buf = b""
def recv_line():
    global buf
    while b"\n" not in buf:
        data = s.recv(65536)
        if not data: return None
        buf += data
    line, _, buf = buf.partition(b"\n")
    return line.decode(errors='replace')

# 吃掉 banner
while True:
    line = recv_line()
    if "Yes I know" in line: break
s.sendall(b"Yes I know\n")

pat = re.compile(r'(-?\d+)\s*([+\-*/])\s*(-?\d+)\s*=\s*\?')
start = time.time()
for i in range(10000):
    line = recv_line()
    m = pat.search(line)
    a, op, b = int(m.group(1)), m.group(2), int(m.group(3))
    s.sendall(f"{OPS[op](a, b)}\n".encode())

# 讀 flag
while True:
    line = recv_line()
    if not line: break
    print(line)
    if "FLAG" in line: break
```

跑 1.3 秒, 跟 server tcp latency 差不多.

## Flag

```
FLAG{Wow, you are really fast! SfpNi7yYEP0BDXDN}
```

## 感想

Programming 類最直覺的題: 題目字面, 寫 script 就贏. 兩個小坑 (**C-style int / signed div truncation**) 是常見踩雷. 一般人用 Python `eval()` 會死在負數除法上.

這題也示範了「**socket 比 nc 可靠**」原則 — nc pipe 給 subprocess 會因 buffer / blocking 問題亂掉, 直接 `socket.create_connection` 讀 line 乾淨.
