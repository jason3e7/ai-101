---
title: "AI 101 - 鐵人賽 Day 20: AI 文化入侵"
tags: [ai, 鐵人賽, ironman, 文化入侵, 約定俗成, 草稿]
created: 2026-10-03
status: draft
---

# Day 20｜AI 文化入侵 — AI's Quiet Cultural Takeover

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 19](./day19-ai-watermark.md) 講官方浮水印不是鐵證. 這篇換個角度, 不問「怎麼辨識 AI」, 問「它正在怎麼變成預設」. 原本中文寫作沒有統一腔調, AI 來了以後一點一點被污染; 下一代一開始就看到 AI 文體, 會當作本來就是這樣. 時間拉長, 本來的多樣性融進一種聲音 — 這不是對未來的推測, 是**現在進行式**的**文化入侵**. 下探一層的擔憂: 如果 AI 統治了語言, 就幾乎等於統治了思考.

> **TL;DR (EN):** Imagine Chinese tech writing before AI: wildly varied voices, no "standard" tone. After AI: writing structures, tone, and style all get polluted bit by bit — not by anyone's intent, just because so many people now use AI (or imitate it) that the default drifts. The next generation reads mostly AI-flavored prose and treats it as the baseline; over time the pre-AI diversity blends into a single voice. Clockwise rotation on clocks is a smaller precedent (northern hemisphere sundials cast shadows that way, enough people saw it, became default), but AI pulls off the same thing in years rather than millennia. This is a cultural invasion by convention, not by force — and this generation of writers is quietly living through the drift.

```markdown
# AI 文化入侵
* 文化入侵 (現在進行式, 不是未來推測)
  * 失語症 體感 (jason3e7)
  * 無 AI → AI 混入 → 下一代 default
  * 約定俗成是機制 (時鐘例子: 對初始條件敏感)
  * 更深的擔憂 (統治語言 ≈ 統治思考)
* 我的重點
```

---

## 文化入侵 — The Cultural Invasion

為什麼寫這篇, 體感先放這:

> 我覺得寫到第 20 天, 跟 AI 的合作讓我快得到失語症了.
> — jason3e7

下面是在試圖拆解它.

### 無 AI → AI 混入 → 下一代 default

> 試想原本的都沒有 AI 產生文章的時候, 直到 AI 能被使用之後, 文章結構和說話態度和方式, 就一點一點被污染了, 當我們的下一代, 一開始就看到大多是 AI 文體和內容時, 就會當作本來就是這樣, 時間再拉長來看, 未來就會交融在一次, 某種程度來說, 文化入侵了.
> — jason3e7

這句話把三個時期攤開:

1. **AI 之前**: 文章有各種寫法, 每個作者的語氣彼此不同, 沒有統一的「標準腔」
2. **AI 之中 (就是現在, 不是未來)**: 一點一點被污染 — 用 AI 寫的、模仿 AI 寫的、下意識跟著 AI 腔的, 混進整個寫作圈. 這不是對未來的推測, 寫這篇的當下就在發生
3. **AI 之後**: 下一代把 AI 文體當 default, 原本的多樣性沒人記得, 慢慢**融合成一種聲音**

這不是誰故意的 — 是**約定俗成**在加速. 用時鐘當小例子最直白.

### 約定俗成是機制 (時鐘順時針是個小例子)

時鐘的分針為什麼順時針轉? **不是因為「順時針」比較對, 是約定俗成**.

