---
title: AI 101 - PG Play writeup 個人文風約束
tags: [ai, 寫作, 風格, 靶機, writeup, 個人筆記, prompt]
created: 2026-09-20
---

# PG Play writeup 個人文風約束 — Style Constraints for jason3e7's Writeup Voice

[← 回主頁](../index.md)｜對照概念：[AI 的文風與語氣](../02-advanced/ai-writing-style-tells.md)

> [!NOTE]
> [AI 的文風與語氣](../02-advanced/ai-writing-style-tells.md) 是**辨識並抑制** AI 的文風習慣；這篇是它的倒影——**辨識並重現** jason3e7 在 2023 年 iThome 鐵人賽〈PG Play 怎麼玩都不累〉三十篇 writeup 的個人文風。目的：讓 Claude 續寫或補寫同系列文章時，讀起來像本人繼續寫下去，不像 AI 接手。作法：把三十篇歸納出的**結構模板、標點慣例、句型節奏、常用零件**，整理成一份可以整段貼進 system prompt 或 skill 的約束清單。

> **TL;DR (EN):** A style-fingerprint note derived from all 30 articles of the author's 2023 PG Play writeup series. Serves as the mirror image of `ai-writing-style-tells.md`: instead of listing AI tells to suppress, it lists the author's tells to reproduce so Claude can continue the series in the same voice. Key constraints: rigid five-block template per box (防雷頁 / 可能的遺漏 / 摘要 / Walkthrough / ref), half-width comma between Chinese clauses, bullet-only prose, imperative verbs, English tech terms inlined without quotes, no em dash, no callout blocks, no TL;DR line, no closing summary — the article stops at `get proof.txt` plus refs. Paste the "Do / Don't" section into a system prompt to constrain output.

---

## 一眼辨識：這系列長什麼樣 — The Silhouette

隨便翻一篇（除了 Day 01、02、30 這三篇 meta），骨架幾乎是同一副：

```
防雷頁

可能的遺漏
- 一行一件事
- 一行一件事

摘要
- 一行一件事
- 一行一件事

Walkthrough
先透過 nmap 確認開什麼 port 和什麼服務

dirb 掃描網站目錄

手動檢查網站

... （每一步一張截圖 + 一行說明，之間空一行）

get local.txt

find setuid program

檢查 /etc/crontab
檢查 /etc/passwd

使用 X 提權為 root, get proof.txt

ref
外部連結標題
另一個外部連結標題
```

多台靶機的日子（Day 24 ~ Day 29）就是**把這副骨架複製一份**，每台前面加一個機器名當標題。沒有例外。

---

## 五塊模板：一定按這個順序，一塊都不能少 — The Five-Block Template

| 區塊 | 作用 | 內容規則 |
|:---|:---|:---|
| **防雷頁** | 純粹的告示，一行文字自己站著 | 底下不寫任何解釋，直接進「可能的遺漏」 |
| **可能的遺漏** | 給卡關的讀者的**檢查清單**，不劇透 | 條列，每條一句話，語氣中性、觀察式 |
| **摘要** | 三行以內講完「這台怎麼打下來的」 | 條列，動詞開頭：「透過 X 取得權限」「找到 Y 弱點」 |
| **Walkthrough** | 完整步驟，一步一張圖 | 每張圖前一行說明；指令直接貼原文，不解釋 |
| **ref** | 引用來源，純標題，不評論 | 一行一個，**不加 `-` 也不編號** |

Day 01、Day 02、Day 30 是 meta 篇（PG Play 介紹、攻擊流程、系列總結），**才會出現「大綱」開頭**。writeup 本體從不出現「大綱」「前言」「結語」「小結」。

---

## 標點與空格：這是最強的指紋 — Punctuation Fingerprint

**中文句子裡用半形逗號和半形句號**——這是這系列辨識度最高的一件事：

```
先透過 nmap 確認開什麼 port 和什麼服務
使用 python 取得 pty shell
是不是很像 RPG 呢? XD
```

