# PHM #68 Pwn — onepunch (未解, 已深入分析)

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 160 |
| 保護 | **No PIE** / Canary ON / NX on / Partial RELRO (GOT 可寫) |
| 狀態 | 卡在「1-byte write 沒 leak 不夠繞 ASLR」 |

## 題目

> `nc ctf.hackme.quest 7718`
>
> Binary: [`/static/onepunch`](https://ctf.hackme.quest/static/onepunch) (ELF 64-bit, non-PIE)

## 題目行為

```c
int main() {
    unsigned long long addr; int byte;
    setbuf(stdout, 0);
    printf("Where What?");
    scanf("%llx %d", &addr, &byte);
    if (scanf_ret == 2) {
        *(char*)addr = (char)byte;     // ← 唯一的 1-byte 任意寫
        if (byte == 0xff) puts("No flag for you");
    }
    return 0;
}
```

**一條線**: scanf 讀 `addr byte` → 寫一個 byte 到任意地址 → 檢查 byte == 0xff → 印 troll 字串 → main return → process exit.

## 關鍵觀察 — Key Finding

### `_()` 函數在 init_array, mprotect 把 .text 變 RWX

```asm
0x4006c6 <_>:
    push rbp; mov rbp, rsp
    lea rdi, [rip+0]           ; rdi = 0x4006d1
    and rdi, 0xfffffffffffff000 ; rdi = 0x400000 (page start)
    mov rsi, 0x1000             ; 4096
    mov rdx, 0x7                ; PROT_READ|WRITE|EXEC
    mov rax, 0xa                ; SYS_mprotect
    syscall                      ; mprotect(0x400000, 0x1000, 7)
```

`.init_array` 塞了這個 `_()` 當 index 1 的 entry → **startup 時就把整張 0x400000-0x401000 轉成 RWX**. gdb 確認:

```
0x400000 - 0x401000  rwxp  /path/onepunch
```

所以 `.text`, `.plt`, `.plt.got`, `.rodata` **全部都可以寫**. 題目名「onepunch」字面上「一拳搞定」, 想定是用一個 byte 寫在這個 RWX 區域裡 trigger shell.

## 卡點 — Where I'm Stuck

### 無 leak 不能繞 libc ASLR

libc 2.23 x86_64 (題目有給). 要跳 `system`, `execve`, one_gadget 等 libc 位址都需要知道 libc base. **程式沒有任何 leak primitive** — scanf 只讀, printf 格式寫死, puts 字串寫死.

### 1-byte 不夠做任何完整 primitive

試過的方向:

1. **GOT[puts] 低字節改寫 → 跳到附近函數**
   - GOT[puts] 初始 = 0x400566 (lazy trampoline). 改低字節能跳到 `_init`, `*@plt`, `_start` 等 0x4005XX 區間
   - **問題**: puts 只在 `byte == 0xff` 時被呼叫, 但我們寫的 byte 就是 0xff, 導致 GOT[puts] 低字節 = 0xff → 跳到 0x4005ff (隨機 bytes, SEGV)
   - 想用 `byte=0xd0` 跳 `_start` (會重入 main 無限迴圈), 但 byte=0xd0 不觸發 puts, GOT 改了沒用

2. **改 `cmp eax, 0xff` 的 imm32**
   - 把 0x400763 從 0xff 改成 0 → `cmp eax, 0` → byte==0 時 puts 被呼叫. 試過 ✓ 跑成功, 但只是 call libc puts("No flag for you") 走正常路線, 不是 shell
   - 無法同時改 cmp **跟** GOT[puts] (需要 2 次寫)

3. **puts 的 256-byte 視窗內有 `call execl` at libc+0x6f503**
   - 問題: puts 被呼叫時 rdi = 0x400828 ("No flag for you"), 走 execl rdi 是 path → "No flag for you" 不是有效 path
   - .rodata 字串在 RWX 範圍可寫, 但 1 byte 不夠改成 "/bin/sh" 之類

4. **改 `_()` syscall number 從 mprotect (0xa) → execve (0x3b)**
   - 問題: `_()` 只在 startup 被呼叫一次, 寫入發生在 main 之後, 來不及影響 startup
   - 要再觸發 `_()` 要再一次 GOT 改寫 → 2 次寫, 做不到

5. **ret2dlresolve 改 reloc index / symbol name**
   - 需要改 `.rela.plt` 或 `.dynstr` 的多 byte, 1 byte 不夠改到有效 libc symbol (e.g., "puts" → "system")

6. **ret2csu / loop main**
   - 讓 main 迴圈需要改 `ret` 或 `je` offset + 補一個 jmp back → 至少 2 bytes
   - 單 byte 的改法 (如 ret → jmp) 都落到不實用的 target

### Memory 不跨連線保留

每次 nc 連線都是新 process, ASLR 重 random, 1-byte 寫入不跨連線累積. 不能分多次連線慢慢 build state.

## 未試過的方向 — Untried

- **FSOP (File Stream Oriented Programming)**: 改 1 byte 的 `stdout` pointer (在 0x601058, bss COPY reloc), 把它偏到 libc 附近的假 FILE struct. 需要對 libc 2.23 的 `_IO_file_jumps` 很熟
- **`__exit_funcs` 鏈**: exit handler 有 PTR_MANGLE 混淆, 繞不出來
- **特定 one_gadget**: 本題 libc 的 one_gadget 位址都跟我們 GOT 函數不在同一個 256-byte 範圍

## Flag

未取得.

## Sources

- [PHM scoreboard — onepunch](https://ctf.hackme.quest/scoreboard/?id=68)
- `/static/onepunch`, `/static/libc-2.23.so.x86_64.xz`
