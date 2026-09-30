---
title: AI 101 - AI 生成內容怎麼標記與辨識
tags: [ai, 浮水印, watermark, c2pa, 溯源, 辨識, 基礎]
created: 2026-08-19
updated: 2026-09-30
---

# AI 生成內容怎麼標記與辨識 — How AI Content Is Watermarked and Detected

[← 回主頁](../index.md)

> [!NOTE]
> 從 2026 年起，Claude 會在產出的內容裡**埋看不見的標記**——文字是嵌入式浮水印，檔案是簽章 metadata。這篇講它怎麼運作、你怎麼查、以及**為什麼「查到」和「沒查到」都不能當定論**。

> **TL;DR (EN):** Anthropic marks Claude output two ways: invisible watermarks woven into the text itself (survives copy-paste), and C2PA-signed metadata on files (.png/.jpg/.svg). Neither is proof: a hit only means Claude touched it (maybe just proofreading), and a miss doesn't mean human-written (old model, heavy editing, too short, stripped metadata). OpenAI wouldn't ship text watermarks (30% would-quit rate) but reversed on images — as of May 2026 all ChatGPT images carry SynthID + C2PA. The EU AI Act Article 50 (in force 2026-08-02) is the reason every vendor moved at once.

---

## Claude 怎麼標記 — How Claude Marks Content

Anthropic 用兩種互補技術：

| 對象 | 技術 | 特性 |
|---|---|---|
| **文字** | **嵌入式浮水印**——把難以察覺的標記織進文字本身 | 不影響品質與可讀性；**複製貼上會跟著跑** |
| **檔案**（.png / .jpg / .svg…） | **簽章 metadata**，遵循 **C2PA** 開放標準（內容來源與真實性聯盟） | 記錄內容來源；但 metadata 可能被平台剝除 |

**適用範圍**：2026-08-02 之後推出的新模型從第一天就支援（舊版本正在補）；涵蓋 API、Claude 網頁版、Claude Code、Claude Cowork，以及 AWS / Google Cloud / Microsoft 等雲端夥伴；全球適用。

**怎麼查（2026-09 現況）**：

