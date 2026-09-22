---
title: "AI 101 - 鐵人賽 Claude AI 組競爭觀察: 40 系列標題盤點與差異化建議"
tags: [ai, 鐵人賽, ironman, 競品分析, 個人筆記, 內部觀察]
created: 2026-09-22
---

# Claude AI 組競爭觀察 — 40 系列標題盤點與差異化建議

[← 回主頁](../../index.md)｜[參賽規劃](./plan.md)｜[三十篇標題](./titles.md)

> [!NOTE]
> 2026-09-22 上午抓的快照, playwright 掃過 [ironman Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai) 六頁, 拿到 40 個獨立系列的標題. 目的: 看清楚**同組在講什麼、哪些題目已經一堆人做、哪些空白區可以吃**, 為 Day 09 到 Day 30 的選題定位.

---

## 概況數字 — At a Glance

- **總系列數**: 40 (Claude AI 組公開系列)
- **今天是 Day 08** (2026-09-15 開賽), 大多數人已發 7 到 8 篇
- **有多少人已發完 30 篇**: 8 個系列預先寫好全部 (30 篇滿). 其中 3 個是進去看是「日更中」的排版, 但列表已顯示 30 條
- **平均已發**: 14.6 篇 (含預發完的那些); 中位數約 8 篇 (每天真的日更的)
- **jason3e7 (我) 位置**: 進度 7 篇, 已發 Day 01 到 Day 07

---

## 六大題材分類 — Six Themes

按主軸歸類. 標「⭐」是特別突出、值得追讀的.

### A. 打造具體專案 — Build a Thing (16 系列, 最擁擠)

大宗題材. 幾乎每個人都在用 Claude Code 打造某個東西, 天天記錄過程. 差別在做什麼專案.

| 系列 | 專案 |
|:---|:---|
| 用 AI Agent 重構 legacy PHP 系統 ⭐ | 真實踩坑案例最多, 講到 CLAUDE.md/Skills/多 agent |
| 打造 AI Agent 驅動的第二大腦 (Go + Obsidian) | 30 天全發完, 完整規劃 |
| 奇幻塔防開發實錄 | 遊戲開發, 有敘事分支設計 |
| 用 Claude 帶我 Bring Up AI 開發板 | 硬體 bring up, 罕見 |
| 從零打造圖書管理系統 (WSL2 × MySQL) | 新手向 |
| 一個人的機房 (AI 機房) | 30 篇全發完, 端到端 |
| 從零 Claude Code 打造 AI 任務管理 Web App | 標準教學向 |
| 研究生自救指南 (論文工具箱) ⭐ | 有踩坑、有算帳、扎實 |
| 用 Claude 打造 AI 校園學習助理 | 新手向 |
| 30 天建立 Claude Code 地城領主 | 桌遊 DM, 擬人化寫法特別 |
| 讓 Claude Code 當我的 K8s 助教 | 專業技術, 前 7 篇偏教學 |
| 文科生的 Claude Code 30 日實戰 (台股研究工作台) | 走「文科生」定位 |
| 從 AI 助理到營運中台 (金融 PM Claude Code 治理) | 治理視角, 少見 |
| 零基礎當產品長 | PM 視角 |
| Side Project 建置 (看展/杯測/llm-wiki) | 多個小專案並行 |
| 圖書管理／證照 App 開源 | 30 篇全發 |

### B. 心法／協作紀律／方法論 — Craft & Discipline (8 系列, 我在這裡)

用 Claude 的**方式**、什麼可信、驗證怎麼做. 這一區跟我最貼近.

| 系列 | 主軸 |
|:---|:---|
| **AI 心法三十天** (jason3e7, 我的) | 從 LLM 原理推所有心法 |
| 跟 Claude Code 協作的摩擦 ⭐ | 摩擦=沒講清楚的規則, 前提條件寫法 |
| 盡信 Claude 不如無 Code | code review 為主, 心法 + 實戰 |
| AI coding 沒有新問題 ⭐ | 標題全反直覺, 引 Anthropic 觀察 |
| 資深工程師的 Claude Code 工作筆記 | 「AI 說做完了我通常不信」 |
| 你怎麼知道 AI 做對了 ⭐ | 驗證思維, 從終端機講起 |
| AI 負責答, 我負責讓答案可信 (公部門) ⭐ | 有真實 build 設定搞出 46 萬檔案案例 |
| 買了 Claude Code, 然後呢? | 自我懷疑式敘事 |

### C. Skills 專題 — Agent Skills (3 系列)

- 今晚來點 Claude Skills (產品開發者)
- 方法圖鑑 × Claude Skills (200 年發現方法, 角度最新) ⭐
- 把 Claude 練成專家 (可驗證的 Agent Skills)

### D. 教育／新手指南／Claude 產品介紹 (4 系列)

