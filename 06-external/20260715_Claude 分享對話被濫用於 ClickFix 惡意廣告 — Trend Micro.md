---
title: "Claude 分享對話被濫用於 ClickFix 惡意廣告"
tags: [ai, 外部觀點, 資安, malvertising, clickfix, claude, social-engineering]
source: https://www.trendmicro.com/en_us/research/26/f/claudeai-shared-chat-abused-in-malvertising.html
author: Trend Micro（TrendAI Research）+ jason3e7 實測
created: 2026-07-15
---

# Claude 分享對話被濫用於 ClickFix 惡意廣告 — Claude Shared Chat Abused in ClickFix Malvertising

[← 回主頁](../index.md)

> [!NOTE]
> 原文：[Trend Micro — Threat Actors Abuse claude.ai Shared Chat for ClickFix Malvertising](https://www.trendmicro.com/en_us/research/26/f/claudeai-shared-chat-abused-in-malvertising.html)（含 jason3e7 實測重現）
> **一句話：** 攻擊者用 Google 廣告假冒 AI 開發工具，把受害者導到**真的 claude.ai 分享對話頁面**，頁面裡是假的客服對話，一步步騙你打開終端機貼上指令、下載惡意程式。台灣是重災區。

> **TL;DR (EN):** A malvertising campaign (Apr 8–Jun 14 2026, tracked by Trend Micro) used Google Ads impersonating AI dev tools, then hosted its ClickFix social-engineering script inside **legitimate claude.ai shared-chat URLs** — bypassing browser and Safe Browsing warnings. Fake "support" chats walked victims through opening a terminal and running a base64 command that fetched malware. APAC = 67.2% of victims; Taiwan alone = 30.5% of traffic.

> [!CAUTION]
> **這是防禦性資安筆記。** 只講怎麼辨識與自保，不提供任何可執行的攻擊指令。

---

## 攻擊手法 — How It Works

這是一種叫 **ClickFix** 的社交工程（用假的「修復／驗證」步驟騙你自己動手裝惡意程式）。整條鏈：

1. **假廣告**：攻擊者買 Google 廣告，假冒熱門 AI 工具（Claude AI、ChatGPT Codex、Perplexity、Cursor IDE、JetBrains 等至少六個品牌）
2. **借正牌網域躲過偵測**：早期用 GitLab Pages（`*.gitlab.io` 免費靜態站）；後來**改用 `claude.ai` 的「分享對話」功能**——因為連結是**真的 claude.ai 網址**，瀏覽器警告、URL 檢查、Safe Browsing 全都放行
3. **假客服對話**：那個分享對話裡是攻擊者預先寫好的假對話，假冒「Apple 支援」或「開發團隊」，一步步教你**打開終端機、貼上一段指令**
4. **執行即中招**：那段指令通常是 base64 編碼的腳本，解開後會去下載第二階段的惡意程式

**規模**：7 週內 106 個惡意主機名、6 波攻擊（2026-04-08 ~ 06-14）。

---

## 為什麼特別危險（台灣尤其）— Why It's Dangerous

- **信任被借用**：受害者看到的是**真的 claude.ai 網址**，不是可疑的釣魚站——這正是它繞過所有網址防護的原因
- **台灣是重災區**：亞太佔全部受害者 **67.2%**，其中**台灣單獨佔總流量 30.5%**
- **紅旗只有一個、但很清楚**：**任何網頁叫你「打開終端機、貼上一段指令」，幾乎都是攻擊**——不管那頁看起來多正規、網址多可信

---

## 我的實測 — My Test

jason3e7 實際把整條攻擊鏈重現了一次：

**① 搜尋看到假廣告** —— Google 搜「mac claude code」，最上面「贊助商搜尋結果」就是假廣告。顯示的網域寫著 `claude.ai`，看起來完全正常，一般人不會起疑。

![Google 搜尋結果最上方的假贊助商廣告，顯示網域為 claude.ai](./assets/clickfix-01-fake-google-ad.png)

**② 落在真的 claude.ai 分享頁** —— 點下去落在一個**真的** `claude.ai/share/...` 頁面，標題「Running Claude Code on Mac」、右上角掛「Shared by Apple Support」，一步步教你「開 Terminal → 貼上下面這行指令」。那行指令是用 base64 包起來的。

![假冒 Apple Support 的 claude.ai 分享頁，教你在終端機貼上 base64 指令（已遮罩）](./assets/clickfix-02-fake-share-page.png)

**③ 解碼露出真面目** —— 把那段 base64 丟進 CyberChef 解開，露出真正的惡意網址 `hxxp://malwareaudit[.]com/curl/...`（此處 defang）——那行 `curl` 會去這個網址抓下一階段的東西。

![CyberChef 把 base64 解碼出惡意網址（已遮罩）](./assets/clickfix-03-decoded-payload.png)

> [!TIP]
> 兩個實測心得：
> - **那個分享頁頂端其實有灰字警告**（"This is a copy of a chat… may include unverified or unsafe content… does not represent the views of Anthropic"）——但一般人根本不會細看，攻擊者就是賭這一點。
> - 惡意網址與 base64 指令**我在截圖上打碼了**（黑框），因為這是公開 repo，不散布可用的惡意 IOC。

---

## 怎麼自保與官方回應 — Defense & Response

**自保**：

- **鐵則**：網頁／客服叫你開終端機貼指令 → 直接關掉，那不是正常的軟體安裝流程
- 下載 AI 工具只認**官方網域**，不要點廣告連結（廣告位最容易被買）
- 看到 `claude.ai/share/...` 之類分享連結，內容仍要用常識判斷——**網址可信 ≠ 內容可信**

**Anthropic 回應**：經 Trend Micro 通報後，已封禁相關帳號、停用惡意分享對話，並正在為「分享對話」功能加上額外的濫用防護。

---

## 來源 — Sources

- 原文：[Threat Actors Abuse claude.ai Shared Chat for ClickFix Malvertising Campaign — Trend Micro](https://www.trendmicro.com/en_us/research/26/f/claudeai-shared-chat-abused-in-malvertising.html)
- [Malvertising Campaign Abuses Claude.ai Shared Chat Feature — CyberPress](https://cyberpress.org/malvertising-campaign-claude-ai-shared-chat-feature/)
- [Hackers Abuse Claude.ai Shared Chat Feature to Host ClickFix Instructions — Cyber Security News](https://cybersecuritynews.com/claude-ai-shared-chat-feature-abused/)
- jason3e7 的相關貼文：[Facebook](https://www.facebook.com/jason.cheng.9615/posts/pfbid02kkCtrr8i5YhXQ84YSDJAAYChQx7T2RMsHdiNdwtcJpFteQAyECT5A8tkuFg6AAT1l)
