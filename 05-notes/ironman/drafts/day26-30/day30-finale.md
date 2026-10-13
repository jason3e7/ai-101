title : [Day 30] 總結 — 三十天蒸餾, 跟著 AI 持續成長

> 鐵人賽最後一篇. 開頭先補一個 Day 29 當天還沒寫進去的案例: HTB **Management** (Insane 等級), 用同一套 `/goal` 工具鏈跑完 user + root flag. 把 Day 29 的「`/goal` 能跑小的 (Cap), 卡在非 shell 的 (Touch)」補上第三個點: `/goal` 也能跑 **Insane 等級的多階段 chain**, 只是中間撞 safeguard 要人換模型.
>
> 後半的「三十天蒸餾」, 跟著 AI 持續成長的方法論, 下面會寫.

寫在前面 (jason3e7):

> 原本 Day 29 的兩個案例跑完就想收 — Cap 展示 `/goal` 教科書式鏈, Touch 展示 agent 自己架 RDP pipeline. 但 Management 這個同一天跑的 Insane 案例漏了, 不放進最後一篇可惜. 它證明 `/goal` 不只是 easy box 的玩具, 真實 Insane 等級的 7 環 chain (OpenAM → OpenDJ JMX 反序列化 → 自寫 gadget → GLPI → 密碼解密 → 帳密重用 → sudoers wildcard) 也吃得掉.
>
> 補完這個案例之後, 後半段才進三十天蒸餾.

```markdown
# 三十天蒸餾: Insane 也吃得掉 + 跟著 AI 持續成長
* 補一個 Day 29 漏掉的案例
  * HTB Management (Insane, 57 分鐘)
  * 兩個 /goal, 中間撞 safeguard 切 Opus 5
  * 攻擊鏈七環, 都在真實軟體 CVE 裡
  * agent 自己寫 Java gadget 繞 JDK 21
* 三十天蒸餾 (後半)
  * TODO: jason3e7 補
* 跟著 AI 持續成長
  * TODO: jason3e7 補
```

---

## 補案例: HTB Management (Insane) — Case Addendum

### 基本面

