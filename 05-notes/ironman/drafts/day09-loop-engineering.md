---
title: "AI 101 - 鐵人賽 Day 09: Loop Engineering, 給它一個測試, 讓它自己跑到過"
tags: [ai, 鐵人賽, ironman, loop-engineering, agent, 自動化, 草稿]
created: 2026-09-06
status: draft
---

# Day 09｜Loop Engineering: 給它一個測試, 讓它自己跑到過

[← 回主頁](../../../index.md)｜[參賽規劃](../plan.md)｜[三十篇標題](../titles.md)

> [!NOTE]
> 前三天一路往外退：prompt 是那一句（[Day 06](./day06-prompt-engineering.md)）、context 是模型看到的全部（[Day 07](./day07-context-engineering.md)）、harness 是外面那台跑每一輪的機器（[Day 08](./day08-harness-engineering.md)）。你可能以為今天是「再包一層」。**不是。今天不是加一層，是換一種站法** - 從「陪它跑每一輪」退到「設好目標和停止規則，走人」。

> **TL;DR (EN):** Loop Engineering means designing the system that prompts your agent, instead of typing every next instruction yourself. Four parts: a trigger, a verifiable goal with a verifier, context management, and stop rules. What changed is a stance: you stop riding each turn (Days 06-08) and start setting goals and stop-conditions. None of the parts are new; they just finally got reliable enough to leave running unattended.

---

## 不用每輪驗, 寫一個測試給它自己對 — Let the Test Do the Checking

前三天 (prompt → context → harness) 你都還在**每一輪**裡陪它跑. Loop 是換一種站法.

同一個任務「修好這 8 個測試」, 兩種站法:

| | 陪跑（Day 06-08 心法） | 走人（Loop Engineering） |
|:---|:---|:---|
| 動作 | 打「看 test 1 為什麼錯」→ 讀輸出 → 給改法 → 打「再跑一次」→ ... | `/goal` 設「所有測試通過」→ 走人吃午餐 |
| 每輪你在做 | 讀 output、決定下一句 | 沒事 |
| 8 個 bug | 你陪跑 8 遍 | 你什麼都沒做 |

**技術是同一套**（背後都是 tool call + retry + verify）, 差的是你花多少時間陪跑. Loop Engineering 就是把那個時間差搬掉.

順帶分清楚兩個常搞混的詞:

| | **鏈（chain）** | **迴圈（loop）** |
|:---|:---|:---|
| 形狀 | 直線、固定（A → B → C） | 循環、可改（會重複、分支、換方向） |
| 行為 | 跑一次就結束 | 看結果再決定，反覆到目標達成 |

Claude Code、Codex、Devin 這些工具的核心運作就是迴圈：**讀檔 → 寫 code → 跑測試 → 讀錯誤 → 修 → 再跑**，不需要你每一步重新下指令。

---

## 一個迴圈由什麼組成 — Anatomy of a Loop

最小骨架就四件事：

1. **觸發（trigger）** - 什麼時候開始跑？手動、排程、CI 失敗、收到訊息。
2. **可驗證的目標 ＋ 驗證者（verifier）** - 怎樣算「做完了」，而且**能被檢查**（測試通過、lint 乾淨）。
3. **上下文管理（context management）** - 每一輪它「看得到什麼」？它記不記得八輪前試過、失敗過什麼？
4. **停止規則與護欄（stop rules & guardrails）** - 防止無限迴圈、防止燒錢失控。

> [!IMPORTANT]
> 第 2 點是整個迴圈的命門。**沒有可驗證的目標，迴圈不知道什麼時候該停** - 它只會一直說「我覺得完成了」。而 [Day 03](./day03-what-it-cannot-do.md) 已經證明過：它不會自己發現自己錯了。所以**驗證者必須是外部的**。

第 3 點也有一個實作重點：

> [!TIP]
> **對話一長就會被壓縮，早期講過的指令可能在壓縮中遺失** - 這正是 [Day 03](./day03-what-it-cannot-do.md) 講的「模型不會從對話裡學會」。設計上，放進 `CLAUDE.md` 這類每輪重新注入的檔案比塞在對話裡更可靠；但**這條的實際效果你要自己在你的設定上測**，不同版本、不同壓縮策略下表現不一定一樣。

Addy Osmani 進一步把「一個完整的迴圈系統」拆成六塊積木：

| 積木 | 作用 | 在 Claude Code 對應 |
|:---|:---|:---|
| **自動化** | 排程自動觸發 | `/loop`、`/goal`、cron、GitHub Actions |
| **Worktree** | 隔離並行的 agent，避免改到同一份檔案打架 | `git worktree` |
| **Skills** | 把專案知識寫成檔，不用每次重講 | `SKILL.md` |
| **連接器** | 接外部工具 | MCP（Linear、Slack、DB、API） |
| **Sub-agents** | 把「寫的人」和「驗的人」分開 | 不同指令／模型的子 agent |
| **狀態檔** | 記已完成／待辦，讓明天的迴圈接得上 | Markdown 或看板 |

---

## 在 Claude Code 上跑一次看看 — Try It Yourself

準備 5 分鐘, 完整體驗一次 loop. 建一個示範 project:

```bash
mkdir loop-demo && cd loop-demo
npm init -y && npm install --save-dev jest
npm pkg set scripts.test=jest

# 故意錯的 function
cat > sum.js << 'EOF'
function sum(a, b) { return a - b; }
module.exports = sum;
EOF

# 兩個會失敗的測試
cat > sum.test.js << 'EOF'
const sum = require('./sum');
test('1 + 1 = 2', () => expect(sum(1, 1)).toBe(2));
test('2 + 3 = 5', () => expect(sum(2, 3)).toBe(5));
EOF

claude   # 開 Claude Code
```

