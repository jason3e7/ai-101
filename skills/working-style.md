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
├── 01-fundamentals/  → 基礎知識
├── 02-advanced/      → 進階思維
├── 03-tools/         → 可安裝使用的工具
├── 04-local-llm/     → 本地模型
├── 05-notes/         → 個人實驗、隨筆、設計稿
├── 06-external/      → 外部文章、貼文、研究收錄
│   └── reference/    → 次要、時效性強、轉述性的外部觀點
├── skills/           → Claude Code Skill（工作模式與判斷邏輯）
├── index.md          → 知識庫索引，有學習路徑分類
├── README.md         → GitHub 首頁
└── CLAUDE.md         → 專案層級指示
```

**檔名 / 資料夾對應 / 子資料夾規則、命名格式**：完整見 [`CLAUDE.md`](../CLAUDE.md) 的「檔案與目錄」段。

---

## 兩種工作模式 — Two Working Modes

### 模式一：研究筆記（Research Notes, 01–05）

用戶指定一個主題，Claude 研究後寫成英文 kebab-case 檔名的 `.md`。

**檔名規則、資料夾對應**：見 [`CLAUDE.md`](../CLAUDE.md) 的「檔案與目錄」。

**筆記結構**（按此順序）：
1. 是什麼（一句話 + callout）＝ **引言**
2. **心智清單**（見下方 skill 選擇規則，夾在引言和本體之間）
3. 安裝
4. 基本使用
5. 進階
6. 常見問題
7. Sources

**心智清單: skill 選擇**（放置與內容格式規則見 [`CLAUDE.md`](../CLAUDE.md) 的「心智清單」段）

每篇 note 都在**引言（開頭 callout ＋ TL;DR）和文章本體之間**放一張心智清單。用哪個 skill，**依這兩步判斷（第一步優先）**：

1. **先看實際材料（優先）**：這篇 note 手上真正要處理的材料多不多？
   * **材料已足、要收斂** → [condense-mindmap](./condense-mindmap/SKILL.md)（濃縮，每節點可追回來源）。例：全文已寫完、已抓回一整篇外部文章、給了一疊筆記。
   * **材料很少、要生成** → [expand-mindmap](./expand-mindmap/SKILL.md)（研究後放大，推測節點標 `(?)`）。例：只有一個主題、一個問題當種子。
2. **判斷不出來時，才看 prompt 輸入量**：這次 prompt 給進來的內容 **≤ 500 字用 expand-mindmap，> 500 字用 condense-mindmap**。

（換句話說：字數只是「拿不準時的預設值」，真正的依據是這篇實際在收斂還是發散。）

**完成後必須更新 `index.md`**，把新筆記加到對應分類的表格。

---

### 模式二：外部觀點收錄（External Views, 06）

用戶給一到多個 URL，每個 URL 一篇 MD，放入 `06-external/`。

**檔名規則**：見 [`CLAUDE.md`](../CLAUDE.md) 的「檔案與目錄 → 檔名格式 → 外部觀點收錄」。

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

（心智清單：夾在這裡，引言和主體之間。模式二的實際材料是抓回的文章內容，不是那幾個 URL，所以固定用 [condense-mindmap](./condense-mindmap/SKILL.md) 濃縮全文，不套 500 字判斷。）

（主體內容）

## 相關筆記
* [[...]]

## 來源
* 原文：[...]
```

**分層判斷**：用 `curate-notes` skill。簡要版：
* 根目錄：第一手來源、具體數字、直接改變工作方式、持久參考價值
* `reference/`：轉述他人研究、工具介紹、新聞、短效觀察

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

**Deploy key、commit message 格式**：見 [`CLAUDE.md`](../CLAUDE.md) 的「Git 操作」段。

**Workflow-specific 額外規則**：

```bash
# 移動檔案保留 git history (CLAUDE.md 沒列, 但收整檔案時常用)
git mv "舊路徑/檔案.md" "新路徑/"
```

---

## Skills 建立邏輯 — Building Skills

Skills 放在 `skills/` 資料夾，commit + push 進 repo。

**是什麼**：從用戶的選擇或行為模式中萃取出可重用的判斷邏輯。

**不是什麼**：
* 不是把某篇筆記的內容變成 skill
* 不是把某個任務的執行步驟記下來

當用戶說「把這個寫成 skill」，先確認你萃取的是**決策邏輯**，不是**內容摘要**。

---

## 呈現與語言規範（Presentation & Language, GitHub-first）

視覺呈現以 **GitHub 為主、Obsidian 為輔**；語言採 **中英雙讀者友善（L2）**。

**完整規則**（Callout / 內部連結 / 雙語 L2 / 老嫗能解 / 標題上限 / 遷移策略）：見 [`CLAUDE.md`](../CLAUDE.md) 的「寫作風格指南」段。

---

## 溝通偏好 — Communication Preferences

* **用戶自己提出的觀點／想法，一律冠名 `jason3e7`**（不要只寫「jason」或「作者」）
* **回應要短**，不要結尾摘要（用戶可以自己讀）
* **繁體中文**為主，技術名詞首次出現附英文
* 遇到多個 URL，**能平行處理的就平行抓**，不要一個一個來
* 用戶說「試試看」或「你幫我決定」時，先**提出方案讓用戶確認**，不要直接執行大範圍操作
* **Commit 前不需要問**；push 前如有破壞性操作要確認
* 遇到問題**直接說**，不要猜測後悄悄跳過
