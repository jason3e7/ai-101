---
title: "AI 101 - 鐵人賽 Day 13: AI 用久了會鈍化, 你以為在驗其實在蓋章"
tags: [ai, 鐵人賽, ironman, 驗證疲勞, 認知外包, human-factors, 草稿]
created: 2026-09-26
status: draft
---

# Day 13｜AI 用久了會鈍化: 兩種鈍, 五招破 — Verification Fatigue and Cognitive Offloading

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> Day 11 教你六招驗證 AI 產出, Day 12 讓你用心智清單降低驗證成本. 這兩天都在磨你的**工具**, 但如果**拿工具的人本身在鈍化**, 工具再利也救不了. 這篇拆兩種鈍法 (急性蓋章 + 慢性判斷力生鏽), 都有 2025 實證, 也給 5 招有靠山的破法.

> **TL;DR (EN):** Day 11-12 gave you verification tools; Day 13 asks who's using them. AI reliance measurably erodes the human verifier itself. **Acute — rubber-stamping** happens in a single session: a 2025 physician RCT (NCT07328815, 72 doctors) needed an ensemble-disagreement nudge to recover +7.6pp accuracy; a Radiology 2023 mammography study found readers 4.89× more likely to err when AI was wrong; GitHub Copilot suggestion acceptance climbs from 28.9% to 34% over six months, mirroring the automation-complacency curve aviation has known for half a century. **Chronic — cognitive offloading** builds up: MIT Media Lab 2025 EEG showed LLM users had 55% lower neural connectivity than the brain-only group, with effects persisting when they later wrote unaided (the paper coined "cognitive debt"); a Microsoft/CMU 2025 survey of 319 knowledge workers found higher AI-confidence predicted less enacted critical thinking. Same mechanism, two timescales: each rubber-stamp trains lower scrutiny long-term. Five evidence-backed interventions: ensemble disagreement flags, explainable AI, pre-commit your own answer, brain-only intervals, adversarial self-testing.

```markdown
# AI 用久了會鈍化: 兩種鈍, 五招破, 都有實證
* 為什麼講這個 (承 Day 11-12 磨工具, 這篇看拿工具的人)
* 兩種鈍法, 同一個機制
  * 急性: 蓋章化 (rubber-stamping) 單次連續就退化
  * 慢性: 判斷力生鏽 (cognitive offloading) 長期依賴累積
  * 每次蓋章訓練下次更快蓋 (Microsoft 因果橋樑)
* 急性證據 (三份研究)
  * 醫療 RCT NCT07328815: ensemble 警示 +7.6pp
  * Radiology 2023 乳房攝影: AI 錯時醫師錯 4.89x
  * GitHub Copilot 接受率半年 28.9% → 34%
* 慢性證據 (三份研究)
  * MIT EEG: 神經連結 -55%, 認知債
  * Microsoft/CMU: 越信 AI 越少動腦
  * Cleverly: 集體判斷會被 AI 偏見複製
* 五招破法 (都有實證靠山)
  * 多 AI 對照 (ensemble)
  * 讓 AI 說原因 (XAI)
  * 自己先寫再問 (pre-commit)
  * 純腦子日 (brain-only)
  * 對抗性自測
```

---

## 為什麼講這個 — Why This Matters

前兩天在磨驗證的**工具**: Day 11 給了六招驗算法, Day 12 給心智清單降低驗證成本. 但驗證這件事有另一個環節你動不了 — **拿工具的人**.

想想這些場景:

- 醫師**知道** AI 可能錯, 還是照單全收
- 工程師用 GitHub Copilot 半年後, 接受它建議的比例爬高了 (不是因為建議變好)
- 用 LLM 寫作的人, 事後**回想不出自己剛寫的東西**

這些不是猜測, 是 2024-2025 累積的實證. 這篇把兩個時間尺度都攤開, 也給有靠山的破法.

---

## 兩種鈍法, 同一個機制 — Two Kinds, One Mechanism

用 AI 用久了會鈍, 分兩層講:

