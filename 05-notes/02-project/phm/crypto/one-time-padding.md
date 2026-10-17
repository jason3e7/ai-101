# PHM #88 Crypto — one-time padding

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 60 |

## 題目

> 一個 web 端點每次呼叫會回傳一段 hex, 內容是 `flag` XOR 一組隨機 pad.

抓幾次會看到每次輸出都不同, 看似 OTP (完全不可破).

## 分析

### Pad 永遠不是 0 這件事

多抓幾次之後, 把每個 position 的所有 output byte 收集起來, 列成一個 set:

```python
seen[i] = {o[i] for o in outputs}
```

如果 pad 真的是 **uniform random 0-255**, 當樣本數夠大, `seen[i]` 應該會把 `0..255` 全部看過一次, 這樣 flag[i] 就無從知道 (任何 byte 都可能).

但實際跑的時候發現: **每個 position 永遠少一個 byte**. 推測作者產 pad 的範圍是 **1-255 (排除 0)**, 導致 flag XOR 0 的那個結果不會出現.

→ 所以 `missing = (set(range(256)) - seen[i]).pop()` 就是 flag 的第 `i` 個 byte!

### 需要幾個樣本?

單個 byte 需要幾乎所有的 1-255 都出現過一次, 用 coupon collector 近似估計:

- 255 值, E[N] ≈ 255 × H(255) ≈ 1580 次

為了讓每個 position 都收斂, 保險值 2000+ 個樣本. 我用 **並發 fetch 收 8000 個**, 確定沒有 position 殘留兩個以上 candidate.

## 解法

```python
import asyncio, aiohttp, re

URL = "https://ctf.hackme.quest/one-time-padding/"

async def fetch(sess):
    async with sess.get(URL) as r:
        text = await r.text()
        m = re.search(r'[0-9a-f]{20,}', text)
        return bytes.fromhex(m.group(0))

async def main():
    async with aiohttp.ClientSession() as sess:
        samples = await asyncio.gather(*[fetch(sess) for _ in range(8000)])
    L = len(samples[0])
    flag = bytearray(L)
    for i in range(L):
        seen = {s[i] for s in samples}
        missing = set(range(256)) - seen
        if len(missing) == 1:
            flag[i] = missing.pop()
        else:
            print(f"pos {i}: {len(missing)} candidates {missing}")
            flag[i] = 0x3f  # '?' placeholder
    print(flag.decode(errors='replace'))

asyncio.run(main())
```

跑完得到:

```
FLAG{otp is most secure way to encrypt data....?!}
```

## Flag

```
FLAG{otp is most secure way to encrypt data....?!}
```

## 感想

OTP 的「不可破解」只在 pad **完全均勻**才成立. 作者故意用 `random.randint(1, 255)` 排除 0, pad 分布就不均勻, 整個安全性就斷在這一點. 這題教訓很乾淨: 看到 OTP 不要直接放棄, 先確認 pad 分布.
