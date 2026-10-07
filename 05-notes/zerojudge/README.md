# ZeroJudge 解題 — Index

[← 回主頁](../../index.md)

[ZeroJudge](https://zerojudge.tw/) (高中生程式解題系統) 的解題紀錄。用 **Playwright 操作瀏覽器 + 本機 g++** 的小迴圈：寫 `.cpp` → 本機測範例 → 自動上傳判題 → 讀 AC/WA 結果。

## 題庫分類 — Problem Sets

| 資料夾 | 題庫 | 進度 |
|:---|:---|:---|
| [basic/](basic/README.md) | 基礎題庫 (第 1–2 頁) | 42 題全 AC |
| [contest/](contest/README.md) | 競賽題庫 (第 1 頁) | 15 AC、1 待修、4 跳過 |
| [uva/](uva/README.md) | UVa 題庫 (第 1 頁) | 20 題全 AC |
| [toi/](toi/README.md) | TOI 題庫 (第 1 頁) | 11 題全 AC、9 跳過 |
| [original/](original/README.md) | ORIGINAL 題庫 (第 1 頁) | 9 AC、2 待修、9 跳過 |

每個資料夾一個 `README.md` 作該題庫的解題對照表（題號、題名、語言、判題結果）。

> TOI / ORIGINAL 兩批已於 2026-10-07 上傳判題；多為 WC / USACO / CERE / 校內競賽題，跳過者多為外部連結無題敘或 olympiad 偏難題。

## 筆記 — Notes

- [用 Playwright 自動操作 ZeroJudge](playwright-zerojudge-automation.md) — 送出答案那一段的三個坑（AJAX 表單、原生輸入、confirm 對話框）
- [批次上傳 67 題遇到的坑與解法](batch-upload-lessons.md) — 一次送大量題目時的瀏覽器自動化經驗（登入狀態、送出冷卻、判題結果對齊）
