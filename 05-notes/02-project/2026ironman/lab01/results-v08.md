# lab01 V08 結果：中文「不是…而是 (regex: 不是[^。！？\n]{1,25}而是)」密度

> 抓取時間 2026-09-30 02:38:08. 公式: per_1k = hits / chars * 1000
> Pattern: 不是…而是 (regex: 不是[^。！？\n]{1,25}而是)
> 範例命中: 不是功能, 而是使用者感受 / 不是技術問題而是商業問題
> jason3e7 voice guide 明確說「不用這種對立句」, 預期是強 AI tell.
> **兩版排行並存**: 門檻版 (chars >= 500) + 無門檻版.
> 對立句本身長度足夠, 短文密度不太會巧合命中, 無門檻版的短文大多是「作者 register 愛用對立句」的 signal.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| 有命中的文章 | 4933（32.8%） |
| 命中總次數 | 9,874 |
| 全篇總字數 | 39,634,634 |
| 全體 per_1k | 0.2491 |
| chars < 500 的短文 | 739 篇 (未進門檻版) |

## [A] per_1k 最高的 20 篇 (chars >= 500)

| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|:---|:---|:---|
| 1 | 7.9156 | 6 | 758 | [Day 1｜我為什麼要教最愛的人用 Google AI？](https://ithelp.ithome.com.tw/articles/10403458) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 2 | 5.5556 | 3 | 540 | [Day 06｜第一週的維運復盤：從被動滅火到架構縱深，一線網管的防禦蛻變](https://ithelp.ithome.com.tw/articles/10414509) | 從網管黑手到資安長思維：一線維運的 30 天防禦進化與證照修煉 | 佛心分享-IT 人職涯歷練 |
| 3 | 5.4845 | 15 | 2735 | [第 1 天：我想跟一個 AI Agent 認識，先從 OpenClaw 開始](https://ithelp.ithome.com.tw/articles/10405915) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 4 | 5.4289 | 10 | 1842 | [Day 4 - 架構原則，如何把企業需求轉化成架構設計？](https://ithelp.ithome.com.tw/articles/10401312) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 5 | 5.0912 | 12 | 2357 | [Day 5 - 好的架構，都有共同的特質](https://ithelp.ithome.com.tw/articles/10401376) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 6 | 5.0569 | 4 | 791 | [Day11 AI做遊戲：讓孩子邊玩邊學](https://ithelp.ithome.com.tw/articles/10414884) | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |
| 7 | 4.9932 | 11 | 2203 | [第 3 天：它怎麼接到第一個任務，從入口開始看](https://ithelp.ithome.com.tw/articles/10406368) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 8 | 4.9246 | 16 | 3249 | [第 14 天：流程不是直線，OpenClaw 的工作流思維](https://ithelp.ithome.com.tw/articles/10409357) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 9 | 4.7880 | 14 | 2924 | [Day 1 - 用 Codex 從零做 LINE Bot：不只寫出 Message action，還把 webhook 和部署一路打通](https://ithelp.ithome.com.tw/articles/10402599) | 這隻 LINE Bot 不是我寫的：30 天讓 Codex 從零幫我做到上線 | ChatGPT & Codex |
| 10 | 4.5011 | 18 | 3999 | [第 7 天：記憶不是記越多越好，而是記對的東西](https://ithelp.ithome.com.tw/articles/10407493) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 11 | 4.4770 | 11 | 2457 | [第 2 天：OpenClaw 的腦袋長什麼樣，先看整體架構](https://ithelp.ithome.com.tw/articles/10406117) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 12 | 4.2644 | 10 | 2345 | [第 27 天：skill 在 ClawHub 裡扮演什麼角色](https://ithelp.ithome.com.tw/articles/10416393) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 13 | 4.2614 | 3 | 704 | [Day 6｜不舒服卻不知道掛哪科？先讓 AI 幫你整理，再練習電話掛號](https://ithelp.ithome.com.tw/articles/10403581) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 14 | 4.2017 | 3 | 714 | [Day 3｜七夕送給先生的 AI 禮物：從最想聊的事開始](https://ithelp.ithome.com.tw/articles/10403567) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 15 | 4.2017 | 3 | 714 | [Day 14｜VoCare 的行動能力觀察](https://ithelp.ithome.com.tw/articles/10417985) | VoCare：從 AI 陪伴到智慧長者照護的實作之路 | AI Engineering |
| 16 | 4.1841 | 9 | 2151 | [Day 3 - 架構師思考的，不只是技術](https://ithelp.ithome.com.tw/articles/10401043) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 17 | 4.1841 | 3 | 717 | [Day 9｜看不懂帳單，先讓 AI 幫你找出三個重點](https://ithelp.ithome.com.tw/articles/10405077) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 18 | 4.1806 | 5 | 1196 | [Day 07：讓記憶變成看得懂的東西](https://ithelp.ithome.com.tw/articles/10401689) | 讓 AI Agent 真的做事：用 Embabel 打造可控、可測試的智慧 Dashboard | AI Engineering |
| 19 | 4.1551 | 3 | 722 | [Day 3｜學習 HTML 結構設計與表單基礎](https://ithelp.ithome.com.tw/articles/10407662) | Vibe Coding 實戰手冊：30 天打造具備預算控管與數據視覺化的 React 記帳應用 | Vibe Coding |
| 20 | 4.1102 | 10 | 2433 | [Day 9 - Landing Zone：一套可參考的架構藍圖](https://ithelp.ithome.com.tw/articles/10401798) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |

## [B] per_1k 最高的 20 篇 (無字數門檻, 全 15057 篇)

| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|:---|:---|:---|
| 1 ⚠️ | 10.2740 | 3 | 292 | [菜雞學習資料結構的 30 日讀書分享Day 5](https://ithelp.ithome.com.tw/articles/10407905) | 菜雞學習資料結構的 30 日讀書分享【Day 1】 | 佛心分享-IT 人自學之術 |
| 2 ⚠️ | 10.2740 | 3 | 292 | [菜雞學習資料結構的 30 日讀書分享Day 5](https://ithelp.ithome.com.tw/articles/10409496) | 菜雞學習資料結構的 30 日讀書分享 | 佛心分享-IT 人自學之術 |
| 3 | 7.9156 | 6 | 758 | [Day 1｜我為什麼要教最愛的人用 Google AI？](https://ithelp.ithome.com.tw/articles/10403458) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 4 ⚠️ | 6.7265 | 3 | 446 | [Day 30｜從不敢打開，到能陪另一個人按一次](https://ithelp.ithome.com.tw/articles/10410750) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 5 | 5.5556 | 3 | 540 | [Day 06｜第一週的維運復盤：從被動滅火到架構縱深，一線網管的防禦蛻變](https://ithelp.ithome.com.tw/articles/10414509) | 從網管黑手到資安長思維：一線維運的 30 天防禦進化與證照修煉 | 佛心分享-IT 人職涯歷練 |
| 6 | 5.4845 | 15 | 2735 | [第 1 天：我想跟一個 AI Agent 認識，先從 OpenClaw 開始](https://ithelp.ithome.com.tw/articles/10405915) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 7 | 5.4289 | 10 | 1842 | [Day 4 - 架構原則，如何把企業需求轉化成架構設計？](https://ithelp.ithome.com.tw/articles/10401312) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 8 ⚠️ | 5.3191 | 2 | 376 | [關於我開始踏入資安這檔事Day13－APT到底是什麼？為什麼這麼難防？](https://ithelp.ithome.com.tw/articles/10405222) | 關於我開始踏入資安這檔事 | Security |
| 9 ⚠️ | 5.1546 | 2 | 388 | [Day 3｜我的 AI 員工：5 秒完成證券對帳](https://ithelp.ithome.com.tw/articles/10401440) | 我的AI 員工- 小幫手幫我做了哪些事 | AI 自動化 |
| 10 ⚠️ | 5.1282 | 2 | 390 | [Day 27｜不用背指令，留下自己最敢說的三句話](https://ithelp.ithome.com.tw/articles/10409639) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 11 ⚠️ | 5.1020 | 2 | 392 | [Day 23｜一個人在家有點無聊？請 AI 一次出一題生活猜謎](https://ithelp.ithome.com.tw/articles/10408513) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 12 | 5.0912 | 12 | 2357 | [Day 5 - 好的架構，都有共同的特質](https://ithelp.ithome.com.tw/articles/10401376) | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 13 | 5.0569 | 4 | 791 | [Day11 AI做遊戲：讓孩子邊玩邊學](https://ithelp.ithome.com.tw/articles/10414884) | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |
| 14 | 4.9932 | 11 | 2203 | [第 3 天：它怎麼接到第一個任務，從入口開始看](https://ithelp.ithome.com.tw/articles/10406368) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 15 | 4.9246 | 16 | 3249 | [第 14 天：流程不是直線，OpenClaw 的工作流思維](https://ithelp.ithome.com.tw/articles/10409357) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 16 | 4.7880 | 14 | 2924 | [Day 1 - 用 Codex 從零做 LINE Bot：不只寫出 Message action，還把 webhook 和部署一路打通](https://ithelp.ithome.com.tw/articles/10402599) | 這隻 LINE Bot 不是我寫的：30 天讓 Codex 從零幫我做到上線 | ChatGPT & Codex |
| 17 ⚠️ | 4.7847 | 2 | 418 | [Day 4｜一個Excel工具，讓我重新理解什麼叫教懂](https://ithelp.ithome.com.tw/articles/10413090) | 教使用生成式AI的老師，也是企業員工，怎麼做AI自動化 | AI 自動化 |
| 18 ⚠️ | 4.6296 | 2 | 432 | [Day 18｜老照片別只放著，用聲音留下一段家庭故事](https://ithelp.ithome.com.tw/articles/10407208) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 19 ⚠️ | 4.6296 | 2 | 432 | [Day 15｜明天出門穿什麼？用一句話問天氣與準備](https://ithelp.ithome.com.tw/articles/10405713) | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 20 | 4.5011 | 18 | 3999 | [第 7 天：記憶不是記越多越好，而是記對的東西](https://ithelp.ithome.com.tw/articles/10407493) | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |

## per_1k 最高的 20 個系列

| # | per_1k | 各篇中位數 | 篇數 | hits | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 2.9721 | 2.5353 | 30 | 43 | 14468 | 教會最愛的人用 Google AI：一位自耕農老公與一位 76+ 老爸的 30 天生活實驗 | Build on Google AI |
| 2 | 2.5728 | 2.7226 | 31 | 258 | 100278 | 30 天走進 OpenClaw：一個 AI Agent 的誕生、掙扎與進化 | 佛心分享-SideProject30 |
| 3 | 2.3824 | 2.2628 | 30 | 180 | 75553 | 30 天建立架構思維 - From Blocks to Castle | IT Operation |
| 4 | 1.7065 | 1.7644 | 2 | 11 | 6446 | AI for Social Good：打造高風險場域的智慧決策支援系統 | 自我挑戰 |
| 5 | 1.6967 | 2.0877 | 15 | 11 | 6483 | 教使用生成式AI的老師，也是企業員工，怎麼做AI自動化 | AI 自動化 |
| 6 | 1.6541 | 1.5822 | 12 | 9 | 5441 | 我的AI 員工- 小幫手幫我做了哪些事 | AI 自動化 |
| 7 | 1.5906 | 1.4948 | 15 | 23 | 14460 | AI陪孩子學習：從答案工具變成思考教練 | AI Engineering |
| 8 | 1.5314 | 1.5314 | 1 | 1 | 653 | ChatGPT & Codex 練功日記 | ChatGPT & Codex |
| 9 | 1.4817 | 1.6315 | 30 | 38 | 25647 | 從模糊想法到可操作原型：我的 30 天 AI 協作開發實驗 | Vibe Coding |
| 10 | 1.4357 | 1.4933 | 36 | 124 | 86369 | 用 Hermes Agent 變成企業同事的 30 天 | AI Engineering |
| 11 | 1.3893 | 1.2343 | 14 | 21 | 15116 | 《AI 說完成，我偏要驗收：ChatGPT × Codex 的 30 天成果查核實驗》 | ChatGPT & Codex |
| 12 | 1.3755 | 0.0000 | 6 | 3 | 2181 | 菜雞學習資料結構的 30 日讀書分享【Day 1】 | 佛心分享-IT 人自學之術 |
| 13 | 1.3245 | 1.3245 | 1 | 1 | 755 | 從實作學 RAG：一步步了解 RAG 技術演變 | AI Engineering |
| 14 | 1.3228 | 1.2821 | 30 | 137 | 103565 | UX 的那些事 | 自我挑戰 |
| 15 | 1.2239 | 0.8818 | 15 | 26 | 21243 | 用 AI 打一場鐵人賽：多系列並行的排程、進度與寫作紀律 | 自我挑戰 |
| 16 | 1.2180 | 1.2180 | 1 | 1 | 821 | 當全世界都在吹 AI Coding 誰來測這坨髒東西 | Vibe Coding |
| 17 | 1.2147 | 1.2967 | 8 | 22 | 18111 | AI 時代的 TDD：讓 AI 寫 Code，但不要讓它決定品質 | Software Development |
| 18 | 1.2111 | 1.0417 | 4 | 12 | 9908 | 讓 LLM 說話有憑有據：打造 RAG 知識助理 | AI Engineering |
| 19 | 1.1967 | 1.3360 | 30 | 75 | 62672 | 43歲非工程師爸爸與Codex共築墨寒：30天打造開源AI桌面伴侶 | ChatGPT & Codex |
| 20 | 1.1346 | 0.9363 | 30 | 96 | 84613 | 這隻 LINE Bot 不是我寫的：30 天讓 Codex 從零幫我做到上線 | ChatGPT & Codex |

> 這是共現訊號, 不是判決.
