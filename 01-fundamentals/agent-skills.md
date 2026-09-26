---
title: AI 101 - Agent Skills 是什麼：跨平台的 SKILL.md 格式
tags: [ai, skill, skill-md, agent, 跨平台, 基礎]
created: 2026-09-26
---

# Agent Skills 是什麼 — What Is an Agent Skill

[← 回主頁](../index.md)

> [!NOTE]
> Skill 就是**一個資料夾，裡面放「教 AI 怎麼做某件事」的說明**，AI 判斷用得上時才把它讀進來。這篇講最基本、跨各平台都通用的那套格式（SKILL.md），不綁單一工具。看完你就能自己寫一個。

> **TL;DR (EN):** An Agent Skill is a folder whose core is a `SKILL.md` file: YAML frontmatter with just two required fields (`name`, `description`) plus a Markdown body of instructions. The same format is portable across Claude apps, Claude Code, the API, and the Agent SDK - and a cross-vendor spec now exists. It works by *progressive disclosure*: only the name+description sit in context always (~100 tokens); the full body loads only when the skill is triggered. So the `description` is the single most important line - it decides whether the skill ever fires.

Skill 不是外掛程式，也不是模型的一部分。它就是**一份寫好的流程說明，包成一個資料夾**，讓 AI 在遇到對的任務時自己讀進來照做：你不用改模型、不用寫程式串接，同一份還能跨平台用。它跟 prompt 的差別 - prompt 是你這次打的話，skill 是**存起來、可重複叫用**的一套做法。打個比方：prompt 是口頭交代一次；skill 是寫成一張 SOP 貼在牆上，需要時照著做。

---

## 最小格式：一個 SKILL.md — The Minimal Format

一個 skill 至少就是一個資料夾，裡面一個 `SKILL.md`：

```markdown
---
name: my-skill
description: 這個 skill 在做什麼，以及「什麼時候」該用它。
---

（這裡開始是給 AI 看的說明：步驟、範例、常見狀況……）
```

**frontmatter 只有兩個必填欄位：**

| 欄位 | 規則 |
|:---|:---|
| `name` | 全小寫、用連字號、少於 64 字元，**要跟資料夾同名** |
| `description` | 少於 1,024 字元，講清楚「做什麼」**和**「何時用」 |

選填的還有 `license`、`allowed-tools`、`metadata`、`compatibility`。**但只要 name 和 description 就能跑。**

> [!IMPORTANT]
> **`description` 是整個 skill 最重要的一行。** 因為它決定 AI 會不會在對的時機想到要用這個 skill（原因見下一節）。只寫「做什麼」不夠，一定要寫「**什麼時候**該用」 - 例如「處理 PDF 時使用」而不只是「處理 PDF」。

---

## 它怎麼運作：漸進式揭露 — Progressive Disclosure

Skill 省 context 的關鍵，是**分三層載入**，不是一次全丟進去：

| 層 | 什麼時候載入 | 大小 |
|:---|:---|:---|
| 1. name ＋ description | **每次對話開頭都在** | 每個 skill 約 100 tokens |
| 2. SKILL.md 全文 | **這個 skill 被觸發時**才載入 | 建議 < 5,000 tokens |
| 3. 參考檔案（腳本、範例、資料） | **真的用到時**才讀 | 不限，放在資料夾裡 |

這解釋了兩件事：

1. **為什麼 description 最重要** - 平常只有它在 AI 眼前，AI 靠它判斷「這題用不用得上這個 skill」。description 寫不好，後面寫得再漂亮也不會被叫到。
2. **為什麼 SKILL.md 本體要精簡** - 它一觸發就整份進 context，太長會排擠掉別的空間。細節、長範例、腳本，放到第三層的參考檔，需要才讀。

---

## 跨平台：同一份到處用 — Portable Across Platforms

同一個 SKILL.md 格式，目前可在這些地方用：

- **Claude 的 app**（Pro／Max／Team／Enterprise）
- **Claude Code**（透過 plugins / marketplace）
- **Claude API**（需開 code execution 的 beta）
- **Agent SDK**（自己蓋 agent 時）

而且它正在變成**跨廠商**的共通格式 - 已經有獨立的 spec（agentskills.io），OpenAI 與 Microsoft 的 agent 框架也各自跟進。所以學這套格式的投資，不會綁死在一家。

> [!NOTE]
> Agent Skills 由 Anthropic 在 **2025-10-16** 正式提出。在這之前，「skill」在 Claude Code 裡比較像「可呼叫的 `/指令`」（見 [Claude Code 生態系](./claude-code/ecosystem.md)）；SKILL.md 把它標準化成一個跨平台、可攜帶的檔案格式。

---

## 怎麼寫一個好的 — How to Write a Good One

四個要點：

1. **description 寫「做什麼 ＋ 何時用」** - 這是被叫到的前提。想像 AI 只看得到這一行，它判斷得出來嗎？
2. **本體寫成可照做的步驟** - 不是講理論，是「第一步做什麼、第二步做什麼」，附輸入／輸出範例、常見的坑。
3. **精簡在 5,000 tokens 內** - 超過的細節丟參考檔。本體是「怎麼做」的骨架，不是百科全書。
4. **一個 skill 只做一件事** - 範圍越清楚，description 越好寫，越容易在對的時機被叫到。範圍太雜，AI 反而不知道何時該用。

> [!TIP]
> 這個 repo 自己的 [`skills/`](../skills/) 資料夾就是活例子（[refactor-note](../skills/refactor-note.md)、[curate-notes](../skills/curate-notes.md)）。它們用的是輕量版：同樣的 `name` ＋ `description` frontmatter，本體是判斷準則與流程。想寫新的，照著抄一個最快。

---

## 常見問題 — FAQ

**Q：skill 跟 prompt、跟 MCP、跟 plugin 差在哪？**
skill 是**存起來、可重複叫用的一套做法**（行為層）；prompt 是你當下打的話；MCP 是接外部工具與資料的**連線**；plugin 是把 skill／hook／MCP 打包起來的**發佈單位**。四者的關係見 [Claude Code 生態系](./claude-code/ecosystem.md)。

**Q：一定要會寫程式嗎？**
不用。最基本的 skill 就是一個 markdown 檔，全是自然語言說明。要跑腳本才需要放程式碼（第三層參考檔）。

**Q：description 到底多重要？**
它幾乎決定一切。平常在 AI 眼前的只有 name ＋ description，本體要被觸發才載入。description 沒寫清楚「何時用」，skill 就等於不存在。

**Q：怎麼開始？**
複製 repo 裡任一個現成 skill，改 name／description，把本體換成你的流程。或用 `/skill-creator`（Claude Code 內建）幫你起草。

---

## 相關筆記 — Related

- [Claude Code 生態系](./claude-code/ecosystem.md) - skill／hook／MCP／plugin 四者的關係
- [兩個心智圖 skill 的設計筆記](../05-notes/mindmap-skills-design.md) - 一個實際在設計中的 skill 案例
- [refactor-note skill](../skills/refactor-note.md) - repo 自己的 skill 範例，可直接照抄格式

## Sources

- [Introducing Agent Skills — Anthropic, 2025-10-16](https://claude.com/blog/skills)
- [Agent Skills 工程細節 — Anthropic Engineering](https://www.anthropic.com/engineering/equipping-agents-for-the-real-world-with-agent-skills)
- [SKILL.md Format Specification — agentskills.io](https://agentskills.io/specification)
- [SKILL.md Format & Manifest Spec — The Prompt Index](https://www.thepromptindex.com/skill-md-format-and-manifest-spec.html)
