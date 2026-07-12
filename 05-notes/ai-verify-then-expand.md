---
title: AI 與未來：先驗證，再用它突破自己（Draft）
tags: [ai, 個人筆記, 驗證, 判斷力, 拓展, 學習, 觀點, 草稿]
created: 2026-07-11
updated: 2026-07-12
status: draft
---

# AI 與未來：先驗證，再用它突破自己 — Verify AI First, Then Break Past Yourself

[← 回主頁](../index.md)

> [!NOTE]
> 草稿 · jason3e7 的思考。AI 最核心的特性，是它會**產出超出你給它的東西**——超出輸入、超出你已知的範圍。這件事有兩面：當你要真相，它是**風險**（可能超出事實 = 幻覺）；當你要成長，它是**禮物**（能超出你的想像天花板）。這篇分兩線談：**先學會驗證（基本功），再用它拓展自己**。

> **TL;DR (EN):** AI's core trait is going beyond what you give it. Two faces: a risk when you want truth (verify it), a gift when you want to grow (use it to expand). Verification comes first — it's the safety rope that lets you venture into domains you don't yet understand. And AI is a multiplier, not an adder: it amplifies the capability you already have (experts +45% vs generalists +20%), so 0 × AI is still 0 — the path to new earning power is *learn enough to judge, then amplify*.

**為什麼兩件事放同一篇**：它們是同一個特性的兩面。而且有先後——**驗證是基本功**，因為 AI 帶你去的新地方，正是你最無法判斷對錯的地方；不會驗證，就不敢也不該往未知走。

---

## 線一：驗證 AI 產出（基本功）— Verify

問題本質和數學一樣：**數學對付「證明看起來對，但可能有漏」，AI 對付「答案看起來對，但可能是幻覺」**——都是在打「表面說服力」。數學磨了幾百年的驗證方法論，可以直接借來當日常姿勢。

### 六個手段（數學借來的）

每列是一個數學驗證手段，兩欄告訴你**prompt 那頭要求什麼**、**答案回來後自己做什麼**。

| 數學方法 | 寫 prompt 時要求它 | 收到答案時自己做 |
|---|---|---|
| **列步驟**（proof，證明過程） | 「先列推理步驟再給結論，不要跳」 | 檢查每一步之間有沒有跳、偷渡假設 |
| **列假設** | 「列出這結論依賴的假設」 | 看那些假設在你的實際情境是否成立 |
| **給依據**（citation） | 「每個斷言附可查證來源（數字/引用/時間）」 | 隨機抽一項回頭查（尤其數字與人名） |
| **反例**（counterexample） | 「舉一個能推翻這結論的情境」 | 自己想一個「如果 X 不是這樣」的反例 |
| **極端／邊界**（edge case） | 「代入 0、極大值、負數、空集合會怎樣？」 | 用極端輸入代進去看它會不會崩 |
| **獨立驗算**（最強一招） | ⚠️（寫進 prompt 沒用） | **換問法或換模型再問一次，對得起來才信** |

**獨立驗算**值得特別講：這是最強也最省事的一招（數學裡「加法用減法驗」就是它）。同一個模型、同一個 session 再問，它會傾向重複剛才的說法；真正的獨立驗算得**跳出這個 session**——換問法、換模型（Claude 問完問 Gemini）、或直接查權威來源。對重要答案，這一招比其他五招加起來更能抓錯。

### 什麼時候不用跑全套

不是每題都驗，否則就變儀式、沒法融入日常。快速判準：

- **可逆 vs 不可逆**：發文、寄信、下單、簽約 → 驗；隨手查、腦力激盪 → 不用
- **有沒有別人依賴這答案** → 有 → 驗
- **我自己判斷得了嗎** → 得了 → 不用

大原則：**越接近「一旦錯了就麻煩」，越該把六招搬出來——尤其獨立驗算。**

---

## 線二：用 AI 拓展自己 — Expand

人很難做到「想像之外」的事——你的學習被你當下的地圖框住，看不到地圖外的路。AI 剛好能延伸這張地圖：它會超出你已知，把你不知道自己不知道的東西攤到眼前。

### 五個手段：破知識舒適圈

| 手段 | 怎麼做 | 破的是什麼 |
|---|---|---|
| **問出未知的未知** | 「關於 X，我這種程度的人**不會想到要問**什麼？」 | 你不知道自己不知道的 |
| **下一步階梯** | 「懂了這個，自然接下來該探索什麼？」 | 學習天花板（不知往哪爬） |
| **借外領域視角** | 「生物學家／經濟學家會怎麼看這問題？」 | 單一學科的框架慣性 |
| **主動找不舒服** | 「給我最能反駁我的論點／我最難接受的觀點」 | 確認偏誤、同溫層 |
| **踩在能力邊緣**（ZPD，最近發展區） | 讓 AI 出「比你現在略難一點」的題，一路往上拉 | 停在會的東西不動 |

