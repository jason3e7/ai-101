---
title: "1Password × OpenAI Codex：AI Agent 用憑證但永遠看不到秘密值"
tags: [ai, 外部觀點, 資安, 1password, codex, mcp, 憑證, agent, credential, secret]
source: https://www.ithome.com.tw/news/176055
author: iThome
created: 2026-05-22
---

# 1Password × OpenAI Codex：AI Agent 用憑證但永遠看不到秘密值

> [!info]
> iThome 報導：[1Password整合OpenAI Codex，讓AI代理使用憑證但又不取得秘密值](https://www.ithome.com.tw/news/176055)（2026-05-20）
> **一句話：** 1Password 推出本地 MCP Server，讓 Codex 可以呼叫資料庫、API、部署流程所需的憑證，但秘密值永遠不進入模型 context、不寫入磁碟、不出現在輸出裡。

---

## 問題背景

AI coding agent（Codex、Claude Code 等）做的事情愈來愈「真實」：
操作資料庫、呼叫 API、執行部署——這些都需要憑證。

過去的做法：把 API key、資料庫密碼放進 `.env`，或直接貼到 prompt 裡。
問題是 **secrets 一旦進入模型 context，就有可能被記錄、被輸出、被洩漏**。

> 「每個 AI Agent 都需要憑證，但沒有任何一個應該持有憑證。」— 1Password

---

## 解法：本地 MCP Server 作為中介層

**1Password Environments MCP Server for Codex** 的設計原則：

```
Codex                    本地 MCP Server              1Password Vault
  │                           │                              │
  │─── 請求：我需要 DB 連線 ──▶│                              │
  │                           │─── 驗證 + 請用戶確認 ────────▶│
  │                           │◀─── 用戶批准 ─────────────────│
  │◀── 「已注入，可以執行了」───│                              │
  │                           │                              │
  └── 執行任務（process 裡直接有 secrets，Codex 從未看到值）
```

**關鍵**：Codex 只能管理「變數名稱」和「環境設定結構」，
1Password 在 runtime 直接把值注入進應用程式的 process memory——**繞過 Codex 完全不經過它**。

---

## Secrets 碰不到的地方

| 位置 | 狀態 |
|---|---|
| Codex context window | ❌ 永不進入 |
| MCP 通道 | ❌ 不透過 MCP 傳遞 |
| 磁碟（.env、log）| ❌ 不寫入 |
| Terminal / prompt 輸出 | ❌ 不出現 |
| 模型輸出 | ❌ 不出現 |
| Process memory（執行當下）| ✅ 只在這裡，執行完即消失 |

---

## 執行流程

1. 開發者叫 Codex 執行需要憑證的任務
2. Codex 透過本地 MCP Server 向 1Password 發出請求
3. **1Password Desktop App 彈出確認視窗，要求用戶明確批准**
4. 用戶批准後，1Password 把 secrets 注入進目標 process 的 runtime
5. Codex 繼續執行任務——它知道「可以用了」，但從未取得實際值

> [!tip] 每次都要批准
> 每一次 Codex 和 1Password 的互動都需要用戶明確授權，AI 無法靜默繞過。

---

## 更大的意義：AI Agent 身分管理問題

這個整合背後代表的是一個更根本的問題：
**AI Agent 加入工作流程後，它是什麼身分？應該有多少權限？**

傳統憑證管理是設計給「人」或「固定服務」的，
但 AI Agent 是：
- 暫時性的（任務完成就消失）
- 行為不完全可預測的
- 可能被 prompt injection 操控的

**Just-in-time 憑證**（只在需要的當下注入，用完即失效）
是目前最務實的解法方向。

---

## 相關脈絡

| 工具 / 事件 | 說明 |
|---|---|
| 1Password Environments MCP Server | 本文主題，給 Codex 用 |
| 1Password + Claude Code | 同一套架構，也支援 Claude Code |
| Keycard | 類似定位的憑證委派工具（同期發布）|
| OWASP FinBot CTF | AI Agent 被 prompt injection 操控後取得憑證是主要風險場景之一 |

---

## 相關筆記

- [[OWASP FinBot CTF：針對 AI Agent 的資安挑戰]] — Prompt injection 操控 Agent 取得憑證的攻擊場景
- [[Nuclei 結合 LLM Agents 完整指南 — BASHCAT]] — AI Agent 執行資安工具的另一面
- [[AI 101 - Claude Code 生態系]] — MCP 架構說明

---

## 來源

- iThome：[1Password整合OpenAI Codex，讓AI代理使用憑證但又不取得秘密值](https://www.ithome.com.tw/news/176055)
- 1Password 官方部落格：[1Password is now a trusted access layer for OpenAI's Codex](https://1password.com/blog/1password-trusted-access-layer-for-openai-codex)
- SiliconANGLE：[1Password extends OpenAI collaboration with Codex MCP server](https://siliconangle.com/2026/05/20/1password-extends-openai-collaboration-codex-mcp-server-just-time-credential-access/)
- SecurityWeek：[1Password Teams With OpenAI to Stop AI Coding Agents From Leaking Credentials](https://www.securityweek.com/1password-teams-with-openai-to-stop-ai-coding-agents-from-leaking-credentials/)
