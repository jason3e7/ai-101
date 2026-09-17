# 上稿版本 — iThome-Ready Drafts

`drafts/` 是 repo 版本（GitHub 語法）；這裡是**貼到 iThome 用的版本**。

## 兩者的差別

iThome 編輯器採 Markdown 並明示「請勿使用 HTML Code」，而且**不支援 GitHub 專屬語法**。上稿前要做這幾件轉換：

| repo 版 | iThome 版 | 原因 |
|---|---|---|
| YAML frontmatter | 刪掉 | iThome 沒有這個欄位 |
| `# 標題` H1 | 刪掉 | 標題另有欄位，正文從 `##` 開始 |
| `[← 回主頁](...)` 導覽列 | 刪掉 | 相對連結在站外無效 |
| `> [!NOTE]` / `> [!TIP]` 等 | 改成一般 `>` 引用，必要時加 `**粗體標籤：**` | GitHub 專屬語法，iThome 會顯示成字面文字 |
| 指向其他草稿的相對連結 | 改成純文字（例如「Day 03」） | 文章還沒發，連結是死的 |
| `Day 19–21` 之類的天數對照 | 視情況簡化 | 排程可能還會動，寫死容易打臉 |
| ` ```text ` | ` ``` ` | iThome 不需要指定語言也能顯示 |
| `## Sources` | 看情況保留 | Day 01 實際保留了 `Sources` |
| `（jason3e7 的觀點／直覺）` | **原樣保留** | 冠名規則在 iThome 一樣適用。作者欄雖然已經是本人，但冠名能讓讀者一眼分辨「這句是我的主觀判斷」還是「這是有依據的事實」——這個區別比省幾個字重要 |

> [!WARNING]
> 這個資料夾是**線上快照**，必須跟 iThome 上看到的一字不差。標了 `[fixButNotPublish]` 的修正**不要進這裡**，只改 `drafts/`，並登記到 [fix-log.md](../fix-log.md)。

## 文章標題格式

```
[Day 01] 它只是在猜下一個字：LLM 的原理，決定了後面 29 天的所有心法
```

`[Day NN] ` 前綴 ＋ 半形空格 ＋ 標題本文。系列名稱由 iThome 另外帶，標題不用重複。

## 圖表限制

**iThome 沒有 mermaid 渲染引擎。** 文章頁只載入 `highlight.js`，` ```mermaid ` 區塊會原封不動顯示成原始碼。要放圖只有兩條路：

1. 用純文字樹狀圖（縮排式、不靠欄位對齊，中英數混排不會跑版）
2. 在 [mermaid.live](https://mermaid.live) 產圖後匯出 PNG，再用編輯器的圖片鈕上傳

> [!NOTE]
> 系列決定**不做每篇心智圖**（2026-09-16）。這裡保留的是格式限制本身，之後真要放圖時才不用重新查一次。

## 上稿檢查清單

- [ ] 全文 > 300 中文字
- [ ] 引用他人內容未超過全文 1/3
- [ ] 沒有殘留的 `[!NOTE]` 等 GitHub alert
- [ ] 沒有殘留的相對連結
- [ ] 封面圖已上傳（`../assets/dayNN-cover.png`）
- [ ] 文末預告下一篇
- [ ] 用「老嫗能解」再讀一遍：有沒有繞著講、可以換成具體比喻的句子

## 已發布

| Day | 連結 |
|---|---|
| 01 | <https://ithelp.ithome.com.tw/articles/10411345> |
| 02 | <https://ithelp.ithome.com.tw/articles/10411919> |
| 03 | <https://ithelp.ithome.com.tw/articles/10412787> |
