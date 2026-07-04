---
title: AI 101 - Claude Code /goal 強制力 Hook
tags: [ai, claude-code, hooks, goal, enforcement, stop-hook, autonomous]
created: 2026-06-11
---

# Claude Code /goal 強制力 Hook

[← 回主頁](../../index.md)

> [!info]
> `/goal` 本身只是設定完成條件——它不能防止 Claude 中途放棄、跑偏或在 context 壓縮後忘記目標。這篇整理四種可以「強制」goal 行為的 hook 類型，含可直接執行的程式碼範例。

---

## 為什麼 /goal 需要 Hook 強化

| 問題 | 沒有 hook 的狀況 | Hook 解法 |
|---|---|---|
| Claude 認為自己「差不多完成了」就停 | Goal evaluator 偶爾誤判為已達成 | Stop hook 攔截，強制驗證後才放行 |
| Context 壓縮後忘記目標 | 壓縮完 Claude 不知道自己在幹嘛 | PreCompact hook 在壓縮前注入 goal 摘要 |
| Claude 開始做 goal 以外的事 | 沒有範圍限制 | PreToolUse hook 封鎖不相關工具 |

---

## 反直覺重點：Stop Hook 用 exit 0，不是 exit 2

```
# 常見誤解
exit 2  →  阻止停止（對），但 stderr 當成 error message 送給 Claude，stdout JSON 被忽略（髒）

# 正確做法
exit 0  +  JSON {"decision": "block", "reason": "..."}  →  乾淨阻止，reason 直接進 Claude context
```

---

## Hook 1：Stop — 防止 Claude 在達成前停下來

這是最核心的強制力機制。Claude 每次準備停止時觸發，可以阻擋。

### 程式碼

`.claude/hooks/goal_stop.py`

```python
#!/usr/bin/env python3
"""Stop hook: block Claude from stopping while goal is active."""
import json, sys

def is_goal_active() -> bool:
    # 最簡單的實作：讀一個旗標檔案
    try:
        with open(".goal_active") as f:
            return f.read().strip() == "1"
    except FileNotFoundError:
        return False

def main():
    try:
        json.load(sys.stdin)  # 讀 payload（此處不需要內容）
    except (json.JSONDecodeError, ValueError):
        sys.exit(0)

    if not is_goal_active():
        sys.exit(0)  # 沒有進行中的 goal，正常停止

    # Goal 還在跑：阻止停止，把目標說明送進 Claude context
    try:
        with open(".goal_objective") as f:
            objective = f.read().strip()
    except FileNotFoundError:
        objective = "（目標未記錄）"

    print(json.dumps({
        "decision": "block",
        "reason": (
            f"進行中的 Goal 尚未達成。\n\n"
            f"目標：{objective}\n\n"
            "繼續工作。完成後把達成的證明寫進 goal_result.txt，"
            "然後刪除 .goal_active 旗標檔案。"
        )
    }))
    sys.exit(0)

if __name__ == "__main__":
    main()
```

### 設定（`.claude/settings.json`）

```json
{
  "hooks": {
    "Stop": [
      {
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/goal_stop.py" }
        ]
      }
    ]
  }
}
```

### 啟動 / 停用 Goal

```bash
# 啟動：建立旗標檔案
echo "1" > .goal_active
echo "讓所有 pytest 測試通過" > .goal_objective

# 停用（目標達成後由 Claude 自己執行，或手動清除）
rm .goal_active
```

### 驗證

1. 建立 `.goal_active` 和 `.goal_objective`
2. 開 Claude Code，請它跑任何任務，等它想停下來
3. 確認它沒有停，而是繼續工作
4. 刪除 `.goal_active`，再讓 Claude 跑一次，確認這次可以正常停止

### Stop Hook 的 JSON 欄位速查

| 欄位 | 型別 | 效果 |
|---|---|---|
| `decision: "block"` | string | 阻止停止，`reason` 送進 Claude context |
| `reason` | string | 顯示給 Claude 的說明（會影響下一個 turn 的行動）|
| `continue: false` | boolean | 強制停止，`stopReason` 顯示給使用者 |
| `stopReason` | string | `continue: false` 時顯示給使用者的訊息 |
| `suppressOutput` | boolean | 隱藏 hook stdout（仍在 debug log）|

---

## Hook 2：PreToolUse — 封鎖 Goal 範圍外的操作

Claude 呼叫工具前觸發，可以拒絕不符合 goal 範圍的工具呼叫。

### 程式碼：封鎖危險指令

`.claude/hooks/goal_scope.py`

```python
#!/usr/bin/env python3
"""PreToolUse hook: block dangerous or out-of-scope tool calls."""
import json, sys, re

BLOCKED_PATTERNS = [
    r"rm\s+-rf\s+/",          # 刪根目錄
    r"git\s+push\s+.*--force", # 強制 push
    r"DROP\s+TABLE",           # SQL 刪表
]

def main():
    try:
        payload = json.load(sys.stdin)
    except (json.JSONDecodeError, ValueError):
        sys.exit(0)

    tool_name = payload.get("tool_name", "")
    tool_input = payload.get("tool_input") or {}
    command = tool_input.get("command", "")

    if tool_name == "Bash":
        for pattern in BLOCKED_PATTERNS:
            if re.search(pattern, command, re.IGNORECASE):
                print(json.dumps({
                    "hookSpecificOutput": {
                        "hookEventName": "PreToolUse",
                        "permissionDecision": "deny",
                        "permissionDecisionReason": (
                            f"指令被 goal scope hook 封鎖：{pattern}\n"
                            "這個操作超出目前 goal 的範圍或可能破壞環境。"
                        )
                    }
                }))
                sys.exit(0)

    sys.exit(0)

if __name__ == "__main__":
    main()
```

