---
title: "AI 101 - 批次上傳 ZeroJudge 67 題遇到的坑與解法"
tags: [playwright, zerojudge, 自動化, 瀏覽器自動化, 批次, 踩坑, 筆記]
created: 2026-10-06
---

# 批次上傳 ZeroJudge 的坑與解法 — Batch-Submitting Lessons

[← 回 ZeroJudge 首頁](README.md)

> [!NOTE]
> 單題上傳的坑（AJAX 表單、原生輸入、confirm 對話框）已經寫在 [另一篇筆記](playwright-zerojudge-automation.md)。這篇記的是**一次送幾十題**才會遇到的問題：怎麼把程式碼餵進去、送出的冷卻、以及判題結果怎麼對齊。實測一輪送了 67 題。

> **TL;DR (EN):** Submitting dozens of solutions to ZeroJudge in one run surfaces problems a single submit never shows. Four that cost real time: (1) load each problem page with `networkidle`, not `domcontentloaded` — the submit button is injected by JS *after* DOM load, so an early read finds nothing; (2) there is a **submit cooldown** — back-to-back submits silently fail to redirect, so space them out (~6 s) and retry once after a longer wait; (3) the Submissions list is a **shared queue** with other users' rows, so never read just the top row — match your own row by account + problem id, and poll until the verdict is final (it may still say "judging"); (4) to get code into the browser without hand-copying base64 per problem, write a Bash-generated batch script that embeds the code as JSON and load it via the automation tool's `filename` param (the page/node sandbox had no `fs`/`require`/`import`). Large batches blow past the tool's 120 s limit and move to the background — collect results from the notification.

```markdown
# 批次送 ZeroJudge 的坑都在「送出前後的狀態」
* 餵程式碼進瀏覽器
    * 沙箱沒有 fs / require / import
    * 用腳本 filename 載入 Bash 產生的批次檔
    * 程式碼以 JSON 內嵌 免手抄 base64
* 送出
    * 頁面要等 networkidle 按鈕才出現
    * 有送出冷卻 連送會失敗
    * 題間間隔 加一次重試
* 讀結果
    * Submissions 是共用佇列 有別人的列
    * 用帳號加題號對齊自己那列
    * 剛送出可能還在判題 要輪詢
* 大批次
    * 超過工具 120 秒會轉背景
    * 靠通知收結果
```

---

## 怎麼把程式碼送進瀏覽器 — Getting Code into the Page

最卡的不是送出，是**怎麼把 67 份原始碼交給瀏覽器**。

瀏覽器內的 script（`page.evaluate` 那層）讀不到本機檔案；而自動化工具的 node 這層，這次實測 **`require`、動態 `import`、`fs` 全都不能用**（會噴 `require is not defined` / `A dynamic import callback was not specified`）。

出路是工具的 **`filename` 參數**：先用 Bash 產生一份自帶程式碼的批次腳本，再請工具用 `filename` 載入。程式碼用 `JSON.stringify` 內嵌成一個物件，**完全不用手抄 base64**（手抄幾十 KB 必錯）：

```python
# 產生批次腳本 (每批 ~8 題)
codes = {pid: open(path(pid)).read() for pid in batch}
js = "async (page) => { const CODES = %s; const jobs = %s; ... }" % (
        json.dumps(codes), json.dumps(batch))
open(f"batch_{i}.js", "w").write(js)
```

> [!TIP]
> 關鍵心法：**資料用 Bash 產生、用檔案帶進去，不要靠自己在工具呼叫裡重打一遍。** 這招同時解決了「跳脫字元」和「大量轉錄出錯」兩個問題。

---

## 送出：兩個狀態坑 — Two State Gotchas on Submit

### 坑一：頁面要等 `networkidle`，按鈕才出現

送出鈕 `#SubmitCode` 是頁面載入**之後**才用 JS 補上的。如果用 `waitUntil: 'domcontentloaded'` 就去找它，會抓到 `null`（錯誤訊息 `no SubmitCode btn`）。改成 `networkidle`（或至少 `load`）就正常：

```js
await page.goto(url, { waitUntil: 'networkidle' });   // 不要用 domcontentloaded
```

### 坑二：送出有冷卻，連送會默默失敗

連續送題時會出現**規律的隔題失敗**：第 1 題成功、第 2 題不跳轉、第 3 題又成功⋯。原因是 ZeroJudge 對同帳號的送出有**冷卻時間**，太快送的那題不會被受理（頁面停在原地、不跳 `/Submissions`）。

解法是**題與題之間留間隔，再加一次重試**：

```js
let r = await doSubmit(id, code);
if (!r.redirected) { await sleep(8000); r = await doSubmit(id, code); }  // 冷卻後重試一次
await sleep(6000);  // 下一題前先等，避免又撞冷卻
```

> [!WARNING]
> 「沒跳轉」不代表程式有問題，多半是**撞到冷卻**。不要因此改程式碼，先把間隔拉開重送。

---

## 讀判題結果：別只看最上面那列 — Reading Verdicts Correctly

`/Submissions` 是**全站共用的佇列**，最上面那列常常是**別人的提交**（會看到其他帳號的題目夾在中間）。只讀第一列會抓到錯的結果（實測出現「拿前一題 / 別人題目的判決」這種對不上的情況）。

正確做法：**掃整張表，用「自己的帳號 + 這題題號」對齊**自己那一列：

```js
const rows = [...document.querySelectorAll('table tr')].slice(1);
for (const r of rows) {
  const td = [...r.querySelectorAll('td')].map(x => x.innerText.trim());
  if (td[1].includes('json3e74101') && td[2].indexOf(id + '.') === 0) return td; // 這才是我的
}
```

還有一點：**剛送出時結果欄可能是空的（還在判題）**，要**輪詢**到出現 `AC` / `WA` / `TLE`⋯ 這類終局字樣。真的來不及，就事後用 `Submissions?problemid=XXX` 單獨查那題的最後結果。

> [!NOTE]
> 順帶一提，登入狀態看導覽列就知道：顯示帳號（像 `unknown 0`）＝已登入；顯示 `Login / Register`＝登出。這個帳號的顯示名稱是 `unknown`，實際帳號是 `json3e74101`（顯示名稱 ≠ 帳號）。

---

## 大批次會轉背景 — Long Runs Go to the Background

一批太多題（間隔 + 重試加起來），單次工具呼叫會**超過 120 秒而被移到背景執行**，結果之後用通知送回來。因應方式：**把批次切小（一批約 5–8 題）**，或接受背景執行、等通知收結果。它們共用同一個瀏覽器分頁，所以**一批沒跑完不要開下一批**，會互相打架。

這輪最後成績：送判 67 題，**64 AC**、3 題未過（演算法邏輯要重想，不是上傳問題）。

## Sources

- [用 Playwright 自動操作 ZeroJudge](playwright-zerojudge-automation.md) — 單題送出那段的三個坑（本篇的前傳）
- [ZeroJudge](https://zerojudge.tw/) — 目標站
- [Playwright — Navigations / waitUntil](https://playwright.dev/docs/navigations) — `networkidle` 等載入狀態的官方說明
