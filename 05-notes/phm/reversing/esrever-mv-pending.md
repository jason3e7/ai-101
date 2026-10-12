# PHM #55 Reversing — esrever-mv (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 240 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> VM is fun
>
> Binary: static ELF 849KB

## 評估

- 題名「esrever-mv」= 反轉的「vm-reverse」, 明示是 VM 題
- static ELF 省很多力 (不用解 dynamic linker)

## 可能方向

- VM 題標準流程:
  1. Dump bytecode (多半在 .data / .rodata)
  2. 分析 opcode handler table (通常一個 big switch)
  3. 寫 disassembler + 看 VM 程式邏輯
  4. 從 VM 程式反推 flag check
