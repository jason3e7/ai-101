---
title: "AI 101 - 鐵人賽 Day 18: 翻面問題 — 哪些文章是純手寫, 完全沒碰 AI 的?"
tags: [ai, 鐵人賽, ironman, 文風檢測, 翻面問題, 純手寫, V12, tampermonkey, lab01-收尾, 草稿]
created: 2026-10-01
status: draft
---

# Day 18｜翻面問題 — 哪些文章是純手寫, 完全沒碰 AI 的? — Can We Detect Pure Human Writing?

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> lab01 文風檢測三部曲收尾. [Day 16](./day16-ai-fingerprint-scan.md) 用單指紋 (`——`) 掃全屆, [Day 17](./day17-composite-score.md) 疊到 6 個 signal 的綜合分數, 這 15 天我又把它養到了 **V12 (9 個 signal ＋ 權重微調)**. 這篇拿這把最細的尺去問一個**翻面**的問題: 不問「誰像 AI」, 改問「誰是純手寫、完全沒碰 AI」. 結論先講: **這把尺答不出來** —— 而且為什麼答不出來, 比它答得出什麼更值得講. 文末附一個 Tampermonkey script, 讓你瀏覽任何網頁時即時看到 N_sum 與 density.

> **TL;DR (EN):** The series closer. After refining the AI-tell detector all the way to V12 (9 signals, tuned weights, overall density 69.66 across all 15,057 articles), I flip the question: can it identify writing that is *purely human*, with no AI involved? It cannot - and the reason is epistemological, not a tuning problem. Zero signals can mean "no AI" or "AI used but cleaned up" (the Day 16 ChatGPT track scored 0.11, essentially invisible, because GPT-5.1 honors "no em-dash"). Proving presence is easy; proving absence is not. The detector gives a reliable *ranking* of AI-flavor, never a verdict of human authorship. Ships with a Tampermonkey userscript that runs the exact V12 scoring in your browser and shows N_sum + density per page.

```markdown
# 把尺養到 V12, 卻答不出「誰是純手寫」
* 為什麼講這個
    * jason3e7 翻面提問: 能不能認出純手寫的
    * 三部曲收尾 Day16 單指紋 Day17 六訊號 今天 V12
* 偵測器養到 V12
    * 9 個訊號 B 總和 乘 N 總和
    * 權重: em4 emoji5 strict3 v08c3 v093 v11 2
    * 全屆 density 69.66
* 翻面行不行
    * 零命中 ≠ 人寫
    * 可能沒用 AI 也可能壓乾淨
    * ChatGPT 組 0.11 就是壓乾淨的例子
    * 證明有 比 證明沒有 容易
* 自己動手玩
    * Tampermonkey 即時算 N_sum density
    * iThome 自動掃 其他頁選單觸發
* 能回答什麼 不能回答什麼
    * 高分 排序有信度
    * 低分 推不出人寫
```

---

## 為什麼講這個 — Why This Matters

原話 (jason3e7):

> 做了這個研究之後, 我反而好奇有那些文章是純手寫, 完全沒有用 AI 的, 是不是也有辦法辨識呢?

這是個**翻面問題**. [Day 16](./day16-ai-fingerprint-scan.md) 到 [Day 17](./day17-composite-score.md), 我都在用 lab01 的 signal 抓「像 AI 的」. 這 15 天裡我把這套工具一路疊上去, 疊到了 V12 (9 個 signal). 既然尺做到這麼細了, 很自然會想: 反過來用, 能不能圈出「這篇是純人寫的」?

直覺上你會想: signal 全部零命中, 不就是人寫的嗎? 但這條反推**不成立**. 這篇就是在講它為什麼不成立 —— 這不是 V12 還不夠好, 是這個問題本質上就偏心, 再怎麼調權重都補不起來.

---

## 偵測器一路養到 V12 — The Detector, Now at V12

先把這把尺現在長什麼樣交代清楚. Day 17 的綜合分數是 6 個 signal, 後來陸續補到 **9 個**, 公式沒變 —— `base = B 總和 × N 總和`, 再除以字數乘一千得 density (每千字).

| signal | 抓什麼 | B (數量) | N (權重) |
|:---|:---|:---|---:|
| em `——` | 雙破折號 | 出現次數 | 4 |
| emoji | 表情符號 (扣掉 ○✗★☆☐) | 字元數 | min(種類, 5) |
| strict | `<li>` 開頭的 **粗體標籤**： | 符合數 | 3 |
| all | `<strong>` 總數 | 粗體數 | 0（只當放大器） |
| bq | `<blockquote>` 引用區塊 | 數量 | 1 |
| hr | `<hr>` 分隔線 | 數量 | 1.5 |
| **v08c** | 「不是⋯而是／更是」對立句 | 符合數 | 3 |
| **v09** | 「最容易⋯的」教學警告腔 | 符合數 | 3 |
| **v11** | 標題含全形 `｜` | 有/無 | 2 |

