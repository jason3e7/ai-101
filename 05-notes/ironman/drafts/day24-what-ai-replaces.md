---
title: AI 101 - 鐵人賽 Day 24：AI 可能會取代什麼，目前不會取代什麼
tags: [ai, 鐵人賽, ironman, 取代, 就業, 判斷力, 專業債, 草稿]
created: 2026-09-06
status: draft
---

# Day 24｜AI 可能會取代什麼，目前不會取代什麼

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 先講結論：**現在並沒有大規模的取代發生**——但有一個非常尖銳的局部效應，而且它打在最不該打的地方。

> **TL;DR (EN):** Ask about tasks, not job titles. Anthropic's Economic Index finds ~57% of usage is augmentation, not automation, concentrated in mid-to-high wage work. Stanford's payroll study finds no broad displacement — but workers aged 22–25 in the most AI-exposed occupations are down 13% relative, widening to about 19% by mid-2026, while senior people in the same jobs keep growing. The tasks AI eats first are entry-level tasks, and entry-level tasks are the ladder people climb to reach judgment. That is the real risk.

---

## 先問對問題：不是職業，是任務 — Tasks, Not Jobs

「AI 會不會取代 ⃝⃝ 這個職業？」這個問法本身就會得到爛答案，因為**沒有一個職業是單一任務**。

職稱是一堆任務的組合。被取代的從來不是職稱，是**任務**。所以正確的問法是：

> **我這份工作裡，哪些任務落在 AI 已經跨過門檻的那一區？**

Anthropic 的 Economic Index 用超過四百萬次對話分析了這件事：**約 57% 的使用是「增強」（augmentation），不是「自動化」（automation）**——人在旁邊迭代、學習、驗證，而不是被換掉。

分布也很有意思：AI 使用集中在**中高薪職業**（程式設計師、資料科學家），**最低薪和最高薪兩端反而都低**。這跟 Day 23 的「中產被夾殺」，是同一件事的兩個角度。

---

## 已經在被取代的 — What's Already Going

用 [Day 04](./day04-capability-landscape.md) 那張圖定位，被吃掉最快的是三種：

| 類型 | 為什麼守不住 | 現場證據 |
|---|---|---|
| **重複、但程序化耗時的** | 知道就能做，只是很花時間——AI 幾秒做完 | Spotify：**250 萬個**自動化維護 PR 已合併，大部分零人工介入 |
| **有標準答案、定義清楚的** | 落在鋸齒狀前沿**之內**，AI 穩定可靠 | GitHub Copilot：完成快 **55.8%** |
| **入門級的產出** | 門檻本來就靠「知道 ＋ 熟練」撐著 | 客服研究：**最低技能者 +34%**，差距被壓縮 |

Spotify 的數字最有畫面：99% 的工程師每週用 AI 寫程式、PR 頻率 **+76%**。首席架構師自己說：

> 到現在，我們出的大部分 PR 都是 AI agent 跟開發者一起寫的。

但他真正的結論**不是「工程師沒用了」**，而是：

> [!IMPORTANT]
> **瓶頸從「寫程式碼的速度」，移到了「做什麼的決策力、審哪些 PR 的判斷力、調用 agent 前後的品味」。**

這句話就是整篇的分水嶺。**工作沒有消失，是瓶頸換了位置。**

---

## 目前還守得住的 — What Holds

三層，由外而內。

**一、判斷力——在鋸齒狀前沿之外。**
[Day 03](./day03-what-it-cannot-do.md) 那個 Harvard–BCG 實驗：在 AI 會出錯、但看起來很合理的任務上，**新手掉了 19 個百分點，能識破瑕疵的專家反而贏**。AI 拉平了基礎，卻在前沿**放大了判斷力的價值**。

**二、隱性知識與情境——是 know-why／know-when，不是 know-what。**
「這個客戶去年就是為了這件事翻臉」「這段 code 動了會炸到誰」——這些沒有寫在任何文件裡，AI 讀不到。

**三、三件不該外包的事。** 這是 Hana Jryun Chang 的整理，我看過最好的版本：

| 不該外包 | 為什麼 |
|---|---|
| **策略** | 建立在你親身走過的脈絡上。AI 給的是統計均值，不是你的路 |
| **架構設計** | 什麼會變、哪裡要留彈性、哪些債可以欠——取決於你對未來的想像，不是模式匹配 |
| **品味** | **AI 是你美感的鏡子和放大器。你沒有品味，它做出來的也不會有。** |

