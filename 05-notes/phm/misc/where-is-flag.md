# PHM #5 Misc — where is flag (PENDING)

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 50 |
| 解題數 | 558 |
| 附檔 | [`/static/flag.xz`](https://ctf.hackme.quest/static/flag.xz) |
| 狀態 | **未解** — 過濾後的 3 個 candidate server 都 reject |

## 題目

> Do you know regular expression?

一個 `flag.xz`, 解開是 665,816 bytes 的 ASCII 文字塞成一行 (no newlines), 裡面充斥大量**像 flag 但不是 flag** 的 noise:

```
FLAGcuonfsE4iJlrp9mCG
f1Ag
fl@g
FROG
FLAG{...FLAG...}  # 真 flag 裡又塞假 FLAG
```

## 分析

單純 `grep FLAG` 會找到數千筆. 題目明示 "regular expression", 要寫**排除 noise 的 regex**:

- 真 flag 應該是 `FLAG{alphanumeric_content}` 格式
- 噪音常見關鍵字: `f1Ag`, `fl@g`, `FROG`, 嵌套 `FLAG`
- noise 常帶 `(`, `[`, `)` 等怪字元

## 解法

```python
import re
d = open('flag').read()
# 候選: FLAG{...} 內只允許常見字元, 排除嵌套括號
cand = re.findall(r'FLAG\{[^{}\[\]()]+\}', d)
# 二次過濾: 排除含有假 token 的
real = [c for c in set(cand)
        if not any(bad in c[5:-1] for bad in ['f1Ag','fl@g','FROG','FLAG'])]
print(real)
# ['FLAG{M4i8gtG95@oOPFg1qr8K}']
```

**兩層 regex 過濾**:
1. 排除嵌套 `{}[]()` 的 — flag 內不應該有這些
2. 排除含 `f1Ag` / `fl@g` / `FROG` / `FLAG` (嵌套 FLAG) 的 — 這些是出題者故意放的 decoy

剩下**唯一一個**: `FLAG{M4i8gtG95@oOPFg1qr8K}`

## Flag

```
(pending — 3 個 candidate 都失敗)
```

## 嘗試過的 candidates

全部都被 server reject:

- `FLAG{M4i8gtG95@oOPFg1qr8K}` — 最乾淨的候選, 無 noise token
- `FLAG{5t4Oc)7beFX}` — 短 + 含 `)`
- `FLAG{NqxnBo4VQkfJSc(}` — 短 + 含 `(`

另外還試過: 直接把開頭的 leetspeak `wH3r3isFLAGc1oudyoufindit?` (where is FLAG cloud you find it?) 當 flag, 以及 `FLAG{cloud}`, `FLAG{whereisflag}` 等變體, 都失敗.

## 待試方向

- [ ] 可能真 flag 不是 `FLAG{...}` 格式, 要找其他 marker (e.g. 隱藏在特定 offset / 用其他編碼)
- [ ] 檢查檔案是否有 UTF-8 / zero-width 字元編碼 flag
- [ ] 試更複雜 regex (balanced brace, lookahead/lookbehind)
- [ ] 字元頻率分析: 看看是否有特定字元只出現在真 flag 範圍

## 感想

- Regex 題的正解通常是「排除法」而非「正向匹配」, 太寬會有一堆 false positive, 太窄會錯過真答案
- 這題 noise 設計得很細 (`f1Ag` 的 `1` 代 `l`, `fl@g` 的 `@` 代 `a`, `FROG` 字型近似) — 作者下的功夫
- 真實資安場景的 log parsing / data extraction 一樣會遇到: **用什麼不接受**比**用什麼接受**更關鍵
- 但這題 noise filter 完全乾淨的 3 個候選都 reject, 代表作者的「regex 題」正解比單純過濾 noise 更進階 — 可能需要特殊 regex 構造或完全不同的視角
