---
title: AI 101 - AI 怎麼知道該用哪種能力
tags: [ai, 原理, in-context-learning, instruction-tuning, system-prompt, 可解釋性, 進階]
created: 2026-09-18
---

# AI 怎麼知道該用哪種能力 — How AI Picks a Capability

[← 回主頁](../index.md)

> [!NOTE]
> 你說「幫我摘要」它就摘要，說「幫我想幾個點子」它就發想 - 中間沒有人幫它選。這篇拆解那個選擇是怎麼發生的：**不是 system prompt 偷偷做掉的**，而是預訓練、指令微調、偏好訓練三層疊出來的結果，system prompt 只管最外面那一層。文末回頭檢驗「分析 ＝ 抽取 → 統整 → 評估」這類等式，到底是機制還是比喻。

> **TL;DR (EN):** Capability selection is not configured anywhere. Pretraining makes the model infer a latent task from context (in-context learning as implicit Bayesian inference); that inferred task provably exists inside the model as a single extractable direction (task vectors / function vectors). Instruction tuning binds natural-language instructions to those tasks. Preference training shapes the output's form, not its type. The published system prompt covers safety, tone, and formatting - it says nothing about which cognitive capability to apply, and the API has no system prompt at all yet still picks correctly.

---

## 先講結論：那個選擇沒有被寫在任何地方 — Nobody Configured It

最直覺的猜測是「背後有一張表，或一段 system prompt，告訴它遇到什麼要用什麼」。

這個猜測有一個很乾脆的反證：**你直接打 API、system prompt 留空，它照樣會摘要、會翻譯、會發想。** claude.ai 有 system prompt，API 沒有 - Anthropic 的文件明說「這些 system prompt 更新不適用於 Claude API」。能力選擇在沒有 system prompt 的情況下完全正常，所以它不可能是 system prompt 做的。

真正的答案是：**能力選擇是模型從你的話裡「推論」出來的，而推論這件事本身是訓練出來的。** 分成四層，每一層負責的東西不一樣。

---

## 四層機制：一層一層疊出來的 — Four Layers

| 層 | 什麼時候形成 | 決定什麼 | 白話 |
|---|---|---|---|
| 1. 預訓練 | 訓練時 | **這段話的潛在任務是什麼** | 認出「你要的是哪一種東西」 |
| 2. 指令微調 | 訓練時 | **聽得懂用講的下指令** | 把「摘要一下」對應到那個任務 |
| 3. 偏好訓練 | 訓練時 | **做到什麼程度、長什麼樣** | 多長、要不要列點、要不要先講結論 |
| 4. System prompt／工具 | 每次對話 | **安全、語氣、格式、能用什麼工具** | 最外層的外掛 |

能力選擇發生在第 1 層和第 2 層。第 4 層完全不碰它。

### 第一層：它在推論「現在是什麼任務」

Xie 等人 2021 年提出一個解釋：in-context learning 其實是**隱式的貝氏推論**（implicit Bayesian inference）。模型在預訓練時就一直在做一件事 - 推論「這份文件的潛在主題是什麼」，因為推對了才猜得準下一個字。到了使用時，它把同一個動作套到你的 prompt 上：**推論「這段對話的潛在任務是什麼」。**

這不只是理論。2023 年有兩組人各自找到了直接的機制證據：

- **Task vectors**（Hendel 等人）- in-context learning 的結果可以被壓縮成**單一一個向量**，從模型內部抽出來。
- **Function vectors**（Todd 等人）- 用因果中介分析定位到特定的 attention head，抽出的向量在全新的語境裡照樣能**觸發那個任務**。

最關鍵的實驗是這個：把示範例子餵給模型 → 抽出那個向量 → 換一個**完全沒有示範**的 zero-shot 場景 → 把向量貼回去 → 模型執行了那個任務，效果跟看過全部示範差不多。

> [!IMPORTANT]
> 這代表模型內部真的存在一個「我現在要做什麼」的表徵，而且它是一個可以被抽出、搬移、貼上的東西。不是隱喻，是可操作的物件。

換成白話：**它不是查表決定「要用摘要能力」，而是從你的話裡算出一個任務座標。** 這也解釋了為什麼給範例（few-shot）常常比堆形容詞有效 - 那個向量是從範例裡抽出來的。

### 第二層：讓「用講的」就能指定任務

第一層解決了「從語境推論任務」，但沒解決「聽得懂命令」。這是指令微調（instruction tuning）補的。