- **檔案 C2PA**：Anthropic 推出 [claude.com/check-content](https://claude.com/check-content) 免費檢查器，任何人可以拖曳 .png / .jpg / .svg 檢視 manifest。
- **文字浮水印**：偵測 API 目前是 **private preview**，只開放給歐盟法規對應的合格對象（監管機關、執法、媒體、fact-checker、研究者、公民團體）與有合規義務的企業。一般使用者還沒得用。

---

## 文字浮水印的技術路線 — How Text Watermarking Works

[Wisely Chen 的拆解](https://www.linkedin.com/posts/wisely-chen_activity-7493094209105625088-rA-z) 整理出三條可能路線（這是外部推測與分析，非官方細節）：

1. **零寬度／特殊空白字元**：在字間插入人眼看不到的字元，例如 `U+00A0`（不換行空白）、`U+2009`（窄空白）
2. **同形字替換**：換成長得一樣、但 Unicode 碼位不同的字，例如拉丁字母 `a` ↔ 西里爾字母 `а`——這也是釣魚網址 `pаypal.com` 騙人的老手法
3. **統計指紋**：微調用詞的機率分布形成可偵測的模式，例如刻意偏好用「所以」而不是「因此」

> [!TIP]
> 前兩種是**字元層級**（可以用工具檢查 Unicode 碼位揪出來），第三種是**統計層級**（要大量文本 + 演算法才驗得出）。**改寫會讓浮水印消失**——重新用自己的話寫過，標記就沒了。

---

## 兩個都不能當定論 — Neither Direction Is Proof

這是全篇最重要的一段，官方自己也特別強調：

> [!WARNING]
> **查到標記 ≠ 這是 AI 寫的。** 只代表內容「可能經 Claude 處理過」——可能只是拿去**校對、翻譯、改格式**，原作者仍是人。
>
> **沒查到標記 ≠ 這是人寫的。** 可能是舊模型產出、被大幅編輯過、文字太短、metadata 被平台剝除，或用了不支援標記的工具。

所以浮水印是**線索，不是判決**。拿它當「抓 AI 代寫」的鐵證會冤枉人。

**「沒查到」的量化脆弱性**：2026 一份研究（[arXiv 2508.20228](https://arxiv.org/abs/2508.20228)）在 Google SynthID-Text 上實測，**單次 paraphrase 就能移除約 98% 的浮水印訊號**，back-translation（英→中→英）與 copy-paste 進其他 AI 潤稿也會嚴重弱化。換句話說，只要作者「用 AI 寫完再用另一個 AI 改寫一次」就能大幅逃過偵測——這不是 SynthID 特有的缺陷，統計指紋類的浮水印都受這種攻擊影響。Anthropic 的文字浮水印技術細節未公開，但一般假設面對 paraphrase 攻擊也不會樂觀太多。

### 人工判斷的老方法（也只是線索）

在浮水印之外，大家常用的特徵：**表情符號用得兇、奇怪的破折號、公式化語氣**（像「我想用最不繞彎、最直接、最能夠接住你的方式」這種）。這些同樣不可靠——寫作風格會互相模仿，人也會這樣寫。

---

## OpenAI 的立場轉變 — OpenAI's Reversal

原本 OpenAI 準備過**文字**浮水印方案，但**調查顯示 30% 用戶表示「加浮水印就不用 ChatGPT 了」**，所以文字部分至今沒上線。

但**影像部分 2026-05 轉向**：OpenAI 加入 C2PA Steering Committee，ChatGPT 與 API 產生的所有影像都嵌入 Google **SynthID 隱形浮水印 + C2PA metadata**，並預告要公開影像驗證工具（[TNW 報導](https://thenextweb.com/news/openai-c2pa-synthid-ai-image-detection-watermark)）。這代表原本「文字 vs 影像」不做的立場，被拆成：

- **文字**：still no watermark（30% 用戶顧慮仍在）
- **影像**：全面加標（C2PA + SynthID）

這反映的張力：**溯源透明** vs **使用者接受度**，在影像領域監管壓力較大（deepfake 政治風險高），使用者反彈相對小；文字則相反。

## 為什麼廠商 2026 忽然都動起來 — Why Everyone Moved in 2026

**驅動來源: EU AI Act Article 50 於 2026-08-02 生效**（[歐盟官方公告](https://digital-strategy.ec.europa.eu/en/news/commission-starts-enforcing-ai-act-rules-and-new-transparency-requirements-2-august)）。這條要求:

1. AI 生成或大幅改動的內容要有**機器可讀的標記**
2. Deepfake 要**明顯揭露**
3. 與 AI 系統互動要**告知使用者**（chatbot 揭露）

Anthropic 從 2026-08-02 開始標記、OpenAI 影像轉向、Google SynthID 快速擴張、TikTok 升格 C2PA Steering Committee（宣稱已標超過 30 億支影片）——都是這條在推。中國 2025-09-01 也上路類似的《AI 生成合成內容標識辦法》，2026 初開始執法。所以未來看到 AI 廠商加浮水印，先問「是不是被監管逼的」，答案通常是「對」。

---

## 相關筆記 — Related

- [先驗證，再用它突破自己](../05-notes/essays/ai-verify-then-expand.md) —— 浮水印是「來源線索」，判斷真偽仍要靠獨立驗證
- [PII Masking（隱私遮蔽）](../03-tools/privacy/pii-masking.md) —— 另一種在內容上動手腳的技術，方向相反（隱藏而非標記）

## Sources

- [Claude 如何標記 AI 生成的內容 — Anthropic 官方說明](https://support.claude.com/zh-TW/articles/16266773-claude-%E5%A6%82%E4%BD%95%E6%A8%99%E8%A8%98-ai-%E7%94%9F%E6%88%90%E7%9A%84%E5%85%A7%E5%AE%B9)
- [claude.com/check-content — Anthropic 的檔案 C2PA 檢查器](https://claude.com/check-content)
- [Commission starts enforcing AI Act rules — 歐盟官方公告 (2026-08-02)](https://digital-strategy.ec.europa.eu/en/news/commission-starts-enforcing-ai-act-rules-and-new-transparency-requirements-2-august)
- [OpenAI joins C2PA, adds SynthID to ChatGPT images — The Next Web (2026-05)](https://thenextweb.com/news/openai-c2pa-synthid-ai-image-detection-watermark)
- [SynthID 官方文件 — Google DeepMind](https://ai.google.dev/responsible/docs/safeguards/synthid)
- [Watermarking LLMs: paraphrase / back-translation attacks (arXiv 2508.20228, 2026)](https://arxiv.org/abs/2508.20228)
- [你想知道網路上的文章是不是 AI 產生的嗎？ — Wisely Chen（LinkedIn）](https://www.linkedin.com/posts/wisely-chen_%E4%BD%A0%E6%83%B3%E7%9F%A5%E9%81%93%E7%B6%B2%E8%B7%AF%E4%B8%8A%E7%9A%84%E6%96%87%E7%AB%A0%E6%98%AF%E4%B8%8D%E6%98%AF-ai-%E7%94%A2%E7%94%9F%E7%9A%84%E5%97%8E-%E9%99%A4%E4%BA%86%E5%8E%BB%E6%89%BE%E8%A1%A8%E6%83%85%E7%AC%A6%E8%99%9F%E5%A5%87%E6%80%AA%E7%9A%84%E7%A0%B4%E6%8A%98%E8%99%9F%E4%BB%A5%E5%A4%96-share-7493094207901859841-PWpZ/)
