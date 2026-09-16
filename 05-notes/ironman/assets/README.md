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

**每篇只換內容，不換配色與版型** - 三十天下來讀者會認得這個系列。
