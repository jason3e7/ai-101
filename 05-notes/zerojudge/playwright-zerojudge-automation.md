---
title: "AI 101 - 用 Playwright 自動操作 ZeroJudge (重點: 送出答案)"
tags: [playwright, zerojudge, online-judge, 自動化, 瀏覽器自動化, 驗證, 筆記]
created: 2026-10-05
---

# 用 Playwright 自動操作 ZeroJudge — Automating ZeroJudge with Playwright

[← 回主頁](../../index.md)

> [!NOTE]
> 把「AI 寫的程式 → 丟上 online judge 判題」這條線接起來, 中間的上傳步驟可以用 Playwright 自動化。這篇記的是 [ZeroJudge](https://zerojudge.tw/) 的實作, **重點在送出答案那一段** —— 因為它有三個會讓你卡住、但一旦知道就很簡單的坑。這也呼應 [Day 21 那篇](../ironman/drafts/day21-25/day21-back-to-verifiable-ground.md): online judge 就是一個標準的 oracle, 程式的對錯它幫你判, 不用人讀。

> **TL;DR (EN):** To automate submitting code to ZeroJudge with Playwright, three things trip you up, all in the submit step: (1) the submit form loads via **AJAX** after you open the modal — wait for the textarea before touching it; (2) setting the code box with `textarea.value = code` in JS **does not register** — the submit handler never sees it, you must use Playwright's native `fill()` (real input); (3) clicking submit fires a `confirm()` dialog that Playwright **auto-dismisses by default**, cancelling the submission — override `window.confirm` to return true first. After submit it **redirects to `/Submissions`**; read the verdict there (AC / WA / CE / TLE…), not inline. Always test locally with the matching compiler (`g++ -std=c++17`) before uploading.

```markdown
# Playwright 丟題上 ZeroJudge, 坑都在送出那一段
* 整條流程
    * 讀題 → 本機 g++ 先測 → 送出 → 看結果 → 不過就改再送
* 送出的三個坑
    * 表單是 AJAX 載入 要先等
    * JS 塞 value 沒用 要用原生輸入 fill
    * 送出會跳 confirm 預設被取消 要先放行
* 看結果
    * 送出後會轉到 Submissions
    * 讀第一列的評分結果 AC/WA/CE/TLE
* 小提醒
    * 顯示名稱不等於帳號
    * 只在自己帳號、練習題上做
```

---

## 這篇能幫你做到什麼 — What This Enables

一句話: **讓機器幫你把程式丟上去判, 並讀回「過沒過」。** 搭配本機編譯, 你可以做一個小迴圈 —— AI (或你自己) 寫一版 → 本機 g++ 編譯測範例 → Playwright 上傳 → 讀判題結果 → 不過就把錯誤訊息餵回去改 → 再送, 直到 AC。

這正是程式產出「好驗」的地方: online judge 是個客觀、自動、秒級回饋的 oracle。文字產出沒有這種東西 (見 [AI 產出怎麼驗](../../02-advanced/limits-and-verification/verifying-ai-output.md))。

---

## 流程總覽 — The Flow

```
1. 讀題     navigate 到 ShowProblem?problemid=XXX, 抓題目與範例
2. 本機測   寫 .cpp, g++ -std=c++17 編譯, 用範例輸入測過再上傳
3. 送出     開送出視窗 → 選語言 → 填程式碼 → 送出  ← 坑都在這
4. 看結果   送出後會轉到 /Submissions, 讀第一列的評分結果
5. 迴圈     不是 AC 就讀錯誤訊息、改 code、回到步驟 3
```

> [!TIP]
> **先在本機測過再上傳。** ZeroJudge 的 CPP 是 `g++ -std=c++17 (g++ 13.3.0)`, 本機用同樣的 flag 編譯 (`g++ -O2 -std=c++17`), 範例輸入都對了再送, 上傳幾乎一次就過 —— a001 就是這樣一次 AC 的。別把編譯好的執行檔進 git (用 `.gitignore` 擋掉)。

---

## 讀題與本機先測 — Read & Test Locally

題目頁網址是 `https://zerojudge.tw/ShowProblem?problemid=a001`。正文與範例在 `#problembody` 裡, 直接抓文字即可:

```js
await page.goto('https://zerojudge.tw/ShowProblem?problemid=a001');
const problem = await page.evaluate(() =>
  document.querySelector('#problembody').innerText
);
// 從裡面挑出「範例輸入 / 範例輸出」自己核對
```

本機測 (以 a001「讀一行, 輸出 hello, <字串>」為例):

```bash
g++ -O2 -std=c++17 -o a001 a001.cpp
echo "world" | ./a001        # 期望: hello, world
printf 'world\r\n' | ./a001  # 順便測 Windows 行尾, \r 要處理掉
```

---

## 送出答案: 三個坑 — Submitting: Three Gotchas

這是整篇的重點。ZeroJudge 的送出不是一個單純的表單, 踩過才知道。

### 坑一: 送出表單是 AJAX 載入的, 要等

題目頁有顆「送出解答」按鈕 (`#SubmitCode`), 按下去會開一個 Bootstrap modal (`#Modal_SubmitCode`)。但 **modal 裡的內容 (語言選項 + 程式碼框) 是點下去之後才用 AJAX 載進來的** —— 一開始只有「載入中…」的轉圈圈, 送出鈕還是 `disabled`。所以要**等 textarea 真的出現**再動手:

```js
await page.evaluate(() => document.querySelector('#SubmitCode').click()); // 開 modal
await page.waitForSelector('#Modal_SubmitCode textarea.form-control');    // 等表單載入
```

載入後的結構:
- 語言用 radio (`input[name=language]`, 值是 `C` / `CPP` / `JAVA` / `PYTHON`), **預設 CPP 已選**
- 程式碼是**一般的 `<textarea class="form-control">`** (沒有 CodeMirror)
- 送出鈕是 `#Modal_SubmitCode button.btn-primary` (文字「送出程式碼」)

### 坑二 (最關鍵): 用 JS 塞 `value` 沒有用, 要用原生輸入

直覺會想用 `textarea.value = code` 把程式碼塞進去。**這行得通地「看起來」填好了, 但送出後判題佇列裡根本沒有你的提交** —— 送出的處理函式沒「看到」那個值。

解法是用 Playwright 的**原生輸入** (`fill`, 會真的 focus + 觸發輸入事件), 跟真人打字一樣:

```js
// ❌ 沒用: 送出後不會出現在 Submissions
// await page.evaluate((c) => { document.querySelector('#Modal_SubmitCode textarea').value = c; }, code);

// ✅ 有用: 原生輸入
await page.locator('#Modal_SubmitCode input[value=CPP]').check();        // 確認語言
await page.locator('#Modal_SubmitCode textarea.form-control').fill(code); // 原生填入
```

> [!WARNING]
> 這個坑最難查, 因為頁面上「看起來」完全正常: 程式碼在框裡、按鈕沒 disabled、點下去也沒報錯, 但就是沒送出去。只要記住一條: **要讓值「像真人打的」, 用 `fill()` 不要用 `.value =`。**

### 坑三: 送出會跳 confirm, 預設會被自動取消

點「送出程式碼」會觸發一個 `confirm()` 確認框。**Playwright 預設會自動 dismiss (等於按取消) 所有對話框** —— 於是送出被取消, 一樣什麼都沒發生。

最穩的做法是在點送出前, 直接把 `window.confirm` 覆蓋成永遠回 true, 連原生對話框都不會跳出來:

```js
await page.evaluate(() => { window.confirm = () => true; });
await page.locator('#Modal_SubmitCode button.btn-primary').click(); // 送出
// 成功的話頁面會自己導向 https://zerojudge.tw/Submissions
```

> [!NOTE]
> 送出成功的訊號是**網址轉到 `/Submissions`**。如果點完還停在 ShowProblem 原地沒動, 多半是上面三個坑之一 (表單沒載完 / 用了 `.value` / confirm 被取消)。

---

## 確認結果與重試迴圈 — Check Result & Retry Loop

送出後會停在 `https://zerojudge.tw/Submissions`, 最新的提交在表格第一列。讀「評分結果」欄:

```js
await page.waitForURL('**/Submissions');
const top = await page.evaluate(() =>
  [...document.querySelector('table tr:nth-child(2)').querySelectorAll('td')].map(td => td.innerText.trim())
);
// top 例: ['18637857', 'json3e74101(unknown)', 'a001. 哈囉', 'AC (1ms, 3.6MB)', 'CPP', '2026-10-05 22:03']
```

常見評分結果 (抓 `AC` 以外的就是要修):

| 代碼 | 意思 | 怎麼辦 |
|:---|:---|:---|
| **AC** | Accepted, 通過 | 收工 |
| **WA** | 答案錯 | 比對範例輸出, 常是格式/邊界 (多空白、換行、`\r`) |
| **TLE** | 超時 | 演算法太慢, 換做法 |
| **MLE** | 超記憶體 | 資料結構太肥 |
| **CE** | 編譯錯 | 點進去看完整編譯訊息, 本機先編過就不會中這個 |
| **RE** | 執行期錯 | 陣列越界、除以零之類 |

**重試迴圈**: 不是 AC → 點該列進去讀錯誤訊息 (CE 有完整編譯器輸出, WA 可看是哪個測資點掛) → 改 `.cpp` → 本機再測 → 回「送出答案」那三步再送一次。

---

## 我的重點 — Takeaways

1. **坑全在送出那一段, 而且三個都「看起來正常」。** 表單 AJAX 要等、`fill()` 不要 `.value =`、confirm 要放行 —— 三個湊齊才送得出去。
2. **`.value = code` 是最坑的一個。** 畫面完全正常卻沒送出, 除非你去 Submissions 查否則不會發現。預設就用原生 `fill()`。
3. **送出成功 = 轉址到 /Submissions。** 用這個當判斷, 比看畫面可靠。
4. **先本機 g++ 測過再上傳** (同 `-std=c++17`), 幾乎一次 AC, 省掉來回。
5. **顯示名稱 ≠ 帳號**: 這個帳號畫面顯示「unknown」, 實際登入帳號是 `json3e74101`。查自己的提交要認帳號。

> [!CAUTION]
> 這會用你**已登入的帳號**對外送出公開提交。只在自己的帳號、練習題上做; 別拿去對別人的帳號或比賽做自動化。送出是對外動作, 送出去就留紀錄了。

---

## Sources

- [ZeroJudge](https://zerojudge.tw/) — 高中生程式解題系統 (本篇的目標站)
- [a001 題目頁](https://zerojudge.tw/ShowProblem?problemid=a001) — 範例題, 含各評分結果代碼說明
- [Playwright — Auto-waiting / Dialogs](https://playwright.dev/docs/dialogs) — 預設自動 dismiss 對話框的官方說明
- [Day 21: 回到可驗證的主場](../ironman/drafts/day21-25/day21-back-to-verifiable-ground.md) — 為什麼 online judge 是個好 oracle
