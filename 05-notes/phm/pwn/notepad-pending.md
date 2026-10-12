# PHM #75 Pwn — notepad (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7713`

## 評估

- 題名「notepad」= 筆記本程式, 多半是 heap note-based 題
- 常見結構: create/edit/delete/list note, 每個 note 是 malloc'd buffer
- 典型漏洞: UAF (delete 不清指標), heap overflow (edit 寫超過 size)

## 可能方向

- 下載 ELF, 看 note struct 跟 glibc version
- 典型 heap exploit 流程:
  - UAF leak heap addr / libc addr
  - tcache poisoning / fastbin dup
  - overwrite __free_hook / __malloc_hook → system("/bin/sh")
