# PHM Misc #11 — buzzing (100 pts) · 卡住未解

[← 返回索引](../README.md)

## 題目

> Here's a strange bmp image

檔案: [`/static/buzzing.txz`](https://ctf.hackme.quest/static/buzzing.txz)

## 現況: 看到作者 overlay 訊息, 真 flag 不明

### 檔案結構

`.txz` → tar + xz, 解開是 `buzzing.bmp` (1024138 bytes)。

BMP header 說 371 × 377 × 32bpp, 但 pixel data 實際是 **1,024,000 bytes = 256,000 pixels**。這是 header 說謊的 strange 部分。

### 猜對維度 → 640 × 400

256,000 pixels 的常見分解裡, **640 × 400** 顯示為一張清晰的影像:
- 背景是 Wikipedia 「Steganography」條目的文字 (灰字)
- 疊了一層**較大的紅色/黃色 outline 字**

(BMP 預設 bottom-up, 要垂直翻轉才正常)

### DIB header 的 BI_BITFIELDS

V5 header (124 bytes), compression = 3 (BI_BITFIELDS), 色彩 mask 是:

```
R mask: 0x41000000
G mask: 0x00410000
B mask: 0x00004100
A mask: 0x00000041
```

每 channel 只用 2 個 bit (位元 0 跟 6 = `0b01000001`)。其他 6 bit 是「不被 Windows 讀取」的空間 — 經典的 stego 藏點。

### 擷取出來的 overlay 文字 (紅色大字)

```
Have Fun with XDD
Super strange OwO
stego challenge QAQ
I love to
Make annoy-
ing challenages
to make you mad
```

這是作者的 "author's message", **沒有 FLAG{} 字串**。

### 已試但失敗的 flag 猜測

全部 server 收下但沒 AC:

```
FLAG{Have Fun with XDD Super strange OwO stego challenge QAQ I love to Make annoying challenages to make you mad}
FLAG{I love to Make annoying challenages to make you mad}
FLAG{Super strange stego challenge}
FLAG{Super Strange Stego Challenge}
FLAG{annoying challenages}
FLAG{Have Fun with XDD}
FLAG{Super strange OwO stego challenge QAQ}
FLAG{Have Fun}
FLAG{challenages}  (作者打錯字, 不是 challenges)
FLAG{BZBZ}
FLAG{XDD}
```

### 已試但沒找到東西的技巧

- Mask 以外的 6 bit 擷取 → 全 0 (作者只用 mask bits, 其他清零)
- `strings` grep FLAG → 沒有
- `tesseract` OCR → 讀出的就是上面那段 author message
- raw bytes 的各種 channel 組合 → 沒有 FLAG prefix

## 可能方向 (未試)

- 可能真 flag 藏在**小字 Wikipedia 文字裡某個被改過的字** (需要跟維基原文 diff)
- 大 overlay 字裡**每個字首字母**可能編碼 flag
- Mask 設計本身可能有梗 (0x41 = ASCII 'A', 四個 `A` 疊成 "AAAA" 或 "AAAABAAA")
- "buzzing" 這個主題名稱可能是 cipher 提示 (buzz cipher? Caesar cipher 旋轉?)

## 收斂 — 到目前為止

想通了 BMP 的 header 說謊 + 640×400 的真實維度, 看到作者 overlay 訊息, 但真 flag 不在 overlay 文字裡。先紀錄, 之後再回來。
