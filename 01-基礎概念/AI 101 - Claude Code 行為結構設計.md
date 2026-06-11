---
title: AI 101 - Claude Code 行為結構設計
tags: [ai, claude-code, hooks, sub-agents, skills, goal, 個人筆記]
created: 2026-06-11
---

# Claude Code 行為結構設計

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> `/goal`、Sub-agents、Skills、Hooks 是 Claude Code 四個讓你「設計 AI 行為」而不只是「下指令給 AI」的核心功能。這篇從為什麼到怎麼做，每個功能都附可直接貼上執行的 Hello World 範例。

---

## 傻傻下 `/goal` vs. 設計行為結構

| 做法 | 你在做什麼 | 問題 |
|---|---|---|
| 只下 `/goal` 或長 prompt | 每次把所有規則塞進 prompt | context 越來越長規則被稀釋；沒辦法跨 session 複用 |
| 設計行為結構 | 用 CLAUDE.md、skills、agents、hooks 分層定義行為 | 常駐規則不佔 context；功能可複用、可組合 |

**分層概念：**

```
CLAUDE.md       → 每個 session 都重新載入的「恆久規則」（角色、環境、限制）
Sub-agents      → 有獨立 context 的專責模組（planner、summarizer、reviewer）
Skills          → 可隨時呼叫的流程 SOP（/handoff、/review、/deploy）
Hooks           → 不依賴 AI 判斷、保證執行的自動化（logging、格式化、通知）
/goal           → 設定完成條件讓 Claude 自動跑到達成，不需要你守著
```

這五層組合起來，才是真正的「設計 AI 行為」，而不是「每次重新解釋給 AI 聽」。

---

## 專案結構

```
my-project/
├── CLAUDE.md                          # 恆久規則
└── .claude/
    ├── settings.json                  # hooks 和權限設定
    ├── agents/
    │   └── greeter.md                 # 自訂 sub-agent
    ├── skills/
    │   └── hello/
    │       └── SKILL.md               # 自訂 skill (/hello)
    └── hooks/
        └── log_cmd.py                 # PostToolUse hook
```

---

## 1. `/goal` — 設定完成條件，自動跑到達成

> [!info]
> 需要 Claude Code v2.1.139 以上。每次 turn 結束後，一個輕量模型自動判斷條件是否達成，沒達成就繼續下一 turn。

### Hello World

```
/goal 目前目錄下有一個 hello.txt，內容是 "Hello, World!"
```

Claude 會自動建立這個檔案然後停下來。

**驗證：**
```bash
cat hello.txt
# 應該看到：Hello, World!
```

### 寫好條件的規則

好的完成條件要「可機器驗證」：

```
# 壞的寫法（太模糊）
/goal 完成登入功能

# 好的寫法（具體、可驗證）
/goal npm test 的 auth 相關測試全部 pass，且 git status 是乾淨的
```

**常用模式：**

```
# 檔案存在且符合格式
/goal flag.txt 存在且內容符合 HTB\{.*\} 格式

# 指令結果
/goal pytest tests/ 全部通過且 ruff check . 沒有 error

# 兩個條件都滿足
/goal user.txt 和 root.txt 都存在，各自符合 ^[a-f0-9]{32}$

# 有 turn 數限制（防止無限跑）
/goal 所有測試通過，或在 20 個 turn 後停止
```

**指令速查：**

```
/goal              # 查看目前進度（turn 數、token、evaluator 的判斷理由）
/goal clear        # 取消（別名：stop / off / reset / none）
```

---

## 2. Sub-agents — 有獨立 context 的專責模組

每個 sub-agent 有自己的 context window，不會污染主對話。主 Claude 委派任務過去，只拿摘要回來。

**為什麼重要：**
- 主 context 保持乾淨（noisy 的 nmap/gobuster 輸出交給 summarizer 消化）
- 不同模組職責分明（planner 只選下一步，不跑指令）
- Sub-agent 無法再 spawn sub-agent（只有一層）

### 檔案格式

`.claude/agents/<name>.md`

```markdown
---
name: greeter
description: >-
  Greets a user by name. Use this when asked to greet someone.
tools: ""
---

You are a greeter. When given a name, respond with exactly:
"Hello, [name]! Sub-agents are working."
```

