---
title: AI 101 - Claude 顯示 thinking 是什麼機制
tags: [claude, thinking, reasoning, extended-thinking, adaptive-thinking, api, 進階]
created: 2026-09-20
---

# Claude 顯示 thinking 是什麼機制 — How Claude Shows Thinking

[← 回主頁](../index.md)

> [!NOTE]
> 你在 Claude Code 或 API 看到的「Thinking…」不是 Claude 真正想的原文，而是**另一個模型**把它的推理**摘要**過一輪的版本。原始的完整思考被加密塞進 `signature` 欄位——你看得到，但看不懂。這篇拆解這套「思考／摘要／簽章」三層機制，為什麼要這樣設計，以及 Claude 4.7 之後 `budget_tokens` 為什麼直接被 400 掉、要改用 `thinking: {"type": "adaptive"}` ＋ `effort` 的原因。

> **TL;DR (EN):** What Claude shows as "Thinking…" is not the raw chain of thought — it is a summary produced by a separate model, while the full reasoning ships encrypted in the `signature` field of each thinking block. The design has three motivations: safety (raw CoT is easier to jailbreak from), faithfulness caveats (the surface reasoning may not reflect all internal factors), and cross-turn continuity without leaking. You are always billed for the *full* thinking tokens, never for the summary length. On Claude 4.7 and later, manual `budget_tokens` is a 400 error — switch to `thinking: {"type": "adaptive"}` and steer depth with `effort`. Interleaved thinking lets Claude reason between tool calls; the newest models add "progress update" blocks written for the human watching, not for the model itself.

---

## 三個角色，只有一個是「Claude 在想」— Three Roles, Only One Is the Thinking

回應裡的每一個 thinking 區塊，其實同時牽涉三個東西：

| 角色 | 誰產生 | 你看得到嗎 |
|:---|:---|:---|
| **原始思考**（raw reasoning） | 你點的那個模型（如 Opus 4.7） | ❌ 從來不外流 |
| **摘要**（summary） | **另一個較小的摘要模型** | ✅ 顯示在 `thinking` 欄位 |
| **簽章**（signature） | 伺服器把原始思考加密後的字串 | ⚠️ 看得到但**不透明** |

一個 API 回應長這樣：

```json
{
  "content": [
    {
      "type": "thinking",
      "thinking": "Let me break this down. The question has two parts, so I'll start with the simpler one…",
      "signature": "WaUjzkypQ2mUEVM36O2Txu..."
    },
    {
      "type": "text",
      "text": "Based on my analysis…"
    }
  ]
}
```

`thinking` 欄位是**摘要**——文件裡寫得很明白：**「No `display` setting returns the raw chain of thought.」** 沒有任何設定能拿到原文。`signature` 才是把完整推理加密後的憑證，下一輪你要照原樣傳回去，伺服器解密後 Claude 才能接著原本的思路走。

> [!IMPORTANT]
> **摘要模型跟思考模型不是同一個。** Anthropic 文件原話：「Summarization is processed by a different model from the one you target in your requests. The thinking model does not see the summarized output.」
>
> 意思是：思考時它是照自己的原文走的，摘要只是給你看的**副本**——所以就算摘要有偏差，也不會回過頭影響它接下來怎麼想。

---

## 為什麼不直接把原文丟給你 — Why Summarize at All

短答：**安全 + 忠實度未知 + 濫用防護**。Anthropic 官方 blog〈Claude's extended thinking〉點名三件事：

**一、原文可以被反向做 jailbreak。** 拿到完整思考就能觀察 Claude 怎麼被拒絕、拒絕的具體語句、以及在什麼點自我約束——這些是攻擊者最想要的訊號。摘要層等於一個天然的資訊瓶頸。

**二、思考未必忠實（faithfulness）。** 官方直言：「We don't know for certain that what's in the thought process truly represents what's going on in the model's mind.」——思考裡寫了什麼，不代表模型內部真的在算什麼。如果拿一個**未必準確**的東西當成模型的「真心話」直接呈現，反而會誤導使用者。

