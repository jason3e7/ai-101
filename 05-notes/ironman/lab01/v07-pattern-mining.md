---
title: lab01 V07 pattern mining — 從 V06B / V01 top 文章挖中文 AI tell
created: 2026-10-01
---

# V07 Pattern Mining — Where Are the Chinese AI Tells?

> [!NOTE]
> V07 想找中文 AI tell. 一個直覺思路: 掃 V06B / V01 排名最高的文章 (推定最 AI 味), 做中文 n-gram, 跟 jason3e7 publish (推定清乾淨) 比對, 找「top 文章常用、我不用」的中文 pattern. 跑完發現: 鐵人賽 corpus 的 AI 味**不走典型翻譯腔 cliche 路線**, 我 v07-preview.md 整理的「換句話說 / 值得注意的是 / 綜上所述」等清單在 top 文章**全軍覆沒**. 這篇記錄過程 + 推翻假設 + 修正 V07 路線.

> **TL;DR (EN):** Scanned Chinese n-grams in top 3 articles of V06B (markdown-heavy formatting signature) and V01 (em-dash prose signal), compared against jason3e7's publish day01-15. Both tops have essentially **zero classic Chinese AI cliches** like 換句話說 / 綜上所述 / 本質上 / 值得注意的是. The "AI cliche list" I compiled in v07-preview.md (sourced from my training impression) does not match this corpus. V06B top 3 vocab is dominated by domain terms (Azure cert prep). V01 top 3 has only 8 maximal n-grams meeting 3+ threshold. **Takeaway**: Taiwanese tech writers using AI (Claude/GPT) in Traditional Chinese produce output that evades the obvious cliches. V07 needs a different strategy: look at structural patterns, lexical diversity, or specific locally-popular phrases rather than translated-essay-flavor cliches.

```markdown
# V07 Chinese pattern mining
* 方法
  * V06B top 3 + V01 top 3 vs jason3e7 publish
  * n-gram 4-10 字元, count >=3, maximal
  * 加上已知 AI 冗詞清單對照
* 結果
  * V06B top 3 幾乎都領域詞 (Azure cert)
  * V01 top 3 只找到 8 個 maximal n-gram
  * 已知 AI 冗詞清單全軍覆沒
* 推測
  * 清單是翻譯腔 (大陸簡體) cliche
  * zh-TW AI 輸出避開這些
  * 鐵人賽 context 是技術非論述
* V07 路線修正
  * 不要靠 pre-made cliche
  * 試結構 / 詞彙多樣性
  * 或 corpus-derived (無 pre-made)
```

---

## 方法 — Method

**資料來源**:
- V06B top 3 (排版 AI 味最高): aid 10405969 (az-900 Day6), 10401963 (Phoenix Day12), 10401261 (JS 核心重構 Day02) — 共 10,598 中文字
- V01 top 3 (em-dash prose 最高): aid 10411391 (用 Claude 打造軟體產品 Day01), 10416320 (白稜 Day12), 10402332 (mpv 懶人包 Day25) — 共 3,788 中文字
- jason3e7 publish day01-15 — 共 23,843 中文字

**掃描流程**:
1. 從 `raw/pages/{aid}.html` 讀正文, 去 `<pre>`, 去標籤, 保留純文字
2. 抽連續中文字串 (`[一-鿿]{3,}`)
3. 掃 n-gram 長度 4-10
4. Count >= 3 的候選, pub 命中 0 或 top rate / pub rate > 3 的篩入
5. Maximal 過濾 (去掉是更長 n-gram 子字串的)
6. 加碼: 對照 v07-preview.md 的「已知 AI 冗詞清單」(50+ 條)

---

## V06B top 3 掃描結果 — Mostly Azure Vocabulary

跑完 89 個候選 maximal n-gram, 看前面幾個就知道**大多是領域詞**:

| n-gram | V06B top 3 次數 | per_1k | publish 次數 | per_1k |
|:---|---:|---:|---:|---:|
| 訂用帳戶 | 16 | 1.51 | 0 | 0.00 |
| 虛擬機器 | 14 | 1.32 | 0 | 0.00 |
| 處理時間 | 12 | 1.13 | 0 | 0.00 |
| 五大支柱 | 11 | 1.04 | 0 | 0.00 |
| 計畫性維護 | 11 | 1.04 | 0 | 0.00 |
| 控制平面 | 10 | 0.94 | 0 | 0.00 |
| 等待時間 | 9 | 0.85 | 0 | 0.00 |
| 基礎設施 | 8 | 0.75 | 1 | 0.04 |
| 維護排程 | 8 | 0.75 | 0 | 0.00 |
| 低使用率 | 7 | 0.66 | 0 | 0.00 |

**全部是 Azure 認證教材詞彙**. 這是因為 V06B top 1「使用gemini 準備 az-900 Day6」是 Azure cert prep 內容, 專有名詞 dominate.

少數偏「AI 味」感的候選:

| n-gram | V06B | per_1k | pub | note |
|:---|---:|---:|---:|:---|
| 拆解與解析 | 6 | 0.57 | 0 | 「拆解」+「解析」是 AI 愛的分析動詞組合 |
| 深度解析 | 5 | 0.47 | 0 | AI 教學典型句型 |
| 考點解析 | 5 | 0.47 | 0 | 同上 |
| 核心功能 | 5 | 0.47 | 0 | 「核心」是 AI 常用 superlative |
| 卓越營運 | 6 | 0.57 | 0 | 跟 Microsoft Well-Architected 有關, 但也很 AI |

但**全部都還是跟 Azure 考題強相關**, 很難獨立成 general AI tell.

---

## V01 top 3 掃描結果 — Even Fewer Hits

