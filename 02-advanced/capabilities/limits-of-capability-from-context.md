---
title: AI 101 - 情境推任務會在哪裡失手
tags: [ai, 情境, in-context-learning, 長情境, prompt-injection, task-vector, 進階]
created: 2026-10-14
---

# 情境推任務會在哪裡失手 — Where Capability-from-Context Breaks

[← 回主頁](../../index.md)

> [!NOTE]
> 姊妹篇 [AI 怎麼知道該用哪種能力](how-ai-picks-capability.md) 解釋了第 1 層怎麼從情境裡「推」出任務座標。這篇是接棒: **那個推論, 什麼時候會失手?** 把 2024-2026 的研究結論收攏成六種極限 — 情境太長、示範太多、順序敏感、任務太「鋸齒」、情境被劫持、情境留白. 每一種都附證據, 每一種都有對應的工程手段.

> **TL;DR (EN):** Context-based task inference (Layer 1 in the sister note) has six documented failure modes. (1) **Long context decay** — NoLiMa shows 11/13 "128K-ready" models drop to ≤50% baseline at 32K; attention falls apart faster than the token budget suggests. (2) **Many-shot saturation** — some tasks degrade past a certain shot count. (3) **Order sensitivity** — the same demos in a different order can swing accuracy wildly. (4) **Task vector rank-one ceiling** — bijection-type tasks are provably unsolvable by the single-vector representation. (5) **Context hijacking** — models can't reliably separate information from instructions. (6) **Silent failure under underspecification** — rather than ask, models pick the training-distribution mode and go. Fixes: put the signal early, trim distractors, randomize demo order and ensemble, call multiple times for high-rank mappings, treat agent-ingested content as untrusted, force-list assumptions.

```markdown
# 情境推任務的 6 種失手
* 情境太長 → attention 衰退
  * NoLiMa 32K ≤50% baseline
  * Context Rot 的三種機制
  * Lost in the Middle
* 示範太多也會壞
  * Many-shot 到某點後退化
  * Sweet spot 任務依賴
* 示範的順序會翻盤
  * 同一組 demo 不同 order 差異大
  * 因果注意力偏尾部
* Task vector 的 rank-one 極限
  * 只能表徵低秩 mapping
  * bijection 類任務理論上無解
* 情境可以被劫持
  * 無法分辨資訊 vs 指令
  * context tampering 塞假完成訊號
* 情境不明時, 它會推錯而且不問
  * silent failure
  * 多次呼叫都收斂到同一版錯答案
```

---

## 1. 情境太長 → attention 衰退 — Long Context Decay

**最反直覺的一點: 「它支援 128K context」不等於「餵 128K 它都看得見」.**

NoLiMa (2025) 挑了 13 個宣稱支援 ≥128K 的模型, 做了一件簡單的事: 把需要的訊息藏在長 context 裡, 但**不用字面重複** (問 Dresden 相關問題, 答案 needle 寫 "Semper Opera House"), 強迫模型用世界知識連起來. 結果:

| 模型 | base (<1K) | 32K | 128K |
|:---|---:|---:|---:|
| GPT-4o | **99.3%** | **69.7%** | **56%** |
| Llama 3.3 70B | **97.3%** | **42.7%** | — |
| Gemini 2.0 Flash | — | — | **16.4% of baseline** |

**32K 已經 11/13 模型掉到 baseline 的 ≤50%.** 這是在模型根本沒拒答的前提下量到的, 不是攔截問題, 是真正的能力衰退.

Chroma 2025 的 **Context Rot** 報告拉得更細 (18 model 跨廠對照), 指出三個獨立機制:

- **語意相似度門檻**: needle 跟問題的 cosine similarity 低時, 效能掉得更快. 相似度 0.44 vs 0.83 的 needle, 在同樣 context 長度下效能差一截.
- **單一 distractor 就夠**: 塞一個看起來相關但答案不對的句子進去, accuracy 立刻掉. 多塞幾個不是線性疊加, 某些 distractor 特別毒.
- **結構良好 vs 隨機排列**: 反直覺地, **邏輯連貫的 haystack 比隨機 shuffle 的 haystack 表現更差** — attention 似乎會被連貫性誤導.

