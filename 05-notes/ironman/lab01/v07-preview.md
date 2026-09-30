---
title: lab01 V07 preview — 掃 jason3e7 自己文章的重複中文 n-gram
created: 2026-09-30
---

# V07 preview — 掃自己文章的重複中文 n-gram

> [!NOTE]
> V07 的目標是**中文 AI 冗詞 / 句型偵測** (前後文 aware, 比 V01–V05 的字元/符號 signal 難). 動手前先掃 jason3e7 自己 publish 的 day01-15 (共 15 篇), 看哪些中文 n-gram 出現 3 次以上. 理由: 你的文章是**「已經被 AI 改寫過但仍保有你風格」**的樣本, 掃出來的重複可以分兩堆:
> - **你的個人風格 / 領域術語** → V07 白名單, 別誤判成 AI
> - **AI 冗詞漏網** → V07 要抓的目標

## 掃描條件

- 範圍: `05-notes/ironman/publish/day01-15/day*-ithome.md` × 15 篇
- 去除: code fence, inline code, HTML tag, markdown 語法, callout marker, hr, frontmatter
- 中文字元: 21,567 字 (3,175 個中文字串塊)
- n-gram 長度: 4–10 個中文字元
- 過濾: `count >= 3` 且 maximal (不是某個更長 n-gram 的子字串)
- 加碼: 對照一份**已知 AI 冗詞清單** (64 條)

執行腳本: [`v07-scan-mycorpus.py`](./v07-scan-mycorpus.py)

---

## 結果 1: Data-driven top n-grams

出現 3 次以上的 maximal n-gram, count DESC. 我把 148 個結果分三類讀 (人工分類, 未寫死):

### A. 你自己的概念 / 術語 (V07 白名單候選)

這些是你反覆使用的**自創詞彙或系列梗**, 不算 AI slop, 是你風格的一部分:

| n-gram | 次數 | 分類 |
|:---|---:|:---|
| 獨立驗算 | 10 | 你的核心概念 |
| 心智清單 | 6 | 你的工具/概念 |
| 五招破法 | 6 | day 某天的框架 |
| 破舒適圈 | 3 | 你的概念 |
| 高階收斂 | 3 | 你的概念 |
| 認知外包 | 3 | 你的概念 |
| 拓展自己 | 4 | 你的用語 |
| 知識中產 | 3 | 你的分類 |
| 頂層專家 | 3 | 你的分類 |

### B. 領域術語 (中性, 不是 AI slop)

| n-gram | 次數 |
|:---|---:|
| 推理模型 | 6 |
| 強化學習 | 4 |
| 鋸齒狀前沿 | 4 |
| 模型內部 | 4 |
| 模型自己 | 4 |
| 機率機器 | 3 |
| 機率分布 | 3 |
| 神經連結 | 3 |
| 任務向量 | 3 |
| 隱性知識 | 3 |
| 猜下一個字 | 4 (+ 相關變體 6+) |

### C. 你反覆用的表達方式 (值得注意, 可能是 filler)

這幾組是你**同一想法的多個變體**, 反覆出現. 有些是文風, 有些接近 filler pattern:

**「什麼X」高頻**:

| n-gram | 次數 |
|:---|---:|
| 什麼時候 | 12 |
| 什麼時候該 | 5 |
| 是什麼(的是什麼 / 是這一層) | 3+ |
| 什麼都沒 | 4 |
| 什麼任務 | 3 |
| 這是什麼 | 3 |
| 自己什麼 | 3 |

**「為什麼X」高頻**:

| n-gram | 次數 |
|:---|---:|
| 為什麼一 | 4 |
| 為什麼講 | 4 |
| 是為什麼 | 4 |
| 它為什麼 | 4 |
| 為什麼講這個 | 3 |

→ 你常用問句起頭 (「什麼時候該...」「為什麼講...」), 是明顯個人風格.

**「XX 的東西」pattern**:

| variant | 次數 |
|:---|---:|
| 到的東西 | 5 |
| 練出來的東西 | 5 (含變體) |
| 拿到的東西 | 3 |
| 學過的東西 | 3 |
| 出來的東西 | 3 |
| 來的東西 | 4 |

