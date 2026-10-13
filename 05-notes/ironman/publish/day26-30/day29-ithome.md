title : [Day 29] 從 CTF 換到 HTB, 用 goal 測試了兩次, agent 還自己組出 RDP 工具鏈

> [Day 26-28](https://ithelp.ithome.com.tw/articles/10423733) 把 PHM CTF 刷到 Top 10, 本篇換場景: 從 CTF 換到 **HTB** (Hack The Box, 真實系統/網路設定的滲透靶機), 從手動 Claude Code 換成 `/goal` **一條指令跑整台**. 用兩個同一天 (2026-10-13) 跑的真實案例看 `/goal` 這個 workflow 的能跟不能: **Cap** (easy, 26 分鐘拿 user+root) 跟 **Touch** (未解, 卡在 kiosk 逃逸, **但 agent 自己架出 RDP 看圖點按鈕的工具鏈**).

寫在前面 (jason3e7):

> `/goal` 跟 Day 26-28 的 PHM CTF 不一樣: PHM 題目是「爛字串裡挑 flag」, agent 跑完一題就結束; HTB 是「真實服務 + 真實憑證 + 真實提權鏈」, 一個 goal 要串 recon → foothold → privesc 整條. 從 CTF 換到 HTB 題目變大, 但 agent 的紀律 (phase 分段, tee 存檔, stop 條件) 反而更能展現價值.
>
> Touch 這題沒解完, 但看到 agent 自己把 RDP + 虛擬螢幕 + 自動點擊 + 截圖這條管線組出來, 比有沒有拿到 flag 更值得記錄.

```markdown
# /goal 跑 HTB — 一贏一卡, 卡的那個 agent 自己架出 RDP 工具鏈
* /goal 四段骨架
  * Target + Environment + 工具自備
  * STRICT logging (全部 tee 到 recon/)
  * Phase 分段 (Recon / Foothold / Flag)
  * 停止條件 + 7 小時硬停
* 案例一 Cap (easy, 贏)
  * 26 分鐘拿 user + root
  * IDOR pcap → FTP → SSH → cap_setuid
  * 分 session 跑 foothold / privesc
  * 中間切 Opus 5 收尾
* 案例二 Touch (卡)
  * Web 進去, 拿到 KioskUser
  * RDP 連得上, 但卡在 Windows kiosk
  * agent 自己架 Xvfb + xfreerdp + xdotool + import
  * safeguard 擋 + Opus 5 太慢放棄
* 收斂
  * /goal 把工程紀律寫進 prompt
  * CTF 吃 pattern, HTB 吃真實服務
  * 真實服務對 agent 比 CTF novelty 友善
```

## `/goal` 是什麼 — The `/goal` Workflow

Claude Code 的 `/goal` 是內建 slash command, 把「目標 + 步驟 + 停止條件」整段 prompt 送給 agent, agent 自己展開成多輪動作, 不用人每輪引導. 本篇兩個案例的 `/goal` 都用同一組四段骨架:

| 段 | 內容 |
|:---|:---|
| Target + Environment | 靶機 IP + 攻擊機 IP + 工具自備清單 (nmap / gobuster / ffuf / nikto / pwncat-cs 等, 缺哪個 agent 自己 apt install) |
| STRICT logging | 所有指令輸出 tee 到 `recon/<name>.txt`, 每個 phase 完成時 append summary 到 `recon/notes.md`, flag 存 `recon/flags.txt`. **這條最重要**, 事後可覆核, 下個 session 也可讀 |
| Phase 分段 | Recon (nmap sC-sV + web enum + ffuf) → Foothold (試 SQLi/SSTI/LFI/RCE/default creds, 記 attempts.txt) → Flag (`/home/*/` 找 local.txt, 存 flags.txt, STOP) |
| 停止條件 | 拿到 flag 就 STOP, 不主動 privesc; 單一 goal 超過 7 小時就停 |

有一份共用的 `CLAUDE.md`. `/goal` 是單次任務的 SOP, `CLAUDE.md` 是 agent 整場的紀律. 原文是英文, 下面是中文摘要:

```markdown
## 環境 (Environment)
- Ubuntu, pentest 工具可能沒裝, HTB VPN 走 tun0

## 工具安裝規則 (Tool installation policy)
- 缺哪個工具 agent 自己裝 (apt / pip3 / go install / 從 GitHub releases 載 binary)

## Logging 規則
- 工作目錄 ~/htb/machine-name/
- 所有輸出 tee 到 ./recon/
- 摘要寫 ./recon/notes.md, flag 寫 ./recon/flags.txt
- 沒 capture 的指令不要跑

## INTEGRITY RULES (CRITICAL)
- 不准上網查這台靶機的 writeup / CVE PoC / 已公開解答
- 一般技術語法 (sqlmap usage 之類) 可以查
- 不准 hydra / medusa 這類帳密爆破
- 所有指令必須完全 non-interactive (不准 vim / python REPL / mysql 互動)
```

這幾條沒寫在 `/goal` prompt 裡, 但 Claude Code 會自動把 `CLAUDE.md` 讀進每次對話的 context, 整場 agent 的行為都受約束. **這才是「agent 自主刷題」跟「script kid 抄 writeup」真正的分野** — 不是靠 agent 聰明, 是靠使用者先把紀律寫成規則. 下面兩個案例會看到這份 CLAUDE.md 直接影響 agent 的解題路徑.

跟 Day 26-28 PHM 的對照兩條:

| 軸 | PHM (Day 26-28) | HTB `/goal` (本篇) |
|:---|:---|:---|
| 任務形狀 | 人指定題目, agent 解題目 | 人提供滲透方法論, agent 用方法論在實務中找解答 |
| 平台等級 | 台灣 Inndy 個人維護的小眾練習站 | 全球 pentest 社群公認, 上百萬使用者, Retired 題有 writeup |

第二條平台等級跟 LeetCode 子系列 (Day 24-25) 走同一條路: 從 zerojudge 爬到 LeetCode. **先小眾練手感, 再上公認平台驗證**, 資安跟演算法兩邊都成立.

## 案例一: Cap (easy, 26 分鐘, 教科書鏈)

### 基本面

| 項目 | 內容 |
|:---|:---|
| 靶機 | HTB **Cap** (Ubuntu 20.04, Easy, Retired, 450 XP) |
| 使用模型 | foothold = **Mythos 5.1** · privesc = **Opus 5** |
| 跑法 | 兩個 `/goal` session (foothold / privesc), 中間人工檢查 |
| 總耗時 | 26 分鐘 (foothold 6m + privesc 1m, 其他是人工檢查) |

### 第一個 `/goal` — foothold

送出去的 prompt 是四段骨架 (Target+Environment / STRICT logging / Phase Recon→Foothold→Flag / 停止條件):

![Cap foothold /goal — 四段 prompt](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/cap-2026-10-13/01-goal-foothold-prompt.jpg)

Mythos 5.1 自己把 Phase 1-3 跑完, 中間經過 nmap / ffuf / 讀 pcap / 試 SSH, 結尾按 goal 的規則**停止**不主動提權:

![Cap foothold 完成 — IDOR → pcap → nathan SSH → user.txt](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/cap-2026-10-13/02-foothold-done.jpg)

### 第二個 `/goal` — privesc

開新 session, 準備照原模型續跑, 一送出 prompt 就被擋. 切 Opus 5 繼續:

![切 Opus 5 + 新 /goal — 本機列舉找提權向量](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/cap-2026-10-13/03-model-opus5-goal-privesc.jpg)

Opus 5 跑完本機列舉, 發現 `/usr/bin/python3.8 = cap_setuid+eip`, 一行 `os.setuid(0)` 拿到 root:

![Privesc 完成 — python3.8 cap_setuid → root.txt](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/cap-2026-10-13/04-privesc-done.jpg)

HTB 平台顯示 Cap 已完成, 全程 26 分鐘:

![HTB Cap solved — jason3e7, 450 XP](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/cap-2026-10-13/05-htb-cap-solved.jpg)

### 攻擊鏈 (agent 自己走)

```
[1] Web dashboard /capture 302 → /data/1  ──IDOR 試 /data/0──▶  其他人的歷史 pcap (72 封包)
[2] 下載 /download/0  ──strings + tcpdump -A──▶  cleartext FTP: nathan / Buck3tH4TF0RM3!
[3] 帳密重用  ──SSH──▶  uid=1001 shell, 讀 user.txt
                        │
[4] 第二個 /goal: sudo -l / SUID / cron 全空
    getcap -r / 命中 → /usr/bin/python3.8 = cap_setuid,cap_net_bind_service+eip
                        │
[5] python3.8 -c 'os.setuid(0)' → uid=0 → /root/root.txt
```

四個環節全部教科書等級, 不是 novelty 梗, 是真實世界常見設定錯誤:

| 環節 | 弱點 |
|:---|:---|
| /data/n | 遞增整數 ID + 無授權檢查 = IDOR |
| FTP | 走 cleartext, pcap 可側錄 |
| 帳密重用 | FTP 密碼跟 SSH 密碼相同 |
| capability | 共用 Python 直譯器被賦予 cap_setuid, 等於全使用者可提權 |

Agent 看一眼 `/netstat` 洩漏「web 以 uid 1001 跑, 能綁 port 80」, 就知道 `getcap` 是提權第一個該查的. 不用人提示.

### 分 session 跑, 中間換模型 (被迫)

Foothold 完成就 STOP, 我手動看 `recon/notes.md` 跟 flag, 再開新 session 跑 privesc. 本來還想用 Mythos 5.1, 一送出 prompt 就被擋:

```
API Error: Mythos 5.1's safeguards flagged this message.
```

處理方式: `/model opus-5`, prompt 一字沒改, Opus 5 直接接受. 純粹就是 safeguard 擋人, 沒有其他原因 (不是為了配置模型強度).

`/goal` 把紀律寫進 prompt, 但擋不住模型本身的 safeguard. 這條分工線 Day 28 已經講過.

## 案例二: Touch (未解, 但 agent 自己架出 RDP 工具鏈)

### 基本面

| 項目 | 內容 |
|:---|:---|
| 靶機 | HTB **Touch** (10.129.4.252, hostname `KIOSK-042`, Windows 11 / Server 2025) |
| 使用模型 | **Mythos 5.1** (中段撞 safeguard, 切 Opus 5 太慢放棄) |
| 跑法 | 一個 `/goal` session, Mythos 跑到 30-40% 卡住 |
| 已拿到 | Web dashboard + `KioskUser / K!0sk2026#` + RDP kiosk 畫面 |
| 沒拿到 | shell / `local.txt` (卡在 kiosk escape) |

### 卡點: 進了 RDP, 但只有一個全螢幕瀏覽器

Agent 透過 Web dashboard 拿到 Windows 帳號密碼 (`KioskUser / K!0sk2026#`). 這台靶機是 Windows Assigned Access + kiosk 模式, RDP 連進去不是桌面, 是個全螢幕的「HTB AIRWAYS Self Check-In」應用. 沒有工作列, 沒有開始鈕, 沒有 cmd. 傳統 pentest 這段靠人用滑鼠在 kiosk 畫面上找 bug, 但 `/goal` 是命令列 agent, 怎麼辦?

### `/goal` 自己架出來的 agentic RDP 工具鏈

這段是本篇最有價值的部分. agent 用 4 個 1990 年代的 Linux 標準套件 (`apt install xfreerdp2 xvfb xdotool imagemagick` 一行搞定) 組出 headless GUI 自動化 pipeline:

```
┌─────────────────────────────────────────────────────────────────┐
│  Claude Code agent (text-only CLI)                              │
│                                                                 │
│  ┌─────────┐  啟動 ┌──────────────┐  連線 ┌──────────────────┐  │
│  │ Xvfb :99│◄─────│ xfreerdp     │─────►│ 10.129.4.252:3389│  │
│  │ 虛擬螢幕│      │ /sec:rdp     │      │ KIOSK-042 Win    │  │
│  └────┬────┘      └──────────────┘      └──────────────────┘  │
│       │ RDP window 畫在 Xvfb                                    │
│       ▼                                                         │
│  ┌─────────┐  截圖  ┌──────────────┐                           │
│  │ import  │─────► │ rdp_shots/   │                           │
│  │ (IM)    │       │ s01.png ...  │                           │
│  └─────────┘       └──────┬───────┘                           │
│                           │ Read tool 讀回來                   │
│                           ▼                                     │
│                    ┌─────────────┐                              │
│                    │ Agent 看圖  │  決定下一步座標 / 按鍵     │
│                    └──────┬──────┘                              │
│                           │                                     │
│                           ▼                                     │
│                    ┌─────────────┐  click / key                 │
│                    │ xdotool     │─────► 送到 Xvfb :99          │
│                    └─────────────┘      送到 xfreerdp 視窗      │
│                                        → 透過 RDP 送到靶機     │
└─────────────────────────────────────────────────────────────────┘
```

四個工具的職責:

| 工具 | 角色 | 關鍵指令 |
|:---|:---|:---|
| **xfreerdp** | RDP client. 這台只允許 Native RDP (RC4), 不吃 NLA/TLS, 要加 `/sec:rdp` 強制 | `xfreerdp /v:10.129.4.252 /u:KioskUser /p:'K!0sk2026#' /sec:rdp /cert:ignore` |
| **Xvfb** | X Virtual Frame Buffer, 沒實體螢幕也能跑 X11 的「虛擬螢幕」 | `Xvfb :99 -screen 0 1280x768x24 &` + `export DISPLAY=:99` |
| **xdotool** | 用程式送 X11 滑鼠/鍵盤事件, 送到 `:99` 等於送進 xfreerdp window | `xdotool mousemove 1197 762 click 1` (點 STAFF LOGIN) |
| **ImageMagick `import`** | 截 X11 window 成 PNG 讓 agent 讀 | `import -window root -display :99 recon/rdp_shots/s03.png` |

### agent 一輪的 loop 長這樣

```bash
# 1) 截圖
import -window root -display :99 recon/rdp_shots/s03.png

# 2) agent 用 Read tool 讀 s03.png 回來, 看到「STAFF LOGIN」按鈕在右下

# 3) 送點擊
DISPLAY=:99 xdotool mousemove 1197 762 click 1

# 4) 等頁面反應
sleep 1

# 5) 截下一張, 回 step 1
import -window root -display :99 recon/rdp_shots/s04.png
```

agent 每一步都用自己的視覺理解決定座標. Claude 讀 PNG 的能力好到可以直接說「STAFF LOGIN 在右下角, 約座標 1197, 762」, 不用 OCR, 不用 element inspector, 不用寫 XPath. 本質跟 Anthropic Computer Use / OpenAI Operator 一樣, 只是 agent 自己用 CLI 工具組出來, 不依賴任何商業產品.

### Agent 看到的畫面, 跟它決定怎麼點

截圖是 agent 自己截自己看的, 不是我截給 agent 看. 兩條點擊路徑:

**路徑一: 待機頁 → STAFF LOGIN → Staff Authentication → SCAN BADGE → Invalid badge format**

![s01 — RDP 連進去的待機頁 "SELF CHECK-IN", 右下有 STAFF LOGIN 按鈕](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s01.png)

這是 agent 連上 RDP 看到的第一張. kiosk 應用「HTB AIRWAYS - Self Check-In」全螢幕, 底部有 Terminal 4 / Gate B7 / Kiosk #042 的機位資訊. 中央大字「SELF CHECK-IN · Touch screen to begin」, 右下角一顆「STAFF LOGIN」按鈕. Agent 想拿 staff 權限 (預期權限較高), 送點擊到右下角座標.

![s02 — 進 Staff Authentication, 中間有 SCAN BADGE 按鈕](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s02.png)

點了右下角之後進到 Staff Authentication 畫面, 中間一個橘色徽章 icon, 下面寫「Place your staff badge face-down on the scanner」, 一顆綠色 SCAN BADGE 按鈕. Agent 下一步點 SCAN BADGE.

![s03 — Invalid badge format, 唯一能做的就是 TRY AGAIN 或 Back](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s03.png)

點了 SCAN BADGE 後畫面變紅色警示: **Invalid badge format**. 這跟 Web 的 `/login/badge` 端點回應一致, 代表 kiosk 上掃 badge 跟 Web 走同一條後端. 試了 60+ 種 QR 格式 (JSON / base64 / JWT / `NX-xxxx` / GUID / MRZ 等) 都是這個錯誤. 這條後來撞 safeguard 停手.

**路徑二: 待機頁點任意位置 → Self Check-In 流程 → LANGUAGE → LOOKUP**

![s06 — LANGUAGE 選單, 20 國語言, 右下有 session timeout 倒數](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s06.png)

從 s01 待機頁點**幾乎任意位置** (不是右下 STAFF LOGIN) 就會進到 LANGUAGE 選單, 20 國語言排成格子. 右下 **Session timeout 01:56**, 2 分鐘閒置就重設, agent 的「截圖 → 看圖 → 送 click」loop 有節奏壓力. 選 LANGUAGE 進下一步.

![s08 — LOOKUP 的 Find Your Booking, 4 種認證方式 (BOOKING REF / PASSPORT / FREQUENT FLYER / E-TICKET)](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s08.png)

選完語言進 LOOKUP 階段. Find Your Booking 有 4 種認證方式切 tab, 預設是 BOOKING REF (需要 ABC123 格式訂位代號 + Last Name). agent 想從輸入欄位找 kiosk escape (檔案選擇器 / 列印對話框 / Alt-Tab / 超連結開新視窗), 但輸入欄位一點就彈畫面上的虛擬鍵盤, `xdotool` 送的實體鍵盤進不去, 這條也斷了.

### 卡住的四條線

| 線 | 怎麼卡的 |
|:---|:---|
| Badge QR 格式 | 60+ 種試過都 Invalid badge format |
| Kiosk 輸入欄位 | 一點就彈虛擬鍵盤, xdotool 的實體鍵盤進不去 |
| Mythos 5.1 safeguard | 試到連續 60 次偽造 token 時整個 turn 被 drop |
| 切 Opus 5 太慢 | 等 context 載入 + 推理超時, 手動按停放棄 |

跟 Day 28 PHM 的[四面牆](https://ithelp.ithome.com.tw/articles/10423733)一致, safeguard 不同模型敏感度不同. 這份 note 的 `recon/` 只有 Mythos 5.1 的紀錄, 放棄後沒截 Opus 5 的輸出.

## 收斂 — Takeaways

一個觀察: `/goal` 的紀律 = 可覆核. tee 存檔 + Phase 分段 + 停止條件讓事後能看懂 agent 做了什麼. 這對 pentest / debugging / 任何有風險的 agentic 任務都重要. 不是「跑完給答案」, 是「跑完有完整證據鏈」.

### Agent 自己架出 GUI pipeline

一週前我還以為 agent 遇到 GUI (RDP / 瀏覽器 / X11 app) 就卡死, 因為 Claude Code 本質是文字 CLI. 但 Touch 看到的是: agent 自己拉出 Xvfb + xfreerdp + xdotool + import 四個套件, 組成「開 RDP → 截圖 → 看圖 → 送點擊」的 loop. 不是我教的, 是 agent 遇到問題後自己想出來的解法.

這跟 Anthropic Computer Use beta 一樣, 只是用 1990 年代的 Linux 工具組出來. 不等 Anthropic 把 Computer Use 做成正式產品, agent 已經能自己架出同樣的能力.

## Sources

- [Day 28: PHM CTF — AI agent 的四面牆](https://ithelp.ithome.com.tw/articles/10423733) — safeguard 的另一面
- [FreeRDP — `/sec:rdp` 文件](https://github.com/FreeRDP/FreeRDP/wiki/CommandLineInterface)
- [xdotool](https://github.com/jordansissel/xdotool) — X11 鍵鼠自動化
