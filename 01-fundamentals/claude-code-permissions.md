---
title: AI 101 - Claude Code 權限模式與設定層級
tags: [ai, claude-code, permissions, security, settings, enterprise, 基礎]
created: 2026-07-03
---

# Claude Code 權限模式與設定層級

[← 回主頁](../index.md)

> [!info]
> 這篇講清楚兩件常被搞混的事：**（1）五種權限模式**分別讓 Claude 能做什麼、把關到哪、風險多高；**（2）四層設定層級**誰蓋過誰，為什麼企業帳號有些設定你改不動。

---

## 一、五種權限模式

在 Claude Code 裡輸入 `/config` 就能切換權限模式（permission mode），它決定 Claude 執行工具（改檔案、跑指令）前要不要先問你同意。

下面由**最保守 → 最放手**排列。

### 1. Plan mode（計畫模式）

Claude **只讀不寫**——能看檔案、搜尋、分析，但**完全不改任何東西、不跑會產生副作用的指令**。

- **用途**：讓 Claude 先研究、提出方案，你看過再決定要不要執行
- **產出**：一份計畫，等你按核准才進入實際執行
- **對應**：`--permission-mode plan`

> [!tip]
> 大型改動前先用 Plan mode 讓它規劃，是最安全的起手式。看過計畫沒問題再切換到執行模式。

### 2. Default（預設）

標準把關模式。**每個有副作用的操作**（改檔案、跑指令）都會**跳確認框**問你。

- 讀取類操作不問，寫入／執行類操作逐一問
- 最平衡，新手預設就用這個

### 3. Accept edits（接受編輯）

**檔案編輯自動放行**，但**執行指令（Bash 之類）還是會問**。

- Claude 改 code 不用你一直按同意
- 但要跑 `rm`、`git push`、安裝套件這種指令，仍會停下來確認
- **對應**：`acceptEdits`

> [!tip]
> 寫程式、重構時最常用——編輯很頻繁，指令才是風險點。把頻繁的低風險動作放行，只在真正危險處把關。

### 4. Auto mode（自動模式）

比 Accept edits 更放手。**編輯和大部分指令都自動執行**，只有**真正高風險**的操作才停下來。

- 適合你信任任務範圍、想讓它連續跑的時候
- 仍保留對極危險操作的最後把關

### 5. Don't ask（不要問）

**幾乎全部放行**，把關降到最低——這是最接近 `bypassPermissions` / `--dangerously-skip-permissions` 的模式。

- 什麼都不問，全速執行
- 常被企業管理者（見下方設定層級）鎖住禁用

> [!warning]
> 這個模式風險最高：Claude 可以無阻力地刪檔、跑任意指令、推 code。只在完全隔離的沙箱環境、且你清楚任務範圍時才用。

### 對照表

| `/config` 顯示 | 底層值 | 改檔案 | 跑指令 | 風險 |
|---|---|---|---|---|
| **Plan mode** | `plan` | ❌ 只讀 | ❌ 只讀 | 最低 |
| **Default** | `default` | 問 | 問 | 低 |
| **Accept edits** | `acceptEdits` | ✅ 自動 | 問 | 中 |
| **Auto mode** | `auto` | ✅ 自動 | ✅ 多數自動 | 中高 |
| **Don't ask** | `bypassPermissions` | ✅ 全放 | ✅ 全放 | 最高 🔒 常被企業鎖住 |

風險由上往下遞增，把關由上往下遞減。

---

## 二、四層設定層級

Claude Code 的設定不是只有一個檔案，而是**四層疊加**。同一個設定若多層都有，**上層蓋過下層**——這就是為什麼企業帳號有些選項你在 `/config` 裡改不動。

```
Enterprise Policy（管理員）      ← 最高優先，使用者無法覆蓋
        ↓
User Settings（/config）         ← 你的個人偏好
        ↓
Project Settings（.claude/settings.json）   ← 專案共用（會進 git）
        ↓
Local overrides（settings.local.json）      ← 只在你本機、不進 git
```

> [!warning]
> 「上層蓋過下層」指的是**優先級**，不是**執行順序**。Claude Code 實際是把四層合併（merge），衝突時採用優先級最高那層的值。所以就算你在 `settings.local.json` 寫了 `bypassPermissions`，只要 Enterprise Policy 禁用它，你的設定就會被無視。

### 逐層說明

#### 1. Enterprise Policy（企業政策）— 最高優先

由組織管理者透過 Anthropic Console 設定，**強制套用到組織內所有成員**，使用者無法覆蓋。

- **推送方式**：可能是本機 managed settings 檔，也可能綁在 OAuth token 上從後端雲端下推
- **本機檔案路徑**（若走檔案推送）：
  - macOS：`/Library/Application Support/ClaudeCode/managed-settings.json`
  - Linux：`/etc/claude-code/managed-settings.json`
  - Windows：`C:\ProgramData\ClaudeCode\managed-settings.json`
