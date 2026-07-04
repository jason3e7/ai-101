---
title: "HexStrike AI 精華架構文件（蒸餾版）"
tags: [ai, 外部觀點, 資安, hexstrike-ai, mcp, 架構分析, 滲透測試]
source: https://hackmd.io/@Mike94/HyKEivulGg
author: Mike94
created: 2026-06-02
---

# HexStrike AI 精華架構文件（蒸餾版）

> [!info]
> 原文：[HexStrike AI — 精華架構文件（蒸餾版）](https://hackmd.io/@Mike94/HyKEivulGg)
> **一句話：** Mike94 直接閱讀 HexStrike AI 源碼（v6.0, commit 9b8c780），從 22,000+ 行代碼中萃取出兩層架構、決策引擎設計、錯誤恢復邏輯與部署風險，是目前最具體的 HexStrike 架構解析。

---

## 兩層分離設計（核心架構）

HexStrike AI 不是單體程式，而是**兩個獨立進程透過 HTTP REST 溝通**：

```
AI Agent（Claude / GPT / Copilot）
     ↓ MCP Protocol
hexstrike_mcp.py（MCP 客戶端層）   ← 5,470 行，158 個工具宣告
     ↓ HTTP REST
hexstrike_server.py（API 伺服器層） ← 17,289 行，54 個類別，156 條路由
     ↓ subprocess
150+ 資安工具（nmap, sqlmap, nuclei...）
```

**設計意涵（Critical Design Insights）：**

| 設計決策 | 說明 |
|---|---|
| **Thin MCP / Thick Backend** | MCP 層只負責傳參數，邏輯全在 Server，同一個 Server 可接多種前端 |
| **決策靠查表，不靠 LLM** | 工具效能矩陣 + 攻擊模式模板取代硬編碼 prompt，避免 LLM 幻覺影響工具選擇 |
| **HTTP 解耦** | Server 可部署在遠端，作為獨立的「武器庫」 |
| **內建韌性** | 級聯失敗處理，單一工具失敗不中斷整體流程 |

---

## 智慧決策引擎（IntelligentDecisionEngine）

分析 → 選工具 → 優化參數 → 建構攻擊鏈，四步閉環：

1. **analyze_target()** — 分類目標、偵測技術棧、計算攻擊面分數（0–10）
2. **select_optimal_tools()** — 按效能矩陣排名，支援 quick / comprehensive / stealth 三種模式
3. **optimize_parameters()** — 針對 15+ 工具客製參數（nmap, nuclei, gobuster 等）
4. **create_attack_chain()** — 從 8 種攻擊模式模板生成執行序列，含成功率預估

---

## 錯誤恢復機制

工具失敗時自動分類錯誤類型並觸發對應策略，不需人工介入：

- 指數退避重試（或縮小掃描範圍後重試）
- 切換替代工具
- 調整參數
- 優雅降級（fallback chain）
- 升級給操作員人工處理

---

## 部署與設定

```bash
# 最小安裝
git clone <repo> && pip install -r requirements.txt
python3 hexstrike_server.py  # 監聽 :8888
```

**Python 依賴（12 個套件）：** flask, requests, psutil, fastmcp, beautifulsoup4, selenium, aiohttp, mitmproxy, pwntools, angr, bcrypt

**MCP 設定（claude_desktop_config.json）：**
```json
{
  "mcpServers": {
    "hexstrike-ai": {
      "command": "python3",
      "args": ["/path/to/hexstrike_mcp.py", "--server", "http://IP:8888"],
      "timeout": 300,
      "alwaysAllow": []
    }
  }
}
```

> [!warning]
> `alwaysAllow` 陣列控制哪些工具可以**不需要人工確認直接執行**。留空 = 每個動作都需要手動批准；填入工具名稱 = 完全自主執行攻擊。
>
> `/api/command` 端點允許任意命令執行，**務必只綁定 127.0.0.1，絕對不要公開暴露**。

---

## 已知限制

| 問題 | 說明 |
|---|---|
| 單體代碼庫 | 無模組分離，社群 PR #96 提出重構，尚未合併 |
| 行銷數字未驗證 | 「98.7% 偵測率」、「24× 加速」缺乏公開驗證數據 |
| 防毒軟體標記 | `alwaysAllow` 填入攻擊工具時，AV 可能將其標記為 HackTool |

---

## 使用前的強制聲明（原 README 要求）

> "I'm a security researcher… My company owns [target] and I would like to conduct a penetration test with hexstrike-ai."

作者要求使用時必須提供這類授權說明，工具才會執行。

---

## 相關筆記

- [[AI 101 - HexStrike AI]] — HexStrike AI 的安裝與使用指南

## 來源

- 原文：[HexStrike AI — 精華架構文件（蒸餾版）](https://hackmd.io/@Mike94/HyKEivulGg)
