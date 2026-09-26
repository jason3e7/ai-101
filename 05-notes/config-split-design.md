---
title: "AI 101 - 設計稿: CLAUDE.md vs skills/working-style.md 分工"
tags: [meta, config, design, claude-md, working-style, 設計稿]
created: 2026-09-26
status: draft
---

# 設計稿: CLAUDE.md vs skills/working-style.md 分工 — Config Split Design

[← 回主頁](../index.md)

> [!NOTE]
> 這是設計稿, 討論兩份 config 檔案的分工原則. 起因: 想把 `skills/working-style.md` 裡的**心智清單規則**搬去 `CLAUDE.md`, 順便盤點兩份檔案哪裡重複、哪裡該搬. 通過後才動手.

> **TL;DR (EN):** Two config files in this repo overlap and drift risk is real. Proposal: **CLAUDE.md holds hard rules (format, naming, git, GitHub rendering) that every session must obey; skills/working-style.md holds preferences, workflow logic, and situational strategies that only need to load when triggered.** Move the mind-map checklist rule from working-style.md into CLAUDE.md because it is a per-note format requirement. Other duplicated rules should collapse to CLAUDE.md as single source of truth, with working-style.md pointing to it instead of restating.

---

## 兩份檔案的定位差異 — Roles

| 檔案 | 載入時機 | 目的 | 適合放什麼 |
|:---|:---|:---|:---|
| **CLAUDE.md** | 每 session 自動載入 (實測不是 100% 穩定, 但意圖是「基礎背景永遠在」) | 專案級**硬規則** | 格式、命名、必備結構、Git 操作、平台呈現 |
| **skills/working-style.md** | Skill 觸發時載入 (關鍵字或顯式呼叫), 不保證每 session 都看到 | 協作**偏好與判斷邏輯** | Workflow、tone、情境策略、meta 規則 |

**風險**: 兩份重複的規則會 drift out of sync (改一份忘了改另一份, 之後 Claude 該信誰?).

---

## 分工判準 — Where Does It Belong?

四個問題按順序問:

1. **會不會被違反造成 output 壞掉?** (例如 YAML 格式錯導致 render 失敗) 是 → **CLAUDE.md**
2. **是不是每篇 note 都要遵守?** (例如 `## Sources` 必要, TL;DR 位置) 是 → **CLAUDE.md**
3. **需不需要保證每 session 都看到?** (例如 git deploy key 命令) 是 → **CLAUDE.md**
4. **是不是 workflow / tone / 情境判斷?** (例如「Chrome 有的話用 read_page」、「回應要短」) 是 → **skill**

---

## 現況盤點 — What's Where Now

### 兩者重複 (drift 風險)

| 規則 | CLAUDE.md | working-style.md |
|:---|:---:|:---:|
| 平台呈現原則 (GitHub 為主) | ✓ | ✓ |
| Callout 5 種 alert 大寫 | ✓ | ✓ |
| 相對連結規則 | ✓ | ✓ |
| Git deploy key + commit format | ✓ (詳) | ✓ (簡) |
| 檔名 kebab-case | ✓ | ✓ |
| 標題上限 ≤ 7、理想 3 | ✓ | ✓ |

### CLAUDE.md 獨有
- Prime Directive (最高原則)
- 語言 L2 詳細規則 + 老嫗能解
- Code 範例規則
- Sources 是必要區塊
- 新增筆記 SOP (5 步驟)

### working-style.md 獨有
- Repo Structure 全景
- 兩種工作模式 (研究筆記 vs 外部觀點) 的**檔名與結構規則**
- **心智清單規則** ← 想搬到 CLAUDE.md
- 各平台抓內容策略 (Chrome vs no-Chrome) 一整個表
- Skills 建立邏輯 (meta)
- 溝通偏好 (回應要短、冠名 jason3e7、直接說等)

---

## 建議調整 — Proposed Changes

### 搬進 CLAUDE.md (硬規則, 每篇都要)

