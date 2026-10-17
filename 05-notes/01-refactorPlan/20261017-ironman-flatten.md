---
title: AI 101 - Ironman 日期型目錄拆平 Refactor Plan
tags: [refactor, structure, ironman, 計劃書, executed]
created: 2026-10-17
status: executed
executed: 2026-10-17
---

# Ironman 日期型目錄拆平 — Refactor Plan (2026-10-17) ✅ EXECUTED

[← 回主頁](../../index.md)｜[三十篇標題](../02-project/2026ironman/titles.md)｜[參賽規劃](../02-project/2026ironman/plan.md)

> [!NOTE]
> 依據 2026-10-17 更新的 [refactor-note](../../skills/refactor-note.md) 規則「**日期型目錄不套 ≤ 10 上限**」, 把 `ironman/drafts/`、`ironman/publish/`、`ironman/assets/` 底下的 5 天分組子資料夾 (`day01-05/` ... `day26-30/`) 全部拆平, 讓 30 篇 `dayNN-*` 檔案平放一層. 本份是計劃書, **等用戶核准後才執行**.

## 1. 現況盤點 — Before

### `drafts/` — 30 檔分 6 子資料夾

| 子夾 | 檔數 |
|---|---:|
| `day01-05/` | 5 |
| `day06-10/` | 5 |
| `day11-15/` | 5 |
| `day16-20/` | 5 |
| `day21-25/` | 5 |
| `day26-30/` | 5 |

### `publish/` — 30 檔分 6 子資料夾 + 1 根 README

| 子夾 | 檔數 |
|---|---:|
| `day01-05/` | 5 |
| `day06-10/` | 5 |
| `day11-15/` | 5 |
| `day16-20/` | 5 |
| `day21-25/` | 5 |
| `day26-30/` | 5 |
| 根 | `README.md` |

### `assets/` — 混亂狀態 (已有部分平放)

| 位置 | 內容 |
|---|---|
| 根層 | `cover-day15.html` ~ `cover-day25.html` (11 檔 HTML) + `day15-cover.png` ~ `day25-cover.png` (11 檔 PNG) + `cover-template.html` + `README.md` + `day18images/` (3 jpg) + `unused/` |
| `day01-05/` | 4 HTML (`cover-day02`~`cover-day05`) + 5 PNG (`day01-cover`~`day05-cover`) |
| `day06-10/` | 5 HTML + 5 PNG |
| `day11-15/` | 4 HTML (`cover-day11`~`cover-day14`, Day 15 的已在根) + 4 PNG |
| `day26-30/` | 5 HTML + 5 PNG |

> 註: `day01-05/` 少 `cover-day01.html` (未知原因, Day 01 cover HTML 不在). 可能被刪, 不處理.
> 註: `day18images/` 是 Day 18 文中內嵌圖片的輔助目錄 (3 張 jpg), 不是封面, 這次不動.
> 註: `unused/` 是棄用的 cover 草稿, 不是日期型, 不動.

### 門檻判定 (套新規則前後對照)

| 層 | 平放後檔數 | 舊規則 (≤10) | 新規則 (日期例外) |
|---|---:|---|---|
| `drafts/` | 30 | 違反 | ✓ 通過 (日期型例外) |
| `publish/` | 31 (含 README) | 違反 | ✓ 通過 |
| `assets/` | ~44 (30 HTML + 30 PNG 不含 README/template/aux) | 違反 | ✓ 通過 |

## 2. 建議結構 — After

### `drafts/`

```
drafts/
├── day01-llm-is-statistics.md
├── day02-why-it-got-strong.md
├── day03-what-it-cannot-do.md
├── day04-capability-landscape.md
├── day05-how-it-picks.md
├── day06-prompt-engineering.md
├── day07-context-engineering.md
├── day08-harness-engineering.md
├── day09-loop-engineering.md
├── day10-xxx-engineering-summary.md
├── day11-verify-ai-output.md
├── day12-mindmap-skills.md
├── day13-ai-atrophy.md
├── day14-squeezed-middle.md
├── day15-expand-yourself.md
├── day16-ai-fingerprint-scan.md
├── day17-composite-score.md
├── day18-detecting-pure-human.md
├── day19-ai-watermark.md
├── day20-ai-style-infection.md
├── day21-back-to-verifiable-ground.md
├── day22-zerojudge-at-scale.md
├── day23-zerojudge-cross-tier.md
├── day24-leetcode-blind75.md
├── day25-leetcode-lc75-nc150.md
├── day26-phm-ctf.md
├── day27-phm-ctf-depth.md
├── day28-phm-ctf-final.md
├── day29-htb-goal-cases.md
└── day30-finale.md
```

