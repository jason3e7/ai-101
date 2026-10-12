# PHM #56 Reversing — termvis (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 230 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> This cool program can print PNG on your terminal but also hurts your brain.

## 評估

- 工具是「終端畫 PNG」的小程式
- flag 多半藏在它畫出來的 PNG 內容裡 (ANSI escape sequence 畫色塊 → 還原成 image)

## 可能方向

- 讀懂它用什麼 ANSI escape 畫圖 (24-bit color?)
- Pipe `./termvis flag.png` 的 stdout 到檔案
- Parse ANSI 把每個 cell 的 RGB 還原成像素, 存回 PNG
- 看還原的 PNG 是不是直接寫 FLAG{...}
