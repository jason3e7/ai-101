---
title: "AI 101 - 鐵人賽 Day 18: 翻面問題 — 哪些文章是純手寫, 完全沒碰 AI 的?"
tags: [ai, 鐵人賽, ironman, 文風檢測, 翻面問題, 純手寫, lab01-收尾, 草稿]
created: 2026-10-01
status: draft-skeleton
---

# Day 18｜翻面問題 — 哪些文章是純手寫, 完全沒碰 AI 的? — Can We Detect Pure Human Writing?

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> lab01 文風檢測三部曲收尾. [Day 16](./day16-ai-fingerprint-scan.md) 用單指紋 (`——`) 掃全屆, [Day 17](./day17-composite-score-v06b.md) 疊到 6 個 signal 的 V06B 綜合分數. 這篇把問題**翻面**: 不問「誰像 AI」, 改問「誰是純手寫、完全沒碰 AI」. 這個問題比抓 AI 味難得多, 因為「沒有訊號」可能是**沒用 AI**, 也可能是**用了 AI 但壓得乾淨**.

> **TL;DR (EN):** TODO — after writing body

---

## 為什麼講這個 — Why This Matters

原話 (jason3e7):

> 做了這個研究之後, 我反而好奇有那些文章是純手寫, 完全沒有用 AI 的, 是不是也有辦法辨識呢?

這是個**翻面問題**. 前兩天 (Day 16-17) 用 lab01 的 signal 抓「像 AI 的」, 但這組工具真的能回答「這篇是人寫的」嗎?

直覺上你會想: signal 都零命中就是人寫. 但實務上這條反推不成立, 原因後面寫.

---

## TODO — 收尾章節骨架

(待寫. 可能展開的角度:)

- **為什麼「反推」失效**: 零命中可能是沒用 AI, 也可能是「用了 AI 但有意識把痕跡壓乾淨」(Day 16 的 ChatGPT & Codex 組 0.11 就是例子)
- **lab01 能回答什麼 / 不能回答什麼**: signal 高 → 排序排名有信度; signal 低 → 推不出「人寫」
- **「純手寫」的正面訊號有嗎?**: 口語化、不工整、typo 不修、個人癖好 (某個固定 emoji、某種口頭禪) — 這些 lab01 都沒抓, 要做要重設計
- **為什麼這個問題本質上更難**: 證明「有」比證明「沒有」容易, 這是認識論上就偏向抓 AI 而不是認人
- **lab01 三部曲回顧**: Day 16 (單指紋) → Day 17 (綜合指紋) → Day 18 (翻面問題) 連成一條脈絡
- **後續可能的 lab02+**: 中文冗詞偵測 (V07 已 preview)、人味正向 signal、時間維度 (同一作者前後期變化)

---

## 邊界 — Caveats

TODO

---

## 我的重點 — Takeaways

TODO

---

## Sources

TODO
