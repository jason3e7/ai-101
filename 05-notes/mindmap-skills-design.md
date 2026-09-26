---
title: AI 101 - 兩個心智圖 skill 的設計筆記：濃縮與放大
tags: [ai, skill, mindmap, 收斂, 發散, markmap, 設計筆記]
created: 2026-09-26
---

# 兩個心智圖 skill 的設計筆記 — Condense & Expand Mind Maps

[← 回主頁](../index.md)

> [!NOTE]
> 記錄兩個想做的 skill：**濃縮心智圖**（一堆資料 → markdown list 心智圖）和**放大心智圖**（一點種子 → 研究 → markdown list 心智圖）。這篇先釐清「它們到底是什麼、有沒有現成的、該長什麼樣」，之後再照 [skills 的格式](../skills/refactor-note.md)寫成正式 skill。

> **TL;DR (EN):** Two mind-map skills, and they are not new inventions - they are the mind-map-shaped versions of two capabilities this repo already maps: condense = convergent (like 摘要/統整), expand = divergent (like 發想). Rendering is already solved by markmap; what a skill adds is the *thinking* (what to keep, what to invent). Shared output: a nested `-` markdown list, capped per level by the attention rule, portable across markmap / Obsidian / GitHub.

---

## 先認清：這其實是舊軸的新形狀 — It's the Old Axis

不要把這兩個當成全新的東西。攤到 [AI 能力全景圖](../02-advanced/ai-capability-landscape.md)那兩軸上，位置很清楚：

| 想法 | 資訊流向 | 對應的既有能力 | 忠實度要求 |
|:---|:---|:---|:---|
| **濃縮心智圖** | 收斂（多 → 少） | 摘要 ＋ 統整 | **高**（不能亂加） |
| **放大心智圖** | 發散（少 → 多） | 發想 ＋ 研究 | **低**（就是要它生） |

> [!IMPORTANT]
> 兩者共用「心智圖」這個**輸出形狀**，但**方向相反、驗收方式相反** - 這跟 [六種能力執行手冊](../02-advanced/capabilities-playbook.md)講摘要與發想時是同一組道理。濃縮要逐節點回原文核對；放大不必對原文，要挑的是有沒有用。**所以它們該是兩個 skill，不是一個 skill 兩個模式** - 硬合在一起，忠實度的相反要求會打架。

---

## 有沒有現成的 — What Already Exists

分兩塊看：**畫出來**已經有現成解，**想什麼**沒有。

