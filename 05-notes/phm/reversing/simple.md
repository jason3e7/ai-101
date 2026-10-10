# PHM #42 Reversing — simple

| 項目 | 值 |
|:---|:---|
| 類別 | Reversing |
| 分數 | 90 |

## 題目

> A little bit harder
>
> 檔案: [`/static/simple-rev`](https://ctf.hackme.quest/static/simple-rev)
> ELF 32-bit, dynamically linked, debug info, not stripped.

## 分析

`objdump -M intel -d simple-rev` 看 main:

```asm
printf("What is flag? ")
fgets(buf, 0x3f, stdin)
; 把 '\n' 換 '\0'
; for i: output[i] = input[i] + 1    ; 每個 byte +1
strcmp(output, "UIJT.JT.ZPVS.GMBH")
if ok: printf("FLAG{%s}\n", input)
else: puts("Try hard.")
```

每個 byte +1 比對字串 `UIJT.JT.ZPVS.GMBH`. 反轉就是**每個 byte -1 = Caesar -1**:

```
U(0x55) → T(0x54) H I S
.(0x2E) → -(0x2D)
...
```

得 `THIS-IS-YOUR-FLAG`.

## 解法

```bash
echo "THIS-IS-YOUR-FLAG" | ./simple-rev
# -> What is flag? FLAG{THIS-IS-YOUR-FLAG}
```

## Flag

```
FLAG{THIS-IS-YOUR-FLAG}
```
