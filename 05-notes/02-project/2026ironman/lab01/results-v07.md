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

## 命中詞主要 Top 20 (附證據文章 Top 3)

> 全 corpus 掃出 4530 種 unique 命中詞, 列出 top 20 跟其對應的 top 3 證據文章
> 證據文章 = 該命中詞在哪 3 篇文章出現最多次, 讀者可直接點進去看語境

**1. 「最重要的」** (全 corpus ×2144)
- [AI Agent 29Eval-first：不要先加複雜架構，先驗證你在解決什麼問題](https://ithelp.ithome.com.tw/articles/10405661) ×6  — 《30 天從零拆解 AI Agent：從 Tool Calling 到多 Agent 協作》 · AI Engineering
- [Day 28 -泰坦之王克洛諾斯誰說改版一定要全部砍掉重寫？聊聊 Strangler Fig Pattern](https://ithelp.ithome.com.tw/articles/10405584) ×5  — 諸神也搖頭的 Legacy Code： 30天 .NET 工程師生存之道 · Software Development
- [RecyclerView × Room：Android 清單顯示與本機資料庫 CRUD](https://ithelp.ithome.com.tw/articles/10405012) ×5  — Android  鐵人賽: 天命最高 - 陪伴大家一步步打造屬於自己的app · Software Development

**2. 「最常見的」** (全 corpus ×803)
- [企業端點完美防禦02-防止惡意軟體感染](https://ithelp.ithome.com.tw/articles/10400924) ×4  — 企業端點完美防禦 · Security
- [Day 22：Audit Log一筆終端事件四條 exit path，還有兩條刻意不發](https://ithelp.ithome.com.tw/articles/10404339) ×4  — Backend 工程師的 Azure GenAI 實戰 · AI Engineering
- [Day 12｜病患資料測試：這次守住了，但守住的理由讓我不太放心](https://ithelp.ithome.com.tw/articles/10412771) ×4  — Medical AI Security Lab：醫療 AI Chatbot 的攻防實驗與自動化 Red Team · AI Security

**3. 「最基本的」** (全 corpus ×681)
- [Day 03 - 快速上手：建立第一個 GitHub Copilot Agent 應用](https://ithelp.ithome.com.tw/articles/10407139) ×5  — GitHub Copilot SDK 實戰：從 Agent 應用到可部署服務 · AI Engineering
- [Day 2｜從零開始：建立並執行第一個 C# 專案](https://ithelp.ithome.com.tw/articles/10409174) ×5  — 現在就學C# 與 ASP.NET Core · Modern Web
- [Framework 組好了，但真的能用嗎？開始做完整整合測試](https://ithelp.ithome.com.tw/articles/10406274) ×3  — 程式設計沒有告訴你的事：30 天破解每一個 Why · Software Development

**4. 「最簡單的」** (全 corpus ×588)
- [Handler 的參數到底從哪裡來？Framework 怎麼做 Parameter Binding？](https://ithelp.ithome.com.tw/articles/10404717) ×4  — 程式設計沒有告訴你的事：30 天破解每一個 Why · Software Development
- [# Day 11｜Message Queue：為什麼大型系統不把所有工作都塞在同一個 Request？](https://ithelp.ithome.com.tw/articles/10417122) ×4  — 30 天從 Full-Stack Engineer 進化到 System Design：從 0 設計可支撐百萬使用者的系統 · Software Development
- [Day 10 \| 為甚麼要幫 Agent 做好 Provenance label？](https://ithelp.ithome.com.tw/articles/10407158) ×3  — 合法呼叫湊出的攻擊鏈：AI Agent 防禦的 30 天觀念養成 · AI Security

**5. 「最核心的」** (全 corpus ×368)
- [把 RouterHandlerResponse 接起來：Framework 的核心流程終於成形](https://ithelp.ithome.com.tw/articles/10405812) ×3  — 程式設計沒有告訴你的事：30 天破解每一個 Why · Software Development
- [Framework 到底怎麼根據 Path 找到正確的程式？](https://ithelp.ithome.com.tw/articles/10402808) ×3  — 程式設計沒有告訴你的事：30 天破解每一個 Why · Software Development
- [Day 23: 你敢不敢當那個逼大家對齊的人？](https://ithelp.ithome.com.tw/articles/10401974) ×3  — Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 · Software Development

**6. 「最直接的」** (全 corpus ×362)
- [Day 23：只移動一個 Node，為什麼要把整張圖重算一次？]] 重賽版[[](https://ithelp.ithome.com.tw/articles/10409302) ×3  — GPU 效能優化實戰：30 天從 Kernel 到 Profiling (重賽版) · Software Development
- [Day2：AIOps 要的不是更多資料，是可推斷的資料](https://ithelp.ithome.com.tw/articles/10402090) ×2  — 賢者大叔的觀測結界：讓 agent 推理得動的 30 天 · AI Engineering
- [Day 3 - 為了知道 Command 跑完沒，我們先把LOG搞髒了：UART LOG 汙染](https://ithelp.ithome.com.tw/articles/10401281) ×2  — 一天一個 PR，讓我告訴你嵌入式開發的殘酷：如何為 AI Agent 建立可信的工程閉環 · AI Engineering

**7. 「最直覺的」** (全 corpus ×337)
- [Day 7 - 陣列(Array) - Leetcode實作](https://ithelp.ithome.com.tw/articles/10402767) ×3  — 從0開始的資料結構旅程! · Software Development
- [Day 15｜為了躲開一種不穩定，換到了另一種不穩定](https://ithelp.ithome.com.tw/articles/10404038) ×2  — 一條線救一隻狗：我用 PixiJS、Matter.js 和一條有閘門的 AI 產線做完一款網頁小遊戲 · JavaScript
- [Day 19｜讓 AI Agent 借得到簽名，拿不到印鑑：ssh-agent](https://ithelp.ithome.com.tw/articles/10405426) ×2  — AI 的駕馭之道：一個 AI Code Reviewer 的養成、評測與邊界實錄 · AI Engineering

**8. 「最關鍵的」** (全 corpus ×300)
- [Day 25：存取控制與最小權限](https://ithelp.ithome.com.tw/articles/10409567) ×3  — 從法條到程式碼：台灣 AI 治理與資安合規實戰指南 · AI Security
- [Day 27資安是管理出來的：ISO 27001 / ISMS 到底在管什麼？新手也能懂的管理精髓](https://ithelp.ithome.com.tw/articles/10410719) ×3  — 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 · Security
- [Day 20  IT 診斷到底需要多大的模型？](https://ithelp.ithome.com.tw/articles/10400908) ×2  — 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 · Claude AI

**9. 「最危險的」** (全 corpus ×298)
- [評估與驗證：如何證明 AI真的做完了](https://ithelp.ithome.com.tw/articles/10409439) ×4  — 用 Hermes Agent 變成企業同事的 30 天 · AI Engineering
- [Day 7｜真正難懂的不是程式碼，而是沒人寫下來的商業規則](https://ithelp.ithome.com.tw/articles/10414280) ×4  — AI 救得了祖傳系統嗎？30 天實戰企業 Legacy System × AI 協作開發 · ChatGPT & Codex
- [[ Enterprise Architecture ] Day 29  自我進化閉環：把手動的那條鏈路接起來](https://ithelp.ithome.com.tw/articles/10410371) ×3  — 從 MCP 到專屬 Agentic 模型：30 天走完一條可評測、可微調、可自架的 AI Agent 模型與服務製作流程 · AI Engineering

**10. 「最有價值的」** (全 corpus ×222)
- [Day 30：30 天寫完我的 AI 管家成本數據與下一步](https://ithelp.ithome.com.tw/articles/10404363) ×2  — 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 · AI Engineering
- [Day5：Weaver 上手，schema 是團隊共識](https://ithelp.ithome.com.tw/articles/10402401) ×2  — 賢者大叔的觀測結界：讓 agent 推理得動的 30 天 · AI Engineering
- [Day 28｜Eval Function：自我改進的天花板](https://ithelp.ithome.com.tw/articles/10406088) ×2  — 模型動不了，那你能動什麼？AI Engineering 四層工程觀：Prompt、Context、Harness、Loop · AI Engineering

**11. 「最常用的」** (全 corpus ×174)
- [效能調教 22.擷取查詢效能指標的方法](https://ithelp.ithome.com.tw/articles/10404401) ×2  — SQL Server  基礎&調教 · 自我挑戰
- [Day7: Assertion:你到底在驗證什麼](https://ithelp.ithome.com.tw/articles/10401828) ×2  — AI 時代下最值得投資的 UI 自動化:30 天用 Claude Code 學會寫 Playwright · Claude AI
- [Day 19 - 30 天手把手學會 Chart.js｜互動事件處理](https://ithelp.ithome.com.tw/articles/10401617) ×2  — 30 天手把手學會 Chart.js v4：從圖表基礎到互動式資料視覺化實戰 · Modern Web

**12. 「最麻煩的」** (全 corpus ×149)
- [Day 28 -泰坦之王克洛諾斯誰說改版一定要全部砍掉重寫？聊聊 Strangler Fig Pattern](https://ithelp.ithome.com.tw/articles/10405584) ×2  — 諸神也搖頭的 Legacy Code： 30天 .NET 工程師生存之道 · Software Development
- [Day 6：為什麼模型會一臉自信地講幹話？最近很新的 JEV 是什麼？](https://ithelp.ithome.com.tw/articles/10414378) ×2  — 一個 AI 可以回答問題，一支 AI 團隊，才能開始真正做事！ · AI Engineering
- [Email 驗證卡在哪？Sniper Link 的解法 -DAY23](https://ithelp.ithome.com.tw/articles/10403794) ×1  — UX 的那些事 · 自我挑戰

**13. 「最便宜的」** (全 corpus ×125)
- [Day 3不知道用哪個模型？官方建議從 Opus 5 開始背後的思維](https://ithelp.ithome.com.tw/articles/10404150) ×5  — Claude 用得對，也用得省：工程師帶你搞懂選模型、Token 優化與底層邏輯 · Claude AI
- [[Day 04] 演算法正確性](https://ithelp.ithome.com.tw/articles/10413048) ×5  — 30 天的資料結構與演算法之旅 · Software Development
- [Day 1Claude 模型怎麼選？2026 最新四階模型 Fable 5 / Opus 5 / Sonnet 5 / Haiku 4.5 完整比較](https://ithelp.ithome.com.tw/articles/10403792) ×3  — Claude 用得對，也用得省：工程師帶你搞懂選模型、Token 優化與底層邏輯 · Claude AI

**14. 「最明顯的」** (全 corpus ×116)
- [企業端點完美防禦09-勒索軟體運作方式受害後的清除方法](https://ithelp.ithome.com.tw/articles/10401461) ×2  — 企業端點完美防禦 · Security
- [Day 29｜這 30 天，我從一個新手變成了什麼樣的人？](https://ithelp.ithome.com.tw/articles/10406052) ×2  — 從看不懂到做出來，用 PawPal 走過前端新手村 · JavaScript
- [Day 05｜有 Wi-Fi 還不夠！什麼才叫適合工作的咖啡廳？](https://ithelp.ithome.com.tw/articles/10407986) ×2  — 咖啡、Wi-Fi 與 AI：30 天打造數位遊牧工作地圖 · Build on Google AI

**15. 「最基礎的」** (全 corpus ×115)
- [Day 24前端如何呼叫 Claude API？Messages 端點入門](https://ithelp.ithome.com.tw/articles/10409102) ×2  — Claude 用得對，也用得省：工程師帶你搞懂選模型、Token 優化與底層邏輯 · Claude AI
- [Day 16 - Multi-Agent 的最基礎形式：Template Workflow Agents](https://ithelp.ithome.com.tw/articles/10409025) ×2  — Google ADK Agent 教戰：30 天從原型到可上線的 AI Agent 系統 · Build on Google AI
- [Day16 - Amazon Bedrock 是什麼？](https://ithelp.ithome.com.tw/articles/10418910) ×2  — 營養師想做一個飲食建議產品 · 佛心分享-SideProject30

**16. 「最適合的」** (全 corpus ×114)
- [使用gemini 準備AZ-900 Day21  Phase 3複習](https://ithelp.ithome.com.tw/articles/10409878) ×4  — 使用gemini  準備 az-900 · Build on Google AI
- [Day 28  拒絕殺雞用牛刀：用 50 行 Python 手刻極簡 RAG 引擎](https://ithelp.ithome.com.tw/articles/10400822) ×2  — 買錯保險的血淚教訓：我用 Python + AI Agent 重構人生財務防禦系統 · AI Engineering
- [Day 28 - 30 天手把手學會 Chart.js｜專案規劃與資料設計](https://ithelp.ithome.com.tw/articles/10403472) ×2  — 30 天手把手學會 Chart.js v4：從圖表基礎到互動式資料視覺化實戰 · Modern Web

**17. 「最容易被忽略的」** (全 corpus ×112)
- [Day 26Claude API 錯誤處理與重試：正式環境該注意什麼](https://ithelp.ithome.com.tw/articles/10409721) ×2  — Claude 用得對，也用得省：工程師帶你搞懂選模型、Token 優化與底層邏輯 · Claude AI
- [Day 16｜平均 Latency 為什麼會騙人？請看尾端](https://ithelp.ithome.com.tw/articles/10418269) ×2  — Learning SRE for the AI Era：從 SRE Lab 到 Production AI Reliability · AI Engineering
- [Day 11｜Production Failure Modes：先替系統想好難看的死法](https://ithelp.ithome.com.tw/articles/10414863) ×2  — Learning SRE for the AI Era：從 SRE Lab 到 Production AI Reliability · AI Engineering

**18. 「最主要的」** (全 corpus ×111)
- [[Day 15] 餵給 AI 的不只是帳單：用 Python 量化 SubWise 的消費結構](https://ithelp.ithome.com.tw/articles/10403274) ×2  — AI 時代的輕量化開發：ChatGPT 打造 LINE 多模態記帳與續訂預警 Agent · ChatGPT & Codex
- [Day 09｜不要只問哪個最好：把研究問題寫成可驗證的假設](https://ithelp.ithome.com.tw/articles/10402405) ×2  — 30 天打造公開資料版急診檢傷系統：Side Project 與實驗計畫 · 佛心分享-SideProject30
- [Day 02｜急診為什麼需要檢傷？先看懂問題，不急著談模型](https://ithelp.ithome.com.tw/articles/10401430) ×2  — 30 天打造公開資料版急診檢傷系統：Side Project 與實驗計畫 · 佛心分享-SideProject30

**19. 「最相關的」** (全 corpus ×97)
- [Day 6什麼是 RAG？把 AI 從閉卷變成開卷考試](https://ithelp.ithome.com.tw/articles/10401845) ×5  — Data Machi 30 天學習系列：從零開始打造企業 AI 知識工作流 · AI 自動化
- [Day 15｜不用 Vector Database，自己實作一次 Semantic Search](https://ithelp.ithome.com.tw/articles/10407997) ×3  — 從 Stateless LLM 到 Agentic Memory：30 天打造會記憶的 AI Agent · AI Engineering
- [[Day 5] 建立第一個 Vector Database：讓 AI 助理搜尋自己的知識庫](https://ithelp.ithome.com.tw/articles/10412208) ×3  — AI 不只會回答：30 天打造一套真正能上線的智慧助理 · 自我挑戰

**20. 「最在意的」** (全 corpus ×95)
- [基礎 15.HA & DR 概念](https://ithelp.ithome.com.tw/articles/10403150) ×2  — SQL Server  基礎&調教 · 自我挑戰
- [Day 14｜權限控管：不是每個人都能做每件事](https://ithelp.ithome.com.tw/articles/10403064) ×2  — 從看不懂到做出來，用 PawPal 走過前端新手村 · JavaScript
- [Day 3｜PixiJS 是渲染器，不是遊戲引擎：它不做的每一件事都會變成你的檔案](https://ithelp.ithome.com.tw/articles/10402113) ×2  — 一條線救一隻狗：我用 PixiJS、Matter.js 和一條有閘門的 AI 產線做完一款網頁小遊戲 · JavaScript


> 這是共現訊號, 不是判決.
