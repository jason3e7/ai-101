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

這套跟 Day 26-28 的 PHM CTF 跑法差很多: PHM 的 Claude Code 偏「人指定題目, agent 解題目」; `/goal` 是「人提供滲透測試方法論, agent 透過方法論在實務中找解答」. 下面兩個案例剛好看出這個差別.

跟 Day 26-28 PHM 的另一個不一樣是**平台等級**: 從**小眾 → 公認平台**. PHM 是台灣 Inndy 個人維護的練習站, HTB 是全球 pentest 社群公認的訓練平台 (累積上百萬使用者 + Retired 的 writeup 可查). 這跟 LeetCode 子系列 (Day 24-25) 的路線一樣 — 從 zerojudge (台灣高中生競賽) 爬到 LeetCode (全球 FAANG 面試題庫). **先在小眾平台練手感, 再上公認平台驗證**, 這條線在資安跟演算法兩邊都成立.

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

### 關鍵設計: 分 session 跑, 中間換模型 (被迫)

- Foothold 跑完 **STOP** (goal 本來就寫了「拿到 user.txt 就停」), 我手動看 `recon/notes.md` 跟 flag, 然後開新 session
- 新 session 本來還是想用 Mythos 5.1, 一送出第二個 `/goal` (提權版) 就被擋:

  ```
  API Error: Mythos 5.1's safeguards flagged this message.
  ```

- 處理方式: `/model opus-5`, prompt **一字沒改**, Opus 5 直接接受. **純粹就是 safeguard 擋人, 沒有其他原因** (不是為了「用更強模型推理」或「速度/深度配置」, 單純是 Mythos 5.1 看到提權關鍵字就拒跑)

這也顯示 `/goal` 跟自由 chat 的差別: **把紀律寫進 prompt** 讓 agent 不會因為 context 疲勞或模型隨性而跑偏, 但也擋不住模型本身的 safeguard — 這條分工線 Day 28 已經講過.

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

### 實際的截圖 (點擊前 / 點擊後)

這段的截圖是 **agent 在 Claude Code 裡自己截自己看的**, 不是我截 agent 看的畫面. 挑兩對代表性的 before/after:

**Pair 1 — Agent 看待機頁 → 決定點右下 STAFF LOGIN → 進到 Staff 認證失敗**

![s01 — RDP 連進去的待機頁, agent 看到右下有 STAFF LOGIN 按鈕](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s01.png)

Agent 在這張看到: 中央 "SELF CHECK-IN" + 底部 "STAFF LOGIN" 按鈕. 目標是進 Staff 介面 (權限較高), 所以決定點右下的 STAFF LOGIN. `xdotool mousemove 1197 762 click 1`.

![s04 — 點 STAFF LOGIN 進去, 再試 SCAN BADGE, 得到 Invalid badge format 錯誤](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s04.png)

幾輪點擊 (STAFF LOGIN → SCAN BADGE) 之後, agent 卡在這張: **Invalid badge format**. 跟 Web 的 `/login/badge` 端點回應一致. agent 試了 60+ 種 QR 內容格式 (JSON / base64 / JWT / `NX-xxxx` / GUID / MRZ 等) 都是這個錯誤. 這條後來撞 safeguard 停手.

**Pair 2 — Agent 切另一條路, 走 Self Check-In 流程探索**

![s05 — 回到待機頁, agent 換一條路, 點中央 Touch 開始 Self Check-In](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s05.png)

Staff 介面卡住後, agent 自己決定試另一條: 點中央 "Touch screen to begin" 開始客人用的 Self Check-In 流程, 看能不能從裡面找 kiosk escape (檔案選擇器 / 列印對話框 / 超連結開新視窗 / Alt-Tab 等).

![s06 — Self Check-In 的 LANGUAGE 選單, 20 國語言 + session timeout 01:56 倒數](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/touch-2026-10-13/rdp-s06.png)

進到 LANGUAGE 選單. 注意右下 **Session timeout 01:56** — kiosk 2 分鐘閒置就重設, agent 每輪都要在 2 分鐘內推進, 這對「截圖 → 看圖 → 送 click」的 loop 是個節奏壓力.

這 10 張截圖連起來看, 等於是 agent 在 Windows kiosk 裡**自己走了一遍 UX**, 像一個不懂中文的用戶在機場自助報到機前面摸索.

### 為什麼卡住

- Kiosk UI 的輸入欄位會彈**畫面上的虛擬鍵盤**, xdotool 送的實體鍵盤輸入不進欄位
- Badge QR 格式 60+ 種試過都 "Invalid badge format", 進不去 Staff 介面
- Mythos 5.1 試到連續 60 次偽造 token 時, cyber safeguard 擋人
- 切 Opus 5 要從頭重讀 context 太慢, 手動放棄

這跟 Day 28 PHM 的[四面牆](https://ithelp.ithome.com.tw/articles/10423733)完全一致 — safeguard 不是全模型一致, 不同模型敏感度不同. 但這次我做了跟前兩案不一樣的決定: **放棄接 Opus 5**. 單純是等 Opus 5 載入 context 跟接續推理花了一段時間, 我手動按停. 這份 note 的 `recon/` 只有 Mythos 5.1 的紀錄, 就是因為放棄後沒截 Opus 5 的輸出.

## 收斂 — Takeaways

一個觀察: **`/goal` 的紀律 = 可覆核**. tee 存檔 + Phase 分段 + 停止條件讓**事後能看懂 agent 做了什麼**. 這點對 pentest / debugging / 任何有風險的 agentic 任務都重要. 不是「跑完給答案」, 是「跑完有完整證據鏈」.

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
