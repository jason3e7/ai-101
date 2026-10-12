# PHM #68 Pwn — onepunch (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 160 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> `nc ctf.hackme.quest 7718`, 題目有 Source Code 連結

## 評估

- 題名「onepunch」= one-gadget RCE 經典暗示
- 多半是 FSB / overflow 可控單一 QWORD, 剛好塞得下 libc 裡的 one-gadget RCE 位址

## 可能方向

1. 下載 ELF + 看 Source Code
2. 找到可控的 ret / GOT 位址 (1 QWORD)
3. 跑 `one_gadget libc.so.6` 找所有可用 gadget
4. Leak libc base (puts GOT / stack leak), 算 one-gadget 絕對位址
5. 條件檢查: one-gadget 的 constraint (rsp/rax 要滿足特定條件)
