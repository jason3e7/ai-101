---
title: AI 101 - AI 用久了會鈍化: 驗證疲勞與判斷力生鏽
tags: [ai, human-factors, verification-fatigue, cognitive-offloading, automation-bias, rct, eeg, 進階]
created: 2026-09-24
---

# AI 用久了會鈍化 — Verification Fatigue and Cognitive Offloading

[← 回主頁](../index.md)

> [!NOTE]
> AI 讓你變快, 但也在**測量得到的意義上**讓你變鈍. 這篇拆兩個時間尺度: **急性**(單次連續使用後, 驗證動作退化成蓋章)和**慢性**(長期依賴後, 獨立思考能力下降). 兩者不是玄學, 2025 已有 RCT、EEG、大規模 survey 三種證據. 也列出 5 個有實證的破法.

> **TL;DR (EN):** Two measurable ways AI use erodes human judgment. **Acute — verification fatigue**: after continuous use, humans mechanically approve AI output (2025 RCT NCT07328815 with 72 physicians showed rubber-stamping; ensemble-disagreement nudges recovered +7.6pp accuracy; RSNA 2024 mammography showed radiologists were 5× more likely to err when AI was wrong; GitHub Copilot acceptance climbs 28.9% → 34% over six months, consistent with automation habituation). **Chronic — cognitive offloading**: long-term reliance measurably weakens neural + behavioral independent reasoning (MIT Media Lab EEG 2025: LLM users showed up to 55% reduced neural connectivity vs brain-only, effects persisted when later writing unaided — the paper coined "cognitive debt"; Microsoft/CMU 2025 survey of 319 knowledge workers found higher AI-confidence predicted less enacted critical thinking). Same phenomenon, two timescales: each rubber-stamp trains lower scrutiny long-term. Interventions with data: ensemble disagreement flags, explainable AI, pre-commit to your own answer, brain-only intervals, adversarial self-testing.

---

## 兩個時間尺度, 同一個問題 — One Problem, Two Timescales

用 AI 用久了會鈍, 分兩層講:

- **急性: 驗證疲勞 / 蓋章化 (rubber-stamping)**. 單次連續使用後, 你的驗證動作退化成機械蓋章
- **慢性: 判斷力生鏽 / 認知外包 (cognitive offloading)**. 長期依賴後, 你的獨立思考能力**測得出**下降

兩者不是兩件事, 是**同一機制在不同時間尺度**上的顯現. 每一次蓋章 = 訓練自己下次更快蓋章 = 累積成長期的認知外包. Microsoft 2025 那份 survey 給了因果橋樑: **越信任 AI, 越少 enact critical thinking; 越信自己, 越多**.

---

## 急性: 驗證疲勞 — Acute: Verification Fatigue

三份 2024-2025 的實證證據, 都指同一件事: 你以為自己在驗, 實際只是在點頭.

### 醫療診斷 RCT (NCT07328815, 2025)

隨機對照試驗, 72 名 AI-trained 醫師評估 6 個含 3 個蓄意錯誤的臨床案例, 配 ChatGPT-5.1 建議. 介入組拿到**兩個行為 nudge**: (1) ChatGPT 的 benchmark accuracy 錨定值, (2) 三個 LLM (Claude Sonnet 4.5 / Gemini 2.5 Pro Thinking / GPT-4o) 共識判斷的紅綠燈警示.

**結果**: 介入組 82.8% vs 對照組 75.6% (+7.6pp, P=0.016). 第一個 RCT 證據: **「多個 model 意見不合」的警示能顯著壓下蓋章行為**.

意思是, 沒 nudge 的組別**明明知道 AI 可能錯**, 還是照單全收. 因為連續閱讀 AI 給的合理答案已經把你的批判性打疲了.

### 影像判讀 (RSNA 2024, 乳房攝影)

沒有 XAI (explainable AI) 情境下, 醫師在 36.1% 被操弄的案例中發生 automation bias. AI 錯的時候, 醫師跟著錯的機率是**基準的 4.89 倍** (OR). 更慘的是新手: 從 79.7% 正確率**掉到 19.8%**, 只因 AI 給了誤導答案. 有 XAI (顯示 AI 為什麼這樣判) 時, bias 從 36.1% 降到 17.8%, 但沒消失.

### 寫 code 也一樣 (GitHub Copilot)

Copilot suggestion 接受率**從第一個月 28.9% 爬到第六個月 34%**. 接受率隨使用時間爬升, 符合航空業對「automation complacency」 (自動化自滿) 半世紀累積的曲線. 你越熟 Copilot, 越傾向直接吞掉它給你的 completion.

---

## 慢性: 判斷力生鏽 — Chronic: Cognitive Offloading

長期依賴 LLM 會讓你的獨立思考能力**測得到**下降. 三份 2025 研究從腦神經、行為、理論框架三個角度都給出證據.

### 腦神經證據 (MIT Media Lab, Kosmyna 2025)

54 位受試者分三組 (LLM / 傳統搜尋 / 純腦子) 寫議論文, 戴 EEG 監測. 結果:

- LLM 組 EEG 神經連結度**下降 55%**, 涵蓋記憶、注意、執行網路
- LLM 組**對自己剛寫的文字擁有感最弱, 事後回想不出來**
- 效應**持續**: 之後單獨寫 (無 AI), LLM 組表現仍差

論文自己創了個詞叫 **"cognitive debt" (認知債)**: 用 AI 省下的思考, 累積成一筆日後要還的認知額度.

### 行為證據 (Microsoft Research + CMU, Lee et al., CHI 2025)

319 名知識工作者, 936 次真實 GenAI 使用調查. 結論:

