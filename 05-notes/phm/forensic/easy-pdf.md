# PHM #100 Forensic — easy pdf

| 項目 | 值 |
|:---|:---|
| 類別 | Forensic |
| 分數 | 40 |
| 解題數 | 190 |
| 附檔 | [`/static/easy.pdf`](https://ctf.hackme.quest/static/easy.pdf) |

## 題目

> easy pdf

一份 Pages 匯出的 PDF, 2 頁, 看起來只有「報告標題」+ Pages 預設的占位文字 (「此為暫存區文字...」).

## 分析

`pdftotext -layout` 看不到 flag — 可見文字全是占位. 但 PDF content stream 裡的文字會夾在 `BT ... ET` 區塊的 `TJ` 操作中, 有時候 pdftotext 會因為:

- 文字座標在頁面外 (negative x/y)
- 顏色跟底色一樣 (白字白底)
- 字體子集化 (subset) 讓文字 fragment

而漏抓. 直接解壓所有 `FlateDecode` stream 掃原始 content 比較穩.

## 解法

```python
import re, zlib
with open('easy.pdf','rb') as f: data=f.read()
pat = re.compile(rb'<<([^<>]*?)>>\s*stream\r?\n(.*?)\r?\nendstream', re.DOTALL)
for m in pat.finditer(data):
    try: inflated = zlib.decompress(m.group(2))
    except: continue
    if b'FLAG' in inflated:
        # 找 FLAG 附近 200 bytes
        for mm in re.finditer(rb'FLAG', inflated):
            s=max(0,mm.start()-80); e=min(len(inflated),mm.end()+200)
            print(inflated[s:e])
```

輸出:

```
BT 10 0 0 10 0 0 Tm /TT3 1 Tf
[ (FLAG{T) 111 (ext in PDF is easy to r) 18 (ecover}) ] TJ ET
```

是個 `TJ` 操作, 字串被切成 `(FLAG{T)` + `(ext in PDF is easy to r)` + `(ecover})`, 中間插的 `111` / `18` 是字距調整 — 所以 `pdftotext` 解出來會壞或漏掉. 把 3 段拼回去就是 flag.

坐標 `BT 10 0 0 10 0 0 Tm` + 外層 `cm -172 230` 的位移把這段文字放到頁面外, 一般檢視器看不到, 但 content stream 一拉就露出來.

## Flag

```
FLAG{Text in PDF is easy to recover}
```

## 感想

PDF 隱藏文字的經典三招 (座標外推 / 同色隱藏 / 字距打散), 這題三個都用了. `pdftotext` 看不到, 直接解壓 stream 掃 content 是最乾淨的做法.
