---
title: AI 101 - HTB Goal Prompt 設計指南
tags: [ai, 個人筆記, ctf, htb, goal, prompt-engineering, pentest, claude-code]
created: 2026-06-10
---

# AI 101 - HTB Goal Prompt 設計指南

> [!info]
> 基於 PentestGPT、BoxPwnr、HackSynth 等框架的研究，整理出在 Claude Code `/goal` 裡跑 HTB 機器的 prompt 設計原則與可直接使用的模板。

---

## 為什麼 AI 在 HTB 會失敗

跨多篇論文確認的共同失敗模式，不是模型笨，而是 prompt 沒給夠結構：

| 失敗模式 | 原因 | 解法 |
|---|---|---|
| 一直重跑同樣的 nmap/gobuster | 沒有外部記憶，recent bias | 每輪把已試路徑寫進 `recon/notes.md`，resume 時重新注入 |
| 找到 RCE 就停了 | 沒有 pre-completion checklist | 明確寫「RCE 不是目標，讀到 local.txt 才是」 |
| context 滿了忘事 | 超過 40% context 後準確度明顯下降 | 分段跑（user flag / root flag 各一次 `/goal`） |
| 卡住原地打轉 | fallback 指示太模糊（「試試別的」） | 列出具體備案指令清單 |
| 互動式指令 hang 住 | `vim`、`mysql`、`python REPL` 等待輸入 | 明確禁止互動式指令 |
| 每次重跑走一樣的死路 | 沒有 resume 機制 | 手動維護 `progress.md` |

---

## 五個核心設計原則

### 1. Stage 分離：Recon 和 Exploit 不要混在一起

Recon stage 只看、不動；Exploit stage 才深入。混在一起會讓 AI 在偵察不完整時就跳去試 exploit，漏掉向量。

### 2. Fallback 清單要具體，不是「試試別的」

```
# 壞的寫法
If stuck, try different approaches.

# 好的寫法
If no RCE after web enum, try in order:
- LFI: ../../../etc/passwd, /proc/self/environ, log poisoning (Apache: /var/log/apache2/access.log)
- SSTI: {{7*7}}, ${7*7}, <%= 7*7 %>, #{ 7*7 }
- SQLi: ' OR 1=1--, UNION SELECT NULL--, time-based: ' AND SLEEP(5)--
- Default creds: admin:admin, admin:password, admin:123456, root:root
- HTTP methods: OPTIONS/PUT/PATCH on every endpoint
- Source code comments and hidden form fields
```

### 3. Pre-completion Checklist：找到 flag 前強制答三題

```
Before reporting completion, verify:
1. Have I run find /home -name local.txt 2>/dev/null? [yes/no]
2. Have I cat'd the file and seen the actual content? [yes/no]
3. Is the content saved to recon/flags.txt? [yes/no]
If any answer is NO, continue.
```

### 4. 禁止互動式指令

```
All commands must be fully non-interactive.
NEVER run: vim, nano, less, more, python (REPL), mysql (interactive),
           psql (interactive), ftp (interactive), telnet.
Instead use: echo/printf pipes, heredocs, -e flags, -c flags, batch mode.
```

### 5. Progress 注入：每次重跑前更新

把死路和目前假設寫進 prompt，讓 AI 不要重走。格式參考 BoxPwnr 的 `progress.md`：

```markdown
## PREVIOUS ATTEMPTS (update before each run)
### Dead Ends — DO NOT RETRY
- SSH: port 22 filtered
- /admin: 401, no default creds worked
- SQLi on login form: WAF blocking

### Current State
- Have: web shell via LFI + log poisoning
- Next: upgrade to reverse shell

### Hypothesis
- Log poisoning via User-Agent header → /var/log/apache2/access.log
```

---

## 模板

### Stage 1：取得 User Flag