- **對 AI 信心越高**, 使用者實際 enact 的 critical thinking 越**少**
- **對自己信心越高**, critical thinking 越**多**
- 工作者的認知任務從**生產** (自己想) 轉成**驗證 / 編輯**. 這不是記憶外包 (calculator 那種), 是**推理本身被外包出去**

### 概念框架 (Cleverly 2025, SSRN)

不是實證, 是提出「cognitive inheritance」和「epistemic injustice」框架, 談 AI 偏誤如何透過長期依賴被放大、複製到集體判斷. 值得看的是**問題定型**, 不是數據.

---

## 歷史其他工具怎麼比 — Historical Parallels

不是第一次人類把認知任務外包給工具了. 有幾個學過的教訓:

- **倫敦計程車司機 (Maguire 2000)**: 為考「The Knowledge」記完整倫敦街道地圖, **後海馬 (hippocampus posterior) 灰質變厚**. GPS 普及後, 這個效應被反轉的研究陸續出現. 附帶發現: 這批專家**建立新視覺聯想的能力較弱**, 專精有取捨
- **拼字檢查 / 計算機**: Generation effect (Slamecka 1978) 早已證明**自己產生的內容比被動接收更容易記住**. 拼字檢查用久, 你不會拼的字反而變多
- **航空自動化 (半世紀教訓)**: automation complacency 這個詞是航空業給的, 有大量意外報告佐證. Copilot acceptance 爬升曲線幾乎複製了自動駕駛的信任累積曲線

意思是: **AI 的認知外包不是新問題, 是舊問題的最新規模**. 過去的解法 (刻意保留手動練習) 也適用.

---

## 有實證的破法 — Evidence-Based Interventions

不是喊「多動腦」就行. 下面 5 招各有 2024-2025 的實驗數據或半世紀航空經驗做靠山.

| 招 | 做什麼 | 靠什麼證據 |
|:---|:---|:---|
| **Ensemble disagreement flags** | 用 2-3 個 model 跑同題, 意見不合就升紅旗 | NCT07328815 RCT (+7.6pp 準確度) |
| **Explainable AI** | 讓 AI 顯示「為什麼這樣答」而不是只給結論 | RSNA 2024 (bias 從 36.1% → 17.8%) |
| **Pre-commit 自己先寫答案** | 打開 AI 前, 先寫下自己的假設或答案, 再問 AI, 再對比 | Generation effect + Kosmyna 建議 |
| **Brain-only 間隔日** | 固定比例 (例如每 3-4 個任務) 完全不用 AI, 純腦子完成 | Kosmyna 「cognitive debt」建議, 防長期累積 |
| **對抗性自測** | 定期餵 AI 已知錯的 output / vignette, 練自己抓錯 | 航空業慣例 (weekly failure drill), 醫療 RCT 已在試 |

**共通模式**: 都是**主動把「自己動腦的機會」種回工作流裡**. AI 太好用, 不會有機會自動出現.

---

## 相關筆記 — Related

- [AI 產出怎麼驗](./verifying-ai-output.md), 技術層的驗證方法; 這篇是人層的補集
- [先收整再展開, 人才驗得動](./converge-before-verify.md), 降低驗證成本的具體手法, 讓「該驗的都驗得動」
- [AI 打破知識壁壘: 被夾殺的知識中產](../05-notes/ai-and-knowledge-barriers.md), jagged frontier + 專家 vs 新手的另一個角度
- [先驗證再拓展](../05-notes/ai-verify-then-expand.md), 個人日常驗證姿勢 (跟數學借的六招)

---

## Sources

### 急性: 驗證疲勞
- [Mitigating Automation Bias in Physician-LLM Diagnostic Reasoning — NCT07328815 Protocol](https://cdn.clinicaltrials.gov/large-docs/15/NCT07328815/Prot_SAP_000.pdf)
- [Mitigating Automation Bias in Physician-LLM Diagnostic Reasoning — medRxiv 2026](https://www.medrxiv.org/content/10.64898/2026.06.01.26354596v1.full)
- [Automation Bias in Mammography with and without XAI — RSNA Radiology 2023](https://pubs.rsna.org/doi/full/10.1148/radiol.222176)
- [Radiologist Interaction with AI Reports — Journal of ACR 2025](https://www.jacr.org/article/S1546-1440(25)00558-7/fulltext)
- [GitHub Copilot Acceptance Rate Study](https://github.blog/news-insights/research/does-github-copilot-improve-code-quality-heres-what-the-data-says/)

### 慢性: 判斷力生鏽
- [Your Brain on ChatGPT — Kosmyna et al., MIT Media Lab 2025](https://www.media.mit.edu/publications/your-brain-on-chatgpt/) ([arXiv 2506.08872](https://arxiv.org/abs/2506.08872))
- [The Impact of Generative AI on Critical Thinking — Lee et al., Microsoft Research + CMU, CHI 2025](https://www.microsoft.com/en-us/research/wp-content/uploads/2025/01/lee_2025_ai_critical_thinking_survey.pdf)
- [AI-Derived Biases Through Automation Dependency — Cleverly, SSRN 2025](https://papers.ssrn.com/sol3/papers.cfm?abstract_id=5359277)
- [Cognitive Offloading in Student-AI Collaboration — Kadoma et al., Computers in Human Behavior Reports 2025-26](https://www.sciencedirect.com/science/article/pii/S2451958826002046)
- [Cognitive Offloading or Cognitive Overload? — Frontiers in Psychology 2025](https://www.frontiersin.org/journals/psychology/articles/10.3389/fpsyg.2025.1699320/full)

### 歷史對照
- [Navigation-Related Structural Change in Hippocampi of Taxi Drivers — Maguire 2000](https://www.researchgate.net/publication/12598624_Navigation-Related_Structural_Change_in_the_Hippocampi_of_Cab_Drivers)