> [!NOTE]
> 粗體那三個新 signal (v08c / v09 / v11) 的權重是這兩天才用全站資料校過的: 排版性的 `｜` 標題 (v11) 從 5 **降到 2**, 文字層的「不是⋯而是」(v08c)、「最容易⋯的」(v09) 各升到 3, 讓「文字層的 AI 味」主導分數, 而不是「排版花俏」. 校的過程見 [v12-weight-tuning](../../lab01/v12-weight-tuning.md).

重跑全站 15,057 篇、814 系列, **V12 全體 density 是 69.66**. 這個數字是後面排名的基準 —— 數字越高, 相對全屆越偏「AI 味濃」. (注: 文末那個瀏覽器工具是**簡化版**, 只取 6 個排版訊號, 對照基準另為 49.4, 不是這裡的 69.66.)

這把尺拿來**排序** AI 味, 到 V12 已經相當穩. 問題來了: 它能不能反過來認「人」?

---

## 翻面行不行: 零命中推不出「人寫」 — Why the Reverse Fails

不行. 關鍵在一句話: **零命中可能是「沒用 AI」, 也可能是「用了 AI, 但把痕跡壓乾淨了」.** 這兩種在 V12 眼裡長得一模一樣, 分不開.

而且「壓乾淨」不是假設, 是 Day 16 就撞到的事實: **ChatGPT & Codex 組的 `——` 密度只有 0.11**, 全組幾乎零命中 —— 不是因為那組的人都手寫, 而是因為 GPT-5.1 (2025 年 11 月) 開始聽話, 你叫它別用破折號它就真的不用. 一個重度用 AI、但會下「不要破折號」指令的作者, 在 V12 上看起來就跟純手寫的人没两样.

> [!IMPORTANT]
> 這是認識論上的不對稱: **證明「有」容易, 證明「沒有」難.** 抓到一個 signal, 可以說「這裡有 AI 味」; 但 signal 全空, 只能說「我沒抓到」, 不能說「它不存在」. 缺席不是證據 (absence of evidence is not evidence of absence). 同樣的道理在[浮水印那篇](../../../../01-fundamentals/ai-content-watermark.md) 也成立 —— 連官方浮水印都只能證明「這是我家模型生的」, 不能證明「這不是 AI 寫的」.

所以 lab01 這套工具的能力是**單向**的: signal 高, 排名靠前, 有參考價值; signal 低, 什麼都推不出來. 它是一把「找 AI 味」的尺, 不是一把「認人」的尺. 把它倒過來用, 會把一堆「調教得好的 AI 文」誤判成「純手寫」—— 這正是現在社群「抓 AI 文」最常犯的錯 (這場現象我另外寫在 [全民抓 AI 文](../../../../02-advanced/writing-style/ai-writing-tells-discourse.md)).

那「純手寫」有沒有**正面**訊號? 直覺上有 —— 口語化、句子長短不一、typo 不修、某個只有你會用的口頭禪或固定 emoji. 但這些 lab01 全沒抓, 要抓得重新設計一套, 而且一旦公開, AI 馬上學得會. 這留給 lab02 以後.

---

## 自己動手玩: 瀏覽器即時版 — A Tampermonkey Script

