---
title: "用 Claude Code 週末建出台股回測系統：Playbook 方法論"
tags: [ai, 外部觀點, claude-code, playbook, workflow, 量化交易, 回測, python, multiprocessing, 實戰]
source: https://www.facebook.com/AV8D.levelup/posts/pfbid02Nzh3w5LFEPeogSnbYy41TfQwcTRuhiAQYU94389kuEwqoXJM46JZtUjKF8THqcrhl
author: 羅達（AV8D LevelUp）
created: 2026-05-26
---

# 用 Claude Code 週末建出台股回測系統：Playbook 方法論

> [!info]
> Facebook 原文：[AV8D LevelUp — 羅達](https://www.facebook.com/AV8D.levelup/posts/pfbid02Nzh3w5LFEPeogSnbYy41TfQwcTRuhiAQYU94389kuEwqoXJM46JZtUjKF8THqcrhl)
> **一句話：** 週末用 Claude Code 建出台股量化回測系統（1,187 檔、2 年資料、80 組參數），從 6 小時壓到 30 分鐘——關鍵不是 prompt 寫得漂不漂亮，而是有沒有 Playbook。

---

## 成果

- 涵蓋台股 1,187 檔股票、2 年歷史資料
- 80 組參數組合平行回測
- MacBook Pro M4 Max（16 核）跑滿 14 核
- **執行時間：從 6 小時壓到 30 分鐘**

---

## 核心方法：Playbook 方法論

> 「用 Claude Code 用得好不好的差距，不在於你 prompt 寫得多漂亮，而在於你有沒有 playbook。」

**Playbook** = Markdown 格式的操作手冊，把專案拆成若干個 prompt，每個 prompt 附上驗收標準。

### 三條鐵規則

1. **一次一個 prompt，等驗收通過再下一個**
2. **出現錯誤先看 message，不要叫 AI 直接重寫——重寫只會把問題藏起來**
3. **每個 prompt 完成後 git commit，出問題可以隨時回退**

---

## 四個版本的演進

| 版本 | 內容 | 有沒有 Playbook |
|---|---|---|
| **v1** | 建立系統骨架（5 個 prompt + 驗收清單）| ✅ |
| **v2** | 資料工程（parquet 快取、API 配額管理）| ❌（最大失誤）|
| **v3** | 執行細節優化（非對稱滑點、三種信心模式）| ✅ |
| **v4** | 平行化（multiprocessing，14 核）| ✅ |

---

## 技術實作亮點

### v2 資料工程

- **Parquet 寬表快取**：一次抓 10 年歷史，依需求切片，後續跑測試只需 5 分鐘
- **空標記系統**：記錄哪些代碼不存在，避免重複 API 呼叫
- **FinMind API 配額管理**：600 次/小時上限到了，自動等待 65 分鐘再重試

### v3 執行細節（Claude 主動提出 + 羅達補充規格）

- **非對稱滑點**：買進 0.2%、賣出 0.3%
- **漲跌停板強制**：停板時禁止掛單
- **部位規模限制**：不超過當日成交量的 0.5%
- **三種信心模式同時輸出**：理想 / 現實 / 悲觀——A/B 比較不破壞舊版本

### v4 平行化（Claude 主動提議）

```python
# 14 核跑滿，2 核留給 OS
multiprocessing.Pool(processes=14)
```

- 每組策略 / 參數組合跑在獨立 process
- 共用唯讀快取，各自寫結果檔案
- 搭配 `tqdm` 進度條
- 4 種策略比較：從 20 分鐘壓到 5 分鐘

---

## 專案架構

```
config/      → 集中式參數管理
data/        → FinMind 抓取 + parquet 快取
strategy/    → 選股、指標、ORB、混合策略
backtest/    → 引擎、執行、指標、視覺化
scripts/     → 自動化（夜間跑、消融實驗、比較）
tests/       → 20+ pytest 測試案例
playbooks/   → v1、v3、v4 markdown 文件
CLAUDE.md    → 每個 session 自動載入的專案 context
```

---

## 三個失誤

### 失誤一：v2 沒有寫 Playbook

資料工程升級時跳過了文件，「設計決策只活在 commit message 裡」。
後續 session 無法重建當時的思路——這是整個過程中最難補救的問題。

### 失誤二：把 CLAUDE.md 命名成 README.md

`CLAUDE.md` 是給 AI 讀的專案 context，每個 session 自動載入。
命名成 README.md 讓 Claude Code 沒有自動抓到，每次 session 都要花 5 分鐘重新解釋。

> [!tip] CLAUDE.md vs README.md
> 兩個受眾不同：`README.md` 是給人看的，`CLAUDE.md` 是給 AI 自動載入的。
> 分開寫，分開命名。

### 失誤三：過早自動化

在策略還沒穩定前就先設定 `.claude/settings.json`、自訂指令、hooks。
**工具的複雜度要跟工作流程的成熟度匹配，不能跑在前面。**

---

## 四個協作場景

**場景一：升規格但不破壞舊版本**
> 「加入非對稱滑點、漲跌停板邏輯，但不要破壞舊的 Execution Model。」
→ Claude 建出三種模式並排，舊版保留作為 baseline。

**場景二：新增功能不動舊程式碼**
> 「不要碰 `smart_screener.py`，新建 `orb_strategy.py` 和 `hybrid_screener.py`，三種一起比。」
→ 舊程式碼零修改，新功能獨立擴展。

**場景三：API 限制——Claude 主動提議**
快取策略、空標記、指數退避——這些邊緣案例羅達沒有預想到，Claude 主動提出。

**場景四：多核利用——Claude 主動提議**
Claude 主動建議 ParallelRunner + `tqdm` 進度條，提案後羅達才意識到硬體沒被充分利用。

> 「跑大規模回測不要用單核思維。你的硬體不是裝飾品，閒置的核心是純粹浪費掉的時間資產。」

---

## 關鍵洞見整理

| 洞見 | 說明 |
|---|---|
| Playbook > Prompt | 跨 session 保留 context 靠的是文件，不是 prompt 技巧 |
| 驗收標準防止漂移 | 每個階段通過才前進，不靠感覺 |
| 平行擴展 > 重寫 | 保留舊版作 baseline，新功能並排比較 |
| 工具成熟度跟工作流程走 | 策略未穩定前不要急著自動化 |
| 每次升級都要寫文件 | v2 跳過文件是整批問題裡最難補的 |
| 問「這能平行化嗎？」 | 未使用的核心 = 浪費的實驗速度 |

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — CLAUDE.md 的作用與 Skills 架構
- [[AI 101 - Harness Engineering]] — 「可靠性來自框架」的完整觀念
- [[AI 101 - Context Engineering]] — Context 跨 session 保留的設計思維
- [[Claude Code 邊寫邊記施工日誌：implementation-notes.html — Thariq（Anthropic）]] — 另一種記錄設計決策的方法

---

## 來源

- Facebook：[AV8D LevelUp — 羅達](https://www.facebook.com/AV8D.levelup/posts/pfbid02Nzh3w5LFEPeogSnbYy41TfQwcTRuhiAQYU94389kuEwqoXJM46JZtUjKF8THqcrhl)
