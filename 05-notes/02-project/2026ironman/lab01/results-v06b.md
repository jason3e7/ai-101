# lab01 V06B 結果：綜合訊號分數 (B 總和 × N 總和型)

> 抓取時間 2026-09-30 02:38:08. 公式:
> B_total = em + emoji + strict + all + bq + hr
> N_sum   = Σ (Nᵢ where Bᵢ > 0)
>        = (4 if em) + (types if emoji) + (3 if strict) + (0 if all) + (1 if bq) + (1.5 if hr)
> base    = B_total × N_sum
> density = base / total_chars × 1000  (per_1k 型)
> N 2026-10-01 保守微調: strict 2→3, hr 1→1.5 (見 v06b-weight-tuning.md).
> V02 all 的 N=0 仍進 B_total 當放大器, 但單獨命中 N_sum=0 不計分.
> 跟 V06 (每個 signal 自己 B×N 加總) 比較見 lab01/v06-vs-v06b.md.

## 總覽

| 項目 | 數值 |
|:---|---:|
| 系列數 | 814 |
| 文章數 | 15057 |
| base 總和 | 2,197,603.0 |
| 全篇總字數 | 39,634,634 |
| 全體 density | 55.4465 |

## density 最高的 20 篇（不設字數門檻）

| # | density | base | B_total | N_sum | —— | emj(種) | strict | all | bq | hr | 總字 | 文章 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|
| 1 | 1141.26 | 19592.0 | 496 | 39.5 | 3 | 49(30) | 111 | 298 | 13 | 22 | 17167 | [使用gemini 準備 az-900 DAY 6Azure Advisor & Service Health：系統健康智慧顧問與跨雲監控](https://ithelp.ithome.com.tw/articles/10405969) | 使用gemini  準備 az-900 | Build on Google AI |
| 2 | 1100.59 | 4097.5 | 149 | 27.5 | 10 | 29(18) | 21 | 76 | 2 | 11 | 3723 | [Day 12: 你敢不敢承認你只優化了自己那一段？](https://ithelp.ithome.com.tw/articles/10401963) | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 3 | 1077.31 | 2473.5 | 97 | 25.5 | 2 | 31(16) | 8 | 39 | 7 | 10 | 2296 | [Day 02：清點魔法物資  變數宣告與作用域的生存法則](https://ithelp.ithome.com.tw/articles/10401261) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 4 | 1008.95 | 7605.5 | 287 | 26.5 | 4 | 51(17) | 78 | 137 | 5 | 12 | 7538 | [使用gemini  準備 az-900  DAY 1](https://ithelp.ithome.com.tw/articles/10404896) | 使用gemini  準備 az-900 | Build on Google AI |
| 5 | 1008.15 | 2597.0 | 106 | 24.5 | 3 | 29(15) | 13 | 45 | 7 | 9 | 2576 | [Day 03：極速短咒  箭頭函式與傳說中的隱形回傳](https://ithelp.ithome.com.tw/articles/10401425) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 6 | 1007.34 | 19837.5 | 575 | 34.5 | 20 | 101(25) | 170 | 248 | 15 | 21 | 19693 | [使用gemini 準備AZ-900 Day21  Phase 3複習](https://ithelp.ithome.com.tw/articles/10409878) | 使用gemini  準備 az-900 | Build on Google AI |
| 7 | 1003.13 | 2082.5 | 85 | 24.5 | 1 | 21(15) | 12 | 37 | 5 | 9 | 2076 | [Day 27：戰略指揮資料驅動 (State-driven)：從動手肌肉，到下達腦袋指令](https://ithelp.ithome.com.tw/articles/10405589) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 8 | 969.09 | 2116.5 | 83 | 25.5 | 1 | 24(16) | 5 | 42 | 4 | 7 | 2184 | [我想像中的未來小豬](https://ithelp.ithome.com.tw/articles/10414681) | 前端三分鐘 X 要轉職養豬還是做被取代的工程師？用 Google AI 打造我的 AI 雙刀流自動化工作流 | Build on Google AI |
| 9 | 963.29 | 2033.5 | 83 | 24.5 | 1 | 21(15) | 11 | 36 | 5 | 9 | 2111 | [Day 26：效能神兵防抖 (Debounce) 與節流 (Throttle) 的冷卻機制](https://ithelp.ithome.com.tw/articles/10405386) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 10 | 938.18 | 1373.5 | 67 | 20.5 | 0 | 22(15) | 13 | 27 | 2 | 3 | 1464 | [驗證使用  [ AI 助教 ]  後的結果](https://ithelp.ithome.com.tw/articles/10407800) | 將考國際證照的應用程式變成開源 | Build on Google AI |
| 11 | 930.82 | 1742.5 | 85 | 20.5 | 0 | 23(15) | 12 | 36 | 5 | 9 | 1872 | [Day 20：真相Promise：給未來的一個承諾，解決你的回呼地獄](https://ithelp.ithome.com.tw/articles/10404251) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 12 | 925.24 | 16758.0 | 588 | 28.5 | 5 | 54(19) | 167 | 336 | 6 | 20 | 18112 | [使用gemini 準備 az-900  Day 4  Azure Portal / CLI / PowerShell / Cloud Shell（管理工具選型對照）](https://ithelp.ithome.com.tw/articles/10405551) | 使用gemini  準備 az-900 | Build on Google AI |
| 13 | 922.18 | 4615.5 | 181 | 25.5 | 1 | 23(16) | 38 | 106 | 5 | 8 | 5005 | [Day 10: 你敢不敢承認，更努力救火只會更慘？](https://ithelp.ithome.com.tw/articles/10401961) | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 14 | 897.10 | 2040.0 | 80 | 25.5 | 2 | 22(16) | 8 | 33 | 5 | 10 | 2274 | [Day 09：原理篇  陣列大戰：從傳統 for 到自動化生產線 map / filter](https://ithelp.ithome.com.tw/articles/10402299) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 15 | 880.65 | 2180.5 | 89 | 24.5 | 2 | 21(15) | 8 | 42 | 7 | 9 | 2476 | [Day 06：原理篇  影印術！展開運算子 ...：破解傳址詛咒的終極神技](https://ithelp.ithome.com.tw/articles/10401883) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 16 | 878.95 | 2200.0 | 80 | 27.5 | 6 | 21(18) | 4 | 38 | 2 | 9 | 2503 | [Day 1兩張截圖引發的血案](https://ithelp.ithome.com.tw/articles/10406906) | [自學筆記] 還在路上！我的 IPAS 資訊安全工程師初級備考紀錄 | Security |
| 17 | 871.15 | 9006.0 | 316 | 28.5 | 5 | 31(19) | 67 | 195 | 3 | 15 | 10338 | [使用gemini 準備 az-900 Day 5ARM Templates & Bicep：基礎架構即程式碼 (IaC) 的精髓 feat. AWS 雙強對照](https://ithelp.ithome.com.tw/articles/10405735) | 使用gemini  準備 az-900 | Build on Google AI |
| 18 | 859.80 | 14694.0 | 372 | 39.5 | 2 | 73(30) | 38 | 195 | 43 | 21 | 17090 | [使用gemini 準備AZ-900 Day28 Az-900 特有題型破解：拖曳 下拉 與 熱點拆解](https://ithelp.ithome.com.tw/articles/10414128) | 使用gemini  準備 az-900 | Build on Google AI |
| 19 | 847.13 | 1862.0 | 76 | 24.5 | 2 | 21(15) | 7 | 31 | 5 | 10 | 2198 | [Day 07：原理篇  記憶膠囊：閉包與守護結界](https://ithelp.ithome.com.tw/articles/10402070) | JS 核心重構：勇者轉職傳說 | JavaScript |
| 20 | 840.00 | 21321.0 | 618 | 34.5 | 30 | 43(25) | 86 | 416 | 27 | 16 | 25382 | [使用gemini 準備 az-900 Day 8 Virtual Machines & VM Scale Sets：控制權擴展維度與 SLA 階梯的三角權衡](https://ithelp.ithome.com.tw/articles/10406404) | 使用gemini  準備 az-900 | Build on Google AI |

## density 最高的 20 個系列

| # | density | 各篇中位數 | 篇數 | —— | emoji | strict | all | bq | hr | base | 總字 | 系列 | 組別 |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|
| 1 | 765.79 | 727.39 | 31 | 17 | 626 | 238 | 970 | 148 | 292 | 47813.5 | 62437 | JS 核心重構：勇者轉職傳說 | JavaScript |
| 2 | 656.47 | 700.31 | 30 | 132 | 1234 | 2581 | 5832 | 318 | 523 | 271711.5 | 413895 | 使用gemini  準備 az-900 | Build on Google AI |
| 3 | 489.14 | 420.74 | 30 | 167 | 428 | 429 | 1510 | 80 | 257 | 57270.5 | 117083 | Phoenix 2026：當《鳳凰專案》遇上 AI Agent —— 30 天 DevOps 職場 RPG 冒險 | Software Development |
| 4 | 487.02 | 496.28 | 30 | 0 | 545 | 149 | 1391 | 193 | 239 | 44899.5 | 92193 | 槍林彈雨下的資安防守：從品質觀念切入，帶開發者從零動手作資安 30 天 | Security |
| 5 | 408.43 | 397.02 | 29 | 326 | 317 | 173 | 1900 | 57 | 302 | 41803.5 | 102352 | OpenShift AI 簡易入門30天 | AI Engineering |
| 6 | 404.19 | 370.10 | 17 | 36 | 119 | 194 | 584 | 14 | 106 | 15676.5 | 38785 | 30 天 從數據思維到自動化稽核實戰 | 佛心分享-IT 人自學之術 |
| 7 | 326.97 | 322.79 | 19 | 182 | 87 | 127 | 885 | 70 | 37 | 18906.0 | 57821 | RE: 從 4,343 筆職缺到 AI Engineer：MLOps × GenAI Engineering 雙主軸實戰 | AI Engineering |
| 8 | 315.57 | 322.90 | 16 | 16 | 120 | 136 | 355 | 3 | 92 | 10978.0 | 34788 | 用 Google AI 生態系 30 天從零打造一個全棧 AI SaaS 服務 | Build on Google AI |
| 9 | 302.32 | 271.17 | 6 | 50 | 72 | 115 | 302 | 24 | 49 | 8615.0 | 28496 | [自學筆記] 還在路上！我的 IPAS 資訊安全工程師初級備考紀錄 | Security |
| 10 | 300.44 | 233.58 | 30 | 15 | 251 | 287 | 704 | 84 | 248 | 18948.5 | 63069 | NodeRED × Google agy CLI 打造個人 Windows 智慧管家 | AI 自動化 |
| 11 | 289.07 | 261.56 | 15 | 66 | 148 | 293 | 671 | 80 | 105 | 25534.5 | 88333 | 30 天的 SAA 學習筆記 | 自我挑戰 |
| 12 | 275.30 | 279.44 | 14 | 283 | 13 | 68 | 763 | 72 | 124 | 12813.5 | 46543 | 它說得頭頭是道 | AI Engineering |
| 13 | 271.33 | 129.18 | 40 | 1 | 421 | 563 | 1314 | 25 | 218 | 41757.0 | 153896 | 將考國際證照的應用程式變成開源 | Build on Google AI |
| 14 | 250.03 | 202.40 | 30 | 33 | 195 | 184 | 691 | 64 | 191 | 15127.0 | 60500 | 前端三分鐘 X 要轉職養豬還是做被取代的工程師？用 Google AI 打造我的 AI 雙刀流自動化工作流 | Build on Google AI |
| 15 | 232.62 | 212.96 | 9 | 60 | 26 | 40 | 172 | 21 | 9 | 3865.0 | 16615 | 我用 AI 養出一個 AWS 維運同事：從查帳單到進機房的 30 天 | IT Operation |
| 16 | 228.24 | 233.62 | 31 | 135 | 278 | 269 | 1590 | 273 | 257 | 32654.0 | 143071 | 從零到 CKA：30 天掌握 Kubernetes 核心觀念與實作 | Kubernetes |
| 17 | 227.86 | 241.90 | 19 | 319 | 2 | 125 | 1066 | 64 | 189 | 16080.5 | 70571 | 《再叩一次》—一個中年轉職叩門者的 OSCP 三十夜 | Security |
| 18 | 222.68 | 251.78 | 8 | 75 | 0 | 53 | 194 | 12 | 70 | 3700.0 | 16616 | 用 Claude Code 打造 AI Agent 艦隊：從零到生產級多 Agent 系統 | Claude AI |
| 19 | 221.78 | 213.18 | 15 | 299 | 0 | 91 | 357 | 15 | 15 | 7321.5 | 33012 | Re:從零開始做直播代購電商平台 | Software Development |
| 20 | 218.44 | 213.12 | 20 | 116 | 47 | 61 | 464 | 32 | 31 | 7339.5 | 33600 | 《30 天從零打造資安語言模型：從微調到 Agent 落地》 | AI Security |

> 這是共現訊號的加權排序, 不是判決.
