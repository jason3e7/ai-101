---
name: working-style
description: jason3e7 在 ai-101 知識庫的完整工作模式與協作偏好，供新 session 快速對齊
tags: [meta, workflow, onboarding]
---

# working-style

這份 skill 描述 ai-101 的擁有者（jason3e7）的工作模式，讓新的 Claude session 不需要重新摸索。

---

## 知識庫結構 — Repo Structure

```
ai-101/
├── 01-fundamentals/  → 基礎知識（Claude Code 生態系、核心概念、模型比較、實用技巧）
├── 02-advanced/      → 進階思維（Context Engineering、Harness Engineering、Subagent）
├── 03-tools/         → 可安裝使用的工具筆記（OpenClaw、Hermes、OpenCode 等）
├── 04-local-llm/     → 本地模型（Ollama、vLLM、Gemma、輕量模型）
├── 05-notes/         → 個人實驗與隨筆
├── 06-external/      → 外部文章、貼文、研究的筆記（資料夾已英文化；檔名待處理）
│   └── reference/    → 次要、時效性強、轉述性的外部觀點
├── skills/           → 工作模式與判斷邏輯的可重用 skill
├── index.md          → 知識庫索引，有學習路徑分類
├── README.md         → GitHub 首頁
└── CLAUDE.md         → 專案層級指示

命名：檔名/資料夾一律英文 kebab-case（全小寫、連字號、無空格、無 AI 101- 前綴）。
      檔案內 # 標題維持中英雙語。01–05 已英文化；06-external 待處理。
```

---

## 兩種工作模式 — Two Working Modes

### 模式一：研究筆記（Research Notes, 01–05）

用戶指定一個主題，Claude 研究後寫成英文 kebab-case 檔名的 `.md`。

**命名規則**：`主題-slug.md`（全小寫、連字號、無 `AI 101 -` 前綴；檔內 `#` 標題仍中英雙語）

**筆記結構**（按此順序）：
1. 是什麼（一句話 + callout）
2. 安裝
3. 基本使用
4. 進階
5. 常見問題
6. Sources

**放置規則**：
- 基礎概念 → `01-fundamentals/`
- 進階思維框架 → `02-advanced/`
- 可安裝工具 → `03-tools/`
- 本地模型 → `04-local-llm/`
- 個人實驗 → `05-notes/`

**完成後必須更新 `index.md`**，把新筆記加到對應分類的表格。

---

### 模式二：外部觀點收錄（External Views, 06）

用戶給一到多個 URL，每個 URL 一篇 MD，放入 `06-external/`。

**命名規則**：`yyyymmdd_標題 — 作者.md`
- 日期用**用戶請我加入的當天**，不是原文發布日
- 標題用繁體中文，作者用原名

**筆記結構**：
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

（主體內容）

## 相關筆記
- [[...]]

