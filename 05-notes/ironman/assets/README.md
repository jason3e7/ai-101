# 鐵人賽封面圖 — Article Covers

每篇文章的封面圖（1200×630，符合社群分享的標準比例）。

## 資料夾結構 — Layout

每 5 天一組（`day01-05/`、`day06-10/` … `day26-30/`），每組放這 5 天的 `cover-dayNN.html`（原始檔）與 `dayNN-cover.png`（成品）。

| 位置 | 放什麼 |
|:---|:---|
| 本層 | `cover-template.html`（範本）、本說明 |
| `dayNN-NN/` | 該組天數的封面 html ＋ png（成對，每組 ≤ 10 個檔案） |
| `unused/` | 沒採用的方案（Day 01 立體派風格） |

## 怎麼產生下一張

1. 複製 `cover-template.html` 到對應的組資料夾（例：Day 16 → `day16-20/cover-day16.html`），改四個地方：
   - `.day` → `Day 16`
   - `<h1>` → 標題（想強調的字包在 `<em>` 裡會變成金色）
   - `.sub` → 副標一句話
   - 中間的視覺區塊 → 換成當篇的主視覺
2. 進到該組資料夾，用無頭 Chrome 截圖：

```bash
cd day16-20
google-chrome --headless --disable-gpu --no-sandbox --hide-scrollbars \
  --window-size=1200,630 --screenshot=day16-cover.png cover-day16.html
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
3. 底部 - 縱軸：相當於人做多久的工作　|　資料：METR

超過三句，封面就變成投影片了。

> [!TIP]
> **有引用數據的圖，一定要標資料來源。** 併在底部那一行後面（`　|　資料：XXX`），不另外佔一句。出處是可信度的一部分，省掉它省錯地方。

**每篇只換內容，不換配色與版型** - 三十天下來讀者會認得這個系列。
