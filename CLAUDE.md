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

## 呈現平台原則

**GitHub 為主，Obsidian 為輔。** 語法一律選「兩邊都正常顯示」的共同子集；衝突時以 GitHub 渲染結果為準。這影響下面的 callout 與連結規則。

---

## 寫作風格指南

### 結構

- 開頭用 callout（`> [!NOTE]` 或 `> [!TIP]`）一句話說明「這是什麼」
- 必備區塊順序：**是什麼 → 安裝 → 基本使用 → 進階 → 常見問題 → Sources**
- 基礎在前，進階在後。不要把初學者卡在進階細節
- 觀念／框架類筆記（非工具）可調整中段結構，但「開頭一句話 + 結尾 Sources」不變

### 語言：中英雙讀者友善（L2）

目標：**繁中讀者順讀，英文讀者也能導覽、抓重點、grep 到關鍵字。** 具體做法：

- **標題雙語**：主標與各 `##` 小標用「中文 — English」格式
  （例：`# AI 能力全景圖 — The AI Capability Landscape`、`## 軸一：資訊流向 (Axis 1: Information Flow)`）
- **英文 TL;DR**：開頭 callout 下方加一行 `> **TL;DR (EN):** ...` 一句話英文摘要
- **保留所有英文術語**：技術名詞第一次出現時中英並列（例：`收斂思考（Convergent Thinking）`），之後可只用其一
- 內文主體維持繁體中文，口語但精確，避免翻譯腔
- 避免冗長的前言和結語
- **不做全文雙語**：內文不逐句翻譯，維護成本太高

### Code 範例

- 每個指令都能直接複製貼上執行，不要用 `...` 省略
- 預設值、推薦值直接寫在範例裡
- 指令前若需要前提條件，用註解說明

### Callout 使用規則（GitHub 原生 alert 語法）

只用 GitHub 支援的 **5 種、大寫**類型，兩邊都渲染：

- `> [!NOTE]` — 補充資訊、背景知識（取代舊的 `[!info]`）
- `> [!TIP]` — 建議、最佳實踐
- `> [!IMPORTANT]` — 必讀重點（GitHub 完美，Obsidian 退化為預設樣式）
- `> [!WARNING]` — 常見踩坑、容易搞錯的地方
- `> [!CAUTION]` — 高風險、破壞性操作（GitHub 完美，Obsidian 退化）

引用名言改用**一般 `>` 引用區塊**（GitHub 沒有 quote alert，舊的 `[!quote]` 不渲染）。

### 內部連結規則（標準相對連結）

- **不用 `[[wiki-link]]`**（GitHub 上點不動）。改用標準相對連結：
  `[顯示文字](./02-進階觀念/AI%20101%20-%20xxx.md)`
- 路徑中的空格一律編碼成 `%20`
- 新筆記一律用相對連結；舊筆記的 `[[ ]]` 隨編輯到時順手改（不做一次性大批改）
- 索引頁（README、主頁）已全面改為相對連結，必須維持

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

## 檔案與目錄（英文命名）

**檔名與資料夾名一律用英文 kebab-case（全小寫、連字號）。** 檔案內的 `# 標題` 維持中英雙語（L2），只有檔名/路徑是英文。

- **檔名格式**：`主題-slug.md`（全小寫、連字號、**不加** `ai-101-` 前綴、無空格）
  - 例：`core-concepts.md`、`claude-code-ecosystem.md`、`ai-capability-landscape.md`
- **資料夾**：`01-fundamentals` / `02-advanced` / `03-tools` / `04-local-llm` / `05-notes` / `06-external` / `skills`
- **索引檔**：`index.md`（repo 根目錄；未來架 MkDocs 剛好是首頁）
- 索引按「**學習路徑**」分類，不要扁平化平行列表
- **遷移狀態**：01–05 已改英文名；`06-external` 的 64 篇檔名仍為舊格式，待單獨一輪處理

---

## 新增筆記的 SOP

1. **研究**：web search + web fetch，確保資訊是最新的（注意今年）
2. **撰寫**：符合上述風格指南
3. **補 Sources**：底部列出參考來源
4. **更新索引**：把新筆記歸到 `index.md` 合適的分類下
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
