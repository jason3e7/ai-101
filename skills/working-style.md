---
name: working-style
description: jason3e7 在 ai-101 知識庫的工作模式與協作偏好，供新 session 快速對齊
tags: [meta, workflow, onboarding]
---

# working-style

這份 skill 描述這個 repo 的擁有者（jason3e7）的工作模式，讓新的 Claude session 不需要重新摸索。

---

## 主要工作：外部觀點筆記收錄

用戶會給一到多個 URL，要求加入 `06-外部觀點/`，每個 URL 一篇 MD。

### 標準流程

1. **抓內容**：用 WebFetch 取得原文，遇到登入牆就請用戶貼文字
2. **寫筆記**：Obsidian 格式 MD，放入 `06-外部觀點/`
3. **命名規則**：`yyyymmdd_標題 — 作者.md`，日期用原文發布日（非今天）
4. **放置位置**：用 `curate-notes` skill 判斷放根目錄還是 `參考/`
5. **Commit + Push**：
   ```bash
   git commit -m "..."
   GIT_SSH_COMMAND="ssh -i ~/.ssh/ai-101-deploy" git push
   ```
6. **Co-authored-by**：每個 commit 必須包含：
   ```
   Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>
   ```

### 筆記格式

```markdown
---
title: "..."
tags: [ai, 外部觀點, ...]
source: https://...
author: 作者名
created: yyyy-mm-dd
---

# 標題

> [!info]
> 原文：[連結文字](url)
> **一句話：** 這篇在說什麼

## 區塊...

## 來源
- 原文：[...]
```

### 各平台處理方式

| 來源 | 處理方式 |
|---|---|
| Facebook 公開貼文 / 粉專 | WebFetch 通常被擋，請用戶貼內容 |
| Facebook 社群（groups/）| 必被擋，請用戶貼內容 |
| YouTube | 用 oEmbed API 取 metadata，再搜尋補充細節 |
| GitHub | WebFetch README，或用 `gh` CLI |
| iThome | 403，用 WebSearch 補標題與摘要 |
| Medium | 注意 redirect（medium.com → author.medium.com）|
| Gemini Share | 被擋，請用戶貼內容 |

---

## 知識庫結構偏好

- 語言：**繁體中文**
- 筆記風格：口語但精確，技術名詞首次出現附英文
- Wiki-links：`[[筆記標題]]` 連到相關筆記
- Callout：`> [!tip]`、`> [!warning]`、`> [!info]`、`> [!quote]`
- 不寫結尾摘要（用戶可以自己讀）

---

## 筆記分層標準

見 `skills/curate-notes.md`。簡要版：

- **根目錄**：第一手來源、有具體數字、直接改變工作方式、持久參考價值
- **`參考/`**：轉述研究、工具介紹、新聞、短效觀察

---

## Skills 的建立邏輯

用戶會從實際工作中觀察出模式，要求 Claude 把模式寫成 skill。

- **不是**把某篇筆記的內容變成 skill
- **而是**從用戶的選擇或行為模式中，萃取出可重複使用的判斷邏輯
- Skill 放在 `skills/` 資料夾，commit + push

當用戶說「把這個寫成 skill」，先確認你萃取的是**決策邏輯**，不是**內容摘要**。

---

## 溝通偏好

- 回應要短，不要結尾摘要
- 不確定的事直接問，不要自己猜然後做錯
- 遇到多個 URL，能平行處理的就一起抓，不要一個一個來
- 用戶說「試試看」時，先提出方案讓用戶確認，不要直接執行大範圍操作
- Commit 前不需要問，但 push 前如果有破壞性操作要確認

---

## 常用指令備忘

```bash
# Push（必須用 deploy key）
GIT_SSH_COMMAND="ssh -i ~/.ssh/ai-101-deploy" git push

# 移動檔案到子資料夾
git mv "舊路徑/檔案.md" "新路徑/"
```
