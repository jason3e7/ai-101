# 鐵人賽封面圖 — Article Covers

每篇文章的封面圖（1200×630，符合社群分享的標準比例）。

## 怎麼產生下一張

1. 複製 `cover-template.html`，改四個地方：
   - `.day` → `Day 02`
   - `<h1>` → 標題（想強調的字包在 `<em>` 裡會變成金色）
   - `.sub` → 副標一句話
   - 中間的視覺區塊 → 換成當篇的主視覺
2. 用無頭 Chrome 截圖：

```bash
google-chrome --headless --disable-gpu --no-sandbox --hide-scrollbars \
  --window-size=1200,630 --screenshot=day02-cover.png cover-day02.html
```

## 設計規則

| 項目 | 值 |
|---|---|
| 尺寸 | 1200 × 630 |
| 底色 | `#14161f` |
| 主文字 | `#f2f0ea` |
| 強調色 | `#e8a13c`（金） |
| 字體 | Noto Sans CJK TC／Noto Sans Mono CJK TC |
| 固定元素 | 左上角「AI 心法 ｜ Day NN」、右下角一句 tagline |

### 一張圖最多三句話

圖表內的標籤（座標說明、刻度、門檻線）不算。Day 02 的三句是：

1. 大標 - 不是它突然變強／是它跨過了你的門檻
2. 副標 - 能力一路在指數成長，你只是現在才有感
3. 底部 - 縱軸：它能自己做完多久的工作

超過三句，封面就變成投影片了。

**每篇只換內容，不換配色與版型** - 三十天下來讀者會認得這個系列。
