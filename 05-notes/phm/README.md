# Please Hack Me CTF — Index

[← 回主頁](../../index.md)

[Please Hack Me](https://ctf.hackme.quest/) (PHM, 作者 [Inndy](https://www.inndy.tw/)) 的解題紀錄。老 CTF 練習站, 共 **101 題** 分 8 類 (Misc / Web / Pwn / Reversing / Crypto / Forensic / Programming / Lucky), 題目設計於 2016–2021 期間.

每題一個 `.md` writeup, 按類別分子資料夾.

## 解題進度 — Progress

| 類別 | 總題數 | 已解 | 分數 |
|:---|---:|---:|---:|
| Misc | 14 | 4 | 160 |
| Web | 26 | 0 | 0 |
| Pwn | 24 | 0 | 0 |
| Reversing | 17 | 0 | 0 |
| Crypto | 16 | 1 | 10 |
| Forensic | 2 | 2 | 120 |
| Programming | 1 | 0 | 0 |
| Lucky | 1 | 1 | 110 |
| **合計** | **101** | **8** | **400** |

## 已解題目 — Solved

| 類別 | 題名 | 分數 | Writeup |
|:---|:---|---:|:---|
| Misc | flag | 10 | [misc/flag.md](misc/flag.md) |
| Misc | corgi can fly | 50 | [misc/corgi-can-fly.md](misc/corgi-can-fly.md) |
| Misc | television | 50 | [misc/television.md](misc/television.md) |
| Misc | encoder | 50 | [misc/encoder.md](misc/encoder.md) |
| Crypto | easy | 10 | [crypto/easy.md](crypto/easy.md) |
| Lucky | you-guess | 110 | [lucky/you-guess.md](lucky/you-guess.md) |
| Forensic | easy pdf | 40 | [forensic/easy-pdf.md](forensic/easy-pdf.md) |
| Forensic | this is a pen | 80 | [forensic/this-is-a-pen.md](forensic/this-is-a-pen.md) |

## 卡住未解 — Pending

| 類別 | 題名 | 分數 | Writeup |
|:---|:---|---:|:---|
| Misc | meow | 50 | [misc/meow.md](misc/meow.md) — ZIP in PNG, 加密 ZIP 密碼未找到 (rockyou 全掃 + Pusheen 相關猜測都 miss) |
| Misc | where is flag | 50 | [misc/where-is-flag.md](misc/where-is-flag.md) — 665K 文字塞滿假 FLAG{...}, 已過濾 noise 篩出 3 個 candidate 但 server 都 reject, 真 flag 可能不是 FLAG{...} 格式或有未發現的 regex 線索 |

## 送 flag 流程 — Flag Submission

PHM 的 flag 送到 scoreboard 統一表單:

```http
POST /scoreboard/?capture=the_flag
Content-Type: application/x-www-form-urlencoded

name=<用戶名>&flag=FLAG{...}
```

成功會 redirect 回 scoreboard, 用戶總分跟 Events 列表會更新.
