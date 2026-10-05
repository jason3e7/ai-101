---
title: "AI 101 - 鐵人賽 Day 21: 回到主場 — 程式的產出, 好驗多了"
tags: [ai, 鐵人賽, ironman, 驗證, 程式, 可驗證性, 轉場, 草稿]
created: 2026-10-04
status: draft
---

# Day 21｜回到可驗證的主場 — Back to Verifiable Ground

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> 過去十天 (Day 11-20) 花了很多力氣, 都在跟一個難題纏鬥: **AI 的文字產出很難驗.** 怎麼知道答案對? 怎麼看出是不是 AI 寫的? 浮水印能當證據嗎? ⋯⋯ 繞了一圈, 結論幾乎都是「沒有乾淨的判准」. 這篇是個轉場: 接下來幾天回到**比較好驗的主場 —— 程式的產出.** 程式有個文字沒有的東西: 你可以**跑跑看**. 這篇講為什麼這件事讓一切變簡單, 以及它的邊界在哪; 中途用一個現成的判准 —— online judge —— 當活例子。

> **TL;DR (EN):** The last ten days (Day 11-20) fought one hard problem: AI's **text** output is hard to verify — you can't prove an answer is right, can't reliably tell if it's AI-written, and watermarks aren't evidence. This post pivots back to friendlier ground: **code**. Code has something prose doesn't — an **automatic verifier** (compiler, runtime, tests, diff). Wrong code mostly announces itself (a red test, an exception), while wrong prose just sits there looking fine. That makes verification cheap, objective, automatable, and repeatable. The catch: a green build means "the checks I wrote passed", not "it's correct" — tests show the presence of bugs, never their absence, and "it runs" isn't "it does the right thing". That gap is exactly what the next few days are about.

```markdown
# 文字難驗, 程式好驗: 回到有判准的主場
* 過去十天在搏鬥什麼
    * 文字沒有客觀判准
* 程式不一樣: 有判准
    * 能編譯、能跑、能測、能 diff
    * 錯誤會自己冒出來
    * 快、客觀、可自動化、可重現
    * online judge 是現成判准 丟上去就判
* 可驗證光譜
    * 文字: 要人讀、主觀、慢
    * 程式: 有判准、可自動、秒級回饋
* 綠燈不等於對
    * 測試只驗你想到的 case
    * 能跑 ≠ 做對的東西
    * 安全與效能不會自己變紅
* 接下來幾天
    * 把可驗證的終點交給 AI
    * 管它能動哪、守住紅線
```

---

## 過去十天在搏鬥什麼 — The Hard-to-Verify Detour

一句話收: **文字沒有客觀判准.** 一段話對不對, 要人讀、要查證、要主觀判斷; 是不是 AI 寫的, 頂多給你機率、給不了鐵證. 累的根源在這 —— 人就是唯一的 bottleneck.

原話 (jason3e7):

> 過去十天花了很多時間, 探討了比較難驗證的文字產出; 接下來要回到比較容易驗證的主場 —— 程式的產出。

---

## 回到主場: 程式可以「跑跑看」 — Back on Home Turf

程式跟文字最大的差別, 一句話: **程式有判准.**

判准指「一個能自動告訴你答案對不對的東西」。文字幾乎沒有這種東西 —— 沒有「編譯器」會告訴你這段話寫錯了。但程式一大堆:

- **編譯器 / 型別檢查**: 拼錯、型別不合, 直接不給過
- **執行**: 跑一下就知道會不會爆、輸出對不對
- **測試**: 寫好的 test 自動判對錯, 紅燈就是錯
- **diff / linter / 靜態分析**: 跟預期比、照規則掃

關鍵在這句: **錯的程式大多會自己冒出來, 錯的文字只會靜靜躺著。** 一個 exception、一條紅色的 CI, 是程式在對你喊「這裡不對」; 一段邏輯怪怪的文字不會喊, 它看起來一樣通順。

這帶來四個文字給不了的好處:

- **客觀**: 對就是對, 不用吵「我覺得」
- **快**: 幾秒內知道結果, 不用讀完再思考
- **可自動化**: CI 幫你跑一千次, 不用人每次重讀
- **可重現**: 同樣 input 同樣結果, 驗證本身也能被驗證

對「驗 AI 產出」這件事, 這是天大的好消息。前十天人是唯一的 bottleneck; 回到程式, 機器可以幫你擋掉一大半。

### 一個現成的判准: online judge — A Ready-Made Verifier

