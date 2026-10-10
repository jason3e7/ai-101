# PHM #18 Web — homepage

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 20 |

## 題目

> Where is the flag? Did you check the code?
>
> 連結: [ctf.hackme.quest](https://ctf.hackme.quest/)

## 分析

### 主頁的 cute.js 是 aaencode

主頁 `<script src="cute.js">`. 下載下來開頭長這樣:

```
﻿ﾟωﾟﾉ= /｀ｍ´）ﾉ ~┻━┻   //*´∇｀*/ ['_']; o=(ﾟｰﾟ)  =_=3; c=(ﾟΘﾟ)=(ﾟｰﾟ)-(ﾟｰﾟ);...
```

這是 **aaencode** (日本顏文字 JS 混淆). 可以在 Node 直接 eval 看它做什麼.

### 攔截 console.log 看它印什麼

```javascript
let args;
console.log = (...a) => { args = a; };
eval(code);
console.error("args[0] preview:", args[0].slice(0, 100));
// %c██%c██%c██... 一堆 "%c██" 配上 740+ 個 "color:#fff" / "color:#333"
```

這是**瀏覽器 DevTools console 的 `%c` 格式化魔法** — 每個 `%c` 後面的字元會用下一個參數的 CSS 樣式染色. 作者用這招在 console 畫一張 **QR Code** (白格 = fff, 深灰格 = 333).

### 用 Python 重建 QR + 解

30 行每行 29 格 (29x29 = QR Version 3). 用 pyzbar 解:

```python
from PIL import Image
from pyzbar.pyzbar import decode

lines = [...]  # 從 cute.js 解出的 "#" / " " grid
# 注意: 要「反向極性」(dark = ' ', light = '#'), 不然 pyzbar 認不出
scale = 20
img = Image.new('1', (29*scale, 29*scale), 1)  # white bg
for y, row in enumerate(lines):
    for x, c in enumerate(row):
        if c == ' ':  # inverted!
            # fill dark pixel
            ...
# 加 100px border 給 QR silent zone
img2 = Image.new('1', (img.size[0]+200, img.size[1]+200), 1)
img2.paste(img, (100, 100))
print(decode(img2))
# -> [Decoded(data=b'FLAG{Oh, You found me!!!!!! Yeeeeeeee.}', ...)]
```

## 解法 (Node + Python 組合)

```bash
# Step 1: Node run cute.js, 攔 console.log 拿 args
node -e '
const fs = require("fs");
let args;
console.log = (...a) => { args = a; };
eval(fs.readFileSync("cute.js", "utf8"));
// Convert to grid: #fff → "#", #333 → " "
const text = args[0];
const colors = args.slice(1);
const lines = text.split("\n");
let i = 0;
const grid = lines.map(line => {
  const n = (line.match(/%c██/g) || []).length;
  return Array.from({length: n}, () => colors[i++] === "color:#fff" ? "#" : " ").join("");
});
fs.writeFileSync("qr.txt", grid.join("\n"));
'

# Step 2: Python decode (記得 invert polarity + 加 border)
python3 decode_qr.py
# -> FLAG{Oh, You found me!!!!!! Yeeeeeeee.}
```

## Flag

```
FLAG{Oh, You found me!!!!!! Yeeeeeeee.}
```

## 感想

連鎖藏: `cute.js` aaencode → `console.log("%c...", "css...")` → DevTools 畫 QR → scan QR. 想不到 "%c" 是真的瀏覽器 feature.

**教訓**: 可疑 JS **直接 eval 攔 console.log** 比看 symbol 爬意圖快多了. 瀏覽器開 DevTools 看 console 應該會直接看到 QR 圖, 但 server-side 複製 cute.js 到 Node 才搞懂機制.