**三、避免給未來模型「藏思考」的動機。** 如果模型知道自己的思考會被完整讀走並被拿來罰它，強化學習可能把它訓練成「表面想 A、實際做 B」——這是 Anthropic 明確要迴避的行為。

摘要一層剛好切開這三件事：**保留可讀性，但拿掉可攻擊的細節、也不承諾這是「模型的真心」**。

> [!WARNING]
> **不要拿 `thinking` 欄位的內容當作 debug 的權威證據。** 它是別的模型的轉述，不是原始 log。真的想追行為，看 `text` 的實際輸出、看 `usage`、必要時做對照實驗——別把摘要當劇本讀。

---

## 計費：你被算的是「原文」，不是「你看到的字」— You Pay for the Original, Not the Summary

這是最常踩的坑：

- `usage.output_tokens` 記的是**原始思考 + 回應文字**的總量
- `usage.output_tokens_details.thinking_tokens` 單獨拆出思考佔了多少
- **摘要的長度跟計費完全無關**——你可能看到 3 行摘要，卻被算了 8,000 個 token

所以：把 `display` 設成 `"omitted"` **不會省錢**，只會省延遲（伺服器不用把摘要字串再串流一次）。想省錢只有兩條路：降 `effort` 或（在還支援的模型上）降 `budget_tokens`。

---

## Adaptive vs Extended：兩種控制方式，一個正在被淘汰 — Two Modes, One Retiring

Claude 對「思考多久」有兩種控制介面，正在世代交替中：

| | Extended（manual） | Adaptive |
|:---|:---|:---|
| **怎麼開** | `thinking: {"type": "enabled", "budget_tokens": 10000}` | `thinking: {"type": "adaptive"}` |
| **誰決定要不要想** | 你——每次一定會想到預算耗盡或提早收 | Claude 自己判斷，簡單題可能**完全不想** |
| **深度旋鈕** | `budget_tokens`（≥ 1024，且必須 < `max_tokens`） | `effort`（`low` / `medium` / `high` / `xhigh` / `max`） |
| **工具間思考** | 需要 beta header `interleaved-thinking-2025-05-14` | 自動 interleave，無需 header |
| **支援哪些模型** | Claude 4.5 及更早（有的只支援這個） | 4.6 以後預設；**4.7 以後只剩這個** |

**Claude 4.7 以後直接把 `type: "enabled"` 400 掉**——文件原話：「Claude 4.7 and later models do not support it and reject requests that use it, returning a 400 error.」在 4.6 上還能用但已 deprecated；在 Opus 5 / Sonnet 5 這些新模型，thinking 是**預設就開著**，你不用配置。

遷移就一個小動作：拿掉 `budget_tokens`，加上 `output_config: {"effort": "high"}`：

```json
// 舊
{"thinking": {"type": "enabled", "budget_tokens": 10000}}

// 新
{"thinking": {"type": "adaptive"}, "output_config": {"effort": "high"}}
```

> [!TIP]
> **`effort` 不是硬上限，`max_tokens` 才是。** 想控成本用 `effort` 建議，想控預算天花板用 `max_tokens` 卡死。兩者職責分開的，別混用。

---

## `display` 三種模式：控制你到底看到什麼 — The `display` Field

`display` 決定 `thinking` 欄位裡實際塞什麼字，跟計費**完全無關**：

- **`"summarized"`** — 塞摘要文字，4.6 及更早的預設
- **`"omitted"`** — 塞空字串，`signature` 照舊。5 系列、Opus 4.7 / 4.8 的預設。**用途：只想要最終答案、跳過摘要串流的延遲**
- **`"updates"`（beta）** — 推理區塊維持空字串，但**工具呼叫之間**的「進度更新」區塊會塞人話。需要 header `thinking-display-updates-2026-08-18`

進度更新（progress updates）是 Claude Fable 5.1 / Mythos 5.1 / Fable 5 新加的：**寫給旁邊看螢幕的人的**一兩句話（「Confirmed the retry path never refreshes the expired token. Editing auth.py to add the refresh call.」），跟推理區塊分開存放、有自己的 signature。它不是給模型自己看的推理，就是狀態列。