講這麼多, 不如你自己拿去量. 我把這套算法的**排版訊號**部分搬進瀏覽器, 做成一個 [Tampermonkey](https://www.tampermonkey.net/) userscript: 瀏覽**任何網頁**時浮出一個小面板, 即時給你**這一頁的 N_sum 和 density**, 外加 6 個排版 signal 的逐項拆解.

**檔案**: [`05-notes/assets/browser-ranker.user.js`](../../../assets/browser-ranker.user.js)

**怎麼裝**:

1. 瀏覽器裝 Tampermonkey 擴充套件
2. 開 Tampermonkey → 新增 script → 把 `browser-ranker.user.js` 內容整段貼上 → 存檔
3. 開**任何網站**, 右上角都會**自動**浮出面板 (要重掃或換頁後再掃, 點面板的 ↻, 或 Tampermonkey 選單選「掃描這頁」)

**面板給你什麼**:

- **N_sum**: 這頁命中了哪些 signal、權重加起來多少 (滿分 14.5)
- **density (每千字)**: `base ÷ 去空白字數 × 1000`, 跟全屆平均 49.4 比
- 6 個排版 signal 的 B (數量) 與 N (權重) 逐項表, 沒命中的淡掉
- density 顏色: 低於平均一半綠、接近平均橘、超過平均紅

> [!NOTE]
> 瀏覽器版只保留**排版層**的 6 個 signal (——、emoji、嚴格 / 一般粗體、blockquote、`<hr>`), 拿掉了文字層那三個 (不是⋯而是 / 最容易⋯的 / 標題 `｜`), 所以它是比 lab01 完整分數**簡化**的通用版, 全體 density 對照基準也降到 49.4. iThome 文章頁自動抓正文區塊 (`.markdown__style`), 其他網頁退而抓 `article` / `main` / 整頁, `<pre>` 程式碼區塊一律排除 —— 跟 analyzer 一樣.

> [!WARNING]
> 面板上的數字是**共現訊號, 不是判決.** density 高只代表「這篇疊了很多常見的 AI 排版/句型習慣」, 不代表「一定是 AI 寫的」; density 低也**不代表**「一定是人寫的」(這就是上一段整段在講的事). 拿它排序、拿它當提醒, 別拿它當證據去指認任何人.

---

## 邊界: 它能回答什麼、不能回答什麼 — What It Can and Can't Say

| 問題 | V12 答得出來嗎 | 為什麼 |
|:---|:---|:---|
| 這批文章裡, 哪些**最像 AI**? | ✅ 排序有信度 | signal 命中就是真的有這些習慣 |
| 這篇**比**那篇更有 AI 味嗎? | ✅ 相對比較可用 | 同一把尺量, 相對關係穩 |
| 這篇是 AI 寫的嗎? | ⚠️ 只能說「像不像」 | 單篇、單一分數不能定罪, 要看密度 ＋ 內容空不空 |
| 這篇是**純手寫、沒碰 AI** 嗎? | ❌ 答不出來 | 零命中 = 沒用 AI 或 壓乾淨, 分不開 |
| 這位作者從沒用過 AI? | ❌ 更答不出來 | 證明「從來沒有」在認識論上不可能 |

一句話收: **lab01 這把尺往「找 AI 味」的方向有用, 往「認人」的方向整個失效.** 這不是 bug, 是這類工具的天花板.

---

## 我的重點 — Takeaways

1. **把尺做到 V12 (9 個 signal) 都答不出「誰是純手寫」** —— 不是調得不夠細, 是問題本身偏心. 證明有容易, 證明沒有難.
2. **零命中不等於人寫.** ChatGPT 組 0.11 的例子擺在那: 壓得乾淨的 AI 文, 跟純手寫在任何 signal 上都一樣. 反推一定會冤枉人.
3. **工具是單向的.** signal 高 → 排序可信; signal 低 → 什麼都別說. 拿「找 AI」的尺去「認人」, 是現在社群抓 AI 文最常犯的錯.
4. **自己量一次最有感.** 文末的 Tampermonkey script 讓你隨手看任何頁的 N_sum / density —— 但看完請記得, 它給的是訊號, 不是判決.
5. **三部曲到此收尾.** Day 16 單指紋 → Day 17 綜合分數 → Day 18 翻面, 連成一條線: 從「量一個數字」到「知道這個數字不能回答什麼」, 後者才是這組實驗真正的收穫.

---

## Sources

- [lab01 V12 結果 (results-v12.md)](../../lab01/results-v12.md) — 本篇的數據基礎 (15,057 篇, density 69.66)
- [lab01 V12 權重微調 (v12-weight-tuning.md)](../../lab01/v12-weight-tuning.md) — v08c/v09/v11 新權重怎麼校的
- [browser-ranker.user.js](../../../assets/browser-ranker.user.js) — 本篇附的 Tampermonkey script (放在固定位置 `05-notes/assets/`)
- [Day 16: 單指紋掃全屆](./day16-ai-fingerprint-scan.md) — ChatGPT 組 0.11 的來源
- [Day 17: 從 1 個訊號擴到 6 個](./day17-composite-score.md) — 綜合分數的起點
- [AI 生成內容怎麼標記與辨識](../../../../01-fundamentals/ai-content-watermark.md) — 為什麼連浮水印都不能證明「不是 AI」
- [全民抓 AI 文](../../../../02-advanced/writing-style/ai-writing-tells-discourse.md) — 把「找 AI 的尺」倒過來用會冤枉人的社群現象
- [Tampermonkey 官網](https://www.tampermonkey.net/)
