# lab01 V06B 結果：綜合訊號分數 (B 總和 × N 總和型)

> 抓取時間 2026-09-30 02:38:08. 公式:
> B_total = em + emoji + strict + all + bq + hr
> N_sum   = Σ (Nᵢ where Bᵢ > 0)
> base    = B_total × N_sum
> density = base / total_chars × 1000  (per_1k 型)
> V02 all 的 N=0 仍進 B_total 當放大器, 但單獨命中 N_sum=0 不計分.
> 跟 V06 (每個 signal 自己 B×N 加總) 比較見 lab01/v06-vs-v06b.md.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| base 總和 | 1,921,010 |
| 全篇總字數 | 39,634,634 |
| 全體 density | 48.4680 |

## density 最高的 20 篇（不設字數門檻）

| # | density | base | B_total | N_sum | —— | emj(種) | strict | all | bq | hr | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 1097.92 | 18848 | 496 | 38 | 3 | 49(30) | 111 | 298 | 13 | 22 | 17167 | [使用gemini 準備 az-900 DAY 6Azure Advisor & Service Health：系統健康智慧顧問與跨雲監控](https://ithelp.ithome.com.tw/articles/10405969) | 使用gemini  準備 az-900 | Build on Google AI |
| 2 | 1040.56 | 3874 | 149 | 26 | 10 | 29(18) | 21 | 76 | 2 | 11 | 3723 | [Day 12: 你敢不敢承認你只優化了自己那一段？](https://ithelp.ithome.com.tw/articles/10401963) | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 3 | 1013.94 | 2328 | 97 | 24 | 2 | 31(16) | 8 | 39 | 7 | 10 | 2296 | [Day 02：清點魔法物資  變數宣告與作用域的生存法則](https://ithelp.ithome.com.tw/articles/10401261) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 4 | 963.54 | 18975 | 575 | 33 | 20 | 101(25) | 170 | 248 | 15 | 21 | 19693 | [使用gemini 準備AZ-900 Day21  Phase 3複習](https://ithelp.ithome.com.tw/articles/10409878) | 使用gemini  準備 az-900 | Build on Google AI |
| 5 | 951.84 | 7175 | 287 | 25 | 4 | 51(17) | 78 | 137 | 5 | 12 | 7538 | [使用gemini  準備 az-900  DAY 1](https://ithelp.ithome.com.tw/articles/10404896) | 使用gemini  準備 az-900 | Build on Google AI |
| 6 | 946.43 | 2438 | 106 | 23 | 3 | 29(15) | 13 | 45 | 7 | 9 | 2576 | [Day 03：極速短咒  箭頭函式與傳說中的隱形回傳](https://ithelp.ithome.com.tw/articles/10401425) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 7 | 941.71 | 1955 | 85 | 23 | 1 | 21(15) | 12 | 37 | 5 | 9 | 2076 | [Day 27：戰略指揮資料驅動 (State-driven)：從動手肌肉，到下達腦袋指令](https://ithelp.ithome.com.tw/articles/10405589) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 8 | 912.09 | 1992 | 83 | 24 | 1 | 24(16) | 5 | 42 | 4 | 7 | 2184 | [我想像中的未來小豬](https://ithelp.ithome.com.tw/articles/10414681) | 前端三分鐘 X 要轉職養豬還是做被取代的工程師？用 Google AI 打造我的 AI 雙刀流自動化工作流 | Build on Google AI |
| 9 | 904.31 | 1909 | 83 | 23 | 1 | 21(15) | 11 | 36 | 5 | 9 | 2111 | [Day 26：效能神兵防抖 (Debounce) 與節流 (Throttle) 的冷卻機制](https://ithelp.ithome.com.tw/articles/10405386) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 10 | 876.55 | 15876 | 588 | 27 | 5 | 54(19) | 167 | 336 | 6 | 20 | 18112 | [使用gemini 準備 az-900  Day 4  Azure Portal / CLI / PowerShell / Cloud Shell（管理工具選型對照）](https://ithelp.ithome.com.tw/articles/10405551) | 使用gemini  準備 az-900 | Build on Google AI |
| 11 | 869.54 | 1273 | 67 | 19 | 0 | 22(15) | 13 | 27 | 2 | 3 | 1464 | [驗證使用  [ AI 助教 ]  後的結果](https://ithelp.ithome.com.tw/articles/10407800) | 將考國際證照的應用程式變成開源 | Build on Google AI |
| 12 | 867.93 | 4344 | 181 | 24 | 1 | 23(16) | 38 | 106 | 5 | 8 | 5005 | [Day 10: 你敢不敢承認，更努力救火只會更慘？](https://ithelp.ithome.com.tw/articles/10401961) | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 13 | 862.71 | 1615 | 85 | 19 | 0 | 23(15) | 12 | 36 | 5 | 9 | 1872 | [Day 20：真相Promise：給未來的一個承諾，解決你的回呼地獄](https://ithelp.ithome.com.tw/articles/10404251) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 14 | 844.33 | 1920 | 80 | 24 | 2 | 22(16) | 8 | 33 | 5 | 10 | 2274 | [Day 09：原理篇  陣列大戰：從傳統 for 到自動化生產線 map / filter](https://ithelp.ithome.com.tw/articles/10402299) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 15 | 831.00 | 2080 | 80 | 26 | 6 | 21(18) | 4 | 38 | 2 | 9 | 2503 | [Day 1兩張截圖引發的血案](https://ithelp.ithome.com.tw/articles/10406906) | [自學筆記] 還在路上！我的 IPAS 資訊安全工程師初級備考紀錄 | Security |
| 16 | 827.15 | 14136 | 372 | 38 | 2 | 73(30) | 38 | 195 | 43 | 21 | 17090 | [使用gemini 準備AZ-900 Day28 Az-900 特有題型破解：拖曳 下拉 與 熱點拆解](https://ithelp.ithome.com.tw/articles/10414128) | 使用gemini  準備 az-900 | Build on Google AI |
| 17 | 826.74 | 2047 | 89 | 23 | 2 | 21(15) | 8 | 42 | 7 | 9 | 2476 | [Day 06：原理篇  影印術！展開運算子 ...：破解傳址詛咒的終極神技](https://ithelp.ithome.com.tw/articles/10401883) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 18 | 825.30 | 8532 | 316 | 27 | 5 | 31(19) | 67 | 195 | 3 | 15 | 10338 | [使用gemini 準備 az-900 Day 5ARM Templates & Bicep：基礎架構即程式碼 (IaC) 的精髓 feat. AWS 雙強對照](https://ithelp.ithome.com.tw/articles/10405735) | 使用gemini  準備 az-900 | Build on Google AI |
| 19 | 803.48 | 20394 | 618 | 33 | 30 | 43(25) | 86 | 416 | 27 | 16 | 25382 | [使用gemini 準備 az-900 Day 8 Virtual Machines & VM Scale Sets：控制權擴展維度與 SLA 階梯的三角權衡](https://ithelp.ithome.com.tw/articles/10406404) | 使用gemini  準備 az-900 | Build on Google AI |
| 20 | 795.27 | 1748 | 76 | 23 | 2 | 21(15) | 7 | 31 | 5 | 10 | 2198 | [Day 07：原理篇  記憶膠囊：閉包與守護結界](https://ithelp.ithome.com.tw/articles/10402070) | JS 核心重構：勇者轉職傳說 | JavaScript |

## density 最高的 20 個系列

| # | density | 各篇中位數 | 篇數 | —— | emoji | strict | all | bq | hr | base | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 710.75 | 674.16 | 31 | 17 | 626 | 238 | 970 | 148 | 292 | 44377 | 62437 | JS 核心重構：勇者轉職傳說 | JavaScript |
| 2 | 619.20 | 657.92 | 30 | 132 | 1234 | 2581 | 5832 | 318 | 523 | 256285 | 413895 | 使用gemini  準備 az-900 | Build on Google AI |
| 3 | 452.36 | 388.35 | 30 | 167 | 428 | 429 | 1510 | 80 | 257 | 52964 | 117083 | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 4 | 449.89 | 455.22 | 30 | 0 | 545 | 149 | 1391 | 193 | 239 | 41477 | 92193 | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 5 | 363.47 | 328.98 | 17 | 36 | 119 | 194 | 584 | 14 | 106 | 14097 | 38785 | 30 天 從數據思維到自動化稽核實戰 | 佛心分享-IT 人自學之術 |
| 6 | 363.36 | 349.38 | 29 | 326 | 317 | 173 | 1900 | 57 | 302 | 37191 | 102352 | OpenShift AI 簡易入門30天 | AI Engineering |
| 7 | 290.97 | 287.57 | 19 | 182 | 87 | 127 | 885 | 70 | 37 | 16824 | 57821 | RE: 從 4,343 筆職缺到 AI Engineer：MLOps × GenAI Engineering 雙主軸實戰 | AI Engineering |
| 8 | 284.44 | 291.50 | 16 | 16 | 120 | 136 | 355 | 3 | 92 | 9895 | 34788 | 用 Google AI 生態系 30 天從零打造一個全棧 AI SaaS 服務 | Build on Google AI |
| 9 | 272.63 | 237.43 | 6 | 50 | 72 | 115 | 302 | 24 | 49 | 7769 | 28496 | [自學筆記] 還在路上！我的 IPAS 資訊安全工程師初級備考紀錄 | Security |
| 10 | 265.99 | 237.78 | 15 | 66 | 148 | 293 | 671 | 80 | 105 | 23496 | 88333 | 30 天的 SAA 學習筆記 | 自我挑戰 |
| 11 | 262.95 | 194.68 | 30 | 15 | 251 | 287 | 704 | 84 | 248 | 16584 | 63069 | NodeRED × Google agy CLI 打造個人 Windows 智慧管家 | AI 自動化 |
| 12 | 247.45 | 106.06 | 40 | 1 | 421 | 563 | 1314 | 25 | 218 | 38082 | 153896 | 將考國際證照的應用程式變成開源 | Build on Google AI |
| 13 | 234.26 | 235.32 | 14 | 283 | 13 | 68 | 763 | 72 | 124 | 10903 | 46543 | 它說得頭頭是道 | AI Engineering |
| 14 | 217.83 | 171.93 | 30 | 33 | 195 | 184 | 691 | 64 | 191 | 13179 | 60500 | 前端三分鐘 X 要轉職養豬還是做被取代的工程師？用 Google AI 打造我的 AI 雙刀流自動化工作流 | Build on Google AI |
| 15 | 203.01 | 185.19 | 9 | 60 | 26 | 40 | 172 | 21 | 9 | 3373 | 16615 | 我用 AI 養出一個 AWS 維運同事：從查帳單到進機房的 30 天 | IT Operation |
| 16 | 199.94 | 205.95 | 31 | 135 | 278 | 269 | 1590 | 273 | 257 | 28606 | 143071 | 從零到 CKA：30 天掌握 Kubernetes 核心觀念與實作 | Kubernetes |
| 17 | 193.07 | 203.70 | 19 | 319 | 2 | 125 | 1066 | 64 | 189 | 13625 | 70571 | 《再叩一次》—一個中年轉職叩門者的 OSCP 三十夜 | Security |
| 18 | 192.97 | 182.57 | 31 | 819 | 80 | 131 | 1339 | 72 | 149 | 24665 | 127817 | 生活中的 AI 應用：我在家用 NAS 養了一隻 Agent，幫我看盤、顧家、盯備考——30 天自架實錄 | AI Engineering |
| 19 | 190.80 | 179.90 | 20 | 116 | 47 | 61 | 464 | 32 | 31 | 6411 | 33600 | 《30 天從零打造資安語言模型：從微調到 Agent 落地》 | AI Security |
| 20 | 188.13 | 212.03 | 8 | 75 | 0 | 53 | 194 | 12 | 70 | 3126 | 16616 | 用 Claude Code 打造 AI Agent 艦隊：從零到生產級多 Agent 系統 | Claude AI |

> 這是共現訊號的加權排序, 不是判決.
