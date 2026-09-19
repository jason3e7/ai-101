---
title: AI 101 - AI 怎麼知道該用哪種能力
tags: [ai, 原理, in-context-learning, instruction-tuning, system-prompt, 可解釋性, 進階]
created: 2026-09-18
---

# AI 怎麼知道該用哪種能力 — How AI Picks a Capability

[← 回主頁](../index.md)

> [!NOTE]
> 你說「幫我摘要」它就摘要，說「幫我想幾個點子」它就發想 - 中間沒有人幫它選。這篇以 **Claude Opus 5** 為基準，把那個選擇拆成八層：**五層在訓練時就長好了，三層在你每次送出請求時自動跑**。重點不是知道有幾層，而是知道**哪幾層你動得了、哪幾層動不了** - 因為那直接決定 prompt 該往哪使力。

> **TL;DR (EN):** Nothing configures which capability Claude uses. Baselined on Opus 5, the mechanism is eight layers: five baked in at training (pretraining's latent-task inference, instruction tuning, Constitutional-AI preference training, character training, verifiable-reward reasoning RL) and three that run by default on every request (sampling - now locked, adaptive thinking - you can only hint, safety classifiers). Only layers 1 and 2 choose the capability, but 5 through 7 change what you get back. Practical upshot: examples beat adjectives, actions beat personas, stated difficulty beats "think carefully", and diversity now has to come from structure because temperature is a 400 error.

---

## 先講結論：那個選擇沒有被寫在任何地方 — Nobody Configured It

最直覺的猜測是「背後有一張表，或一段 system prompt，告訴它遇到什麼要用什麼」。

這個猜測有一個很乾脆的反證：**你直接打 API、system prompt 留空，它照樣會摘要、會翻譯、會發想。** claude.ai 有 system prompt，API 沒有 - Anthropic 的文件明說「這些 system prompt 更新不適用於 Claude API」。能力選擇在沒有 system prompt 的情況下完全正常，所以它不可能是 system prompt 做的。

真正的答案是：**能力選擇是模型從你的話裡「推論」出來的，而推論這件事本身是訓練出來的。**

但「推論出任務」只是第一步。從你按下送出到拿到回答，中間還有好幾層各自在做事，而且多數沒有開關。下一節按層拆開。

---

## 機制有幾層：訓練時五層，推論時三層 — Eight Layers, None of Them a Setting

以 **Claude Opus 5** 為基準拆開來看，能力選擇不是單一機制，是八層疊出來的。分成兩組：**五層在訓練時就長好了**（模型出廠就帶著，改不了），**三層在你每次送出請求時自動跑**（預設就會做，多數你關不掉）。

| # | 層 | 什麼時候 | 你能動嗎 | 決定什麼 |
|:---|:---|:---|:---|:---|
| 1 | 預訓練 | 訓練時 | ❌ | **這段話的潛在任務是什麼** |
| 2 | 指令微調 | 訓練時 | ❌ | 聽得懂用講的下指令 |
| 3 | 偏好訓練（RLHF ＋ 憲法 AI） | 訓練時 | ❌ | 回答的形狀、什麼該拒絕 |
| 4 | 性格訓練 | 訓練時 | ❌ | 語氣、好奇心、怎麼權衡 |
| 5 | 推理訓練（RLVR） | 訓練時 | ❌ | **什麼時候該多想一下** |
| 6 | 取樣設定 | 每次請求 | ❌ **鎖死了** | 輸出的隨機性 |
| 7 | 自適應思考 | 每次請求 | ⚠️ 只能建議 | 這題要不要想、想多久 |
| 8 | 安全分類器 | 每次請求 | ❌ | 這題會不會被攔 |

**能力選擇主要發生在第 1、2 層。** 但第 5–7 層會實質改變你拿到的東西，而它們最常被忽略，因為它們沒有開關。

---

### 訓練時長好的五層

**第 1 層：預訓練 - 它在推論「現在是什麼任務」**

Xie 等人 2021 年提出的解釋：模型在預訓練時，為了把下一個字猜準，不得不一直做一件事 - **推論這份文件的潛在主題是什麼**。到了使用時，它把同一個動作套到你的 prompt 上：推論「這段對話的潛在任務是什麼」。這叫**隱式貝氏推論**（implicit Bayesian inference）。

這不只是理論。2023 年有兩組人各自找到直接的機制證據：

- **Task vectors**（Hendel 等人）- in-context learning 的結果可以被壓縮成**單一一個向量**，從模型內部抽出來
- **Function vectors**（Todd 等人）- 用因果中介分析定位到特定 attention head，抽出的向量在全新語境裡照樣能**觸發那個任務**

最關鍵的實驗：把示範例子餵給模型 → 抽出那個向量 → 換一個**完全沒有示範**的 zero-shot 場景 → 把向量貼回去 → 模型執行了那個任務，效果跟看過全部示範差不多。

> [!IMPORTANT]
> 模型內部真的存在一個「我現在要做什麼」的表徵，而且它可以被抽出、搬移、貼上。不是隱喻，是可操作的物件。
>
> **換成白話：它不是查表決定「要用摘要能力」，而是從你的話裡算出一個座標。**

**第 2 層：指令微調 - 讓「用講的」就能指定**

第 1 層解決了「從語境推論任務」，但沒解決「聽得懂命令」。FLAN（Wei 等人，2021）是這條路線的代表：一個 137B 模型，在 60 多個任務（後續擴到 1,800 多個）上用**自然語言指令**包裝後微調，結果在**沒訓練過的任務類型**上 zero-shot 大幅提升，25 個評測任務裡 20 個贏過規模大得多的 GPT-3。

消融實驗指出三個要素缺一不可：任務數量、模型規模、**以及有沒有用自然語言指令包裝**。這一層教會的不是「怎麼摘要」（第 1 層就會了），是「**聽到『摘要一下』要對應到哪個任務座標**」。

**第 3 層：偏好訓練 - 決定回答長什麼樣**

這一層不決定用哪種能力，決定輸出的形狀：多長、要不要列點、要不要先給結論、要不要加但書。

Claude 這一層的特別之處是**憲法 AI（Constitutional AI）**，分兩階段：先讓模型依一份寫好的原則**自我批判並修訂**自己的回答，用修訂後的結果做監督式微調；再用 **AI 回饋取代人類標註**（RLAIF）跑強化學習。有害性的回饋幾乎全部由模型自己產生，只有「有沒有幫上忙」還保留人類回饋。

**第 4 層：性格訓練 - 語氣與傾向**

Anthropic 另外做了一個「**性格版**」的憲法 AI：讓 Claude 自己生出跟某個特質相關的訊息、產生符合該特質的回應，再由它**自己排序哪個回應最符合那個性格**，用這批合成資料訓練偏好模型。

Anthropic 明講這不只是體驗設計，而是對齊工作本身的一部分 - 好的性格特質會影響模型**在什麼情況下判斷該拒絕**，不只影響語氣。

**第 5 層：推理訓練 - 什麼時候該多想**

這是推理模型那一代新增的：用**可驗證獎勵的強化學習**（RLVR，如 GRPO）訓練，答案對不對可以程式化判定。

重點是它的副作用：**RLVR 不只優化正確率，還會塑造思考的模式**。用最終答案當獎勵訓練出來的模型，會漂移成一種「生成式搜尋」的風格 - 列出幾條暫時的分支、修改中間步驟、明確地自我修正。

> [!NOTE]
> 這一層就是為什麼現在的模型會「自己決定要想多久」。那個判斷力是訓練出來的，不是規則寫死的。

---

### 推論時自動跑、你關不掉的三層

**第 6 層：取樣設定 - 已經鎖死了**

`temperature`、`top_p`、`top_k` 在 Claude Opus 5 上**設成非預設值會直接回 400 錯誤，不管有沒有開思考**。

這件事的意義比它看起來大：**「調高 temperature 讓它更有創意」這個手段，在現行模型上已經不存在了。** 隨機性由 Anthropic 決定，你動不了。要更發散只剩兩條路 - 在 prompt 裡明講要幾個互不重疊的方向，或者多次獨立呼叫再自己彙整。

**第 7 層：自適應思考 - 你只能建議，不能指定**

Opus 5 預設就會思考，而且**由模型自己決定這題要不要想、想多深**。官方說明：「Claude 會不會在某個請求上思考、想多深，取決於你的思考設定**與請求的複雜度**。」

你手上的旋鈕是 `effort`（`low`／`medium`／`high`／`xhigh`／`max`，預設 `high`），但官方明講它是「**行為訊號，不是嚴格的 token 預算**」 - 設低了，遇到夠難的題目它還是會想，只是想得比高的時候少。

而且你看不到它真正想了什麼：**回傳的 thinking 區塊永遠是摘要，不是原始思路**，而 `display` 的預設值在多數模型上是 `omitted`（整段空白）。

> [!WARNING]
> 這一層對 prompt 的意義最直接：**「請仔細思考」這類話不是沒用，但它是在跟第 7 層商量，不是在下指令。** 真正的槓桿是把任務的難度講清楚 - 難度是它判斷要想多久的依據。

**第 8 層：安全分類器 - 在你看不到的地方跑**

Opus 5 的網安防護是**兩階段**的：一個探針（probe）掃描**所有流量**的內部激活狀態，命中就把該對話升級給一個受過訓練的 LLM 分類器判斷。

這一層跟能力選擇無關，但會實質影響你的體驗 - 它是為什麼同一類問題有時會被擋。Anthropic 表示 Opus 5 的分類器觸發頻率比前一代**少 85%**（FrontierBench 上從 42% 降到 5%），誤攔變少了，但機制仍然在跑。

---

### 所以 prompt 要怎麼調 — What This Means For Prompting

把八層對應回實際做法，得到四條：

| 因為這一層…… | 所以…… |
|:---|:---|
| **1. 任務座標是算出來的** | **給範例勝過給形容詞。** 話越模糊，算出來的位置越糊；而 task vector 本來就是從範例裡抽的 - 範例是你操作它最直接的介面 |
| **3＋4. 形狀與語氣是訓練死的** | **別用人設去指定能力。** 「你是一位資深分析師」動的是第 3、4 層（語氣），動不到第 1 層（任務）。要改任務就寫動作：「先把數字抽出來，再判斷哪些重要」 |
| **5＋7. 思考深度由它自己判斷** | **與其叫它「想仔細一點」，不如把難度說清楚。** 講明有幾個限制條件、要權衡什麼、錯了會怎樣 - 這些是它判斷要想多久的輸入 |
| **6. 隨機性鎖死了** | **要多樣性只能靠結構。** 明講「三個核心思路不可重疊」，或分多次獨立呼叫，不要再想著調參數 |

## 補充：那 system prompt 管什麼 — The Layer That Is a Setting

> [!NOTE]
> 上面八層都不是設定檔。System prompt 是唯一一層**真的可以用文字改**的，所以特別容易被當成答案。這節用公開原文說明它實際管什麼 - 結論是：不管能力選擇。

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

這一條跟前面的機制扣得很緊：**第 7 層回傳的思考區塊本來就只是摘要，不是原始思路**，預設甚至整段空白。你看到的「我的步驟」跟它實際跑的不是同一個東西 - 兩個獨立的理由指向同一個結論。

**要確認它有沒有做某一步，看產出對不對，不要看它說它做了什麼。** 這跟 [LLM 的極限](./llm-limitations.md)裡「驗證必須來自外部」是同一條。

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
- [Thinking — Claude Platform Docs（自適應思考、思考摘要、取樣參數限制）](https://platform.claude.com/docs/en/build-with-claude/thinking)
- [Effort — Claude Platform Docs（effort 是行為訊號而非 token 預算）](https://platform.claude.com/docs/en/build-with-claude/effort)
- [Model deprecations — Claude Platform Docs（temperature／top_p／top_k 棄用）](https://platform.claude.com/docs/en/about-claude/model-deprecations)
- [Constitutional AI: Harmlessness from AI Feedback — Anthropic, 2022](https://arxiv.org/abs/2212.08073)
- [Claude's Character — Anthropic（性格版憲法 AI 的訓練方式）](https://www.anthropic.com/research/claude-character)
- [Claude Opus 5 System Card — Anthropic, 2026-07-24](https://www-cdn.anthropic.com/c5fbac3f0b1280a933ebd26d3cb8bb9f5bdeaf48/Claude%20Opus%205%20System%20Card.pdf)
- [RLVR Implicitly Incentivizes Correct Reasoning in Base LLMs — 2025](https://arxiv.org/abs/2506.14245)
- [A Comprehensive Survey of Mixture-of-Experts: Algorithms, Theory, and Applications — 2025](https://arxiv.org/abs/2503.07137)
- [Mixture-of-Experts (MoE) vs. Dense LLMs — Sebastian Raschka](https://sebastianraschka.com/faq/docs/mixture-of-experts.html)
