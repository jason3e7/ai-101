---
title: AI 101 - Qwen3.6 27B 原版 vs Uncensored（abliterated）
tags: [ai, local-model, qwen, ollama, abliteration, uncensored, 安全]
created: 2026-09-04
---

# Qwen3.6 27B：原版 vs Uncensored — Base vs Abliterated

[← 回主頁](../index.md)

> [!NOTE]
> 兩者是**同一個模型**。差別不在重新訓練，而在權重被動了手術——把「拒絕」這個方向從權重裡投影掉。檔案一樣大、參數一樣多，但代價不是零，而且代價**不只是「它會回答壞問題」**。

> **TL;DR (EN):** The "uncensored" build is not a different model. It is Qwen3.6-27B with the refusal direction ablated out of its weight matrices — no retraining, same parameter count, same file size. Capability mostly survives, but the cost is real and often invisible: depending on implementation GSM8K swings from +1.51 to −18.81 points, and a 2026 study found abliterated models become measurably more optimistic and express uncertainty less, with confidence shifting in opposite directions across model families. Instruction-following stays intact, which is exactly why the damage is easy to miss.

---

## 差在哪 — What Actually Differs

| | **Qwen3.6-27B（原版）** | **Qwen3.6-27B abliterated / uncensored** |
|---|---|---|
| 誰做的 | Alibaba Qwen 團隊，2026-04-22 釋出 | 第三方社群（huihui-ai、Youssofal、HauhauCS 等） |
| 授權 | Apache 2.0 | Apache 2.0（衍生） |
| 怎麼來的 | 完整預訓練 ＋ 後訓練對齊 | **沒有再訓練**，直接改權重 |
| 拒絕行為 | 敏感請求會拒絕 | 幾乎不拒絕（有的版本標「0/465 refusals」） |
| 參數量 | 約 27–28B dense | 一樣 |
| 檔案大小 | 18 GB（Ollama `27b`） | 17 GB（Q4_K_M） |
| 官方支援 | 有 | 無，出事自己負責 |

> **一句話：它們是同一份權重，其中一份被拿掉了一個方向。**

原版的規格是兩者共同的底子：

- **2026-04-22 釋出**，Qwen3.6 家族第一個 dense 開放權重模型，Apache 2.0
- **256K context**，最多 131K 輸出
- 混合架構：Gated DeltaNet 線性注意力 ＋ 傳統 self-attention，另有 Thinking Preservation 機制
- SWE-bench Verified **77.2**、Terminal-Bench 2.0 **59.3**（與 Claude 4.5 Opus 打平），贏過自家 397B 的 MoE 模型
- 單張 RTX 4090（24GB）或 5090（32GB）跑得動，還留得下 128K context

---

## 「無審查」是怎麼做出來的 — How Abliteration Works

技術名稱是 **abliteration**（ablation ＋ obliterate），2024 年 4 月由 Arditi 等人提出。三個步驟：

1. 準備兩組提問：一組**會被拒絕的**、一組**不會被拒絕的**
2. 兩組都跑過模型，取殘差流（residual stream）的平均活化值，相減 → 得到一條「**拒絕方向**」（refusal direction）
3. 把每一層的權重矩陣對這條方向做正交化，讓模型**沒辦法表達拒絕**

> [!IMPORTANT]
> 關鍵理解：**它不是「學會了不拒絕」，而是「被拿掉了拒絕的能力」。** 沒有重新訓練，只是把一個方向從權重裡投影掉。

現在有自動化工具 **Heretic**（`pip install heretic-llm`），用 Optuna 做參數搜尋，同時最小化「拒絕次數」和「跟原模型的 KL 散度」。在 Gemma 3 12B 上它做到 3/100 refusals、KL 只有 0.16，而人工版本是 0.45 與 1.04——**越接近 0，代表跟原模型差越少**。

這也帶出一件重要的事：

> [!WARNING]
> **同樣叫 abliterated，品質差很多。** Youssofal 那版用 Heretic 的兩階段流程；huihui-ai 用的是另一套工具，作者自己在說明裡寫「crude, proof-of-concept」（粗糙的概念驗證）。下載前先看它有沒有公布 KL 散度或 benchmark。

---

## 代價：不是只有拿掉拒絕 — The Real Cost

這是最容易被忽略的一段。**abliteration 不是手術刀，它會誤傷。**

### 一般能力會掉，幅度看實作

Young 等人在 2025 年 12 月系統性比較了各種 abliteration 方法：GSM8K 數學推理的變化從 **+1.51 到 −18.81 個百分點**。同一個技術、不同實作，差距接近 20 分。

### 更麻煩的是「性格」被改了，而且不可預測

2026 年的《Abliteration Is Not a Scalpel》測了決策傾向，同樣的證據餵給原版與 abliterated 版：

