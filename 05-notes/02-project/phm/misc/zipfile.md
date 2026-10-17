# PHM Misc #14 — zipfile (100 pts) · 卡住未解

[← 返回索引](../README.md)

## 題目

> Unzip this file

檔案: [`/static/zipfile.zip`](https://ctf.hackme.quest/static/zipfile.zip)

## 現況: 結構拆到底了, 但 XOR 組合沒解出 FLAG{...}

### 外層結構

從 `zipfile.zip` 開始, **999 層單一 entry 的遞迴 zip** (每層一個檔案也叫 `zipfile.zip`, 大小逐層減少 ~37 bytes). 到第 1000 層, 內層 zip 出現 **552 個 entries**, 每個 entry 的 filename 是:

```
\r\x7fELF\n0/0/0/0/.../bit/bit/bit\xffPK
```

- 前綴 `\r\x7fELF\n` = ELF magic + 換行 (5 bytes)
- 中間 `0/0/.../bit` = 1022 bits (每 bit 用 `0/` 或 `1/` 編碼)
- 結尾 `\xff` + `PK` (ZIP sentinel)

filename 長度一律 2056 bytes, data 長度 251-408 bytes 不等.

### 552 entries 的內部結構

每個 entry 的 data 又是一個 zip. 繼續遞迴單 entry 到某深度後, 又出現 multi-entry 層, 這層的 **每個 entry 都叫 `X/O/R/_/T/H/E/S/E/_/F/I/L/E/S`** — 明示提示「XOR THESE FILES」.

這些 inner entries 的特性:
- 每個 top entry 的 inner entries 數 N = 3-8 (差異分布)
- 每個 top entry 的 N 個 inner data **完全相同** (D)
- 不同 top entry 的 (N, D) 都不同
- D 長度 1-3 bytes

### 用 filename bits 做分組

filename 的最後 14 bits 算成 integer, 共 **92 個 unique values (0-91)**. 每個 position (val) 剛好有 6 個 entries, 其中 2 種不同的 (N, D) pair, 各出現 3 次.

### XOR 嘗試 (全部失敗)

- 全部 552 entries data 直接 XOR → 看不到 FLAG
- 遞迴到 inner 層 XOR N 份 (奇數 N 保留 D, 偶數 N = 0) → 看不到 FLAG
- 每個 position 的 2 個 D 直接 XOR → 得到 227 bytes 的 binary blob, 看不到 FLAG
- 每個 position 的 2 個 D × N 複製 XOR → 跟上一個相同
- 以 size progression 的 diff mod 256 當 bytes → random
- filename 的 bit 流當 bytes (big-endian / little-endian) → 全 0 + 尾巴幾個 bit

### 最接近「有意義」的輸出

`XOR 兩個 D per position` (concat 92 個 position):

```
e\x15\x94T\xe4\x00\xc3\xa3\xb6\xd6\x0f\xd8i\xcb\x1a\x0b\xb0\xcdn...
```

開頭有 `e`, 中間有 `T`, 其他是 noise. **沒有找到 `FLAG{...}` pattern**.

## 可能方向 (未試)

- 92 個 position 的 2 個 D 可能不是直接 XOR, 而是**用 `X/O/R/_/T/H/E/S/E/_/F/I/L/E/S` 這 15 bytes 當 key 做 stream XOR**
- filename bits 真正解碼成 bytes 的方式可能不是 `0/0/0/...1` → bit, 而是 run-length encoding
- `\r\x7fELF\n` 可能是**完整 ELF binary header 的開頭**, 整個 filename 要還原成 ELF binary 執行拿 flag
- Position 排序可能是 bit-reverse 或是 **gray code**
- `XOR_THESE_FILES` 當 key XOR 552 entries 的 data

## 收斂

結構拆到底 (999 層 → 552 entries → multi-copy inner data), 看懂 "XOR_THESE_FILES" 的暗示, 但實際的 XOR 組合試了多種都不是 flag. 可能解碼 scheme 跟我想的差一步. 先存著, 之後再回來.
