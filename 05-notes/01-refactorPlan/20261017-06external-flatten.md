---
title: AI 101 - 06-external 週範圍子夾拆平 Refactor Plan
tags: [refactor, structure, 06-external, 計劃書, pending-approval]
created: 2026-10-17
status: pending-approval
---

# 06-external 週範圍子夾拆平 — Refactor Plan (2026-10-17)

[← 回主頁](../../index.md)

> [!NOTE]
> 依據 2026-10-17 更新的 [refactor-note](../../skills/refactor-note.md) 規則「**日期型目錄不套 ≤ 10 上限**」, 把 `06-external/archive/yyyymm/` 底下的週範圍子夾 (`0513-0518/`、`0520-0526/` 等) 全部拆平, 月份目錄直接平放檔案. 當初建週範圍是因為舊規則「月內超過 10 篇就再分」, 現在這條規則被日期型例外覆寫, 不再需要. 本份是計劃書, **等用戶核准後才執行**.

## 1. 現況盤點 — Before

### 整體結構

```
06-external/
├── README.md
├── assets/
├── 20260921_*.md × 3 (當月, 其實是 2026-09 舊檔)
├── reference/ (當月次要, 目前 0 檔)
└── archive/
    ├── 202605/
    │   ├── 0513-0518/ × 6 檔
    │   ├── 0520-0526/ × 6 檔
    │   ├── 0527-0529/ × 3 檔
    │   └── reference/
    ├── 202606/
    │   ├── 0602-0609/ × 2 檔
    │   ├── 0611/      × 8 檔
    │   └── reference/
    └── 202607/
        ├── 20260704_*.md × 4 檔 (已平放, 當作對照組)
        └── reference/ × 6 檔
```

### 問題點

- **月份內又用週範圍再分**: `202605` 的 15 檔 + `202606` 的 10 檔都走這個模式, 原因是舊規則「單層 ≤ 10」
- **新規則「日期型目錄不套 ≤ 10 上限」已經在 CLAUDE.md** 跟 `skills/refactor-note.md` 寫好, 現狀的週範圍子夾變成冗餘
- **對照組**: `202607/` 已經直接把 4 檔平放在月份根 (當時應該 ≤ 10 就沒再分), 證明平放可行

### 門檻判定

| 月份 | 平放後檔數 (不含 reference/) | 舊規則 (≤10) | 新規則 (日期例外) |
|---|---:|---|---|
| 202605 | 15 | 違反 (當初用週分法) | ✓ 通過 |
| 202606 | 10 | 貼邊 (當初用週分法) | ✓ 通過 |
| 202607 | 4 | ≤ 10 | ✓ 通過 (本來就平放) |

## 2. 建議結構 — After

```
06-external/
├── README.md (更新 Layout 段)
├── assets/
├── 20260921_*.md × 3  (當月, 不動)
├── reference/
└── archive/
    ├── 202605/
    │   ├── 20260513_*.md ... 20260529_*.md × 15 (全部平放)
    │   └── reference/ (不動)
    ├── 202606/
    │   ├── 20260602_*.md ... 20260611_*.md × 10 (全部平放)
    │   └── reference/ (不動)
    └── 202607/ (不動, 已是目標狀態)
        ├── 20260704_*.md × 4
        └── reference/
```

> **不動**:
> - 當月 root (`06-external/*.md`) + `reference/`: 日常 curate-notes 流程會處理, 不是本次 scope
> - 各月份內的 `reference/`: 是「重要性」維度, 不是日期分組, 保留
> - 2026-09 舊檔**封月**: 嚴格說 `20260921_*.md` 應該搬進 `archive/202609/`, 但這是日常封月維護操作, 不在本次 refactor 範圍 (另開一次)
> - 檔名: `yyyymmdd_標題 — 作者.md` 格式沿用, 不改名

## 3. 具體搬動 — Moves

### A. `archive/202605/` — 15 個 `git mv`

| 來源 | 目標 |
|---|---|
| `archive/202605/0513-0518/*.md` × 6 | `archive/202605/*.md` |
| `archive/202605/0520-0526/*.md` × 6 | `archive/202605/*.md` |
| `archive/202605/0527-0529/*.md` × 3 | `archive/202605/*.md` |

搬完後 `rmdir` 3 個空夾.

### B. `archive/202606/` — 10 個 `git mv`

| 來源 | 目標 |
|---|---|
| `archive/202606/0602-0609/*.md` × 2 | `archive/202606/*.md` |
| `archive/202606/0611/*.md` × 8 | `archive/202606/*.md` |

搬完後 `rmdir` 2 個空夾.

### C. 連結同步

- **外部引用**: `grep -rn "0513-0518\|0520-0526\|0527-0529\|0602-0609\|0611/"` 掃過全 repo 只命中 `06-external/README.md` 本身. 無外部 .md 檔引用這些路徑.
- **檔案本身的 raw.githubusercontent URL**: `06-external` 的 .md 檔不太會自己 ref 兄弟檔. 執行時仍 grep 確認.

### D. README 更新

- `06-external/README.md` 的 Layout 段把「月內若超過 10 篇，再依**日期範圍**分資料夾（如 `0513-0518/`）」拿掉, 改成「月份內直接平放, 日期型目錄不套 ≤ 10 上限 (見 refactor-note)」.
- `skills/refactor-note.md` 的 Monthly Archiving 段也要掃一輪, 若有提及週範圍分法要移除.

## 4. 風險 — Risks

- **外部引用**: 已 grep 確認無本機 .md 引用週範圍路徑. GitHub 上的 raw URL 若有人分享過會失效, 不可控 (跟之前 ironman flatten 一樣的 trade-off).
- **檔名長度 / 特殊字元**: 檔名含空格、中文、`—` dash. `git mv` 用引號包起來即可. 若有 shell glob 失敗再逐檔處理.
- **沒有不確定分類** — 日期型規則清楚, 無需用戶拍板.
- **不會壞的**: iThome 線上內容、本 repo 其他路徑引用、檔案內容本身 (本次只搬路徑).

## 5. 執行順序 — Order

**3 個 commit**:

### Commit 1 — 計劃書 (本份)
- `05-notes/01-refactorPlan/20261017-06external-flatten.md`
- 推上去等用戶核准

### Commit 2 — `git mv` 搬檔
- 批次 `git mv` 25 個檔 (15 + 10)
- `rmdir` 5 個空夾
- 本 commit **只動路徑**, 內容 0 修改

### Commit 3 — README + 規則同步
- `06-external/README.md` Layout 段更新
- `skills/refactor-note.md` 若有提週範圍, 掃過更新
- 本份計劃書 frontmatter `status: pending-approval` → `status: executed`

---

**中斷點保證**: Commit 2 跑完中斷 → repo 處於「檔已搬但 README 還寫週範圍」狀態, 不致命. 補 Commit 3 即可.

**跟 iThome / 外部無關**: 本次完全是 repo 內部重構, 不影響任何外部連結.

---

**待用戶核准後執行.** 核准方式: 回「照這份做」/「OK」/「GO」, 或指出要修改的點.
