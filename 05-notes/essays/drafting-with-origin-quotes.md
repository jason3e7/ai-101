---
title: AI 101 - 用「原話」當 draft 的錨點
tags: [寫作方法, voice, draft, 原話, 文風保留, essay]
created: 2026-10-04
---

# 用「原話」當 draft 的錨點 — Drafting with Origin Quotes

[← 回主頁](../../index.md)

> [!NOTE]
> 跟 AI 一起寫文章時, 把**自己原本說出來的那句話**保留成文章裡的 blockquote, 然後 AI 根據它展開. 這招的作用: 文章有一個**抵抗 AI 磨平**的錨點, 讀者看得到你的語氣, 你也能回頭對照自己最初想說的是什麼.

> **TL;DR (EN):** When drafting with AI, keep your own raw spoken sentence as a blockquote in the final piece, then let AI expand around it. The quote acts as an anchor against AI-smoothing: the article now carries a verifiable fragment of your actual voice, readers can hear your tone, and you can check back against your original intent. The technique has deep echoes in craft writing (Anne Lamott's "Shitty First Drafts," Natalie Goldberg's "first thoughts"), oral history methodology (verbatim preservation), journalism (direct quotes beat paraphrases on credibility), and verbatim theatre (performing real people's exact words). In the AI era this becomes defensive: without a voice anchor, AI's smoothing tendency plus the user's own 文化入侵 drift will erase personality within a few drafts.

```markdown
# 用原話當 draft 的錨點
* 這招是什麼 (保留原話 + Claude 展開)
* 為什麼有效
  * AI 傾向磨平, 原話是抵抗
  * 具體入口而非抽象 brief
  * 可追溯到最初意圖
* 相關概念 (有歷史靠山)
  * freewriting / 第一念
  * Shitty First Drafts
  * 口述歷史保真
  * 新聞直接引語
  * verbatim theatre
* 怎麼套 (workflow)
* 什麼時候別用
```

---

## 這招是什麼 — What This Is

跟 AI 一起寫一篇文章時, 流程通常是:

1. 自己口頭或打字給 AI 一段**想法**
2. AI 整理 / 擴寫 / 排版成一篇稿
3. 自己再改

**這招加一步**: 在步驟 2 之前, 把那段「自己原本說出來的話」整段摘出來, **原封不動**當成 blockquote 塞進最終文章裡, 冠名「原話 (某某):」或在末尾署名. 然後 AI 繞著這段展開, 不是替換它.

實例 (這個 repo 的鐵人賽系列):

- Day 16 開頭有 jason3e7 原話: 「在用 Claude 寫鐵人賽的過程, 一直要修正 AI 產出不通順、不自然的大量內容, 非常厭世...」
- Day 18 開頭有 jason3e7 原話: 「做了這個研究之後, 我反而好奇有那些文章是純手寫...」
- Day 20 開頭有 jason3e7 原話: 「我覺得寫到第 20 天, 跟 AI 的合作讓我快得到失語症了」

這些 quote 是文章的**種子**, 不是裝飾. 整篇是繞著它長出來的.

---

## 為什麼有效 — Why It Works

三個理由, 一個比一個深:

### 1. AI 有磨平傾向, 原話是抵抗

RLHF 訓練讓 AI 偏向「安全、工整、通順」的輸出. 這在表達**個人判斷**時變成問題: AI 會把個人用語改成通用句、把強烈情緒 (「厭世」「失語症」) 改成中性描述、把破碎句改成完整句. **原話當 blockquote**, 這一塊就不會被磨 — 它是個固定錨點, AI 只能在**周圍**展開, 不會進去改.

### 2. 具體入口比抽象 brief 強

跟 AI 說「寫一篇關於 AI 文化入侵的文章」vs 「這是我原本說的: [...]. 繞著這段展開」, 後者結果會穩定得多. 原話把**模糊的概念**鎖成**具體的語氣跟角度**. AI 猜不準抽象 brief, 但能複製具體語感.

### 3. 可追溯到最初意圖

幾個月後回頭看, 原話是**時間膠囊**. 稿子被改過幾次後, 作者自己可能都忘了最初為什麼要寫. 原話還在, 就能對照「我當時真的這樣想嗎」. 對 AI 協作尤其重要, 因為每輪改動都可能偏離, 需要定期回到起點校準.

---

## 相關概念 — Prior Art

這招不是新發明, 五個有歷史靠山的親戚:

### Freewriting / 第一念 (First Thoughts)

Natalie Goldberg 在 *[Writing Down the Bones](https://en.wikipedia.org/wiki/Writing_Down_the_Bones)* 強調「第一念」: 寫作時第一個冒出來的想法最誠實, 之後的 self-editing 會把它洗掉. Peter Elbow 的 *Writing Without Teachers* 系統化了這個做法, 稱 freewriting — 不停筆、不編輯、不管文法, 讓真實的聲音先流出來. **原話錨點是 freewriting 的保留版**: 不要求整篇都 freewrite, 但至少把最誠實的那一段框起來不動.

### Shitty First Drafts (Anne Lamott)

Anne Lamott 在 *[Bird by Bird](https://en.wikipedia.org/wiki/Bird_by_Bird)* 用「Shitty First Drafts」當章節標題: 第一稿本來就該爛, 爛才真實, 之後再修. 跟 AI 協作有個反向問題 — **AI 的第一稿不會爛, 它會立刻給你一篇工整的文章**, 把你跳過「爛但真實」那階段的機會剝奪了. 保留原話等於**強制留下一塊「爛但真實」的區域**不被 AI 整理.

### 口述歷史的保真原則 (Oral History)

[口述歷史](https://en.wikipedia.org/wiki/Oral_history)方法論的核心: 訪談紀錄要**逐字保存**, 連說錯、重複、口頭禪都保留, 因為這些細節才承載受訪者的真實樣貌. 整理成「流暢版」等於失真. AI 協作下的作者 ≈ 自己採訪自己, 保留原話就是對自己做口述歷史.

### 新聞的直接引語 (Direct Quotation)

新聞學講 "quotes don't lie, paraphrases do". 一句直接引語 (加引號) 的可信度遠高於記者的轉述, 因為讀者可以**自己判斷**那個人的語氣跟意圖. AI 協作時放原話的效果類似: 讀者看到你的真實聲音, 不只是 AI 幫你整理後的版本.

### Verbatim Theatre (紀實戲劇)

[Verbatim theatre](https://en.wikipedia.org/wiki/Verbatim_theatre) 完全由真實訪談的**逐字稿**組成, 演員演的是「活人說過的真話」. 代表作如 Anna Deavere Smith 的 *Fires in the Mirror*. 這個流派的信念: 真實的語言結構 — 停頓、口語、文法錯誤 — 承載的意義, 腳本寫不出來. 跟保留原話錨點同一邏輯.

---

## 怎麼套 — How to Apply

具體 workflow, 以我用 Claude Code 寫鐵人賽為例:

1. **產生原話**: 用語音輸入 / IM / 聊天視窗把想法打給 AI (或自己錄音). 不要先改. 關鍵: 這段要有**強度** — 情緒詞、具體場景、不工整的句子
2. **立刻存檔**: 把那段原話當 blockquote 貼進 draft 檔, 冠名「原話 (你的名字):」或末尾署「— 你的名字」. **在 AI 動任何筆之前先存**
3. **叫 AI 繞著展開**: 告訴 AI 「這段原話是文章的 anchor, 不要改, 繞著它寫 N 字」
4. **檢查**: 定稿前再回頭看那段原話, 確認文章的其他部分**服務它**, 不是**蓋過它**
5. **留一個以上**: 一篇文章可以有多個原話錨點 (開頭 + 中段 + 結尾), 分別鎖住不同論點的入口

### 什麼算「夠好的原話」

- **有情緒**: 「厭世」「爽」「卡住」「失語症」> 「我覺得這個議題值得探討」
- **有具體場景**: 「寫到第 20 天...」> 「做了一陣子以後...」
- **不工整也沒關係**: 口語、重複、typo 都可以留, 甚至該留 (更接近口述歷史的真實)
- **一兩句就夠**: 保留原話不是把整篇文章口述化, 是**釘住關鍵一兩個點**

---

## 什麼時候別用 — When Not To

- **純技術文件**: API 用法、安裝步驟, 需要的是精確不是語氣, 原話只會干擾
- **商業寫作**: 客戶要的是結果不是你的個人聲音, 這招會模糊焦點
- **原話本身很弱**: 如果原話是「這個議題值得探討」這種空話, 保留下來反而暴露弱點, 不如直接寫
- **多人共寫**: 原話承載的是**個人語氣**, 群體署名的文章 (如公司 blog) 用這招會格格不入

---

## 我的重點 — Takeaways

- **原話當錨點**是跟 AI 協作時**保留自己聲音**的具體技術, 不是修辭花樣
- **三個作用**: 抵抗 AI 磨平、給 AI 具體入口、留時間膠囊給未來的自己對照
- **有五條歷史血緣**: Freewriting / Shitty First Drafts / 口述歷史 / 新聞直接引語 / Verbatim theatre — 不同領域都在做類似的事, 核心共通: **真實語言的細節承載意義, 整理成「順稿」就失真**
- **AI 時代更重要**: 跟傳統寫作比, AI 第一稿就工整的傾向更強, 不主動留原話錨點, **一兩輪協作就會把你的聲音磨光**
- **要有強度才值得保留**: 不是任何自己說過的話都該原話留下, 只有**有情緒、有場景、有張力**的那幾句

---

## 相關 — Related

- [jason3e7-writing-voice skill](../../skills/jason3e7-writing-voice.md) — 把這套 anchor-first 做法編成 skill 的 Do/Don't 版
- Day 20 iThome 鐵人賽草稿 — 本招的實戰示範 (開頭兩段原話 + 內文展開): `../ironman/drafts/day16-20/day20-ai-style-infection.md`
- [AI 的文風與語氣](../../02-advanced/writing-style/ai-writing-style-tells.md) — AI 磨平的具體症狀, 解釋為什麼需要錨點
- [先驗證再拓展](./ai-verify-then-expand.md) — 跟 AI 協作的另一條基本原則

## Sources

- [Writing Down the Bones — Natalie Goldberg (Wikipedia)](https://en.wikipedia.org/wiki/Writing_Down_the_Bones) — 第一念、真實優先於工整
- [Bird by Bird — Anne Lamott (Wikipedia)](https://en.wikipedia.org/wiki/Bird_by_Bird) — Shitty First Drafts 章節
- [Freewriting — Wikipedia](https://en.wikipedia.org/wiki/Free_writing) — Peter Elbow 等人系統化的做法
- [Oral history — Wikipedia](https://en.wikipedia.org/wiki/Oral_history) — 逐字保存的方法論
- [Verbatim theatre — Wikipedia](https://en.wikipedia.org/wiki/Verbatim_theatre) — 紀實戲劇的語言保真原則
- [On Writing: A Memoir of the Craft — Stephen King (Wikipedia)](https://en.wikipedia.org/wiki/On_Writing:_A_Memoir_of_the_Craft) — 真實性先於完美
