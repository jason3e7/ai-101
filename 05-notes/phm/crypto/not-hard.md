# PHM #84 Crypto — not hard

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 50 |

## 題目

> `Nm@rmLsBy{Nm5u-K{iZKPgPMzS2I*lPc%_SMOjQ#O;uV{MM*?PPFhk|Hd;hVPFhq{HaAH<`
>
> Tips: `pydoc3 base64`

## 分析

字串裡混了標點 (`@`, `{`, `*`, `%`, `#`, `|`) 跟大小寫字母, 不是 base64 字符集. 作者的 Tips 直接說 `pydoc3 base64` — 看 `base64` module 的全部編碼. 字串長度搭配字元範圍, 判斷是 **base85**.

試 base85 解看看:

```python
import base64
s = "Nm@rmLsBy{Nm5u-K{iZKPgPMzS2I*lPc%_SMOjQ#O;uV{MM*?PPFhk|Hd;hVPFhq{HaAH<"
decoded = base64.b85decode(s)
# -> 大寫 + 數字 + '=' 填充
```

結尾一堆 `=` + 全大寫 A-Z + 數字 2-7, 是 **base32**:

```python
flag = base64.b32decode(decoded).decode()
# -> FLAG{Do you know base32 encoding?}
```

## 解法

```python
import base64
s = "Nm@rmLsBy{Nm5u-K{iZKPgPMzS2I*lPc%_SMOjQ#O;uV{MM*?PPFhk|Hd;hVPFhq{HaAH<"
print(base64.b32decode(base64.b85decode(s)).decode())
```

## Flag

```
FLAG{Do you know base32 encoding?}
```

## 感想

兩層標準編碼, base85 外層 + base32 內層. 看字符集就能判斷每一層, 一次做對.