V01 top 3 (em-dash prose 最高) 中文字只 3,788, 候選 maximal n-gram **僅 8 個**:

| n-gram | V01 top 3 | per_1k | pub | per_1k |
|:---|---:|---:|---:|---:|
| 軟體產品 | 5 | 1.32 | 4 | 0.17 |
| 產品開發 | 4 | 1.06 | 0 | 0.00 |
| 工程師的 | 4 | 1.06 | 0 | 0.00 |
| 每一步都 | 4 | 1.06 | 0 | 0.00 |
| 產品發想 | 3 | 0.79 | 0 | 0.00 |
| 維志旁邊 | 3 | 0.79 | 0 | 0.00 |
| 每一步都有 | 3 | 0.79 | 0 | 0.00 |
| 一切看起來 | 3 | 0.79 | 0 | 0.00 |

這幾個 (每一步都 / 一切看起來) 感覺比較像 AI 的句型, 但只有 top 1「用 Claude 打造軟體產品」這一篇集中使用, **不是跨多作者的 pattern**.

「維志旁邊」是人名+方位, 不是 AI tell.

---

## 已知 AI 冗詞清單對照 — Complete Wipeout

對照 v07-preview.md 整理的 50+ 條 AI 冗詞:

| phrase | V01 top3 | V06B top3 | publish | 評語 |
|:---|---:|---:|---:|:---|
| 換句話說 | 1 | 0 | 1 | 大家都 1, 無差別 |
| 舉個例子 | 0 | 0 | 3 | **publish 多於 top!** |
| 首先 | 0 | 1 | 0 | 低頻 |
| 然後 | 8 | 1 | 4 | V01 top 1 的作者愛用, 不是 AI 專屬 |
| 最後 | 2 | 1 | 8 | **publish 用得最多** |
| 之所以 | 0 | 0 | 1 | 低頻 |
| **其他 44+ 條** | **全部 0** | **全部 0** | **全部 0 或 0-1** | — |

**沒命中的清單**: 值得注意的是 / 值得一提的是 / 具體而言 / 進一步而言 / 綜上所述 / 不僅如此 / 這樣一來 / 一言以蔽之 / 在這個過程中 / 這意味著 / 不容忽視 / 尤為重要 / 從本質上 / 本質上 / 無庸置疑 / 顯而易見 / 由此可見 / 深入探討 / 至關重要 / ... (全部都是 0)

**這是重大翻盤**: 我當初在 v07-preview.md 列的清單**對這個 corpus 幾乎沒用**.

---

## 推測原因 — Why the List Fails

幾個可能:

1. **清單是翻譯腔 cliche** — 「換句話說 / 綜上所述 / 本質上 / 值得注意的是」這些比較像**大陸簡中 AI 輸出的 register**, 也可能來自英文教科書式論述翻譯 (「In other words / In summary / Essentially / It's worth noting that」)
2. **zh-TW 的 AI 輸出避開這些** — Claude / GPT 在繁中模式下可能有不同的 register, 用更口語化的表達, 不走正式論述風
3. **鐵人賽 context 是技術文章, 不是論述文** — AI 冗詞清單裡大半是「論述過渡詞」(透過第 X 點加強論點那種), 技術文章本來就少用
4. **我清單本身就不準** — 我是「憑印象」整理的, 可能把英文的 AI tell 概念直接套到中文, 選錯類別

對照 Kobak 2025 的英文清單 (delve / underscore / meticulous / tapestry): 這些是**單詞**層面的 overrepresentation. 中文如果有對應, 應該也是單詞, 不是**句型冗詞**.

---

## 對 V07 的啟示 — Pivoting V07

這次 pattern mining 給 V07 的方向修正:

### 不走的路

- **預先列中文 AI 冗詞清單**: 證明對這個 corpus 無效. v07-preview.md 的清單**作為英文 Kobak 的中文對應推測可以保留**, 但不要當 V07 的實作 baseline
- **直接套用英文 AI tell 的翻譯**: 中英文的 AI 輸出 register 不同, 不能 1-to-1 對應

### 可以走的路

**路 A: 結構層面 pattern**
- 句子長度分佈 (AI 偏長句)
- 段落結構 (每段一個標題 / 每段固定長度)
- 標點混用 (全形半形混亂, 「：」vs「:」)

**路 B: 詞彙多樣性 (lexical diversity)**
- Type-Token Ratio (TTR)
- Yule's K
- 中文斷詞後計算
- 低多樣性 = AI (重複用一樣詞彙)

**路 C: 真正的 corpus-derived**
- 走 Kobak 方法論: 2020 pre-ChatGPT 中文 corpus 當 baseline, 2026 corpus 找 excess vocabulary
- 要時間找 baseline corpus 也要處理中文斷詞, 但這條做出來才是 novelty

**路 D: 以作者為單位的風格一致性**
- 一個系列 30 篇, 如果**句型變異過低** (每天差不多的句子結構), 可能是 AI pattern 複製
- 需要 sequence-level feature

**推薦起點**: 路 B (TTR). 工具成熟 (jieba 或 CKIP), 數字好解釋, 跟詞彙清單無關, 不帶主觀偏見.

---

## Related

- [v07-preview.md](./v07-preview.md) — 原本的 AI 冗詞清單推測 (保留作英文對照, 但清單本身對這 corpus 無效)
- [v07-scan-mycorpus.py](./v07-scan-mycorpus.py) — jason3e7 publish 的掃描腳本
- [v06-vs-v06b.md](./v06-vs-v06b.md) — V06 vs V06B 排名對照
- [composite-score-v06.md](./composite-score-v06.md) — V06 原始設計
