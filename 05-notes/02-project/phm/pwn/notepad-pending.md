# PHM #75 Pwn — notepad (已完整逆向, 卡在 glibc 2.23 heap exploit 本機測不了)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 連線 | `nc ctf.hackme.quest 7713` |
| 檔案 | [`/static/notepad`](https://ctf.hackme.quest/static/notepad) (ELF 32-bit, not stripped) |
| 保護 | No PIE, canary on, NX, Partial RELRO |
| 狀態 | **未解** — 結構/函式全逆向; 利用需 glibc 2.23 heap fengshui, 本機 glibc 2.35 測不了 |

## 選單

頂層 `a> bash / b> cmd / c> notepad / d> exit`。**`bash`/`cmd` 是誘餌** (只 `fgets`+`printf("bash: %s: command not found", input)`, 不真的執行; format 是固定字串, 非 FSB)。

`notepad` 子選單: `a New / b Open / c Delete / d Set readonly / e Keep the secret` (jump table @ `0x80494f0`)。

## note struct (`notepad_new`, `malloc(size+0x10)`)

| 偏移 | 欄位 |
|---:|:---|
| `+0x0` | **show 函式指標** = `notepad_show` (0x80489f7) |
| `+0x4` | **destroy 函式指標** = `notepad_destory` (0x8048a17, `memset(data,0,size)`) |
| `+0x8` | flag (1=可編輯; `readonly` 設 0) |
| `+0xc` | size (使用者輸入, `0 < size <= 0x400`) |
| `+0x10` | data; `fgets(data, size, stdin)` |

slot table @ `0x804b080` (16 格), `notepad_find_slot` / `notepad_choose` 皆限 index 0..15 (有界)。

## 各操作

- **new**: find_slot → readint(size, 1..0x400) → `malloc(size+0x10)` → 填 fptr/flag/size → `fgets(data,size)`。
- **open**: choose → 若 `note[8]!=0` 問 edit, yes 則 `fgets(stackbuf,0x1000)` + `strncpy(note+0x10, stackbuf, size)` (**size-bounded, 不溢位 header**); 然後**子選單 call `note[0]` 或 `note[4]`** (`show note`/`destory note`) → `call eax`(0x8048ce8), arg=note。
- **delete**: `free(note); slot=0` (NULL 掉 slot → 無 slot-UAF)。
- **readonly**: `note[8]=0`。
- **keepsec**: `note[0]=notepad_forbidden` (0x80489de)。

## 已確認的點 / 可疑面

- **函式指標 note[0]/note[4] 可經 open 子選單呼叫** → 若能蓋掉就是任意 call (本題的 win 面)。
- **leak primitive**: open 的 edit 用 `strncpy(,,size)` **不保證 null 結尾**; 填滿 size bytes 後 `show` 的 `printf("%s", data)` 會讀過界 → 洩漏相鄰 heap。
- `hash_data` (0x8048e5e, 自製 hash) 位址被 `.data` @ ~`0x80424e4` 引用一次, 但沒有直接 caller — 可能是某 keepsec/加密表或死碼, **待確認是否有隱藏路徑**。

## 還沒拼出的關鍵

從主要路徑看**沒有明顯的 header overflow / double-free / slot-UAF** (edit 受 size 限、delete NULL 掉 slot)。要蓋 `note[0]` 多半得靠 **heap fengshui**: 利用 free 後的 chunk 重用 + 不同 size 造成 overlap, 或 data 內嵌 fake note + 某個 off-by-something 讓 slot/指標落進 data。需要更深一輪分析確認觸發點。

## 卡點 (同 mailer)

- 伺服器是 **glibc 2.23**; 本機只有 snap 的 **glibc 2.35**, fastbin/unsorted 行為與檢查差很多 → heap 利用**本機測不準**, 遠端又只能盲打。
- 站上有 [`libc-2.23.so.i386`](https://ctf.hackme.quest/static/libc-2.23.so.i386.xz) 可對 offset; 需要一個 2.23 的本機執行環境 (qemu-i386 + 2.23 sysroot) 才好開發。

## 下一步 (resume)

1. 架 glibc 2.23 (32-bit) 本機環境重現 heap 佈局。
2. 確認觸發點: 先用 strncpy-no-null 的 leak 漏 heap/libc, 再找 overlap 蓋 `note[0]` → `call eax` 導向 one-gadget/system。
3. 釐清 `hash_data` 是否為 keepsec 的隱藏加密路徑 (可能才是真正的 bug 入口)。

> 索引見 [README.md](../README.md)。