> [!NOTE]
> 想做「隱藏推理但秀出狀態列」的 agent 介面：模型挑支援的（Fable 5.1 系列）、`display: "updates"`、只渲染 `thinking` 欄位非空的區塊——那些就是進度更新。

---

## Interleaved thinking：工具呼叫**之間**也能想 — Reason Between Tool Calls

預設情況下 Claude 是「一次想完 → 呼工具 → 拿結果 → 直接寫答案」。**Interleaved thinking** 開啟後多了一個位置：每次收到 tool result，先產一個新的 thinking 區塊思考「這個結果代表什麼、下一步該做什麼」再繼續。

這對多步驟的 agent 特別重要——沒有它，模型很容易在拿到不預期的 tool 結果時硬走原本的計畫；有它，才能真的**根據中間結果調整**。

**Adaptive 模式自動就有；Extended 模式**在 Claude 4 / 4.5 上要加 beta header，Sonnet 4.6 手動模式的 header 也還能用但已 deprecated，Opus 4.6 手動模式**完全沒有** interleaved thinking（要就換 adaptive）。Haiku 4.5 系列從頭到尾不支援。

---

## 跨 turn 的思考該怎麼傳 — Passing Thinking Back Across Turns

規則不是「盡量精簡」，是**照原樣傳回去，讓伺服器決定**：

- **同一個 tool-use turn 內**：所有 thinking 區塊**必須**原封不動傳回，順序不能改、內容不能編、`redacted_thinking` 區塊也不能過濾掉，否則會 400
- **跨 turn**：建議全部傳回。伺服器會依照模型自動決定要保留還是丟——你不用自己修剪
- **模型的保留策略**：Opus 4.5+ / Sonnet 4.6+ / 5 系列**保留所有先前 turn 的 thinking**（會計入 input token）；Haiku 全系列＋更舊的 Opus / Sonnet 只**保留最近一 turn**，其他自動剝掉

`signature` 是這一切的鑰匙——它同時**驗證 thinking 是 Claude 產的**（不是你偽造的），也**確認 prefix 沒被動過**（`system` / `tools` / 前面的訊息任一改動，這個 block 之後全都失效）。

> [!CAUTION]
> **千萬別自己重寫 thinking 內容再傳回。** API 會直接 400。想瘦身 context 有正規做法：用 `clear_thinking_20251015` context-editing 策略，或伺服器端 compaction，別 DIY。

---

## 常被誤解的幾件事 — Common Misconceptions

| 誤解 | 事實 |
|:---|:---|
| 「Claude Code 顯示的 Thinking 就是模型思考的原文」 | 是**摘要模型**的產出，不是原文 |
| 「把 `display` 關成 omitted 可以省錢」 | 只省延遲，計費照原始 thinking token 算 |
| 「摘要不對表示模型想錯了」 | 摘要跟思考是兩個模型，摘要偏差 ≠ 推理偏差 |
| 「adaptive 模式一定會想」 | 低 `effort` 下遇到簡單問題可能**完全不想**、沒有 thinking 區塊 |
| 「4.7 也支援 budget_tokens」 | 直接 400，`"thinking.type.enabled" is not supported` |
| 「思考文字沒回傳就不會計費」 | 照傳照算，`usage.output_tokens_details.thinking_tokens` 有明細 |
| 「thinking 區塊可以自己編輯後傳回」 | 立刻 400，signature 對不上就是對不上 |

---

## Sources

- [Thinking — Claude Docs](https://platform.claude.com/docs/en/build-with-claude/thinking)
- [Extended thinking — Claude Docs](https://platform.claude.com/docs/en/build-with-claude/extended-thinking)
- [Claude's extended thinking — Anthropic News](https://www.anthropic.com/news/visible-extended-thinking)
- [Preserved thinking — Claude Docs](https://platform.claude.com/docs/en/build-with-claude/preserved-thinking)
- [Effort — Claude Docs](https://platform.claude.com/docs/en/build-with-claude/effort)
