---
title: "當漏洞掃描學會說人話：Nuclei 結合 LLM Agents 完整指南"
tags: [ai, 外部觀點, 資安, nuclei, llm, agent, mcp, devsecops, pentest, 漏洞掃描]
source: https://hackmd.io/@BASHCAT/SyePFm5jWe
author: BASHCAT
created: 2026-05-22
---

# 當漏洞掃描學會說人話：Nuclei 結合 LLM Agents 完整指南

> [!info]
> 原文：[hackmd.io/@BASHCAT/SyePFm5jWe](https://hackmd.io/@BASHCAT/SyePFm5jWe)
> **一句話：** 四個層級把 Nuclei 漏洞掃描器接上 LLM——從一行指令自然語言掃描，到完全自主的滲透測試 Agent。

---

## 什麼是 Nuclei

[Nuclei](https://github.com/projectdiscovery/nuclei) 是 ProjectDiscovery 開發的開源漏洞掃描器，
用 YAML 模板定義偵測邏輯，社群維護超過萬個模板。

過去你要掃描漏洞，需要先選模板、看文件、手動組合指令。
**接上 LLM 之後，用自然語言說出你要找什麼，AI 幫你產出模板並執行。**

---

## 四個整合層級

### Level 1：`-ai` 旗標（5 分鐘上手）

直接用自然語言描述你想找的漏洞：

```bash
nuclei -auth   # 設定 API key

nuclei -ai "check for XSS in query parameters" -u https://target.com
nuclei -ai "detect open redirect on login page" -u https://target.com
nuclei -ai "find SQL injection in search functionality" -list urls.txt
```

**運作方式**：自然語言 prompt 送到 ProjectDiscovery 的 AI 後端，生成 YAML 模板後直接執行。

> [!warning] 限制
> - 依賴外部 API（ProjectDiscovery）；不適合掃內網或機密系統
> - 免費方案有速率限制
> - 無法使用本地模型；要用本地模型的話看 HackingBuddyGPT

---

### Level 2：MCP Server 整合

讓 AI Agent（如 Claude Desktop）直接控制 Nuclei 的掃描決策與分析。

**Claude Desktop 設定：**

```json
{
  "mcpServers": {
    "nuclei": {
      "command": "go",
      "args": ["run", "cmd/nuclei-mcp/main.go"]
    }
  }
}
```

設定完後，你可以直接對 Claude 說：
> 「掃描 example.com，重點放在 nginx 和 Node.js 相關漏洞」

Claude 會自動選模板、執行掃描、解析 JSON 結果、用人話解釋發現。

**可用的 MCP 實作：**

| 專案 | 語言 | 特色 |
|---|---|---|
| addcontent/nuclei-mcp | Go | 模板管理、結果快取、並發操作 |
| crazyMarky/mcp_nuclei_server | Python | Tag 篩選、JSON 輸出 |
| FuzzingLabs/mcp-security-hub | 多語言 | Nmap + Ghidra + Nuclei + SQLMap 四合一 |

---

### Level 3：DevSecOps Pipeline 自動化

從 CVE 發布到偵測模板上線，壓縮到數小時內：

```
新 CVE 發布
  └── n8n 觸發
        └── LLM 萃取漏洞細節
              └── 生成 Nuclei 模板
                    └── 自動掃描資產
                          └── 存入知識庫 → 通知資安團隊
```

ProjectDiscovery 的 **nuclei-templates-ai** 專案已在做這件事：
監控 CVE → AI 生成模板 → 社群 review → 合併進官方模板庫。

---

### Level 4：自主 Agent 架構

定義好範圍和允許工具，讓 Agent 完全自主執行滲透測試：

```yaml
scope:
  targets:
    - example.com
  allowed_tools:
    - nuclei
    - nmap
    - sqlmap
  constraints:
    - no_dos_attacks
    - stay_in_scope
```

執行流程：偵察 → 攻擊面發現 → 工具選擇 → 測試執行 → 漏洞驗證 → 報告

> [!warning] 必要措施
> 自主 Agent 可能超出預期行為，**務必設定嚴格範圍限制，在隔離環境執行**。

---

## 工具生態

### 自主滲透測試 Agent

| 工具 | 特色 |
|---|---|
| **CAI**（Alias Robotics）| 模組化框架，可追蹤推理鏈，學術背書（arXiv:2504.06017）|
| **PentAGI** | 知識圖記憶，跨任務持久學習 |
| **Strix** | 整合 Nuclei + Caido + Playwright |
| **HexStrike AI** | 單一 MCP Server 整合 150+ 安全工具 |

### MCP 安全 Hub

| 工具 | 說明 |
|---|---|
| **Pentest-MCP-Server** | 在 Kali Docker 中執行，強制 scope.yaml 限制 |
| **Garak**（NVIDIA）| 測試 LLM 本身的漏洞（Prompt injection、Jailbreak）|

---

## 依場景選擇方案

| 場景 | 建議方案 |
|---|---|
| 個人開發者 | `nuclei -ai`（5 分鐘上手）|
| 資安團隊 | MCP Server + Claude Desktop（監督式掃描）|
| 專業/Bug Bounty | CAI / PentAGI + HexStrike AI |

---

## 已知限制與注意事項

> [!warning]
> 1. **AI 生成模板錯誤率約 20–30%**——matcher 邏輯和邊緣案例仍需人工審查
> 2. **`-ai` 旗標會把掃描意圖送到外部 API**——內網或機密系統請改用本地模型方案（HackingBuddyGPT）
> 3. **自主 Agent 可能超出預期行為**——嚴格範圍限制 + 隔離環境是必要條件
> 4. **工具生態碎片化**——大多數專案仍在早期，優先選有組織背書的（ProjectDiscovery、NVIDIA、Alias Robotics）

---

## 相關筆記

- [[OWASP FinBot CTF：針對 AI Agent 的資安挑戰]] — AI Agent 的攻防邊界
- [[六個 AI 互相入侵伺服器實驗 — 林亦LYi]] — AI 網路安全能力實測
- [[AI 101 - Claude Code 生態系]] — MCP 的架構說明

---

## 來源

- 原文：[hackmd.io/@BASHCAT/SyePFm5jWe](https://hackmd.io/@BASHCAT/SyePFm5jWe)
- Nuclei GitHub：[github.com/projectdiscovery/nuclei](https://github.com/projectdiscovery/nuclei)
- CAI 論文：[arXiv:2504.06017](https://arxiv.org/abs/2504.06017)