日晷是最早的計時器, 立一根棒子, 太陽曬出影子, 影子轉一圈就是一天. 日晷發源在**北半球**, 太陽從東邊升、南邊過、西邊落, 影子的方向從西 → 北 → 東, 這方向被人記成「順時針」. 北半球的人造機械鐘時, 自然延續了這個方向 (詳見 [關鍵評論網 — 時鐘順時針的起源](https://www.thenewslens.com/article/111804)).

如果人類文明發源在南半球, 日晷影子會反過來轉, 現在的手錶可能全部都是逆時針. 沒有誰對誰錯, 只是**早到 + 數量多, 久了變預設**.

這其實是「**對初始條件的敏感依賴**」(Sensitive Dependence on Initial Conditions, 混沌理論的核心概念) 的典型例子: 系統最後長什麼樣, 高度取決於**最早那一刻的細微條件**, 而不是後來的「對錯」. 日晷恰好先在北半球發明, 70 億人的手錶就跟著往同一個方向轉. **AI 文化入侵同一邏輯** — 訓練資料的早期 bias、哪家公司先 ship 哪種 voice、RLHF 評分員的個人偏好, 這些**初始條件**決定了 AI 的預設腔調, 透過規模化擴散變成下一代眼中的「標準」.

**時鐘花了幾千年完成這次約定俗成. AI 這次用幾年**. 規模一樣、速度兩個數量級的差距. 這就是「文化入侵」用詞比「約定俗成」強的理由 — 不是壞, 只是**又大又快又全面**.

### 更深的擔憂: 統治語言 ≈ 統治思考

> 如果 AI 真的統治了人類的語言, 拿走了人類溝通的方式, 是不是就能統治人類?
> — jason3e7

這個擔憂有歷史回聲. Orwell 的《1984》設計了「新語」(Newspeak), 刻意刪減詞彙, 目標是**讓某些思考無法被說出來** — 當詞不存在, 概念也不存在, 異議就不會發生. Sapir-Whorf 假說 (語言相對論) 進一步主張, 一個人能想到的範圍, 受他會用的語言限制. Wittgenstein 更直接: 「**我的語言的界限, 就是我的世界的界限**」.

AI 版本的這條邏輯是: 如果整個中文圈都寫 AI 腔、用 AI 的句型結構、聽 AI 選的字 — 下一代**連想別的講法都想不到**. 不是沒選擇, 是**沒看過其他選擇**. 這跟公開審查不同, 更隱蔽也更徹底: 不用禁止什麼, 只要**預先定義什麼能被想**.

寫到這聽起來像陰謀論, 但這篇**不是要讀者悲觀, 是要警覺**. 看得見正在發生的事, 跟事後才發現, 本質不同.

---

## 我的重點 — Takeaways

- **這是現在進行式, 不是未來推測**. AI 文體正在變成預設, 寫這篇的當下就在發生
- **文化入侵不是對錯問題, 是數量問題**. 多數方向變預設, 少數方向變成「需要特別解釋」. 時鐘花幾千年, AI 用幾年
- **想守住自己的語言要主動擋**. 「寫自然一點」這種模糊指令沒用, 要具體列 Don't ([我寫了一份 anti-convention guide](../../../../skills/jason3e7-writing-voice.md), 可以當起點改成你自己的)
- **這代人正在記錄入侵前的 baseline**. 現在這個時間點 — AI 已經大規模滲入、但多樣性還沒完全被洗平 — 寫下來的文字之後要對照 10 年、20 年的 corpus, 才看得出入侵的速度跟幅度. 這代人剛好站在歷史切片上
- **最深的擔憂**: 統治語言幾乎等於統治思考 (Orwell 新語 / Wittgenstein 的 language limits). AI 不用公開審查, 只要讓某種腔調變成**唯一聽過的**, 其他講法就不會被想到

---

## Sources

- [Day 19: 浮水印怎麼運作, 為什麼不能當證據](./day19-ai-watermark.md) — 子系列前一篇
- [時鐘順時針的起源 — 關鍵評論網](https://www.thenewslens.com/article/111804)
- [為什麼時鐘順時針轉 (Wikipedia: Clockwise)](https://en.wikipedia.org/wiki/Clockwise#Origin_of_the_convention)
- [jason3e7-writing-voice skill: 我的 anti-convention 寫作規則](../../../../skills/jason3e7-writing-voice.md)
