---
title: 用數學驗證的方法論來判斷 AI（Draft）
tags: [ai, 個人筆記, 驗證, 判斷力, prompt, 觀點, 草稿]
created: 2026-07-11
status: draft
---

# 用數學驗證的方法論來判斷 AI — Verifying AI Like We Verify Math

[← 回主頁](../index.md)

> [!NOTE]
> 草稿——jason3e7 提出的想法：AI 產出「聽起來對，不一定真對」的問題，其實和數學要對抗的問題本質相同。既然數學已經發展出一整套驗證方法論，那就把它借過來，變成日常判斷 AI 的姿勢。**討論還沒收完**，等後續補完再定稿。

> **TL;DR (EN):** jason3e7's angle: math's verification methods are humanity's answer to "sounds right but isn't", which is exactly the AI hallucination problem. Reuse them. Six math methods (list steps, list assumptions, cite evidence, counterexamples, edge cases, independent recomputation) map to two everyday moves each — bake into the prompt, and check after the answer arrives. Independent recomputation (ask again a different way or on a different model) is the single strongest move.

---

## 為什麼從數學借 — Why Borrow From Math

問題本質是一樣的：**數學處理的是「證明看起來對，但可能有漏」**；**AI 處理的是「答案看起來對，但可能是幻覺」**。兩件事都在對付同一個敵人——**表面說服力**。

差別只在：數學經過幾百年打磨，已經發展出一整套「怎樣才算真的被驗證過」的方法論。AI 才幾年，還沒有。所以與其重新發明，不如把數學的方法**直接搬過來**當日常姿勢。

jason3e7 這個想法要覆蓋的場景：

- **雙向**：寫 prompt 時就把驗證條件放進去，收到答案時自己也能查
- **融入日常**：不是特別場合、不是研究，是每天用 AI 時就能套的姿勢——所以要**輕、記得住、不變成儀式**

---

## 主軸：六個手段對照 — The Core Table

每一列都是一個數學驗證手段，兩欄告訴你**在 prompt 那頭要求什麼**、**答案回來後自己做什麼**。

| 數學方法 | 寫 prompt 時要求它 | 收到答案時自己做 |
|---|---|---|
| **列步驟**（proof，證明過程） | 「先列推理步驟再給結論，不要跳」 | 檢查每一步之間有沒有跳、有沒有偷渡假設 |
| **列假設** | 「列出這個結論依賴的假設」 | 看那些假設在你的實際情境是否成立 |
| **給依據**（citation） | 「每個斷言附可查證來源（數字/引用/時間）」 | 隨機抽一項回頭查（尤其是數字與人名） |
| **反例**（counterexample） | 「舉一個能推翻這結論的情境」 | 自己想一個「如果 X 不是這樣」的反例試試 |
| **極端／邊界**（edge case） | 「代入 0、極大值、負數、空集合會怎樣？」 | 用極端輸入代進去看它會不會崩 |
| **獨立驗算**（independent recomputation） | ⚠️（寫進 prompt 沒用） | **換問法或換模型再問一次，答案對得起來才信** |

### 特別強調：獨立驗算 — The Strongest Move

**這是六招裡最強、也最省事的一招。** 數學裡老師教「加法用減法驗」就是這個——用**獨立的路徑**得到同一個答案，才真的可信。

為什麼寫在 prompt 裡沒用：如果同一個模型、同一個 session 再問它一次，它會傾向重複自己剛才的說法（訓練特性）。真正的獨立驗算得**跳出這個 session**：

- **換問法**：把問題改一個角度、用不同措辭再問（同一個模型也行）
- **換模型**：問 Claude 拿到答案，再問 Gemini 或 GPT 一次
- **換來源**：直接 Google 或查權威資料，不透過 AI

對重要的答案，這一招值得每次做——它比其他五招加起來更能抓錯。

---

## 什麼時候不用跑全套 — When Not To

不是每題都要跑全套；否則「融入日常」的初衷就變成儀式。快速判準：

- **可逆 vs 不可逆**：要發文、寄信、下單、投稿、簽合約 → 值得驗；隨手查東西、腦力激盪 → 不用
- **有沒有別人依賴這個答案** → 有 → 要驗
- **我自己判斷得了嗎** → 得了 → 不用

大原則：**越接近「一旦錯了就麻煩」的答案，越該把六招搬出來——尤其是獨立驗算。**

---

## 討論待續（TODO for jason3e7）

- 這篇的定位：純方法論、還是要加真實案例？
- 需不需要一組「日常最常見的三個情境 + 對應要跑哪幾招」的速查？
- 和 [meta-prompting](./meta-prompting.md) 的關係要不要明講？（那篇「5 要素檢查」其實就是這裡「列假設 + 列步驟 + 給依據」的 prompt 端具體實作）

---

## 相關筆記 — Related

- [用 Prompt 生成好 Prompt（Meta-Prompting）](./meta-prompting.md) —— 「寫 prompt 端」的具體實作
- [AI 打破知識壁壘：被夾殺的知識中產](./ai-and-knowledge-barriers.md) —— 為什麼「判斷力」變成關鍵護城河
- [四種能力執行手冊](../02-advanced/four-capabilities-playbook.md) —— 依任務性質決定要不要驗（發想不必、摘要要驗）
