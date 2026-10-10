# Please Hack Me CTF — Index

[← 回主頁](../../index.md)

[Please Hack Me](https://ctf.hackme.quest/) (PHM, 作者 [Inndy](https://www.inndy.tw/)) 的解題紀錄。老 CTF 練習站, 共 **101 題** 分 8 類 (Misc / Web / Pwn / Reversing / Crypto / Forensic / Programming / Lucky), 題目設計於 2016–2021 期間.

每題一個 `.md` writeup, 按類別分子資料夾.

> [!TIP]
> 連線操作共用: [nc-usage.md](nc-usage.md) — PHM 的 Pwn / Misc 題幾乎都 `nc host port`, 常用模式跟我踩過的坑都寫在那.

## 解題進度 — Progress

| 類別 | 總題數 | 已解 | 分數 |
|:---|---:|---:|---:|
| Misc | 14 | 7 | 370 |
| Web | 26 | 2 | 30 |
| Pwn | 24 | 1 | 10 |
| Reversing | 17 | 1 | 40 |
| Crypto | 16 | 12 | 1000 |
| Forensic | 2 | 2 | 120 |
| Programming | 1 | 1 | 40 |
| Lucky | 1 | 1 | 110 |
| **合計** | **101** | **27** | **1720** |

## 已解題目 — Solved

| 類別 | 題名 | 分數 | Writeup |
|:---|:---|---:|:---|
| Misc | flag | 10 | [misc/flag.md](misc/flag.md) |
| Misc | corgi can fly | 50 | [misc/corgi-can-fly.md](misc/corgi-can-fly.md) |
| Misc | television | 50 | [misc/television.md](misc/television.md) |
| Misc | encoder | 50 | [misc/encoder.md](misc/encoder.md) |
| Misc | pusheen.txt | 40 | [misc/pusheen-txt.md](misc/pusheen-txt.md) |
| Misc | big | 70 | [misc/big.md](misc/big.md) |
| Misc | drvtry vpfr | 100 | [misc/drvtry-vpfr.md](misc/drvtry-vpfr.md) |
| Crypto | easy | 10 | [crypto/easy.md](crypto/easy.md) |
| Crypto | r u kidding | 20 | [crypto/r-u-kidding.md](crypto/r-u-kidding.md) |
| Crypto | not hard | 50 | [crypto/not-hard.md](crypto/not-hard.md) |
| Crypto | classic cipher 1 | 50 | [crypto/classic-cipher-1.md](crypto/classic-cipher-1.md) |
| Crypto | classic cipher 2 | 50 | [crypto/classic-cipher-2.md](crypto/classic-cipher-2.md) |
| Crypto | easy AES | 60 | [crypto/easy-aes.md](crypto/easy-aes.md) |
| Crypto | one time padding | 60 | [crypto/one-time-padding.md](crypto/one-time-padding.md) |
| Crypto | shuffle | 70 | [crypto/shuffle.md](crypto/shuffle.md) |
| Crypto | xor | 90 | [crypto/xor.md](crypto/xor.md) |
| Crypto | emoji | 120 | [crypto/emoji.md](crypto/emoji.md) |
| Crypto | multilayer | 150 | [crypto/multilayer.md](crypto/multilayer.md) |
| Crypto | ffa | 270 | [crypto/ffa.md](crypto/ffa.md) |
| Pwn | catflag | 10 | [pwn/catflag.md](pwn/catflag.md) |
| Web | hide and seek | 10 | [web/hide-and-seek.md](web/hide-and-seek.md) |
| Web | homepage | 20 | [web/homepage.md](web/homepage.md) |
| Reversing | helloworld | 40 | [reversing/helloworld.md](reversing/helloworld.md) |
| Programming | fast | 40 | [programming/fast.md](programming/fast.md) |
| Lucky | you-guess | 110 | [lucky/you-guess.md](lucky/you-guess.md) |
| Forensic | easy pdf | 40 | [forensic/easy-pdf.md](forensic/easy-pdf.md) |
| Forensic | this is a pen | 80 | [forensic/this-is-a-pen.md](forensic/this-is-a-pen.md) |

## 卡住未解 — Pending

| 類別 | 題名 | 分數 | Writeup |
|:---|:---|---:|:---|
| Misc | meow | 50 | [misc/meow.md](misc/meow.md) — ZIP in PNG, 加密 ZIP 密碼未找到 (rockyou 全掃 + Pusheen 相關猜測都 miss) |
| Misc | where is flag | 50 | [misc/where-is-flag.md](misc/where-is-flag.md) — 665K 文字塞滿假 FLAG{...}, 已過濾 noise 篩出 3 個 candidate 但 server 都 reject, 真 flag 可能不是 FLAG{...} 格式或有未發現的 regex 線索 |
| Misc | otaku | 90 | [misc/otaku.md](misc/otaku.md) — 4 張動漫角色圖, 3 張找到 troll flag (FAKE/F14G/OOPS), Miku 應該藏真 flag 但 LSB/bit plane/zsteg 都沒找到 |
| Misc | buzzing | 100 | [misc/buzzing.md](misc/buzzing.md) — BMP header 說謊 (真實 640×400 + BI_BITFIELDS 0x41 mask), 看到作者 overlay 訊息但不含 FLAG{}, 真 flag 位置不明 |
| Misc | BZBZ | 50 | [misc/bzbz.md](misc/bzbz.md) — 山寨 bilibili 登入頁, `login.php` 無條件 alert「You must be a employee from bilibili!」, 試遍 credentials / headers / cookies / IP / 洩漏 appkey 都沒 bypass |
| Misc | zipfile | 100 | [misc/zipfile.md](misc/zipfile.md) — 999 層單 entry 遞迴 + 1000 層 552 entries, 每個 entry filename 編 ELF+1022 bits, inner data 多份複製暗示「XOR THESE FILES」, 試了多種 XOR 組合但沒解出 FLAG |
| Crypto | slowcipher | 200 | [crypto/slowcipher.md](crypto/slowcipher.md) — 逆向 ELF 做 LCG + 密碼混合, 寫 fast C 版 (log-N closed-form) 把每 byte 的 N 次 LCG 壓到 O(log N), rockyou 1M + anime/Accel World 關鍵字都沒 hit, password 還沒猜中 |

## 送 flag 流程 — Flag Submission

PHM 的 flag 送到 scoreboard 統一表單:

```http
POST /scoreboard/?capture=the_flag
Content-Type: application/x-www-form-urlencoded

name=<用戶名>&flag=FLAG{...}
```

成功會 redirect 回 scoreboard, 用戶總分跟 Events 列表會更新.
