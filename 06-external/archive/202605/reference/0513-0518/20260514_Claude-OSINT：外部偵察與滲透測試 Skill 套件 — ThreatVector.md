---
title: Claude-OSINT：外部偵察與滲透測試 Skill 套件
tags: [ai, 外部觀點, claude-code, osint, 資安, red-team, bug-bounty, skill]
source: https://www.facebook.com/threatvector/posts/pfbid0wRB6PWRCfTPbwRNN7qu87YsjUVUmYZ1tnR93rWn5au6Kyv4PKXxQNy6BiukqPeRMl
author: ThreatVector
github: https://github.com/elementalsouls/Claude-OSINT
created: 2026-05-14
---

# Claude-OSINT：外部偵察與滲透測試 Skill 套件

> [!warning] 僅限授權使用
> 本工具設計用於 **合法授權的滲透測試、漏洞獎勵計畫（Bug Bounty）、安全研究**。
> 未經授權對任何系統使用偵察工具屬於違法行為。

> [!info]
> GitHub：[elementalsouls/Claude-OSINT](https://github.com/elementalsouls/Claude-OSINT)
> **一句話：** 把 90+ 個 OSINT 偵察模組封裝成 Claude Code Skill，讓 Claude 具備系統性的外部攻擊面分析能力。

---

## 是什麼

**Claude-OSINT** 是一組 `SKILL.md` 檔案，安裝進 Claude Code 後，
讓 Claude 能執行專業等級的開源情報（OSINT）收集與外部偵察工作。

規模：
- 2 個配對 Claude Skills
- 90+ 偵察模組
- 48 個 Secret Regex 模式（偵測洩漏的 API key、憑證）
- 80+ 搜尋 Dork
- 27 個攻擊路徑模板
- 5,500+ 行結構化攻擊情報（tradecraft）

---

## 涵蓋的偵察面向

| 類別 | 能做什麼 |
|---|---|
| **資產發現** | 子網域、IP 範圍、關聯資產枚舉 |
| **M365 / SSO 偵察** | 識別組織使用的 Microsoft 服務與 SSO 配置 |
| **Secret / API Key 獵捕** | 在公開來源掃描洩漏的憑證與金鑰 |
| **GraphQL / Swagger 探索** | 找出未保護或過度暴露的 API 文件端點 |
| **雲端 / K8s / CI-CD 暴露** | 偵測錯誤配置的雲端儲存、容器、Pipeline |
| **供應商指紋識別** | 識別目標使用的技術棧與第三方服務 |
| **洩漏情報** | 查詢 Breach 資料庫中的相關資訊 |
| **社交圖譜** | 員工、組織架構、社群曝光面分析 |

---

## 安裝方式

把 `SKILL.md` 檔案放進 Claude Code 的 Skills 目錄即可（drop-in）：

```bash
# 從 GitHub 取得
git clone https://github.com/elementalsouls/Claude-OSINT

# 把 skill 檔放進你的專案或全域 skills 目錄
cp Claude-OSINT/*.md ~/.claude/skills/
```

---

## 適用情境

- **Red Team** — 模擬真實攻擊者的偵察流程
- **Bug Bounty** — 快速建立目標攻擊面地圖
- **Attack Surface Management** — 持續監控組織的外部暴露面
- **安全研究** — 系統性分析特定技術的已知暴露模式

---

## 為什麼值得關注（從 AI 工具角度）

這個專案示範了 Claude Code Skills 的一個重要使用模式：
**把深度領域專業知識（5,500+ 行 tradecraft）封裝成可重複使用的 Skill，
讓不具備該專業背景的人也能執行專業級的工作流。**

這不只適用於資安——同樣的模式可以套用在任何需要系統化專業知識的領域。

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — Skills 的概念與架構
- [[AI 101 - Harness Engineering]] — 如何設計安全可靠的 Agent 執行框架