→ 你愛用「XX 的東西」代替「XX 事物」, 一種口語化風格. 這算個人 filler.

**「一個/一次」量詞**:

| n-gram | 次數 |
|:---|---:|
| 一個問題 | 4 |
| 一個模型 | 4 |
| 一次對話 | 4 |
| 一個動作 | 3 |
| 一個主題 | 3 |
| 一個字的機率 | 3 |
| 的一句話 | 3 |

**「最...的」形容**:

| n-gram | 次數 |
|:---|---:|
| 最危險的 | 3 |
| 最重要的 | 3 |
| 最好的那 | 3 |

---

## 結果 2: 對照已知 AI 冗詞清單

清單共 64 條. 你的 15 篇文章命中如下 (`★` = 3 次以上):

| 冗詞 | 次數 | |
|:---|---:|:---|
| 的話 | 11 | ★ (常見 filler, 不一定是 AI) |
| 最後 | 8 | ★ (常見, 中性) |
| 然後 | 4 | ★ (口語 filler) |
| 舉個例子 | 3 | ★ (AI cliche, 唯一明確命中) |
| 的方式 | 3 | ★ (中性) |
| 換句話說 | 1 | |
| 之所以 | 1 | |
| 首當其衝 | 1 | |

**觀察**: 你的中文寫作出奇乾淨. 64 條經典 AI 冗詞清單裡只有 8 條有命中, 3 次以上只 5 條, 其中只有「**舉個例子**」是明確的 AI 味 filler. 其他 4 條 (的話 / 最後 / 然後 / 的方式) 都是**任何中文寫作都會有的自然出現**.

**沒命中的**清單包含: 換句話說(僅 1)、值得注意的是(0)、值得一提的是(0)、具體而言(0)、進一步而言(0)、綜上所述(0)、不僅如此(0)、本質上(0)、無疑(0)、無庸置疑(0)... 這些是最典型的 AI slop, 你完全避開了.

---

## 補: 已知 AI 冗詞清單的來源 — Where Do These Lists Come From?

### 中文清單的來源 — 坦白說沒有

前面用的那份「已知 AI 冗詞清單」(64 條) 是**我根據訓練經驗手動整理的**, 沒有引用單一權威來源. **目前中文 LLM detection 領域沒有等同於英文 Kobak 研究那種系統性大規模統計的清單**.

有的中文相關資源, 但都不列具體特徵詞:

- **NLPCC 2025/2026 Task 6** — LLM-Generated Text Detection (澳門大學 NLP2CT lab, 22 隊參賽), 但重點是 classifier 效果排名, 沒公開特徵詞清單
- **DetectRL-ZH** 資料集 (同組) — 有標註但沒詞頻分析
- **WaveDetect** (arXiv:2506.23336) — 小波變換方法, 語言中性, 不列詞

「中文 AI 冗詞的系統性清單」目前是空白, 你這個 lab 的中文清單本身就是**可以拿去發表的 novelty**. 可以參考英文的方法論反推中文.

### 英文有權威清單: Kobak et al. Science Advances 2025

英文有一份目前引用最多、方法最扎實的清單:

