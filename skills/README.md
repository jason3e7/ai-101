---
title: AI 101 - Skills 索引
tags: [ai, skill, index, 索引]
created: 2026-09-26
---

# Skills 索引 — Skills Index

[← 回主頁](../index.md)

> [!NOTE]
> 這個資料夾放 ai-101 用的 skill（教 AI 怎麼做某件事的可重複叫用說明）。什麼是 skill、格式怎麼寫，見 [Agent Skills 說明](../01-fundamentals/agent-skills.md)。

## 啟用中 — Active

| Skill | 做什麼 | 格式 |
|:---|:---|:---|
| [condense-mindmap](./condense-mindmap/SKILL.md) | 一堆資料 → 濃縮成 markdown 巢狀清單心智圖（收斂、忠實度優先，每節點可追回原文） | SKILL.md（英） |
| [expand-mindmap](./expand-mindmap/SKILL.md) | 一點種子 → 研究後放大成心智圖（發散，推測節點用 `(?)` 標記） | SKILL.md（英） |
| [refactor-note](./refactor-note.md) | 重構筆記或資料夾時套「注意力上限」：每層 ≤ 7、硬上限 10，超過就分組 | 輕量（中） |
| [curate-notes](./curate-notes.md) | 判斷一篇外部筆記該放 `06-external/` 根目錄還是 `reference/` 子夾 | 輕量（中） |
| [working-style](./working-style.md) | jason3e7 在本庫的工作模式與協作偏好，供新 session 快速對齊 | 輕量（中） |

## 未啟用 — Draft

| Skill | 做什麼 | 狀態 |
|:---|:---|:---|
| [note-lifecycle](./tmp/note-lifecycle.md) | 判斷一篇筆記何時該刪、刪前的替代方案 | 草稿，放 `tmp/`，待啟用決定 |

## 兩種格式 — Two Formats

- **SKILL.md（可攜格式）**：資料夾 ＋ `SKILL.md`，`name`＋`description` frontmatter，跨 Claude apps／Code／API／SDK 通用。心智圖那兩個用這種，且**全英文**。
- **輕量（本庫慣例）**：單一 `.md`，同樣 `name`／`description`（可加 `tags`）frontmatter，本體是判斷準則與流程，主要給本庫維護時在對話裡叫用。

> [!TIP]
> 兩個心智圖 skill 是一組**收斂／發散**對照，共用同一套輸出契約（一個根、深度 ≤ 4、每層 ≤ 7、節點是短語），但忠實度心法相反。設計緣由見 [心智圖 skill 設計筆記](../05-notes/mindmap-skills-design.md)。
