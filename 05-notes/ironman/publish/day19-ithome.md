title : [Day 19] 這段字是 AI 寫的嗎? 浮水印怎麼運作、為什麼不能當證據


## 為什麼講這個 — Why This Matters

[Day 16](https://ithelp.ithome.com.tw/articles/10419414) 到 [Day 18](https://ithelp.ithome.com.tw/articles/10420151) 做的是**自己疊的粗偵測** (共現訊號 — `——`、emoji、粗體、…). 這些訊號有價值 (排序用), 但明顯有盲區 (單一指紋被 upstream 壓就失效, 翻面問題「純手寫」更難回答).

那**官方正式做的呢?** 2026-08-02 起, Claude 的產出開始**埋看不見的標記**: 文字是嵌入式浮水印 (複製貼上會跟著跑), 檔案是 C2PA 簽章 metadata. OpenAI 的影像從 2026-05 也全面加 SynthID + C2PA. 這些不是我自己疊的指標, 是模型廠商官方 ship 的技術.

**然後呢?** 這篇講三件事:

1. Claude 跟 OpenAI 現在怎麼標 (文字 + 檔案兩條路)
2. 為什麼**查到跟沒查到都不能當判決** — 包括一個驚人數字: 單次 paraphrase 就能移除 **98%** 浮水印訊號
3. 誰在推這件事 — 不是廠商自願, 是監管 (EU AI Act Article 50) 逼的

結論跟 Day 16-18 一樣: **是線索, 不是鐵證**. 但這個結論對官方浮水印更重要, 因為它天生帶權威光環, 讓人以為「官方的驗證結果就是真相」. 不是.

---

## Claude 怎麼標記 — How Claude Marks Content

Anthropic 用兩種互補技術 (2026-08-02 起):

| 對象 | 技術 | 特性 |
|:---|:---|:---|
| **文字** | **嵌入式浮水印** — 把難以察覺的標記織進文字本身 | 不影響品質與可讀性; **複製貼上會跟著跑** |
| **檔案** (.png / .jpg / .svg…) | **簽章 metadata**, 遵循 **C2PA** 開放標準 (內容來源與真實性聯盟) | 記錄內容來源; 但 metadata 可能被平台剝除 |

適用範圍: 2026-08-02 之後推出的新模型從第一天就支援, 涵蓋 API / Claude 網頁版 / Claude Code / Claude Cowork / 各雲端夥伴. 全球適用.

**怎麼查**:

- **檔案 C2PA**: Anthropic 的 [claude.com/check-content](https://claude.com/check-content) 免費檢查器, 拖曳 .png / .jpg / .svg 進去檢視 manifest
- **文字浮水印**: 偵測 API 目前是 **private preview**, 只開放給監管機關、執法、媒體、fact-checker、研究者、公民團體等合格對象. 一般使用者還沒得用

### 文字浮水印的三條技術路線

([Wisely Chen 的拆解](https://www.linkedin.com/posts/wisely-chen_activity-7493094209105625088-rA-z) 整理, 外部推測, 非官方細節):

1. **零寬度/特殊空白字元**: 字間插入人眼看不到的字元, 例如 `U+00A0` (不換行空白)、`U+2009` (窄空白)
2. **同形字替換**: 換成長得一樣但 Unicode 碼位不同的字, 例如拉丁 `a` ↔ 西里爾 `а` — 跟釣魚網址 `pаypal.com` 騙人同一手法
3. **統計指紋**: 微調用詞的機率分布形成可偵測的模式, 例如刻意偏好「所以」而不是「因此」

前兩種是**字元層級** (工具檢查 Unicode 碼位揪得出來), 第三種是**統計層級** (要大量文本 + 演算法才驗得出). **改寫會讓浮水印消失** — 下一節是重點.

---

## 兩個都不能當證據 — Neither Direction Is Proof

這是全篇最重要的一段, Anthropic 官方自己也特別強調:

> **⚠️ 查到標記 ≠ 這是 AI 寫的**: 只代表內容「可能經 Claude 處理過」 — 可能只是拿去校對、翻譯、改格式, 原作者仍是人.
>
> **⚠️ 沒查到標記 ≠ 這是人寫的**: 可能是舊模型產出、被大幅編輯過、文字太短、metadata 被平台剝除, 或用了不支援標記的工具.

### 「沒查到」有多脆弱: 98% paraphrase 移除

2026 一份研究 ([arXiv:2508.20228](https://arxiv.org/abs/2508.20228)) 在 Google SynthID-Text 上實測:

- **單次 paraphrase 就能移除約 98% 的浮水印訊號**
- back-translation (英→中→英) 跟 copy-paste 進其他 AI 潤稿也會嚴重弱化

換句話說, 只要作者「用 AI 寫完**再用另一個 AI 改寫一次**」就能大幅逃過偵測. 這不是 SynthID 特有的缺陷, 統計指紋類的浮水印**都受這種攻擊影響**. Anthropic 的技術細節未公開, 但一般假設面對 paraphrase 攻擊也不會樂觀太多.

所以浮水印是**線索, 不是判決**. 拿「沒查到」當「這是人寫的」會冤枉人, 拿「查到」當「這是 AI 代寫」也會冤枉人.

---

## 誰在推這件事 — Why Everyone Moved in 2026

不是廠商自願, 是監管逼的.

**驅動來源: EU AI Act Article 50 於 2026-08-02 生效** ([歐盟官方公告](https://digital-strategy.ec.europa.eu/en/news/commission-starts-enforcing-ai-act-rules-and-new-transparency-requirements-2-august)). 這條要求:

1. AI 生成或大幅改動的內容要有**機器可讀的標記**
2. Deepfake 要**明顯揭露**
3. 與 AI 系統互動要**告知使用者** (chatbot 揭露)

所以 2026 一堆廠商同步動起來:

- **Anthropic** 從 2026-08-02 開始標記 (文字 + 檔案)
- **OpenAI** 的立場轉變 — **文字至今沒上線** (調查顯示 30% 用戶表示「加浮水印就不用 ChatGPT 了」), 但**影像 2026-05 轉向**, 全面加 SynthID + C2PA
- **Google SynthID** 快速擴張
- **TikTok** 升格 C2PA Steering Committee (宣稱已標超過 30 億支影片)
- **中國** 2025-09-01 上路類似的《AI 生成合成內容標識辦法》, 2026 初開始執法

**張力**: 溯源透明 vs 使用者接受度. 在影像領域監管壓力較大 (deepfake 政治風險高)、使用者反彈相對小, 文字則相反. 未來看到 AI 廠商加浮水印, 先問「是不是被監管逼的」, 答案通常是「對」.

---

## 我的重點 — Takeaways

- 浮水印是**線索**, 不是**判決**. 查到不等於 AI 寫, 沒查到不等於人寫
- 「沒查到」特別脆弱: 單次 paraphrase 移除 98% 訊號 (SynthID 實測, Claude 細節未公開但假設差不多)
- 跟 [Day 16](https://ithelp.ithome.com.tw/articles/10419414)-[Day 18](https://ithelp.ithome.com.tw/articles/10420151) 的**共現訊號** (`——`、emoji、粗體、…) 本質一樣: 都是證據, 都不是鐵證. 把兩者疊起來也不會變鐵證, 只會提高「可能性」
- 廠商 2026 動起來不是良心發現, 是 EU AI Act Article 50 (2026-08-02 生效) 逼的. 中國 2025-09 類似規範也上路
- **實務建議**: 看到別人拿「AI 偵測結果」當判決指控人, 把這篇數據丟給他

---

## Sources

- [Claude 如何標記 AI 生成的內容 — Anthropic 官方說明](https://support.claude.com/zh-TW/articles/16266773-claude-%E5%A6%82%E4%BD%95%E6%A8%99%E8%A8%98-ai-%E7%94%9F%E6%88%90%E7%9A%84%E5%85%A7%E5%AE%B9)
- [claude.com/check-content — Anthropic 的檔案 C2PA 檢查器](https://claude.com/check-content)
- [Commission starts enforcing AI Act rules — 歐盟官方公告 (2026-08-02)](https://digital-strategy.ec.europa.eu/en/news/commission-starts-enforcing-ai-act-rules-and-new-transparency-requirements-2-august)
- [OpenAI joins C2PA, adds SynthID to ChatGPT images — The Next Web (2026-05)](https://thenextweb.com/news/openai-c2pa-synthid-ai-image-detection-watermark)
- [SynthID 官方文件 — Google DeepMind](https://ai.google.dev/responsible/docs/safeguards/synthid)
- [Watermarking LLMs: paraphrase / back-translation attacks (arXiv:2508.20228, 2026)](https://arxiv.org/abs/2508.20228)
- [你想知道網路上的文章是不是 AI 產生的嗎? — Wisely Chen (LinkedIn)](https://www.linkedin.com/posts/wisely-chen_%E4%BD%A0%E6%83%B3%E7%9F%A5%E9%81%93%E7%B6%B2%E8%B7%AF%E4%B8%8A%E7%9A%84%E6%96%87%E7%AB%A0%E6%98%AF%E4%B8%8D%E6%98%AF-ai-%E7%94%A2%E7%94%9F%E7%9A%84%E5%97%8E-%E9%99%A4%E4%BA%86%E5%8E%BB%E6%89%BE%E8%A1%A8%E6%83%85%E7%AC%A6%E8%99%9F%E5%A5%87%E6%80%AA%E7%9A%84%E7%A0%B4%E6%8A%98%E8%99%9F%E4%BB%A5%E5%A4%96-share-7493094207901859841-PWpZ/)
