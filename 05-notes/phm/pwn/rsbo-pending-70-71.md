# PHM Pwn #70 rsbo / #71 rsbo-2 · 分析完成, 卡在 PRNG 預測

| 項目 | 值 |
|:---|:---|
| 題目 | #70 rsbo (120) / #71 rsbo-2 (170, stack migration 拿 shell), 同一支 binary |
| 連線 | `nc ctf.hackme.quest 7706` |
| 檔案 | [`/static/rsbo-01c51ca9a7b9db3d69289c6dbb1cd758`](https://ctf.hackme.quest/static/rsbo-01c51ca9a7b9db3d69289c6dbb1cd758) |
| 保護 | 32-bit, NX, **No PIE**, No canary, dynamic |
| Tips | ROP, open, read |
| 狀態 | **未解** — 機制全逆向、ROP 料齊、echo 置換 oracle 已驗證; 卡在「預測下一條連線的洗牌」(flag-seeded PRNG) |

> rsbo = **R**andom **S**huffle **B**uffer **O**verflow。

## 機制 (逆向)

`main`:
1. `read_80_bytes` → 其實 `read(0, buf, 0x80)` 讀 **128 bytes** 進 stack buffer (`esp+0x10`), 回傳實讀長度 `len`。
2. **Fisher–Yates 逐位元組洗牌**: `for i in 0..len-1: j=rand()%(i+1); swap(buf[i],buf[j])`。
3. `write(1, buf, len)` 把**洗牌後**的 buffer 回吐 (→ 這就是 permutation 洩漏 oracle)。
4. `leave; ret`。buffer→ret 位移 ≈ **112 (0x70)**(由 `and esp,0xf0; sub esp,0x70` 推估, **待遠端確認**)。ret 落在洗牌範圍內, 所以 ROP chain 的 bytes 一定會被打亂。

`init` (`0x804858d`) — **亂數種子**:
- `open("/home/ctf/flag")`, `read` 前 **16 bytes of the flag** 進 buf。
- `seed = time(0)`; `for i in 0..15: seed = (seed*0x539 + flag[i]) mod (2^31-1)`; `memset(buf,0,16)`; `srand(seed)`。
- ⇒ **種子 = `(A*time + C) mod M`**, 其中 `M=2147483647`(=2³¹−1)、`A=0x539^16 mod M`、`C=Σ flag[i]·0x539^(15-i) mod M` 是**跟 flag 綁死的常數**(每次連線都一樣)。

所以洗牌的 permutation 依賴我們不知道的 flag → 無法直接預測; 但種子隨 time 做**仿射**變化。

## 已驗證: echo 洩漏 permutation

送 `ABCDEFGHIJ` → 回 `DCHFGAIJBE`(本次連線種子下的置換)。證實可從回吐還原本次連線的 `j_i = rand()%(i+1)` 序列。

## 完整利用路線 (intended)

1. **校準連線**: 送已知 128 bytes, 由回吐還原 permutation → 還原 glibc `rand()` 的輸出/狀態 → 回推 `seed1`。
2. **仿射預測**: `C = (seed1 - A*t1) mod M`; 下一條連線時間 `t2` → `seed2 = (A*t2 + C) mod M`。(flag/C 是常數, 只要校準一次。)
3. **逆洗牌**: 用 `seed2` 本機模擬 Fisher–Yates 得 permutation `P`; 把輸入排成「洗牌後剛好等於目標 ROP chain」。
4. ROP (NX, 用現成 PLT 做 open→read→write 印 flag):

```
open@plt  = 0x8048420   read@plt = 0x80483e0   write@plt = 0x8048450
"/home/ctf/flag" = 0x80487d0   .bss = 0x804a040
pop3 (pop esi;pop edi;pop ebp;ret) = 0x804879d   ; 清 cdecl 3 參數
chain (接在 ret 之後, offset≈112):
  open  ; pop3 ; 0x80487d0 ; 0 ; <pad>          # open("/home/ctf/flag",0) -> fd=3
  read  ; pop3 ; 3 ; 0x804a040 ; 0x40           # read(3, bss, 0x40)
  write ; pop3 ; 1 ; 0x804a040 ; 0x40           # write(1, bss, 0x40)
```

#71 rsbo-2: 同 binary 但要 **get shell**(Tips: stack migration)— 因 128 bytes 不夠放完整 ret2libc, 需 pivot 到 bss 再做第二段 (leak libc → system)。

## 卡點 / 風險

- **glibc `rand()` 狀態還原**: 從 `j_i mod (i+1)` 還原完整狀態 (TYPE_3 加法回饋, `o[i]=o[i-3]+o[i-31]`) 要寫一段不短的還原碼; 低位元攻擊可用 `i+1` 為 2 冪的那幾步取低位。
- **無法本機測**: 這支是 32-bit dynamic, 開發機沒有 32-bit loader/libc → ROP 與 ret 位移只能在遠端試。
- **ret 位移 112 待確認**(alignment 推估)。

## 下一步 (resume)

1. 校準連線還原 seed1 → 驗證仿射關係 (`A=pow(0x539,16,M)`) 在兩條連線上成立。
2. 寫 glibc rand 模擬 + Fisher–Yates 逆置換。
3. 套上面的 open/read/write ROP, 遠端調 ret 位移。

> 索引見 [pending-67-81.md](pending-67-81.md)。
