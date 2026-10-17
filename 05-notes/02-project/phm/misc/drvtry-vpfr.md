# PHM Misc #12 — drvtry vpfr (100 pts) · AC

[← 返回索引](../README.md)

## 題目

> G:SH}Djogy <u Lrunpstf Smf Yu[omh Dp,ryjomh|

標題: `drvtry vpfr`

## 解法

標題跟 hint 都是**在 QWERTY 鍵盤上, 每個鍵往左移一位**打出來的。

把「drvtry vpfr」拿鍵盤左移還原:

| d → s | r → e | v → c | t → r | r → e | y → t |
|---|---|---|---|---|---|
| v → c | p → o | f → d | r → e |  |  |

得到: **"secret code"**. 確認方向正確, 用同樣規則解整段 hint:

```python
m = r"qwertyuiop[]\\asdfghjkl;zxcvbnm,./"
M = r"QWERTYUIOP{}|ASDFGHJKL:ZXCVBNM<>?"

def shift(c):
    if c in m:
        i = m.index(c); return m[i-1] if i > 0 else c
    if c in M:
        i = M.index(c); return M[i-1] if i > 0 else c
    return c

s = "G:SH}Djogy <u Lrunpstf Smf Yu[omh Dp,ryjomh|"
print("".join(shift(c) for c in s))
# FLAG{Shift My Keyboard And Typing Something}
```

符號也要照著移: `:` (Shift+`;`) → `L` (Shift+`l`), `}` (Shift+`]`) → `{` (Shift+`[`), `<` (Shift+`,`) → `M` (Shift+`m`), `[` → `p`, `|` (Shift+`\`) → `}` (Shift+`]`)。

**Flag**: `FLAG{Shift My Keyboard And Typing Something}`

## 收斂

- 標題本身就是明示: `drvtry vpfr` = `secret code` 左移一鍵
- 做這題不用寫 code, 盯著鍵盤手動 shift 也可
- 符號的 shift 要跟著規則走 (shift+key 的對位), 不能只處理字母
