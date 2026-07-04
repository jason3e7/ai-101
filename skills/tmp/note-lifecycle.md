---
name: note-lifecycle
description: （草稿·未啟用）判斷一篇筆記何時該刪除，以及刪除前的替代方案與守則
tags: [draft, note-management, lifecycle, pruning]
---

# 筆記生命週期與刪除條件 — Note Lifecycle & Deletion Criteria

> [!WARNING]
> **這是草稿，尚未啟用。** 放在 `skills/tmp/` 供 review。確認要用再移到 `skills/` 正式生效。

> **TL;DR (EN):** Deletion is the last resort. Prefer merge / deprecate / downgrade-to-reference. Only delete when a note is redundant, wrong-and-not-worth-fixing, or its subject is dead with no learning value — and always fix inbound links and remove it from the index first. Git keeps it recoverable, so keep the visible KB clean.

---

## 核心立場 — Core Stance

1. **刪除是最後手段。** 先問「能不能合併、標記過時、或降級到 `參考/`」，都不行才刪。
2. **Git 讓刪除可回復。** 刪掉的筆記還在歷史裡，所以「讓可見的知識庫保持乾淨」比「怕刪錯」更重要——但這不代表可以隨手刪。
3. **判準回到最高原則**：一篇筆記若已無法提供「清楚概念」或「快速上手」任一價值，才是候選。

---

## 筆記的五個生命階段 — Five Stages

| 階段 | 狀態 | 該做的事 |
|---|---|---|
| **新生 Active** | 剛寫、正在被引用 | 維護、更新 |
| **成熟 Stable** | 內容穩定、仍準確 | 偶爾校對 |
| **過時 Stale** | 資訊落後但主題還在 | **更新** 或標記 deprecated，通常不刪 |
| **廢棄 Obsolete** | 主題本身消失／被取代 | 評估是否有學習或歷史價值 |
| **候選 Prunable** | 對兩個最高原則都無貢獻 | 走下面的刪除流程 |

---

## 成為刪除候選的條件 — When a Note Becomes Prunable

符合**任一**才進入評估（不是符合就刪）：

- **A. 冗餘 Redundant**：內容已被另一篇更完整地涵蓋，這篇沒有獨特資訊
- **B. 錯誤且不值得修 Wrong**：關鍵內容有誤，且更正的成本高於重寫或刪除
- **C. 主題死亡 Dead subject**：工具停更／服務關閉／技術被淘汰，**且**這篇沒有學習或歷史參考價值
- **D. 未竟稿 Abandoned stub**：從未寫完的殘稿，且主題已不再相關
- **E. 時效過期 Expired（僅限 `06-external/reference/`）**：時效性快照（版本評測、新聞、社群討論）過了有效期，不會再被查閱

---

## 刪除前先考慮的替代方案 — Prefer These First

| 情況 | 比刪除更好的做法 |
|---|---|
| 內容被別篇涵蓋 | **合併**：把獨特片段搬進主篇，再刪空殼 |
| 只是過時、主題還在 | **更新**，或加 `> [!WARNING] 已過時` 標記 |
| 主題淘汰但有歷史價值 | **降級**到 `06-external/reference/`，或加 deprecated 註記保留 |
| 一手經驗、實驗紀錄 | **永遠保留**（見下方「不刪」） |

---

## 刪除前檢查清單 — Pre-Deletion Checklist

真的要刪，逐項確認：

- [ ] **查入站連結**：`grep -rn "檔名" .`，把指向它的連結改掉或移除
- [ ] **確認不是一手內容**：不是親身實驗、原創框架、獨有紀錄
- [ ] **確認不是最後一份**：這個資訊在別處還找得到
- [ ] **從 `index.md` 移除**對應列
- [ ] **git commit**，訊息寫清楚刪除理由（保留可回溯）

> [!CAUTION]
> 只要有一項不確定，就**不要刪**——改用替代方案（合併／降級／標記）。刪錯的成本（斷連結、丟失脈絡）通常高於留著一篇稍嫌多餘的筆記。

---

## 決策流程 — Decision Flow

```
這篇還有「清楚概念」或「快速上手」價值嗎？
├─ 有 → 保留（過時就更新/標記）
└─ 沒有 → 是一手內容 / 實驗紀錄嗎？
          ├─ 是 → 保留（歷史價值）
          └─ 否 → 內容被別篇涵蓋嗎？
                  ├─ 是 → 合併獨特片段 → 刪空殼
                  └─ 否 → 主題死亡或內容錯誤嗎？
                          ├─ 是 → 跑檢查清單 → 刪除
                          └─ 否 → 保留（再想想）
```

---

## 永遠不刪 — Never Delete

- **一手經驗與實驗紀錄**（如 `05-notes/` 的 HTB 實戰）——就算方法過時，過程本身是紀錄
- **原創的思考框架**（如能力全景圖）——不會因工具換代失效
- **仍被多篇引用的樞紐筆記**——先處理依賴，不能直接刪

---

## 待你決定的設計問題 — Open Questions（review 用）

1. `06-external/reference/` 的**時效期**要不要訂具體天數（例：滿 1 年自動列入候選）？還是永遠人工判斷？
2. 「標記 deprecated」要用什麼統一形式？（frontmatter 加 `status: deprecated`？還是頂部 callout？）
3. 刪除要不要留一個 `graveyard.md` 記錄「刪了什麼、為什麼」，還是純靠 git log？