### `permissionDecision` 選項

| 值 | 效果 |
|---|---|
| `"deny"` | 阻止工具執行，`permissionDecisionReason` 送給 Claude |
| `"allow"` | 強制允許，跳過正常權限流程 |
| `"ask"` | 升級給使用者確認 |
| `"defer"` | 走正常權限流程（預設）|

---

## Hook 3：PreCompact — Goal 跨越 Context 壓縮存活

Context 快滿時，Claude Code 會自動壓縮對話。壓縮後如果沒有 goal 摘要，Claude 可能不知道自己在幹嘛。

### 程式碼

`.claude/hooks/goal_compact.py`

```python
#!/usr/bin/env python3
"""PreCompact hook: inject goal summary before context compression."""
import json, sys

def main():
    try:
        json.load(sys.stdin)
    except (json.JSONDecodeError, ValueError):
        sys.exit(0)

    try:
        with open(".goal_active") as f:
            if f.read().strip() != "1":
                sys.exit(0)
        with open(".goal_objective") as f:
            objective = f.read().strip()
    except FileNotFoundError:
        sys.exit(0)

    # 也可以讀 findings.md 把已完成的進度一起帶進去
    progress = ""
    try:
        with open("findings.md") as f:
            progress = f"\n\n已記錄的進度（findings.md 摘要）：\n{f.read()[:500]}"
    except FileNotFoundError:
        pass

    print(json.dumps({
        "hookSpecificOutput": {
            "additionalContext": (
                f"[壓縮前 Goal 快照]\n"
                f"進行中的目標：{objective}\n"
                f"這個目標在壓縮後仍然有效，請繼續朝目標工作。"
                f"{progress}"
            )
        }
    }))
    sys.exit(0)

if __name__ == "__main__":
    main()
```

---

## Hook 4：PostToolUse — 工具執行後注入進度反饋

工具跑完後觸發，不能阻擋，但可以把驗證結果塞進 Claude 下一個 turn 的 context。

### 程式碼：測試結果追蹤

`.claude/hooks/goal_verify.py`

```python
#!/usr/bin/env python3
"""PostToolUse hook: inject test results after pytest runs."""
import json, sys, subprocess

def main():
    try:
        payload = json.load(sys.stdin)
    except (json.JSONDecodeError, ValueError):
        sys.exit(0)

    tool_input = payload.get("tool_input") or {}
    command = tool_input.get("command", "")

    # 只在跑 pytest 後才注入
    if "pytest" not in command:
        sys.exit(0)

    # 快速跑一次，取得最新狀態
    result = subprocess.run(
        ["pytest", "--tb=no", "-q"],
        capture_output=True, text=True
    )
    summary = result.stdout.strip().split("\n")[-1] if result.stdout else "無法取得測試結果"

    passed = result.returncode == 0
    print(json.dumps({
        "hookSpecificOutput": {
            "additionalContext": (
                f"測試結果：{summary}\n"
                f"{'✅ Goal 達成條件滿足' if passed else '❌ 測試仍在失敗，繼續修復'}"
            )
        }
    }))
    sys.exit(0)

if __name__ == "__main__":
    main()
```

---

## 完整 settings.json

```json
{
  "hooks": {
    "Stop": [
      {
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/goal_stop.py" }
        ]
      }
    ],
    "PreToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/goal_scope.py" }
        ]
      }
    ],
    "PreCompact": [
      {
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/goal_compact.py" }
        ]
      }
    ],
    "PostToolUse": [
      {
        "matcher": "Bash",
        "hooks": [
          { "type": "command", "command": "python3 .claude/hooks/goal_verify.py" }
        ]
      }
    ]
  }
}
```

---

## 各 Hook 強制力總覽

| Hook | 可阻擋？ | 核心欄位 | 最適合用途 |
|---|---|---|---|
| `Stop` | ✅ | `decision: "block"` | 防止 Claude 在達成前停止 |
| `PreToolUse` | ✅ | `permissionDecision: "deny"` | 封鎖 goal 範圍外的操作 |
| `PreCompact` | ✅ | `additionalContext` | Goal 跨壓縮存活 |
| `PostToolUse` | ❌ | `additionalContext` | 注入工具執行後的驗證結果 |
| `SubagentStop` | ✅ | 同 Stop | 讓 sub-agent 也不能提早停止 |

---

## jthack/claude-goal 的實作邏輯

這個第三方 skill 就是靠 Stop hook 實作「不達標不停」：

1. 安裝時把 Stop hook 寫進 `~/.claude/settings.json`（user-level，對所有專案生效）
2. Goal 活著時：`exit 0` + `{"decision": "block", "reason": "..."}`
3. Goal 清除後：hook 變 no-op，正常停止
4. 防跑偏：計數 Stop-hook 繼續次數，超過 500 次（可設定）就放行並顯示警告

---

## 相關筆記

- [AI 101 - Claude Code goal](./goal.md) — `/goal` 完整說明（包含第三方 jthack/claude-goal skill）
- [AI 101 - Claude Code 行為結構設計](./behavior-design.md) — Hook 設定格式的 Hello World 入門

## Sources

- [Claude Code Hooks Reference](https://code.claude.com/docs/en/hooks)
- [Keep Claude working toward a goal](https://code.claude.com/docs/en/goal)
- [jthack/claude-goal](https://github.com/jthack/claude-goal)
