# PHM #73 Pwn — stack (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7716`, "Here is my C programming homework" + Source Code

## 評估

- 作者說「C 程式作業」 → 新手 C 程式常犯的 stack 類漏洞
- 題名就是 "stack" → 最直接的 stack-based 題
- 有 Source Code → 可以直接讀原始碼找 bug

## 可能方向

1. 下載 Source Code, 看 C 程式邏輯
2. 常見新手 bug: `gets()`, `strcpy()` 不檢查, printf 用 user input 當 format string
3. 針對找到的 bug 寫 pwntools exploit
