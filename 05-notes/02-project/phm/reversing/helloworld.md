# PHM #41 Reversing — helloworld

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 40 |

## 題目

> Guess a number please :D
>
> 檔案: [`/static/helloworld`](https://ctf.hackme.quest/static/helloworld)
> ELF 32-bit, dynamically linked, with debug info.

## 分析

### 執行環境準備 — 32-bit libc

Ubuntu 預設沒裝 32-bit glibc, 跑 32-bit ELF 會 `cannot execute: required file not found`:

```bash
sudo apt-get install -y libc6-i386
# 之後 ./helloworld 就能跑
```

### 看反組譯 main (`objdump -M intel -d helloworld`)

```asm
mov DWORD PTR [ebp-0x2d], 0xc881e8f1     ; 塞 29 bytes 到 stack buffer
mov DWORD PTR [ebp-0x29], 0xcecf81d2
mov DWORD PTR [ebp-0x25], 0x81c081d5
...
mov BYTE PTR [ebp-0x11], 0x0              ; null terminator
push "What is magic number? "
call printf
...
scanf("%d", &num)
cmp eax, 0x12b9b0a1                        ; magic number check
jne .wrong                                 ; 錯 → "Try Hard."
```

正確分支做 **XOR 迴圈**:

```c
for (int i = 0; buf[i]; i++) buf[i] ^= (num & 0xff);
printf("Flag is FLAG{%s}\n", buf);
```

每個 byte XOR **input 的低 8 bits**.

### 兩個關鍵數字

1. **Magic number** = `0x12b9b0a1` = **314159265** 十進位. π × 10^8 ≈ 3.14159265... 這就是「pi 彩蛋」.
2. **XOR key** = low byte of magic = `0xa1`.

### 直接跑驗證

```bash
echo 314159265 | ./helloworld
# → What is magic number? Flag is FLAG{PI is not a rational number.}
```

(29-byte buffer XOR 0xa1 剛好拼出 `PI is not a rational number.` + null.)

## Flag

```
FLAG{PI is not a rational number.}
```

## 感想

40 分熱身題, 考:

1. ELF 32-bit 準備環境 (`apt install libc6-i386`)
2. objdump 讀 main 的 cmp + XOR loop
3. 把 magic number 轉十進位當 scanf 吃
4. 彩蛋 (`314159265 = π × 10^8`)

Reversing 入門, 完全不用 angr / radare2 / IDA, `objdump` + `printf "%d" 0x...` 就夠.
