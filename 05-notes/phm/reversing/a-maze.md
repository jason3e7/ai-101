# PHM #54 Reversing — a-maze

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 200 |
| Flag | `FLAG{W41k 410n3 1n m4z3 15 d4ng3r0u5, y0u m1gh7 n07 f1nd w4y r3v3r53}` |

## 題目

> Escape from my a-maze
>
> `maze.txz` → 6KB ELF binary + 2MB map file. 用法 `./maze input-map hidden-message`.

## 分析 — Analysis

### Binary 本體就是個 DFA interpreter

反組譯 `./maze`:

```asm
maze_fn(char *input, int state=0):
  rax = state          ; 初始 state = 0
  r8  = &map           ; map 檔整個讀進來的 buffer
loop:
  edx = *input         ; 當前字元
  rax = rax * 512      ; state * 512 (shl rax, 9)
  rax = r8 + rax       ; = &map[state*512]
  rcx = edx & 0x7f     ; char & 0x7F (ASCII)
  rax = map[rax + rcx*4]  ; 讀 32-bit signed, 下一個 state
  if rax == -1: puts("You got it."); exit(0)   ; ← 成功
  if *input == '\0': puts("Too bad, maybe next time!"); exit(1)
  input++
  goto loop
```

> [!IMPORTANT]
> 關鍵: 不是「走到 EOF 就過關」, 而是「某個 transition 讀到 -1 就過關」. 尾字元讀到 null 之前就要觸發 -1.

### Map 結構

- Map 檔 2056192 bytes = **4016 states × 512 bytes/state**
- 每個 state 的 512 bytes 存 128 個 32-bit transition (每 ASCII char 一個)
- `-1` (0xFFFFFFFF) = success transition, 其他是 next state index

### 解法 — BFS 找最短 -1 路徑

從 state 0 出發, BFS 每個 state 的 128 個 transition, 找到任何一個指向 -1 的字元就結束. 路徑上的字元串接起來就是 flag.

```python
import struct
from collections import deque

m = open('map','rb').read()
STATE_SIZE = 512

def trans(state, ch):
    return struct.unpack_from('<i', m, state * STATE_SIZE + ch * 4)[0]

visited = {0}
queue = deque([(0, b'')])
while queue:
    state, path = queue.popleft()
    for ch in range(1, 128):  # 跳過 null
        nxt = trans(state, ch)
        if nxt == -1:
            print((path + bytes([ch])).decode())  # ← 最後 byte 是觸發 -1 的字元, 不屬於 flag
            exit()
        if nxt >= 0 and nxt not in visited:
            visited.add(nxt)
            queue.append((nxt, path + bytes([ch])))
```

跑出來路徑長度 70, 去掉最後一個 trigger byte (`\x01`) 就是:

```
FLAG{W41k 410n3 1n m4z3 15 d4ng3r0u5, y0u m1gh7 n07 f1nd w4y r3v3r53}
```

本地驗證 `./maze ./map 'FLAG{...}'` → `You got it.` ✓

## Sources

- [PHM scoreboard — a-maze](https://ctf.hackme.quest/scoreboard/?id=54)
- `/static/maze.txz`
