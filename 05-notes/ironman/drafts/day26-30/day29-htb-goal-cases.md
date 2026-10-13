title : [Day 29] 從 CTF 換到 HTB — 用 `/goal` 一次打贏一次卡在 kiosk, agent 自己架出 RDP 工具鏈

> [Day 26-28](https://ithelp.ithome.com.tw/articles/10423733) 把 PHM CTF 刷到 Top 10, 本篇換場景: 從 CTF 換到 **HTB** (Hack The Box, 真實系統/網路設定的滲透靶機), 從手動 Claude Code 換成 `/goal` **一條指令跑整台**. 用兩個同一天 (2026-10-13) 跑的真實案例看 `/goal` 這個 workflow 的能跟不能: **Cap** (easy, 26 分鐘拿 user+root) 跟 **Touch** (未解, 卡在 kiosk 逃逸, **但 agent 自己架出 RDP 看圖點按鈕的工具鏈**).

寫在前面 (jason3e7):

> `/goal` 跟 Day 26-28 的 PHM CTF 不一樣: PHM 題目是「爛字串裡挑 flag」, agent 跑完一題就結束; HTB 是「真實服務+真實憑證+真實提權鏈」, 一個 goal 要串 recon → foothold → privesc 整條. 從 CTF 換到 HTB 是一次升級 — 題目變大, 但 agent 的紀律 (phase 分段、tee 存檔、stop 條件) 反而更能展現價值.
>
> Touch 這題雖然沒解完, 但看到 agent **自己把 RDP + 虛擬螢幕 + 自動點擊 + 截圖** 這條管線組出來, 比有沒有拿到 flag 更值得記錄.

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

Claude Code 的 `/goal` 是一條**內建 slash command**, 把一段「目標 + 步驟 + 停止條件」整段 prompt 送給 agent, agent 就會自己展開成多輪動作, 不用人每輪引導. 本篇兩個案例的 `/goal` 都用同一組**四段骨架**:

1. **Target + Environment**: 靶機 IP + 攻擊機 IP + 工具自備清單 (`sudo apt install -y nmap gobuster curl netcat-traditional python3 python3-pip whatweb nikto; pip3 install pwncat-cs; sudo apt install -y ffuf`). 遇到工具不在, agent 自己裝
2. **STRICT logging**: 所有指令輸出 `tee` 到 `recon/<name>.txt`, 每個 phase 完成時 append 一段 summary 到 `recon/notes.md`, flag 存 `recon/flags.txt`. **這條最重要** — 事後覆核 + 下個 session 可讀
3. **Phase 分段**: Recon (nmap sC-sV + web enum + ffuf) → Foothold (研究服務漏洞, 試 SQLi/SSTI/LFI/RCE/default creds, 記 attempts.txt) → Flag (`/home/*/` 找 local.txt, 存 flags.txt, STOP)
4. **停止條件**: 拿到 flag 就 STOP, 不主動 privesc; 單一 goal 超過 7 小時就停

這套跟 Day 26-28 的 PHM CTF 跑法差很多: PHM 的 Claude Code 偏「人指路, agent 跑單題」; `/goal` 是「人設方向, agent 跑整條鏈」. 下面兩個案例剛好看出這個差別.

## 案例一: Cap (easy, 26 分鐘, 教科書鏈)

### 基本面

| 項目 | 內容 |
|:---|:---|
| 靶機 | HTB **Cap** (Ubuntu 20.04, Easy, Retired, 450 XP) |
| 使用模型 | foothold = **Mythos 5.1** · privesc = **Opus 5** |
| 跑法 | 兩個 `/goal` session (foothold / privesc), 中間人工檢查 |
| 總耗時 | 26 分鐘 (foothold 6m + privesc 1m, 其他是人工檢查) |

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

四個環節全部**教科書等級** — 不是 novelty 梗, 是真實世界常見設定錯誤:
- `/data/<n>` 用遞增整數 ID + 無授權檢查 = IDOR
- FTP 走 cleartext = pcap 可側錄
- FTP 密碼跟 SSH 密碼重用
- 共用 Python 直譯器被賦予 `cap_setuid` = 全系統使用者的 root 權限

Agent 看一眼 `/netstat` 洩漏 "web 以 uid 1001 跑 + 能綁 port 80", 心裡就知道 **getcap** 是提權第一個該查的 — 這跳跟教科書上寫的一樣, 不需要人提示.

### 關鍵設計: 分 session 跑, 中間換模型

- Foothold 跑完 **STOP** (goal 本來就寫了「拿到 user.txt 就停」), 我手動看 `recon/notes.md` 跟 flag, 然後開新 session
- 新 session: `/model opus-5` + 新 `/goal` (提權版 prompt, 不帶 Phase 1 的 recon 內容)
- 好處: **context 乾淨, 沒 recent bias** (foothold session 的 context 已經塞滿 nmap/ffuf/web_data 輸出, 繼續跑 agent 容易一直想 revisit 已跑過的)
- 另一個好處: 可以**用對速度 / 深度各自適合的模型** — Mythos 5.1 跑 web 列舉飛快, Opus 5 讀列舉輸出 → 推理哪條線最有希望

這就是 `/goal` 跟自由 chat 的最大差別: **把紀律寫進 prompt**, 讓 agent 不會因為 context 疲勞或模型隨性而跑偏.

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

Agent 透過 Web dashboard 拿到 Windows 帳號密碼 (`KioskUser / K!0sk2026#`), 但這台靶機是 **Windows Assigned Access + kiosk 模式** — RDP 連進去不是桌面, 是個全螢幕的 "HTB AIRWAYS — Self Check-In" 應用, 沒有工作列、沒有開始鈕、沒有 cmd. 傳統 pentest 這段靠人**用滑鼠在 kiosk 畫面上找 bug**, 但 `/goal` 是**命令列 agent**, 怎麼辦?

### `/goal` 自己架出來的 agentic RDP 工具鏈

這段是本篇最有價值的部分. agent 用 **4 個 1990 年代的 Linux 標準套件** (`apt install xfreerdp2 xvfb xdotool imagemagick` 一行搞定) 組出 headless GUI 自動化 pipeline:

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

**agent 每一步都用自己的視覺理解決定座標**. Claude 讀 PNG 的能力好到可以直接說「STAFF LOGIN 在右下角, 約座標 1197, 762」, 不用 OCR、不用 element inspector、不用寫 XPath. 這**本質跟 Anthropic Computer Use / OpenAI Operator 一樣**, 只是 agent 自己用 CLI 工具組出來, 不依賴任何商業產品.

### 為什麼卡住

- Kiosk UI 的輸入欄位會彈**畫面上的虛擬鍵盤**, xdotool 送的實體鍵盤輸入不進欄位
- Badge QR 格式 60+ 種試過都 "Invalid badge format", 進不去 Staff 介面
- Mythos 5.1 試到連續 60 次偽造 token 時, cyber safeguard 擋人
- 切 Opus 5 要從頭重讀 context 太慢, 手動放棄

這跟 Day 28 PHM 的[四面牆](https://ithelp.ithome.com.tw/articles/10423733)完全一致 — safeguard 不是全模型一致, 不同模型敏感度不同. 但這次我做了跟前兩案不一樣的決定: **放棄接 Opus 5**. 原因是這次 Mythos 只推到 30-40% 就被擋, Opus 5 要整段重跑, 等的時間可能大於自己接手手動幹.

**新規則**: safeguard 擋人後, **先看題目推到多深**才決定要不要換模型. 推到 90% 換模型最賺 (Opus 5 收尾快), 推到 30% 換模型 = 整段重跑, 不如停手.

## 收斂 — Takeaways

兩個案例攤開, 可以看出 `/goal` 這個 workflow 的三個性質:

| 性質 | Cap (贏) | Touch (卡) |
|:---|:---|:---|
| 把工程紀律寫進 prompt | ✅ phase 分段讓 agent 不混亂 | ✅ 同樣的骨架也 work |
| agent 自己展開能力 | 看 netstat → 推 getcap | **自己架 RDP + Xvfb + xdotool + import pipeline** |
| safeguard + 模型切換 | 分 session 中間換, 乾淨切 Opus 5 | 中段撞牆, Opus 5 續跑太慢放棄 |

兩個觀察:

1. **`/goal` 的紀律 = 可覆核**. tee 存檔 + Phase 分段 + 停止條件讓**事後能看懂 agent 做了什麼**. 這點對 pentest / debugging / 任何有風險的 agentic 任務都重要. 不是「跑完給答案」, 是「跑完有完整證據鏈」
2. **真實攻擊面比 CTF novelty 梗對 agent 友善**. HTB (包括 Cap easy) 的漏洞都在真實軟體的 CVE / writeup 裡, agent training data 吃得下. PHM CTF 作者發明的 trivia 梗 agent 反而吃不下 (見 [Day 28 四面牆](https://ithelp.ithome.com.tw/articles/10423733)). 這條分工線 **對日常工作有啟示** — agent 做「用別人造的輪子組出解法」強, 做「自己想一個沒人想過的梗」弱

### Agent 展現的新能力 — 自己架 GUI pipeline

這點值得單獨強調. 一週前我還會以為 agent 遇到 GUI (RDP / 瀏覽器 / X11 app) 就卡死, 因為 Claude Code 本質是文字 CLI. 但 Touch 這題看到的是: **agent 自己拉出 Xvfb + xfreerdp + xdotool + import 四個套件, 組成一條「開 RDP → 截圖 → 看圖 → 送點擊」的 loop**. 這不是我教的, 是 agent 遇到問題 (「我要操作 Windows kiosk 怎辦」) 之後自己想出來的解法.

這跟 Anthropic 的 Computer Use beta 一樣, 只是 agent 用 1990 年代的 Linux 工具組出來的. **不等 Anthropic 把 Computer Use 做成正式產品, agent 已經能自己架出同樣的能力**. 這個 pattern 以後只會更常見.

這也是整個鐵人賽子系列 (Day 24-29) 想傳達的: AI 的能力邊界不是「它會什麼」, 是**「我們願意把 workflow 設計到多工程化」** — `/goal` 的四段骨架 + tee 存檔 + phase 分段, 不是魔法, 是把人在腦袋裡的 pentest SOP 寫下來給 agent 讀. Day 30 總結會把這條線拉完.

## Sources

- [HTB Cap — 本 repo 完整案例](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/cap-2026-10-13/cap-with-goal.md)
- [HTB Touch — 本 repo 完整案例 + 10 張 RDP 截圖](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/touch-2026-10-13/touch-with-goal.md)
- [HTB Goal Prompt 設計指南 (本 repo)](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/htb-goal-prompt-guide.md)
- [Day 28: PHM CTF — AI agent 的四面牆](https://ithelp.ithome.com.tw/articles/10423733) — safeguard 的另一面
- [FreeRDP — `/sec:rdp` 文件](https://github.com/FreeRDP/FreeRDP/wiki/CommandLineInterface)
- [xdotool](https://github.com/jordansissel/xdotool) — X11 鍵鼠自動化
