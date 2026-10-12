# PHM #69 Pwn — raas (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 160 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7719`

## 評估

- 題名「raas」= "ret-as-a-service" (或 ret2libc / return-oriented 的縮寫)
- 多半是 BOF 讓玩家一次呼叫一個 libc function, 透過**多輪互動**組 ROP chain

## 可能方向

- 下載 ELF, 看 input 格式
- 每輪可能可控 1 個 call: `call(func_addr, arg1, arg2, ...)`
- 用 printf/puts leak libc, 下一輪 call system("/bin/sh")
- 或 open+read+write syscall chain 讀 flag
