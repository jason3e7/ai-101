---
title: "Cloudflare security-audit-skill：把編碼代理變資安審計員"
tags: [ai, 外部觀點, 參考, security, agent-skill, code-audit, cloudflare]
source: https://github.com/cloudflare/security-audit-skill
author: Cloudflare
created: 2026-07-13
---

# Cloudflare security-audit-skill：把編碼代理變資安審計員 — Turning a Coding Agent Into a Security Auditor

[← 回主頁](../../index.md)

> [!NOTE]
> 原文：[cloudflare/security-audit-skill — GitHub](https://github.com/cloudflare/security-audit-skill)
> **一句話：** Cloudflare 開源的 skill，裝上後叫編碼代理「幫這個 codebase 做資安稽核」，它會跑一條六階段流程——偵察、多角度攻擊、對抗驗證、產報告、結構化輸出、獨立查核。

> **TL;DR (EN):** An open-source skill from Cloudflare that turns a coding agent into a security auditor via a six-phase pipeline (recon → hunt → verify → report → structured output → independent verification). Multiple agents attack the code from different angles, then a separate agent adversarially reviews the findings. Just say "security audit this codebase". JS/Node, MIT.

---

## 這是什麼 — What It Is

一般叫 AI「看看有沒有漏洞」很容易得到零散、沒把握的結果。這個 skill 把稽核**流程化**，用多個代理分工，最後還互相查核。六個階段：

| 階段 | 在做什麼 |
|---|---|
| **1 偵察 Recon** | 多代理並行研究應用架構 |
| **2 狩獵 Hunt** | 多代理從不同角度攻擊（注入、存取控制、業務邏輯…） |
| **3 驗證 Verify** | 獨立代理對發現做**對抗式**審查 |
| **4 報告 Report** | 產出人看得懂的報告 + 詳細追蹤 |
| **5 結構化輸出** | 產出符合 JSON schema 的機器可讀發現 |
| **6 獨立查核** | 新代理逐條驗證所有主張的真偽 |

**支援攻擊面**：記憶體安全、LLM/AI 後端、Web 協議認證、瀏覽器/客戶端。

---

## 怎麼用與技術 — Usage & Stack

**用法**：裝好 skill 後，直接叫編碼代理：

```
security audit this codebase
find security vulnerabilities in ./src
```

**技術**：JavaScript（100%）+ Node.js（做 schema 驗證）。**授權**：MIT。

> [!TIP]
> 最值得學的是它的**設計哲學**：不信任單一代理的判斷，用「多角度攻擊 → 對抗審查 → 獨立查核」層層把關。這正是 [先驗證，再用它突破自己](../../05-notes/ai-verify-then-expand.md) 裡「獨立驗算」的工程化版本——換一個獨立代理再驗一次，才敢信。

---

## 來源 — Sources

- [cloudflare/security-audit-skill — GitHub](https://github.com/cloudflare/security-audit-skill)
