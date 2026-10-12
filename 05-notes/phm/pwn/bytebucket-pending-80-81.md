# PHM Pwn #80 bytebucket / #81 bytebucket-2 · 逆向完成、利用卡在 heap leak

| 項目 | 值 |
|:---|:---|
| 題目 | #80 bytebucket (100) / #81 bytebucket-2 (250), 同一支 binary |
| 連線 | `nc ctf.hackme.quest 7722` |
| 檔案 | [`/static/bytebucket`](https://ctf.hackme.quest/static/bytebucket) |
| 保護 | 64-bit, **PIE**, NX, **Full RELRO**(GOT 不可寫), No canary, stripped |
| 狀態 | **未解** — 已完整逆向, 推出完整利用鏈, 但卡在「需要一次 heap leak」(難度等同 #81 ACE) |

> [!NOTE]
> #80「第一面旗」= 洩漏 `FLAG1` 環境變數的 heap 複本; #81「第二面旗」= 拿 shell 讀 `/home/ctf/flag`。兩者共用同一支 binary / 同一組原語。

## 程式結構 (逆向)

8 選單的 bucket 管理器, bucket 是 heap 上的鏈結串列。**bucket chunk = `malloc((size+4)*8)`**:

| 偏移 | 欄位 |
|---:|:---|
| `+0x00` | `next` (下一個 bucket) |
| `+0x08` | `size` (slot 數) |
| `+0x10` | **name (內嵌, 最多 16 bytes, `strcpy` 進來)** ← 不是指標! |
| `+0x20 + i*8` | slot i (指向 content chunk) |

啟動時 (`main` @ `0x1a6e`):

- `make(2, "/root/locked")` → **ROOT** (全域 `*0x203148`)。`ROOT.slot0 = 0x203010`(sentinel/locked 標記)、`ROOT.slot1 = flag1 複本`。
- `steal_flag1` (`0x19e4`): `malloc(0x40)` → `getenv("FLAG1")` → `strncpy` 進去 → **`memset` 把原環境字串清 0**(所以讀 `/proc/self/environ` 沒用) → 存進 `ROOT.slot1`。
- `make(1, "/home/ctf/flag")` 掛在 ROOT 下當 child, 設為 current。它的 slot0 = `"Get a shell for second flag"`。

選單 handler: Make `0x1455` / List `0x15e5` / Find `0x1602` / Next `0x174d` / Drop `0x176a` / Open `0x1828` / Sort `0x19b1`。Open 子選單: Show(印所有 in-range slot 當 `%s`) / Edit / Drop data / Rename。

## 關鍵障礙

- **flag(#80) 在 `ROOT.slot1`**, 但唯一的讀取 (Show) 在 Open 子選單裡, 而 **Open 會 `strstr(name,"/locked")` 把 ROOT 鎖住** → 打不開就 Show 不了。
- name 是**內嵌**的, 不能像改指標那樣轉走。
- `cba` 內容讀取 = `read(0, dest, size)` 剛好讀 size、realloc 到 size → **沒有線性 heap overflow**。
- Full RELRO → **不能改 GOT**。

## 唯一的 bug: 有號數越界索引 (Edit / Drop data)

兩個 handler 讀行號後只用**有號數上界**檢查 (`cmp idx,size; jl ok`) → **負索引通過**, `*(cur + (idx+4)*8)` 越界:

- **Edit(idx,newsize,content)**: `slot = *(cur+(idx+4)*8)`; 把 content 寫到 `*slot`(newsize bytes)。= 任意寫, 但目標是「某個 heap qword 所指的位址」。
- **Drop(idx)**: `slot = *(cur+(idx+4)*8)`; `slot==sentinel` 跳過, 否則 **`free(slot)`**; 然後把該 OOB qword 歸零。= 任意 free + 定點歸零 (但 free 在歸零**之前**, 所以若 `*slot` 非法會先 crash)。

## 實測佈局 (gdb, 關 ASLR, PIE base `0x555555400000`)

```
ROOT @ ...6042a0  name="/root/locked"  slot0=sentinel(0x203010)  slot1=...6042e0("FLAGTEST..")
/home/ctf/flag bucket (current) @ ...604330  size=1
佈局: [name 字串chunk][ROOT 0x40][flag複本 0x50][/home/ctf/flag bucket 0x30]
```

各 chunk 大小 (`malloc(0x40)→0x50`、`make(2)→0x40`、`make(1)→0x30`) 在 glibc 2.23–2.31 一致 → **相對位移跨版本穩定**。

## 推導出的完整利用鏈 (只差 heap leak)

1. **leak heap base** ← **就是卡住的這步**。
2. Edit 把 `p64(ROOT+0x10)` 寫進我控制的某個 content chunk。
3. Edit 用 `idx' = (那個 chunk 位址 − (cur+0x20))/8`, 讓 `slot == ROOT+0x10`; 寫 11 個非 `/` 字元蓋掉 `/root/locked`(newsize 11 < strlen 12 → 走 direct-write 不 realloc) → **解鎖**。
4. Next(無 next → reset to root) → current = ROOT → Open(已解鎖) → Show → **Row[1] = flag1 複本**。

> 直接對 ROOT name 做 Drop 歸零行不通: `*(ROOT+0x10/0x18)` 是 name 的 ASCII bytes(非法指標) → `free()` 先 crash。要寫 `ROOT+0x10` 需要「記憶體存在一個值 == `ROOT+0x10` 的 qword」當 slot, 所以一定要先知道 `ROOT+0x10` 的實際位址 → 回到 heap leak。

## 為什麼 leak 這步 = #81 等級

slots 被 Drop 一律歸零、沒有乾淨的 UAF/overflow, 要生出 leak 得做 **tcache 錯位/chunk 重疊**(free flag chunk 只能保住 offset 16 之後的 bytes, 前 16 被 fd/key 蓋掉, 不足以還原完整 flag)。這套 overlap 技巧做出來, 其實 #81 的任意寫/ACE 也差不多到手。所以這題名義 100 分, 實際 leak 工程量 ≈ 250 分那題。

## 下一步 (resume 時)

- 做 tcache dup / chunk overlap 取得 heap leak (需在遠端反覆試, 相依伺服器 glibc 版本; 先 `strings libc` 或由行為判版本)。
- 拿到 leak 後, 上面第 2–4 步已驗證可行 (佈局/idx 關係都量過)。
- 本機可直接跑 (64-bit, 設 `FLAG1=...` 即可用 gdb 開發)。binary 下載在開發機 `/tmp`。

> 索引總表見 [pending-67-81.md](pending-67-81.md)。
