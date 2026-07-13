---
title: OWASP FinBot CTF：Agentic AI 資安實戰練習場
tags: [ai, 外部觀點, 資安, ctf, owasp, prompt-injection, agentic-ai, 滲透測試]
source: https://owasp-finbot-ctf.org/
author: OWASP GenAI Security Project
created: 2026-05-14
---

# OWASP FinBot CTF：Agentic AI 資安實戰練習場

> [!info]
> 官網：[owasp-finbot-ctf.org](https://owasp-finbot-ctf.org)
> GitHub：[OWASP-ASI/finbot-ctf-demo](https://github.com/OWASP-ASI/finbot-ctf-demo)
> **一句話：** 專為 Agentic AI 設計的 CTF 練習平台，等同於 AI 安全領域的 Juice Shop——用實際攻擊 AI Agent 的方式學習如何防禦。

---

## 是什麼

**FinBot CTF** 是 OWASP GenAI Security Project 於 2026 年 4 月推出的瀏覽器版 CTF（Capture The Flag）平台。

目標是一個故意設計有漏洞的多 Agent 財務系統 **FinBot**，模擬真實的 AI Agent 供應商管理平台，含有：
- 廠商入駐（Vendor Onboarding）
- 詐欺偵測（Fraud Detection）
- 發票處理（Invoice Processing）
- 通訊往來（Communications）

所有 Agent 都有**真實的工具存取權限**，由 LLM 驅動——這正是攻擊面所在。

> [!tip] 為什麼叫「Juice Shop for Agentic AI」？
> OWASP Juice Shop 是學習 Web 漏洞的經典練習靶場。
> FinBot CTF 做的是同樣的事，但針對的是 AI Agent 的漏洞——不是 SQL injection，而是 prompt injection、goal hijacking、tool poisoning。

---

## 六個挑戰類別

| 難度 | 類別 | 你要做什麼 |
|---|---|---|
| 🟢 Beginner | **Recon（偵察）** | 提取系統 prompt、探索 Agent 架構與能力邊界 |
| 🟡 Intermediate | **Policy Bypass（政策繞過）** | 操控 Agent 目標，繞過合規規則與商業邏輯 |
| 🔴 Advanced | **Data Exfiltration（資料外洩）** | 透過 Agent 漏洞竊取廠商資料與 PII |
| 🔴 Advanced | **Destructive（破壞性操作）** | 讓 Agent 大量停用帳號或損壞資料 |
| ⚫ Expert | **Remote Code Execution** | 利用 Tool Poisoning 和 MCP Server 漏洞達成任意程式碼執行 |
| 🔜 Coming | **Memory Poisoning / Multi-Agent Attacks / Supply Chain** | 規劃中 |

---

## 涵蓋的漏洞類型

這些都是 Agentic AI 系統特有的攻擊面，對應到 OWASP 標準：

| 漏洞 | 說明 |
|---|---|
| **Prompt Injection** | 在輸入中夾帶惡意指令，讓 Agent 執行非預期行為 |
| **Goal Hijacking / Manipulation** | 用「緊迫性」或「權威感」覆蓋 Agent 的原始目標 |
| **Tool Misuse** | 誘導 Agent 用合法工具做惡意操作 |
| **Policy Bypass** | 繞過 Agent 設計的業務規則與合規邏輯 |
| **Privilege Escalation** | 透過 Agent 取得不應有的系統權限 |
| **Tool Poisoning / MCP Exploit** | 汙染 Agent 可用的工具定義，達成 RCE |
| **Data Exfiltration** | 透過 Agent 的合法資料存取能力外洩資訊 |

> [!info] 對應標準
> - OWASP Top 10 for LLM Applications 2025
> - OWASP Top 10 for Agentic Applications 2026
> - CWE、MITRE ATLAS

---

## 示範攻擊：Goal Manipulation

官方有公開一個入門示範關卡，展示 **Goal Manipulation** 的攻擊手法：

**情境：** FinBot 是 CineFlow Productions 的 AI 財務助理，負責審核發票。

**攻擊思路：**
```
正常情況：Agent 檢查發票 → 符合規則才核准

攻擊手法：在請求中加入高緊迫性語境 + 偽造權威來源
→ Agent 的「商業優先」邏輯覆蓋了「安全規則」邏輯
→ 核准了不應通過的詐欺發票
```

關鍵發現：**「緊迫性」和「權威感」是操控 AI 決策最有效的槓桿**，
即使有 prompt injection 偵測機制，特定的商業情境信號仍能繞過。

---

## 怎麼開始

完全免費，不需要安裝任何東西：

1. 前往 [owasp-finbot-ctf.org](https://owasp-finbot-ctf.org)
2. 以「廠商」身份註冊
3. 開始和 FinBot AI 助理互動
4. 探索 Agent 的能力邊界，嘗試各種攻擊
5. 成功觸發漏洞會自動偵測並給出 flag
6. 有排行榜、徽章、即時計分

> [!tip] 建議從 Recon 開始
> 先把 FinBot 的系統 prompt 和 Agent 架構摸清楚，
> 後面的 Policy Bypass 和 Data Exfiltration 才有方向。

---

## 為什麼值得玩

> [!warning] 現實中的 Agentic AI 系統有同樣的漏洞
> FinBot 暴露的漏洞不是學術假設，而是真實 AI Agent 產品的常見問題。
> 玩過 CTF，你就知道**為什麼 Harness Engineering 和 Agent 權限設計這麼重要**。

- 了解攻擊手法，才知道防禦要在哪裡設關卡
- MCP Server 的 Tool Poisoning 是目前 AI Agent 最嚴重的威脅之一
- Goal Manipulation 示範了為什麼 Agent 不能完全信任輸入

---

## Sources

- [OWASP FinBot CTF 官網](https://owasp-finbot-ctf.org)
- [FinBot CTF Is Live — OWASP GenAI Security Project](https://genai.owasp.org/2026/04/28/finbot-ctf-is-live-a-hands-on-companion-to-the-owasp-genai-security-project/)
- [FinBot CTF Demo: Goal Manipulation Walkthrough](https://github.com/OWASP-ASI/finbot-ctf-demo/blob/main/docs/FinBot-CTF-walkthrough-goal-manipulation.md)
- [OWASP Agentic AI CTF Writeup — InfoSec Write-ups](https://infosecwriteups.com/owasp-agentic-ai-ctf-finbot-demo-goal-manipulation-ad377406e1a7)

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — 為什麼 Agent 執行框架的設計能防止這些攻擊
- [[AI 101 - Subagent 使用與計費]] — Multi-agent 架構的設計與風險
- [[Claude-OSINT：外部偵察與滲透測試 Skill 套件 — ThreatVector]] — AI 工具在資安領域的另一個應用