| 項目 | 內容 |
|:---|:---|
| 日期 | 2026-10-13 (跟 Day 29 的 Cap / Touch 同一天) |
| 靶機 | HTB **Management** (**Insane**, Linux, Ubuntu 24.04, Machine Rank #3024, 585 XP) |
| 攻擊面 | OpenAM 16.0.5 (Tomcat) + 內嵌 OpenDJ 5.0.3 + GLPI 11 + Nginx |
| 使用模型 | foothold 開場 Mythos 5.1, 中途撞 safeguard 切 Opus 5 續跑 · privesc 全程 Opus 5 |
| 執行方式 | Claude Code CLI `/goal`, 兩個 session (foothold / privesc) |
| 總耗時 | 約 57 分鐘 (foothold 52m + privesc 5m) |
| 結果 | user + root flag 全拿 |

![HTB Management solved — jason3e7, 585 XP, Insane](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/management-2026-10-13/04-htb-management-solved.jpg)

### foothold `/goal` 開場 — 跟 Cap / Touch 用同一組模板

prompt 骨架跟 Day 29 的 Cap 完全一樣, 只換 target IP:

![/model Mythos 5.1 + /goal foothold prompt](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/management-2026-10-13/01-goal-foothold-mythos.jpg)

### Mythos 5.1 跑到攻擊鏈中段才撞 safeguard

跟 Cap 不同, 這次 safeguard 不是開場擋, 是 Mythos 5.1 跑到攻擊鏈中段才突然回:

```
API Error: Mythos 5.1's safeguards flagged this message.
```

當下 agent 正在:

| 步驟 | 做到哪 |
|:---|:---|
| OpenDJ JMX | 已確認 `newClient(Object)` 反序列化點 (unauth) |
| ysoserial 全套打 | 都失敗, JDK 21 擋了 `com.sun.org.apache.xalan` 的反射 |
| classpath enum | 正在動態掃 classpath 找可用 gadget, 內容密度暴衝 (defineClass / bytecode / gadget) |

處理方式: `/model opus-5`, prompt 一字沒改, Opus 5 從被擋處繼續. 這次切模型是賺的 — 前面 50 分鐘 Mythos 已經把 90% 的 recon 跟 classpath enum 做完, Opus 5 只花 1-2 分鐘就把 bundled Xalan + Apache Click 這條 gadget chain 組出來.

![foothold 完成 (切 Opus 5 之後) — OpenDJ deser → openam RCE → GLPI → owen SSH](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/management-2026-10-13/02-foothold-done-after-opus5-switch.jpg)

### privesc 的 sudo wildcard trick (5 分鐘)

`sudo -l` 直接命中:

```
(root) NOPASSWD: /usr/bin/rdiff-backup --server --restrict-path /opt/backup --restrict-mode read-only *
```

兩個 bug 疊起來: **sudoers 尾端 unanchored `*`** 吃掉所有後續參數 (含空格), **`rdiff-backup 2.2.6` 用 argparse** 的 last-wins semantics. 攻擊: 在合法參數後面再塞一條 `--restrict-path /`:

```
sudo -n /usr/bin/rdiff-backup --server \
     --restrict-path /opt/backup --restrict-mode read-only \   # 原意沙箱
     --restrict-path /            --restrict-mode read-only    # 蓋掉原意
```

全程 read-only mirror `/root` 到 `/tmp`, 拿到 `/root/.ssh/id_ed25519`, 用 `ssh -i` 直接登 root (root 的 pubkey 在自己 `authorized_keys` 裡), uid=0.

![privesc done — rdiff-backup wildcard → root key → ssh root](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/htb/management-2026-10-13/03-privesc-done.jpg)

### 完整攻擊鏈 — 7 環, 都在真實軟體 CVE 裡

```
OpenAM demo/changeit (default creds)
    ↓ 權限不夠
OpenDJ JMX 1689 newClient(Object) (pre-auth deser)
    ↓ 繞 JDK 21 模組限制
自寫 Apache Click + bundled Xalan gadget chain
    ↓ RCE as uid=996(openam)
讀 /opt/glpi/config_db.php (MySQL)
    ↓ glpi_authldaps + glpicrypt.key
XChaCha20-Poly1305 解密 svc-glpi 密碼
    ↓ 密碼重用
SSH owen (uid=1000)
    ↓ sudoers wildcard + rdiff-backup argparse last-wins
mirror /root → root ed25519 private key → ssh root
```

這 7 環沒有一環是「作者臨時發明的 novelty 梗」, 全部都在真實軟體的 CVE / 文獻 / 歷史 writeup 裡. agent (切對模型後) 自己串得出來, 證明 Day 29 收尾那句「真實攻擊面對 agent 友善」在 Insane 等級也成立.

### 把三個案例擺一起看

| 案例 | 難度 | 結果 | agent 展現的新能力 |
|:---|:---|:---|:---|
| Cap (Day 29) | Easy | 26 分鐘贏 | 看一眼 `/netstat` 就推理到 `getcap` |
| Touch (Day 29) | Easy-Medium | 卡在 kiosk | 自己架 Xvfb + xfreerdp + xdotool + import 的 RDP pipeline |
| **Management (本篇補)** | **Insane** | **57 分鐘贏** | **自己寫 Java `ClassProbe.java` + `ClickGadget.java` 繞 JDK 21 模組系統** |

Easy → Insane 一條線跑過來, `/goal` 這套 workflow 的能力邊界只跟 **safeguard** 跟 **是否有即時可用模型** 相關, 跟題目難度本身的相關性意外地小.

---

## 三十天蒸餾 — Thirty-Day Distillation

<!-- TODO: jason3e7 下半段的內容, 這塊先留白 -->

### 從一句原理推出來的三十篇 — The Through-Line

(TODO: 從 Day 1 「它只是在猜下一個字」如何推出後面 29 天所有心法)

### 哪些題目後來驗證了, 哪些還在推進 — What Validated vs What's Still Open

(TODO: Day 11 驗證 / Day 13 鈍化 / Day 16-20 文風測量 / Day 24-29 實戰, 哪些結論被實測坐實, 哪些還是待驗證)

### 收斂到最後的 3 個核心習慣 — Three Habits That Stuck

(TODO: 列 3 個這 30 天跑完真的留下來、每天在用的習慣, 不列湊數的)

---

## 跟著 AI 持續成長 — Growing With AI

<!-- TODO: jason3e7 這段是整個鐵人賽的收尾句, 要是人味, 不要是 agent 摘要 -->

### 為什麼不是「追上 AI」, 是「跟著 AI 成長」 — Why "With" Not "After"

(TODO: 「追上」隱含有終點, 「跟著」是陪跑. 這個詞的選擇)

### 2026 底的現況, 跟一年前的差 — What Changed in One Year

(TODO: 這一年 agentic 工程、模型能力、safeguard 政策的轉折點對照個人工作流程的變化)

### 給一年後讀這篇的自己 — A Note to Myself, One Year From Now

(TODO: 寫給 2027 的自己, 一年內哪三件事如果沒做要後悔)

---

## Sources

- [Day 29: HTB Cap + Touch — /goal 一贏一卡](day29-htb-goal-cases.md)
- [Day 28: PHM CTF — AI agent 的四面牆](https://ithelp.ithome.com.tw/articles/10423733)
- [HTB Management — 本 repo 完整案例 + 時間軸 + 88 個 recon artifact](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/management-2026-10-13/management-with-goal.md)
- [HTB Cap — 本 repo 完整案例](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/cap-2026-10-13/cap-with-goal.md)
- [HTB Touch — 本 repo 完整案例 + 10 張 RDP 截圖](https://github.com/jason3e7/ai-101/blob/master/05-notes/htb/touch-2026-10-13/touch-with-goal.md)
- [三十篇標題與素材對照 (本 repo)](https://github.com/jason3e7/ai-101/blob/master/05-notes/ironman/titles.md)
