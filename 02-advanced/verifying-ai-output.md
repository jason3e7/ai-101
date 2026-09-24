---
title: AI 101 - AI 產出怎麼驗
tags: [ai, verification, hallucination, llm-as-judge, human-factors, nist, owasp, 進階]
created: 2026-09-24
---

# AI 產出怎麼驗 — Verifying AI Output: Methods, Limits, Frameworks

[← 回主頁](../index.md)

> [!NOTE]
> 「驗證 AI 產出」自成一門學問, 累積 4 年學術方法一堆, 但**大部分 2024 後被推翻或被 reasoning 模型內化**. 這篇把主流方法、已知極限、人的因素、產業框架攤在同一張桌上, 讓你知道什麼還能用、什麼是學術玩具, 以及**日常真正用得上的是哪三招**.

> **TL;DR (EN):** Verifying LLM output is a mature research area but ~half of the 2022-2023 techniques (Self-Refine, intrinsic self-consistency, verbalized confidence) are known to be circular or overconfident once tested on 2024+ reasoning models. What survives and still helps: **(a) external grounding** (RARR / attribution, FActScore), **(b) reasoning judges** (generative reward models with CoT, not scalar PRMs), and **(c) independent recomputation** (different session, different model, or authoritative source). Human factors dominate the failure mode — automation bias, verification fatigue, and expert-vs-novice gaps mean the *human* verifier is usually the weakest link, not the algorithm. Practical teams anchor on NIST AI RMF + OWASP Top 10 for LLM as governance baseline; day-to-day, three moves cover 90% of cases: **cite external sources, cross-check across models, decompose long outputs into atomic claims**.

---

## 為什麼「驗證 AI 產出」自成一門學問 — Why It Needs Its Own Playbook

驗證軟體 output 有 60 年方法論 (assert、TDD、fuzzing、formal proof). 驗證 AI 產出**不能直接套**, 因為:

- **沒有 ground truth**: 自由散文 / 長論述 / 開放式問答, 沒有「跑起來對不對」的自動判準
- **答案看起來永遠合理**: LLM 的表面說服力天生高, 錯的和對的長得一樣
- **同一題答案會變**: 溫度、seed、context 順序稍微變, output 就不同, 傳統 regression test 不適用
- **驗證的成本高過生成**: 生 1000 字花 3 秒, 人工細讀花 5 分鐘. 每題都驗, 系統跑不動

過去 4 年學術和產業各自發展一批方法, 有的還活著, 有的**被 reasoning 模型內化或推翻**. 下面攤開.

---

## 學術主流方法 — Academic Methods and Their 2026 Status

八個代表性技術, 標明現在還能不能用:

| 方法 | 誰提的 (年) | 一句話 | 2026 現況 |
|:---|:---|:---|:---|
| **Self-Consistency** | Wang et al. (2022) | 同題採 N 條 CoT, 多數決 | ✅ 舊模型還有用; reasoning 模型內部已在做, 外掛不划算 |
| **Chain-of-Verification (CoVe)** | Meta / Dhuliawala (2023) | 草稿 → 生驗證題 → 各自答 → 修 | ⚠️ 事實類 QA 有效; reasoning 模型內部複製了這 pattern |
| **Self-Refine** | Madaan (2023) | 同一 model 自寫自評自改 | ❌ 大半被推翻: ICLR 2024 證明**同 model 自我修正推理常常反而變差** |
| **RARR (attribution)** | Gao (2022) | 事後檢索證據, 改掉沒依據的句子 | ✅ 活得好, 是 Perplexity / Claude Search / GPT Search 的祖宗 |
| **FActScore** | Min (2023) | 長文拆成原子事實, 每個對照來源, 算命中率 | ✅ 長文事實性的標準 metric, 2024-25 有 VeriFastScore / Google Long-form Factuality 延伸 |
| **LLM-as-a-Judge** | Zheng et al. (2023, MT-Bench) | 用強 model 打分開放式答案 | ✅ 主流 eval 手段, 但有已知 bias (下節) |
| **Constitutional AI / RLAIF** | Anthropic (Bai 2022) | model 依「憲法」自評自改, 大半替代人類偏好標註 | ✅ Anthropic 在用, 業界廣泛複製 |
| **Process Reward Model (PRM)** | Lightman / OpenAI (2023) | 給推理**每一步**打分, 不只看最終答案 | ⚠️ 被 2025 的 generative reward models (RM-R1、DeepSeek-GRM、JudgeLRM) 取代 |
| **Generative Reasoning Judges** | 2025 多篇 | reward model 本身是 reasoning model, 先 CoT 再打分 | ✅ 現在 SOTA, 可 scale with inference compute |

