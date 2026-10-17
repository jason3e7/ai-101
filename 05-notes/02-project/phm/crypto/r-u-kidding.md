# PHM #83 Crypto — r u kidding

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 20 |

## 題目

> `EKZF{Hs'r snnn dzrx, itrs bzdrzq bhogdq}`

## 分析

Flag 格式應該是 `FLAG{...}`, 但這邊顯示 `EKZF{...}`. 比對一下:

- `E` → `F` = +1
- `K` → `L` = +1
- `Z` → `A` = +1
- `F` → `G` = +1

每個字母都往後移一位, 就是最經典的 Caesar cipher shift +1 (= ROT25 的反向).

## 解法

把每個字元往後位移一位:

```python
cipher = "EKZF{Hs'r snnn dzrx, itrs bzdrzq bhogdq}"
plain = ''.join(
    chr((ord(c) - ord('A') + 1) % 26 + ord('A')) if c.isupper() else
    chr((ord(c) - ord('a') + 1) % 26 + ord('a')) if c.islower() else c
    for c in cipher
)
print(plain)
# -> FLAG{It's tooo easy, just caesar cipher}
```

## Flag

```
FLAG{It's tooo easy, just caesar cipher}
```

## 感想

真的在 kidding 的程度. 看到 `GMBH` 第一眼就猜 Caesar +1, 試一次就中.
