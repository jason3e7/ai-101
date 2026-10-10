# PHM #43 Reversing — passthis

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 80 |

## 題目

> You should be able to pass this
>
> 檔案: [`/static/passthis.exe`](https://ctf.hackme.quest/static/passthis.exe)
> Windows PE32, stripped.

## 分析

`objdump -s -j .rdata` 看字串區, 緊接 "Good flag ;)" 後面有一段 binary:

```
404040 c1cbc6c0 fcc9e8ab a7dee8f2 a7f4efe8  ................
404050 f2ebe3a7 e9e8f3a7 f7e6f4f4 a7f3efe2  ................
404060 a7e1ebe6 e0fa                        ......
```

共 39 bytes. 判斷: 前 4 bytes `c1 cb c6 c0` 如果是 `FLAG` XOR 某 key, 算 key = `0x46 ^ 0xc1 = 0x87`. 四個 byte 都 XOR 0x87 都得 FLAG. 繼續整段 XOR 0x87:

```python
data = bytes.fromhex('c1cbc6c0fcc9e8aba7dee8f2a7f4efe8f2ebe3a7e9e8f3a7f7e6f4f4a7f3efe2a7e1ebe6e0fa')
print(bytes(b ^ 0x87 for b in data).decode())
# -> FLAG{No, You should not pass the flag}
```

## Flag

```
FLAG{No, You should not pass the flag}
```
