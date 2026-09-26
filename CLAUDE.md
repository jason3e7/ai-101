# AI 101 筆記專案 — Project Guide

這個 repo 是一份關於「聰明使用 AI」的持續成長知識庫。
未來所有新增、修改內容都必須遵守以下原則。

---

## 最高原則 — Prime Directive

**有清楚概念，又能快速上手。**

每一份筆記都要同時滿足兩件事：

1. **清楚概念** — 初學者讀完能理解「這是什麼、為什麼重要」
2. **快速上手** — 能立刻照著做，不需要再查其他資料

如果一份筆記只能滿足其中一件，就不該合格。

---

## 呈現平台原則 — Rendering Platform

**GitHub 為主，Obsidian 為輔。** 語法一律選「兩邊都正常顯示」的共同子集；衝突時以 GitHub 渲染結果為準。這影響下面的 callout 與連結規則。

---

## 寫作風格指南 — Writing Style Guide

### 結構

- 開頭用 callout（`> [!NOTE]` 或 `> [!TIP]`）一句話說明「這是什麼」
- 必備區塊順序：**是什麼 → 心智清單 → 安裝 → 基本使用 → 進階 → 常見問題 → Sources**
- 基礎在前，進階在後。不要把初學者卡在進階細節
- 觀念／框架類筆記（非工具）可調整中段結構，但「開頭一句話 + 結尾 Sources」不變
- **標題數量上限（注意力原則）**：每篇一個 `#` H1（文章大標題）；`##` 主章節 **≤ 7、理想 3**；`###` 等更深層每一層也適用同一上限。超過就依 [[refactor-note]] 分組或合併。

### 心智清單 — Mind-Map Checklist

每篇 note 在**開頭 callout + TL;DR 之後、文章本體之前**放一張心智清單，讓讀者一眼看完全篇脈絡。

**放置格式**：包在一個 ` ```markdown ` code block 裡（不是渲染成清單，是等寬框）。原因是根節點要用 `#`（markmap 的中心），放進 code block 後那個 `#` 只是字面文字，不會撞到「每篇一個 H1」的規則。

**內容規則**：

- 第一行 `# 根節點`（一句話，全篇主旨）
- 分支用 `*`（不用 `-`），巢狀縮排
- 一個根 · 深度 ≤ 4 · 每層 ≤ 7 · 節點是短語
- 不加粗體標籤（根節點本身就是「看全篇」那句）
- code block 內是字面文字，別用行內程式碼語法（例如寫 `(?)` 而非 `` `(?)` ``）

**產生方式（用哪個 skill）**：詳細判斷邏輯見 [`skills/working-style.md`](./skills/working-style.md) 的「心智清單: skill 選擇」段。簡版：材料已足要收斂 → `condense-mindmap`；材料少要生成 → `expand-mindmap`。判斷不出來看字數（≤ 500 字 expand、> 500 字 condense）。

### 語言：中英雙讀者友善（Bilingual, L2）＋老嫗能解

目標：**繁中讀者順讀，英文讀者也能導覽、抓重點、grep 到關鍵字。** 具體做法：

- **老嫗能解**：唐代詩人白居易主張「老嫗能解」——寫完唸給老太太聽，聽不懂就改。筆記文字要淺白到非專業讀者也能讀懂：能用日常詞就別用專業術語、非用不可時附解釋、避免翻譯腔與艱澀句子。
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

### Callout 使用規則（GitHub Alerts）

只用 GitHub 支援的 **5 種、大寫**類型，兩邊都渲染：

- `> [!NOTE]` — 補充資訊、背景知識（取代舊的 `[!info]`）
- `> [!TIP]` — 建議、最佳實踐
- `> [!IMPORTANT]` — 必讀重點（GitHub 完美，Obsidian 退化為預設樣式）
- `> [!WARNING]` — 常見踩坑、容易搞錯的地方
- `> [!CAUTION]` — 高風險、破壞性操作（GitHub 完美，Obsidian 退化）

引用名言改用**一般 `>` 引用區塊**（GitHub 沒有 quote alert，舊的 `[!quote]` 不渲染）。

