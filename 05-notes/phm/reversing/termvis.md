# PHM #56 Reversing — termvis

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 230 |
| Flag | `FLAG{Brainfuck in the PNG f**ks your brain!}` |

## 題目

> This cool program can print PNG on your terminal but also hurts your brain.
>
> `termvis.tar.xz` → ELF + 4 張 PNG (`echo.png`, `pan.png`, `checkflag.png`, `show-flag.png`)

## 分析 — Analysis

### 第一層: PNG → ANSI 256-color terminal 畫圖

`./termvis img.png` 會把每個像素量化成 ANSI 256-color (`\x1b[48;5;Nm  `) 畫在 terminal, 常規的「圖轉終端藝術」工具. show-flag.png 是美國國旗 (廢話), pan.png 是鍋 (chicken 梗), checkflag.png 是戴墨鏡的 Pusheen.

### 第二層 — "hurts your brain": 像素當 Brainfuck opcode 跑

反組譯看到 main 畫完圖後呼叫 `0x4065f0`, 這函數在一個全域 tape pointer `[0x6080c0]` 上跑 8 opcodes dispatch, 以 `byte & 7` 當 opcode. 每張 PNG 的像素資料被當 BF 程式執行:

| byte & 7 | 動作 |
|:---:|:---|
| 0 | `>` 右移指標 |
| 1 | `<` 左移指標 |
| 2 | `+` tape 值 +1 |
| 3 | `-` tape 值 -1 |
| 4 | `.` putc(tape[ptr]) |
| 5 | `,` tape[ptr] = getc() |
| 6 | `[` loop start |
| 7 | `]` loop end |

跑完各張圖:
- `pan.png` → "Chicken Chicken, Winner Dinner."
- `show-flag.png` → "What? I just show you the flag! Oh, You want the flag for CTF? Decode check-flag.png instead."
- `checkflag.png` → "Flag?" 等 stdin → 比對後印 "Status: Good flag" 或 "Status: Bad flag"
- `echo.png` → 把輸入 echo 回去 (無聊 troll)

> [!IMPORTANT]
> 真正要解的是 `checkflag.png`.

### 第三層: 從 BF 原碼反推 expected flag

把 checkflag.png 的像素抽出 → BF 程式. 每個 input 字元前有一段固定 pattern:

```brainfuck
,              # 讀一個字元到 tape[k]
>>+++++++      # tape[k+2] = 7
[<++++++++++>-] # tape[k+1] += 7 * 10 = 70
<< +NNN        # tape[k+1] += NNN (實際程式用 1 次 < 不是 <<)
```

所以第 i 位的「期望字元」 = `tape[k+1] = 7 * 10 + NNN = 70 + NNN`.

用正則抓 `, >> +A [< +10 > -] < +B` 模式解析, A=7 固定, B 各字元不同. `A*10 + B` 就是 expected ASCII:

| i | A | B | ASCII | char |
|:---:|:---:|:---:|:---:|:---:|
| 0 | 7 | 0 | 70 | F |
| 1 | 7 | 6 | 76 | L |
| 2 | 6 | 5 | 65 | A |
| 3 | 7 | 1 | 71 | G |
| 4 | 12 | 3 | 123 | { |
| 5 | 6 | 6 | 66 | B |
| ... | | | | |

掃到第 43 個 `,` 開始出現 `}` (= 125), 就是 flag 結尾. 共 44 字元:

```
FLAG{Brainfuck in the PNG f**ks your brain!}
```

本地驗證 `echo '...' | ./termvis checkflag.png` → `Status: Good flag` ✓.

## 解法 — Exploit

```python
# 把 PNG pixel bytes 當 BF opcode, 每位 input 比對 pattern 抽 expected char
from PIL import Image
import re

im = Image.open('checkflag.png').convert('RGB')
bytes_ = [b for p in im.getdata() for b in p]
OPCODES = ['>', '<', '+', '-', '.', ',', '[', ']']
bf = ''.join(OPCODES[b & 7] for b in bytes_)

def tok(s):
    out, i = [], 0
    while i < len(s):
        c = s[i]
        if c in '+-<>':
            j = i
            while j < len(s) and s[j] == c: j += 1
            out.append((c, j - i)); i = j
        else:
            out.append((c, 1)); i += 1
    return out

tokens = tok(bf)
reads = [i for i, t in enumerate(tokens) if t[0] == ',']
flag = bytearray()
for pos in reads:
    block = tokens[pos+1:]
    if block[0] != ('>', 2): break
    a = block[1][1]  # multiplier (= '+')
    # skip past the loop body [...]
    k, depth = pos + 2, 0
    while k < len(tokens):
        if tokens[k][0] == '[': depth += 1
        elif tokens[k][0] == ']':
            depth -= 1
            if depth == 0: k += 1; break
        k += 1
    if tokens[k][0] != '<': break
    k += 1
    extra = tokens[k][1] if k < len(tokens) and tokens[k][0] == '+' else 0
    flag.append(a * 10 + extra)

print(bytes(flag))
# → b'FLAG{Brainfuck in the PNG f**ks your brain!}...'
```

## Sources

- [PHM scoreboard — termvis](https://ctf.hackme.quest/scoreboard/?id=56)
- `/static/termvis.tar.xz`
