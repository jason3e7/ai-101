# PHM #79 Pwn — tictactoe-2 (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 250 |
| 狀態 | 僅評估, 尚未攻擊 |

## 題目

> `nc ctf.hackme.quest 7714`, "Get a shell pls"
>
> 跟 [#78 tictactoe-1](tictactoe-1.md) 同 binary

## 評估

- 同 binary, 目標從「贏遊戲」升級成「拿 shell」
- 已知 bug: OOB write via `move` 參數 (write-before-validate) → 可以寫任意 byte 到任意 .data 位址
- #78 只利用 OOB 改 `current_player` 讓 AI 跳不到回合; #79 要再進一步利用 OOB 達成 code execution

## 可能方向

1. 用 OOB write 覆蓋 GOT entry (如 printf / puts) → 跳到 shellcode / libc gadget
2. 或 OOB write 改 saved return address (如果 player_char 在 stack frame 內)
3. 需要 leak libc base (binary 是否 PIE? GOT entry 是否可控?)