### `publish/`

```
publish/
├── README.md
├── day01-ithome.md ... day30-ithome.md  (30 檔, 平放)
```

### `assets/`

```
assets/
├── README.md
├── cover-template.html
├── cover-day02.html ... cover-day30.html  (29 檔 HTML, 缺 day01)
├── day01-cover.png ... day30-cover.png     (30 檔 PNG)
├── day18images/                            (3 jpg, 不動)
└── unused/                                 (不動)
```

> **不處理**: `cover-dayNN.html` 跟 `dayNN-cover.png` 的檔名前後綴順序不一致 (歷史遺留). 想一次改需要另開命名統一的 refactor, 範圍會更大, 這次只做「拆分組」.

## 3. 具體搬動 — Moves

### A. `drafts/` — 30 個 `git mv`

每檔從 `drafts/dayXX-YY/dayNN-slug.md` 搬到 `drafts/dayNN-slug.md`. 搬完後 `rmdir drafts/day??-??` 清掉 6 個空夾.

### B. `publish/` — 30 個 `git mv`

每檔從 `publish/dayXX-YY/dayNN-ithome.md` 搬到 `publish/dayNN-ithome.md`. README.md 本來就在根, 不動.

### C. `assets/` — ~40 個 `git mv`

從 `assets/day01-05/`, `assets/day06-10/`, `assets/day11-15/`, `assets/day26-30/` 底下的 HTML + PNG 全部搬到 `assets/` 根. 搬完後清掉 4 個空夾.

### D. 連結 / 路徑同步

全 repo grep 到 14 個命中 (`grep -rn "ironman/\(drafts\|publish\|assets\)/day[0-9][0-9]-[0-9][0-9]"`), 分佈:

| 檔 | 要改的地方 |
|---|---|
| `05-notes/02-project/2026ironman/drafts/day30-finale.md` (×2) | `assets/day06-10/day10-cover.png` → `assets/day10-cover.png`; `assets/day11-15/day13-cover.png` → `assets/day13-cover.png` |
| `05-notes/02-project/2026ironman/publish/day30-ithome.md` (×2) | 同上 |
| `05-notes/02-project/zerojudge/playwright-zerojudge-automation.md` (×2) | `drafts/day21-25/day21-...md` → `drafts/day21-...md` |
| `05-notes/essays/drafting-with-origin-quotes.md` (×1) | `drafts/day16-20/day20-...md` → `drafts/day20-...md` |
| `05-notes/02-project/2026ironman/lab01/v07-preview.md` (×1) | `publish/day01-15/day*-ithome.md` → `publish/day*-ithome.md` (這條本來就寫錯, 順便修) |
| `02-advanced/engineering/context-engineering-in-depth.md` (×2) | `drafts/day06-10/day06-...md`, `day07-...md` → `drafts/day06-...md`, `drafts/day07-...md` |
| `02-advanced/engineering/harness-engineering.md` (×1) | `drafts/day06-10/day07-...md` → `drafts/day07-...md` |
| `02-advanced/limits-and-verification/converge-before-verify.md` (×3) | `drafts/day06-10/day06, day09` → `drafts/day06, day09` |

另外**不會被 grep 命中但需人工掃**的:
- 所有 30 個 drafts 檔案開頭的 `[← 回主頁](../../../../index.md)` → `../../../index.md` (少一層); `../../plan.md` → `../plan.md`; `../../titles.md` → `../titles.md`. 共 30 檔 × ~3 條 = ~90 處
- `publish/README.md` 的 Layout 段 (`每 5 天一組: day01-05/ ... day26-30/`) 要改成「平放一層」描述
- `assets/README.md` 若有分組描述要改
- `logs/fix-log.md` 若有 `publish/dayXX-YY/` 路徑要改

