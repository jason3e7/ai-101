# PHM #11 Misc — buzzing

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 100 |
| 附檔 | [`/static/buzzing.txz`](https://ctf.hackme.quest/static/buzzing.txz) |

## 題目

> Some data was hidden in these images, please find the flag.

解開 `buzzing.txz` → 一張 `buzzing.bmp` (1,024,138 bytes), 用瀏覽器/image viewer 開看起來就是 Wikipedia 的 Steganography 條目截圖, 上面蓋了一層半透明的英文留言 (Have Fun with XDD / Super strange OwO / stego challenge QAQ / I love to / Make annoying challenages / to make you mad). flag 完全看不到.

## 分析

### BMP header 說謊

```
Signature: BM, filesize: 1024138, pixel offset: 138
DIB size: 124 (BITMAPV5HEADER), width=371, height=377, bpp=32
Compression: 3 (BI_BITFIELDS)
R mask: 0x41000000
G mask: 0x00410000
B mask: 0x00004100
A mask: 0x00000041
```

Header 寫 371×377 = 139,867 pixels, 但 pixel data 只有 1,024,000 bytes = **256,000 pixels**. 試幾個常見分辨率 → **640×400** 剛好對上.

→ **DIB header 的 width/height 被改掉了, 真實尺寸要靠 pixel data 大小推回來**.

### 四個 mask 都是 0x41 (bits 0, 6)

每個 channel 只用 `0b01000001` 這兩個 bit 顯示 → 剩下的 6 bits (`0b10111110 = 0xBE`) 可以藏東西.

### Alpha channel 永遠是 0x41

```python
unique(A channel) = [65]   # 只有這一個值
unique(R/G/B)     = [1..254]  # 全 256 個 (無 0 跟 255)
```

→ Alpha 乖乖的, **R/G/B 三個 channel 的 bit 0 跟 bit 6 都藏了獨立訊息**.

### Pixel 在 file 裡的 byte order

32-bit little-endian pixel + 上面的 mask 配置, byte 順序是:
```
byte[0] = A (mask 0x00000041)
byte[1] = B (mask 0x00004100)
byte[2] = G (mask 0x00410000)
byte[3] = R (mask 0x41000000)
```

渲染器把 `byte & 0x41` 的兩個 bit 當這個 channel 的強度輸出, bit 6 大 bit 0 小; 整體看起來就是 Wikipedia 文字 + 雜湊的英文 overlay.

## 解法 — Bit plane 分離

把每個 channel 的 bit 0 單獨拉出來看, 再求「只有這個 channel 有」的 pixel (exclusive bit), 就會把三組 overlay 分開:

```python
import numpy as np
from PIL import Image

d = open('buzzing.bmp', 'rb').read()
pixels = np.frombuffer(d[138:138+640*400*4], dtype=np.uint8).reshape(400, 640, 4)
pixels = pixels[::-1]  # BMP 是 bottom-up

r = pixels[:,:,3] & 1  # R bit 0
g = pixels[:,:,2] & 1  # G bit 0
b = pixels[:,:,1] & 1  # B bit 0

# "只有 R 有的 bit" → 單獨紅色 overlay
r_only = (r & ~g & ~b) * 255
Image.fromarray(r_only.astype(np.uint8), 'L').resize((640*3, 400*3)).save('r-only.png')
```

產出的 `r-only.png` 字面上寫著:

```
your
flag
is
FLAG{M0dIfy_
ME7A_D474_
c@n_B_fUn}
```

連起來: **`FLAG{M0dIfy_ME7A_D474_c@n_B_fUn}`** (= "Modify metadata can be fun" 的 leet)

### 其他 bit plane 的 overlay (順便看)

- G channel bit 0 → "Super Strange stego challenge Modify making challenages make you mad" 玩笑話
- B channel bit 0 → "your Have Fun with XDD OwO QAQ..." 開場白
- 三個 channel 的 bit 6 → 底層 Wikipedia 條目文字 + 其他雜湊
- 真正的 flag 只藏在 **R bit 0 `AND NOT` G bit 0 `AND NOT` B bit 0** 的 pixel 裡

## Flag

```
FLAG{M0dIfy_ME7A_D474_c@n_B_fUn}
```

## 感想

- 題名 "buzzing" 雙關: 蜂群 (bee / `BI_BITFIELDS`) + visual noise (多層 overlay 疊得滿滿的看起來很吵)
- 關鍵提示都在 BMP header: 四個 mask 都是 `0x41 = 0b01000001`, 一看就知道只用了 2 bit, 剩下 6 bit 可藏資料
- 標題雙關 + 把 `width/height` 寫錯 + 用 BI_BITFIELDS 不常見的 mask → 全在暗示「這張圖不是看起來那樣」
- flag 本體 "Modify Metadata can be fun" 一語雙關: 解法本質就是去 **改變我們讀取 pixel metadata 的方式** — 不信 header 的尺寸、挑真正藏資料的 bit plane
- 讀 BMP 遇到 `BI_BITFIELDS` 一定要看 mask, 遇到 bpp=32 + alpha 一定要分 channel 看, 這題把這兩個觀念當 primitive

## Sources

- [PHM scoreboard — buzzing](https://ctf.hackme.quest/scoreboard/?id=11)
- `/static/buzzing.txz`
- [BMP file format — BITMAPV5HEADER 跟 BI_BITFIELDS](https://en.wikipedia.org/wiki/BMP_file_format)
