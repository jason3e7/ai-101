---
title: "Claude Fable 5 和 Mythos 5 發布公告"
tags: [ai, 外部觀點, claude, fable-5, mythos-5, anthropic, benchmark, safety, release]
source: https://www.anthropic.com/news/claude-fable-5-mythos-5
author: Anthropic
created: 2026-06-11
---

# Claude Fable 5 和 Mythos 5 發布公告

> [!info]
> 來源：[Claude Fable 5 and Claude Mythos 5](https://www.anthropic.com/news/claude-fable-5-mythos-5)（Anthropic）
> 發表：2026-06-09
> **一句話：** Fable 5（通用版，所有使用者可用）和 Mythos 5（限制存取，僅供特定生科和資安研究合作夥伴）同日發布，Fable 5 在軟體工程和知識工作 benchmark 上系統性領先前代和競爭對手。

---

## 兩個模型

| 模型 | 存取 | 定價 |
|---|---|---|
| **Claude Fable 5** | 所有使用者 | $10 / 百萬 input，$50 / 百萬 output tokens |
| **Claude Mythos 5** | 限制存取（Project Glasswing 合作夥伴） | 未公開 |

**Fable 5 免費使用期：** 2026-06-22 前，Pro / Max / Team / Enterprise 計畫免費（之後需使用 credits 直到容量恢復）。

Fable 5 的定價比前代 Mythos Preview 低不到一半。

---

## 軟體工程能力

| Benchmark | 得分 | 說明 |
|---|---|---|
| SWE-Bench Pro | 80.3% | 真實 GitHub Issue 端對端解決率 |
| Cognition FrontierCode Diamond | 業界最高 | 最難編程問題集，前代 Opus 4.8 得 13.4% |

**實際案例：** 在一天內完成一個 5,000 萬行 Ruby codebase 的遷移——人工團隊估計需要兩個月。

**客戶測試數據：**
- Cursor：CyberCode 達到 SOTA，解鎖之前無法處理的長週期問題
- GitHub：複雜長時間編程任務的自主可靠性超越先前 benchmark
- Anthropic 內部物理團隊：36 小時完成 GPT-5.5 需要 4 天才能做到的工作，且只使用三分之一的推理 token

---

## 知識工作與視覺能力

- **Hebbia Finance Benchmark**：最高分（高階分析推理）
- 文件分析、圖表解讀優於前代
- 只用截圖（無任何輔助）通關了電玩《Pokémon FireRed》
- 能從科學圖表抽取精確數字
- 能從截圖重建 Web App 原始碼

---

## 長 Context 能力

用《Slay the Spire》測試持久記憶（檔案記憶）：
- Fable 5 的進步幅度是 Opus 4.8 的 **3 倍**
- 到達最終關卡的頻率是 Opus 4.8 的 **3 倍**

---

## Mythos 5：生命科學能力（限制存取）

| 指標 | 數字 |
|---|---|
| 蛋白質設計加速 | ~10 倍 |
| 科學家對分子生物學假說的偏好（盲測）| ~80% 選擇 Mythos 5 |
| 14 個蛋白質目標中的強藥物設計候選 | 9 個 |

- 一個關於 E. coli 蛋白質的假說被後來獨立發表的研究所佐證
- 對 138 個動物物種的單細胞資料進行自主基因組學研究，建立了一個比近期 Science 期刊論文更優秀、但小 100 倍的 ML 模型

---

## 安全架構

### 分類器覆蓋三個領域

| 類別 | 說明 |
|---|---|
| 網路安全 | 攻擊性技術、惡意軟體、exploit 工具 |
| 生物 / 化學 | 實驗室方法、分子機制 |
| 模型蒸餾 | 防止提取模型的「摘要思考」 |

### 降版機制（Fallback）

- 分類器觸發時，自動切換到 Claude Opus 4.8 執行並通知使用者
- **超過 95% 的 Fable 5 對話不會觸發任何降版**
- 外部紅隊超過 **1,000 小時**測試未發現通用 jailbreak
- 一個合作夥伴確認：30 種公開 jailbreak 技術均未使其遵從有害的網路攻擊請求

### Mythos 5 限制

- 僅限 Project Glasswing（資安）合作夥伴和特定生醫研究人員
- 企業客戶需同意 30 天資料保留

---

## 相關筆記

- [[AI 101 - 為什麼 Claude 在對話中切換了模型 — Anthropic]] — 切換機制的完整說明
- [[AI 101 - 不通知就自動降版等於錯位 AI — Nathan Lambert]] — 外部對這個安全架構的批評

---

## 來源

- [Claude Fable 5 and Claude Mythos 5 — Anthropic](https://www.anthropic.com/news/claude-fable-5-mythos-5)