```
Environment: Kali Linux (install missing tools with apt/pip/go)

Logging (STRICT — no exceptions):
- All tool output: tee to ./recon/<tool>.txt
- Summaries: append to recon/notes.md after each phase
- Flag: save to recon/flags.txt

Rules:
- NO credential brute force with wordlists (no hydra/medusa on SSH/FTP)
- ALL commands must be fully non-interactive (no vim, python REPL, mysql interactive)
- Breadth-first: max 3 attempts per vector
  Exception: RCE/injection — iterate payload variations until confirmed or ruled out
- Before any reverse shell, confirm outbound connectivity (try curl/wget/ping on port 80, fallback 443)
- If no progress after exhausting a vector, stop and update recon/notes.md with what was tried,
  then move to next vector

## PREVIOUS ATTEMPTS
### Dead Ends — DO NOT RETRY
(fill in before each run)

### Current State
(fill in before each run)

---

Objective: Read local.txt → STOP. Do NOT attempt privilege escalation.

Before reporting done, verify:
1. find /home -name local.txt 2>/dev/null returned a path? [yes/no]
2. cat'd the file and saw content? [yes/no]
3. Saved to recon/flags.txt? [yes/no]
If any NO → continue.

── STAGE 1: RECON ──────────────────────────────────
Only enumerate. No exploitation in this stage.

1. nmap -sC -sV -p- <TARGET> | tee recon/nmap_full.txt
2. For each HTTP port:
   - gobuster dir -u http://<TARGET> -w /usr/share/wordlists/dirb/common.txt | tee recon/gobuster.txt
   - curl -s http://<TARGET>/robots.txt
   - whatweb http://<TARGET> | tee recon/whatweb.txt
   - view page source for comments/hidden fields
3. Note all services, versions, interesting findings to recon/notes.md

── STAGE 2: FOOTHOLD ───────────────────────────────
Research identified services → exploit.
If a vector shows ANY sign of life, exhaust it fully before moving on.
Log all attempts to recon/exploit_attempts.txt

If stuck with no shell after all vectors:
- LFI: ../../../etc/passwd, /proc/self/environ, log poisoning
- SSTI: {{7*7}}, ${7*7}, <%= 7*7 %>
- SQLi: ' OR 1=1--, UNION SELECT, AND SLEEP(5)--
- Default creds: admin:admin, admin:password, root:root, guest:guest
- HTTP methods: OPTIONS/PUT/PATCH on all endpoints
- Re-read page source for hints

── STAGE 3: FLAG ───────────────────────────────────
find /home -name local.txt 2>/dev/null
cat <path> | tee -a recon/flags.txt
Report flag content and STOP.
```

### Stage 2：Privilege Escalation

```
Environment: Kali Linux
Already have: user shell as <USERNAME> on <TARGET>

Logging:
- All output: tee to ./recon/privesc_<tool>.txt
- Notes: append to recon/privesc_notes.md

Rules:
- ALL commands non-interactive
- If a privesc vector fails twice, move to next

## PREVIOUS ATTEMPTS
### Dead Ends — DO NOT RETRY
(fill in)

---

Objective: Read /root/proof.txt → STOP.

Before reporting done:
1. Confirmed root/SYSTEM shell? [yes/no]
2. cat /root/proof.txt returned content? [yes/no]
3. Saved to recon/flags.txt? [yes/no]

── ENUM ────────────────────────────────────────────
whoami && id && uname -a | tee recon/privesc_enum.txt
sudo -l 2>/dev/null
find / -perm -4000 2>/dev/null | tee recon/suid.txt
cat /etc/crontab; ls -la /etc/cron* 2>/dev/null
find / -writable -not -path "*/proc/*" 2>/dev/null | grep -v "^/sys" | tee recon/writable.txt
cat /etc/passwd | grep -v nologin | grep -v false

If nothing obvious: download and run linpeas.sh
curl -L https://github.com/peass-ng/PEASS-ng/releases/latest/download/linpeas.sh -o /tmp/linpeas.sh
chmod +x /tmp/linpeas.sh && /tmp/linpeas.sh | tee recon/linpeas.txt

── EXPLOIT ─────────────────────────────────────────
Identify most promising vector → exploit → root shell
Log steps to recon/privesc_notes.md

── FLAG ────────────────────────────────────────────
cat /root/proof.txt | tee -a recon/flags.txt
Report and STOP.
```

---

## 為什麼比直接改 prompt 更有效

原本一個 prompt 塞所有規則，AI 在 context 累積後會忘記前面的規則。把 Stage 1 和 Stage 2 拆開：

- 每次 `/goal` 的 context 從零開始，不帶前一輪的雜訊
- `## PREVIOUS ATTEMPTS` 手動更新，只帶「有用的資訊」進去
- Stage 2 比 Stage 1 的 token budget 小很多（目標明確，不需要探索）

---

## 相關筆記

- [[AI 101 - HTB Abducted — goal 實戰案例]] — 第一次成功的完整紀錄（舊 prompt 版本）
- [[AI 101 - BoxPwnr]] — progress.md 注入模式的出處
- [[AI 101 - AI 自主滲透測試工具比較]] — PentestGPT 等框架的 prompt 設計參考

## Sources

- [PentestGPT USENIX 2024](https://www.usenix.org/conference/usenixsecurity24/presentation/deng)
- [BoxPwnr GitHub](https://github.com/0ca/BoxPwnr)
- [HackSynth arXiv 2412.01778](https://arxiv.org/abs/2412.01778)
- [Guided Reasoning with Structured Attack Trees arXiv 2509.07939](https://arxiv.org/abs/2509.07939)
- [What Makes a Good LLM Pentest Agent arXiv 2602.17622](https://arxiv.org/abs/2602.17622)
