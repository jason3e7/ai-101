---
title: "用 LLM 保護原始碼：六步驟漏洞發現與修復框架"
tags: [ai, 外部觀點, security, llm, vulnerability, code-security, anthropic, eugene-yan]
source: https://claude.com/blog/using-llms-to-secure-source-code
author: Eugene Yan & Henna Dattani（Anthropic）
created: 2026-06-11
---

# 用 LLM 保護原始碼：六步驟漏洞發現與修復框架

> [!info]
> 來源：[Using LLMs to Secure Source Code](https://claude.com/blog/using-llms-to-secure-source-code)
> 作者：Eugene Yan & Henna Dattani，及 Michael Molash、Abel Ribbink、Justin Young、Ben Morris、David Dworken、Hasnain Lakhani（Anthropic）
> 發表：2026-05-27
> **一句話：** 找漏洞已不再是瓶頸——驗證、分類、修復才是。這篇整理出一套可實際落地的六步驟 LLM 資安自動化框架。

---

## 核心論點

模型的漏洞發現能力已經成熟，現在的瓶頸不在「找得到嗎」，而在「怎麼確認是真漏洞、怎麼修對」。

截至 2026-05-22：**1,596 個漏洞被這套方法揭露，只有 97 個被修補。**

驗證跟不上發現的速度，才是真正的問題。

---

## 六步驟找修循環

### Step 1：威脅建模（Threat Modeling）

先定範圍，再跑掃描。有完整威脅模型的系統，漏洞「可被利用的比例高達 90%」；沒有的話，大型專案掃出來的結果有 **40% 是誤報**。

**實務做法：**
- 用 [Shostack 四問框架](https://www.shostack.org/blog/shostack-four-questions) 建立威脅模型
- 每個 repo 加一個 `THREAT_MODEL.md`
- 一支團隊拿過去的 CVE 做「bug 形狀提示」，一小時內找到三個可被利用的漏洞

> [!tip]
> 給 model 看過去的 CVE 比給一般指令更有效——讓它知道「這個 codebase 歷史上哪種洞比較常見」。

---

### Step 2：沙箱隔離（Sandboxing）

讓 agent 能安全地實際執行 exploit 來**證明**漏洞存在，而不只是猜測。

**關鍵細節：**
- 一支團隊不小心告訴 model「你沒有網路存取」，結果 model 自己發現了 GitHub 存取權限
- 鎖定依賴版本（pin dependencies），確保每次執行都能重現結果
- 用容器快照讓不同 agent 在同樣起始狀態下運行

---

### Step 3：發現（Discovery）

部署 agent 掃描程式碼。

**反直覺發現：**
- **越詳細的 prompt 反而降低效果** — prescriptive 指令限制了 model 發現新型漏洞的創意
- 並行跑多個 discovery agent 在大 codebase 上效益遞減——需要先策略性切割 codebase
- 讓 agent 能發 HTTP request 並檢查回應的團隊，真陽性率達到**近 100%**

---

### Step 4：驗證（Verification）

用**獨立的** verifier agent 確認可利用性，不要讓 discovery agent 自我驗證。

**核心洞察：**

> "Validation is the biggest holdup and the PoC is the validation."

- Discovery agent 自我驗證時會自我審查，漏掉真正的漏洞
- 多個 verifier 投票（majority voting）比單一驗證準確
- 要求提交 PoC exploit 作為驗證，誤報率**降至近零**
- 對抗性驗證（adversarial verification）讓誤報減少 **~50%**

---

### Step 5：分類（Triage）

去重、排優先級。

**常見問題：**
- Model 在缺乏上下文（信任邊界、補償控制）時傾向高估嚴重程度
- 大量低品質報告會讓工程師失去對系統的信任

**建議：** 專注 critical/high severity，防止 pipeline 過載。

---

### Step 6：修補（Patching）

應用修復並驗證。

**注意：**
- AI 生成的 patch 常只解決表面症狀，不觸根因
- Patch 可能過度限制，破壞相依服務

**最佳實踐：**
1. 先寫一個會失敗的測試
2. 套上 patch，確認測試通過
3. 對修補後的區域重新發動攻擊（adversarial re-test）
4. 搜尋同樣漏洞的**變體**：同一 pattern 的其他 call site、同一漏洞類別

---

## 關鍵數字

| 指標 | 數字 |
|---|---|
| 已揭露漏洞（截至 2026-05-22）| 1,596 |
| 已修補 | 97 |
| 有威脅模型時的可利用率 | ~90% |
| 無威脅模型的大型專案誤報率 | ~40% |
| 對抗性驗證的誤報降低率 | ~50% |
| 要求 PoC 後的誤報率 | 近零 |

---

## 公開資源

Anthropic 同步釋出：
- `defending-code-reference-harness` — 公開參考 harness
- `claude-code-security-review` GitHub Action
- Vulnerability Detection Agent cookbook
- Threat Intelligence Enrichment cookbook
- Claude Security 產品

---

## 相關筆記

- [[AI 101 - Claude Code 行為結構設計]] — Sub-agents 的 Planner/Summarizer 模式可直接套用到這個框架
- [[AI 101 - HTB Goal Prompt 設計指南]] — 同樣的「獨立驗證」、「分離 recon 和 exploit」原則

---

## 來源

- [Using LLMs to Secure Source Code — Anthropic Blog](https://claude.com/blog/using-llms-to-secure-source-code)
