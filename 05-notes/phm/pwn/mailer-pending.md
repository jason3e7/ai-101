# PHM #77 Pwn — mailer (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 150 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7721`

## 評估

- 題名「mailer」= 寄信程式
- 相對低分 (150), 可能是 FSB / 簡單 BOF, 不像 heap 類那麼複雜
- 題目沒給 Source Code, blackbox

## 可能方向

- 下載 ELF, 跟服務互動看 input format
- 常見 mailer 題: subject/body 輸入, 其中一個觸發 format string 或 overflow
- 若有 printf user_input: FSB leak canary/libc, 寫 GOT overwrite
