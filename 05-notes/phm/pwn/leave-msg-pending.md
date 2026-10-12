# PHM #72 Pwn — leave_msg (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7715`, "I am on a vacation, leave the message for me."

## 評估

- 題名 + 題目描述 → **留言板**題
- 多半是「輸入訊息存進 buffer, 之後可以 echo」 → 經典 BOF / off-by-one / heap note
- 200 分, 中等難度

## 可能方向

- 下載 ELF, 看是 stack-based 還是 heap-based
- Stack: 留言長度超過 buffer → overflow 覆蓋 saved ret
- Heap: malloc 存留言, UAF / double-free / tcache poisoning
- 典型 flag 位置: `cat flag` via `system()` 或 ROP 讀檔