她還多講了一條，我覺得最戳：**你真正享受的事，不要自動化。** 自動化「有意義的手工過程」，省掉的不只是時間，還有那個讓你之所以是你的部分。

---

## 最尖銳的那個發現：階梯正在被抽掉 — The Ladder

前面聽起來還好——直到你看這組數字。

史丹佛 Digital Economy Lab 用 ADP 的薪資資料（涵蓋數萬家公司、數百萬名工作者）追蹤**實際就業**，研究名字叫《Canaries in the Coal Mine?》（礦坑裡的金絲雀）。結論分兩半：

**好消息：沒有大規模取代。** 整體就業沒有崩，護理這類低 AI 暴露的職業照常成長。

**壞消息：**

> [!CAUTION]
> **22–25 歲、在高 AI 暴露職業（軟體開發、客服）的年輕人，就業相對下降 13%**；到 2026 年中的更新，這個缺口**擴大到約 19%**。
>
> 同樣的職業、同樣的公司，**資深的人沒事，而且還在成長。**

再配上另一組數字更難看：有公司以「AI 提升生產力」為由裁員，再用更低的薪資把同一批人聘回來訓練那套系統；資料工作者 **86% 面臨財務困境，年收入中位數低於 2.3 萬美元**。

把兩件事疊在一起，真正的風險才浮出來：

> **AI 吃掉的正好是「入門級任務」——而入門級任務，就是人爬上判斷力那一層的階梯。**

判斷力不是讀來的，是**做過、做錯、被修正**累積出來的。當入門任務消失，新人一開始就沒有練習的地方。Day 23 把這個叫做**專業債（expertise debt）**。

所以真正該問的不是「AI 會不會取代我」，而是：

> **十年後，誰來當那個有判斷力的專家？**

---

## 那要怎麼辦 — What To Do

三件現在就做得到的：

1. **盤點你的任務，不是你的職稱。** 把這週做過的事列出來，每一項標上「AI 已經能做／還不能／我不確定」。**不確定的那一格，去測一次**——[Day 03](./day03-what-it-cannot-do.md) 的「自己試」就是為這個準備的。
2. **刻意留下練判斷力的機會。** 不是拒絕用 AI，而是**先自己判斷，再去對答案**。順序反了，你就只是在核對，不是在學。
3. **往上一層走，但別跳過中間。** 這是最難的一條——AI 幫你跳過的那些步驟，正好就是長出判斷力的地方。

Day 03 說鋸齒狀前沿看不見。今天可以補一句：

> **能在前沿站得住的人，是自己走過那條路的人。**

明天開始講「已經在發生、現在該怎麼接」的幾件事。第一件是：這段字，到底是不是 AI 寫的。

---

## Sources

- [Canaries in the Coal Mine? Six Facts about the Recent Employment Effects of AI — Brynjolfsson, Chandar & Chen, Stanford Digital Economy Lab](https://digitaleconomy.stanford.edu/publication/canaries-in-the-coal-mine-six-facts-about-the-recent-employment-effects-of-artificial-intelligence/)
- [No Widespread Displacement, but the AI Employment Gap for Young Workers Has Widened to 19%（2026-08 更新）](https://digitaleconomy.stanford.edu/news/canariesaug26/)
- [Anthropic Economic Index report: Economic primitives（2026-01）](https://www.anthropic.com/research/anthropic-economic-index-january-2026-report)
- [Navigating the Jagged Technological Frontier — Dell'Acqua et al., HBS Working Paper 24-013](https://www.hbs.edu/faculty/Pages/item.aspx?num=64700)
- [不用急，還有三件事別交給 AI — Hana Jryun Chang](https://www.facebook.com/hana.j.chang/posts/pfbid0hp2BwKnSkRnnLxo7Kg6Yn1QaZQzGt3eHjsMbUu2moKAAwKBHbMpaRYudSpwjQRuRl)
- [Spotify 工程師不寫程式碼了：Honk Agent 與瓶頸轉移 — 科科科技人（Code with Claude London 2026）](https://www.facebook.com/permalink.php?story_fbid=pfbid02qpwHCoDz1xEPjHhEtA5hxevAi5rQPkjdzQ1BmEyq6DbenU7PKM3b4kntZJvxA1i7l&id=100092499602680)
- [AI 的隱形勞工與知識工作 Uberization — 矽谷輕鬆談](https://www.facebook.com/jktech.io/posts/pfbid02k6VYgQf77J8WiwzGhdh88PVtYhduFxJ8uWHvc8P9w27nGbwRC6YZB5EEJJ8RJLJxl)