| 觀察項目 | Gemma | Qwen |
|---|---|---|
| 押注在「上行結果」的傾向 | **＋12.2 個百分點** | **＋7.4 個百分點** |
| 自我辯解的字數 | ＋4.0 字 | ＋7.4 字 |
| 表達出來的信心 | **−0.008（變低）** | **＋0.109（變高）** |

同樣的介入，**信心卻往相反方向跑**——這代表它跟每個模型的內部表徵互動方式不同，沒辦法一體套用。研究還發現 abliterated 版本**少用不確定性的字眼**、多用「雖然／儘管」這類讓步語氣：嘴上說的不確定，跟實際行為的不確定，脫鉤了。

而指令遵循完全沒掉（JSON 格式正確率 100%）。

> [!CAUTION]
> **這才是危險的地方：能力看起來沒事，判斷傾向卻悄悄偏了。** 特別是接到會自己動手的 agent 迴圈上時——一個「更樂觀、更有信心、更少表達不確定」的模型配上自動執行，錯誤會被放大。

### 責任在你

model card 自己寫得很清楚：安全過濾已顯著降低、不適合未成年人與高安全需求場景、**產出內容的法律與倫理責任由使用者承擔**、建議只在受控環境中做研究測試。

---

## 怎麼裝、怎麼選 — Install & Choose

```bash
# 原版：18 GB，256K context
ollama run qwen3.6:27b

# abliterated 版：17 GB，Q4_K_M，27.8B
ollama run huihui_ai/Qwen3.6-abliterated:27b
```

| 你的情況 | 建議 |
|---|---|
| 一般工作、寫程式、日常 agent | **用原版。** abliterated 在這些場景沒有任何好處，只有代價 |
| 資安研究（需要模型講出攻擊細節） | 可以考慮，但要在隔離環境，而且要知道判斷傾向會偏 |
| 醫療／法律／心理等常被過度拒絕的題材 | **先試原版 ＋ 把用途和情境講清楚的 system prompt**，多數過度拒絕是 prompt 沒交代清楚 |
| 要接進會自動執行的迴圈 | **不要。** 理由見上面的樂觀偏移 |

---

## 常見問題 — FAQ

**Q：abliterated 會變笨嗎？**
看實作。會盯著 KL 散度做的（例如 Heretic）接近原模型；粗糙的實作 GSM8K 可能掉將近 19 分。**下載前先看有沒有公布 KL 或 benchmark**，沒有就當它是未知數。

**Q：為什麼檔案大小幾乎一樣？**
因為參數量沒變，只是權重被改過。它不是另一個模型，是同一個模型動過手術。

**Q：我只是覺得原版太愛拒絕。**
那八成不需要 abliterated。原版的過度拒絕多半靠 system prompt 把用途、身分、情境講清楚就能解決——這是最便宜的一步，先試。

**Q：合法嗎？**
模型本身是 Apache 2.0，下載使用沒問題。但**你產出的內容，責任在你**——model card 自己就是這樣寫的。

---

## 相關筆記 — Related

- [輕量模型推薦](./lightweight-models.md) —— 依 VRAM 分級選型
- [Ollama 指令教學](./ollama-guide.md) —— 安裝與管理
- [AI 生成內容怎麼標記與辨識](../01-fundamentals/ai-content-watermark.md) —— 本地模型不會帶官方浮水印

## Sources

- [Qwen3.6-27B — Hugging Face（官方權重）](https://huggingface.co/Qwen/Qwen3.6-27B)
- [Qwen3.6 — Ollama Library](https://ollama.com/library/qwen3.6)
- [Alibaba Qwen Team Releases Qwen3.6-27B — MarkTechPost](https://www.marktechpost.com/2026/04/22/alibaba-qwen-team-releases-qwen3-6-27b-a-dense-open-weight-model-outperforming-397b-moe-on-agentic-coding-benchmarks/)
- [Huihui-Qwen3.6-27B-abliterated — Hugging Face](https://huggingface.co/huihui-ai/Huihui-Qwen3.6-27B-abliterated)
- [Qwen3.6-27B-Abliterated-Heretic-Uncensored — Hugging Face](https://huggingface.co/Youssofal/Qwen3.6-27B-Abliterated-Heretic-Uncensored-BF16)
- [huihui_ai/Qwen3.6-abliterated:27b — Ollama](https://ollama.com/huihui_ai/Qwen3.6-abliterated:27b)
- [Heretic：全自動移除語言模型審查 — GitHub](https://github.com/p-e-w/heretic)
- [Abliteration Is Not a Scalpel: Off-Target Effects of Refusal Removal — arXiv 2607.17427](https://arxiv.org/abs/2607.17427)
- [Abliteration — Learn AI Wiki（技術源流與 Arditi et al. 2024）](https://ai.miraheze.org/wiki/Abliteration)