**重要 frontmatter 欄位：**

| 欄位 | 說明 |
|---|---|
| `name` | 唯一識別名稱（lowercase + hyphens）|
| `description` | Claude 根據這個決定要不要委派，要寫清楚「什麼時候用」|
| `tools` | 可用工具白名單；留空字串 `""` 代表不給工具；省略代表繼承所有工具 |
| `model` | `sonnet` / `opus` / `haiku`，省略繼承主模型 |
| `maxTurns` | 最多幾個 agentic turn 後停止 |

### Hello World

**建立 `.claude/agents/greeter.md`：**

```markdown
---
name: greeter
description: >-
  Greets a user by name. Use this when asked to greet someone.
tools: ""
---

You are a greeter. When given a name, respond with exactly:
"Hello, [name]! Sub-agents are working."
```

**測試（在 Claude Code 對話中輸入）：**

```
Use the greeter agent to greet Alice.
```

或用 @-mention 強制指定：

```
@"greeter (agent)" greet Alice
```

**驗證：** 你應該看到 `Hello, Alice! Sub-agents are working.`

> [!warning]
> Agent 定義檔是 session 啟動時載入的。修改現有 agent 檔案後，目前 session 會更新；但**新增**一個之前不存在的 agent 目錄，需要重啟 Claude Code 才能識別。

### 真實場景：Planner + Summarizer 分工

```markdown
---
name: planner
description: >-
  Decides the single best next step. Invoke before any non-trivial action.
  Given current state, returns ONE command with rationale and expected outcome.
tools: Read, Grep, Glob
---

You are the planning module. Do NOT run commands. Given:
- Current objective
- Distilled findings so far

Return exactly:
1. **Next step**: one concrete command
2. **Rationale**: why this reduces uncertainty most
3. **Expected outcome**: what confirms or refutes the hypothesis
4. **If it fails**: immediate fallback
```

```markdown
---
name: summarizer
description: >-
  Distills verbose tool output into structured findings. Invoke after any noisy
  command (nmap, gobuster, sqlmap) to keep main context clean.
tools: Read, Write, Edit
---

Extract only decision-relevant facts from the raw output.
Append to findings.md. Return a 3-5 line summary.
```

主 Claude 使用方式：

```
Before running nmap, delegate to planner with the current state.
After nmap completes, pass the raw output to summarizer.
```

---

## 3. Skills — 可呼叫的流程 SOP

Skill 是寫在 `SKILL.md` 裡的指令集，用 `/skill-name` 呼叫。和 CLAUDE.md 不同的是：**只有被呼叫時才載入 context**，不會每次都佔位置。

### 檔案格式

`.claude/skills/<skill-name>/SKILL.md`

目錄名稱決定 `/指令名稱`，frontmatter 的 `name` 只是顯示標籤。

```markdown
---
name: hello
description: Say hello to verify skills are working. Use when asked to test skills.
---

Say exactly: "Hello, World! Skills are working."
```

**重要 frontmatter 欄位：**

| 欄位 | 說明 |
|---|---|
| `description` | Claude 根據這個決定要不要自動觸發。留空則用第一段內文。 |
| `argument-hint` | 在自動補全顯示的提示，如 `[target-ip]` |
| `disable-model-invocation: true` | 只允許使用者手動 `/呼叫`，Claude 不會自動觸發 |
| `allowed-tools` | 執行這個 skill 時允許的工具（不用每次跳確認） |

### Hello World

**建立 `.claude/skills/hello/SKILL.md`：**

```markdown
---
name: hello
description: Say hello to verify skills are working. Use when asked to test skills.
---

Say exactly: "Hello, World! Skills are working."
```

**測試：** 在 Claude Code 對話中輸入 `/hello`

**驗證：** Claude 應該回應 `Hello, World! Skills are working.`

### 動態注入（`!` 語法）

Skill 內容可以在執行前先跑 shell 指令，把輸出插進去：

```markdown
---
name: status
description: Show current git status and last 3 commits
---

## Current state

### Git status
!`git status --short`

### Recent commits
!`git log --oneline -3`

Review the above and summarize what's in progress.
```

