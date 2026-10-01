# lab01 V07 結果：中文「最...的 (regex: 最[一-鿿]{2,5}的, 排除 3 字如最好的)」加權密度

> 抓取時間 2026-09-30 02:38:08. 公式:
> length_weight(m) = len(m) - 3   (4字=1, 5字=2, 6字=3, 7字=4)
> hit_sum  = Σ length_weight(m);  distinct = unique match 數
> base    = hit_sum × distinct     (越長 + 越多樣 加權越大)
> density = base / chars × 1000    ← 主指標 (per_1k 型)
> Pattern: 最...的 (regex: 最[一-鿿]{2,5}的, 排除 3 字如最好的). 範例命中: 最常成功的 / 最後發生的 / 最不可或缺的
> 假設 AI 愛用 superlative pattern 強調語氣. 對照組 V06B.
> **排行榜最小字數門檻 500 字** (CSV 仍包含全部文章). 排除 739 篇短文.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| 有命中的文章 | 8956（59.5%） |
| 命中總次數 (raw hits) | 20,234 |
| base 總和 (hit_sum × distinct) | 108,742 |
| 全篇總字數 | 39,634,634 |
| 全體 raw per_1k | 0.5105 |
| 全體 density (加權) | 2.7436 |
| 排行榜門檻後文章 | 14318 (排除 739 篇 < 500 字) |

## density (加權) 最高的 20 篇 (chars >= 500)