**位置偏好** (Lost in the Middle, Liu 2023 的老結論在新模型上仍成立): 關鍵訊息放開頭最準, 中段最糟, 結尾次之. 這跟 Transformer 的 positional encoding 跟 causal attention 的結構有關, 不是 prompt 寫法能補的.

> [!TIP]
> **換成白話**: 「我 attach 一堆文件讓它 reason」在 10K 內還行, 到 30K 就要開始怕. 要讓 128K 能用, 不是信它, 是**自己做 chunk + retrieval**, 把真正相關的那 2K 挑出來給它看.

---

## 2. 示範太多也會壞 — Many-Shot ICL Saturation

Google DeepMind 2024 的 **Many-Shot In-Context Learning** 把這題挖出來: 在 Gemini 1.5 Pro 的 1M context 下塞進 **8192 個示範**會怎樣? 答案是「視任務而定, 多半不線性」:

- **低資源機器翻譯** (英→Bemba / Kurdish): 爬到幾百 shot 持續進步, 42% → 62%, 甚至打敗 NLLB 跟 Google Translate.
- **Code verifier / planning**: 到某個 shot 數後效能**反而退化**.
- **新舊版 Gemini 1.5 Pro**: 曲線完全不同, saturation point 跟模型版本耦合.

**MIR-Bench** (2025) 更進一步: 複雜 pattern 的 recognition task, many-shot 的 scaling 比簡單 pattern 平很多. 示範數量不是通用解藥.

**為什麼會這樣**: 示範不是「越多越可靠」, 而是「越多就越容易混進噪音」. 第 100 個示範如果跟前 99 個格式略有不同, 可能把 task vector 的方向帶偏. 當 task 本身結構複雜, 示範之間的輕微不一致會累積成系統性偏誤.

> [!NOTE]
> 2026 現行模型大多沒有「自動找 sweet shot 數」的能力. 要嘛你離線校正過, 要嘛假設 **5-20 shot** 已經是甜區, 不要塞 500 個.

---

## 3. 示範的順序會翻盤 — Order Sensitivity

Zhao et al. (2021) 的老發現: GPT-3 **同一組示範不同順序, accuracy 可以差 ±20%**. 這題在 2026 依然沒解乾淨.

**The Order Effect** (2025, arxiv 2502.04134) 展示新一代模型仍然敏感. 關鍵機制是 **causal attention 讓尾部 example 的影響偏大**: 模型回答時最後看到的 token 權重最高, 所以把答對的例子放結尾, 效能會比放開頭好.

**找最佳順序本來是階乘問題** (10 個示範就有 3,628,800 種 order), 2025 ICLR 有論文用 embedding cluster 把搜索時間壓低 92%+, 但這代表到今天都還**沒有 scalable 的自動解**.

實務收斂到兩條規則:

- **放幾個保險位**: 開頭放最具代表性的、結尾放最接近 target 分布的, 中間亂序.
- **ensemble 多個 order**: 對同一組示範做 3-5 種 order, 投票, 比單次呼叫穩很多. 跟取樣已經鎖死 (姊妹篇第 6 層) 搭配更需要這招.

---

## 4. 任務太「鋸齒」—— Task Vector 的 Rank-One 極限

這一條是**理論上無解**的失敗 — 不是 prompt 寫得不好能補的.

ICLR 2026 的 **Understanding Task Vectors in In-Context Learning** (2506.09048) 證明: 從示範裡抽出來的 task vector, 本質是**低秩 (rank-one) meta-predictor** — 它是原示範的某種線性組合, 可以表徵的 input→output mapping 有上限.

具體什麼會崩: **bijection 類任務** — 每個 input 一對一映射到不同 output, 沒有可用的 pattern 壓縮空間. 這類任務可以**可證明地** rank-one 無解. 真實的 LLM 上也確認了這個預測.