### 賺錢的關鍵：AI 是放大器，0 乘再多還是 0

這是「拓展」最現實的引擎。研究講得很直白：**AI 放大你已有的能力，不會替你憑空生出能力。**

- 技術專家用 AI 表現 **+45%**，一般員工只 **+20%**——放大倍率跟你的**領域底子**成正比
- 「AI amplifies what you already know. If you know little, even perfect prompts produce shallow results.」

**用你的借貸例子**：不懂借貸規則的人，就算有 AI 也賺不到這方面的錢——因為 (1) 你不知道要問什麼；(2) AI 給的分析你**無法判斷對錯**；(3) 一旦照著錯的做，AI 讓錯誤**放大得更快**（好決策放大成大賺，壞決策放大成大賠）。反過來，懂借貸的人能用 AI 一次分析大量案子、跑情境、找機會——**同一個 AI，槓桿天差地遠。**

> [!IMPORTANT]
> 那「突破原本沒有的能力」怎麼辦？**路徑是：先用線二的學習，把某領域的底子從 0 拉到「足以判斷」，跨過門檻，AI 的放大器才開始對你有用。** 跳過學習那一步，就永遠卡在 `0 × AI = 0`。所以拓展不是「叫 AI 替你懂」，是「用 AI 加速學到你能判斷」，再放大。

### 陷阱：別讓 AI 變成新舒適圈

線二有個和線一對稱的危險：**被動接受 AI 的第一個答案，它就變成你的新舒適圈**——你以為在拓展，其實只是換一個更大的籠子（呼應 [發散多樣性收窄](../02-advanced/four-capabilities-playbook.md) 那條：94% 點子同一核心）。更糟的是，AI 太好用會讓人**連現有舒適圈都不想離開**（反正問它就好，何必自己學）——那是把成長本身外包出去。**破舒適圈必須主動要分歧、要不舒服，不是躺著讓它餵。**

---

## 收尾：驗證是拓展的前提 — The Loop

兩條線在這裡接起來：

**拓展 = 把自己丟進不熟的領域**（要賺錢、要成長，就得進新地方）。而**新領域正是你最不會判斷對錯的地方**——你在那裡是新手。KB 裡的 BCG 研究說得很清楚：AI 出錯的前沿，**新手掉 19 分，能識破瑕疵的專家反而勝**（見 [知識壁壘那篇](./ai-and-knowledge-barriers.md)）。

所以**驗證不是拓展的對立面，是它的安全繩**：

```
先練驗證（基本功） → 用 AI 學到「足以判斷」（拓展） → 才能安全放大（賺錢／成長）
```

沒有驗證能力就衝進新領域用 AI 下重注，是加速虧損；有了它，你才敢往地圖外走，而且走得穩。

**一句話**：AI 時代，人的價值不在「知道什麼」，在於**能判斷、且敢往未知走還走得穩**。

---

## 相關筆記 — Related

- [AI 打破知識壁壘：被夾殺的知識中產](./ai-and-knowledge-barriers.md) —— 為什麼判斷力與拓展力變成關鍵
- [用 Prompt 生成好 Prompt](./meta-prompting.md) —— 線一「寫 prompt 端」的具體實作（5 要素 = 列假設 + 列步驟 + 給依據）
- [AI 能力全景圖](../02-advanced/ai-capability-landscape.md) —— 忠實度梯度：幻覺是 bug（線一）還是 feature（線二）

## Sources

- [AI Skills Multiplier: Expert Developers Know — SquaredTech](https://www.squaredtech.co/ai-skills-multiplier-the-shocking-truth-about-developer-productivity)
- [The Knowledge Paradox: Why AI Without Domain Knowledge Is Worthless — Carlo Alberto Cuman](https://medium.com/@carlo.cuman/the-knowledge-paradox-why-ai-expertise-without-domain-knowledge-is-worthless-f1867fea16a0)
- [AI as Cognitive Amplifier: Rethinking Human Judgment — arXiv](https://arxiv.org/html/2512.10961v1)
- [Why AI Makes Human Judgment Priceless — Forbes Tech Council](https://www.forbes.com/councils/forbestechcouncil/2025/11/07/why-ai-makes-human-judgment-priceless-and-how-to-scale-it/)
- [What Quant Investing Looks Like in 2026: Data, AI, and Human Judgment — AXA IM](https://core.axa-im.com/investment-strategies/equities/insights/what-quant-investing-looks-2026-data-ai-and-human-judgment)
- [The Economics of Human–Machine Complementarity](https://drdawoodmamoon.substack.com/p/the-economics-of-humanmachine-complementarity)
