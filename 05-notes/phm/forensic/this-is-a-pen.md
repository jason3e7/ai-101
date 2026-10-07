# PHM #101 Forensic — this is a pen

| 項目 | 值 |
|:---|:---|
| 類別 | Forensic |
| 分數 | 80 |
| 解題數 | 129 |
| 附檔 | [`/static/this-is-a-pen.pdf`](https://ctf.hackme.quest/static/this-is-a-pen.pdf) |

## 題目

> this is a pen

Keynote 匯出的 PDF, 2 頁. 可見內容:

- 頁 1: `Hack me Please / Are you cool enough to hack me?`
- 頁 2: `This is a pen / But I don't have apple or pineapple :(` (PPAP 梗)

## 分析

Content stream 掃 `FLAG` 沒命中 — 跟 easy pdf 不同套路. 看 `pdfimages -list` 結果:

```
page   num  type   width height
   1     0 image    1024   768
   2     1 image    1024   768      # 背景
   2     2 image     640   400      # <<< 可疑
   2     3 smask     640   400
   2     4 image     620   387      # 上層 (PPAP 貼紙)
   2     5 image     821   478
   2     6 smask     821   478
   2     7 image     140    42      # 膠帶裝飾
   2     8 smask     140    42
```

頁 2 有 **多張重疊的圖片**. 題名 "image behind image" 的提示很明顯 — 把每張圖存出來看一遍, 看 flag 在不在底層被蓋住的那張.

## 解法

```bash
pdfimages -all this-is-a-pen.pdf img
```

展出來的 `img-002.png` (640x400) 就是 flag 圖:

> `FLAG{Image behind another image. LoL}`

這張在 Keynote 原稿裡位於下層, 被 `img-004` (PPAP 貼紙圖) 完全蓋住, 肉眼看不到.

## Flag

```
FLAG{Image behind another image. LoL}
```

## 感想

跟 easy pdf 是對照組:

- easy pdf — flag 是**文字**, 藏在 content stream 的座標外 / 字距打散
- this is a pen — flag 是**圖**, 藏在 PDF 物件列表的下層 image, 被上層蓋住

共同點: 可見的渲染結果看不到, 但 PDF 容器本身保留了完整物件. 工具選對 (`pdftotext` vs `pdfimages`) 就能把隱藏物件拉出來.
