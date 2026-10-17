# PHM Misc #8 — pusheen.txt (40 pts) · AC

[← 返回索引](../README.md)

## 題目

> Do you think pusheen is cute?

檔案: [`/static/pusheen.txt.xz`](https://ctf.hackme.quest/static/pusheen.txt.xz)

## 解法

解壓 `pusheen.txt.xz` 得到 **6527 行**的 ASCII 文字檔, 內容是**同一隻 Pusheen 貓的 ASCII art 畫 408 次**。每 16 行一隻 + 1 行空白當分隔。

掃幾隻就看出來: Pusheen 有**兩種版本**。一種是「乾淨」版 (只有 `▀ ▄ ▌ ▐ █` 這類粗體框塊字元), 另一種是「有填充」版 (身體裡填滿 `▒` 的網點)。

**408 / 8 = 51 bytes = 剛好可以塞一行 flag**. 把每隻 Pusheen 當 1 bit:

- 有 `▒` 填充 → `1`
- 沒 `▒` → `0`

```python
with open('pusheen.txt','r') as f:
    text = f.read()
lines = text.split('\n')
chunks = []
cur = []
for L in lines:
    if L.strip() == '':
        if cur: chunks.append(cur); cur = []
    else:
        cur.append(L)
if cur: chunks.append(cur)
bits = ''.join('1' if any('▒' in L for L in c) else '0' for c in chunks)
out = bytes(int(bits[i:i+8], 2) for i in range(0, len(bits), 8))
print(out)
```

輸出:

```
b'FLAG{Pusheen OIOOOIIOOIOOIIOOOIOOOOOIOIOOOIII Cute}'
```

**Flag**: `FLAG{Pusheen OIOOOIIOOIOOIIOOOIOOOOOIOIOOOIII Cute}`

直接提交就 AC。中間那段 `OIO...III` 看起來像第二層 binary (O=0, I=1), 但 server 收的是這個原封字串。

## 收斂

單一字元 `▒` 當作 1 bit 的視覺 steganography. 數量 408 剛好是 51 bytes 這個細節讓「每隻一個 bit」的切法很自然。
