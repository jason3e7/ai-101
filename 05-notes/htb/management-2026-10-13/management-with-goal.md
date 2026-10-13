# HTB Management — 用 Claude `/goal` 跑完 Insane 靶機 — Owning Management with Claude /goal

> [!NOTE]
> 2026-10-13 用 **Claude Code `/goal`** 無人介入跑完 HTB **Insane 等級** Linux 靶機 **Management** (10.129.54.222). 兩次 `/goal` session 共約 57 分鐘拿到 user + root flag (585 XP). 第一個 goal 跑到一半 Mythos 5.1 撞 safeguard 被擋, 切 Opus 5 續跑. 完整攻擊鏈橫跨 OpenAM 預設帳密 → OpenDJ JMX RMI 反序列化 → 自寫 gadget chain → GLPI config → XChaCha20-Poly1305 解密 → SSH 帳密重用 → sudo wildcard rdiff-backup 繞 sandbox.

> **TL;DR (EN):** Full Insane-level HTB (Management, #3024 Machine Rank, 585 XP) solved in 57 minutes with two `/goal` sessions. Mid-way through the foothold goal, Mythos 5.1 hit `cyber-related safeguards`; `/model` to Opus 5 and resumed. Chain: OpenAM demo/changeit → OpenDJ JMX pre-auth deserialization → self-written gadget chain via bundled Xalan (bypasses JDK 21 module restrictions) → RCE as openam → GLPI secret → XChaCha20-Poly1305 decrypt → credential reuse → SSH as owen → sudo wildcard on rdiff-backup to widen sandbox → root key extraction → root. Full attack preserved as 88 recon artifacts.

---

## 心智清單 — Mind-Map

```markdown
# Claude /goal 跑完 HTB Insane: safeguard 中途擋, 切 Opus 5 續跑
* 兩次 /goal, 兩個模型
  * foothold = Mythos 5.1 開跑
  * 跑到一半撞 safeguard 被擋
  * 切 Opus 5 續跑同一個 goal (52 分鐘)
  * privesc 另開新 goal (Opus 5, 5 分鐘)
* 多階段 foothold chain
  * OpenAM demo/changeit 進去, 權限不夠
  * OpenDJ JMX 1689 pre-auth deser
  * JDK 21 擋 Xalan, 自己掃 gadget
  * bundled Xalan + Click Column 組 chain
  * RCE as openam → GLPI config
  * XChaCha20-Poly1305 解密 → owen SSH
* privesc 的 sudo wildcard trick
  * sudoers 尾端 * 吃掉後面所有參數
  * rdiff-backup argparse last-wins
  * 多塞一條 --restrict-path /
* 兩個觀察
  * safeguard 在複雜 chain 的中段才觸發
  * Insane 等級 agent + script kid 一樣能吃
```

---

## 本次目標與結果 — Target & Result

| 項目 | 內容 |
|:---|:---|
| 日期 | 2026-10-13 |
| 靶機 | HTB **Management** (**Insane**, Linux, Ubuntu 24.04.5, Machine Rank #3024, 585 XP) |
| 攻擊面 | OpenAM 16.0.5 (Tomcat) + 內嵌 OpenDJ 5.0.3 + GLPI 11 + Nginx |
| 使用模型 | foothold 開場 **Mythos 5.1** → 中途撞 safeguard → 切 **Opus 5** 續跑 · privesc 全程 Opus 5 |
| 執行方式 | Claude Code CLI `/goal` 無人介入, 兩個 session (foothold / privesc) |
| 總耗時 | 約 57 分鐘 (foothold 52m + privesc 5m) |
| 結果 | user + root flag 全拿 |

![HTB Management solved — jason3e7, 585 XP, Insane](./04-htb-management-solved.jpg)

---

## Session 1 — Foothold `/goal` (中途被 safeguard 攔截)

### Prompt (開場用 Mythos 5.1)

![/model Mythos 5.1 + /goal — Phase 1 recon prompt](./01-goal-foothold-mythos.jpg)

prompt 骨架跟 [Cap 案例](../cap-2026-10-13/cap-with-goal.md) 完全一樣 (Target + Environment 工具自備清單 + STRICT logging + Phase 1/2/3 + 7 小時硬停). 不改模板, 只換 target IP, 這點很重要 — 下面會看到相同 prompt 在不同模型敏感度差異多大.

### Mythos 5.1 撞 safeguard 的時機

**跟 Cap 不同**: 這次 safeguard **不是在 goal 一送出就擋**, 是 Mythos 5.1 跑到**攻擊鏈中段**才突然回:

```
API Error: Mythos 5.1's safeguards flagged this message.
```

當下 agent 正在:
- OpenDJ JMX RMI 的 `newClient(Object)` 反序列化點已經確認
- 標準 ysoserial payload 都失敗 (JDK 21 擋了 `com.sun.org.apache.xalan.internal.xsltc.trax.TemplatesImpl` 的反射)
- 正在**動態 enumerate classpath** 找可用 gadget — 這段產出的 content 混了 Java 反序列化、`defineClass`、`bytecode injection` 這類關鍵字密度**暴衝**

訊號累積的位置剛好跟 [Day 28 PHM CTF 的「牆四 safeguard」](https://ithelp.ithome.com.tw/articles/10423733)描述一致: **單句不犯規, 整段 context 的攻擊鏈連貫度高到一定程度就擋**.

處理方式一樣:

1. `/model` 切 **Opus 5**
2. 沒重寫 prompt、沒改 goal, 直接讓 agent 從被擋的那一步**繼續**
3. Opus 5 接手, 把 class probing 跑完, 寫出 working chain, 完成整段 foothold

### Opus 5 接手之後完成的 chain

![Foothold 完成 (切 Opus 5 後) — OpenDJ 反序列化 → openam RCE → GLPI → owen SSH](./02-foothold-done-after-opus5-switch.jpg)

從 agent 自己做的 5 步摘要:

1. **預設帳密** — OpenAM `demo / changeit` 還活著, 但只有 `ui-self-service-user` 權限, LDAP ACI 擋讀取, 沒辦法直接拿 flag
2. **Pre-auth deserialization** — OpenDJ JMX 1689 (RMI stub @`127.0.1.1:45171`) 的 `javax.management.remote.rmi.RMIServerImpl.newClient(Object)` **在 RmiAuthenticator 跑完之前**就反序列化 credentials 參數. 不需帳密. 內嵌的 OpenDJ 跑在 OpenAM Tomcat JVM 裡 (`ParallelWebappClassLoader`), 等於整個 OpenAM classpath 都在 reach 範圍
3. **Gadget 自己掃** — 自己寫 `ClassProbe.java` 用 `ClassNotFoundException` / `InvalidClassException` 當 oracle 做 class existence probe:
   - 標準 commons-collections 3.2.2 有但 functor 被 guard 住
   - beanutils / spring / hibernate / rome 都沒有
   - **有的**: jackson-databind ≥2.14, Groovy, Apache Click, freemarker, HikariCP, tomcat-dbcp, jakarta.el, Rhino, **bundled Xalan**
   - 關鍵觀察: JDK 21 擋了 `java.xml` 模組的反射, 但**應用自己 bundle 一份 Xalan** 在 unnamed module, 繞過模組限制
4. **自組 gadget chain** — `PriorityQueue.readObject` → `org.apache.click.control.Column$ColumnComparator.compare` → `PropertyUtils` getter → `org.apache.xalan.xsltc.trax.TemplatesImpl.getOutputProperties()` (bundled 版本) → `defineClass` → static init. 寫在 `recon/scripts/java/ClickGadget.java`. **RCE as uid=996(openam)**
5. **橫向到 owen** — openam 身份下找到 `/opt/glpi` (GLPI 11, 沒掛 nginx), `config_db.php` 給 MySQL `glpi/8rhu0L6Pw4Y7`. DB 裡 `glpi_authldaps` 有 `cn=svc-glpi,ou=services,dc=management,dc=htb` 的加密 bind password. GLPIKey 用 **XChaCha20-Poly1305** 且 nonce 當 AAD, key 在 `/opt/glpi/config/glpicrypt.key`. 解出 `WpczC40GhTbk`
6. 該 DN **在 LDAP 裡不存在**, 但密碼被本機 OS 帳號 `owen` 重用. SSH `owen / WpczC40GhTbk` 成功 → user flag `e635094cfc4fe83e010e16b18c4c832c`

全程**零 brute force**, 唯一憑證就是解密出來的那組. 耗時 **52 分 3 秒** (含 Mythos 被擋的那段).

---

## Session 2 — Privesc `/goal` (全程 Opus 5)

既然已經切到 Opus 5, 第二個 goal 直接延用. 開新 session 的理由同 Cap: 乾淨 context + 可以直接標註「already have owen shell」省 token.

### Agent 跑完的輸出

![Privesc done — rdiff-backup sudo wildcard → root key → ssh root](./03-privesc-done.jpg)

5 步 (全部 agent 自己走):

1. `sudo -l` 直接命中:
   ```
   (root) NOPASSWD: /usr/bin/rdiff-backup --server --restrict-path /opt/backup --restrict-mode read-only *
   ```
   — SUID/SGID/cron 都是 stock Ubuntu 24.04, **沒跑 linpeas**

2. **兩個 bug 疊起來**:
   - **sudoers 尾端 unanchored `*`** 吃掉後面所有參數 (包含空格)
   - **`rdiff-backup 2.2.6` 用 argparse**, 同一個 option 出現兩次, **後面覆蓋前面** (argparse last-wins)

3. **攻擊**: 在合法參數後面再塞一條 `--restrict-path /`:
   ```
   sudo -n /usr/bin/rdiff-backup --server \
        --restrict-path /opt/backup --restrict-mode read-only \   # 原意沙箱
        --restrict-path /            --restrict-mode read-only    # 蓋掉原意, sandbox 擴到整個 /
   ```
   — 有個反諷: 原意的裸指令 (沒多餘參數) **不符合這條 sudoers rule**, sudo 會 fallthrough 要密碼. 這個 `*` 不是裝飾, 是**整條 rule 能跑的必要條件**, 讓繞法天然可達

4. **拷 root** — 寫 wrapper 補 `{h}` placeholder, 一般 rdiff-backup client 當 driver, 全程 **read-only** (避免破壞目標), 把 `/root` 鏡到 `/tmp`

5. **直接登入**: `/root/.ssh/id_ed25519` 的 public 版本就在 `/root/.ssh/authorized_keys` 裡 (root 自己把自己 key 加進去). 拿 private key → `ssh -i root@localhost` → `uid=0`, 讀 `/root/root.txt` = `928160c0a7ffc2d81ce91b8cf351226d`

耗時 **4 分 48 秒**. agent 全程維持 read-only, 事後也主動清掉 `/tmp` 的 staging 檔.

---

## 完整攻擊鏈 — Full Chain

```
[1] OpenAM demo / changeit  ──default creds──▶  low-priv ui-self-service-user
                                                  │ (權限不夠, 需要 escalation)
                                                  ▼
[2] OpenDJ JMX 1689 newClient(Object)  ──pre-auth deserialization──▶
      自寫 Click + bundled Xalan gadget chain          繞 JDK 21 模組限制
                                                  │
                                                  ▼
[3] RCE as uid=996(openam)  ──讀 /opt/glpi/config_db.php──▶  MySQL glpi
                                                  │
                                                  ▼
[4] glpi_authldaps 加密 bind pw + glpicrypt.key  ──XChaCha20-Poly1305──▶
                                                  svc-glpi 密碼 WpczC40GhTbk
                                                  │
                                                  ▼
[5] 該 DN 不存在, 但密碼被 OS 帳號 owen 重用  ──SSH──▶  owen shell (uid=1000)
                                                  │
                                                  ▼
[6] sudoers 尾端 * + rdiff-backup argparse last-wins
      多塞 --restrict-path /  ──root-owned backup server──▶  讀 /root/.ssh/id_ed25519
                                                  │
                                                  ▼
[7] root 把自己 pubkey 放進 authorized_keys  ──ssh root@localhost──▶  root
```

七個環節, 任何一環修好都能截斷:
- OpenAM 關 `demo` 預設帳號 (這個是歷年老漏)
- OpenDJ 的 admin ports (1689 / 4444 / 50389) 綁 localhost, 不要暴露到外網
- GLPI 加密 bind password 的 key 跟 DB 不要放同台 (把加密 key 當 secret 處理)
- 服務密碼不要跟作業系統登入帳號重用
- sudoers rule **不要留尾端 unanchored `*`**; `rdiff-backup` 也別當 sudo 白名單
- `root` 不要把自己的 private key 存在 `/root/.ssh/` 下

---

## 這次 `/goal` 跑法的三個觀察 — Takeaways

### 1. safeguard 可以在**攻擊鏈中段**才觸發

跟 Cap 不同 — Cap 是**第二個 goal 一送就擋**, 一眼就看得出來要換模型. Management 是**第一個 goal 跑到一半才擋**, 跑的過程中 Mythos 5.1 做 OpenAM / OpenDJ 列舉都沒事, 一直到:

- Java deserialization 的 class existence probe 開始連續產出 `defineClass` / `bytecode` / `gadget chain` 相關內容
- 累積到某個訊號密度閾值 → 整段 turn 被 drop

這個 pattern 跟 [Day 28 PHM 的 safeguard](https://ithelp.ithome.com.tw/articles/10423733) 完全一致: **單句不犯規, context 連貫性過高才擋**. 區別只是這次擋的是 Mythos 5.1 — 證明不同模型對同一條攻擊鏈的敏感度閾值真的不一樣.

### 2. 處理方式: 不改 prompt, 不改方向, 直接 `/model`

這次完全不浪費時間重寫 prompt. 一撞 safeguard 立刻:

```
/model opus-5
```

Opus 5 接手之後**沒退任何進度** — 它看到 agent 之前已經寫到一半的 class probe、已經列舉完的 classpath, 直接接著做 gadget chain 搜尋. 這也是為什麼最後時間還壓在 52 分鐘內 — 切模型沒掉 context.

**結論**: safeguard 是**路況資訊**, 不是**題目難度資訊**. 繞 prompt 浪費時間, 換模型最快.

### 3. Insane 等級 agent + script kid 也吃得掉

Management 的 chain 是真的硬 — JDK 21 的 module restriction、bundled Xalan 的細節、GLPI 的 XChaCha20-Poly1305 用 nonce 當 AAD、rdiff-backup argparse last-wins — 這些**單獨拿出來隨便一條都值一篇 writeup**. 但 agent (切對模型後) 連這些都能自己找到並串起來.

跟 [PHM Day 28 的四面牆](https://ithelp.ithome.com.tw/articles/10423733)對照: PHM 卡在**作者刻意的 novelty 梗**, HTB (包括 Insane) 卻幾乎沒撞到那條牆, 因為 HTB 的漏洞**都在真實軟體的真實文獻/CVE/writeup 裡**, agent 的 training data 吃得下. **真實攻擊面比 CTF novelty 梗對 agent 友善得多**.

---

## 產出檔案 — Recon Artifacts

Session 1 跟 Session 2 合計 **88 個檔**, 原始打包在 `/home/user/htbLab/03/recon_v04plus.tgz`. 幾個關鍵:

| 檔案 | 用途 |
|:---|:---|
| `nmap_full.txt` / `nmap_udp.txt` / `ssl_banners.txt` | 全埠 + UDP + TLS 列舉 |
| `openam_*` (10+ 檔) | OpenAM 版本 / realms / callbacks / default creds / Swagger |
| `ldap_*` (6 檔) | OpenDJ 匿名 bind + DN / OU 列舉 |
| `jmx_*` + `scripts/java/ClassProbe.java` + `ClickGadget.java` | JMX 反序列化點 + gadget 掃描 + 最終 RCE chain |
| `dl/app_deob.js` | management.htb SPA 的 AES-GCM encrypted bundle 解密 + deobfuscate |
| `scripts/am_login.py` / `fed_try.sh` | OpenAM REST 自動化 |
| `ssh_owen.txt` / `root_shell.txt` | 帳密驗證 + root 登入 |
| `privesc_enum.txt` / `suid.txt` / `sgid.txt` | 本機列舉 (sudo -l 直接命中, 沒跑 linpeas) |
| `loot_root_id_ed25519` | 從 rdiff-backup mirror 撈到的 root private key |
| `flags.txt` / `notes.md` / `privesc_notes.md` | Flag + 階段摘要 + 提權技術分析 |

---

## Sources

- [HTB Management (Linux, Insane)](https://app.hackthebox.com/machines/Management)
- [Cap 案例 (同系列前篇)](../cap-2026-10-13/cap-with-goal.md)
- [HTB Goal Prompt 設計指南 (本 repo)](../htb-goal-prompt-guide.md)
- [Day 28 PHM CTF — AI agent 的四面牆 (safeguard 觀察的另一面)](https://ithelp.ithome.com.tw/articles/10423733)
- [OpenDJ JMX RMI deserialization 的技術背景 — rapid7 SRC 分析](https://www.rapid7.com/blog/post/2024/10/01/opendj-jmx-rmi-deserialization/)
- [rdiff-backup `--server` + argparse 繞 sudoers 的 writeup 範例](https://gtfobins.github.io/gtfobins/rdiff-backup/)