- 跟著 Claude Academy (Chat / Cowork / Projects / Skills 全講)
- 跟 Claude Code CLI 變成好朋友 (30 篇全發)
- 大學教師心理計量教材 (Claude 當學習輔助)
- Claude AI 新手筆記 (學生日記)

### E. 系統／架構理論 — LLM/Agent Architecture (2 系列)

- 用 AI Agent 撰寫長篇技術系列文章 (meta, 用 agent 寫這個系列)
- 從 LLM 到 Agent 拆解每層 ⭐ (跟我的定位最像)

### F. 硬體／地端／推論 — Hardware & Inference (1 系列)

- **128GB 統一記憶體 DGX Spark** ⭐ (作者 ivanusto; 跟 06-external 收的 kerr 是**不同人**, 兩人都寫 DGX Spark 是巧合)

### G. 測試 QA (2 系列, 同一人)

- 探索式測試 (30 篇全發, 內容深)
- Playwright UI 自動化 (30 篇全發)
- Agentic SDET Playwright (另一人, 也是 QA)

### H. Backend／雲端 (1 系列)

- **Backend 工程師的 Azure GenAI 實戰** ⭐ (30 篇全發, 每篇標題都很精, 深)

### I. 資安 (1 系列)

- 打靶機 30 天 (與 Claude 關聯度低)

### J. Agentic Workflow (1 系列)

- 如何讓 AI 主動完成複雜任務 (前 7 篇偏概念介紹)

---

## 差異化位置 — Where I Sit

跟我最貼近的 7 個系列 (B 類):

- **摩擦-規則** 系列講「協作前提條件的寫法」, 我講「原理推心法」, 切點不同但有重疊
- **AI coding 沒有新問題** 標題風格跟我很像 (反直覺 + 具體數字), 但是他抓 Anthropic blog 觀察, 我從 LLM 原理長出來
- **你怎麼知道 AI 做對了** 講驗證, 我第三天就講過 (Day 03), 我更早但沒展開
- **AI 負責答, 我負責讓答案可信** 案例扎實, 但視角偏公部門合規

**我的獨特之處**:

1. **從 LLM 原理出發** — 40 系列裡只有「從 LLM 到 Agent」跟我有點像, 但他偏架構層, 我偏使用者心法層
2. **每天一個小突破** — Day 04「兩軸能力全景圖」、Day 05「6 層裡只有前 2 層選能力」、Day 06「Prompt 只是骨架四件事之一」、Day 07「prompt 只是 5%」, 都是**單一 memorable insight**
3. **半形風格＋短句** — 沒看到別人這樣寫; 大多數用全形、鋪陳長
4. **有 4 場自己跑的 field test** — 大部分人的失敗都藏在成功包裝下

**我的重疊區 (要小心不要撞題)**:
- 別再寫「什麼是 Claude Code」「怎麼安裝」「怎麼寫 CLAUDE.md」— 這些 D 類有一堆人寫
- 別再重複「你怎麼知道 AI 做對了」的具體驗證做法, 已經有專篇
- 別做「打造具體專案」的 build-a-thing 序列, 那是 A 類的主場

---

## 標題風格觀察 — Title Craft

用心讀了 40 系列的 Day 01-08 標題, 抽出四種吸睛套路 (哪個作者用最多):

### 一、反直覺／打臉常識
- 「不是它突然變強, 是它跨過了你的門檻」(jason3e7)
- 「AI 說『做完了』, 我通常不信」(資深工程師)
- 「你的自動化測試, 大部分是在演戲」(探索式測試)
- 「餵給 AI 的 Context, 有 70% 是雜訊」(AI coding 沒有新問題)
- 「Anthropic 砍掉 80% 的 AI 指令, 效果沒變差」(同上)

**這是最強的鉤子**, 有具體數字更強. 我 Day 02、03、06、07 都用了.

### 二、質疑式標題
- 「買了 Claude Code, 然後呢?」
- 「你怎麼知道 AI 做對了」
- 「這件事, 真的需要 AI 嗎?」(買了 Claude Code)
- 「AI 寫程式很快, 但為什麼我還是不敢 Approve」(同上)

引反問, 讀者要看答案. 我用得少, 可以更多.

### 三、隱喻／擬人化
- 「新進同事」(Agentic SDET)
- 「地城領主」/「梅拉不見了」(Claude Code 地城領主)
- 「一個人的機房」/「同事」(AI 機房)
- 「LLM 是你接過最不守規矩的下游依賴」(Backend Azure GenAI)

我系列風格偏原理, 隱喻可以用但不濫用.

### 四、具體數字／量化
- 「9 個區塊」「4 大核心思維」「五個反直覺教訓」(金融 PM)
- 「4-bit 兩個字, 古今多少事」(DGX Spark)
- 「一句話生出會跑的 Playwright 測試」(UI 自動化)

