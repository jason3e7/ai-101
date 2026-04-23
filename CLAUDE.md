# AI 101 筆記專案

這個 repo 是一份關於「聰明使用 AI」的持續成長知識庫。
未來所有新增、修改內容都必須遵守以下原則。

---

## 最高原則

**有清楚概念，又能快速上手。**

每一份筆記都要同時滿足兩件事：

1. **清楚概念** — 初學者讀完能理解「這是什麼、為什麼重要」
2. **快速上手** — 能立刻照著做，不需要再查其他資料

如果一份筆記只能滿足其中一件，就不該合格。

---

## 寫作風格指南

### 結構

- 開頭用 callout（`> [!info]` 或 `> [!tip]`）一句話說明「這是什麼」
- 必備區塊順序：**是什麼 → 安裝 → 基本使用 → 進階 → 常見問題 → Sources**
- 基礎在前，進階在後。不要把初學者卡在進階細節

### 語言

- 繁體中文為主
- 口語但精確，避免翻譯腔
- 技術名詞第一次出現時附英文原文（例：`Context Engineering（上下文工程）`）
- 避免冗長的前言和結語

### Code 範例

- 每個指令都能直接複製貼上執行，不要用 `...` 省略
- 預設值、推薦值直接寫在範例裡
- 指令前若需要前提條件，用註解說明

### Callout 使用規則

- `> [!tip]` — 建議、最佳實踐
- `> [!warning]` — 常見踩坑、容易搞錯的地方
- `> [!info]` — 補充資訊、背景知識
- `> [!quote]` — 引用名言、重要觀點

---

## Sources 是必要區塊

每份筆記的最後必須有 `## Sources` 區塊，列出研究時參考的來源。

格式：

```markdown
## Sources

- [文章標題](https://example.com/url)
- [另一個來源](https://example.com/url2)
```

沒有外部來源的純整理類筆記可以省略，但有做 web search 的筆記一定要有。

---

## 檔案與目錄

- 所有筆記放在 repo 根目錄
- 檔名格式：`AI 101 - 主題.md`
- 索引檔：`AI 101 - 主頁.md`
- 索引按「**學習路徑**」分類，不要扁平化平行列表

---

## 新增筆記的 SOP

1. **研究**：web search + web fetch，確保資訊是最新的（注意今年）
2. **撰寫**：符合上述風格指南
3. **補 Sources**：底部列出參考來源
4. **更新主頁**：把新筆記歸到合適的分類下
5. **Commit & Push**：用有 `Co-Authored-By: Claude` 的 commit message

---

## Git 操作

Push 使用 deploy key：

```bash
GIT_SSH_COMMAND="ssh -i ~/.ssh/ai-101-deploy" git push
```

Commit message 格式（需包含 Co-Authored-By）：

```bash
git commit -m "$(cat <<'EOF'
簡短的變更描述

Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>
EOF
)"
```
