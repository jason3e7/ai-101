# PHM Misc #9 — big (70 pts) · AC

[← 返回索引](../README.md)

## 題目

> It's a big file, read the flag.

檔案: [`/static/big.xxz`](https://ctf.hackme.quest/static/big.xxz)

## 解法

`.xxz` 不是標準副檔名. `file` 看出來是 **xz compressed data**, 直接 rename 跟解壓:

```bash
cp big.xxz big.xz
xz -d big.xz
file big   # XZ compressed data, checksum CRC64 (又一層)
cp big big2.xz
xz -d big2.xz
file big2  # ASCII text, 17179869207 bytes (16 GB!)
```

兩層 xz 解完變成 **16 GB 的純文字檔**. 開頭:

```
THISisNOTFLAG{}
THISisNOTFLAG{}
THISisNOTFLAG{}
...
```

整個檔塞滿 `THISisNOTFLAG{}` 行。真 flag 混在裡面某處, 用 grep 排除掉假的:

```bash
grep -aoP 'FLAG\{[^}]+\}' big2 | sort -u | grep -v THISisNOT
# FLAG{Really long file}
```

**Flag**: `FLAG{Really long file}`

## 收斂

- `.xxz` 是 xz 兩層打包的梗
- 題目「big」+ 17 GB 是暴力解法 (sparse file + 字串重複) 塞的
- 不用真的掃 16 GB, grep 吃一遍很快就吐出來
