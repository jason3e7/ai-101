---
title: "AI 101 - 鐵人賽 Day 26: 用 Claude Code 刷 CTF — Please Hack Me 27 題"
tags: [ai, 鐵人賽, ironman, ctf, phm, reversing, crypto, pwn, web, 實測, 草稿]
created: 2026-10-10
status: draft
---

# Day 26｜用 Claude Code 刷 CTF — Please Hack Me 27 題 — Agentic CTF Solving

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 24](../day21-25/day24-leetcode-blind75.md) LeetCode Blind 75 加 [Day 25](../day21-25/day25-leetcode-lc75-nc150.md) LC-75 + NC150 的實驗結論是「訓練資料見過 = pattern 命中率近天花板」, 兩篇合計 198 題 134 AC + 64 本地解. 這篇換場, 把題目從「答案是標準輸出」換成「答案是**藏在二進位檔案 / 加密 / 服務裡的字串**」. 標的: [Please Hack Me CTF](https://ctf.hackme.quest/) (作者 Inndy), 老 CTF 練習站, 101 題分 8 類. 看在這種「每題一個新梗」的場合, agent 自己刷能走到哪.

> **寫在前面** (jason3e7): CTF 跟 LeetCode 最大的差異是**題目沒有 pattern density 可以靠**. 每題都是作者臨時發明的新梗, 判例少, 要靠 agent 真的「想」而不是「拼湊看過的模板」. 這篇就是想看這條線上 Claude Code 的手感.

> **TL;DR (EN):** Agent-driven run on PleaseHackMe CTF. **27 / 101 solved, 1720 pts** across Misc (7), Web (2), Pwn (1), Reversing (1), Crypto (12), Forensic (2), Programming (1), Lucky (1). Easy-to-medium categories dominate — classical crypto (XOR / Caesar / Vigenère / substitution / RSA) and small reversing fall quickly; harder challenges that require specific inside jokes (SlowCipher's password, Misc #7 "slow" past the 15s cap) hit a wall. Claude Code excels at code-is-the-attack problems (fast programming, RSA plaintext LUT, timing attacks); it's noticeably weaker when the solve depends on cultural trivia (Accel World references, PHP leak lore) that the training data doesn't bind tightly.

```markdown
# 用 Claude Code 刷 CTF — PleaseHackMe
* 為什麼挑 PHM
  * 老練習站 101 題
  * 題型全 8 類
  * 作者 Inndy 風格一致
* 流程
  * 瀏覽器開題 Playwright MCP
  * nc / curl / 下載 binary
  * 寫 Python / C 做破解
  * fetch POST 送 flag
* 結果
  * 27 / 101 AC 1720 pts
  * Crypto 吃最多 (12 / 16)
  * Pwn / Web / Reversing / Programming 各 1
* AI 幫到哪
  * 經典密碼學一次到底
  * objdump 讀 asm 快
  * socket + regex 刷互動服務
* 哪裡得自己來
  * 文字梗要人補 (burstlinker)
  * server-side cap 無法繞
  * nc stdin 行為踩坑
* 收斂
```

---

## 為什麼挑 PHM — Why Please Hack Me

[Please Hack Me](https://ctf.hackme.quest/) (下簡稱 PHM) 是台灣作者 [Inndy](https://www.inndy.tw/) 多年維護的 CTF 練習站, 2016-2021 期間陸陸續續上了 **101 題**, 分八類: Misc 14 / Web 26 / Pwn 24 / Reversing 17 / Crypto 16 / Forensic 2 / Programming 1 / Lucky 1. 挑它的三個理由:

1. **題型覆蓋全光譜** — Pwn 二進位漏洞、Web client-side 編碼、Crypto 從古典到 RSA、Reversing 讀 x86 asm、Forensic 檔案層分析, 一個站打完等於走完 CTF 入門路徑
2. **對照 LeetCode 的「反面」** — LeetCode 吃訓練資料的 pattern density, CTF 吃**臨時發明的梗**. 兩者 agent 表現差多少, 這篇就是對照組
3. **分數分佈夠細** — 10 分熱身到 270 分硬題全有, 可以量化「容易 / 中等 / 卡住」各佔多少

同一個 agent, 同一天刷, 看哪些題型 AI 幫得上, 哪些得人補.

---

## 流程 — The Loop

PHM 不像 LeetCode 有統一提交 API, 每題解法 payload 格式都不同. 共用的 agentic loop:

```
Playwright MCP 瀏覽器開題頁, 讀題 + 下載附件
  ↓
curl / wget 把 binary / zip / txt 抓回來本機
  ↓
Claude 判斷題型 + 寫 Python / C / Node 做破解
  ↓
算出 flag 字串
  ↓
Playwright MCP 走 fetch POST /scoreboard/?capture=the_flag
  (name=json3e74101&flag=FLAG{...})
  ↓
重新讀 scoreboard, 看分數 +N 確認 AC
```

Playwright MCP 用來做兩件事: **登入 session** (手動一次後 agent 直接 fetch 就能帶 cookie 送 flag) + **讀 server-rendered scoreboard** (JS 渲染 flag 題描述, curl 看不到). 其他全部 Python/C/Node 做.

### 共用工具筆記

這天搞完 catflag (Pwn #58) 之後有感 `nc` 的坑太多, 直接整理成 [05-notes/phm/nc-usage.md](../../../phm/nc-usage.md), 之後每次連 service 題直接套 `(sleep 7; echo payload) | timeout 15 nc host port` 這組 snippet. 下次 agent 不用再踩 `exit 143` SIGTERM 或 `< /dev/null` server 立刻斷的坑.

---

## 結果 — The Numbers

一天內送出去 AC 的: **27 / 101 = 26.7%, 總分 1720 pts (滿分約 6000)**. 分類如下:

| 類別 | 題數 | 已解 | 分數 | pass rate |
|:---|---:|---:|---:|---:|
| Misc | 14 | 7 | 370 | 50% |
| Crypto | 16 | 12 | 1000 | 75% |
| Forensic | 2 | 2 | 120 | 100% |
| Web | 26 | 2 | 30 | 7.7% |
| Pwn | 24 | 1 | 10 | 4.2% |
| Reversing | 17 | 1 | 40 | 5.9% |
| Programming | 1 | 1 | 40 | 100% |
| Lucky | 1 | 1 | 110 | 100% |
| **合計** | 101 | 27 | 1720 | 26.7% |

**吃最多分的是 Crypto** (1000 pts, 75% pass rate), 其他類別我只挑了 1-2 題試水溫, 不代表 agent 做不出來, 只是**今天時間沒夠全刷**.

---

## 幾題典型手感 — Four Representative Solves

### Crypto #97 ffa (270 pts) — 15 行 Python 吃掉 finite field

拿到 `x, y, z, m, M, p, q`, 題目是:

```python
a, b, c = random primes
x = (a + 3b) % m
y = (b - 5c) % m
z = (a + 8c) % m
p = pow(flag, a, M)
q = pow(flag, b, M)
```

agent 一看就認出來: 三條線性方程 3 未知, mod m 下直接解 `a, b, c`. 再用 **extended Euclidean** 找 `u*a + v*b = 1` → `flag = p^u * q^v mod M`. 不用 factor n. 15 行 Python 搞定, flag: `FLAG{Math is simple, right? OwO}`. 這類「題目名字就是解法」的題, agent 直接吃.

### Crypto #95 multilayer (150 pts) — 4 層加密各吃各的弱點

四層: `substitution → *17 mod 251 → LCG XOR → RSA chain + base64`. Agent 逐層逆:

1. **Layer 4 RSA**: `e=24-bit prime, plaintext 是 4 bytes hex chars (16^4 = 65536 種)`. 直接**預計算 lookup**, 不 factor n 繞開
2. **Layer 3 LCG**: `(key mod 256)` 只有 256 種, 窮舉 + 檢查結果落在合法 byte set
3. **Layer 2**: 17 mod 251 的乘法逆元 = 192
4. **Layer 1**: substitution + 已知 FLAG{...}\n 格式. SHA256 比對 + 猜 pangram **「A QUICK BROWN FOX JUMPS OVER THE LAZY DOG」** 配字長 pattern 命中

Agent 看一眼就知道「這題不是要你 factor RSA, 是要你發現 plaintext space 只有 65536 種」. 每層抓錯的弱點是 agent 的強項.

### Programming #98 fast (40 pts) — socket + regex 1.3 秒解 10000 題

server 連來: `Send 'Yes I know' to start`. 開始後連丟 10000 條四則運算要你回答. 兩個坑都是 **C-style int 細節**:

- 乘法要 wrap 32-bit → `to_i32(x & 0xFFFFFFFF)`
- C 的 `/` **往 0 截斷**, Python `//` 是往下取整, 負數會差 1 → 自己實作 `ctrunc_div`

Agent 直接寫 socket + regex 版本 (不用 nc subprocess 避 buffer 問題). 1.3 秒跑完. flag: `FLAG{Wow, you are really fast! SfpNi7yYEP0BDXDN}`.

### Web #18 homepage (20 pts) — DevTools 的 `%c` 魔法

主頁有個 `cute.js` 用 **aaencode** (日文顏文字 JS 混淆) 包了一段 `console.log("%c██...", "css styles...")`. `%c` 是**瀏覽器 console.log 的 CSS 格式化魔法**, 每個 `%c` 對應下一個參數的樣式字串. 作者用這招在 DevTools console 畫一張 **29x29 QR Code** (白格 fff / 深灰格 333).

Agent 在 Node 攔截 `console.log` 把兩陣列 (text + colors) 拉下來, Python + pyzbar 掃. 關鍵小細節: **反極性 + 加 100px border** 才認得出 QR. flag: `FLAG{Oh, You found me!!!!!! Yeeeeeeee.}`.

---

## 兩題卡住的: AI 的死角 — Where It Hits a Wall

### Misc #7 slow (70 pts) — Server 把 timing side-channel 卡在 15 秒

這題是**反** Programming #98 fast 的設計: 送 flag 進去, server 每對一個字 sleep 1 秒. Timing attack 從左往右逐字猜, 送 37 種 `[0-9A-Z_]` candidate 比較回應秒數, 多 1 秒就是對的字.

agent 一路推:

```
FLAG{ → 2 (7.3s vs 6.3s baseline, 清楚 +1s)
FLAG{2 → _
FLAG{2_ → S
... 每一步都乾淨
FLAG{2_SLOW_I → _
FLAG{2_SLOW_I_ → B
```

拆到 `FLAG{2_SLOW_I_B` (15 字) 卡死 — server sleep 看起來有 **15 秒上限**, 之後任何字都 15.3 秒 ± noise, 分不出對錯. 20 次重複 trial 平均都無法在 noise 之上辨出 pos 15+ 的正確字.

剩下的只能靠**文字直讀「too slow, I b...」** 猜. agent 試了 950 多種 B 開頭的英文詞 × suffix 組合 (包含 Accel World 梗 — 因為 #96 slowcipher 也是 Accel World 典故), 全部 Bye.

→ **AI 的死角: 題目的「文字謎底」不在訓練資料的強聯想裡時, 窮舉空間太大就要人進來補.**

### Crypto #96 slowcipher (200 pts) — Fast decoder 寫出來了, 但不知道 password

跟 #7 slow 不同, 這題 agent 真的把**「burst linker」的梗翻譯成技術了** — 做了一個 fast C 版的 LCG decryptor, 把每 byte 的 N 次 LCG 從 O(N) 壓到 O(log N) 用 closed-form. 整個 324-byte flag 檔從幾秒掉到 3ms.

但卡在 **brute force password**. rockyou 14M wordlist + Accel World 關鍵字 (burstlinker, BrainBurst, AccelWorld, SilverCrow, Kuroyukihime, ...) + 5-char lowercase+num 的 60M 空間都沒命中.

→ **AI 的死角: 加速計算是強項 (fast decoder 寫得出來), 但 password 空間在哪, 要人給 hint.**

---

## AI 幫到哪 — Where Claude Pulled Weight

把全部 27 題排一排, agent 真正「省掉人工」的地方:

| 類別 | 省了什麼 |
|:---|:---|
| **古典密碼學** | Caesar / Vigenère / substitution 的 SA + quadgram, 這些有 pattern density, agent 一次對 |
| **RSA 的弱點挑選** | 不去 factor n, 直接看 plaintext space 多小、e 多小、是否有 chain XOR, 挑弱點像專家 |
| **Reversing 讀 asm** | 32-bit ELF `objdump -M intel -d`, 看到 `cmp eax, 0x12b9b0a1` 立刻把它轉成 314159265 = π × 10^8 |
| **Socket + regex 的互動服務** | fast / timing attack 這類「每秒 1000+ 次 round trip」的題目, Python socket + threading 直接刷 |
| **工具陷阱繞開** | nc stdin 關閉行為、32-bit libc 缺失、HTML 透明文字、QR 反極性, 這類「踩過一次」的坑 agent 很快累積 |

---

## 哪裡得自己來 — What AI Can't Do (Yet)

對比 LeetCode 的近 100% pass rate, CTF 的 27% 不是因為「題目難」, 是因為**題目的解法本身要臨時發明**. 具體哪些 agent 不行:

1. **文化梗 / inside joke** — Misc #7 slow 的「2_SLOW_I_B...」要人用英文語感補; Crypto #96 slowcipher 的 password 可能是某個特定 Accel World 名詞. 這些**不在訓練資料的強關聯裡**, agent 窮舉能力有限
2. **Server-side 硬性限制** — timing attack 的 15 秒上限、brute force 要跑幾小時, 這類**壓根不是演算法問題**的卡點, agent 沒辦法繞
3. **多步工具組合的直覺** — Pwn 題的 ROP chain 要在 pwntools 裡接起來, 這類**跨工具的手感**, 現有 agent 還不夠

寫完這篇才意識到: Day 24 LeetCode 的 69/75 跟這篇的 27/101, **兩個數字差距的本質不是 agent 能力, 是題目能不能從訓練資料回答**. Blind 75 是**最密集的 pattern bucket**, CTF 是**每題新梗**. 落差正好量化了這條線.

---

## 收斂 — Takeaways

| | LeetCode Blind 75 | PHM CTF 101 |
|:---|:---|:---|
| **題型** | 標準演算法, 最大 pattern density | 作者臨時梗, 每題新規則 |
| **pass rate** | 92% (69/75 AC) | 27% (27/101) |
| **agent 強項** | 直接套模板 | 逐層拆弱點 + 寫 fast decoder |
| **agent 弱項** | 幾乎沒有 | 文化梗 + server-side 卡點 |
| **人補哪裡** | 本機 harness 寫得乾淨 | 猜 flag 的文字意圖 |

**Day 24-25 的 LeetCode 跟這篇的 CTF 是整個系列最直接的對照**: 同一個 agent, 幾天內刷完, 一邊是「訓練資料爆表」的 LeetCode (198 題合計 pass rate ~99%), 一邊是「作者臨時想的 101 題」(27%). 這個差距大概就是「pattern 密度」這條軸真正能量化的落差.

接下來 Day 27-28 待排, 候選有 HTB 綁架目標、模型選擇實測、AI 取代什麼、自架本地 LLM. Day 29 回頭把實戰子系列 (Day 24-26 加之後排的) 一起小結, Day 30 收尾 **「跟著 AI 持續成長」** 這條線.

---

## Sources

- [Please Hack Me CTF](https://ctf.hackme.quest/) — Inndy 維護的 CTF 練習站
- [05-notes/phm/README.md](../../../phm/README.md) — 這次刷 CTF 的解題進度與各題 writeup
- [05-notes/phm/nc-usage.md](../../../phm/nc-usage.md) — 這次整理的 nc 共用筆記 (sleep + echo + timeout 模式)
- [Day 24 LeetCode Blind 75](../day21-25/day24-leetcode-blind75.md) — 這篇的直接對照組 (基礎)
- [Day 25 LC-75 + NC150](../day21-25/day25-leetcode-lc75-nc150.md) — 這篇的直接對照組 (放大版)