呼叫 `/status` 時，`!` 後的指令會先跑，輸出直接插進 skill 內容，Claude 才看到。

### 真實場景：Handoff（進度交接文件）

```markdown
---
name: handoff
description: >-
  Generate progress.md, a structured handoff document for a failed or interrupted
  attempt so a later run can resume without redoing dead ends.
  Invoke when stuck, hitting a dead end, or before an interrupted run ends.
---

Review the conversation history and findings.md, then write progress.md with:

## 🎯 Target Information
## 🔍 Discoveries
## 🛡️ Vulnerabilities Identified
## ⚡ Attack Vectors Attempted
## 🚫 Dead Ends (DO NOT RETRY)
## 📍 Current State
## 🎯 Recommended Next Steps
## 💡 Key Insights
```

呼叫：`/handoff`
下次 resume：`/goal Continue. First read progress.md. Done when flag.txt matches HTB\{.*\}.`

---

## 4. Hooks — 保證執行的自動化

Hook 是在特定事件自動跑的 shell 指令。**不依賴 AI 判斷**，條件符合就跑。

### 事件總覽

| 事件 | 觸發時機 | 可阻擋？ |
|---|---|---|
| `PreToolUse` | 工具執行前 | 是（exit 2）|
| `PostToolUse` | 工具執行後 | 否 |
| `Stop` | Claude 準備停止時 | 是（exit 2）|
| `UserPromptSubmit` | 使用者送出訊息後 | 是（exit 2）|
| `SessionStart` | session 開始 | 否 |

### 設定格式（`.claude/settings.json`）

```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          {
            "type": "command",
            "command": "python3 .claude/hooks/log_cmd.py"
          }
        ]
      }
    ],
    "Stop": [
      {
        "hooks": [
          {
            "type": "command",
            "command": "python3 .claude/hooks/summarize.py"
          }
        ]
      }
    ]
  }
}
```

**Matcher 規則：**

| 寫法 | 意思 |
|---|---|
| 省略 / `"*"` | 所有工具 |
| `"Bash"` | 只有 Bash 工具 |
| `"Edit\|Write"` | Edit 或 Write（`\|` 分隔）|
| `"^mcp__"` | 正則：所有 MCP 工具 |

### 退出碼語意

| exit code | 效果 |
|---|---|
| `0` | 成功，解析 stdout JSON（若有）|
| `2` | 阻擋操作（PreToolUse → 不跑工具；Stop → 不停止）|
| 其他 | 非阻擋，顯示第一行 stderr |

### Hello World

**建立 `.claude/hooks/log_cmd.py`：**

```python
#!/usr/bin/env python3
"""PostToolUse(Bash): append every command to commands.log"""
import json, sys, time

try:
    payload = json.load(sys.stdin)
except (json.JSONDecodeError, ValueError):
    sys.exit(0)

cmd = (payload.get("tool_input") or {}).get("command", "")
if cmd:
    ts = time.strftime("%H:%M:%S")
    with open("commands.log", "a") as f:
        f.write(f"[{ts}] {cmd}\n")

sys.exit(0)
```

**設定 `.claude/settings.json`：**

```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/log_cmd.py" }
        ]
      }
    ]
  }
}
```

**測試：** 在 Claude Code 對話中請 Claude 跑任何 bash 指令，例如 `Run ls -la`

**驗證：**
```bash
cat commands.log
# 應該看到帶時間戳的 ls -la 指令
```

### 真實場景：阻止危險指令

```python
#!/usr/bin/env python3
"""PreToolUse(Bash): block rm -rf"""
import json, sys, re

try:
    payload = json.load(sys.stdin)
except (json.JSONDecodeError, ValueError):
    sys.exit(0)

cmd = (payload.get("tool_input") or {}).get("command", "")
if re.search(r"rm\s+-rf\s+/", cmd):
    print("BLOCKED: rm -rf / is not allowed", file=sys.stderr)
    sys.exit(2)  # exit 2 = 阻擋

sys.exit(0)
```

Matcher 設 `"Bash"`，事件設 `PreToolUse`。

### Hook 收到的 stdin JSON 結構