- **典型用途**：鎖定模型、強制關閉遙測、限制可用 MCP server、**禁用 `bypassPermissions`**

一個實際的 policy 片段長這樣：

```json
{
  "permissions": {
    "defaultMode": "default",
    "disableBypassPermissionsMode": "disable"
  }
}
```

`disableBypassPermissionsMode: "disable"` 就是把「Don't ask / 危險模式」整個關掉的那一行。被鎖住時，`claude --dangerously-skip-permissions` 會回：

```
Error: --dangerously-skip-permissions has been disabled by your organization policy
```

> [!info]
> 如果 policy 走**雲端下推**（不落地成本機檔案），一般使用者查不到完整內容——`find / -name managed-settings.json` 找不到檔案、也沒有 API 可列出。設計上就是不讓被管的人列舉管理者設了什麼。要看完整清單只能請管理者到 [console.anthropic.com](https://console.anthropic.com) → Organization → Settings。

#### 2. User Settings（使用者設定）

你的個人偏好，存在 `~/.claude/settings.json`，`/config` 改的就是這一層。跨所有專案生效。

```json
{
  "theme": "dark",
  "model": "claude-sonnet-4-6",
  "permissions": { "defaultMode": "acceptEdits" }
}
```

#### 3. Project Settings（專案設定）

存在專案根目錄的 `.claude/settings.json`，**會進 git**，用來讓整個團隊共用同一套專案規則（例如這個 repo 一律用某模型、某些指令白名單）。

```json
{
  "permissions": { "defaultMode": "auto" },
  "model": "sonnet"
}
```

#### 4. Local overrides（本機覆蓋）

存在專案的 `.claude/settings.local.json`，**不進 git**（通常被 `.gitignore` 排除）。用來放你個人在這個專案的臨時偏好，不想影響到隊友。

### 層級對照表

| 層級 | 檔案 | 範圍 | 進 git？ | 誰能改 |
|---|---|---|---|---|
| Enterprise Policy | `managed-settings.json`（系統路徑）或雲端 | 全組織 | ❌ | 只有管理者 |
| User Settings | `~/.claude/settings.json` | 你的所有專案 | ❌ | 你 |
| Project Settings | `.claude/settings.json` | 單一專案、全團隊 | ✅ | 專案成員 |
| Local overrides | `.claude/settings.local.json` | 單一專案、只有你 | ❌ | 你 |

---

## 三、兩個容易搞混的設定

### `permissions` vs `skipDangerousModePermissionPrompt`

這兩個名字很像，作用完全不同：

| 設定 | 管什麼 | 類比 |
|---|---|---|
| `permissions` | 執行每個操作**要不要問你**（就是上面五種模式）| 大門的門禁系統 |
| `skipDangerousModePermissionPrompt` | 進危險模式前**那張一次性同意書要不要簽** | 拆掉門禁前的「你確定？」告示 |

- `skipDangerousModePermissionPrompt: true` **不會授予任何權限**，只是省掉進入危險模式時「你確定要關掉所有把關嗎？」那個開場確認畫面。
- 如果組織 policy 已經禁用了 bypass 模式，這個設定就形同虛設——你根本進不了危險模式，那張同意書永遠不會出現。**Policy 層 > 個人設定**。

---

## 常見問題

**Q：我在 `/config` 選 Don't ask 被擋，怎麼查是誰擋的？**
先跑 `claude --dangerously-skip-permissions`，若回 `disabled by your organization policy` 就是企業 policy。再 `find / -name managed-settings.json 2>/dev/null` 找本機檔；找不到代表走雲端下推，只能請管理者查 Console。

**Q：專案設定和我的個人設定衝突，以哪個為準？**
Project Settings 優先於 User Settings。若還有 `settings.local.json`，它又優先於 Project Settings。但四者之上都壓不過 Enterprise Policy。

**Q：切到 Plan mode 後 Claude 說要改檔案卻沒動作？**
正常。Plan mode 只規劃不執行，你核准後切到執行模式（Default / Accept edits 等）它才會真的動手。

---

## 相關筆記

- [AI 101 - Claude Code 生態系](./claude-code-ecosystem.md) — Skills、Hooks、MCP 的關係
- [AI 101 - Claude Code goal 強制力 Hook](./claude-code-goal-enforcement-hooks.md) — 用 Hook 在權限之外再加一層行為控制

## Sources

- [Claude Code Settings — 官方文件](https://code.claude.com/docs/en/settings)
- [Claude Code IAM & Permissions — 官方文件](https://code.claude.com/docs/en/iam)
- [Manage Claude Code for your organization — 官方文件](https://code.claude.com/docs/en/setup)
