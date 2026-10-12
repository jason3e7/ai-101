# PHM #52 Reversing — mov (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `MOV` instruction is turing complete!
>
> Binary: 5.9MB ELF

## 評估

- 用 [movfuscator](https://github.com/xoreaxeaxeax/movfuscator) (Chris Domas) 編譯
- 整個 binary 幾乎只有 `mov` 指令, 把 x86 當 Turing-complete 的 single-instruction compiler
- 5.9MB 大小是 mov 展開特性 (正常 C 程式 << 1MB)

## 可能方向

- 用 **demovfuscator** (同作者) 自動還原成正常 x86
- r2 / IDA 做基本 pattern recognition
- 不用自己打死讀 mov instructions