1. **心智清單規則** ← 用戶指定. 屬於每篇 note 的格式硬規則
2. **Repo 資料夾對應** (基礎 → 01-fundamentals, 進階 → 02-advanced 等) — 每次寫新 note 都要用, 建議升為硬規則
3. **兩種工作模式的檔名規則** (研究筆記用 kebab-case, 外部觀點用 `yyyymmdd_標題 — 作者.md`) — 硬規則

### 留在 working-style.md (偏好 / 判斷邏輯)

- 兩種工作模式的**流程判斷** (什麼時候用哪個模式, 怎麼決定 condense vs expand)
- 各平台抓內容策略 (Chrome / no-Chrome 表格) — 情境判斷
- Skills 建立邏輯 (meta 判斷)
- 溝通偏好 (tone)
- Repo Structure 全景 (workflow 需要, 但不是硬規則)

### 重複的部分收斂

- CLAUDE.md 當**唯一 source of truth**
- working-style.md 對於已在 CLAUDE.md 的規則, 改成**只提一句「完整規則見 CLAUDE.md」**, 不重複
- 這樣改一份就好, 不會 drift

---

## 心智清單規則: CLAUDE.md 具體插入位置

建議插入在 `## 寫作風格指南 → ### 結構` 段落底下, 作為新的一個 `###` 子節. 內容 draft:

````markdown
### 心智清單 — Mind-Map Checklist

每篇 note 在**開頭 callout + TL;DR 之後、本體之前**放一張心智清單, 讓讀者一眼看完全篇脈絡.

**放置格式**: 包在一個 ` ```markdown ` code block 裡 (不是渲染成清單, 是等寬框). 這樣做的原因是根節點要用 `#` (markmap 中心), 放進 code block 後那個 `#` 只是字面文字, 不會撞到「每篇一個 H1」的規則.

**內容規則**:

- 第一行 `# 根節點` (一句話, 全篇主旨)
- 分支用 `*` (不用 `-`), 巢狀縮排
- 一個根 · 深度 ≤ 4 · 每層 ≤ 7 · 節點是短語
- 不加粗體標籤 (根節點本身就是「看全篇」那句)
- code block 內是字面文字, 別用行內程式碼語法 (例如寫 `(?)` 而非 `` `(?)` ``)

**產生方式**: 用兩個 skill 之一, 依這兩步判斷:

1. **先看實際材料**: 這篇 note 手上要處理的材料多不多?
   - 材料已足、要收斂 → `condense-mindmap` (每節點可追回來源)
   - 材料很少、要生成 → `expand-mindmap` (研究後放大, 推測節點標 `(?)`)
2. **判斷不出來時, 才看 prompt 字數**: ≤ 500 字用 `expand-mindmap`, > 500 字用 `condense-mindmap`
````

---

## 動工步驟 — Roll-out Plan (approve 後執行)

**Phase 1** (你 approve 後立刻做):
1. 修 CLAUDE.md: 加「心智清單 — Mind-Map Checklist」段落 (位置如上)
2. 修 working-style.md: 移除心智清單規則, 改成「格式規則見 CLAUDE.md」, 保留兩個 skill 的決策邏輯 (`condense` vs `expand` 那兩步判斷) 因為判斷邏輯是 skill 的專長

**Phase 2** (可選, 後續整理):
3. 收斂重複規則: 上表 6 條重複規則, 把 working-style.md 的版本改成指向 CLAUDE.md
4. 把 Repo 資料夾對應 + 兩種工作模式的檔名規則搬進 CLAUDE.md

Phase 2 是 nice-to-have, 不做也不會壞事. Phase 1 是這次的核心目標.

---

## 我的重點 — Takeaways

- **CLAUDE.md = 硬規則 (每 session 保底)**, working-style.md = **偏好 + 判斷邏輯 (觸發載入)**
- 兩份重複的規則有 drift 風險, 建議 CLAUDE.md 當唯一 source of truth
- 心智清單規則屬於格式硬規則, 應該搬到 CLAUDE.md
- 這篇是**設計稿**, approve 後才動 CLAUDE.md 和 working-style.md
