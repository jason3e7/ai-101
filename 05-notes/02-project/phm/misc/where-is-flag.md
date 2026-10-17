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

## 字元頻率分析 (2026-10-12 新發現)

byte frequency 檢查:
- `?` **只出現 1 次**, 就在開頭 `wH3r3isFLAGc1oudyoufindit?` 的句尾
- `!`, `.`, `_`, `-` 完全不出現
- `@` 只在 `fl@g` noise 內 (7760 次)
- 唯一的非 printable 字元是檔尾的 `\n`
- 噪音 token 計數: `FLAG` 7954, `f1Ag` 16096 (= 2x FLAG), `fl@g` 7760, `FROG` 8127

**→ 真 flag 不可能含 `.`, `_`, `-`, `!`** (這些字元整個 corpus 都沒有). 之前試的 `FLAG{where_is_flag}` 等含 `_` 的本就不可能.

## 試過但沒中的 candidate (累計)

- 乾淨 regex 過濾出的 3 個: `FLAG{M4i8gtG95@oOPFg1qr8K}`, `FLAG{5t4Oc)7beFX}`, `FLAG{NqxnBo4VQkfJSc(}`
- 開頭問句的各種包裝: `wH3r3isFLAGc1oudyoufindit?`, `FLAG{wH3r3isFLAGc1oudyoufindit?}`, 大小寫 / 去 `?` / 全 leet / 不同片段
- Cloud / could 系列: `FLAG{cloud}`, `FLAG{c1oud}`, `FLAG{incloud}`, `FLAG{thecloud}`, `FLAG{c0u1dy0uf1nd1t}` 等 10+ 個
- Regex pattern 本身當 flag: `FLAG{/FLAG\{[^{}]+\}/}`, `FLAG\{[^{}]+\}` 等
- Base-4 編碼 (FLAG=0, f1Ag=1, fl@g=2, FROG=3) 解出來: 無意義亂碼
- 空 flag `FLAG{}` (出現 7 次是 corpus 裡最多的乾淨 pattern) → 也不中

## 待試方向

- [ ] 真 flag 可能完全不是 ASCII — 檢查 UTF-8 / zero-width Unicode 編碼 (但 byte 分析顯示全是 printable ASCII, 這條路不太可能)
- [ ] Base64 解析某段內容 (file 本身有 6104 個 base64-like 32+ 字元段落)
- [ ] 找特定 offset / 座標的字元組成 flag (e.g. 每 N 個字一個, 或噪音 token 之間的 "payload")
- [ ] 看噪音 token 中間的 payload 字串是否組成有意義文字 (已試單純 strip 看不出來)
- [ ] 真的放棄, 等有別人 writeup

## 感想

- Regex 題的正解通常是「排除法」而非「正向匹配」, 太寬會有一堆 false positive, 太窄會錯過真答案
- 這題 noise 設計得很細 (`f1Ag` 的 `1` 代 `l`, `fl@g` 的 `@` 代 `a`, `FROG` 字型近似) — 作者下的功夫
- 真實資安場景的 log parsing / data extraction 一樣會遇到: **用什麼不接受**比**用什麼接受**更關鍵
- 但這題 noise filter 完全乾淨的 3 個候選都 reject, 代表作者的「regex 題」正解比單純過濾 noise 更進階 — 可能需要特殊 regex 構造或完全不同的視角