## 來源
- 原文：[...]
```

**分層判斷**：用 `curate-notes` skill。簡要版：
- 根目錄：第一手來源、具體數字、直接改變工作方式、持久參考價值
- `reference/`：轉述他人研究、工具介紹、新聞、短效觀察

---

## 各平台抓內容策略 — Content Fetching by Platform

有無 Chrome 能力差異很大，分開列：

### 有 Chrome（claude --chrome）

| 來源 | 處理方式 | 工具 |
|---|---|---|
| Facebook 動態牆貼文 | ✅ 直接 navigate + `read_page`，accessibility tree 可讀到貼文文字與 hashtag；注意 tree 雜亂，`get_page_text` 效果較差 | `navigate` → `read_page` |
| Facebook 粉專 / 公開頁面 | ✅ 已登入狀態可讀公開內容；私人貼文視隱私設定 | `navigate` → `get_page_text` |
| Facebook 社群（groups/）| ✅ 已加入的社群可讀；未加入仍擋 | `navigate` → `read_page` |
| YouTube | ✅ 等 ~7 秒讓 JS 渲染完畢，`get_page_text` 可讀標題、描述、頻道、相關影片；`read_page` 在 YouTube 有時回傳空值，優先用 `get_page_text` | `navigate` → wait 7s → `get_page_text` |
| iThome | ✅ Chrome 直接繞過 403，`get_page_text` 取 `<article>` 全文 | `navigate` → `get_page_text` |
| Medium 公開文章 | ✅ 可讀，但未登入 Medium 帳號時 member-only 仍截斷；用戶若有 Medium 帳號且已在瀏覽器登入則可讀完整 | `navigate` → `get_page_text` |
| GitHub repo | ✅ 可讀，但 `gh` CLI 仍是首選（有 auth token、速度快、支援 API）| `gh` CLI 優先，Chrome 備用 |
| Gemini Share | 未驗證，待測試 | — |

### 無 Chrome（純 WebFetch / WebSearch）

| 來源 | 處理方式 |
|---|---|
| Facebook 公開貼文 / 粉專 | WebFetch 幾乎都被擋，請用戶貼文字 |
| Facebook 社群（groups/）| 必被擋，請用戶貼文字 |
| YouTube | oEmbed API 取 metadata，再 WebSearch 補細節 |
| GitHub repo | WebFetch README；目錄結構用 Bash `ls` 或 `gh` CLI |
| iThome | 403，用 WebSearch 補標題與摘要 |
| Medium | 注意 302 redirect（medium.com → author.medium.com），需再 fetch 一次 |
| Gemini Share | 被擋，請用戶貼文字 |

---

## Git 工作流 — Git Workflow

```bash
# Push 固定用 deploy key
GIT_SSH_COMMAND="ssh -i ~/.ssh/ai-101-deploy" git push

# 每個 commit 必須包含 Co-Authored-By
git commit -m "$(cat <<'EOF'
簡短描述

Co-Authored-By: Claude Sonnet 4.6 <noreply@anthropic.com>
EOF
)"

# 移動檔案保留 git history
git mv "舊路徑/檔案.md" "新路徑/"
```

---

## Skills 建立邏輯 — Building Skills

Skills 放在 `skills/` 資料夾，commit + push 進 repo。

**是什麼**：從用戶的選擇或行為模式中萃取出可重用的判斷邏輯。

**不是什麼**：
- 不是把某篇筆記的內容變成 skill
- 不是把某個任務的執行步驟記下來

當用戶說「把這個寫成 skill」，先確認你萃取的是**決策邏輯**，不是**內容摘要**。

---

## 呈現與語言規範（Presentation & Language, GitHub-first）

視覺呈現以 **GitHub 為主、Obsidian 為輔**；語言採 **中英雙讀者友善（L2）**。完整規則在 `CLAUDE.md`，重點：

| 項目 | 規則 |
|---|---|
| **Callout** | 只用 GitHub 5 種大寫 alert：`[!NOTE]` `[!TIP]` `[!IMPORTANT]` `[!WARNING]` `[!CAUTION]`。引用名言用一般 `>`。舊的 `[!info]`→`[!NOTE]`、`[!quote]`→`>` |
| **內部連結** | 標準相對連結 `[文字](./資料夾/檔名.md)`，空格編碼 `%20`。**不用 `[[ ]]`**（GitHub 點不動） |
| **雙語 L2** | 標題「中文 — English」、開頭加一行 `> **TL;DR (EN):**`、術語中英並列；內文主體維持繁中，不逐句雙語 |
| **遷移策略** | 新筆記照新規；舊筆記的 callout / `[[ ]]` **隨編輯到時順手改**，不做一次性大批改。索引頁（README、index.md）已全面改為相對連結 |

---

## 溝通偏好 — Communication Preferences

- **回應要短**，不要結尾摘要（用戶可以自己讀）
- **繁體中文**為主，技術名詞首次出現附英文
- 遇到多個 URL，**能平行處理的就平行抓**，不要一個一個來
- 用戶說「試試看」或「你幫我決定」時，先**提出方案讓用戶確認**，不要直接執行大範圍操作
- **Commit 前不需要問**；push 前如有破壞性操作要確認
- 遇到問題**直接說**，不要猜測後悄悄跳過