| # | density | base | hit_sum | distinct | hits | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 132.42 | 348 | 29 | 12 | 18 | 2628 | [Day 4｜為什麼復原要從最後一步開始？](https://ithelp.ithome.com.tw/articles/10406450) | 生活中的資料結構與演算法：30 天學會把現實問題變成可推理的模型 | JavaScript |
| 2 | 98.40 | 160 | 20 | 8 | 8 | 1626 | [Day 4｜取消訂閱的道別藝術：最後一個畫面，是如何重塑你的體驗記憶？](https://ithelp.ithome.com.tw/articles/10405760) | 沒有壞設計，只有剛好戳中你的心機：30天拆解UI裡的心理學筆記 | 自我挑戰 |
| 3 | 78.43 | 253 | 23 | 11 | 11 | 3226 | [Day 29｜如果今天全部重來，我只會留下這幾件事](https://ithelp.ithome.com.tw/articles/10407614) | 當一個有紀律的人，開始用 AI 管理自己的健康 系列。 | ChatGPT & Codex |
| 4 | 65.93 | 42 | 7 | 6 | 6 | 637 | [MySQL/PostgreSQL 弱密碼攻擊實錄](https://ithelp.ithome.com.tw/articles/10412482) | 打靶機 30 天:從 Metasploitable2 到 3 的滲透測試學習筆記 | Claude AI |
| 5 | 61.82 | 171 | 19 | 9 | 9 | 2766 | [Day 11｜一句 Prompt 可以把 UI 改到什麼程度？實測 Stitch](https://ithelp.ithome.com.tw/articles/10409570) | 咖啡、Wi-Fi 與 AI：30 天打造數位遊牧工作地圖 | Build on Google AI |
| 6 | 61.69 | 266 | 19 | 14 | 15 | 4312 | [Day 20  IT 診斷到底需要多大的模型？](https://ithelp.ithome.com.tw/articles/10400908) | 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 | Claude AI |
| 7 | 59.62 | 242 | 22 | 11 | 11 | 4059 | [Day 4｜鑄劍](https://ithelp.ithome.com.tw/articles/10409396) | 《再叩一次》—一個中年轉職叩門者的 OSCP 三十夜 | Security |
| 8 | 55.89 | 243 | 27 | 9 | 10 | 4348 | [Day 2知識工作包含哪五個關鍵步驟？FUDAT 框架完整解析](https://ithelp.ithome.com.tw/articles/10401127) | Data Machi 30 天學習系列：從零開始打造企業 AI 知識工作流 | AI 自動化 |
| 9 | 53.63 | 242 | 22 | 11 | 14 | 4512 | [Day 21：積木能排出幾百種組合，那就全跑一遍挑最好的？這正是會虧錢的地方](https://ithelp.ithome.com.tw/articles/10404045) | 量化交易入門：從 K 線到可組合的交易策略引擎 | Software Development |
| 10 | 52.98 | 105 | 15 | 7 | 7 | 1982 | [Day 16｜第十六章：捨本逐末](https://ithelp.ithome.com.tw/articles/10409181) | 群島計畫：你複製的是結論，還是看見的能力？ | 自我挑戰 |
| 11 | 52.86 | 60 | 12 | 5 | 6 | 1135 | [Day 04  如何把二十年的踩坑經驗變成決策樹](https://ithelp.ithome.com.tw/articles/10400856) | 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 | Claude AI |
| 12 | 52.63 | 126 | 14 | 9 | 9 | 2394 | [Day 9｜第一題不放最重要的，放最不需要背景知識的](https://ithelp.ithome.com.tw/articles/10402925) | 我以為我保留了五道閘門 | AI 自動化 |
| 13 | 50.64 | 288 | 24 | 12 | 12 | 5687 | [Day 16 漫遊探索  旅遊區：軟體也需要自由行：收藏家超模酒吧客，竟然都能拿來抓 Bug](https://ithelp.ithome.com.tw/articles/10402853) | 你的自動化測試，大部分是在演戲｜AI coding 時代的探索性測試 30 講 | Software Development |
| 14 | 49.94 | 136 | 17 | 8 | 10 | 2723 | [Day 13｜Jailbreak：能不能讓它徹底忘記自己是誰？](https://ithelp.ithome.com.tw/articles/10413447) | Medical AI Security Lab：醫療 AI Chatbot 的攻防實驗與自動化 Red Team | AI Security |
| 15 | 49.07 | 50 | 10 | 5 | 5 | 1019 | [Day 10 堆疊：最後放進去的，最先拿出來](https://ithelp.ithome.com.tw/articles/10416500) | 30 天資料結構修行：從零開始理解資料結構 | Software Development |
| 16 | 49.07 | 50 | 10 | 5 | 5 | 1019 | [# [Day 10] Wireshark 下載安裝與介面導覽：開啟圖形化的封包視野](https://ithelp.ithome.com.tw/articles/10416399) | 網管與資安基礎實戰：從 Linux 指令到 Wireshark 封包分析 | Security |
| 17 | 47.98 | 120 | 15 | 8 | 9 | 2501 | [Day 14｜`_Layout.cshtml` 是什麼？共用版型和 `@RenderBody()` 怎麼組成完整頁面？](https://ithelp.ithome.com.tw/articles/10402856) | 學過一點 React，卻被撈進 C#：新手用 AI 硬啃 MVC 企業專案的 3 個月實錄 | 佛心分享-IT 人自學之術 |
| 18 | 47.85 | 40 | 8 | 5 | 6 | 836 | [DAY2 介紹html](https://ithelp.ithome.com.tw/articles/10410869) | 從結構到互動：現代前端HTML、CSS 、JS 的 30 天修練 | Modern Web |
| 19 | 46.62 | 40 | 10 | 4 | 5 | 858 | [程式到底是什麼？為什麼電腦看得懂我們寫的程式？](https://ithelp.ithome.com.tw/articles/10401061) | 程式設計沒有告訴你的事：30 天破解每一個 Why | Software Development |
| 20 | 46.47 | 160 | 16 | 10 | 10 | 3443 | [Day 03｜高風險場域的共同挑戰：資訊過載時間壓力與認知負荷](https://ithelp.ithome.com.tw/articles/10401375) | 打造高風險場域的智慧決策支援系統（AI for Social Good） | Build on Google AI |

## density (加權) 最高的 20 個系列

| # | density | 各篇中位數 | 篇數 | base | hits | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 27.00 | 27.00 | 1 | 98 | 9 | 3629 | 當 AI Agent 走進風機現場：Claude × MCP × 工業維運的 30 天 | Claude AI |
| 2 | 21.14 | 21.14 | 1 | 60 | 6 | 2838 | 台股日頻量化交易：用 AI Engineering 的方法蓋一條從資料、特徵、模型到每日下單清單的產線 | AI Engineering |
| 3 | 16.69 | 12.04 | 15 | 906 | 91 | 54288 | 30億的維運教訓：一個 IT 工程師的 AI 落地避坑指南與營運思維 | IT Operation |
| 4 | 15.95 | 22.20 | 3 | 150 | 16 | 9406 | TW-OCR：高準確率繁體中文公文辨識與版面重建系統 | Software Development |
| 5 | 13.82 | 13.82 | 1 | 30 | 5 | 2170 | 兒時的遊戲圓夢之旅：一人美術、一人開發、一人陣亡 | 佛心分享-SideProject30 |
| 6 | 13.77 | 7.86 | 19 | 972 | 99 | 70571 | 《再叩一次》—一個中年轉職叩門者的 OSCP 三十夜 | Security |
| 7 | 13.77 | 11.84 | 31 | 3199 | 227 | 232264 | 量化交易入門：從 K 線到可組合的交易策略引擎 | Software Development |
| 8 | 13.39 | 2.25 | 3 | 33 | 8 | 2464 | AI Recipe — AI 智慧食譜推薦系統 | 自我挑戰 |
| 9 | 13.26 | 13.26 | 1 | 10 | 2 | 754 | 30 天資安自救指南:輕量網站與 vibecoding | 自我挑戰 |
| 10 | 12.12 | 5.13 | 30 | 899 | 104 | 74160 | 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 | Claude AI |
| 11 | 11.89 | 5.84 | 30 | 1114 | 129 | 93710 | 生活中的資料結構與演算法：30 天學會把現實問題變成可推理的模型 | JavaScript |
| 12 | 11.85 | 4.80 | 15 | 84 | 18 | 7086 | 30天刷完leetcoode75 | Software Development |
| 13 | 11.29 | 6.66 | 11 | 545 | 54 | 48274 | Data Machi 30 天學習系列：從零開始打造企業 AI 知識工作流 | AI 自動化 |
| 14 | 10.93 | 4.71 | 15 | 662 | 60 | 60544 | 零基礎也能當產品長：30 天用 Claude 身兼數職，從零打造軟體產品 | Claude AI |
| 15 | 10.81 | 4.19 | 30 | 901 | 97 | 83372 | 當一個有紀律的人，開始用 AI 管理自己的健康 系列。 | ChatGPT & Codex |
| 16 | 10.52 | 5.14 | 15 | 1013 | 71 | 96259 | 資深工程師的 Claude Code 工作筆記 | Claude AI |
| 17 | 10.39 | 4.89 | 15 | 209 | 40 | 20106 | 網管與資安基礎實戰：從 Linux 指令到 Wireshark 封包分析 | Security |
| 18 | 10.04 | 5.36 | 5 | 129 | 17 | 12854 | 30 天打造讓人敢簽核的 AI Agent：從會回答到可信任的審核型 AI | AI Engineering |
| 19 | 9.78 | 7.33 | 30 | 1294 | 137 | 132334 | 醫院裡的 AI 品管員：30 天，把品質管理交給 AI 試試看 | AI 自動化 |
| 20 | 9.76 | 9.76 | 1 | 10 | 2 | 1025 | 架構師的 Vibe 之道：30 天零預算打造高質感全方位 AI 學習系統 | Vibe Coding |

> 這是共現訊號, 不是判決.
