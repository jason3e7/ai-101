# PHM #3 Misc — television

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 50 |
| 解題數 | 930 |
| 附檔 | [`/static/television.bmp`](https://ctf.hackme.quest/static/television.bmp) |

## 題目

> Looks like my television was broken

一張 120x90 的 32-bit BMP, 用圖片檢視器打開是純彩色雜訊 (壞掉的電視畫面).

## 分析

視覺上是雜訊, 但 flag 可能就埋在 pixel data 的某處. BMP 32bpp 的 pixel area 是 43200 bytes, 直接當 bytes 掃 `FLAG` 看看就知道.

## 解法

```python
with open('television.bmp','rb') as f: d = f.read()
pix = d[54:]  # BMP header 54 bytes
idx = pix.find(b'FLAG')
print(pix[idx:idx+50])
```

輸出:

```
b'FLAG{PuRe_R@ND0M_DaTa_Fr0M/D3V/UR@ND0M}\x10R\x08\xda...'
```

Flag 自爆位置在 pixel offset 42048. 整張圖根本是 `/dev/urandom` 吐出來的隨機 bytes, flag 直接夾在中間.

## Flag

```
FLAG{PuRe_R@ND0M_DaTa_Fr0M/D3V/UR@ND0M}
```

## 感想

- 看到「雜訊」別急著跑 stego 工具, 先用 `strings` 或 raw byte search 掃一遍 — 有時候 flag 就在明面, 只是被雜訊視覺遮蔽
- flag 內容本身就是答案的 meta 註解 (`PuRe_R@ND0M_DaTa_Fr0M/D3V/UR@ND0M`), 作者的小玩笑