為什麼這跟實務有關? 因為很多工作 task 看起來簡單, 本質是 bijection:

- 「把這 100 個客戶名字對應到 100 個獨特 ID」
- 「把這份清單裡每個 bug 分配到不同工程師」
- 「每一題配一個獨特的解題策略」

這些題用 ICL 很容易看起來前幾個對了, 中間開始崩. 補救不是「加更多示範」, 是**改成多次呼叫**(每次一個 input, 不要壓進同一個 context) 或**改用 retrieval + lookup table** 這類外部機制.

> [!IMPORTANT]
> Task vector 的這個極限跟 [LLM 極限](../limits-and-verification/llm-limitations.md) 裡的「鋸齒狀前沿」是**同一個問題的機制面**: 看起來簡單的任務, 它偶爾歪得莫名其妙, 很多時候就是撞到這條 rank-one 牆.

---

## 5. 情境可以被劫持 — Context Hijacking

LLM 的一條核心弱點: **它無法可靠分辨「這段文字是資訊」還是「這段文字是指令」**. BIPIA、InjecAgent、AgentDojo 這幾個 agent-level benchmark 在 2024-2026 一致指出 SOTA 模型對 indirect prompt injection 的攻擊成功率仍然偏高.

典型手法 **Context Tampering** (2602.16958): 在 agent 要讀的檔案或網頁內容裡塞進一行假的「task completed」訊號, 接著塞新的未授權指令. 模型傾向把對話史當權威, 就跟著走.

2026 的新研究把這條線拉得更細:

- **AgentBreaker** (2026-10): 評測 web agent 在真實頁面情境下的 indirect injection 風險.
- **PISmith** (2026): 用 RL 訓練紅隊 agent, 自動找 prompt injection 的 payload.

**對照姊妹篇**: 第 1 層的任務推論從**所有** context 裡算座標, 它沒有「這段是來自使用者 / 這段是來自工具 / 這段是來自 attack surface」的概念. 這個分不清就是劫持能成功的根因.

> [!CAUTION]
> **實務**: Agent 從外部 (file、網頁、tool output) 讀進來的任何內容, 一律當 untrusted. 這也是為什麼 Claude Code 的 `/goal` + CLAUDE.md INTEGRITY RULES 一直存在 — 不是防惡意使用者, 是防被 context 劫持.

---

## 6. 情境不明時, 它會推錯而且不問 — Silent Failure Under Underspecification

人類遇到不清楚的任務會停下來問. LLM 多半**不會**, 它會從訓練分佈裡挑一個最高頻的解讀, confidently 做下去.

**Underspecification Doesn't Imply Incoherence** (2607.01953) 的發現: 給 LLM 一個 underspecified coding task 多次呼叫, 自然預期是「它會給出多種合理解讀的實作」, 實際上**語意崩塌** — 多次呼叫往往收斂到同一版 (而且不一定對). 它不是知道有歧義還是選了一個, 是**沒意識到有歧義**.

**A Taxonomy of Prompt Defects in LLM Systems** (2509.14404) 把這類失敗歸到 "Specification & Intent Defects" 底下, 分成「ambiguous」(多種解法可行) 跟「underspecified」(資訊不足). 兩者都會導致 silent failure, 但後者更危險 — ambiguous 多半能從輸出看出多樣性, underspecified 看不出.

為什麼它不會問: 第 2 層指令微調把它訓練成「照指令執行」, 不是「反問釐清」. 要讓它停下來問, 要**明確在 prompt 裡強制**:

```
Before you start, list every assumption you'd have to make.
If any assumption would materially change the answer, ask me first.
```

或在 CLAUDE.md 裡寫:

```
When a task has multiple valid interpretations, STOP and surface them
before producing output.
```

> [!WARNING]
> 這一條跟 [LLM 極限](../limits-and-verification/llm-limitations.md) 的「自我修正 gap」互補. 自我修正 gap 是「它做錯了不會自己回頭」; underspecification silent failure 是「它根本沒意識到可能做錯」. 兩個都要**外部驗證**才接得住.