FLAN（Wei 等人，2021）是這條路線的代表：拿一個 137B 的模型，在 60 多個任務（後續版本擴到 1,800 多個）上微調，每個任務都用**自然語言指令**包裝。結果是在**沒訓練過的任務類型**上 zero-shot 大幅提升，25 個評測任務裡有 20 個贏過規模大得多的 175B GPT-3。

消融實驗指出三個要素缺一不可：任務數量、模型規模、**以及有沒有用自然語言指令包裝**。最後那項最說明問題 - 同樣的資料，不包成指令，效果就掉下來。

這一層教會的不是「怎麼摘要」（那第一層就會了），是「**聽到『摘要一下』要對應到哪個任務**」。

### 第三層：決定做到什麼程度

偏好訓練（RLHF 一類）不決定用哪種能力，決定輸出的形狀：多長、要不要列點、要不要先給結論、要不要加但書。同一個「摘要」任務，訓練成三句話或三頁，是這一層的事。

推理模型多了一個變種：用可驗證獎勵做 RL，讓模型學會**什麼時候該多想一下**。這也是能力調度，但調的是「思考深度」，不是「哪一種能力」。

---

## System prompt 實際上管什麼 — What the System Prompt Actually Does

這題可以直接查，不用猜。Anthropic 從 2024 年 8 月起公開 claude.ai 的 system prompt，每次改版都發布。

以 Claude Opus 5 的版本（2026-07-24）為例，它的段落標籤是：

```
product_information          產品資訊、有哪些模型
fable_safeguards_routing     安全分流說明
default_stance               預設要幫忙
refusal_handling             什麼該拒絕
legal_and_financial_advice   法律與財務問題的分寸
tone_and_formatting          語氣與格式
user_wellbeing               使用者身心狀況
anthropic_reminders          系統提醒的處理
evenhandedness               爭議議題的中立性
knowledge_cutoff             知識截止日
```

**沒有任何一段在講「遇到 X 類任務要用 Y 能力」。** 最接近的兩句在 `tone_and_formatting` 裡，而且管的是版面不是能力：

> Claude uses lists and bullet points when asked to or when the content is multifaceted enough that they help with clarity.

> when asked to explain something, Claude gives a high-level summary unless an in-depth one is specifically requested.

一句講「什麼時候用條列」，一句講「解釋要給多深」。都是**輸出的形狀**，不是**認知的種類**。

> [!TIP]
> 實務推論：想指定它用哪種能力，要寫在**請求裡**，不是寫在人設裡。把「你是一位資深分析師」換成「先把資料裡的數字抽出來，再判斷哪些重要」，後者才是在動第一層那個任務座標。

### 順帶澄清：MoE 的 router 不是能力選擇器

很多人聽到 Mixture-of-Experts 有個「router」，就以為那就是在選能力。不是。

MoE 的 router 是**逐 token** 運作的 - 同一句話裡，不同的字可能被送到不同的專家。專家學到的是「在這個語境下處理這個 token」的模式，通常偏向淺層的語法特徵，不是「摘要專家」「翻譯專家」這種分工。近年還有研究直接指出，路由反映的比較像表徵空間的幾何結構，而不是領域專長。

**它是架構層的效率機制，跟「要用哪種能力」是兩件事。**

---

## 回頭檢驗那兩條等式 — Are Those Equations Real?

[AI 能力全景圖](./ai-capability-landscape.md)裡有兩條拆解：

```
分析 ＝ 抽取（低階收斂）→ 統整（高階收斂）→ 評估（高階收斂）
規劃 ＝ 分析現況 ＋ 發想選項（高階發散）＋ 收斂成步驟（高階收斂）
```

那篇已經標明「沒有直接來源，是本文自己的表述」。現在從機制這邊再檢驗一次，會得到更細的答案。

**站得住的部分：模型內部確實會走多步驟。** Anthropic 2025 年的《On the Biology of a Large Language Model》給了兩個證據：問「Dallas 所在的那一州，首府是哪裡」，模型內部先浮現 Texas 再導出 Austin - 把中間那步換掉，答案跟著變；寫詩會先選好韻腳再回頭鋪陳。所以「一個動作其實由好幾步組成」這件事是真的。

**站不住的部分：不能因此推論它照著「抽取 → 統整 → 評估」這個順序走。** 上面那些是針對特定小任務做的機制研究，不是對「分析」這種複合任務的完整拆解。目前沒有研究驗證過這個三段式。

