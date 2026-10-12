# PHM #76 Pwn — petbook (未解, 已找到 UAF + libc leak)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 保護 | Non-PIE 64-bit / Canary ON / NX on / Partial RELRO |
| 狀態 | **UAF + libc leak 已拿到**, 卡在 write primitive (fastbin dup 的 fake chunk size check) |

## 題目

> `nc ctf.hackme.quest 7710`
>
> Binary: [`/static/petbook`](https://ctf.hackme.quest/static/petbook) (ELF 64-bit, non-PIE, 18KB).

### 介面

Register / Login 後能 Post / View Wall / Edit Post / Change Password / Adopt Pet / Rename / Abandon Pet.

### 結構

- **user struct 0x218**: `uid(4) | username(0x100) | password(0x100) | is_admin(4) | pet*(8) | posts*(8)`
- **post struct 0x110**: `uid(4) | title(0x100) | padding(4) | content_ptr(8)`
- **pet struct 0x18**: `uid(4) | name*(8) | species_str*("Dog") (8)`
- `uid = (magic & 0xFFFF0000) | n++`; 幾乎每個 handler 開頭都 `test (user->uid ^ magic), 0xFFFF0000` 做反 heap corruption 檢查

## 關鍵漏洞 — Bugs

### 1. `user_edit_post` 的 `realloc(ptr, 0)` UAF ← **已利用**

```asm
realloc(post[0x108], new_size)   ; new_size 來自 user, 可以傳 0
test rax, rax
je Realloc_failed                 ; 跳到印 "Realloc failed!"
mov [rbx+0x108], rax               ; ← 只有成功才更新 content_ptr
```

glibc 2.23 的 `realloc(ptr, 0)` 等同 `free(ptr); return NULL`. return NULL 走 je → `post[0x108]` **不更新**, 變 dangling pointer 指向 freed chunk.

### 2. `user_list_post` dump 272 bytes post struct 當 base64

```asm
mov rsi, 0x110
call base64_then_dump          ; 整個 post struct base64 dump
```

包含 `content_ptr` → 直接拿 heap address.

## 已拿到的 Primitive

### Libc leak (via %s on freed unsorted bin chunk)

```python
# content size 0x100 → chunk 0x110 (超過 fastbin max 0x80 → unsorted bin)
post(T1, 'A'*255)                 # post 1
post(T2, 'B'*255)                 # post 2 (防止 content_1 跟 top chunk 合併)
edit_post(id=2, size=0)           # realloc(content_1, 0) → free
view_wall()                        # printf("%s", content_1) 讀被 free 的 chunk
                                   # fd 欄位 = &main_arena.bins[0] - 0x10
# 實測 remote 吐出 6 bytes: 78 CB C2 31 87 7F → 0x00007F8731C2CB78
```

對 libc 2.23 x86_64 (PHM 的 `/static/libc-2.23.so.x86_64`):

- `__malloc_hook @ libc+0x3C3B10` → `main_arena = libc + 0x3C3B20`
- `fake_chunk @ main_arena + 0x58 = libc + 0x3C3B78`
- `libc_base = leak - 0x3C3B78 = 0x7F8731869000` ✓ page-aligned

### 可用 offset

| Symbol | Offset in libc 2.23 x86_64 |
|:---|---:|
| `system` | 0x45380 |
| `__free_hook` | 0x3C57A8 |
| `__malloc_hook` | 0x3C3B10 |

## 卡在哪裡 — Write Primitive

需要: 任意位址寫, 蓋 `__free_hook = system`, 然後 `realloc(content, 0)` 觸發 `free(content)` → `system(content)` → `system("/bin/sh")` → shell.

### 試過的路線

- **Fastbin dup**: content size 0x60 → chunk 0x70 → fastbin. 連 free 兩次 (間插 free 不同 chunk 繞雙 free 檢查), overwrite fd. 但 malloc 回 fake chunk 時 glibc 檢查 `chunksize(victim) == fastbin_sz`, 需要 fake chunk 附近 bytes 當 size.
- **`__malloc_hook - 0x23` 經典 trick**: libc 2.23 在 runtime 這個位置會有 0x7F byte, 當作 `size = 0x7F → masked = 0x70 → 符合 fastbin[0x70]`. 但 file 裡那段 bytes 都是 0, 需要 runtime 載完才有. 沒時間跑 gdb 驗證.
- **Unsorted bin attack 寫 `__free_hook`**: 寫入值是 `main_arena+0x58` libc 位址, 不是 system, 得搭配 one_gadget. one_gadget 偏移 (`0x45216` / `0x4526a` / `0xf02a4` / `0xf1147`) 都不等於 `main_arena+0x58` 的偏移, 直接寫沒用.

## 下一步 — Resume

1. **gdb 跑一遍 remote libc 看 runtime bytes near `__malloc_hook`**, 確認 fake chunk size trick 的 offset
2. 或改走 **House of Orange / FSOP** 覆蓋 `_IO_list_all` → 繞到 one_gadget
3. 或試更簡單: **unsorted bin attack 蓋 `global_max_fast` → 之後任意 size 都 fastbin**, 開第二階段任意 malloc

## Flag

未取得. UAF primitives + libc leak 全部 reproducible, 只差最後 write primitive.

## Sources

- [PHM scoreboard — petbook](https://ctf.hackme.quest/scoreboard/?id=76)
- `/static/petbook`, 共用 libc `/static/libc-2.23.so.x86_64.xz`
