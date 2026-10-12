# PHM #52 Reversing — mov (已實測可執行, 尚未解出 flag)

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 200 |
| 檔案 | [`/static/mov.xz`](https://ctf.hackme.quest/static/mov.xz) → 解壓 5.9MB ELF 32-bit |
| 狀態 | **已下載 + 跑起來 + 逆向結構**; flag 未取得 (卡在 demovfuscator 依賴太重 / 需深度動態逆向) |

## 題目

> `MOV` instruction is turing complete! — 用 [movfuscator](https://github.com/xoreaxeaxeax/movfuscator) (Chris Domas) 編譯, 幾乎整支只有 `mov`。

## 怎麼在這台機器跑起來 (重點!)

主機沒有 32-bit libc/loader、沒 root、沒 qemu, 但 **snap 目錄裡有 32-bit glibc**, 用顯式 loader 就能直接執行:

```bash
LD=/snap/core22/2955/usr/lib/i386-linux-gnu/ld-linux.so.2
LP=/snap/core22/2955/usr/lib/i386-linux-gnu
echo "AAAA" | "$LD" --library-path "$LP" ./mov
# Input flag: Bad flag
```

gdb 也可以 (記得 `handle SIGSEGV nostop noprint pass`, 因為 movfuscator 用 SIGSEGV 當跳轉):

```bash
gdb -q --args "$LD" --library-path "$LP" ./mov
```

## 逆向到的結構

- **M/o/Vfuscator2 的 SIGSEGV-trampoline 變體**: 進入點 `0x804824c`, 開頭兩個 `sigaction` 安裝 **SIGSEGV(11)+SIGILL(4)** handler (= 分支/結束的 trampoline)。
- **真正邏輯只有 ~18KB** (`.text` = `0x804824c`, size `0x47f1`); 那 5.9MB 幾乎都是 `.data` 的 MOV-ALU 查表 (xor/add/cmp 全查表)。
- I/O: `read`/`write` 走 PLT (非 `int 0x80`)。`read(0, buf, 128)` 讀 flag, buf **@ `0x85f6c44`**。
- 無 flag 明文、無可讀 immediate (比對常數都編進查表索引)。

## 動態觀察

- 輸入會**就地 XOR 轉換**再比對: `buf[0]` 讀入 `0x41`('A') → 之後變 `0xc6` (`0x41 ^ 0x87`)。推測每位元組 XOR 一個 (可能逐位置不同的) 常數後跟 cipher 比。
- 分支/旁路: branchless, 輸出只有全有全無的 Good/Bad, **沒有 timing / fault-count 旁路** (strace 看不到 rt_sigreturn, 幾乎直線執行到結尾才 fault 結束)。
- 工作緩衝區跑到結尾會被複用 (直接 dump buf 看不到 transform 後的完整結果, 要在比對當下攔)。

## 為什麼還沒解

1. **demovfuscator** (最正統) 依賴 **capstone + keystone + z3 + openssl** 四個函式庫, 沒 root 要全部原始碼編 (z3 特別久) → 這環境成本太高。
2. **純動態逆向可行但工作量大**: 要追到決定 Good/Bad 的累加器:
   - 若累加器是「逐位元組**加總** diff」→ 可用 gdb 當 per-byte oracle, 固定其他位元組、掃某位元組 256 值取最小 → 爬出 flag。
   - 若是 **OR** 則要更細 trace, 從 transformed buffer 反推 cipher 再反 XOR。

## 下一步 (resume)

- gdb: 攔「結果 write」前的比對點, 找累加器變數位址; 判斷加總 or OR。
- 若加總: 寫 per-byte 爬山 (每次一個 gdb run 讀累加器)。
- 或在有 `qemu-user` / 已編好 `demovfuscator` 的環境再開。

> binary + gdb 腳本留在開發機 `/tmp/claude-1000/phm52/` (會隨 session 清掉, 重抓用上面的 `/static/mov.xz`)。