**簡化版**:
- **「同 model 自己驗自己」的技術大半失效** (Self-Refine、intrinsic self-consistency)
- **「有外部證據 / 獨立模型 / decomposition」的技術活著** (RARR、FActScore、CoVe、LLM-as-Judge、CAI)
- **PRM 被 generative judge 取代**, 但方向 (per-step 驗證) 是對的

---

## 已知極限: 別以為驗證就一勞永逸 — Known Limits

過去 3 年至少五個負向研究值得記住:

- **LLM-Judge 有 bias (位置、長度、自我偏好)**: 前後對調可讓準確率變動 >10%; 模型會**高估自己風格的 output**. 對策: swap-and-average、rubric、跨家族 judge (Wataoka 2024)
- **Intrinsic self-correction 是循環**: DeepMind 2024 (Huang et al., ICLR 2024) 標題直接叫「LLMs Cannot Self-Correct Reasoning Yet」. 產生錯的 prior 也會產生「驗證」, 沒有外部訊號就打回原樣
- **Verbalized confidence 系統性過高**: 讓 model 說「你有幾成把握」, 它給的數字**普遍高於實際準確率**, RLHF 訓練後更嚴重 (Xiong ICLR 2024、ICLR 2025 相關研究)
- **CoT 對 reasoning 模型是負收益**: Wharton 2025 報告, 加「think step by step」對 o1 / Claude 4.x / GPT-5 只有邊際 gain, 卻多 20-80% latency
- **Self-correction illusion (2025)**: 模型能修**別人**的錯, 修不了**自己**的錯. 除非外部指出錯在哪, verification 就過不了

**共通結構**: 只要驗證訊號和被驗訊號**同源 (同 model、同 prompt 家族)**, 驗證會失敗. **獨立性**是關鍵.

---

## 人的因素: 你的注意力才是最脆的環節 — Human Factors

技術問題可以 patch, 人的問題比較難. 四個實證發現:

- **Automation bias**: 使用者對 AI 建議的**信任程度高於等值的人類專家**, 即使有情境訊號提示 AI 可能錯 (2024 replications; International AI Safety Report 2026)
- **Verification fatigue / rubber-stamping**: 臨床試驗 (NCT07328815, 2025) 記錄醫師連續使用 AI 診斷後, 驗證動作退化成蓋章
- **Jagged frontier — 專家 vs 新手**: HBS/BCG Dell'Acqua et al. (2023), 758 名顧問實驗, **在 AI 能力內的任務新手獲益最多**, 但**在 AI 能力外的任務專家反而變差** — 因為他們相信了合理但錯誤的 output
- **Cognitive offloading / 判斷力鏽掉**: 2025 SSRN + ScienceDirect 綜述, 長期依賴 LLM 會**測得到獨立思考能力下降**

> [!IMPORTANT]
> 這四條連起來說的是: **驗證不是把方法學會就結束**, 你自己的注意力和判斷力會被 AI **系統性稀釋**. 沒有意識地防這一點, 再好的 verification 技術都會被人這一環抵銷.

---

## 實務框架: 團隊落地公版 — Practical Frameworks

不是每個團隊都要自己發明驗證流程. 這幾個是**業界事實上的 baseline**:

| 框架 | 誰出 | 內容 | 適合 |
|:---|:---|:---|:---|
| **NIST AI RMF ＋ Generative AI Profile** | NIST (AI 600-1, 2024-07) | Govern / Map / Measure / Manage 四階段, 400+ 建議動作, 涵蓋 confabulation、CBRN、資訊完整性 | 美國事實 baseline, 治理層必讀 |
| **OWASP Top 10 for LLM Applications 2025** | OWASP | 10 大風險: prompt injection、sensitive info、supply chain、data poisoning、improper output handling、excessive agency、system prompt leakage、vector weakness、misinformation、unbounded consumption | AppSec / DevSecOps 落地清單 |
| **Anthropic Responsible Scaling Policy v3.1** | Anthropic (2026-04) | Capability threshold (CBRN、AI R&D 自動化) 觸發強制 safeguard; safety-case 方法論 | 前沿 model 提供商必看 |
| **Model cards + System cards + Safety cases** | Anthropic / OpenAI / Google DeepMind 業界慣例 | 每個 model release 附帶行為描述、已知限制、evals | 開發者評估要不要用某 model |

**日常實用抓法**: OWASP Top 10 for LLM 是**開發者最好切入點** (具體、可 audit); NIST 是**治理 / 合規層**; RSP / safety cases 給 model provider 自己內部用.

---

## 我的整理: 什麼真的管用 — My Take

四年學術累積下來, 過濾掉被推翻的、被內化的、只在 benchmark 上有效的, **日常真正還用得上的驗證動作**大概剩三個:

1. **外部證據 (grounding)**: 逼 output 附可查證來源, 隨機抽查. 對應 RARR / FActScore / attribution. **這一招最穩**
2. **獨立跨模型驗算**: Claude 問完問 Gemini / GPT, 或換問法從反面問. 對應 self-consistency 的**跨模型版**, 因為同 model 循環已被證明無效
3. **分解成原子事實再驗**: 長文拆成一句一句的 claim, 每一句獨立驗. 對應 FActScore / decomposition. 對事實密度高的 output 最有效

三招之外的都是**特殊情境用**:

- LLM-as-Judge: 有 eval 需求時才用, 記得對付 bias
- Constitutional AI / RLAIF: 你在訓 model 才用得到, 一般 API user 不用管
- Process reward models: 訓 reasoning model 的內功, 一般開發者接觸不到

而且**技術驗證做完不等於安全**. 上一節四個人的因素會把技術驗證的 gain 稀釋掉, 得同時處理:

- 別讓 AI 建議變成預設 (刻意保留「拒絕」的空間)
- 週期性讓自己回頭做**沒 AI** 的判斷 (防 offloading)
- 越是專家越要防 jagged frontier: 在你熟的領域相信 AI, 在你不熟的領域**特別要驗**

一句話: **驗證是個持續的姿勢, 不是一次性的技術決定**.

---

## 相關筆記 — Related

- [先收整再展開, 人才驗得動](./converge-before-verify.md), 講「怎麼把驗證挪到小的中間層」的具體手法
- [LLM 的極限](./llm-limitations.md), 為什麼有些錯根本驗不出來 (結構性限制)
- [AI 打破知識壁壘](../05-notes/ai-and-knowledge-barriers.md), jagged frontier 的另一個角度
- [先驗證再拓展](../05-notes/ai-verify-then-expand.md), 個人日常六招 (跟數學借的驗算法) + 為什麼獨立驗算最強

---

## Sources

### 主流方法
- [Self-Consistency Improves Chain of Thought Reasoning — Wang et al., 2022](https://arxiv.org/abs/2203.11171)
- [Chain-of-Verification Reduces Hallucination in LLMs — Dhuliawala / Meta, 2023](https://arxiv.org/abs/2309.11495)
- [Self-Refine: Iterative Refinement with Self-Feedback — Madaan et al., 2023](https://arxiv.org/abs/2303.17651)
- [RARR: Researching and Revising What LLMs Say — Gao et al., 2022](https://arxiv.org/abs/2210.08726)
- [FActScore: Fine-grained Atomic Evaluation of Factual Precision — Min et al., 2023](https://arxiv.org/abs/2305.14251)
- [Judging LLM-as-a-Judge with MT-Bench — Zheng et al., 2023](https://arxiv.org/abs/2306.05685)
- [Constitutional AI: Harmlessness from AI Feedback — Bai et al. / Anthropic, 2022](https://arxiv.org/abs/2212.08073)
- [Let's Verify Step by Step — Lightman et al. / OpenAI, 2023](https://arxiv.org/abs/2305.20050)
- [RM-R1: Reward Modeling as Reasoning — 2025](https://arxiv.org/abs/2505.02387)

### 負向結果
- [LLMs Cannot Self-Correct Reasoning Yet — Huang et al. / DeepMind, ICLR 2024](https://arxiv.org/abs/2310.01798)
- [Can LLMs Express Their Uncertainty? Verbalized Confidence — Xiong et al., ICLR 2024](https://arxiv.org/abs/2306.13063)
- [Self-Preference Bias in LLM-as-a-Judge — Wataoka, 2024](https://arxiv.org/abs/2410.21819)
- [The Decreasing Value of Chain of Thought in Prompting — Wharton GAIL, 2025](https://gail.wharton.upenn.edu/research-and-insights/tech-report-chain-of-thought/)
- [The Self-Correction Illusion in LLMs — 2025](https://arxiv.org/html/2506.05976v1)

### 人的因素
- [Navigating the Jagged Technological Frontier — Dell'Acqua et al., HBS + BCG, 2023](https://www.hbs.edu/faculty/Pages/item.aspx?num=64700)
- [Automation-Dependency Framework — SSRN, Cleverly 2025](https://papers.ssrn.com/sol3/papers.cfm?abstract_id=5359277)

### 實務框架
- [NIST AI Risk Management Framework](https://www.nist.gov/itl/ai-risk-management-framework)
- [OWASP Top 10 for LLM Applications 2025](https://genai.owasp.org/resource/owasp-top-10-for-llm-applications-2025/)
- [Anthropic Responsible Scaling Policy](https://www.anthropic.com/responsible-scaling-policy)