最乾淨的例子是 **online judge** (線上解題系統, 像 [ZeroJudge](https://zerojudge.tw/))。你把程式丟上去, 它拿一堆**藏起來的測資**跑你的 code, 回一個**判決**: AC (通過)、WA (答案錯)、CE (編譯錯)、TLE (超時)⋯ 這不是「我覺得不錯」, 是一個客觀、秒級、可重複的結果 —— 判准長這樣最清楚。

我實際接了一條全自動的小迴圈來玩這件事 (用 Playwright 操作瀏覽器送題, 過程另寫在 [這篇筆記](../../../zerojudge/playwright-zerojudge-automation.md)):

```
寫 .cpp  →  本機 g++ 編譯、跑範例  →  Playwright 自動上傳  →  判題回 AC/WA/CE/TLE  →  不過就讀訊息改, 再送
```

結果: a001、a002、a003 **三題都第一次送就 AC**。這不是運氣, 是因為**上傳前先在本機用同一版 g++ (`-std=c++17`) 把範例跑過** —— 等於先對著判准自測一輪, 再交出去。這就是「有判准」的威力: 你不用猜對不對, 跑一下就知道; 連送出、讀結果都能讓機器代勞。

換成文字呢? 一篇文章沒有「判題系統」會回你 AC 還是 WA。這就是主場跟客場的差別。

---

## 可驗證光譜: 文字 vs 程式 — The Verifiability Spectrum

驗證難不難, 不是是非題, 是一條光譜。把文字跟程式擺一起看最清楚:

| 面向 | 文字產出 | 程式產出 |
|:---|:---|:---|
| 有沒有客觀判准 | 幾乎沒有, 靠人讀 | 有: 編譯、執行、測試 |
| 錯了會不會自己冒出來 | 不會, 靜靜躺著 | 大多會: 報錯、紅燈 |
| 能不能自動化 | 很難 | 可以, CI 一鍵跑 |
| 回饋多快 | 慢 (要讀、要查) | 快 (秒級) |
| 可不可重現 | 每次人讀結果可能不同 | 同 input 同結果 |
| 能不能切小塊各別驗 | 難, 對錯是整體的 | 可以, 一個函式一個測試 |

> [!NOTE]
> 不是說程式「一定好驗」、文字「一定沒救」。有些程式超難驗 (並行、分散式、浮點數、UI), 有些文字有明確事實可查 (數字、日期、引用來源)。重點是**平均而言程式靠光譜「好驗」那端近得多** —— 而且它好驗的理由 (有判准、錯誤會自曝) 剛好是文字最缺的。

---

## 綠燈不等於對 — Green Doesn't Mean Correct

回到主場不代表從此輕鬆。程式的判准很強, 但有邊界, 不認清這點會踩更大的坑 —— 因為它給你一種「有在驗」的安全感。

> Program testing can be used to show the presence of bugs, but never to show their absence.
> — Edsger W. Dijkstra

這句話是整段的核心。幾個具體的陷阱:

- **測試只驗你想到的 case。** 你沒想到的那個 edge case, 照樣安靜地爆。綠燈的意思是「我寫的那些檢查過了」, 不是「全對」。
- **能編譯、能跑 ≠ 做對的東西。** 這是驗證 (verification, 把東西做對) 跟確認 (validation, 做對的東西) 的差別。AI 很會把一個**錯的需求**實作得又乾淨又能跑。
- **有些錯不會讓燈變紅。** 安全漏洞、效能退化、資料外洩, 常常測試全綠照樣存在 —— 因為沒人寫測試去抓它。
- **AI 會「幫你」讓燈變綠。** 叫它修到測試過, 它可能改測試、而不是改程式。判准被它繞過去了。

> [!NOTE]
> 上面那個 online judge 其實是個**比自測更強**的判准 —— 它有出題者寫的隱藏測資, 會抓到你自己沒想到的 case (WA 就是這樣冒出來的)。但那是解題題目的奢侈: 真實世界的程式**很少附一套完整判題**, 多數時候 判准得你自己建 (寫測試) —— 一旦自己建, 又回到「只驗你想到的」這條限制。judge 幫你驗的是「答案對不對」, 驗不了「這題該不該這樣解」。

所以主場不是「不用驗了」, 是「**驗得動了, 但要補上機器驗不到的那幾關**」: 你想要的終點是什麼、哪裡是不能踩的紅線、誰來看 AI 到底動了什麼。

---

## Sources

- [AI 產出怎麼驗](../../../../02-advanced/limits-and-verification/verifying-ai-output.md) — 前十天那條驗證線的方法論底本
- [AI 用久了會鈍化](../../../../02-advanced/limits-and-verification/ai-atrophy.md) — 為什麼「人當 bottleneck」會累、會鈍 (驗證疲勞)
- [先收整再展開, 人才驗得動](../../../../02-advanced/limits-and-verification/converge-before-verify.md) — 把難驗的長輸出變得驗得動的手法
- [用 Playwright 自動操作 ZeroJudge](../../../zerojudge/playwright-zerojudge-automation.md) — 本篇 online judge 活例子的實作 (自動送題、讀判題結果)
- [ZeroJudge](https://zerojudge.tw/) — 文中用的線上解題系統 (現成的判准)
- [E. W. Dijkstra, "The Humble Programmer" (1972) — "testing shows the presence, not the absence of bugs"](https://www.cs.utexas.edu/~EWD/transcriptions/EWD03xx/EWD340.html)
- [Verification vs Validation — 把東西做對 vs 做對的東西 (Barry Boehm)](https://en.wikipedia.org/wiki/Software_verification_and_validation)
