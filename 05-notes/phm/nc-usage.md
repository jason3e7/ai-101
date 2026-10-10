# PHM — nc 用法與踩坑

[← 回索引](README.md)

> [!NOTE]
> PHM 的 Pwn / Misc / Web 題不少是 `nc host port` 連線互動. 這份記錄常用模式跟我踩過的坑, 下次照用.

**TL;DR (EN):** Common `nc` idioms for CTF challenges with countdown prompts, interactive shells, and stdin pipe timing — plus the exact fixes for issues I actually hit.

---

## 心智清單 — Mind-Map

```markdown
# PHM nc 連線手冊
* 基本連線
  * nc host port (互動)
  * timeout 外包 (nc 不會自己退)
* 送 input
  * echo cmd | nc (單指令)
  * (sleep N; echo cmd) | nc (等 server prompt)
  * printf 多行 (保留換行)
* 接收 output
  * head / tail / grep 篩
  * tee file 存檔同步看
* 踩坑
  * exit 143 = SIGTERM (timeout 殺)
  * 空 stdin < /dev/null (server 立刻關)
  * countdown 要先 sleep 才能送
```

---

## 基本連線 — Baseline

### 單純互動 (手動 debug)

```bash
nc ctf.hackme.quest 7709
```

純手動輸入, 退出按 `Ctrl+C` 或 `Ctrl+D` (EOF). debug 時用, script 不用這個.

### 配 timeout, 不然會卡

```bash
timeout 15 nc ctf.hackme.quest 7709
```

PHM 的 server 有時連完 **不會自己斷**, 需要外面包 `timeout <秒>` 強制切斷. **一律加 timeout**.

---

## 送 input 的三種模式

### 模式 1 — 單一指令, 連完立刻送

```bash
echo "cat flag" | timeout 10 nc ctf.hackme.quest 7709
```

- server 不吐任何 prompt, 直接等 input 的題用這個
- `echo` 自動加 `\n`

### 模式 2 — 有 countdown / banner, 等再送

PHM Pwn #58 catflag 就是這個模式:

```
plz capture the flag after 5 seconds...
plz capture the flag after 4 seconds...
...
```

這時直接 `echo cmd | nc` 會 **太早送**, server 還在倒數就被你 input 塞爆. 要先 `sleep`:

```bash
(sleep 7; echo "cat flag") | timeout 15 nc ctf.hackme.quest 7709
```

- `sleep 7` > countdown 時間 (5s + 2s 緩衝)
- `()` 包起來讓 subshell 的 stdout pipe 給 nc

### 模式 3 — 多行互動 (menu / 多階段)

```bash
(
  sleep 3; echo "1"              # 選 menu 1
  sleep 1; echo "payload here"
  sleep 1; echo "exit"
) | timeout 20 nc ctf.hackme.quest PORT
```

每行間 `sleep 1` 給 server 反應時間. 複雜的用 `pwntools` 的 `remote()` + `recvuntil()` 比較穩, 但簡單互動這樣就夠.

---

## 踩坑 — 我真的遇到的

### 坑 1 — `exit code 143` (SIGTERM)

```bash
timeout 15 bash -c 'echo "cat flag" | nc ctf.hackme.quest 7709'
# → exit 143, 沒看到任何輸出
```

**原因**: `bash -c '...'` 的 subshell 在 `timeout` 到期時被 `SIGTERM` 整個殺掉, 連 nc 的 output 都還沒 flush 到父 shell.

**修法**: 不要包 `bash -c`, 直接讓 `timeout` 作用於 nc:

```bash
echo "cat flag" | timeout 15 nc ctf.hackme.quest 7709
```

### 坑 2 — `< /dev/null` server 立刻 EOF

```bash
timeout 10 nc -v -w 5 ctf.hackme.quest 7709 < /dev/null
```

**行為**: server 看到 stdin 關掉 (EOF), 可能直接結束連線. 看到 countdown 跑完就斷, 沒機會送指令.

**修法**: 用 `(sleep N; echo cmd)` 的 subshell, stdin 一直開著直到送完指令.

### 坑 3 — `nc -w` 的行為不一致

- Ubuntu 的 `nc` (`netcat-openbsd`): `-w` 是 timeout for connection + idle
- `ncat` (nmap 版): `-w` 只是 connect timeout
- `netcat-traditional`: 根本沒 `-w`

**修法**: 不依賴 `-w`, 一律用外面的 `timeout` 控制.

### 坑 4 — 看不到 server 吐的 byte-by-byte 輸出

某些 server 一個字母一個字母慢慢吐 (`stdbuf`-less), pipe 到 shell 時被 buffered.

**修法**: 用 `stdbuf -o0` 或 `script -qc`:

```bash
stdbuf -o0 nc ctf.hackme.quest PORT | tee out.log
```

或改用 pwntools, `remote()` 不會 buffer.

### 坑 5 — binary payload (shellcode / 格式字串)

`echo` 會多加 `\n`, 遇到 `-n` flag 不一定好用. 用 `printf` 或 `python3`:

```bash
# printf
printf '\x90\x90\x90...' | timeout 10 nc host port

# python3 更可控
python3 -c 'import sys; sys.stdout.buffer.write(b"\x90"*8 + b"\xaa\xbb\xcc\xdd")' | timeout 10 nc host port
```

### 坑 6 — stdin 送完 nc 就斷, 看不到 server 的回應

```bash
echo "payload" | nc host port
# payload 送完 → stdin EOF → nc 認為 TCP 連線 half-close → server 關連線
```

**修法**: 送完後保持 stdin 開著:

```bash
(echo "payload"; sleep 5) | timeout 10 nc host port
```

或是用 `cat` 把 terminal stdin 再接過去:

```bash
(echo "payload"; cat) | timeout 10 nc host port
```

---

## 進階 — pwntools 比較穩

對於 pwn 題有 recv/send 時序要求的, 直接上 pwntools:

```bash
pip3 install --break-system-packages pwntools
```

```python
from pwn import *
r = remote('ctf.hackme.quest', 7709)
r.recvuntil(b'after 1 seconds...\n')  # 等 countdown
r.sendline(b'cat flag')
print(r.recvall(timeout=5).decode())
```

優點: `recvuntil` 等 server 出現特定字串才送, 不用猜 sleep 幾秒.

---

## 總結 — 最常用的 snippet

```bash
# 90% 的 PHM nc 題直接套這個
(sleep 7; echo "payload") | timeout 15 nc ctf.hackme.quest PORT
```

- **sleep 時間** 看 server banner 多長 (多半 5-10 秒)
- **timeout 時間** 比 sleep 多 5-10 秒緩衝
- 看到 `exit 143` → 檢查有沒有 `bash -c` wrapper
- 看到沒輸出 → 檢查 stdin 有沒有 `< /dev/null`
- 二進位 payload → 換 `printf` 或 `python3 sys.stdout.buffer`
