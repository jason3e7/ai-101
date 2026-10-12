# PHM #57 Reversing — rc87cipher (未解, 未實測)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 500 |
| 狀態 | 僅評估, 尚未下載分析 |

## 題目

> Tips: password length is 40

PHM 最高分題 (500 分), 全站很少人解.

## 評估

- 自製 cipher ("rc87" 是作者取名, RC4 風格)
- Password 40 chars → 窮舉空間太大 (理論 256^40), 肯定要**找結構缺陷**
- 題目通常附三個檔: rc87 encryptor + flag.enc + rc87.enc (作者用自己工具加密 encryptor 本身)

## 可能方向

- `rc87.enc` 是「encryptor 加密 encryptor」 → **known plaintext attack** (plaintext 就是 encryptor binary)
- 從 encryptor binary 分析 key schedule, 看 state transition 是否有 bias / 可解
- RC4 類的 weakness (RC4 本身有 key-recovery attacks for weak keys), rc87 可能被作者簡化到有洞
