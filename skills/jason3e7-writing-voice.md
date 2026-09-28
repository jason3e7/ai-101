---
name: jason3e7-writing-voice
description: jason3e7 個人文風約束 (半形標點、動詞開頭、無破折號、無 AI 冗詞). 寫或改任何要用 jason3e7 名義發表的文章 (ai-101 note、鐵人賽 draft、外部 blog) 時套用. 只管文風, 結構層由目標平台格式決定 (ai-101 走 CLAUDE.md、writeup 走五塊模板).
tags: [writing, voice, style, jason3e7]
---

# jason3e7-writing-voice

jason3e7 的**個人文風約束**. **只管文風** (標點、句型、動詞用法、詞彙選擇), **不管結構** — 結構層照目標格式的模板走 (ai-101 用 [CLAUDE.md](../CLAUDE.md) 的寫作風格指南, PG Play writeup 用五塊模板).

寫錯這裡的規則, 讀者一眼就看出「這不是本人手寫的」.

---

## 什麼時候套用 — When to Invoke

- 寫或改任何要**用 jason3e7 名義發表**的內容: ai-101 note、iThome 鐵人賽 draft/publish、外部 blog、社群貼文
- 續寫 PG Play writeup 系列
- 改寫他人版本的文字回 jason3e7 口吻

不套用: skill 檔案本身、CLAUDE.md 這類專案級規範 (那些用專案語調, 不是個人語調).

---

## Do

- **標點**: 中文句子裡一律用半形逗號和半形句號, 中英之間留一個半形空格
- **動詞開頭、主詞省略**: 「先透過 X 確認 Y」「使用 X 提權為 root」
- **一句一件事**, 多用逗號串短子句, 不用「並且」「而且」「因此」「所以」
- **英文技術詞** (nmap、ssh、command injection、port) 保持原大小寫; prose 中可以不加 backtick, 只有指令、路徑、程式碼識別字 (`budget_tokens` 這類) 才用 `code` 標記
- 寫 writeup 類內容時, 沿用系列固定句 (「先透過 nmap 確認開什麼 port 和什麼服務」、「dirb 掃描網站目錄」、「手動檢查網站」、「使用 python 取得 pty shell」、「find setuid program」、「檢查 /etc/crontab」、「檢查 /etc/passwd」、「get local.txt」、「get proof.txt」); 失敗嘗試用「沒有弱點, 請嘗試其他攻擊」帶過, 不改寫成漂亮的成功流程

## Don't

- 不用全形標點 (，。！？：；) — 一律半形
- 不用破折號 (—— 或 —), 這是 AI 最強的辨識指紋
- 不用「其實」「值得注意的是」「在某些情況下」「換句話說」「更在於」等 AI 常見冗詞
- 不用三段式湊節奏、不用「不只是 X, 而是 Y」對立句
- 不寫「讓我們一起看看」「首先／接著／最後」這種鋪陳詞
- 不加沒必要的比喻譬喻 (例外: writeup meta 篇一句話濃度)

---

## 前後對照 — Before / After

**AI 預設寫法** (常見 tells 全標粗):

> 這台靶機的解題過程可以分為三個階段——**首先是資訊收集，接著是漏洞利用，最後是權限提升**。**值得注意的是**，Walkthrough 的重點不只是找到弱點，**更在於**理解每個工具背後的邏輯。**讓我們一起看看**如何一步步拿下 root 權限。

**jason3e7 風格改寫**:

```
先透過 nmap 確認開什麼 port 和什麼服務

dirb 掃描網站目錄

手動檢查網站

發現 command injection 弱點, 打 reverse shell

使用 python 取得 pty shell

get local.txt

find setuid program

使用 vim 提權為 root, get proof.txt
```

差異一眼可見: **沒有前言、沒有解釋、沒有情緒詞、沒有全形標點、沒有破折號、沒有三段式**. 只有動作序列.

---

## 資料來源與完整版

從 [PG Play writeup 個人文風約束](../05-notes/pgplay-writeup-style-guide.md) 的「Do / Don't 清單」抽出來當 skill.

完整版 (含五塊模板結構層、標點指紋詳述、常用零件目錄、個性只在特定位置露頭、對照範例) 見原筆記. Skill 只提取**文風層**這部分.

對照概念: [AI 的文風與語氣](../02-advanced/ai-writing-style-tells.md) 是「辨識並抑制 AI 的文風」, 這 skill 是它的倒影 (辨識並重現 jason3e7 的文風).