**具體數字比抽象形容詞好記憶 3 倍**. 我 Day 05 (6 層)、Day 06 (58 種、5 個模型)、Day 07 (5%) 有用, 繼續.

### 五、我看到的兩個獨特寫法, 可以偷

- **「先 X, 才 Y」條件式** — 「先把工具搞清楚 —— Claude 生態的名詞地圖」、「先寫規格, 再讓 Plan Mode 補完整」. 建立**順序感**
- **「不是 A, 是 B」對比** — 我常用, 但可以更精; 例:「不是叫 AI 寫教科書」(心理計量)

---

## 給我 Day 09-30 的建議 — Suggestions

現在剩 22 天. 基於競爭態勢, 我認為要做的 4 件事:

### 1. 維持「原理／機制」優勢 (第一優先)

40 系列裡幾乎沒人從 LLM 統計原理出發推心法. **這是我最強的差異化**, 不要跑掉. Day 08 Harness Engineering、Day 09 Loop Engineering 都繼續走「拆一層講一層」的節奏.

### 2. 提早推出 4 場 field test

大部分系列缺**實測數據**. 我原本規劃在 Day 22 到 Day 24 集中放, 建議提前散到 Day 10 到 Day 15 之間插播. 一場 field test 的說服力抵得上三篇心法. 現在同組已經有 ivanusto 的 DGX Spark 實測撐場, 但他做的是硬體, 我做的是**應用層行為實測** (例: 「請仔細思考」實測有沒有效), 不衝突.

### 3. 一週熱點速覽系列 (可選)

同組沒人做「AI 業界新聞分析」. 我 06-external 已經在收 kerr 的筆記、H200 貼文、Anthropic 官方blog. 可以每週一篇「本週 AI 值得看的三則」+ jason3e7 觀察, 差異化很強. 但成本高, 只做一到兩篇即可.

### 4. 多工具比較 (完全空白區)

40 系列**全部**都是 Claude 為主, 沒有一個做 Claude vs GPT-5 vs Gemini vs Codex 的實測比較. 我可以做一篇: 同一個真實任務丟 4 個工具跑一次, 記錄差異. 這一篇會非常突出.

### 5. 避開的紅海

- 「什麼是 Claude Code」「怎麼安裝」→ 至少 5 個系列在寫
- 「怎麼寫 CLAUDE.md」→ 幾乎每個系列都會提, 別做專篇
- 「Skills 教學」→ 有 3 個專題系列, 我最多提一次
- 「Sub-agent / MCP 名詞介紹」→ 教學系列已包
- 「打造 X Web App」→ 我不做 build-a-thing

### 具體題目建議 (順序可調)

**已定**: Day 08 Harness、Day 09 Loop Engineering (原 Day 07 挪來).

**建議插播 (Day 10 到 Day 15)**:

- Day 10: **同一句「請仔細思考」在 5 個場景各是什麼命運** (field test, 承接 Day 06)
- Day 11: **AI 講的跟它實際做的差多少** (Claude thinking 摘要 vs 原始) — 我有 how-claude-shows-thinking 那篇 note 可以套
- Day 12: **一個 CLAUDE.md 對照組實驗**: 有 vs 無, 20 個任務對比 (field test)

**Day 13 到 Day 20 (中段)**:
- 補 Context Engineering 進階
- 六種能力執行手冊
- 模型費用比較 (2026 版)
- AI 文風辨識 (我有 note)
- AI 生成內容辨識 (我有 note)

**Day 21 到 Day 27 (實用工具)**:
- 一週熱點速覽 (1 到 2 篇)
- **4 個工具比較實測** (jason3e7 差異化必殺)
- 本地 LLM (Ollama, 輕量模型)
- Subagent 計費與策略
- AI 取代什麼 / 不取代什麼

**Day 28 到 Day 30 (收線)**:
- 30 天總結
- 給讀者的 checklist / 心法對照表
- 下一步: Loop → Multi-agent → 自主系統

---

## 三個「該追讀」的競爭對手 — Follow These

以我的角度, 三個系列值得每週回去看:

1. **AI coding 沒有新問題** (資深觀察, 標題全反直覺)
2. **Backend 工程師的 Azure GenAI 實戰** (最專業的一位, 每篇標題都有殺氣; 30 篇全發)
3. **ivanusto - 128GB DGX Spark** (30 篇全發, 實測硬派; 跟 06-external 的 kerr 是不同人)

其他 3 個候選: 摩擦-規則系列、AI 負責答我負責可信 (公部門)、方法圖鑑 × Skills.

---

## Sources

- 資料來源: [2026 iThome 鐵人賽 Claude AI 組](https://ithelp.ithome.com.tw/2026ironman/claude-ai) (2026-09-22 上午抓)
- 40 系列的完整標題清單: [competitor-titles.md](./competitor-titles.md) (只收標題與原文 URL, 不轉載內文)
