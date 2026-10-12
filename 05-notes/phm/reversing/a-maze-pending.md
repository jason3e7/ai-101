# PHM #54 Reversing — a-maze (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> Escape from my a-maze
>
> Binary: ELF + 2MB map file

## 評估

- 2MB map file + VM runner, 經典「迷宮搜尋」題
- Flag 多半是從起點走到終點的 path 編碼

## 可能方向

- 讀懂 VM runner 的 input format (map 怎麼 parse)
- 寫 BFS/DFS 找最短可達路徑
- Path (U/D/L/R 序列) 可能 base64 / xor 後是 flag