**而且不能拿它自己的說法當證據。** Chen 等人 2025 年的《Reasoning Models Don't Always Say What They Think》測了一件事：給模型一個提示、確認它用了那個提示，再看它的 chain-of-thought 有沒有承認。結果是**承認率通常低於 20%**。用可驗證獎勵做 RL 會在早期改善忠實度，但很快就停在一個高原，不會飽和。

> [!WARNING]
> 所以：**那兩條等式是行為層的好用模型，不是機制層的已驗證描述。** 它幫你想清楚要交辦什麼、要驗什麼；它不能拿來宣稱模型內部就是這樣跑的。
>
> 特別是 - 你叫它「先抽取再統整」，然後它在回答裡寫「我先抽取了以下要點……」，**那段話不構成它真的這樣做了的證據。**

### 三個可以直接用的推論

1. **模糊的 prompt ＝ 模糊的任務推論。** 任務座標是從你的話裡算出來的，話越模糊，算出來的位置越糊。給範例比給形容詞有效。
2. **能力要寫進請求，不是寫進人設。** 人設影響語氣（第三、四層），請求影響任務（第一、二層）。
3. **它的自述不能當驗證。** 要確認它有沒有做某一步，看產出對不對，不要看它說它做了什麼。這跟 [LLM 的極限](./llm-limitations.md)裡「驗證必須來自外部」是同一條。

---

## 常見問題 — FAQ

**Q：所以是模型「自己知道」，還是被教的？**
兩者都有，但分工明確。「從語境認出任務」是預訓練的副產品（為了猜準下一個字，它不得不學會推論主題）；「聽懂用講的下指令」則是指令微調明確教的。

**Q：既然是推論，它會推錯嗎？**
會，而且這是最常見的一種失敗。你以為你在要求「統整」，它推成「摘要」，於是你拿到一份忠實但沒有新判斷的整理。修法不是重講一次，是換成可驗收的描述：與其說「幫我統整」，不如說「把這五份的共識和分歧各列一條，每條註明來自哪幾份」。

**Q：我可以直接操作那個 task vector 嗎？**
研究環境可以（開源模型 + 抽取 hidden state），一般 API 使用者不行。對日常使用來說，範例就是你操作它的介面。

**Q：那 system prompt 到底有沒有用？**
非常有用，只是管的不是能力。它管安全邊界、語氣、格式、可用工具 - 這些會實質改變你拿到的東西長什麼樣。只是「它會不會摘要」不歸它管。

---

## 相關筆記

- [AI 101 - AI 能力全景圖](./ai-capability-landscape.md) - 這篇檢驗的那兩條等式的出處
- [AI 101 - LLM 的極限](./llm-limitations.md) - 為什麼驗證必須來自外部
- [AI 101 - Context Engineering](./context-engineering.md) - 既然任務是從語境推論的，餵什麼就決定推成什麼
- [AI 101 - 核心概念](../01-fundamentals/core-concepts.md) - Agent、RAG、幻覺等基礎詞彙

## Sources

- [An Explanation of In-context Learning as Implicit Bayesian Inference — Xie, Raghunathan, Liang & Ma, ICLR 2022](https://arxiv.org/abs/2111.02080)
- [In-Context Learning Creates Task Vectors — Hendel, Geva & Globerson, Findings of EMNLP 2023](https://aclanthology.org/2023.findings-emnlp.624/)
- [Function Vectors in Large Language Models — Todd et al., ICLR 2024](https://functions.baulab.info/)
- [Finetuned Language Models Are Zero-Shot Learners（FLAN）— Wei et al., 2021](https://arxiv.org/abs/2109.01652)
- [Claude Opus 5 system prompts — Anthropic（2026-07-24 版全文）](https://platform.claude.com/docs/en/release-notes/system-prompts/claude-opus-5)
- [System prompts 總覽（說明不適用於 API）— Anthropic](https://platform.claude.com/docs/en/release-notes/system-prompts)
- [Reasoning Models Don't Always Say What They Think — Chen et al., Anthropic, 2025](https://arxiv.org/abs/2505.05410)
- [On the Biology of a Large Language Model — Anthropic, 2025](https://transformer-circuits.pub/2025/attribution-graphs/biology.html)
- [A Comprehensive Survey of Mixture-of-Experts: Algorithms, Theory, and Applications — 2025](https://arxiv.org/abs/2503.07137)
- [Mixture-of-Experts (MoE) vs. Dense LLMs — Sebastian Raschka](https://sebastianraschka.com/faq/docs/mixture-of-experts.html)