```json
{
  "session_id": "abc123",
  "transcript_path": "/path/to/transcript.jsonl",
  "hook_event_name": "PostToolUse",
  "tool_name": "Bash",
  "tool_input": { "command": "ls -la" },
  "tool_output": "total 8\ndrwxr-xr-x ..."
}
```

`PostToolUse` 才有 `tool_output`；`PreToolUse` 只有 `tool_input`。

---

## 完整組合範例（驗證四層全部正常）

以下是一個最小的「四層都有」的專案結構，可以直接貼上建立並驗證：

**第一步：建立目錄結構**

```bash
mkdir -p my-test/.claude/{agents,skills/hello,hooks}
cd my-test
```

**第二步：建立 CLAUDE.md**

```markdown
# Test Project
This is a test project to verify Claude Code structural features.
Always respond in Traditional Chinese.
```

**第三步：建立 `.claude/agents/greeter.md`**

```markdown
---
name: greeter
description: Greets a user by name. Use when asked to greet someone.
tools: ""
---
When given a name, respond with exactly: "你好，[name]！Sub-agent 運作正常。"
```

**第四步：建立 `.claude/skills/hello/SKILL.md`**

```markdown
---
description: Test that skills are working
---
Say exactly: "你好！Skill 運作正常。"
```

**第五步：建立 `.claude/hooks/log_cmd.py`**

```python
#!/usr/bin/env python3
import json, sys, time
try:
    payload = json.load(sys.stdin)
except (json.JSONDecodeError, ValueError):
    sys.exit(0)
cmd = (payload.get("tool_input") or {}).get("command", "")
if cmd:
    with open("commands.log", "a") as f:
        f.write(f"[{time.strftime('%H:%M:%S')}] {cmd}\n")
sys.exit(0)
```

**第六步：建立 `.claude/settings.json`**

```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/log_cmd.py" }
        ]
      }
    ]
  }
}
```

**第七步：在 `my-test/` 目錄開 Claude Code，依序測試**

```
# 測試 /goal
/goal 目錄下有一個 test.txt，內容是 "goal works"
→ 驗證：cat test.txt

# 測試 skill
/hello
→ 驗證：看到「你好！Skill 運作正常。」

# 測試 sub-agent
請用 greeter agent 問候 Alice
→ 驗證：看到「你好，Alice！Sub-agent 運作正常。」

# 測試 hook
請跑 ls -la
→ 驗證：cat commands.log 裡出現 ls -la 的紀錄
```

---

## 常見問題

**Q: settings.json 改了沒有生效？**
`hooks` 設定是 session 啟動時讀取的，更改後需要重啟 Claude Code（`/exit` 再重開）。

**Q: Sub-agent 找不到我定義的 agent？**
如果是新增（之前不存在的目錄），需要重啟 Claude Code。如果是修改現有 agent，目前 session 會即時更新。

**Q: Skill 的 `name` 欄位和目錄名稱哪個才是 `/指令名稱`？**
**目錄名稱**才是 `/指令名稱`。`name` 欄位只是顯示標籤。

**Q: Hook 要怎麼確認有被觸發？**
在 hook script 最開頭加 `import sys; print("hook triggered", file=sys.stderr)` 暫時輸出到 stderr，Claude Code 會在 UI 顯示。確認後移除。

**Q: `disable-model-invocation: true` 是什麼意思？**
這個 skill 只能用 `/手動呼叫`，Claude 不會根據 `description` 自動觸發它。適合敏感操作（部署、清除資料）。

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — 各功能關係概覽（MCP、Plugins 等）
- [[AI 101 - Claude Code goal]] — `/goal` 的完整說明
- [[AI 101 - HTB Goal Prompt 設計指南]] — 把這四層用在 HTB 滲透測試的實例

## Sources

- [Keep Claude working toward a goal](https://code.claude.com/docs/en/goal)
- [Hooks Reference](https://code.claude.com/docs/en/hooks)
- [Create custom subagents](https://code.claude.com/docs/en/sub-agents)
- [Extend Claude with skills](https://code.claude.com/docs/en/slash-commands)
- [HackSynth: Planner-Executor-Summarizer Architecture](https://arxiv.org/abs/2412.01778)