**不是**：

```
先透過 nmap，確認開什麼 port 和什麼服務。   ← 全形逗號句號，錯
使用 Python 取得 pty shell。                 ← Python 開頭大寫，錯
```

具體規則：

- 中文與英文/數字之間**留一個半形空格**：`先透過 nmap`、`22, 80, 443 port`、`ssh 登入 johannes`
- 中文與中文之間用**半形逗號**分句：`, `（逗號後接半形空格）
- 句尾用半形 `.` 或 `?`；`!` 幾乎不用
- 英文技術詞**保持原大小寫、不加引號、不加程式碼標記**：`nmap`、`ssh`、`command injection`、`port`、`webshell`
- CVE 編號直接寫：`shellshock(CVE-2014-6271)`、`PwnKit(CVE-2021-4034)`——**括號用半形、內容前不留空格**

> [!IMPORTANT]
> 這一條做錯就露餡。全形標點 = AI 直覺；半形標點 + 中英之間有空格 = jason3e7 手動打的。Claude 預設會全形化，必須明確約束。

---

## 句型與節奏：短、平、指令式 — Terse, Flat, Imperative

看幾個典型段落：

```
發現 sudo 可以執行任意指令.
(Attack)使用 sudo su 切換為 root, 切換成功.
(Foothold)取得最高權限 root.
```

```
先透過 nmap 確認開什麼 port 和什麼服務
dirb 掃描網站目錄
手動檢查網站
功能正常可以 ping 127.0.0.1
```

規則：

- **一句一件事**，句子多用逗號串短子句，不用「並且」「而且」「因此」「所以」
- **動詞開頭**：「透過 X」「使用 X」「發現 X」「檢查 X」「取得 X」——**主詞省略**
- **形容詞和副詞極少**，幾乎沒有「非常」「其實」「明顯地」「值得注意的是」
- **不用比喻，不用對立句**（「不是 X，而是 Y」這種 AI 愛用的節奏在整個系列出現 0 次）
- **不用三段式**：想到什麼就寫幾條，四條就四條、兩條就兩條，不會為了節奏湊三條
- **不用破折號**：整個系列 30 篇 0 次

---

## 常用零件：可以直接複用的一句話 — Reusable Building Blocks

下面這些**幾乎每篇都會出現**，可以當成模板變數直接套：

**開場偵察（Recon）**：

- `先透過 nmap 確認開什麼 port 和什麼服務`
- `dirb 掃描網站目錄`
- `gobuster dir ...`（同義替換）
- `手動檢查網站`
- `wpscan 掃描網站(01)` / `(02)` / `(03)`（多張截圖時用括號編號）
- `enum4linux(01)` ~ `enum4linux(05)`

**建立立足點（Foothold）**：

- `發現 X 弱點`
- `使用 python 打 reverse shell 回到攻擊機`
- `使用 python 取得 pty shell` + `python3 -c 'import pty; pty.spawn("/bin/bash")'`
- `ssh 成功登入 [user]`
- `get local.txt`

**提權（Privilege Escalation）**：

- `find setuid program`
- `檢查 /etc/crontab`
- `檢查 /etc/passwd`
- `檢查 sudo -l`
- `使用 [X] 提權為 root`
- `get proof.txt`

**失敗嘗試（不刪，留在流程裡）**：

- `沒有弱點, 請嘗試其他攻擊`
- `登入失敗, 請嘗試其他方式`

> [!TIP]
> **失敗步驟要留著。** 這系列的一個特色是**不會只寫成功路徑**——嘗試過但沒用的攻擊會照樣列出來、標「沒有弱點」再往下走。這反映真實打題順序，不是 AI 事後整理的乾淨解法。

---

## 個性只在特定位置露頭 — Where the Personality Peeks Out

30 篇裡整個系列的**「人味」總量非常少**，只集中在三個地方：