**一、渲染：已解決，不用自己做。**
`markdown 巢狀清單 → 心智圖` 是成熟功能。標準是 **markmap**（[markmap.js.org](https://markmap.js.org/)），語法就是縮排的 `-`：第一層當中心，往下分支。Obsidian 有一票外掛（Mindmap NextGen、Mind Map、Mindmap Blocks）都吃同一套語法。**GitHub 沒有心智圖引擎**，但巢狀清單本來就會退化成正常的縮排清單 - 不會壞，只是不畫成圖。

**二、思考：沒有現成的，這才是 skill 的價值。**
工具只負責「把清單畫成圖」。**「這堆資料該濃縮成哪幾條」「這個種子該往哪幾個方向長」是判斷，不是渲染** - 這正是 skill 要補的那塊。

**三、repo 內：沒有心智圖 skill，但有一條直接可用的規則。**
現有 skill 是 curate-notes、refactor-note、working-style、note-lifecycle。沒有心智圖相關的。但 [refactor-note](../skills/refactor-note.md) 的「注意力上限」（每層 ≤ 7、硬上限 10）**直接就是心智圖每個節點該有幾個分支的規則** - 兩個新 skill 都該引用它。

---

## 共用的輸出格式 — The Shared Output Contract

兩個 skill 吐出來的東西要長一樣，才好接 markmap：

```markdown
# 中心主題（一句話）
- 主要分支 A
  - 子節點
  - 子節點
- 主要分支 B
  - 子節點
- 主要分支 C
```

三條硬規則：

1. **一個根**：最上面一行是唯一的中心主題，一句話講完。
2. **每層 ≤ 7、理想 3**：套 [refactor-note](../skills/refactor-note.md) 的注意力上限。超過就分組或合併，別讓一層攤十幾個。
3. **節點是短語不是句子**：心智圖的節點要能一眼掃過，一句完整的話塞進去就失去心智圖的意義。

> [!TIP]
> 這個格式剛好符合 repo 的[呈現平台原則](../CLAUDE.md)：markmap／Obsidian 畫成圖，GitHub 退化成乾淨的縮排清單，兩邊都不壞。不綁單一工具。

---

## 兩個 skill 的設計草案 — The Two Skills

### 濃縮心智圖（收斂）— Condense

**輸入**：一堆資料（多篇筆記、一長串逐字稿、幾份文件）。
**輸出**：一張把它們收斂成結構的心智圖。
**本質**：摘要 ＋ 統整的心智圖版，忠實度高。

**流程草案：**

1. **抽取** - 把資料裡的重點事實、主張、數字挑出來（低階收斂）。
2. **分組** - 把同質的併成一個分支，找出它們共同的上層概念（高階收斂＝統整）。
3. **命名分支** - 每個分支給一個短語標題，這個標題要能概括底下所有子節點。
4. **修剪** - 套注意力上限：每層超過 7 就再分組或砍掉最弱的。

> [!WARNING]
> **濃縮的坑是「悄悄加料」。** 收斂端最怕它為了讓結構好看，補上原文沒有的節點。skill 要硬性要求：**每個節點都能追回原始資料的哪一段**，加不出處的就是幻覺。這跟 [LLM 的極限](../02-advanced/llm-limitations.md)「驗證必須來自外部」是同一條。

> [!NOTE]
> **這個 skill 已經寫好了（2026-09-26）：** [`skills/condense-mindmap/SKILL.md`](../skills/condense-mindmap/SKILL.md)，全英文、採官方可攜格式（資料夾 ＋ SKILL.md，見 [Agent Skills 說明](../01-fundamentals/agent-skills.md)）。上面這些設計決定都寫進去了：一個根、深度 ≤ 4、每層 ≤ 7、節點是短語、每個節點可追回原文、超量用 map-reduce、只吐 markdown 不附渲染指令。兩個都寫好了。

---

### 放大心智圖（發散）— Expand

**輸入**：一點點種子（一個主題、一句話、一個問題）。
**輸出**：研究之後，把它展開成一張有內容的心智圖。
**本質**：發想 ＋ 研究的心智圖版，忠實度低（但研究部分要有依據）。

**流程草案：**

1. **先發散方向** - 從種子生出幾個**互不重疊**的切入角度（高階發散）。別急著填細節。
2. **各自研究** - 每個方向去查、去補內容（這步讓它跟純發想不同 - 有事實墊底）。
3. **結構化** - 把研究結果整理進各分支，套注意力上限。
4. **標記虛實** - 哪些節點是查到的、哪些是推測的，分開標。

> [!WARNING]
> **放大有兩個相反的坑。** 一是[發想多樣性收窄](../02-advanced/capabilities-playbook.md)：LLM 發想會自動收斂到同一核心（一實驗中 94% 點子同源），所以第 1 步要**強制不重疊**。二是研究步驟的幻覺：發散端可以天馬行空，但只要標成「查到的」就必須真的查到 - **虛實要分明**。

> [!NOTE]
> **這個 skill 也寫好了（2026-09-26）：** [`skills/expand-mindmap/SKILL.md`](../skills/expand-mindmap/SKILL.md)，全英文。虛實用 ` (?)` 標記推測節點、未標記＝已查證。**採選項 A（各自獨立，不依賴濃縮）**：兩個 skill 的輸出契約刻意保持一致（同樣的一個根／深度 ≤ 4／每層 ≤ 7／短語規則），但忠實度心法相反 - 濃縮「追不回原文就丟」，放大「可以生但要標虛實」。之所以不讓放大呼叫濃縮，是因為 (1) 會破壞 skill 的可攜性、(2) 兩者忠實度規則會打架，重疊的只有格式那 15 行，是良性重複。

---

## 規格與待決 — Decisions

**已定（2026-09-26, jason3e7）：**

- **深度上限**：**預設 3 層，最多 4 層**。再深就不是心智圖是大綱了。
- **不包 markmap 指令**：skill 只吐 markdown，不附「貼到哪裡看圖」的話。渲染完全交給使用者的工具（markmap／Obsidian／GitHub）。
- **放大可用 web 研究**：**不限制工具或方法** - 種子太冷就去查，怎麼查由它決定。

**濃縮的輸入上限（2026-09-26 定）：** 一次盡量餵，**當資料超過 context window 時，改用 map-reduce**：先分段各自濃縮成小圖，再把小圖合併濃縮成一張。並告知使用者用了 map-reduce、分了幾段（跨段的關聯可能較弱）。細節見 [六種能力執行手冊](../02-advanced/capabilities-playbook.md)摘要那節的長文處理。

---

## 相關筆記 — Related

- [AI 能力全景圖](../02-advanced/ai-capability-landscape.md) - 收斂／發散兩軸，這兩個 skill 的定位來源
- [六種能力執行手冊](../02-advanced/capabilities-playbook.md) - 摘要與發想的實戰打法，直接可套
- [refactor-note skill](../skills/refactor-note.md) - 每層 ≤ 7 的注意力上限，心智圖分支數的規則
- [AI 怎麼知道該用哪種能力](../02-advanced/how-ai-picks-capability.md) - 為什麼 skill 描述要給範例、寫清楚任務

## Sources

- [markmap — Visualize your Markdown as mindmaps](https://markmap.js.org/)
- [Mindmap NextGen — Obsidian Plugin（markmap 渲染）](https://github.com/gera2ld/markmap)
- [obsidian-markdown-mindmap（縮排清單即心智圖）](https://github.com/Paultagoras/obsidian-markdown-mindmap)