在 Claude Code 裡打:

```
/goal 跑 npm test 全部通過
把 sum.js 修到測試綠. 你自己跑 npm test 驗證, 不用問我.
```

它會自己: 讀檔 → 認出 `-` 應該是 `+` → 改 → 跑 `npm test` → 綠. 通常一輪就結束. **你只寫了 1 次 goal ＋ 1 句 prompt, 之後沒再打字**, 停止條件是「測試綠」而不是「你按 Ctrl+C」. 這就是最小可跑的 loop.

**變態版練習**: 把 `sum.test.js` 加更多邊界條件 (負數、浮點、大數、空 argument), 或把 `sum.js` 邏輯換複雜一點 (例如寫錯 recursion). 它會多輪修 → 跑 → 修 → 跑, 你在旁邊倒杯咖啡看它跑.

---

**進階零件, 用來組更複雜的 loop**:

| 零件 | 用途 |
|:---|:---|
| PostToolUse Hook | Write / Edit 後自動跑 lint、test、format |
| `/loop` | 讓它按節奏反覆執行某個任務 |
| `Worktree` | 並行多個 agent 不會改到同一份檔案 |
| `Sub-agent` | 「寫的人」跟「驗的人」分開, 避免它改自己的考卷 |
| MCP connector | 接 Linear / Slack / DB, 讓 loop 能開 PR、更新票、發通知 |

一個完整迴圈長這樣 (Addy Osmani 的範例):

```text
每天排程觸發 → skill 讀 CI 失敗與待辦 issue
   → 每項開一個獨立 worktree
   → 一個 sub-agent 草擬修復, 另一個 sub-agent 驗證
   → connector 自動開 PR、更新票、通知團隊
   → 狀態檔記錄進度, 明天的迴圈接著跑
```

---

## Web chat 版本呢 — What About Web Chat

Web chat 天生對 loop 不友善: 沒 Hook、沒 verifier CLI、每輪都要你按 send. Loop Engineering 在 web chat 上有三條路:

1. **用內建的 Research mode**. ChatGPT Deep Research、Claude Research、Gemini Deep Research 就是 web 版的 loop, 你設一個問題, 它自己搜、讀、寫、驗、產出報告, 20 到 40 分鐘後回來看. **一次性研究這條最省事**
2. **Custom GPT / Claude Project 內嵌自檢**. 在 Instructions 寫「每輪回答完先自我 review, 有問題就修正再輸出」, 每輪還是要你按 send, 但你不用親手 review. **半自動化, 適合反覆固定的工作流**
3. **走 API 包 loop**. 想要完全自動化, 得離開 web chat, 用 Claude API / OpenAI API + 幾十行 Python: fetch prompt → 執行 → 驗 → 決定停止, 全部你自己控制. **控制粒度最高, 但也最花工**

**選擇邏輯**: 一次性研究 → Research mode; 反覆固定工作流 → Custom GPT / Project; 完全自動化 → 離開 web chat, 上 CLI (Claude Code、Codex) 或 API.

---

## 陷阱，還有一個誠實的問題 — Pitfalls and an Honest Question

跑得越自動，越要小心這四個坑：

- **無人監督的驗證** - 迴圈自己驗自己。驗證機制不可靠，錯的東西就會被自動放行。
- **理解債（understanding debt）** - code 生成越快，你對自己專案的理解欠得越多，總有一天要還。
- **認知投降（cognitive surrender）** - 設計迴圈很容易變成「用它來避免思考」，而不是「用它來加速思考」。
- **無限迴圈、燒錢失控** - 沒有停止規則與成本護欄，它會一直跑。

> Addy Osmani 的提醒：**以打算留任的工程師身份來建迴圈，而不是只按下執行鍵的人。** 迴圈是放大器 - 放大你的產出，也放大你偷懶的後果。

最後是一個必須誠實回答的問題。

> [!TIP]
> （jason3e7 的觀察）第一眼就覺得「這跟原本的技術差不多啊」 - 這個直覺是對的。

Loop Engineering **沒有技術突破，零件全是舊的**：act → observe → decide → repeat 就是 2022 年的 ReAct；`/goal`、worktree、skills、MCP 全是現成工具。它新在兩件事：

1. **命名與重點轉移。** 像「DevOps」 - 沒發明任何技術，只是幫一種做法取了名字。一旦「迴圈」成為你思考的單位，你才會開始問對的問題：verifier 是什麼？停止規則是什麼？狀態怎麼存？
2. **一個門檻被跨過了。** 早期 agent 跑幾步就漂移，你非盯著不可。2026 年因為自動壓縮、`CLAUDE.md` 重注入、worktree 安全並行，**「設好迴圈、走人」第一次變得實際可行** - 這正好是 [Day 02](./day02-why-it-got-strong.md) 那條門檻曲線的另一個切面。

---

## Sources

- [Loop Engineering — Addy Osmani](https://addyosmani.com/blog/loop-engineering/)
- [What Is Loop Engineering? The New Meta for AI Coding Agents — MindStudio](https://www.mindstudio.ai/blog/what-is-loop-engineering-ai-coding-agents)
- [Agentic Loops: From ReAct to Loop Engineering (2026) — Data Science Dojo](https://datasciencedojo.com/blog/agentic-loops-explained-from-react-to-loop-engineering-2026-guide/)
- [How the agent loop works — Claude Code Docs](https://code.claude.com/docs/en/agent-sdk/agent-loop)
