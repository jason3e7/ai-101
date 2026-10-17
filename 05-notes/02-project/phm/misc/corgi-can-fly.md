# PHM #2 Misc — corgi can fly

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 50 |
| 解題數 | 1130 |
| 附檔 | [`/static/corgi-can-fly.png`](https://ctf.hackme.quest/static/corgi-can-fly.png) |

## 題目

> Corgi is cute, right? Pillow (Python) and Bitmap (.NET) are your friends. (Maybe you can try stegsolve)

一張 1000x667 的 PNG, 畫面是柯基在雪地上跳 (所以「可以飛」). Hint 明示: 用能讀像素的函式庫 + 試試 stegsolve.

## 分析

兩道線索疊起來:

1. **PNG tEXt chunk** — 用 Python 掃 chunks, 看到 `Artist` 欄位放了一段 base64:
   ```
   RGlkIHlvdSB0cmllZCBMU0I/Cg==  →  Did you tried LSB?
   ```
   出題者直接留了提示: 試 LSB.

2. **24 個 bit-plane 挑一** — 把每個 channel (R/G/B) 的每個 bit (0-7) 拉成單色圖儲存. **R channel bit 0** 出現一個明顯的 QR code (G/B 同位也有同一個 QR, 推測是同時寫到三個 channel).

## 解法

```python
# 1. 掃 chunks 找線索
import struct, base64
with open('corgi-can-fly.png','rb') as f: d = f.read()
off = 8  # 跳過 PNG magic
while off < len(d):
    length = struct.unpack('>I', d[off:off+4])[0]
    ctype = d[off+4:off+8].decode('latin1')
    data = d[off+8:off+8+length]
    if ctype == 'tEXt':
        print(ctype, data)   # Artist | RGlkIHlvdSB0cmllZCBMU0I/Cg==
    if ctype == 'IEND': break
    off += 12 + length

# 2. 拉 R channel bit 0 成單色圖
from PIL import Image
im = Image.open('corgi-can-fly.png').convert('RGB')
W, H = im.size
pixels = list(im.getdata())
out = Image.new('L', (W, H))
out.putdata([(p[0] & 1) * 255 for p in pixels])   # R bit 0
out.save('R_bit0.png')

# 3. 掃 QR
# $ zbarimg R_bit0.png
# -> QR-Code:FLAG{Corgi is cutest aniaml on the earth >////////<}
```

> [!TIP]
> 不用 `stegsolve` (需要 Java) 也行 — 24 個 bit-plane 全部 dump 成 PNG, 肉眼掃一遍就知道哪個藏東西 (有結構的那張一看就不像雜訊). 這招對任何 bit-plane stego 都通用.

## Flag

```
FLAG{Corgi is cutest aniaml on the earth >////////<}
```

> [!WARNING]
> flag 裡 "aniaml" 是**出題者原本的拼寫錯誤**, 不是我打錯. 送 flag 要照原字送才會 AC.

## 感想

LSB QR 是 CTF stego 的 101 題型. 這題的工程面重點:

- **別跳過 metadata**: PNG 的 tEXt / zTXt / iTXt chunk 經常被忽略, 這題作者還好心留了 "Did you tried LSB?" 當 breadcrumb
- **bit-plane dump 比 stegsolve 快**: 24 張 PNG 用 PIL 寫 10 行 code 全出, 比開 Java GUI 一個個切快多了
- **三個 channel 同資料是常見冗餘手法**: 作者把 QR 寫到 R/G/B bit 0, 任何一個 channel 都能恢復, 增加容錯
