---
title: "Anthropic Cybersecurity Skills：754 個 AI 資安技能庫"
tags: [ai, 外部觀點, 資安, skill, claude-code, mcp, mitre, dfir, threat-hunting, red-team, ThreatVector]
source: https://www.facebook.com/threatvector/posts/pfbid0Uq8n9bo5X1574rYP62maz4GadcQCT259dRfYrZqtpTFezMhWH9eixL7o1C78rDqhl
author: ThreatVector
created: 2026-05-27
---

# Anthropic Cybersecurity Skills：754 個 AI 資安技能庫

> [!info]
> Facebook 原文：[ThreatVector](https://www.facebook.com/threatvector/posts/pfbid0Uq8n9bo5X1574rYP62maz4GadcQCT259dRfYrZqtpTFezMhWH9eixL7o1C78rDqhl)
> GitHub：[mukul975/Anthropic-Cybersecurity-Skills](https://github.com/mukul975/Anthropic-Cybersecurity-Skills)
> **一句話：** 社群建立的 754 個 production-grade 資安 Skill，涵蓋 DFIR、Threat Hunting、Red Team、Cloud Security 等，對應 MITRE ATT&CK / D3FEND / ATLAS 等五大框架，可直接接上 Claude Code 和 MCP。

---

## 是什麼

由社群貢獻、Apache 2.0 開源的 AI Agent 資安技能庫，
讓資安人員可以用自然語言呼叫結構化的資安工作流程。

安裝一行指令：

```bash
npx skills add mukul975/Anthropic-Cybersecurity-Skills
# 或
git clone https://github.com/mukul975/Anthropic-Cybersecurity-Skills.git
```

---

## 規模

| 項目 | 數值 |
|---|---|
| 技能數量 | 754 個 |
| 涵蓋安全領域 | 26 個 |
| 對應框架 | 5 個 |
| 相容 AI 平台 | 26+ 個 |

---

## 五大框架對應

| 框架 | 版本 | 涵蓋範圍 |
|---|---|---|
| **MITRE ATT&CK** | v18 | 14 個戰術、200+ 技術 |
| **NIST CSF** | 2.0 | 全部 6 個功能、22 個類別 |
| **MITRE ATLAS** | v5.4 | AI / ML 對抗性威脅 |
| **MITRE D3FEND** | v1.3 | 267 個防禦技術 |
| **NIST AI RMF** | 1.0 | AI 風險管理框架 |

---

## 26 個安全領域（部分）

| 類別 | 代表技能方向 |
|---|---|
| **DFIR** | 數位取證、事件回應 |
| **Threat Hunting** | 威脅獵捕、行為分析 |
| **AppSec** | 應用程式安全、程式碼審查 |
| **Cloud Security** | AWS / Azure / GCP 資安 |
| **Red Teaming** | 滲透測試、攻擊模擬 |
| **Malware Analysis** | 惡意程式分析 |
| **Penetration Testing** | 主動滲透測試 |

---

## 技能格式

每個 skill 遵循 [agentskills.io](https://agentskills.io) 標準：

```yaml
---
name: threat-hunt-lateral-movement
framework: MITRE ATT&CK
tactic: Lateral Movement
technique: T1021
platforms: [Claude Code, Cursor, Gemini CLI]
---

# Threat Hunt: Lateral Movement Detection
...（結構化工作流程）
```

YAML frontmatter 供 AI 自動發現，Markdown 內容是執行工作流程。

---

## 相容平台

Claude Code、GitHub Copilot、Cursor、Gemini CLI，以及任何支援 agentskills.io 標準的 MCP 平台。

---

## 社群反應

- Facebook 貼文：425+ 讚、298 分享
- 社群評論：「約 120 個 skill 是真正實用的」——說明 754 個裡有相當比例是 production-grade，但使用者仍需判斷哪些適合自己的環境

> [!tip] 選用建議
> 754 個全部用可能太多，建議依自己的工作領域（DFIR / Red Team / Cloud）先篩選出相關的 subset 使用。

---

## 相關筆記

- [[Nuclei 結合 LLM Agents 完整指南 — BASHCAT]] — AI Agent 結合資安工具的另一個角度
- [[OWASP FinBot CTF：針對 AI Agent 的資安挑戰]] — AI Agent 本身的安全邊界
- [[AI 101 - Claude Code 生態系]] — Skills 架構與安裝說明
- [[六個 AI 互相入侵伺服器實驗 — 林亦LYi]] — AI 在資安攻防的能力邊界

---

## 來源

- Facebook：[ThreatVector](https://www.facebook.com/threatvector/posts/pfbid0Uq8n9bo5X1574rYP62maz4GadcQCT259dRfYrZqtpTFezMhWH9eixL7o1C78rDqhl)
- GitHub：[mukul975/Anthropic-Cybersecurity-Skills](https://github.com/mukul975/Anthropic-Cybersecurity-Skills)
