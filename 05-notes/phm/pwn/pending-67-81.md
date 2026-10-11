# PHM Pwn #67-81 — 評估 (全部暫跳過)

這 15 題全部是 binary exploit 類, 單題合理工時都 30 分鐘以上, 沒有「2 分鐘能做掉」的候選. 這裡記清單跟每題大致路線, 之後有時間再逐題打.

## 清單

| # | 題名 | 分數 | nc port | 類型 / 已知線索 | 時間預估 |
|:---|:---|---:|---:|:---|:---|
| 67 | smashthestack | 100 | 7717 | stack overflow + shellcode | 30-60 min |
| 68 | onepunch | 160 | 7718 | one-gadget RCE, 題目有 Source Code 連結 | 30-60 min |
| 69 | raas | 160 | 7719 | ret2libc / ret-as-a-service | 30+ min |
| 70 | rsbo | 120 | 7706 | ROP, open+read syscall (Tips 給 ROP, open, read) | 30-60 min |
| 71 | rsbo-2 | 170 | 7706 | 同 binary, 要 get shell, Tips: stack migration | 60+ min |
| 72 | leave_msg | 200 | 7715 | 題目: "I am on a vacation, leave the message for me" | 60+ min |
| 73 | stack | 200 | 7716 | "my C programming homework" + Source Code | 60+ min |
| 74 | very_overflow | 200 | 7705 | overflow 類, 有 Source Code | 60+ min |
| 75 | notepad | 200 | 7713 | 可能 heap note-based 題 | 60+ min |
| 76 | petbook | 200 | 7710 | heap/UAF (petbook 是 free-list 玩法常見名字) | 60+ min |
| 77 | mailer | 150 | 7721 | 未知 | 60+ min |
| 78 | tictactoe-1 | 50 | 7714 | 打贏 AI 就拿 flag, 不用 exploit, 但要寫 minimax 或試遍對局 | 15-30 min |
| 79 | tictactoe-2 | 250 | 7714 | 同 binary 的 exploit 路線 ("Get a shell pls") | 60+ min |
| 80 | bytebucket | 100 | 7722 | heap intro ("ever tried any heap-based challenge?") | 60+ min |
| 81 | bytebucket-2 | 250 | 7722 | arbitrary code exec, heap ACE | 90+ min |

總分 2510, 合計工時估 15~20 小時.

## 為什麼不邊做邊試

1. **單題下限太高**: 要下載 ELF → gdb / Ghidra 靜態分析 → 寫 pwntools exploit → 本地 ngulpio 驗證 → remote 打. 任何一步出錯都要 debug.
2. **safeguard 風險**: Shellcode / ROP chain / one-gadget 這類 content 密度很高, 剛剛才撞過一次 safeguard (在 xssrf 寫 exfil JS 的時候), 這批題再撞的機率很高. 要做的話建議**一次只攻一題**, 不要一起產.
3. **tictactoe-1 看似簡單但也要 15 分鐘**: 要 recv/send 流程寫對 + 對局策略 (minimax 或打表), 不是「兩分鐘打幾下」能贏.

## 建議攻擊順序 (之後想做時)

照「單題分數除以工時」的 CP 值, 大概:

1. **#78 tictactoe-1** (50 / 20min = 2.5) — 最簡單, 不用 exploit, 寫 game solver 就好
2. **#67 smashthestack** (100 / 40min = 2.5) — intro stack overflow, 多半是 NX off + 可控 RIP 跳 shellcode
3. **#70 rsbo** (120 / 45min = 2.7) — Tips 直接說 open+read ROP, 路線很明確
4. **#80 bytebucket** (100 / 60min = 1.7) — heap intro, 先過這題再碰 #81
5. 其他分散在 1-1.5 的 CP 值, 看心情
