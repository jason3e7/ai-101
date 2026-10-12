# PHM #77 Pwn — mailer (已逆向, 卡在 House of Force 需伺服器端盲打)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 150 |
| 連線 | `nc ctf.hackme.quest 7721` |
| 檔案 | [`/static/mailer`](https://ctf.hackme.quest/static/mailer) (ELF 32-bit, not stripped) |
| Tips | how2heap by shellphish |
| 保護 | No PIE, canary on, Partial RELRO, **GNU_STACK=RWE** |
| 狀態 | **未解** — 結構全逆向、漏洞與技術確定 (House of Force), 但本機 glibc 太新測不了、伺服器 glibc 2.23 只能盲打 |

## 程式結構

選單: 0 Exit / 1 Write mail / 2 Dump mails。

**mail struct** (`new_mail(size)` = `malloc(size+0x48)`):
- `+0x00` next (單向串列, `write_mail` 做 `m->next = head; head = m` 前插)
- `+0x04` Title (`gets`, **無界**)
- `+0x44` size (int)
- `+0x48` Content (`gets`, **無界**)

`service` 開場在 **BSS 建一個固定 mail @ `0x804a080`** (memcpy 72 bytes + 12 bytes), `head @ 0x804a110` 初值指向它 (串列尾, next=0)。

**write_mail**: `size=readint()`; `m=new_mail(size)`; `gets(m+4)`; `gets(m+0x48)`; 前插。
→ 兩個無界 `gets` = **heap overflow**; `size` 由使用者輸入。

**dump_mail**: 從 head 走訪, 每個 mail: `printf("%s", m+4)` 印 Title; **`fwrite(m+0x48, 1, m[0x44], stdout)`** 印 Content `size` bytes → **size 可控 = 任意長度 leak primitive**。

## 漏洞

1. `gets(m+4)` 無界 → 可蓋到自己的 `size`(m+0x44)、Content(m+0x48), 以及**往高位**蓋 top chunk / 之後配置的 chunk。
2. `dump` 的 `fwrite(m+0x48, size)` → 蓋大 `size` 就能往高位 dump。

## 為什麼難 (需 House of Force)

- **沒有 `free()`** → 不能 UAF / fastbin / tcache。只有「overflow + 可控 malloc size」→ 典型 **House of Force**: 蓋 top chunk size = `0xffffffff`, 再 `malloc(target - top - 8)` 把 top 移到任意位址, 下一個 mail 就配置在 target。
- **forward-only overflow**: `gets` 只能往高位蓋。GOT (`0x804a00c`-`0x804a064`, 含 `stdin/stdout` 的 libc 指標) 與固定 mail 都在 heap **下方**, 無法直接 dump → **libc leak 一定要靠 HoF** 先把 chunk 配到低位 BSS/GOT 再 dump。
- heap leak 可行 (dump 大 size → 露出某 mail 的 `next` = heap 位址 → 回推 heap base, HoF distance 要用)。

## 完整路線 (intended)

1. **heap base leak**: 建 2 封 mail, 蓋第一封 size 大 → dump 露出第二封的 `next`(heap 位址)。
2. **House of Force**: 蓋 top size=0xffffffff → `malloc` 一個巨大 size 把 top 移到 `0x804a060-ish` → 下一封 mail 落在 BSS 低位。
3. **libc leak**: 讓那封 mail 的 `fwrite` 讀 `0x804a060/64` (stdin/stdout FILE* = libc) → 算 libc base (伺服器是 **glibc 2.23**, 站上有 [`libc-2.23.so.i386`](https://ctf.hackme.quest/static/libc-2.23.so.i386.xz) 可對 offset)。
4. **拿 shell**: HoF 再配一封到 GOT, `gets` 寫 `__malloc_hook`/`__free_hook`(2.23 有) 或某 GOT = `system`/one-gadget; 或因 `GNU_STACK=RWE`, HoF 到 stack 寫 ret→shellcode。

## 卡點

- **本機測不了 HoF**: 開發機只有 snap 的 **glibc 2.35** (32-bit), top chunk size 有完整性檢查 → HoF 直接被擋, 無法本機驗證。伺服器 glibc 2.23 才吃得下, 但只能遠端盲打 (無 gdb/無法迭代), distance 算錯就 crash。
- 需要正確的 2.23 malloc 行為 (top chunk 相對 heap base 的 offset、對齊) 才能算 HoF distance; 盲打成功率低, 要多次連線試。

## 下一步 (resume)

- 取一份 32-bit glibc 2.23 + ld, 用 `qemu-i386` 或對應 loader 在本機重現 HoF, 把 distance/offset 都算準再打遠端。
- 或先只做 step 1-3 (leak), 確認 HoF distance 公式, 再接 step 4。

> 索引見 [README.md](../README.md)。
