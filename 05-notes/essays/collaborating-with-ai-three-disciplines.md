---
title: 跟 AI 協作的三個紀律：切小、先試、定標準
tags: [ai, 個人筆記, 觀點, 協作方法, task-decomposition, spike, definition-of-done, 驗證]
created: 2026-11-15
updated: 2026-11-15
---

# 跟 AI 協作的三個紀律 — Three Disciplines for Working With AI

[← 回主頁](../../index.md)

> [!NOTE]
> 這是 jason3e7 的方法論筆記。跟 AI 做「大型、複雜一點」的任務, 其實**跟一般工作很像**——不需要發明新方法, 真正管用的就三件事: **把任務切小、先做可行性小測試、把流程和標準講明確。** 這三件事在軟體與專案管理裡各有成熟的名字 (task decomposition / spike / definition of done), 本文把它們接起來, 並說明為什麼換成跟 AI 協作後, 它們從「加分項」變成「必需品」。

> **TL;DR (EN):** Collaborating with AI on large, complex tasks isn't a new discipline — it's ordinary good work practice. Three things carry most of the weight: **decompose the task into pieces small enough to get right, de-risk with a small feasibility spike before committing, and state the process and done-criteria explicitly.** Each maps to an established engineering practice (task decomposition, Kent Beck's XP "spike", definition of done / acceptance criteria). AI doesn't let you skip them — its failure modes (jagged frontier, "looks-done ≠ done", limited context, over-trust) make the cost of skipping them bigger and faster, turning three "nice-to-haves" into necessities.

---

## 核心論點 — The Thesis

跟 AI 協作最容易踩的坑, 是把它當成「許願機」: 丟一個又大又模糊的任務, 期待一次給出完美結果。但**大型複雜任務跟 AI 協作, 本質上跟帶一個能力強、但會踩鋸齒前沿的新同事做事一樣**——你會用的那套帶人、帶專案的方法, 原封不動就適用。

jason3e7 的三個紀律:

1. **切小任務** — 把大任務拆成「小到能一次做對」的塊。
2. **可行性小測試** — 動手做大之前, 先用一個小實驗證明這條路走得通。
3. **明確的流程和標準** — 事前講清楚「怎麼做」跟「做到什麼程度算完成」。

這三點都不是 AI 時代發明的, 但 AI 讓它們**從 best practice 變成底線**。下面一個一個講, 最後說為什麼。

## 三個紀律 — The Three Disciplines

### 一、把任務切到「小到能一次做對」— Decompose

業界叫 **task decomposition**。要領不是「切就好」, 而是切到**對的粒度**:

- 抓出 **3–7 個主要工作塊**; 每一塊都問自己「這塊能**直接做完**嗎, 還是裡面**還藏著好幾步**?」藏著就再切。
- 認清**依賴關係**: 哪些塊可以並行、哪些必須照順序 (前一塊的產出是後一塊的輸入)。
- **切錯了要重切**。最常見的錯是「切一次就不再回頭」——當某塊做下去才發現當初切錯, 要允許重新拆解, 不要硬著頭皮把錯的切法做完。

跟 AI 協作時, 切小有三個額外的好處: **每一塊都能單獨驗** (對上「明確標準」)、**context 不會爆**(一次塞一大包, AI 容易失焦漏細節)、**錯了好退**(壞的是一小塊, 不是整包重來)。

### 二、先用可行性小測試探路 — Spike First

這就是 Kent Beck 在 XP (極限編程, 1990 年代末) 講的 **spike solution**: **一段 time-box 的小實驗, 目的是降低不確定性**, 不是交付功能。

- **spike 的產出是「一個決定 / 一份 POC (proof of concept)」, 不是一個功能。** 它回答的是「這條路走不走得通」「這個做法可不可行」。
- 三條規則讓它不變成無底洞: **① 每個 spike 是一個問題 + 一個要做的決定; ② 有硬性 time-box 和驗收條件; ③ 結束一定留下書面結論。**

跟 AI 協作時這特別重要: AI 很敢「看起來很有信心地盲幹一大包」。**先讓它在小範圍試一次、證明路通了, 再放手讓它規模化**, 比直接叫它做完整包安全得多。這跟 [模型升級階梯](../../02-advanced/capabilities/model-escalation-ladder.md) 的「先用便宜的試、過門檻再放大」是同一個精神——先用最小成本換「這條路可行」的確定性。

### 三、明確的流程和標準 — Clear Process & Done

對應兩個經典概念: **流程 = 步驟與順序**, **標準 = definition of done / acceptance criteria (完成的定義 / 驗收條件)**。

- **標準要「可跑可驗」, 不能是「讀起來順」。** 好的完成定義是機器能判、你一眼能信的東西: compile 過 / test 綠 / 符合某份 checklist / 達到某個數字。原則見 [AI 產出怎麼驗](../../02-advanced/limits-and-verification/verifying-ai-output.md)。
- **流程寫死關鍵順序與停止條件**: 什麼先做、什麼後做、做到哪裡該停下來給人看。這正是 [Loop Engineering](../../02-advanced/engineering/loop-engineering.md) 說的「可驗證目標 + 停止規則」。
- 把標準寫在**可自動判定**的地方 (test、CI、`/goal` 的完成條件), AI 就能自己跑到達標才停, 不用你每一步盯著。

沒有明確標準, 「完成」就退化成「AI 說它做完了、你掃一眼覺得還行」——這是假的完成。

## 為什麼 AI 讓這三點從「加分」變「必需」— Why AI Raises the Stakes

一般工作裡, 這三點是「做了會更好」; 跟 AI 協作, 不做的代價**更大也更快**, 因為 AI 的失敗模式剛好專咬這三個缺口:

- **鋸齒狀前沿 (jagged frontier)**: AI 的能力邊界看不見, 兩個難度相仿的子任務, 可能一個完美一個離譜 ([LLM 的極限](../../02-advanced/limits-and-verification/llm-limitations.md))。**不切小, 你根本不知道它會在哪一塊崩**; 切小 + 每塊可驗, 才接得住。
- **看起來成功 ≠ 真的成功**: AI 很會產出「跑完沒報錯、格式漂亮」但其實是錯的東西。**沒有可驗標準, 你只會蓋橡皮章** ([AI 用久了會鈍化](../../02-advanced/limits-and-verification/ai-atrophy.md))。可行性小測試和明確標準, 就是逼出「真的成功」的證據。
- **context 有限**: 任務不切小、一次塞一大包, AI 容易失焦、漏掉中段細節。切小是在幫它 (也幫你) 守住注意力。
- **信任會累積到不再檢查**: 合作越順, 越容易省掉驗證——這正是 [一次自我鎖死的事故復盤](../../06-external/20261027_當合作太順利連驗證都省了：一次自我鎖死的事故復盤%20—%20kerr.md) 的教訓。**這三個紀律, 本質上就是「不靠信任、靠流程」接住高風險任務的安全帶。**

一句話收尾 (jason3e7): **跟 AI 協作不需要玄學, 需要的是你帶人、帶專案本來就該有的紀律——切小、先試、定標準。AI 不會讓你跳過它們, 只會讓「沒做」的帳單來得更快。**

## Sources

- oneuptime: How to Create Task Decomposition (切 3–7 塊、依賴圖、失敗就重切) — <https://oneuptime.com/blog/post/2026-01-30-task-decomposition/view>
- apxml: Task Decomposition Prompts for Agents — <https://apxml.com/courses/prompt-engineering-agentic-workflows/chapter-4-prompts-agent-planning-task-management/breaking-down-problems-prompts>
- AgileHour: Spike Work in Agile — How Teams De-Risk Delivery (spike = time-boxed、產出是決定/POC、三條規則) — <https://www.agilehour.org/blog/spike-work-in-agile-how-teams-de-risk-delivery-without-losing-speed>
- aqua-cloud: Agile Testing Spikes — A Complete Guide (Kent Beck / XP spike solution 起源) — <https://aqua-cloud.io/agile-testing-spike/>
- ozimmer.ch: A Definition of Done for Architectural Decision Making — <https://ozimmer.ch/practices/2020/05/22/ADDefinitionOfDone.html>
- ai-101: [模型升級階梯](../../02-advanced/capabilities/model-escalation-ladder.md) · [Loop Engineering](../../02-advanced/engineering/loop-engineering.md) · [AI 產出怎麼驗](../../02-advanced/limits-and-verification/verifying-ai-output.md)
