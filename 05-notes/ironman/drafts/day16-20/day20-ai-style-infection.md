---
title: "AI 101 - 鐵人賽 Day 20: AI 文風入侵, 五個記號 + 抗體 + 子系列小結"
tags: [ai, 鐵人賽, ironman, 文風, 文化入侵, 子系列, 草稿]
created: 2026-10-03
status: draft
---

# Day 20｜AI 文風入侵, 五個記號 + 抗體 + 子系列小結 — AI Writing Style as Cultural Convention

[← 回主頁](../../../../index.md)｜[參賽規劃](../../plan.md)｜[三十篇標題](../../titles.md)

> [!NOTE]
> [Day 16](./day16-ai-fingerprint-scan.md)-[Day 19](./day19-ai-watermark.md) 把 AI 寫作的痕跡量化到最細, 這篇反過來講**為什麼它會變成預設**. 時鐘的分針順時針轉, 不是因為「順時針」比較對, 是因為日晷在北半球剛好這樣轉, 數量多了變成約定俗成. AI 文風現在正做同一件事. 用五個記號 + 一組抗體收尾子系列.

> **TL;DR (EN):** Clock hands rotate clockwise not because clockwise is "correct" but because sundials in the Northern Hemisphere happened to cast shadows that way, and once enough people saw it, it became default. AI writing style is doing the same thing to Chinese tech prose: em dashes, 「不是 X 而是 Y」 dichotomy, 「最容易...的」 teaching tone, markdown-heavy formatting, and ｜ as a title separator are becoming defaults because enough people (and models) use them, not because they are better. This closes the Day 16-19 sub-series: Day 16 single signal, Day 17 composite, Day 18 flip side (cannot prove pure-human), Day 19 watermark (not proof). Day 20 completes the arc: measurement is one half, the other half is noticing that convention itself has been infected. The antidote is active — jason3e7's voice guide is my personal anti-convention manual, intentionally avoiding each tell. Not because the convention is wrong, but because being default means losing choice.

```markdown
# AI 文風入侵: 五個記號 + 抗體 + 小結
* 時鐘的隱喻 (convention 是數量贏的, 不是對錯)
* 五個記號
  * ——
  * 不是 X 而是 Y
  * 最容易...的
  * 排版堆料
  * 標題 ｜
* 抗體 (jason3e7 voice guide 的 Do/Don't)
* 子系列小結
  * Day 16 單指紋
  * Day 17 綜合分數
  * Day 18 翻面問題
  * Day 19 浮水印
  * Day 20 (本篇) 文化入侵
* 我的重點
  * 保留自己風格要主動
  * 單一記號不是證據, 密集 + 無擔當才是
```

---

## 為什麼講這個 — Why This Matters

原話 (jason3e7):

> 我覺得寫到第 20 天, 跟 AI 的合作讓我快得到失語症了.

---

## 時鐘順時針, 只是約定俗成 — Clockwise, Only a Convention

時鐘的分針為什麼順時針轉? **不是因為「順時針」比較對, 是約定俗成**.