### 內部連結規則（Relative Links）

- **不用 `[[wiki-link]]`**（GitHub 上點不動）。改用標準相對連結：
  `[顯示文字](../02-advanced/context-engineering.md)`（跨資料夾用 `../`）
- 英文 kebab-case 檔名已無空格；若連到 `06-external` 舊檔（仍含空格），空格編碼成 `%20`
- 新筆記一律用相對連結；舊筆記的 `[[ ]]` 隨編輯到時順手改（不做一次性大批改）
- 索引頁（README、index.md）已全面改為相對連結，必須維持

---

## Sources 是必要區塊 — Sources Are Required

每份筆記的最後必須有 `## Sources` 區塊，列出研究時參考的來源。

格式：

```markdown
## Sources

- [文章標題](https://example.com/url)
- [另一個來源](https://example.com/url2)
```

沒有外部來源的純整理類筆記可以省略，但有做 web search 的筆記一定要有。

---

## 檔案與目錄（English Naming）

**檔名與資料夾名一律用英文 kebab-case（全小寫、連字號）。** 檔案內的 `# 標題` 維持中英雙語（L2），只有檔名/路徑是英文。

### 檔名格式

- **研究筆記**（01–05 資料夾）：`主題-slug.md`（全小寫、連字號、**不加** `ai-101-` 前綴、無空格）
  - 例：`core-concepts.md`、`claude-code-ecosystem.md`、`ai-capability-landscape.md`
- **外部觀點收錄**（`06-external/`）：`yyyymmdd_標題 — 作者.md`
  - 日期用**當天**（用戶請我加入這篇的當天），不是原文發布日
  - 標題用繁體中文，作者用原名

### 資料夾對應（哪類筆記放哪）

- `01-fundamentals/` → 基礎知識（核心概念、模型比較、實用技巧）
  - `claude-code/` → Claude Code 專用主題（`/goal`、hooks、permissions、workflow 等）
- `02-advanced/` → 進階思維（Context / Harness / Loop Engineering、Subagent、驗證方法論）
- `03-tools/` → 可安裝使用的工具筆記
  - `security/` → 資安工具
  - `agents-platforms/` → 模型平台 / 個人 agent
- `04-local-llm/` → 本地模型（Ollama、vLLM、Gemma、輕量模型）
- `05-notes/` → 個人實驗、隨筆、實測、進行中設計稿
- `06-external/` → 外部文章、貼文、研究的收錄
  - `reference/` → 次要、時效性強、轉述性的外部觀點
- `skills/` → Claude Code Skill（工作模式與判斷邏輯）

> 子資料夾依 `refactor-note` 原則建立（單層 ≤ 10、理想 7）。放筆記前先看該層是否已接近上限。

### 索引與遷移狀態

- **索引檔**：`index.md`（repo 根目錄；未來架 MkDocs 剛好是首頁）
- 索引按「**學習路徑**」分類，不要扁平化平行列表
- **遷移狀態**：01–05 已改英文名；`06-external` 的 64 篇檔名仍為舊格式，待單獨一輪處理

---

## 新增筆記的 SOP — New Note Workflow

1. **研究**：web search + web fetch，確保資訊是最新的（注意今年）
2. **撰寫**：符合上述風格指南
3. **補 Sources**：底部列出參考來源
4. **更新索引**：把新筆記歸到 `index.md` 合適的分類下
5. **Commit & Push**：用有 `Co-Authored-By: Claude` 的 commit message

---

## Git 操作 — Git Workflow

Push 使用 deploy key：

```bash
GIT_SSH_COMMAND="ssh -i ~/.ssh/ai-101-deploy" git push
```

Commit message 格式（需包含 Co-Authored-By）：

```bash
git commit -m "$(cat <<'EOF'
簡短的變更描述

Co-Authored-By: Claude <當前型號> <noreply@anthropic.com>
EOF
)"
```

`<當前型號>` 填**這個 session 實際使用的模型**（例：`Sonnet 4.6`、`Opus 4.8`），不要寫死。
