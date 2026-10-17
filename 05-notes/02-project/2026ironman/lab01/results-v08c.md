# lab01 V08C 結果：中文「不是/不只是…而是/更是 (regex: 不(?:只)?是[^。！？\n]{1,25}(?:而是|更是))」密度

> 抓取時間 2026-09-30 02:38:08. 公式: per_1k = hits / chars * 1000
> Pattern: 不是/不只是…而是/更是 (regex: 不(?:只)?是[^。！？\n]{1,25}(?:而是|更是))
> 範例命中: 不是功能, 而是使用者感受 / 不是技術問題而是商業問題
> jason3e7 voice guide 明確說「不用這種對立句」, 預期是強 AI tell.
> **兩版排行並存**: 門檻版 (chars >= 500) + 無門檻版.
> 對立句本身長度足夠, 短文密度不太會巧合命中, 無門檻版的短文大多是「作者 register 愛用對立句」的 signal.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| 有命中的文章 | 5336（35.4%） |
| 命中總次數 | 11,136 |
| 全篇總字數 | 39,634,634 |
| 全體 per_1k | 0.2810 |
| chars < 500 的短文 | 739 篇 (未進門檻版) |

## [A] per_1k 最高的 20 篇 (chars >= 500)

| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|:---|:---|:---|
| 1 | 9.2348 | 7 | 758 | [Day 1｜我為什麼要教最愛的人用 Google AI？](https://ithelp.ithome.com.tw/articles/10403458) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 2 | 8.4317 | 5 | 593 | [Day1-2026鐵人賽（ AI 不只是工具，而且是工作夥伴）](https://ithelp.ithome.com.tw/articles/10400866) | 風裡雨裡我在AI世界等你 | AI 自動化 |
| 3 | 6.3640 | 15 | 2357 | [Day 5 - 好的架構，都有共同的特質](https://ithelp.ithome.com.tw/articles/10401376) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 4 | 5.9718 | 11 | 1842 | [Day 4 - 架構原則，如何把企業需求轉化成架構設計？](https://ithelp.ithome.com.tw/articles/10401312) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 5 | 5.6075 | 3 | 535 | [Day 04｜從需求開始整理：VoCare 到底需要哪些功能？](https://ithelp.ithome.com.tw/articles/10413345) | AI 不只會聊天：30 天打造 VoCare 智慧陪伴系統 | ChatGPT & Codex |
| 6 | 5.5556 | 3 | 540 | [Day 06｜第一週的維運復盤：從被動滅火到架構縱深，一線網管的防禦蛻變](https://ithelp.ithome.com.tw/articles/10414509) | 從網管黑手到資安長思維：一線維運的 30 天防禦進化與證照修煉 | 佛心分享-IT 人職涯歷練 |
| 7 | 5.4845 | 15 | 2735 | [第 1 天：我想跟一個 AI Agent 認識，先從 OpenClaw 開始](https://ithelp.ithome.com.tw/articles/10405915) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 8 | 5.4348 | 3 | 552 | [Day 14｜ChatGPT AI × SQLite 整合](https://ithelp.ithome.com.tw/articles/10403175) | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 9 | 5.4348 | 3 | 552 | [Day 14｜ChatGPT AI × SQLite 整合](https://ithelp.ithome.com.tw/articles/10406813) | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 10 | 5.1300 | 15 | 2924 | [Day 1 - 用 Codex 從零做 LINE Bot：不只寫出 Message action，還把 webhook 和部署一路打通](https://ithelp.ithome.com.tw/articles/10402599) | 這隻 LINE Bot 不是我寫的：30 天讓 Codex 從零幫我做到上線 | ChatGPT & Codex |
| 11 | 5.0761 | 8 | 1576 | [Day 2 - 架構從來不是一張圖，而是一連串設計決策](https://ithelp.ithome.com.tw/articles/10400935) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 12 | 5.0569 | 4 | 791 | [Day11 AI做遊戲：讓孩子邊玩邊學](https://ithelp.ithome.com.tw/articles/10414884) | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |
| 13 | 4.9932 | 11 | 2203 | [第 3 天：它怎麼接到第一個任務，從入口開始看](https://ithelp.ithome.com.tw/articles/10406368) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 14 | 4.9246 | 16 | 3249 | [第 14 天：流程不是直線，OpenClaw 的工作流思維](https://ithelp.ithome.com.tw/articles/10409357) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 15 | 4.6908 | 11 | 2345 | [第 27 天：skill 在 ClawHub 裡扮演什麼角色](https://ithelp.ithome.com.tw/articles/10416393) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 16 | 4.5506 | 8 | 1758 | [Day 1 - 為什麼需要架構思維](https://ithelp.ithome.com.tw/articles/10400926) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 17 | 4.5181 | 3 | 664 | [Day 02｜先把問題想清楚：VoCare 到底要解決什麼？](https://ithelp.ithome.com.tw/articles/10412107) | AI 不只會聊天：30 天打造 VoCare 智慧陪伴系統 | ChatGPT & Codex |
| 18 | 4.5011 | 18 | 3999 | [第 7 天：記憶不是記越多越好，而是記對的東西](https://ithelp.ithome.com.tw/articles/10407493) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 19 | 4.4770 | 11 | 2457 | [第 2 天：OpenClaw 的腦袋長什麼樣，先看整體架構](https://ithelp.ithome.com.tw/articles/10406117) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 20 | 4.4629 | 14 | 3137 | [Day 17 - 架構師需要培養哪些能力？](https://ithelp.ithome.com.tw/articles/10401889) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |

## [B] per_1k 最高的 20 篇 (無字數門檻, 全 15057 篇)

| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|:---|:---|:---|
| 1 ⚠️ | 10.2740 | 3 | 292 | [菜雞學習資料結構的 30 日讀書分享Day 5](https://ithelp.ithome.com.tw/articles/10407905) | 菜雞學習資料結構的 30 日讀書分享【Day 1】 | 佛心分享-IT 人自學之術 |
| 2 ⚠️ | 10.2740 | 3 | 292 | [菜雞學習資料結構的 30 日讀書分享Day 5](https://ithelp.ithome.com.tw/articles/10409496) | 菜雞學習資料結構的 30 日讀書分享 | 佛心分享-IT 人自學之術 |
| 3 | 9.2348 | 7 | 758 | [Day 1｜我為什麼要教最愛的人用 Google AI？](https://ithelp.ithome.com.tw/articles/10403458) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 4 | 8.4317 | 5 | 593 | [Day1-2026鐵人賽（ AI 不只是工具，而且是工作夥伴）](https://ithelp.ithome.com.tw/articles/10400866) | 風裡雨裡我在AI世界等你 | AI 自動化 |
| 5 ⚠️ | 6.7265 | 3 | 446 | [Day 30｜從不敢打開，到能陪另一個人按一次](https://ithelp.ithome.com.tw/articles/10410750) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 6 | 6.3640 | 15 | 2357 | [Day 5 - 好的架構，都有共同的特質](https://ithelp.ithome.com.tw/articles/10401376) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 7 | 5.9718 | 11 | 1842 | [Day 4 - 架構原則，如何把企業需求轉化成架構設計？](https://ithelp.ithome.com.tw/articles/10401312) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 8 ⚠️ | 5.8480 | 2 | 342 | [有 AI 菜有辦法Day 25](https://ithelp.ithome.com.tw/articles/10418837) | 從農地守護餐桌食安！《審時農曆：短期葉菜栽耕時序決策系統》 | Build on Google AI |
| 9 | 5.6075 | 3 | 535 | [Day 04｜從需求開始整理：VoCare 到底需要哪些功能？](https://ithelp.ithome.com.tw/articles/10413345) | AI 不只會聊天：30 天打造 VoCare 智慧陪伴系統 | ChatGPT & Codex |
| 10 | 5.5556 | 3 | 540 | [Day 06｜第一週的維運復盤：從被動滅火到架構縱深，一線網管的防禦蛻變](https://ithelp.ithome.com.tw/articles/10414509) | 從網管黑手到資安長思維：一線維運的 30 天防禦進化與證照修煉 | 佛心分享-IT 人職涯歷練 |
| 11 | 5.4845 | 15 | 2735 | [第 1 天：我想跟一個 AI Agent 認識，先從 OpenClaw 開始](https://ithelp.ithome.com.tw/articles/10405915) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 12 | 5.4348 | 3 | 552 | [Day 14｜ChatGPT AI × SQLite 整合](https://ithelp.ithome.com.tw/articles/10403175) | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 13 | 5.4348 | 3 | 552 | [Day 14｜ChatGPT AI × SQLite 整合](https://ithelp.ithome.com.tw/articles/10406813) | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 14 ⚠️ | 5.3191 | 2 | 376 | [關於我開始踏入資安這檔事Day13－APT到底是什麼？為什麼這麼難防？](https://ithelp.ithome.com.tw/articles/10405222) | 關於我開始踏入資安這檔事 | Security |
| 15 ⚠️ | 5.1546 | 2 | 388 | [Day 3｜我的 AI 員工：5 秒完成證券對帳](https://ithelp.ithome.com.tw/articles/10401440) | 我的AI 員工- 小幫手幫我做了哪些事 | AI 自動化 |
| 16 | 5.1300 | 15 | 2924 | [Day 1 - 用 Codex 從零做 LINE Bot：不只寫出 Message action，還把 webhook 和部署一路打通](https://ithelp.ithome.com.tw/articles/10402599) | 這隻 LINE Bot 不是我寫的：30 天讓 Codex 從零幫我做到上線 | ChatGPT & Codex |
| 17 ⚠️ | 5.1282 | 2 | 390 | [Day 27｜不用背指令，留下自己最敢說的三句話](https://ithelp.ithome.com.tw/articles/10409639) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 18 ⚠️ | 5.1020 | 2 | 392 | [Day 23｜一個人在家有點無聊？請 AI 一次出一題生活猜謎](https://ithelp.ithome.com.tw/articles/10408513) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 19 | 5.0761 | 8 | 1576 | [Day 2 - 架構從來不是一張圖，而是一連串設計決策](https://ithelp.ithome.com.tw/articles/10400935) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 20 | 5.0569 | 4 | 791 | [Day11 AI做遊戲：讓孩子邊玩邊學](https://ithelp.ithome.com.tw/articles/10414884) | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |

## per_1k 最高的 20 個系列

| # | per_1k | 各篇中位數 | 篇數 | hits | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 4.0733 | 4.0733 | 1 | 2 | 491 | 研究所新手村的 30 天技術探索：用 AI 陪我從「不知道學什麼」開始 | 自我挑戰 |
| 2 | 3.9370 | 3.9370 | 1 | 2 | 508 | ChatGPT 陪我從 0 打造 LINE Bot | ChatGPT & Codex |
| 3 | 3.1103 | 2.5977 | 30 | 45 | 14468 | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 4 | 3.1027 | 3.2120 | 2 | 20 | 6446 | AI for Social Good：打造高風險場域的智慧決策支援系統 | 自我挑戰 |
| 5 | 3.0628 | 3.0628 | 1 | 2 | 653 | ChatGPT & Codex 練功日記 | ChatGPT & Codex |
| 6 | 2.9383 | 2.8156 | 30 | 222 | 75553 | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 7 | 2.6327 | 2.7226 | 31 | 264 | 100278 | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 8 | 2.1438 | 2.3952 | 15 | 31 | 14460 | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |
| 9 | 1.9210 | 1.6850 | 16 | 23 | 11973 | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 10 | 1.8900 | 1.6850 | 14 | 20 | 10582 | AI照護小幫手：智慧健康與情緒關懷系統 | ChatGPT & Codex |
| 11 | 1.6967 | 2.0877 | 15 | 11 | 6483 | 教使用生成式AI的老師，也是企業員工，怎麼做AI自動化 | AI 自動化 |
| 12 | 1.6541 | 1.5822 | 12 | 9 | 5441 | 我的AI 員工- 小幫手幫我做了哪些事 | AI 自動化 |
| 13 | 1.6511 | 1.7729 | 30 | 171 | 103565 | UX 的那些事 | 自我挑戰 |
| 14 | 1.5206 | 1.6315 | 30 | 39 | 25647 | 從模糊想法到可操作原型：我的 30 天 AI 協作開發實驗 | Vibe Coding |
| 15 | 1.4589 | 1.5560 | 36 | 126 | 86369 | 用 Hermes Agent 變成企業同事的 30 天 | AI Engineering |
| 16 | 1.4034 | 1.2399 | 16 | 22 | 15676 | VoCare：從 AI 陪伴到智慧長者照護的實作之路 | AI Engineering |
| 17 | 1.3893 | 1.2343 | 14 | 21 | 15116 | 《AI 說完成，我偏要驗收：ChatGPT × Codex 的 30 天成果查核實驗》 | ChatGPT & Codex |
| 18 | 1.3755 | 0.0000 | 6 | 3 | 2181 | 菜雞學習資料結構的 30 日讀書分享【Day 1】 | 佛心分享-IT 人自學之術 |
| 19 | 1.3252 | 1.2967 | 8 | 24 | 18111 | AI 時代的 TDD：讓 AI 寫 Code，但不要讓它決定品質 | Software Development |
| 20 | 1.3245 | 1.3245 | 1 | 1 | 755 | 從實作學 RAG：一步步了解 RAG 技術演變 | AI Engineering |

> 這是共現訊號, 不是判決.