---

## 所以 prompt 跟 pipeline 要怎麼調 — Practical Takeaways

| 極限 | 徵狀 | 工程手段 |
|:---|:---|:---|
| **1. 長 context 衰退** | 128K context 塞進去, 結果忘了關鍵訊息 | 關鍵訊息放前段; 自己做 chunk + retrieval, 別信「全丟給它」 |
| **2. 示範飽和** | 塞了幾百個 example 效能反而掉 | 5-20 shot 當起點; 如果掉就減, 不是加 |
| **3. 順序敏感** | 同一組 demo, 不同天結果不同 | 代表性放開頭 + 接近 target 分佈放結尾; 多 order ensemble |
| **4. Rank-one 極限** | 看起來簡單的 bijection 類任務莫名歪 | 改成多次呼叫; 或配 retrieval + lookup table |
| **5. 情境可劫持** | Agent 讀了外部內容後行為怪異 | 外部內容一律 untrusted; INTEGRITY RULES 寫在 CLAUDE.md 跟 goal 裡 |
| **6. Silent failure** | 它產出看似合理但方向完全錯 | Prompt 強制列假設清單; 歧義時先問再做 |

**一條共同線**: 這六種失敗底下有個共同根因 — **第 1 層的任務推論沒有「不確定度」的輸出**. 它總是算出一個座標, 不管那個座標是算對、算錯、被劫持、還是被 context 衰退掩蓋. 工程手段的核心邏輯都是「**在它外面補一個不確定度的來源**」: 多次呼叫比對、retrieval 篩訊號、ensemble、強制反問 — 都是在補模型本身不會輸出的 confidence signal.

---

## 相關筆記

- [AI 101 - AI 怎麼知道該用哪種能力](how-ai-picks-capability.md) — 這篇的前提, 情境推任務的 8 層機制
- [AI 101 - LLM 的極限](../limits-and-verification/llm-limitations.md) — 結構性、暫時、鋸齒狀三類做不到
- [AI 101 - Context Engineering](../engineering/context-engineering.md) — 怎麼佈置 context, 跟這篇互為正反面
- [AI 101 - AI 產出怎麼驗](../limits-and-verification/verifying-ai-output.md) — 不確定度要從外部補的具體做法

## Sources

- [NoLiMa: Long-Context Evaluation Beyond Literal Matching — Modarressi et al., 2025](https://arxiv.org/abs/2502.05167)
- [Context Rot: How Increasing Input Tokens Impacts LLM Performance — Chroma, 2025](https://www.trychroma.com/research/context-rot)
- [Lost in the Middle: How Language Models Use Long Contexts — Liu et al., 2023](https://arxiv.org/abs/2307.03172)
- [Many-Shot In-Context Learning — Agarwal et al., Google DeepMind, 2024](https://arxiv.org/abs/2404.11018)
- [MIR-Bench: Can Your LLM Recognize Complicated Patterns via Many-Shot In-Context Reasoning? — 2025](https://arxiv.org/abs/2502.09933)
- [The Order Effect: Investigating Prompt Sensitivity to Input Order in LLMs — 2025](https://arxiv.org/html/2502.04134v2)
- [Understanding Task Vectors in In-Context Learning: Emergence, Functionality, and Limitations — ICLR 2026](https://arxiv.org/abs/2506.09048)
- [Benchmarking and Defending against Indirect Prompt Injection Attacks on Large Language Models (BIPIA) — 2024](https://arxiv.org/abs/2312.14197)
- [Automating Agent Hijacking via Structural Template Injection — 2026](https://arxiv.org/abs/2602.16958)
- [Underspecification does not imply Incoherence: The Risks of Semantic Collapse in Coding Models — 2026](https://arxiv.org/abs/2607.01953)
- [A Taxonomy of Prompt Defects in LLM Systems — 2025](https://arxiv.org/abs/2509.14404)
