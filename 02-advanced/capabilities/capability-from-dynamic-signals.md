---
title: AI 101 - 情境擴到感測與市場, 推任務還撐得住嗎
tags: [ai, 情境, world-model, 自動車, VLA, 股市, chaos, 效率市場, 進階]
created: 2026-10-14
---

# 情境擴到感測與市場, 推任務還撐得住嗎 — Can Capability-from-Context Scale to Perception and Markets?

[← 回主頁](../../index.md)

> [!NOTE]
> 姊妹篇 [AI 怎麼知道該用哪種能力](./how-ai-picks-capability.md) 的 Layer 1 說「從情境推任務座標」. 但那是**文字情境**. 如果情境換成**動態實體訊號** — 自動車感測 → 下一步要怎麼開、新聞 + 買賣量 → 股市下一步怎麼走 — 這套推論還撐得住嗎? 短答: **撐到物理牆為止**. 這篇把四面牆拆出來, 配 2024-2026 的實證數字, 再回頭看兩個具體場景 (自動車 VLA + 股市 LLM) 的現況與極限.

> **TL;DR (EN):** Layer 1 (context → task) assumes the context is text. Swap it for sensor streams or market signals and you hit four walls. (1) **Ontology wall** — sequence models learn P(x|context), not P(s'|s,a). Flux game: LLMs produce ~10% invalid moves vs RL's 0%. (2) **Chaos wall** — error grows exponentially at the system's Lyapunov rate; no scaling fixes this. (3) **Distribution wall** — VLA models drop ~40% on novel geometries; illumination shifts can make them color-blind. (4) **Temporal consistency** — fidelity decays like √(horizon); long-horizon tasks drift. For markets add a fifth: **efficient-markets wall** — Lopez-Lira finds GPT-4 beats sentiment baselines but only ~1-2 trading days of drift. π0.5 generalizes to new homes but still reopens drawers it just closed. The common root: pure sequence models always emit an answer; in dynamic contexts the answer silently decouples from the environment. Fix is outside the model: tight-loop verification, short horizons + fast replan, ensembles, don't deploy open-loop.

```markdown
# 情境擴到感測與市場的四面牆
* 本體落差 — text 序列 ≠ world 狀態
  * P(x|context) vs P(s'|s,a)
  * Flux game 10% invalid moves
* 混沌牆 — Lyapunov horizon
  * 誤差 exponential growth
  * 不是模型問題, 是物理
* 分佈牆 — distribution shift
  * VLA 新 geometry 掉 40%
  * 光線變了就 color-blind
* 時間一致性牆 — sqrt(horizon) decay
  * 長時任務漂移
  * 行為跟 goal 脫鉤
* 效率市場牆 — 僅限股市
  * alpha 窄 (1-2 天 drift)
  * 公開資訊被套利掉
* 共同根 — 沒有不確定度輸出
  * 工程只能 outside loop 補
```

---

## 1. 本體落差: text 序列 ≠ world 狀態 — Sequence Models Aren't World Models

LLM 的訓練目標是 **P(x_t | x_<t)** — 看完前面所有 token 猜下一個. 這個目標對「文字情境推任務」很合適, 因為任務本身就是文字. 但到了動態實體訊號 (感測、價格、物理), 需要的是另一個東西: **s_{t+1} = f(s_t, a_t)** — 給定當前狀態跟行動, 預測下一步狀態.

這兩個目標**本質不同**. 2026 年 arxiv 2605.23972 (《Why We Need World Models for AGI》) 把差別講得乾淨:

- **LLM 學的是觀察 → 觀察的 correlation** (passive observation): P(Y|X)
- **World model 要學的是行動 → 狀態改變的 causation** (interventional): **P(Y|do(X))**

高 token 預測準確率, 不保證 causal consistency、不保證可以 compositional updatable、不保證跟環境 transition dynamics 對齊.

這不是抽象差異. 同篇論文的 **Flux game** 實驗給了具體數字:

| 失敗類別 | 具體行為 |
|:---|:---|
| **Sum blindness** | 總和已經接近 loss 條件 20, LLM 還持續加大數字 |
| **Row-length miscounting** | 盤面 contract 後, LLM 還引用已不存在的 cell index |
| **Invalid move rate** | LLM ~**10%** / RL agent **0%** |

這三種失敗都指向同一件事: **LLM 沒有一個「persistently updated latent state」**. 它每一步都在 token level 猜下一個合理的字, 不是在內部維護一份「盤面現在長什麼樣」的狀態. 文字情境裡這個差異看不太出來, 動態實體情境裡會以 10% invalid 的頻率炸開.

> [!IMPORTANT]
> 這一條是**所有其他極限的母牆**. 下面講的混沌、distribution shift、時間一致性, 都可以追到「序列預測不等於狀態演進」這個根上.

---

## 2. 四面物理牆 — Four Walls Every Dynamic Context Hits

### 牆 1: 混沌牆 — Lyapunov Horizon

某些系統是**混沌動力系統** (chaotic dynamical system): 初始狀態的微小差異, 隨時間 **指數放大**. 誤差放大的速率叫 **maximum Lyapunov exponent** (λ), 預測誤差大約是 ε₀ × e^(λt).

這代表**無論模型多強, 預測精度在時間 1/λ 內就會崩**. 這是物理牆, 不是技術限制.

天氣是經典案例: 全球天氣系統的 Lyapunov 時間大約 1-2 週, 所以氣象預報再怎麼進步, 兩週以上基本都是猜. 股市更短 — intraday 到幾天, 看規模.

- 2026 Advanced Science 的 《Causal-Guided Ultra-Long-Term Time Series Forecasting》 直接點名: **「prediction error for chaotic systems grows exponentially with the forecast horizon as governed by the system's maximum Lyapunov exponent」**.

**工程意義**: 不要用 LLM 做超過系統 Lyapunov 時間的 open-loop 預測. 自動車預測他車 3 秒內的軌跡還好, 30 秒就不要信. 股市下一分鐘的方向有可能有訊號, 下一週就沒有.

### 牆 2: 分佈牆 — Distribution Shift

訓練分佈 ≠ 部署分佈. 這對 LLM 文字任務還好 (網路文字差異有限), 對感測-行動模型是**致命的**.

**Vision-Language-Action (VLA)** 這條線的最新模型是 **π0.5** (Physical Intelligence, 2025-04, arxiv 2504.16054). 它能到真實家庭 (2-5 分鐘的 kitchen cleaning / bedroom tidying), 但失敗模式都在**分佈外**:

- **不熟悉的物理界面**: 「unfamiliar handles on drawers, or cabinets that are physically hard for the robot to open」. 把手跟訓練資料裡的不同形狀, 就卡.
- **部分可觀測性**: 「the robot arm occluding a spill that should be wiped」. 自己的手遮住要擦的東西, 就斷鏈.

更一般的量化, VLA 研究整體收斂到幾個數字:

- **新 geometry 掉 ~40%** performance
- **Scaling law exponent 僅 0.15-0.25** — diminishing returns 很明顯, 不是加資料加算力就能補
- **Illumination shift 直接 color-blind**: 2026 arxiv 2607.14698 《Lights, Camera, Malfunction》 — 光線條件改變, VLA 對顏色的辨識整個崩掉

對比 **RT-2** (Google 2023) 從 RT-1 的 32% unseen scenarios success rate 翻倍到 62% — 這還是「一倍」, 不是「十倍」. 感測 → 行動模型即便 scale 到 55B 參數, 在新場景的 generalization 仍然遠低於文字 LLM 在新 prompt 上的表現.

### 牆 3: 時間一致性牆 — √(Horizon) Temporal Decay

即便場景沒變, 時間拉長本身就是問題. VLA 的 temporal consistency 大約跟 **√(時間 horizon)** 退化. 行為序列越長, 偏離越嚴重.

π0.5 的具體症狀: 「closing and opening a drawer multiple times while putting away items」 — 把東西放進抽屜後, 又關起來開起來循環, 卡在局部循環出不來. 這對應 2026 arxiv 2609.17419 《World Model Science: Self-Organized Criticality, Weak Chaos, and Metastable Belief Dynamics in Long-Horizon LLM Agents》 的核心發現:

> **Locally valid actions can persist after global state fidelity fails.**

局部每一步都是合法的 (開櫃子、關櫃子都是訓練資料裡看過的動作), 但**整體 state 的追蹤已經崩了**, 只是模型不知道.

**工程意義**: 長時任務要**切短**. 每 5-10 秒重跑一次 state estimation, 不要信單一 session 的連續 10 分鐘推理.

### 牆 4: 效率市場牆 — 僅限市場 (Efficient Markets Hypothesis)

這一條是股市特有的. **公開資訊會迅速被價格吸收**. 讀新聞的 alpha 本質是「比別人快」, 不是「比別人準」. 一旦大家都用 LLM 讀新聞, alpha 就被套利掉.

Alejandro Lopez-Lira & Yuehua Tang (UF, 2023 → 2025 revised) 的 《Can ChatGPT Forecast Stock Price Movements?》 (arxiv 2304.07619) 給了迄今最乾淨的量化結果. Sample 從 **2021-10 到 2024-05** (在 GPT 的 training cutoff 之後, 避免 lookahead bias):

- **Overnight headline 的即時反應命中率 93.3%**, intraday 新聞 88.8%
- **初始反應之後, 價格繼續朝 ChatGPT 判斷的方向走 1-2 個交易日**
- Daily rebalanced strategy: **pre-cost 日均 34 bp, 命中率 58%, Sharpe 2.97** — 看起來很好, 但這是 pre-cost
- 2026 擴展研究: Fama-French 五因子 alpha ~**14.94% / 年**, 顯著但扣除交易成本後顯著性剩多少是另一題

但也別高估. **2026 的回頭研究** (research results in search) 提到 LLM 投資建議常犯:

- **Financial misconceptions**: 把會計概念或財務比率搞混
- **Carryover errors**: 把前面文件裡的錯誤帶到結論
- **Hallucinated / outdated info**: 用訓練時的舊資料當現況

結論: **「適當監督下 LLM 可以 outperform, 但要大量人為 oversight 才能真的賺到」**. 這個時間窗跟 Lyapunov 一樣, 會隨市場適應持續壓縮.

---

## 3. 兩個具體場景的現況 — Two Real Cases

### 自動車感測 → 下一步移動 — The Driving Case

這題不是「LLM 開車」, 是**感測訊號 → 行動**的通用問題. 2024-2026 的主流方向是 **Vision-Language-Action (VLA)**, 不是純 LLM:

| 代表模型 | 做什麼 | 關鍵數字 |
|:---|:---|:---|
| **RT-2** (Google, 2023) | PaLI-X / PaLM-E fine-tune, 把動作 discretize 成 token | 新場景 32% → 62% (6000+ trials) |
| **π0.5** (PI, 2025) | 跨 robot 共訓, high-level semantic prediction | 完整家庭任務 2-5 分鐘, 104 training locations 接近 target home 水準 |
| **DriveLM / OpenDriveVLA / World4Drive** (2024-2026) | 把 world model 嵌進 VLA 的 latent | 多篇 arxiv, 強調 planning-relevant 的 latent world model |

**這條線的大方向**: 單純堆 LLM 不夠, 要**加 world model 當內部 state** (ForeDrive, LWDrive, World4Drive 等 2026 新論文的共同標題). 等於是在模型內部補一個「state transition f(s, a)」, 彌補純序列模型的本體落差.

**實務極限**:

- **短 horizon (3-5 秒內的軌跡)**: 可用, 配 ensemble 跟 fallback 可以上路
- **長 horizon (路口決策、繞開施工區)**: 需要外部 planning 層, 不能全靠 end-to-end VLA
- **分佈外物理**: 下雨天、炫光、新路況, VLA 掉 40% 就是正常狀況
- **安全邊界**: 幾乎所有量產自駕系統都還是 modular (perception / prediction / planning 分離), VLA 這條線在 demo 強, 量產車仍遠

### 新聞 + 買賣量 → 股市下一步 — The Market Case

| 可以做 | 做不到 |
|:---|:---|
| 新聞 sentiment → 1-2 天 price drift (Lopez-Lira) | 長期 alpha (會被套利掉) |
| 分類「這是好消息還是壞消息」 | 判斷「這個好消息已經 priced in 多少」 |
| 從公開 10-K 找財務指標的異常 | 預測下一個財報週期 (混沌 + 效率市場雙重作用) |
| 事件驅動 (財報、M&A、policy announcement) 的即時反應 | 市場 regime shift (牛熊轉換) |

**關鍵 insight**: LLM 在市場上的 edge 不是「比人聰明」, 是「比人快讀」. 一旦 LLM 普及到買方端, edge 就壓到接近零 — 這是**反射性 (reflexivity)** 問題: 預測工具普及後, 預測本身改變被預測的現象.

**實務極限**:

- **分鐘到 1-2 天**: 可能有訊號, 但要配 transaction cost / slippage 才算真的賺
- **週到月**: Lyapunov + 效率市場雙殺, 幾乎沒有可持續的 alpha
- **季到年**: 這段跟宏觀經濟週期有關, LLM 的優勢不大於傳統 econometrics
- **執行層**: LLM 判斷 → 下單這段仍需人類風控, Carryover error 跟 hallucination 在真金白銀的場景下不能容忍

---

## 4. 共同根跟工程手段 — Common Root and Playbook

把四面牆加兩個 case 串起來, **共同根**跟姊妹篇 [情境推任務的 6 種失手](./limits-of-capability-from-context.md) 一樣: **Layer 1 的任務推論, 永遠會輸出一個答案, 但在動態實體情境下, 這個答案會跟現實悄悄脫鉤而模型不自覺**.

文字情境裡這個脫鉤可以人工 review 抓到; 動態情境裡**沒有人在每一步 review**, 脫鉤會累積.

工程手段都是**在模型外補一個「現實錨點」**:

| 牆 | 工程手段 |
|:---|:---|
| **本體落差** | 不要用純 LLM 做 state tracking; 加 explicit world model (RL / Kalman filter / symbolic planner) |
| **混沌牆** | 預測 horizon 壓短; 系統的 Lyapunov 時間是上限, 不要超過 |
| **分佈牆** | OOD detection + fallback; 遇到不熟的場景降級到保守模式 (自動車靠邊停、交易停單) |
| **時間一致性** | 短 session + 定期重估 state; 長任務切成多段 |
| **效率市場** | 不要指望 public info 的持久 alpha; LLM 當輔助不是主策略 |
| **(共同根)** | **Outside loop** 的 verification: tight-loop sensor + action monitor, ensemble 多模型 vote, 強制 human oversight 在關鍵決策點 |

**底線**: 文字情境的 Layer 1 推論, 擴到動態實體訊號**不是壞**, 但極限變得更低、更實體化. 短 horizon + 配 world model + 外部 verification 可以用; open-loop + 長 horizon + 相信單一 LLM 就是在撞牆.

---

## 相關筆記

- [AI 101 - AI 怎麼知道該用哪種能力](./how-ai-picks-capability.md) — 這篇的前提, Layer 1 的 8 層機制
- [AI 101 - 情境推任務會在哪裡失手](./limits-of-capability-from-context.md) — 文字情境下 Layer 1 的 6 種極限 (姊妹篇, 不同切面)
- [AI 101 - LLM 的極限](../limits-and-verification/llm-limitations.md) — 鋸齒狀前沿等結構性極限
- [AI 101 - AI 產出怎麼驗](../limits-and-verification/verifying-ai-output.md) — Outside-loop verification 的具體做法

## Sources

- [Why We Need World Models for AGI: Where LLMs Fail and How World Models May Outperform — 2026](https://arxiv.org/html/2605.23972)
- [World Model Science: Self-Organized Criticality, Weak Chaos, and Metastable Belief Dynamics in Long-Horizon LLM Agents — 2026](https://arxiv.org/pdf/2609.17419)
- [Causal-Guided Ultra-Long-Term Time Series Forecasting Via Anticipated Covariates — Zhao et al., Advanced Science 2026](https://advanced.onlinelibrary.wiley.com/doi/10.1002/advs.76201)
- [π0.5: a Vision-Language-Action Model with Open-World Generalization — Physical Intelligence, 2025](https://arxiv.org/abs/2504.16054)
- [Where Success Breaks: Failure-Boundary Learning for Robust Vision-Language-Action Models — 2026](https://arxiv.org/pdf/2609.06114)
- [Lights, Camera, Malfunction: When Illumination Robustness Leaves VLA Models Blind to Color — 2026](https://arxiv.org/pdf/2607.14698)
- [World4Drive: End-to-End Autonomous Driving via Intention-aware Physical Latent World Model — 2025](https://arxiv.org/pdf/2507.00603)
- [DriveLM: Driving with Graph Visual Question Answering — ECCV 2024](https://arxiv.org/abs/2312.14150)
- [Can ChatGPT Forecast Stock Price Movements? Return Predictability and Large Language Models — Lopez-Lira & Tang, 2023 (revised 2025)](https://arxiv.org/abs/2304.07619)
- [Can LLM-based Financial Investing Strategies Outperform the Market in Long Run? — 2025](https://arxiv.org/html/2505.07078v5)
- [Inducing Causal World Models in LLMs for Zero-Shot Physical Reasoning — 2025](https://arxiv.org/html/2507.19855)
