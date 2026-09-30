# lab01 V02 結果：條列項「粗體標籤 + 一般文字」比例 (嚴格版)

> 抓取時間 2026-09-30 02:38:08。ratio = bold_li / total_li
> bold_li = 以 `<strong>...</strong>` 開頭, 且後面**直接接一般字型文字**的 `<li>`.
> 排除「只有粗體」、「粗體後接標籤 (連結/程式碼)」的情況. 排名不設條列數門檻, 短列表仍會爆.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| 有條列的文章 | 10531（69.9%） |
| 有粗體條列的文章 | 4239（28.2%） |
| 總條列項 (li) | 156281 |
| 粗體開頭條列項 | 29178 |
| 全體比例 | 18.67% |
| 全體粗體條列每千字 | 0.74 |

## 粗體條列比例最高的 20 篇（不設條列數門檻）

| # | 比例 | 粗體 li | 總 li | 字數 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 100.00% | 11 | 11 | 1244 | [NVIDIA GTC Taipei 2026 主題演講-DAY4](https://ithelp.ithome.com.tw/articles/10401309) | UX 的那些事 | 自我挑戰 |
| 2 | 100.00% | 5 | 5 | 908 | [Day 22 Agent安全性](https://ithelp.ithome.com.tw/articles/10404455) | 和AI學習gcp - 建立對話代理 | 自我挑戰 |
| 3 | 100.00% | 8 | 8 | 2026 | [# Day7 IAP > IAM based Auth](https://ithelp.ithome.com.tw/articles/10401944) | 和AI學習gcp - 建立對話代理 | 自我挑戰 |
| 4 | 100.00% | 3 | 3 | 1407 | [Day5 IaC 核心：用 Terraform 定義 POC 環境](https://ithelp.ithome.com.tw/articles/10401622) | 和AI學習gcp - 建立對話代理 | 自我挑戰 |
| 5 | 100.00% | 4 | 4 | 1306 | [Day2 資源階層與預算初探](https://ithelp.ithome.com.tw/articles/10400987) | 和AI學習gcp - 建立對話代理 | 自我挑戰 |
| 6 | 100.00% | 4 | 4 | 2359 | [Day 23｜同一條短網址，讓 iPhone 開 App StoreAndroid 開 Google Play](https://ithelp.ithome.com.tw/articles/10410803) | toui：一條短網址能做到哪些事——從轉址到 AI 工作流 | 佛心分享-SideProject30 |
| 7 | 100.00% | 3 | 3 | 1951 | [Day 09｜AWS SES 費用：上線前五天，我把已經能動的寄信服務拆掉重做](https://ithelp.ithome.com.tw/articles/10406653) | toui：一條短網址能做到哪些事——從轉址到 AI 工作流 | 佛心分享-SideProject30 |
| 8 | 100.00% | 2 | 2 | 2980 | [Day 08｜名字取好只是開始：side project 轉成正式作品的命名學](https://ithelp.ithome.com.tw/articles/10406372) | toui：一條短網址能做到哪些事——從轉址到 AI 工作流 | 佛心分享-SideProject30 |
| 9 | 100.00% | 4 | 4 | 1552 | [Day 06｜短網址的點擊次數：誰在數什麼時候數為什麼會晚一點到](https://ithelp.ithome.com.tw/articles/10405899) | toui：一條短網址能做到哪些事——從轉址到 AI 工作流 | 佛心分享-SideProject30 |
| 10 | 100.00% | 5 | 5 | 2390 | [Day 03｜縮網址工具這麼多，我自己在挑的時候會看什麼](https://ithelp.ithome.com.tw/articles/10405256) | toui：一條短網址能做到哪些事——從轉址到 AI 工作流 | 佛心分享-SideProject30 |
| 11 | 100.00% | 4 | 4 | 1520 | [Day 07  GitHub Pages 部署踩坑實錄](https://ithelp.ithome.com.tw/articles/10400892) | 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 | Claude AI |
| 12 | 100.00% | 3 | 3 | 1341 | [Day 02  現有 AI 工具最大的問題：太會回答，卻不會排障](https://ithelp.ithome.com.tw/articles/10400853) | 從現場踩坑到 AI 工具 — IT Diagnostic Agent 開發實錄 | Claude AI |
| 13 | 100.00% | 4 | 4 | 4338 | [Day 27：一句話重啟容器MCP Server 工具層實作](https://ithelp.ithome.com.tw/articles/10404365) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 14 | 100.00% | 5 | 5 | 2858 | [Day 26：自架系統資安自白token 進了 git密碼寫進 Markdown](https://ithelp.ithome.com.tw/articles/10404362) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 15 | 100.00% | 2 | 2 | 2229 | [Day 24：給 AI 立法任何排程任務都要過的五道閘門](https://ithelp.ithome.com.tw/articles/10404361) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 16 | 100.00% | 3 | 3 | 2868 | [Day 21：第三週小結我自動化的不是決策，是那些讓我看不見狀況的雜事](https://ithelp.ithome.com.tw/articles/10403160) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 17 | 100.00% | 2 | 2 | 5058 | [Day 20：服務掛了不用我管兩層監控的自癒架構](https://ithelp.ithome.com.tw/articles/10403159) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 18 | 100.00% | 2 | 2 | 6796 | [Day 15：一個系統，兩個排程器後來我把大半的 AI 拿掉了](https://ithelp.ithome.com.tw/articles/10402040) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 19 | 100.00% | 13 | 13 | 2357 | [Day 13：我的 AI 沒有資料庫全 Markdown 架構的瘋狂與合理](https://ithelp.ithome.com.tw/articles/10402000) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 20 | 100.00% | 10 | 10 | 3066 | [Day 11：兩個 AI 怎麼不分裂跨系統記憶協作](https://ithelp.ithome.com.tw/articles/10401998) | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |

## 粗體條列比例最高的 20 個系列

| # | 比例 | 各篇中位數 | 篇數 | 粗體 li | 總 li | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 100.00% | 0.00% | 16 | 3 | 3 | 如何讓 AI 主動完成複雜任務？Claude Code × Agentic Workflow 實戰 | Claude AI |
| 2 | 100.00% | 100.00% | 1 | 7 | 7 | 結合 Google AI 的網路迷因意圖辨識與社群情緒分析系統 | Build on Google AI |
| 3 | 95.83% | 100.00% | 20 | 138 | 144 | 群島手記：你要的是答案，還是找答案的方法？ | 佛心分享-IT 人自學之術 |
| 4 | 88.24% | 83.33% | 2 | 15 | 17 | AI 產品經理的決策修練：在不確定性下打造 AI 功能 | AI Engineering |
| 5 | 83.49% | 100.00% | 15 | 91 | 109 | Re:從零開始做直播代購電商平台 | Software Development |
| 6 | 83.33% | 80.00% | 3 | 10 | 12 | 讓 Agent 敢上 production：30 天蓋一套 AI 可靠性工程 | AI Engineering |
| 7 | 81.71% | 82.84% | 30 | 277 | 339 | 老爺爺練習VIBE CODING | 佛心分享-IT 人自學之術 |
| 8 | 81.25% | 0.00% | 17 | 13 | 16 | 當 AI 加入團隊：打造可審查、可驗證、會自我改善的 AI 開發工作流 | AI Engineering |
| 9 | 80.65% | 0.00% | 30 | 25 | 31 | 怎麼找到新東西？把 200 年的「發現」老方法教給 AI——方法圖鑑 × Claude Skills | Claude AI |
| 10 | 79.94% | 90.42% | 30 | 247 | 309 | 迎接 AI 開發爆發期：告別手動部署，帶領企業團隊從 Git 規範到 CI/CD 實戰 | IT Operation |
| 11 | 79.31% | 84.52% | 30 | 184 | 232 | 前端三分鐘 X 要轉職養豬還是做被取代的工程師？用 Google AI 打造我的 AI 雙刀流自動化工作流 | Build on Google AI |
| 12 | 78.26% | 0.00% | 30 | 54 | 69 | 就決定是你了！打造寶可夢持有追蹤系統 (React x Express) | 佛心分享-SideProject30 |
| 13 | 77.96% | 80.00% | 15 | 145 | 186 | 零基礎也能當產品長：30 天用 Claude 身兼數職，從零打造軟體產品 | Claude AI |
| 14 | 77.40% | 79.94% | 30 | 435 | 562 | 新手上路不當砲灰！30 天一章一章啃完 CompTIA SecAI+ 備考筆記 | AI Security |
| 15 | 76.74% | 0.00% | 30 | 33 | 43 | 模型動不了，那你能動什麼？AI Engineering 四層工程觀：Prompt、Context、Harness、Loop | AI Engineering |
| 16 | 76.70% | 100.00% | 15 | 293 | 382 | 30 天的 SAA 學習筆記 | 自我挑戰 |
| 17 | 76.61% | 100.00% | 17 | 95 | 124 | 《解構生命暗物質：用 AlphaGenome Atlas 破譯 98% 非編碼基因組的 30 天實戰》 | Build on Google AI |
| 18 | 76.34% | 83.63% | 8 | 71 | 93 | 30天從零打造 AI 中台自學之路 | AI Engineering |
| 19 | 75.05% | 87.31% | 40 | 358 | 477 | 資訊安全 | 佛心分享-IT 人自學之術 |
| 20 | 75.00% | 70.29% | 16 | 81 | 108 | 文科生的軟體工程啟蒙：用一個代購 App，看懂 30 個系統設計觀念 | Software Development |

> 沒條列 (total_li = 0) 的文章 ratio = 0, 全部沉底 (不代表沒 AI 味).
> 短列表 (1-3 個 li) 中 1 個粗體就是 33-100%, 排行看時要一起看「總 li」欄.
> 這是共現訊號, 不是判決.