1. **Day 01 對讀者分類的短招呼**：「如果想要自己作題目的觀眾, 建議先不要閱讀; 遇到卡關的挑戰者, 建議頁面緩緩往下滑, 看到未嘗試過部分或新觀點, 先自行嘗試看看; 吃瓜朋友請隨意.」——**分號串三種讀者、口氣輕、不多解釋**
2. **Day 02 結尾**：「是不是很像 RPG 呢? XD」——**唯一一個顏文字**，且只在 meta 篇出現
3. **Day 30 心理層面**：「學習從模仿開始, 一開始當 script kiddie 不要覺得很羞恥」——**建議語氣，但仍然一句一行、不鋪陳**

Writeup 本體（Day 03 ~ Day 29 的 27 篇）**沒有任何個人語氣**，純粹技術流程。要寫像本人的續集，比例要照抓：**技術篇 0% 個性，meta 篇一句話濃度**。

---

## Do / Don't 清單：可以整段貼進 prompt — Copy-Paste Constraints

```
寫作約束（PG Play writeup 風格）：

DO：
- 標點：中文句子裡一律用半形逗號和半形句號，中英之間留一個半形空格
- 結構：每台靶機依序寫「防雷頁 / 可能的遺漏 / 摘要 / Walkthrough / ref」五塊，不加標題以外的裝飾
- Walkthrough 每一步一行說明，中間空一行；指令直接原文貼上、不解釋
- 動詞開頭、主詞省略：「先透過 X 確認 Y」「使用 X 提權為 root」
- 失敗過的嘗試也要列出來，用「沒有弱點, 請嘗試其他攻擊」帶過
- 英文技術詞（nmap、ssh、command injection、port）保持原大小寫、不加引號
- ref 區塊一行一連結，不加 `-` 也不加編號
- 沿用系列固定句：「先透過 nmap 確認開什麼 port 和什麼服務」、「dirb 掃描網站目錄」、
  「手動檢查網站」、「使用 python 取得 pty shell」、「find setuid program」、
  「檢查 /etc/crontab」、「檢查 /etc/passwd」、「get local.txt」、「get proof.txt」

DON'T：
- 不用全形標點（，。！？：；）
- 不用破折號（—— 或 —）
- 不用 markdown callout（> [!NOTE] 這類）
- 不寫「TL;DR」、「前言」、「結語」、「小結」、「總結一下」
- 不用「其實」「值得注意的是」「在某些情況下」「換句話說」等 AI 常見冗詞
- 不用三段式湊節奏、不用「不只是 X，而是 Y」對立句
- 不加比喻和譬喻（例外：Day 02 用一次《鋼の錬金術師》，Day 30 一次 script kiddie）
- 不解釋指令做什麼（指令直接貼原文，讀者自己看）
- 不在 writeup 本體寫個人感想（感想只在 Day 01 / 02 / 30 這種 meta 篇才有）
- 不省略失敗路徑改寫成漂亮的成功流程
```

---

## 前後對照：AI 預設 vs 本人風格 — Before/After

**AI 預設寫法**（會出現的 tells 全部標粗）：

> 這台靶機的解題過程可以分為三個階段——**首先是資訊收集，接著是漏洞利用，最後是權限提升**。**值得注意的是**，Walkthrough 的重點不只是找到弱點，**更在於**理解每個工具背後的邏輯。**讓我們一起看看**如何一步步拿下 root 權限。

**本人風格改寫**：

```
防雷頁

可能的遺漏

web 頁面輸入
可能存在防火牆
進入機器後, 檢查程式檔案權限

摘要

透過 web 頁面 command injection 取得權限
透過找到有 setuid 的程式, 使用 vim 進行提權

Walkthrough

先透過 nmap 確認開什麼 port 和什麼服務

dirb 掃描網站目錄

手動檢查網站

發現 command injection 弱點, 打 reverse shell

使用 python 取得 pty shell

get local.txt

find setuid program

使用 vim 提權為 root, get proof.txt
```

差異一眼可見：**沒有前言、沒有解釋、沒有情緒詞、沒有全形標點、沒有破折號、沒有三段式**。只有動作序列。

