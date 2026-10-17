# PHM #7 Misc — slow · 卡住未解

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 70 |

## 題目

> OMG, It's slow.
>
> `nc ctf.hackme.quest 7708`

連進去 server 回:

```
Tips: flag has no lowercase or space, and should match this regex: FLAG\{[0-9A-Z_]+\}
What is your flag?
```

送任何字串進去 → 等 N 秒 → `Bye`.

## 分析

### Side channel: 時間差

有 timing attack. 不同送入字串回應時間差異:

- 送 `"A"` (完全不符格式) → 1.25 秒
- 送 `"FLAG{A}"` (前 5 字 `FLAG{` 對) → 6.25 秒
- 送 `"FLAG{2}"` → 7.34 秒 (6 字對)

公式: **1.25 + N × 1 秒**, N = **從左 match 的字數**. 每對一字 sleep 1 秒.

### Char-by-char timing attack

對當前已知 prefix, 平行試 37 個 candidate (`[A-Z0-9_]`), 選回應**顯著較慢** (`+1s`) 的那個. 一路推:

```
FLAG{ → 2    (7.3s vs 6.3s baseline)
FLAG{2 → _   (8.3s vs 7.3s)
FLAG{2_ → S  (9.3s vs 8.3s)
... (每位 +1s 乾淨)
FLAG{2_SLOW_I → _
FLAG{2_SLOW_I_ → B
```

結果到 `FLAG{2_SLOW_I_B` (15 字).

### 死路: Server 把 sleep 卡在 15 秒

送 `FLAG{2_SLOW_I_B<任意>` 都是 **15.2-15.3 秒**, 怎麼多字對都沒超過. 推測 server:

```python
for i, ch in enumerate(user):
    if i >= 15 or ch != flag[i]: break
    time.sleep(1)
```

或 server 有 wrapper timeout 15s. 15s cap 一到就返 `Bye`, timing side-channel **徹底失效**.

多次 20-trials 平均都無法在 noise 之上辨出 pos 15+ 的正確字.

### 試過的猜測 (全部 `Bye`)

英文語感 "too slow I b..." 的 50+ 種延續, 包含 Accel World 梗跟一般 slang:

```
FLAG{2_SLOW_I_BURST_LINK}      FLAG{2_SLOW_I_BURST}
FLAG{2_SLOW_I_BRAIN_BURST}     FLAG{2_SLOW_I_BURST_LINKER}
FLAG{2_SLOW_I_BAILED}          FLAG{2_SLOW_I_BOOSTED}
FLAG{2_SLOW_I_BE_FAST}         FLAG{2_SLOW_I_BE_FASTER}
FLAG{2_SLOW_I_BLAME_U}         FLAG{2_SLOW_I_BLAME_SRV}
FLAG{2_SLOW_I_BLAZING}         FLAG{2_SLOW_I_BLAZE_IT}
FLAG{2_SLOW_I_B4}              FLAG{2_SLOW_I_B4_U}
FLAG{2_SLOW_I_B4D}             FLAG{2_SLOW_I_BORED}
... + 很多 B 開頭 word × suffix 組合 (~950 candidates)
```

## 收斂

Timing attack 把前 15 字拆出來, server cap 讓後面全盲. 剩下得靠**猜 flag 的文字意圖**, 但窮舉空間太大 (37^N for N 剩餘字). 70 pts 題但只 111 人解, 應該是真有別的 side channel 或某個特別典故我沒 get 到.

**已知前綴: `FLAG{2_SLOW_I_B`**. 文字直讀「too slow, I b...」, 繼續走就是個 Fermi 題: 後面要配 `}` 收尾, 典型長度 20-30 字. 之後找到別的線索再回來.

## 可能方向 (未試)

- 用 **parallel connections** 搞 bursty send 讓 server race condition 洩漏更多字
- 猜一波跟作者 Inndy 其他 PHM 題有關的梗 (Accel World 已試了)
- 看作者 blog / Twitter 歷史題解, 找這題的 writeup 風格
