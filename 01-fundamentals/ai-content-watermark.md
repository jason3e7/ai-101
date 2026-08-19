---
title: AI 101 - AI 生成內容怎麼標記與辨識
tags: [ai, 浮水印, watermark, c2pa, 溯源, 辨識, 基礎]
created: 2026-08-19
---

# AI 生成內容怎麼標記與辨識 — How AI Content Is Watermarked and Detected

[← 回主頁](../index.md)

> [!NOTE]
> 從 2026 年起，Claude 會在產出的內容裡**埋看不見的標記**——文字是嵌入式浮水印，檔案是簽章 metadata。這篇講它怎麼運作、你怎麼查、以及**為什麼「查到」和「沒查到」都不能當定論**。

> **TL;DR (EN):** Anthropic marks Claude output two ways: invisible watermarks woven into the text itself (survives copy-paste), and C2PA-signed metadata on files (.png/.jpg/.svg). Neither is proof: a hit only means Claude touched it (maybe just proofreading), and a miss doesn't mean human-written (old model, heavy editing, too short, stripped metadata). OpenAI built a watermark but never shipped it — 30% of users said they'd quit if it did.

---

## Claude 怎麼標記 — How Claude Marks Content

Anthropic 用兩種互補技術：

| 對象 | 技術 | 特性 |
|---|---|---|
| **文字** | **嵌入式浮水印**——把難以察覺的標記織進文字本身 | 不影響品質與可讀性；**複製貼上會跟著跑** |
| **檔案**（.png / .jpg / .svg…） | **簽章 metadata**，遵循 **C2PA** 開放標準（內容來源與真實性聯盟） | 記錄內容來源；但 metadata 可能被平台剝除 |

**適用範圍**：2026-08-02 之後推出的新模型從第一天就支援（舊版本正在補）；涵蓋 API、Claude 網頁版、Claude Code、Claude Cowork，以及 AWS / Google Cloud / Microsoft 等雲端夥伴；全球適用。

**怎麼查**：Anthropic 表示正在支援使用者與第三方偵測這些標記，細節會在後續技術文件公布。

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

### 人工判斷的老方法（也只是線索）

在浮水印之外，大家常用的特徵：**表情符號用得兇、奇怪的破折號、公式化語氣**（像「我想用最不繞彎、最直接、最能夠接住你的方式」這種）。這些同樣不可靠——寫作風格會互相模仿，人也會這樣寫。

---

## 為什麼 OpenAI 沒做 — Why OpenAI Didn't Ship

OpenAI 內部其實準備過浮水印方案，但**調查顯示 30% 用戶表示「加浮水印就不用 ChatGPT 了」**，所以至今沒上線。

這點出整件事的張力：**溯源透明**（社會想知道內容從哪來）vs **使用者接受度**（沒人想被標記）。Anthropic 選了前者，OpenAI 選了後者。

---

## 相關筆記 — Related

- [先驗證，再用它突破自己](../05-notes/ai-verify-then-expand.md) —— 浮水印是「來源線索」，判斷真偽仍要靠獨立驗證
- [PII Masking（隱私遮蔽）](../03-tools/pii-masking.md) —— 另一種在內容上動手腳的技術，方向相反（隱藏而非標記）

## Sources

- [Claude 如何標記 AI 生成的內容 — Anthropic 官方說明](https://support.claude.com/zh-TW/articles/16266773-claude-%E5%A6%82%E4%BD%95%E6%A8%99%E8%A8%98-ai-%E7%94%9F%E6%88%90%E7%9A%84%E5%85%A7%E5%AE%B9)
- [你想知道網路上的文章是不是 AI 產生的嗎？ — Wisely Chen（LinkedIn）](https://www.linkedin.com/posts/wisely-chen_%E4%BD%A0%E6%83%B3%E7%9F%A5%E9%81%93%E7%B6%B2%E8%B7%AF%E4%B8%8A%E7%9A%84%E6%96%87%E7%AB%A0%E6%98%AF%E4%B8%8D%E6%98%AF-ai-%E7%94%A2%E7%94%9F%E7%9A%84%E5%97%8E-%E9%99%A4%E4%BA%86%E5%8E%BB%E6%89%BE%E8%A1%A8%E6%83%85%E7%AC%A6%E8%99%9F%E5%A5%87%E6%80%AA%E7%9A%84%E7%A0%B4%E6%8A%98%E8%99%9F%E4%BB%A5%E5%A4%96-share-7493094207901859841-PWpZ/)