---

## Sources

30 篇原文（jason3e7 @ iThome 鐵人賽 2023，系列編號 6091）：

- [\[Day 01\] What is PG Play ?](https://ithelp.ithome.com.tw/articles/10318502)
- [\[Day 02\] How to practice PG Play from zero ?](https://ithelp.ithome.com.tw/articles/10319853)
- [\[Day 03\] PG Play Shakabrah Writeup](https://ithelp.ithome.com.tw/articles/10320833)
- [\[Day 04\] PG Play Sar Writeup](https://ithelp.ithome.com.tw/articles/10321340)
- [\[Day 05\] PG Play FunboxEasy Writeup](https://ithelp.ithome.com.tw/articles/10321387)
- [\[Day 06\] PG Play BBSCute Writeup](https://ithelp.ithome.com.tw/articles/10321388)
- [\[Day 07\] PG Play Potato Writeup](https://ithelp.ithome.com.tw/articles/10321389)
- [\[Day 08\] PG Play FunboxRookie Writeup](https://ithelp.ithome.com.tw/articles/10321390)
- [\[Day 09\] PG Play CyberSploit1 Writeup](https://ithelp.ithome.com.tw/articles/10321392)
- [\[Day 10\] PG Play Katana Writeup](https://ithelp.ithome.com.tw/articles/10321394)
- [\[Day 11\] PG Play PyExp Writeup](https://ithelp.ithome.com.tw/articles/10328160)
- [\[Day 12\] PG Play Gaara Writeup](https://ithelp.ithome.com.tw/articles/10328231)
- [\[Day 13\] PG Play DC-1 Writeup](https://ithelp.ithome.com.tw/articles/10329345)
- [\[Day 14\] PG Play DC-2 Writeup](https://ithelp.ithome.com.tw/articles/10329346)
- [\[Day 15\] PG Play InfosecPrep Writeup](https://ithelp.ithome.com.tw/articles/10329347)
- [\[Day 16\] PG Play Lampiao Writeup](https://ithelp.ithome.com.tw/articles/10329348)
- [\[Day 17\] PG Play Blogger Writeup](https://ithelp.ithome.com.tw/articles/10329349)
- [\[Day 18\] PG Play DriftingBlues6 Writeup](https://ithelp.ithome.com.tw/articles/10329350)
- [\[Day 19\] PG Play OnSystemShellDredd Writeup](https://ithelp.ithome.com.tw/articles/10333566)
- [\[Day 20\] PG Play Moneybox Writeup](https://ithelp.ithome.com.tw/articles/10333568)
- [\[Day 21\] PG Play Vegeta1 Writeup](https://ithelp.ithome.com.tw/articles/10334615)
- [\[Day 22\] PG Play EvilBox-One Writeup](https://ithelp.ithome.com.tw/articles/10335263)
- [\[Day 23\] PG Play Empire-breakout Writeup](https://ithelp.ithome.com.tw/articles/10335267)
- [\[Day 24\] PG Play Amaterasu & Photographer Writeup](https://ithelp.ithome.com.tw/articles/10335268)
- [\[Day 25\] PG Play Dawn & Solstice Writeup](https://ithelp.ithome.com.tw/articles/10335269)
- [\[Day 26\] PG Play Ha-natraj & Inclusiveness Writeup](https://ithelp.ithome.com.tw/articles/10337466)
- [\[Day 27\] PG Play FunboxEasyEnum & Monitoring Writeup](https://ithelp.ithome.com.tw/articles/10338040)
- [\[Day 28\] PG Play Sumo & Seppuku Writeup](https://ithelp.ithome.com.tw/articles/10338043)
- [\[Day 29\] PG Play SunsetDecoy & SunsetNoontide Writeup](https://ithelp.ithome.com.tw/articles/10338044)
- [\[Day 30\] PG Play Summary](https://ithelp.ithome.com.tw/articles/10338045)
- [系列首頁](https://ithelp.ithome.com.tw/users/20078298/ironman/6091)
