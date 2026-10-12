# PHM #76 Pwn — petbook (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7710`

## 評估

- 題名「petbook」= 寵物名冊, 另一個「集合物件」題
- 跟 [#75 notepad](notepad-pending.md) 同系: create/edit/delete pet, 每個 pet 是 malloc'd struct
- 典型 heap exploit (UAF / free-list / tcache poisoning)

## 可能方向

- 下載 ELF, 看 pet struct size + glibc version
- 多半有 "name", "age" 等 field, 其中一個是 pointer → UAF leak heap/libc
- 經典 heap tricks: fastbin dup / tcache poisoning / unsorted bin attack
