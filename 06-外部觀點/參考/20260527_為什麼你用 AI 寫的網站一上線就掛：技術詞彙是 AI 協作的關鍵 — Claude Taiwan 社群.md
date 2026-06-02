---
title: "為什麼你用 AI 寫的網站一上線就掛：技術詞彙是 AI 協作的關鍵"
tags: [ai, 外部觀點, agentic-coding, 軟體工程, context, 技術詞彙, load-balance, cache, cicd, 影片推薦]
source: https://www.facebook.com/groups/1224997379198346/posts/1306304891067594/
author: Claude Taiwan 社群成員
created: 2026-05-27
---

# 為什麼你用 AI 寫的網站一上線就掛：技術詞彙是 AI 協作的關鍵

> [!info]
> 原文：[Claude Taiwan 社群貼文](https://www.facebook.com/groups/1224997379198346/posts/1306304891067594/)
> 推薦影片：[為什麼你用 AI 寫的網站，一上線就掛？](https://www.youtube.com/watch?v=t5CtfUWJjm4)（Debug 土撥鼠，約 12 分鐘）
> **一句話：** 用泡沫紅茶店比喻軟體工程核心技術（load balance、CICD、cache 等），並指出「知道正確的技術名詞」是讓 AI 寫出穩健程式的關鍵前提。

---

## 影片核心

影片用**一間泡沫紅茶店的營運問題**類比軟體工程的痛點與解法：

| 泡沫紅茶店的問題 | 對應的軟體技術 |
|---|---|
| 尖峰時段排隊太長 | Load Balance（負載均衡）|
| 訂單爆量時人手不夠 | Auto Scale Up（自動擴容）|
| 新菜單上線容易出錯 | CI/CD（持續整合 / 持續部署）|
| 常點的飲料備料好 | Cache（快取）|
| 點單與備料分開跑 | 讀寫分離 |

---

## 延伸技術脈絡

影片提到的每個技術都可以繼續往下延伸：

| 基礎技術 | 可延伸的進階概念 |
|---|---|
| **Cache** | Cache Avalanche（雪崩）、Cache Penetration（穿透）、Cache Invalid（失效）|
| **Load Balance** | Sticky Sessions、健康檢查、加權輪詢 |
| **Auto Scale** | HPA（K8s 水平擴展）、冷啟動問題 |

---

## 最有價值的洞見：技術詞彙 = AI 協作的槓桿

> 「AI 可能知道怎麼實作，但有時候想不到這個解法。所以作為人類要能夠知道在碰到這個情境下，可能可以用什麼技術來解決並餵給 AI，他就能實現出來。」

這是「Context 要給得夠多」的一個具體面向——**不是資料量，而是技術詞彙的準確度**。

### 作者的親身案例

> 「我自己就曾經因為可觀測性的問題很頭痛，但是有位前輩提點我：『你這個要串 ELK』，幫我省下了讓 AI 吐出 O11y 相關技術的時間。」

這個案例說明了三層節省：
1. 省下自己踩到痛點的時間
2. 省下碰運氣讓 AI 吐出正確技術名詞的時間
3. 省下讓 AI 解釋「這個技術是什麼」的時間

---

## 從 Vibe Coding 走向 Agentic Coding

作者的核心主張：

```
Vibe Coding
  └── 靠感覺給 AI 任務，不知道底層技術
        └── AI 有時能解，有時猜不到你要什麼

Agentic Coding
  └── 知道「這個情境該用什麼技術」
        └── 精準告訴 AI 要實現什麼
              └── AI 把它做出來，你再深入學習細節
```

> 「軟體工程發展幾十年，肯定有很多的輪子可以用，用 AI 能夠幫你省下很多的時間，但要在正確的時候應用正確的技術是需要經驗的。」

---

## 這部影片適合誰

- 用 AI 寫了功能但上線後不穩定，不知道問題在哪
- 知道 AI 很強但覺得「做出來的東西很脆」
- 想把技術廣度補起來，讓 AI 協作更有效
- 從個人專案走向需要考慮穩定性的正式環境

---

## 相關筆記

- [[AI 101 - Context Engineering]] — 「Context 給得夠多」的完整觀念，技術詞彙是 context 的一部分
- [[AI 101 - Harness Engineering]] — 框架設計決定 AI 效能的 70%
- [[蜂群 Agent 系列四：Workflow 工作流編排取代萬能 Prompt — 新人類聯盟]] — 讓 AI 系統穩健的另一個角度
- [[用 Claude Code 週末建出台股回測系統：Playbook 方法論 — 羅達]] — 實戰中如何把技術知識轉化為 AI 可執行的步驟

---

## 來源

- 原文：[Claude Taiwan 社群](https://www.facebook.com/groups/1224997379198346/posts/1306304891067594/)
- YouTube：[為什麼你用 AI 寫的網站，一上線就掛？](https://www.youtube.com/watch?v=t5CtfUWJjm4)（Debug 土撥鼠）
