# PHM Misc #10 — otaku (90 pts) · 卡住未解

[← 返回索引](../README.md)

## 題目

> Some data was hidden in these images, please find the flag.

檔案: [`/static/otaku.7z`](https://ctf.hackme.quest/static/otaku.7z)

## 現況: 找到 3 個假 flag, 真 flag 不明

7z 解開有 4 個檔案 (四位御宅族人氣角色):

- `Kuroyuki_Hime.png` (《加速世界》黒雪姫)
- `Mikoto_Misaka.png` (《魔法禁書目錄》御坂美琴)
- `Miku_Hatsune.png` (初音ミク)
- `Yuuki_Asuna.jpg` (《刀劍神域》結城明日奈)

### 已找到的 3 個假 flag

| 檔案 | 位置 | 文字 |
|:---|:---|:---|
| Kuroyuki_Hime.png | tEXt chunk | `FAKE{D1d y0u u5ed br41n bur5t?}` |
| Mikoto_Misaka.png | alpha channel LSB plane 顯示 | `F14G{This is not you want}` |
| Yuuki_Asuna.jpg | EOI 後 gzip trailing | `OOPS{You have the wrong file}` |
| Miku_Hatsune.png | **???** | **沒找到** |

三個都是明顯的 troll (`FAKE / F14G / OOPS`), 不是 `FLAG{}`。

### Mikoto alpha 細節

Mikoto 的 alpha channel 只有 4 個不同值: `255, 254, 251, 249`。LSB 平面繪出來剛好是 `F14G{This is not you want}` 這張文字圖。四個 channel (R/G/B/A) 的 LSB 平面都顯示同一組文字 — 那是圖本身, 不是另外藏的 stego。

bit 1, bit 2, bit 3 各別的平面都是純黑, 沒有別的訊息。

### Miku 分析

Miku 是真正藏 flag 的目標 (因為前三個都是 troll)。但我做了:

- 四個 channel 各別 LSB plane 圖 → 都是正常影像 noise
- 全部 bit plane (bit 0-7) × 4 channels → 都看起來像一般圖像的區塊
- zsteg -a 掃全部組合 → 沒有可讀文字
- 檢查 PNG chunks (pngcheck) → 乾淨, IHDR / bKGD / pHYs / tIME / IDAT × 多個 / IEND, 沒有隱藏 chunk
- alpha channel → 全 255 (無變化)
- 檢查 background pixels (R/G/B > 240 但不全 255) → 只是 anti-aliasing 邊緣, 不是文字
- strings scan + grep FLAG → 無結果
- 試 steghide 密碼: 空字串, 常見角色名 (Miku/Asuna/Mikoto/...) → 都失敗 (steghide 只支援 JPG/BMP/WAV, 對 Asuna.jpg 試過)

## 可能方向 (未試)

- 把 4 張圖的 pixel data **逐 pixel XOR / 相減 / 疊合** — 可能某兩張相減會露出 flag
- 檢查 PNG filter bytes (每行開頭的 filter type) 是否編碼額外 bit
- Mikoto alpha 4 個值 (255/254/251/249) 的「2-bit 編碼」可能有未解讀的訊息 (但我試了 4 種對應方式都是 `0x55` 或 `0xAA` 的填充 pattern)
- Yuuki_Asuna.jpg 做 JPEG DCT 分析 (F5 / JSteg)
- 這題只有 28 人解出, 非常低, 應該不是直覺 LSB

## 收斂 — 到目前為止

四張角色圖裡, Mikoto / Kuroyuki / Yuuki 都有 troll flag, Miku 應該藏真 flag 但 LSB / bit plane / chunk / strings / zsteg 都沒看出來。先紀錄, 之後再回來。