> Kobak, D., et al. (2025). "Delving into LLM-assisted writing in biomedical publications through excess vocabulary." *Science Advances*.
> [DOI](https://www.science.org/doi/10.1126/sciadv.adt3813) · [arXiv 預印本](https://arxiv.org/abs/2406.07016) · [GitHub 完整 CSV](https://github.com/berenslab/llm-excess-vocab)

**方法論核心**: 分析 1500 萬篇 PubMed 摘要 (2010-2024). 比較「ChatGPT 出現前」的年份與「ChatGPT 出現後」的年份, 用類似流行病學「超額死亡」的統計法找出**特定詞的頻率異常上升** — 稱為 excess vocabulary. 共列出 **900 個 excess words**, 全部開源.

這比人工列 cliche 客觀得多, 也是目前唯一有「頻率變化 vs 時間」這種 empirical evidence 的 AI 詞頻研究.

**RLHF 起源分析**: [arXiv 2412.11385](https://arxiv.org/abs/2412.11385) 專門追 "delve" 的來源 — 指出 RLHF 標註者背景 (奈及利亞英語愛用 "delve") 直接影響了 ChatGPT 的詞頻分布. 這解釋了為什麼很多 AI cliche 有一致的「異國化學術腔」感覺.

### 英文 AI 冗詞 30+ 條 (Kobak + 交叉驗證)

依類別整理, r = Kobak 論文的頻率比率 (越高越明顯):

| # | 詞 / 片語 | 類別 | 頻率變化 / 來源 |
|:---|:---|:---|:---|
| 1 | **delve / delves** | pretentious verb | Kobak r=25.2 (最高) |
| 2 | **showcase / showcasing** | verb | Kobak r=9.2 |
| 3 | **underscore / underscores** | emphasis verb | Kobak r=9.1 |
| 4 | **intricate** | adjective | Kobak |
| 5 | **meticulous / meticulously** | adverb/adj | Kobak |
| 6 | **realm** | pretentious noun | Kobak |
| 7 | **pivotal** | emphasis adj | Kobak |
| 8 | **crucial** | intensity marker | Kobak |
| 9 | **notably** | transition | Kobak |
| 10 | **particularly** | transition | Kobak |
| 11 | **additionally** | transition | Kobak |
| 12 | **comprehensive** | corporate adj | Kobak |
| 13 | **enhance / enhancing** | vague verb | Kobak |
| 14 | **insights** | vague noun | Kobak |
| 15 | **tapestry** | pretentious noun | Kobak (低量高比率, 已成迷因) |
| 16 | **unwavering** | emphasis adj | Kobak |
| 17 | **commendable** | emphasis adj | Kobak |
| 18 | **robust** | corporate adj | Max Planck +50% (Kobak) |
| 19 | **holistic** | corporate adj | 二級來源共識 |
| 20 | **multifaceted** | corporate adj | 二級來源共識 |
| 21 | **seamless / seamlessly** | corporate adj | 二級來源共識 |
| 22 | **leverage** | pretentious verb | 二級來源共識 |
| 23 | **utilize** (代替 use) | pretentious verb | 二級來源共識 |
| 24 | **facilitate** | pretentious verb | 二級來源共識 |
| 25 | **navigate the complexities of** | metaphor phrase | 二級來源共識 |
| 26 | **foster** | vague verb | 二級來源共識 |
| 27 | **harness** | vague verb | 二級來源共識 |
| 28 | **elucidate** | pretentious verb | 二級來源共識 |
| 29 | **embark on a journey** | metaphor phrase | 二級來源共識 |
| 30 | **testament to** | phrase | 二級來源共識 |
| 31 | **cornerstone** | metaphor noun | 二級來源共識 |
| 32 | **paradigm shift** | pretentious noun | 二級來源共識 |
| 33 | **furthermore / moreover** | transition | Kobak + 各家 |
| 34 | **"It is important to note that…"** | hedging phrase | 各家共識 |
| 35 | **"In conclusion, …"** | template phrase | 各家共識 |
| 36 | **"In today's fast-paced world…"** | opening cliché | 二級來源共識 |

「二級來源共識」= 多個部落格 (SlopDetector、ContentBeta、HumanizeThisAI 等) 都列, 但不在 Kobak 論文中. Kobak 是統計方法, 只能抓出**頻率變化明顯**的詞; 有些 tell 是**新造 (post-ChatGPT 才出現)** 或**片語** (Kobak 只做單詞), 統計不到. 兩類都有價值.

### 對照: 中文有沒有對應詞?

Kobak 英文清單裡有不少可以概念對應到中文:

| 英文 (Kobak) | 中文對應 (我推的) |
|:---|:---|
| delve into | 深入探討 / 深入剖析 |
| underscore | 強調 / 突顯 / 凸顯 |
| comprehensive | 全面的 / 全方位的 |
| meticulous | 一絲不苟 / 精雕細琢 |
| realm | 領域 |
| pivotal | 至關重要 / 舉足輕重 |
| crucial | 關鍵的 |
| notably | 值得注意的是 |
| furthermore / moreover | 此外 / 更進一步 |
| foster / cultivate | 培養 / 培育 / 孕育 |
| leverage | 運用 / 借助 |
| utilize | 使用 / 運用 |
| testament to | ……的證明 / ……的體現 |
| navigate the complexities | 駕馭 / 應對複雜性 |
| embark on a journey | 踏上……的旅程 |
| paradigm shift | 典範轉移 |

這裡的中文對應是我推的, 沒有實證. 但**如果有夠大的中文 pre-ChatGPT vs post-ChatGPT corpus, 完全可以複製 Kobak 的方法論**, 產出中文版 excess vocabulary — 這是 lab04+ 的一個明確方向.

### 2026 新趨勢

1. **em-dash 政治化**: 2025-11 Sam Altman 宣布 GPT-5.1 起 ChatGPT 終於能遵守「不要用 em-dash」的 custom instruction. em-dash 曾是頭號 AI tell (見 V01), **對 ChatGPT 部分失效**, 但 **Claude / Gemini 仍大量使用**
2. **"delve" 頻率 2025 年比 2024 更高** — 已進入公眾意識, 部分寫手主動迴避, 但主流輸出未收斂
3. **markdown fingerprint 假說** ([arXiv 2603.27006 "The Last Fingerprint"](https://arxiv.org/abs/2603.27006)) — 主張 em-dash / 條列 / hr 等排版習慣源於**訓練資料含大量 markdown 洩漏到散文**. 這也是 V02/V03/V04 的理論基礎
4. **三段式結構偵測 > 單一詞彙偵測** — 更難迴避, 是 2026 主流方向 (lab02 未來要做)
5. **Claude 4.6 / 4.7 被評為「最難偵測」** — 仍有 77% 命中率, 意味著單靠詞彙清單命中率會下降, **要結合結構特徵**

---

## 對 V07 的啟示

### 1. 靠關鍵字表法可行, 但清單要**排除白名單**

V07 可以做「已知 AI 冗詞頻率」偵測, 但清單需要處理兩層:

- **黑名單 (AI cliche)**: 換句話說 / 值得注意的是 / 綜上所述 / 進一步而言 / 本質上 / 不僅如此 / 首當其衝 / 顯而易見 / 由此可見 …
- **灰名單 (需 context 判斷)**: 舉個例子 / 首先 / 其次 / 然後 / 最後 / 的話 / 的方式 — 可用但不能過量
- **白名單 (作者風格 / 領域術語)**: 獨立驗算 / 心智清單 / 推理模型 / 猜下一個字 … 不算 tell

問題: 白名單因人而異. 一個做法是**只算命中率**, 不試圖識別「這是誰的風格」.

### 2. 「重複自身」pattern 比「AI 特定詞」強

觀察你自己的重複, 大多是**「同一想法用不同變體重複表達」**:
- XX 的東西 (6+ 變體)
- 什麼時候 / 什麼任務 / 什麼都沒 (共同前綴)
- 為什麼講 / 為什麼一 / 是為什麼 (共同 root)

這種「同一詞根反覆出現」的模式在 AI 生成文本更常見 (AI 詞彙多樣性偏低). 對 V07 可能是更強的 signal 而不是特定冗詞.

### 3. V07 可能的三條路

**路 A: 冗詞清單頻率 (簡單)**
- 硬編碼 30-50 條 AI cliche
- 每篇算 `hit_count / chars * 1000`
- 缺點: 清單要維護, 且不同 AI 模型 / prompt 產出的詞不同

**路 B: 詞彙多樣性 (TTR — type-token ratio)**
- 用 jieba 或 4-gram sliding window 算詞彙 unique 率
- 低 TTR = 可能 AI
- 缺點: 短文 TTR 天然高, 長文天然低, 要 normalize

**路 C: 高頻 n-gram 佔比 (data-driven)**
- 全 corpus 掃出 top-k 高頻 4–8-gram
- 對每篇算「高頻 n-gram 佔字元比例」
- 佔比高 = 使用 corpus 平均語彙較多 = 可能是 AI (AI 傾向產出常見組合)
- 缺點: 同組 (組別 / 系列) 內天然相似, 可能誤判

**路 D: Kobak excess vocabulary (真 novelty)**
- 找到「pre-ChatGPT 中文 corpus」當 baseline (例如 2020-2021 年的鐵人賽, 或 CC-News 中文子集抓 2020 年以前)
- 對 2026 corpus 用 Kobak 的統計方法找出「頻率異常上升」的中文詞
- 產出**實證有據**的中文 excess vocabulary 清單, 不再靠人工猜
- 這條路做出來就是中文 AI detection 的 novelty, 沒人做過, 可以發表
- 缺點: 要找到夠大的 baseline corpus, 且要處理中文斷詞

**推薦: 短期路 A + 路 C 混合, 長期路 D**
- 短期 (V07 MVP): A 抓明確 AI cliche (參考上面英文對應清單 + 我手編的中文 64 條) + C 抓語彙貧乏
- 長期 (lab04+): 走路 D, 用實證方法替換手編清單, 才是 defensible science

### 4. Baseline 建議

用你自己 15 篇當作「已知不是純 AI」的 baseline:
- V07 對你 15 篇的分數應該**在低到中間**, 不該衝榜首
- 若真的衝榜首, 表示 V07 過度敏感, 該調參
- 這是免費的 negative test set

### 5. 下一步 (等你決定)

1. 選路 A / B / C / D / 混合
2. 決定 AI cliche 清單 (可用手編 64 條為起點, 或先花時間找中文 baseline corpus 走路 D)
3. 要不要處理 context (不只算命中次數, 還算「這個詞前後文有沒有對得起 AI 味」)
4. 要不要吃 jieba 或其他分詞器 (v01-v06 都純 stdlib, V07 若要 TTR / 詞頻對齊可能得破例)

寫完等你 review 再動 V07 code.

## Related

- [scan 腳本](./v07-scan-mycorpus.py) — 這份報告的產出來源
- [composite-score.md](./composite-score.md) / [composite-score-v06.md](./composite-score-v06.md) — 綜合分數設計, V07 完成後可加入權重
- [README.md](./README.md) — lab01 整體

## Sources

**英文 AI 冗詞的權威來源**:

- [Kobak et al. (2025) "Delving into LLM-assisted writing in biomedical publications through excess vocabulary" — Science Advances](https://www.science.org/doi/10.1126/sciadv.adt3813) — 1500 萬篇 PubMed 摘要統計出 900 個 excess words, 目前引用最多的實證研究
- [arXiv 預印本 (免費全文)](https://arxiv.org/abs/2406.07016) — 同上論文
- [berenslab/llm-excess-vocab (GitHub)](https://github.com/berenslab/llm-excess-vocab) — Kobak 論文的 900 詞完整 CSV 開源
- [arXiv 2412.11385 "Why Does ChatGPT 'Delve' So Much"](https://arxiv.org/abs/2412.11385) — 追 "delve" 源於 RLHF 標註者背景 (奈及利亞英語)
- [arXiv 2603.27006 "The Last Fingerprint"](https://arxiv.org/abs/2603.27006) — em-dash / 排版指紋來自 markdown 訓練資料洩漏

**中文檢測相關 (無公開特徵詞清單)**:

- [NLPCC-2026 Task 6 (GitHub)](https://github.com/NLP2CT/NLPCC-2026-Task6-Detection) — 中文 AI 生成文字檢測任務 (澳門大學 NLP2CT)
- WaveDetect ([arXiv:2506.23336](https://arxiv.org/abs/2506.23336)) — 小波變換方法, 語言中性

**本 lab 自製**:

- 前面 64 條中文冗詞清單: 我 (jason3e7 的 Claude 助手) 根據訓練經驗手動整理, 沒引用單一權威來源, 是**推測性 baseline**
- 本份掃描腳本: [`v07-scan-mycorpus.py`](./v07-scan-mycorpus.py)
