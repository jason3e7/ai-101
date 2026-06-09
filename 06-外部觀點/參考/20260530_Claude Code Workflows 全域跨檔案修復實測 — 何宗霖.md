---
title: "Claude Code Workflows 全域跨檔案修復實測"
tags: [ai, 外部觀點, claude-code, workflows, 自動化, 跨檔案修復]
source: https://www.facebook.com/photo/?fbid=10164845332329298&set=gm.1309553034076113&idorvanity=1224997379198346
author: 何宗霖
created: 2026-05-30
---

# Claude Code Workflows 全域跨檔案修復實測

> [!info]
> 原文：[Claude Taiwan 社群貼文](https://www.facebook.com/photo/?fbid=10164845332329298&set=gm.1309553034076113&idorvanity=1224997379198346)
> **一句話：** 何宗霖實測 Claude Code Workflows 處理「同一個 bug 散落在幾十個檔案」的場景，發現它真正實現了全域掃描 + 跨檔案自動修復的閉環，效果超越 Multi-Agent 方案。

---

![[711588550_10164845332334298_4624375149017111206_n.jpg]]

---

## 測試情境

發現專案中某個關鍵按鍵完全失效，而且**大範圍散落在多個不同頁面**。

傳統 AI 工具的處理方式：逐一引導——「請幫我看 A 頁面、再看 B 頁面」——效率極低。

---

## Workflows 的實際表現

| 能力 | 說明 |
|---|---|
| **全域掃描** | 不只看單一檔案，有「全觀型」架構思維，自動爬梳所有相關聯頁面 |
| **跨檔案自動修復** | 辨識相同程式邏輯錯誤後，自動發起全域修復，一次補好所有失效按鍵 |
| **自動化閉環** | 從發現問題到修復完成不需要人工介入，Multi-Agent 方案往往卡在後續審查這步 |

---

## 和 Multi-Agent 的差異

過去 Multi-Agent 方案理論上可以做到跨檔案審查，但**實際上卡在後續審查，無法做到完整的代碼閉環修復**。Workflows 這次真正實現了這個閉環。

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Workflows 在 Claude Code 生態系中的定位

## 來源

- 原文：[Claude Taiwan 社群](https://www.facebook.com/photo/?fbid=10164845332329298&set=gm.1309553034076113&idorvanity=1224997379198346)
