# PHM #66 Pwn — echo3 · 卡住未解

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 200 |
| 連線 | `nc ctf.hackme.quest 7720` |
| 檔案 | [`/static/echo3`](https://ctf.hackme.quest/static/echo3) (ELF 32-bit, not stripped) |
| 保護 | NX, No PIE, **canary** |
| 狀態 | **未解** (已分析, 卡在 stack pivot) |

## 題目結構

`main`: `open("/dev/urandom")` → `read` 8 bytes 到 stack、4 bytes 到 global `0x804a060` → 依隨機值做一段 alloca → `call hardfmt`。

`hardfmt`: 把 `*0x804a060` 複製到一塊 alloca buffer; 迴圈 **5 次** `read(0, 0x804a080, 0x1000)` → `printf(0x804a080)` (**格式字串漏洞**); 迴圈後比較 buffer 與 `*0x804a060`, 不論相等與否最後都 `exit()`。

## 為什麼難 (三個同時成立)

1. **輸入不在 stack 上**: 讀進固定 global `0x804a080`, 但 `printf` 的 vararg 是從 **stack** 取 → 無法把「要寫入的目標位址」放進自己的輸入, `%n` 只能透過「stack 上既有的指標」去寫。
2. **沒有 `system`**: PLT 只有 `open/read/puts/close/printf/...` → 必須自己 ROP `open(flag)/read/puts` 把 flag 印出來。
3. **全程沒有 `ret` 會被執行**: `hardfmt` 永遠 `exit()`, `main` 也因此拿不回控制 → 只能靠蓋 **GOT** (printf/exit/puts) 改某次間接呼叫的目標。

## 卡點: 缺 esp pivot

在 `printf(0x804a080)` 當下 `eax = 0x804a080` (前一行 `mov eax,0x804a080; push eax`)。本來最漂亮是把 `printf@got` 蓋成 `xchg eax,esp ; ret` → `esp` 直接落到我們完全控制的 global (把 ROP chain 放那), 但 binary 裡 **沒有** `xchg eax,esp ; ret` / `mov esp,eax ; ret` / `pop esp ; ret` 這類 gadget。

有的只有 `leave ; ret` (0x8048535)、`pop ebp ; ret` (0x80487fb)、`add esp,0x10 ; leave ; ret` 等, 都依賴 `ebp`, 無法把 `esp` 直接設到固定 global。

## 下一步 (未驗證方向)

正統解應是「無 stack 控制的格式字串」標準套路: 用 stack 上既有的 **ebp/stack 指標鏈** (probe 看到 `%18$p` 是指向 stack 的指標、`%19$p` 是回到 main 的返回位址 0x804877b), 分多輪 `%n` **先改某個指標的低位元組讓它指向目標 slot, 再透過它寫入** → 把一條 `open("flag")→read→puts` 的 ROP chain 寫到 stack 上 main 的 frame, 最後把 `exit@got` 蓋成 `leave ; ret` 讓 `esp` 落到那條 chain。步驟多且要盲打, 這輪先存著。

> 其餘已解: [echo (#64)](echo.md)、[echo2 (#65)](echo2.md)。
