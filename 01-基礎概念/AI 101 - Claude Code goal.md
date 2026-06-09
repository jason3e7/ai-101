---
title: AI 101 - Claude Code /goal
tags: [ai, claude-code, goal, autonomous, loop, hook, 自動化]
created: 2026-06-09
---

# AI 101 - Claude Code /goal

> [!info]
> `/goal` 讓你設定一個**完成條件**，Claude 會在每個 turn 結束後自動評估是否達成，沒達成就繼續工作——不需要你一直按 Enter。需要 Claude Code v2.1.139+。

---

## 是什麼

一般的 Claude Code 工作方式：Claude 做完一件事 → 停下來等你 → 你再叫它繼續。

`/goal` 的工作方式：你寫好「完成的樣子」→ Claude 自己做、自己檢查、不符合就繼續做，直到條件成立才停。

**評估機制：** 每個 turn 結束後，條件和對話內容被送給 Haiku（小快模型）判斷是否達成。Haiku 回傳「是/否」和說明，「否」時 Claude 繼續下一個 turn。評估 token 費用可忽略不計。

---

## 和其他持續執行方式的比較

| 方式 | 下一個 turn 觸發時機 | 停止條件 |
|---|---|---|
| **`/goal`** | 上一個 turn 結束後 | 模型確認條件達成 |
| **`/loop`** | 固定時間間隔 | 你手動停，或 Claude 判斷完成 |
| **Stop hook** | 上一個 turn 結束後 | 你的腳本或 prompt 決定 |
| **auto mode** | — | Claude 自行判斷（單 turn 內） |

> [!tip]
> `/goal` + auto mode 是黃金組合：auto mode 消除每個工具呼叫的確認提示，`/goal` 消除每個 turn 的等待。兩者一起用才能做到真正的無人值守執行。

---

## 安裝 / 需求

- Claude Code **v2.1.139** 或更新版本
- 工作區需通過 trust dialog（因為 `/goal` 底層走 hooks 系統）
- 設定中未啟用 `disableAllHooks` 或 `allowManagedHooksOnly`

---

## 基本使用

### 設定 goal

```
/goal all tests in test/auth pass and the lint step is clean
```

設定後**立即開始**第一個 turn，不需要另外發 prompt。

### 查看狀態

```
/goal
```

顯示：條件內容、執行時間、已評估 turn 數、目前 token 消耗、最近一次評估原因。

### 提前清除

```
/goal clear
```

別名：`stop`、`off`、`reset`、`none`、`cancel` 都可以。

---

## 如何寫好一個 condition

評估模型**無法主動執行指令**，只能讀取對話裡已有的輸出。所以 condition 要讓 Claude 自己能「証明」它達成。

**好的 condition 三元素：**

| 元素 | 說明 | 例子 |
|---|---|---|
| 一個可量測的終點 | 測試結果、build exit code、檔案數量、空的 queue | `all 47 tests pass` |
| 明確的驗證方式 | Claude 要怎麼證明它達成了 | `` `npm test` exits 0 `` |
| 需要保持的約束 | 做到的路上不能破壞的東西 | `no other test file is modified` |

**加上上限防止無限跑：**

```
/goal all lint errors in src/api are fixed, or stop after 20 turns
```

Claude 每個 turn 會回報進度，Haiku 從對話內容判斷這個子句是否成立。

**完整範例：**

```
/goal all tests in test/auth pass, `npm run build` exits 0,
and no file outside src/auth was modified
```

---

## 進階

### 搭配 auto mode（無人值守）

```bash
claude --permission-mode auto
```

再設定 `/goal`，Claude 執行每個工具時不需要你確認，目標達成前也不會停下來問你。

### 非互動模式（CI / 腳本）

```bash
# 在 CI pipeline 或腳本裡執行
claude -p "/goal CHANGELOG.md has an entry for every PR merged this week"
```

goal 達成後 process 自動結束。Ctrl+C 可中斷。

### 續接 session

用 `--resume` 或 `--continue` 恢復 session 時，**未達成的 goal 會自動還原**——但 turn 計數、計時器、token 基準點重置。已達成的 goal 不會恢復。

### Goal vs Stop hook 的選擇

| 情境 | 建議 |
|---|---|
| 只用在這個 session | `/goal`（設完即走，session 結束自動清除）|
| 每個 session 都要套用 | Stop hook（寫進 settings.json）|
| 需要執行腳本做確定性判斷 | Stop hook（可呼叫 shell script）|
| 快速試驗條件 | `/goal`（改起來快）|

---

## 常見問題

**Q：Goal 設好之後我可以去做其他事嗎？**
可以。搭配 auto mode 後完全不需要守在旁邊，達成後 Claude 停止，狀態欄顯示 achieved。

**Q：Condition 裡可以寫多長？**
最多 4,000 字元，但越簡潔越好——條件複雜時 Haiku 判斷也容易出錯。

**Q：Haiku 評估錯了怎麼辦？**
Haiku 有時會誤判「已完成」（偽陽性）。高精度場景改用 Stop hook + shell script 做確定性驗證。

**Q：`/goal` 和 `/loop` 有什麼不同？**
`/goal` 是「做到條件成立才停」；`/loop` 是「每隔一段時間跑一次」，適合定期任務（例如每小時檢查一次 queue）。

---

## Sources

- [Claude Code 官方文件：/goal](https://code.claude.com/docs/en/goal)
- [Claude Code Best Practices](https://code.claude.com/docs/en/best-practices)
- [The Complete Claude /goal Guide for AI Agents (2026)](https://linas.substack.com/p/the-complete-claude-goal-guide)