- **急性**: **蓋章化** (rubber-stamping, 學術詞). 白話說 = **看都不看就過, 像蓋章一樣機械**. 單次連續使用就出現
- **慢性**: **判斷力生鏽** (cognitive offloading, 認知外包). 白話說 = **長期把思考丟給 AI 做, 自己的獨立思考能力測得到下降**

**兩者不是兩件事**, 是**同一個機制在不同時間尺度**上的顯現. Microsoft 2025 那份 survey 給了因果橋樑: 越信任 AI, 就越少**實際去動腦**; 每一次蓋章 = 訓練自己下次更快蓋 = 累積成長期的認知外包.

---

## 急性: 你以為在驗, 其實在點頭 — Acute Evidence

三份 2024-2025 的研究都指同一件事.

### 醫療 RCT: 醫師知道 AI 會錯, 還是照單全收

**研究代號**: NCT07328815 (2025). RCT = 隨機對照試驗, 兩組隨機分配, 一組拿介入一組拿對照, 是醫學實證裡最硬的證據等級之一.

**設計**: 72 名 AI-trained 醫師評估 6 個臨床案例, 其中 3 個是**蓄意錯的**, 配 ChatGPT-5.1 建議. 介入組多兩個提醒: (1) ChatGPT 平均準確度 (讓你知道它不是萬能), (2) 三個 LLM (Claude / Gemini / GPT-4o) 意見不合時的紅綠燈警示.

**結果**: 介入組 82.8% vs 對照組 75.6%, 差 +7.6pp (P=0.016, 意思是這個差異不是巧合).

**意義**: 沒警示的組別**明明知道 AI 可能錯**, 還是照單全收. 因為連續讀 AI 給的合理答案已經把批判性打疲了. **這是 rubber-stamping 的 RCT 級證據**.

### 乳房攝影: AI 錯的時候, 醫師跟著錯的機率變 4.89 倍 (Radiology 2023)

沒 XAI 的情境 (XAI = explainable AI, 會告訴你「為什麼這樣答」的 AI) — 醫師在 36.1% 被操弄的案例發生 **automation bias** (白話說 = AI 說什麼就信什麼). AI 錯的時候, 醫師跟著錯的機率是**基準的 4.89 倍**.

新手更慘: 從 79.7% 正確率**掉到 19.8%**, 只因 AI 給了誤導答案.

有 XAI 的時候, bias 從 36.1% 降到 17.8% — **有幫助但沒消失**.

### GitHub Copilot: 用越熟, 接受越多

Copilot 建議的接受率**從第一個月 28.9% 爬到第六個月 34%**. 接受率隨使用時間爬升.

這個曲線跟航空業對 **automation complacency** (自動化自滿, 白話 = 用習慣自動化後的鬆懈) 累積半世紀的事故資料**幾乎一樣**. 你越熟 Copilot, 越傾向直接吞掉它給的補完.

---

## 慢性: 你的腦子測得到變鈍 — Chronic Evidence

三份 2025 研究從**腦神經、行為、理論框架**三個角度都指同一方向.

### 腦神經證據: EEG 直接看到神經連結下降 (MIT Media Lab, 2025)

Kosmyna et al., 2025. **EEG = 腦波檢測**, 用貼在頭皮上的電極測腦電活動.

54 位受試者分三組 (LLM / 傳統搜尋 / 純腦子) 寫議論文, 全程戴 EEG. 結果:

- LLM 組 EEG 神經連結度**下降 55%**, 涵蓋記憶、注意、執行網路
- LLM 組**對自己剛寫的文字沒感覺, 事後回想不出來**
- 效應**持續**: 之後單獨寫 (無 AI), LLM 組表現**仍然差**

論文自己創了個詞叫 **"cognitive debt" (認知債)**: 用 AI 省下的思考, 累積成一筆**日後要還的**認知額度. 概念跟技術債一樣, 只是這次是你自己的腦子.

### 行為證據: 越信 AI, 越少動腦 (Microsoft × CMU, 2025)

Lee et al., CHI 2025. 319 名知識工作者, 936 次真實 GenAI 使用的調查:

- **對 AI 信心越高**, 使用者**實際去做的** critical thinking (批判性思考) 越**少**
- **對自己信心越高**, critical thinking 越**多**
- 工作者的認知任務從**生產** (自己想) 轉成**驗證/編輯**. 這不是記憶外包 (計算機那種只是幫你記數字), 而是**推理這個動作本身被外包出去**

這份 survey 提供了「急性 → 慢性」的**因果橋樑**: 每次蓋章都在訓練「不動腦」變成默認.

### 概念框架: 集體判斷會被 AI 偏見複製 (Cleverly, SSRN 2025)

Cleverly 2025 不是實證研究, 是提出 **cognitive inheritance** (認知繼承) 框架 — AI 的偏誤如何透過長期依賴被放大、複製到**集體判斷**. 值得看的是**問題定型**, 不是數據.

---

## 五招破法, 都有靠山 — Evidence-Based Interventions

不是喊「多動腦」就行. 下面 5 招各有 2024-2025 的實驗數據或半世紀航空業經驗做靠山.

| 招 | 白話說 | 靠什麼 |
|:---|:---|:---|
| **多 AI 對照** (ensemble) | 用 2-3 個 model 跑同題, 意見不合就升紅旗 | NCT07328815 RCT (+7.6pp 準確度) |
| **讓 AI 說原因** (XAI) | 讓 AI 顯示「為什麼這樣答」而不是只給結論 | Radiology 2023 (bias 從 36.1% → 17.8%) |
| **自己先寫再問** (pre-commit) | 打開 AI 前, 先寫下自己的假設或答案, 再問 AI, 再對比 | Generation effect (自己想的比較記得住) ＋ Kosmyna 建議 |
| **純腦子日** (brain-only) | 固定比例 (例如每 3-4 個任務) 完全不用 AI, 純腦子完成 | Kosmyna「認知債」建議, 防長期累積 |
| **對抗性自測** | 定期餵 AI 已知錯的 output / 案例, 練自己抓錯 | 航空業慣例 (weekly failure drill), 醫療 RCT 已在試 |

**共通模式**: 都是**主動把「自己動腦的機會」種回工作流裡**. AI 太好用, 不會有機會自動出現.

---

## 我的重點 — Takeaways

- 前兩天教你怎麼驗, 這篇是**你自己會鈍**, 得防這一環. 工具再利, 用的人鈍了也沒用
- 兩種鈍是**同一機制在不同時間尺度**: 急性蓋章 → 訓練慢性外包 → 判斷力測得到下降
- 五招破法**都有實證靠山**, 不是喊多動腦. 挑一招今天就開始
- 更完整的整理 (含歷史對照: 倫敦計程車、拼字檢查、航空業自動化) 見 repo 的 [AI 用久了會鈍化](../../../02-advanced/ai-atrophy.md)

---

## Sources

### 急性: 驗證疲勞
- [Mitigating Automation Bias in Physician-LLM Diagnostic Reasoning — NCT07328815 Protocol](https://cdn.clinicaltrials.gov/large-docs/15/NCT07328815/Prot_SAP_000.pdf)
- [Mitigating Automation Bias in Physician-LLM Diagnostic Reasoning — medRxiv 2026](https://www.medrxiv.org/content/10.64898/2026.06.01.26354596v1.full)
- [Automation Bias in Mammography with and without XAI — Radiology 2023](https://pubs.rsna.org/doi/full/10.1148/radiol.222176)
- [GitHub Copilot Acceptance Rate Study](https://github.blog/news-insights/research/does-github-copilot-improve-code-quality-heres-what-the-data-says/)

### 慢性: 判斷力生鏽
- [Your Brain on ChatGPT — Kosmyna et al., MIT Media Lab 2025](https://www.media.mit.edu/publications/your-brain-on-chatgpt/) ([arXiv 2506.08872](https://arxiv.org/abs/2506.08872))
- [The Impact of Generative AI on Critical Thinking — Lee et al., Microsoft Research + CMU, CHI 2025](https://www.microsoft.com/en-us/research/wp-content/uploads/2025/01/lee_2025_ai_critical_thinking_survey.pdf)
- [AI-Derived Biases Through Automation Dependency — Cleverly, SSRN 2025](https://papers.ssrn.com/sol3/papers.cfm?abstract_id=5359277)