日晷是最早的計時器, 立一根棒子, 太陽曬出影子, 影子轉一圈就是一天. 日晷發源在**北半球**, 太陽從東邊升、南邊過、西邊落, 影子的方向從西 → 北 → 東, 這方向被人記成「順時針」. 北半球的人造機械鐘時, 自然延續了這個方向 (詳見 [關鍵評論網 — 時鐘順時針的起源](https://www.thenewslens.com/article/111804)).

如果人類文明發源在南半球, 日晷影子會反過來轉, 我們現在的手錶可能全部都是逆時針. 沒有誰對誰錯, 只是**早到 + 數量多, 久了變預設**.

這就是**約定俗成**的核心: 一個本來可以是另一個樣子的選擇, 因為先來的人這樣做、後來的人跟著做, 數量累積到一個臨界點, 整個系統就把它當成「正確」. 要反過來不是不行, 但要**主動**擋 — 不然就被預設拖著走.

AI 文風現在正在做同一件事.

[Day 16](./day16-ai-fingerprint-scan.md) 到 [Day 18](./day18-detecting-pure-human.md) 掃出來的那些 pattern — 破折號、對立句、「最容易...的」教學腔、排版堆料、標題 ｜ 分隔 — 原本中文寫作沒有統一寫法. 現在**越來越多人在模仿 AI 的腔**, 甚至不是用 AI 寫, 只是看多了, 腦袋自動學起來. 這就是 convention 的擴散過程: 多數方向變預設, 少數方向變成「需要特別解釋」.

這篇做兩件事: **一次把五個記號講完**, 再給一組**抗體** — 想保留自己的寫法, 不能靠「寫自然一點」這種模糊指令, 要主動擋.

---

## 五個記號一次講完 — The Five Tells

### 1. `——` (雙破折號)

[Day 16](./day16-ai-fingerprint-scan.md) 單指紋掃了全屆鐵人賽, 全體每千字 0.86. 這是 AI 寫作**最頑固的指紋**: markdown 訓練資料洩漏到散文, RLHF 又獎勵「看起來工整」的文字, 兩層放大疊成鐵指紋. 但 ChatGPT & Codex 組**全體最低 0.11**, 因為 GPT-5.1 (2025-11) 開始能遵守「不要用 em-dash」的 custom instruction. 這個記號對 GPT 生態已經半失效, 但對 Claude / Gemini 還成立.

### 2. 「不是 X 而是 Y」(對立句)

[Day 18](./day18-detecting-pure-human.md) 的 lab01 V08C ranker 掃的就是這個. 全體每千字 0.28, 不算高但分布集中. 典型用法:

- 不是功能的問題, 而是體驗的問題
- 不只是學會一個工具, 更是培養思維

AI 愛用對立句, 因為對立感**乾淨、有力、聽起來果斷**. 一句話把 X 跟 Y 的差異擺出來, 讀者不用自己想. RLHF 評分者看到會給高分, 模型就學起來.

但同樣的句型, 技術文章在**糾正讀者誤解**時很自然會用. 單一出現不是證據, 密集出現才是.

### 3. 「最容易...的」(教學警告腔)

[Day 18](./day18-detecting-pure-human.md) 的 lab01 V09 ranker 掃的就是這個, corpus 覆蓋率只 6.9%, 但命中就極強. 熱門命中詞:

- 最容易被忽略的 (全 corpus 112 次)
- 最容易犯的 (81 次)
- 最容易漏掉的 (34 次)
- 最容易踩的 (33 次)
- 最容易出錯的 (29 次)

這組是 AI 教學文典型腔調 — 告訴你**最常見的錯誤 / 最容易踩的坑**. 寫作上像在幫讀者整理考點, 聽起來細心, 但用多了就定型. 跟破折號比, 這個記號**比較難被 custom instruction 壓掉**, 因為它看起來「有內容」, 不是純裝飾.

### 4. 排版堆料 (粗體 / blockquote / `<hr>` / emoji)

[Day 17](./day17-composite-score.md) 的 lab01 V06B 綜合分數抓的就是這個. 單一元素都正常用, 但**堆在一起**就是 AI markdown-heavy 排版風:

- 每段都塞粗體標籤
- 大量 `<hr>` 分章節
- blockquote 擺觀點
- emoji 散滿全文

AI 愛這樣排, 因為訓練資料大多是 markdown. **人寫久了也會模仿** — 你看別人這樣排版版面好看, 自己也學. 這就是 convention 擴散的過程.

### 5. 標題用 `｜` 當分隔符

[Day 18](./day18-detecting-pure-human.md) 順便測的 lab01 V11 ranker. 鐵人賽全體 34.8% 文章標題有 ｜, 但 **ChatGPT & Codex 組高達 64.1%** (全體 2x).

對照 ChatGPT 組 em-dash 密度全體最低 (0.11), 推論是:

- GPT-5.1 的 custom instruction 把 em-dash 壓掉了
- 但「標題 ｜ 分隔符」不在 custom instruction 管理範圍
- GPT 的「標題 AI convention」從 `——` 遷移到 `｜`

這叫 **em-dash → pipe shift**. AI tell 不會消失, 只會變形到工具管不到的地方.

---

## 把味道壓回去的抗體 — The Antidote

既然這些記號會變 convention, 想保留自己的寫法要**主動擋**. [jason3e7-voice](../../../../skills/jason3e7-writing-voice.md) 就是我寫的個人 anti-convention 手冊, 核心規則:

### Do

- **半形標點**: 中文句子用半形逗號、半形句號, 中英之間空一個半形空格
- **動詞開頭、主詞省略**: 「先透過 X 確認 Y」而非「我們先透過 X 來確認 Y」
- **一句一件事**: 多用逗號串短子句, 讓每個子句自己站得住
- **英文技術詞保原大小寫**: 寫 nmap, ssh, port, 不用全形, 不加 backtick
- **保留系列固定句**: writeup 類文章沿用「先用 nmap 確認」「使用 python 取得 pty shell」這種熟悉開頭

### Don't

- **不用破折號** (`——` 或 `—`): 這是 AI 最強辨識指紋
- **不用「其實」「值得注意的是」「換句話說」「更在於」「進一步而言」**: AI 常見冗詞
- **不用「首先 / 接著 / 最後」湊三段節奏**
- **不用「不只是 X, 而是 Y」對立句**
- **不加沒必要的譬喻**: 譬喻要能幫理解, 不是文采裝飾

這組規則**不是因為 convention 本身錯**, 是因為「變成預設」等於失去選擇. 我想自己決定要不要用, 不是讓訓練資料替我決定.

---

## 子系列小結 (Day 16-20) — The Arc

| Day | 題目 | 核心問題 | 工具 |
|---:|:---|:---|:---|
| 16 | 掃當屆鐵人賽, 量一次 AI 味 | 單指紋夠不夠 | lab01 V01 (`——` density) |
| 17 | 從 1 個訊號擴到 6 個 | 怎麼綜合排版訊號 | lab01 V06B (B_total × N_sum) |
| 18 | 翻面問題: 哪些文章是純手寫 | 能不能證明「沒碰 AI」 | lab01 V12 (9 signal 合併) ＋ Tampermonkey |
| 19 | 浮水印怎麼運作, 為什麼不是鐵證 | 官方標記能證明嗎 | Claude 嵌入式浮水印 ＋ C2PA |
| 20 | AI 文風入侵 (本篇) | 這些 pattern 怎麼變成 convention | 五個記號 ＋ voice guide 抗體 |

這五天其實在繞一個問題: **「這段字是 AI 寫的嗎」這個問題本身能不能回答**.

**答案是不能**, 但可以回答兩個相關問題:

1. **某段字有幾個 AI tell 命中?** 可以. lab01 V12 給你數字.
2. **某篇文章風格有沒有被 convention 入侵?** 可以. 看它踩了幾個記號.

至於「這段字是 AI 寫的嗎」本身, 浮水印不能回答 (查到 ≠ AI 寫, 沒查到 ≠ 人寫), 共現訊號也不能回答 (ChatGPT 組 em-dash 幾乎 0, 但那不代表他們沒用 AI). 這個問題在**認識論上就是不可解**的 (至少以目前的工具).

---

## 我的重點 — Takeaways

- **convention 不是對錯問題, 是數量問題**. 時鐘往反方向轉的人, 不是錯, 只是變少數
- **想保留自己的風格要主動擋**, 「寫自然一點」這種模糊指令沒用, 要具體列 Don't
- **五個記號不是 AI 專屬**, 人寫久了也學起來. 真正的訊號是**成群出現 + 沒有具體擔當的判斷**
- **「這段字是不是 AI 寫的」這個問題不可解**. 可解的是「這段字有幾個 AI tell 命中」, 跟「這篇風格有沒有被 convention 入侵」. 這兩個都是量化問題, 答得清楚
- **這組 lab (Day 16-20) 真正的收穫, 是知道工具的邊界在哪**. 做出來的 ranker 很好用, 但要知道**它不是判決器, 是放大鏡**

下一組會換個主題: Day 21 開始聊**實作面** — 權限、`/goal`、hooks、workflow、HTB 靶機實測. 從「文風觀察」拉回「怎麼用 Claude Code 做事」.

---

## Sources

- [Day 16: 掃當屆鐵人賽, 量一次「AI 味」有多少](https://ithelp.ithome.com.tw/articles/10419414)
- [Day 17: 從 1 個訊號擴到 6 個, 綜合分數再測「AI 味」](https://ithelp.ithome.com.tw/articles/10419809)
- [Day 18: 翻面問題: 哪些文章是純手寫](https://ithelp.ithome.com.tw/articles/10420151)
- [Day 19: 浮水印怎麼運作, 為什麼不能當證據](./day19-ai-watermark.md)
- [AI 的文風與語氣: 五個記號的詳細拆解](../../../../02-advanced/writing-style/ai-writing-style-tells.md)
- [jason3e7-writing-voice skill: 本篇講的「抗體」原始版](../../../../skills/jason3e7-writing-voice.md)
- [為什麼時鐘順時針轉 (Wikipedia: Clockwise)](https://en.wikipedia.org/wiki/Clockwise#Origin_of_the_convention)
- [時鐘順時針的起源 — 關鍵評論網](https://www.thenewslens.com/article/111804)
