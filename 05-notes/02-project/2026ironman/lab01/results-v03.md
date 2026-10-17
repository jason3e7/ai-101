# lab01 V03 結果：blockquote (`> `) 密度

> 抓取時間 2026-09-30 02:38:08. 公式:
> ratio  = bq_chars / total_chars      (blockquote 佔全文的視覺比例)
> per_1k = bq_count / total_chars * 1000  (每千字幾個 blockquote)
> 排名不設字數門檻, 短文仍可能爆.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| 有 blockquote 的文章 | 6464（42.9%） |
| blockquote 總數 | 26976 |
| blockquote 內字元總和 | 1501899 |
| 全篇總字數 | 39634634 |
| 全體 ratio (字元佔比) | 3.79% |
| 全體 per_1k (頻率) | 0.68 |

## ratio 最高的 20 篇（不設字數門檻）

| # | ratio% | per_1k | bq_count | bq_chars | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 69.25% | 2.99 | 1 | 232 | 335 | [Day 04｜實戰安裝：VirtualBox 下載](https://ithelp.ithome.com.tw/articles/10407247) | 大學生的VirtualBox虛擬機與Ubuntu實戰30天 | IT Operation |
| 2 | 64.55% | 0.72 | 1 | 896 | 1388 | [進度+10%](https://ithelp.ithome.com.tw/articles/10417275) | 不會 Code，也要 Vibe :((( | Vibe Coding |
| 3 | 58.77% | 2.04 | 12 | 3456 | 5881 | [資安事件處置流程（SOC 團隊版）page 3](https://ithelp.ithome.com.tw/articles/10415699) | 企業管理自動化與執行框架-以SOC運作為實例 | Security |
| 4 | 54.72% | 0.47 | 1 | 1154 | 2109 | [做了兩個頁面，今天才想起來要把它們接起來](https://ithelp.ithome.com.tw/articles/10417039) | 不會 Code，也要 Vibe :((( | Vibe Coding |
| 5 | 54.64% | 10.14 | 7 | 377 | 690 | [[Day 09] 走出平均值盲點：巴西各州單量集中度與客單價 (AOV) 反差](https://ithelp.ithome.com.tw/articles/10411764) | 拯救混亂數據：30 天 Python 輕量級 ETL 與電商關聯資料分析 | 自我挑戰 |
| 6 | 54.17% | 0.60 | 2 | 1812 | 3345 | [Day 7 - 緩衝日：後台 CRUD 流程實戰演練](https://ithelp.ithome.com.tw/articles/10412716) | Playwright 練功房：從零開始的 30 天 E2E 測試教學筆記 | 自我挑戰 |
| 7 | 53.99% | 2.59 | 6 | 1253 | 2321 | [Day 15 \| 中場採訪：當 AI 回頭看著那位與我共舞的工程師](https://ithelp.ithome.com.tw/articles/10418001) | 在贏家書寫歷史之前：我所看見的 AI，與一位工程師共舞著 | Build on Google AI |
| 8 | 53.47% | 17.16 | 50 | 1558 | 2914 | [DAY15 備援營運持續BIARTO/RPO/MTPD備份](https://ithelp.ithome.com.tw/articles/10418512) | 格鬥教練想轉職學資安是否搞錯了什麼(自學第一步)IPAS初級筆記 | Security |
| 9 | 53.36% | 4.75 | 8 | 898 | 1683 | [Day 04專家與開發者的盲點落差：你以為只防 SQLi/XSS，其實駭客玩的是這些！](https://ithelp.ithome.com.tw/articles/10404576) | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 10 | 50.57% | 3.26 | 15 | 2324 | 4596 | [Day 09動手做零成本建置資安檢測站：使用 Docker 5 分鐘架設 SonarQube](https://ithelp.ithome.com.tw/articles/10405628) | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 11 | 48.73% | 1.37 | 2 | 710 | 1457 | [Day 20生活感和小確幸](https://ithelp.ithome.com.tw/articles/10417340) | 台北女子，東京在住——AI 時代的海外工作與生活觀察 | 自我挑戰 |
| 12 | 48.14% | 11.32 | 21 | 893 | 1855 | [DAY03 Risk Assessment（風險評鑑）](https://ithelp.ithome.com.tw/articles/10412240) | 格鬥教練想轉職學資安是否搞錯了什麼(自學第一步)IPAS初級筆記 | Security |
| 13 | 48.08% | 3.49 | 7 | 965 | 2007 | [Day 9 - 訂製 system prompt](https://ithelp.ithome.com.tw/articles/10414569) | 手刻 AI Agent！大一新生的 Python 實戰筆記 | Software Development |
| 14 | 47.06% | 2.70 | 4 | 696 | 1479 | [Day 21 \| 用Vibe coding創造~不用呼叫語言模型，一樣能生出綜合評估!](https://ithelp.ithome.com.tw/articles/10404609) | 與Claude一起從零打造產值評估工具 | Vibe Coding |
| 15 | 47.04% | 2.01 | 2 | 469 | 997 | [Day 08｜Design system：tokentheme元件的邊界](https://ithelp.ithome.com.tw/articles/10415437) | 從 Fragment 到 Compose — 老 Android App 重寫的架構取捨 | Software Development |
| 16 | 46.86% | 7.54 | 9 | 559 | 1193 | [[Day 13] 用 pd.cut 特徵分箱拆解重量級別與延遲率](https://ithelp.ithome.com.tw/articles/10414246) | 拯救混亂數據：30 天 Python 輕量級 ETL 與電商關聯資料分析 | 自我挑戰 |
| 17 | 46.61% | 2.78 | 12 | 2012 | 4317 | [Day 08動手做在 IDE 裝上第一道防護網：SonarQube for IDE 安裝與即時掃描示範](https://ithelp.ithome.com.tw/articles/10405418) | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 18 | 45.73% | 10.21 | 31 | 1388 | 3035 | [DAY16 網路分層模型與網路設備](https://ithelp.ithome.com.tw/articles/10418567) | 格鬥教練想轉職學資安是否搞錯了什麼(自學第一步)IPAS初級筆記 | Security |
| 19 | 45.66% | 1.68 | 3 | 816 | 1787 | [Day 16 - 工作目錄的選擇](https://ithelp.ithome.com.tw/articles/10418328) | 手刻 AI Agent！大一新生的 Python 實戰筆記 | Software Development |
| 20 | 45.45% | 11.36 | 1 | 40 | 88 | [Day 24：體積雲實作 2 - Raymarching](https://ithelp.ithome.com.tw/articles/10416615) | 因為 AI 看不懂老舊程式，只好乖乖從零開始學 DirectX 12 與 HLSL | Software Development |

## ratio 最高的 20 個系列

| # | ratio% | 各篇中位數 | per_1k | 篇數 | bq_count | bq_chars | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 33.15% | 34.53% | 3.22 | 17 | 119 | 12249 | 36946 | 手刻 AI Agent！大一新生的 Python 實戰筆記 | Software Development |
| 2 | 32.06% | 36.02% | 0.37 | 24 | 26 | 22600 | 70501 | 凡人修 Agent 傳－我這一生如履薄冰，你說我的 Agent 能結嬰嗎？ | Vibe Coding |
| 3 | 30.35% | 29.18% | 2.37 | 31 | 148 | 18952 | 62437 | JS 核心重構：勇者轉職傳說 | JavaScript |
| 4 | 29.44% | 24.96% | 15.64 | 16 | 567 | 10670 | 36249 | 格鬥教練想轉職學資安是否搞錯了什麼(自學第一步)IPAS初級筆記 | Security |
| 5 | 26.25% | 33.19% | 9.34 | 5 | 120 | 3374 | 12854 | 30 天打造讓人敢簽核的 AI Agent：從會回答到可信任的審核型 AI | AI Engineering |
| 6 | 25.89% | 21.09% | 2.09 | 30 | 193 | 23871 | 92193 | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 7 | 22.84% | 22.85% | 1.91 | 31 | 273 | 32681 | 143071 | 從零到 CKA：30 天掌握 Kubernetes 核心觀念與實作 | Kubernetes |
| 8 | 22.77% | 22.77% | 4.11 | 1 | 13 | 720 | 3162 | Hack The Box 30 日修行：寫給初學者的滲透測試解題路線 | 自我挑戰 |
| 9 | 21.80% | 21.80% | 2.76 | 1 | 6 | 473 | 2170 | 兒時的遊戲圓夢之旅：一人美術、一人開發、一人陣亡 | 佛心分享-SideProject30 |
| 10 | 21.04% | 24.01% | 1.29 | 16 | 42 | 6860 | 32597 | 從第一線應變到企業治理：30 天打造資安溝通與營運韌性 | 自我挑戰 |
| 11 | 20.19% | 2.84% | 0.80 | 16 | 52 | 13192 | 65327 | 企業管理自動化與執行框架-以SOC運作為實例 | Security |
| 12 | 19.31% | 16.57% | 6.49 | 15 | 384 | 11429 | 59177 | 30 天從 Full-Stack Engineer 進化到 System Design：從 0 設計可支撐百萬使用者的系統 | Software Development |
| 13 | 18.92% | 20.06% | 2.32 | 15 | 113 | 9207 | 48657 | Build on Google AI ：長者照護 —— 口腔機能訓練 與 延緩認知退化 | Build on Google AI |
| 14 | 18.70% | 17.94% | 1.74 | 17 | 19 | 2044 | 10933 | 當 AI 加入團隊：打造可審查、可驗證、會自我改善的 AI 開發工作流 | AI Engineering |
| 15 | 18.51% | 18.49% | 8.44 | 15 | 312 | 6842 | 36959 | AI 改變產品設計的起點：產品經理的 30 個 AI Native 設計思考 | Software Development |
| 16 | 17.57% | 18.68% | 1.23 | 30 | 92 | 13117 | 74666 | 怎麼找到新東西？把 200 年的「發現」老方法教給 AI——方法圖鑑 × Claude Skills | Claude AI |
| 17 | 17.50% | 14.71% | 1.33 | 30 | 84 | 11040 | 63069 | NodeRED × Google agy CLI 打造個人 Windows 智慧管家 | AI 自動化 |
| 18 | 17.39% | 16.72% | 2.68 | 15 | 314 | 20338 | 116981 | 資安這條路：從攻擊者視角看 Kubernetes | Kubernetes |
| 19 | 17.29% | 15.67% | 5.51 | 15 | 171 | 5364 | 31024 | AI 時代的 Android Engineer：30 天打造我的 AI 開發工作流 | Software Development |
| 20 | 16.61% | 14.75% | 6.01 | 21 | 269 | 7432 | 44732 | 現在就學C# 與 ASP.NET Core | Modern Web |

> 沒 blockquote (bq_count = 0) 的文章 ratio = 0, 全部沉底 (不代表沒 AI 味).
> 這是共現訊號, 不是判決.