執行時用一條 regex 一次全掃 + 替換 `s|/day[0-9][0-9]-[0-9][0-9]/|/|g`, 可以涵蓋大部分. 相對連結的層級 (`../../../../` → `../../../`) 要另一條 `s|\.\./\.\./\.\./\.\.|../../../|g` 之類.

## 4. 風險 — Risks

- **raw.githubusercontent.com 圖片 URL**: Day 30 draft/publish 用 `raw.githubusercontent.com/.../assets/day06-10/day10-cover.png` 這類路徑. 搬完後路徑中間層消失, 要同步改. GitHub raw URL 跟 iThome 線上文章用的 upload URL 無關, 不影響已發布內容.

- **Obsidian 內部連結**: 不預期有 `[[wiki-link]]` 指向 ironman 檔 (CLAUDE.md 禁用). 執行前仍 `grep -rn "\[\[" 05-notes/02-project/2026ironman/` 確認一次.

- **外部連結 (blog / tweet / email)**: 若有人把 GitHub 連結分享過, 連結會壞. 不可控, 忍受代價. (iThome 文章用自己上傳的圖, 不受影響.)

- **CI / Playwright 腳本**: `lab01/`, `tools/` 下的 Python 可能用相對路徑讀 drafts. 執行前 `grep -rn "day[0-9][0-9]-[0-9][0-9]" 05-notes/02-project/2026ironman/{lab01,tools}/ 2>/dev/null` 掃一遍.

- **`publish/README.md` 跟 `assets/README.md` 的 Layout 描述**: 要同步改成「平放一層」. 不改會跟實際結構衝突, 讀者困惑.

- **`CLAUDE.md` ironman 行**: 已於上一個 commit (43aab9f) 改成「平放一層」描述, 不用再改.

- **沒有不確定分類** — 日期型規則清楚, 不需要用戶拍板.

- **不會壞的**:
  - iThome 線上文章 (完全不碰 GitHub)
  - 跨檔案的 iThome URL (`https://ithelp.ithome.com.tw/articles/10xxx`)
  - 本 repo 內其他筆記 (如 02-advanced/) 的互引 (不經 ironman 路徑)

## 5. 執行順序 — Order

**4 個 commit**, 按順序:

### Commit 1 — 計劃書 (就是本份)
- `05-notes/01-refactorPlan/20261017-ironman-flatten.md`
- 內容: 本文件
- 推上去等用戶審

### Commit 2 — `git mv` 搬檔
- 批次 `git mv` 100 個檔案 (30 drafts + 30 publish + ~40 assets)
- 刪 10 個空夾 (6 drafts + 6 publish + 4 assets — publish 只有 6 子夾、assets 只有 4 子夾, 所以是 16 總空夾)

> 修正: drafts 6 + publish 6 + assets 4 = **16 個空夾**

- 這 commit **只動路徑**, 檔內容 0 修改

### Commit 3 — 連結與 README 修正
- 全 repo 掃 regex `ironman/(drafts|publish|assets)/day[0-9][0-9]-[0-9][0-9]/`, 一條一條替換
- 修 drafts 30 檔開頭的 `[← 回主頁]` 等 3 條相對連結 (少一層)
- 修 `publish/README.md`, `assets/README.md` 的 Layout 段
- 修 `logs/fix-log.md` (若有命中)
- 修 Day 30 draft+publish 的 raw.githubusercontent 圖片 URL (2 檔 × 2 處)

### Commit 4 — 收尾 (可能併入 commit 3)
- 根 `index.md` 若有命中 (本次掃無, 應該不用)
- 本份計劃書 frontmatter `status: pending-approval` → `status: executed`
- 可選: 刪本份計劃書 (或保留當歷史紀錄, 建議保留)

---

**中斷點保證**: 若執行到 commit 2 後中斷, repo 處於「檔已搬但連結還斷」狀態. 不致命, 下次接 commit 3 即可. 若執行到 commit 3 中斷, 可能漏改少數引用, 用 `git grep "day[0-9][0-9]-[0-9][0-9]"` 可補上.

**跟 iThome 線上無關**: 線上文章用 iThome 自己上傳的圖片 URL, 本次重構不影響任何線上文章的渲染. 只影響 GitHub repo 內的結構.

---

**待用戶核准後執行.** 核准方式: 回「照這份做」/「OK」/「GO」, 或指出要修改的點.
