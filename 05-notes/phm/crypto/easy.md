# PHM #82 Crypto — easy

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 10 |
| 解題數 | 774 |

## 題目

> `526b78425233745561476c7a49476c7a4947566863336b7349484a705a3268305033303d`

## 分析

題目字串長度 72, 全是 `0-9a-f`, 典型 hex. Hex 解開是 44 字元 ASCII, 結尾是 `=` — 標準 base64. Base64 再解一次拿到 flag.

## 解法

```python
import base64
h = '526b78425233745561476c7a49476c7a4947566863336b7349484a705a3268305033303d'
b64 = bytes.fromhex(h).decode()
# -> 'RkxBR3tUaGlzIGlzIGVhc3ksIHJpZ2h0P30='
print(base64.b64decode(b64).decode())
# -> FLAG{This is easy, right?}
```

## Flag

```
FLAG{This is easy, right?}
```

## 感想

Crypto 類別的最入門暖身, 兩層標準編碼 (hex → base64 → plaintext). 10 分該有的樣子, 練手感而已.
