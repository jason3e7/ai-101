# PHM #74 Pwn — very_overflow (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7705`, Source Code 連結

## 評估

- 題名就是「非常 overflow」, 多半是 stack overflow 類
- 有 Source Code
- 200 分, 不是最 trivial 的 BOF (那會是 50-100 分), 應該有防護繞

## 可能方向

1. 讀 Source Code 看 overflow 位置跟保護機制 (canary / NX / PIE / RELRO)
2. 根據保護決定路線:
   - NX off → shellcode
   - NX on + no canary → ROP
   - canary on → leak canary (printf?) + ROP
