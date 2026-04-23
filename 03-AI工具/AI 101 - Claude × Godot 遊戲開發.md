---
title: AI 101 - Claude × Godot 遊戲開發
tags: [ai, godot, game-dev, gdscript, claude-code, mcp, testing, gut, automation]
created: 2026-04-23
updated: 2026-04-23
---

# Claude × Godot Engine 遊戲開發與自動化測試

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> **結論：可以，而且已有成熟工具鏈。**
> Claude 可以透過三種層次介入 Godot 開發，從輔助寫程式碼到全自動生成遊戲。
> 自動化測試用 GUT 框架，可在 headless 模式跑，接進 CI/CD。

---

## 三種介入層次

| 層次 | 做法 | 難度 | 自動化程度 |
|---|---|---|---|
| **① 輔助開發** | Claude Code 直接寫 GDScript / C# | ⭐ | 半自動（人審查） |
| **② 引擎整合** | Godot MCP — Claude 直接操控 Godot 編輯器 | ⭐⭐ | 高 |
| **③ 全自動生成** | Godogen — 一句話生成完整遊戲 | ⭐⭐⭐ | 全自動（含視覺 QA）|

---

## 層次一：Claude Code 輔助開發

最直接的方式：把 Godot 專案當一般程式碼庫，用 Claude Code 開發。

### 設定 CLAUDE.md（讓 Claude 了解 Godot 專案）

在 Godot 專案根目錄建立 `CLAUDE.md`：

```markdown
# 遊戲專案說明

## 技術棧
- Godot 4.4（GDScript）
- 單人平台動作遊戲

## 場景結構
- Main.tscn — 遊戲主場景
- Player.tscn — 玩家角色（CharacterBody2D）
- Enemy.tscn — 敵人（CharacterBody2D）
- UI.tscn — HUD

## 規範
- 所有腳本用 static typing（var speed: float = 300.0）
- 信號名稱用 snake_case
- 節點路徑用 $NodeName 或 @onready
- 測試放在 test/ 目錄，用 GUT 框架
```

### Sample：Player 腳本（Claude 生成）

```gdscript
# player.gd
class_name Player
extends CharacterBody2D

signal health_changed(new_health: int)
signal player_died

@export var speed: float = 300.0
@export var jump_velocity: float = -400.0
@export var max_health: int = 100

var health: int = max_health:
    set(value):
        health = clampi(value, 0, max_health)
        health_changed.emit(health)
        if health == 0:
            player_died.emit()

const GRAVITY: float = 980.0

func _physics_process(delta: float) -> void:
    if not is_on_floor():
        velocity.y += GRAVITY * delta

    if Input.is_action_just_pressed("ui_accept") and is_on_floor():
        velocity.y = jump_velocity

    var direction: float = Input.get_axis("ui_left", "ui_right")
    velocity.x = direction * speed

    move_and_slide()

func take_damage(amount: int) -> void:
    health -= amount
```

---

## 層次二：Godot MCP

Claude 透過 MCP 直接連接 Godot 編輯器，能**啟動專案、建立場景、抓 debug 輸出**。

### 安裝

```bash
# 安裝 Godot MCP 到 Claude Code
claude mcp add godot -- npx @coding-solo/godot-mcp

# 或手動設定（~/.claude/settings.json）
```

```json
{
  "mcpServers": {
    "godot": {
      "command": "npx",
      "args": ["@coding-solo/godot-mcp"],
      "env": {
        "GODOT_PATH": "/usr/local/bin/godot"
      }
    }
  }
}
```

### Claude 能做什麼（透過 MCP）

| 工具 | 功能 |
|---|---|
| `launch_editor` | 啟動 Godot 編輯器 |
| `run_project` | 執行遊戲並擷取輸出 |
| `get_debug_output` | 取得 console 錯誤與訊息 |
| `create_scene` | 建立新場景 |
| `add_node` | 在場景中新增節點 |
| `list_projects` | 列出本地 Godot 專案 |

### 使用範例（對 Claude 說）

```
幫我在 Godot 專案裡新增一個 Enemy 場景：
- 根節點：CharacterBody2D，命名 Enemy
- 子節點：CollisionShape2D（矩形碰撞體）
- 子節點：AnimatedSprite2D
- 加上一個 enemy.gd 腳本，讓敵人會左右巡邏
```

Claude 透過 MCP 直接操作，不需要你手動開編輯器。

---

## 層次三：Godogen（全自動生成）

> [!info]
> GitHub：[htdt/godogen](https://github.com/htdt/godogen)
> 發布日期：2026 年 3 月

一個完整的 AI Pipeline，從一句話描述自動生成可玩的 Godot 4 遊戲。

### 技術架構

```
使用者輸入：「生成一個 2D 平台跳躍遊戲，有三個關卡和敵人」
      ↓
Claude Code（Opus 4.7 / Sonnet 4.6）
  → 設計架構、產生 C# 程式碼、建立場景結構
      ↓
Gemini API
  → 生成 2D 美術資源
      ↓
Tripo3D
  → 2D 圖轉 3D 模型（3D 遊戲用）
      ↓
視覺 QA 迴圈（關鍵特色）
  → 啟動遊戲，截圖
  → Gemini Flash 分析截圖（有沒有材質缺失、物理錯誤）
  → Claude 修正問題
  → 重複直到通過
      ↓
輸出：可執行的 Godot 4 專案
```

### 安裝與使用

```bash
# 前置需求
# - Godot 4（.NET 版）
# - Python 3
# - API keys: Google AI, xAI, Tripo3D

# 設定環境變數
export GOOGLE_API_KEY="your_key"
export XAI_API_KEY="your_key"
export TRIPO3D_API_KEY="your_key"

# 安裝系統套件（Ubuntu/Debian）
sudo apt install vulkan-tools xvfb ffmpeg imagemagick

# 發布 skills 到你的遊戲 repo
git clone https://github.com/htdt/godogen
cd godogen
./claude/publish.sh ~/my-game-project

# 進入遊戲 repo，啟動 Claude Code
cd ~/my-game-project
claude   # 然後告訴 Claude 你想做什麼遊戲
```

> [!warning] 時間與資源
> 完整生成一款遊戲需要數小時，建議用 `tmux` 背景執行。
> 需要三個第三方 API（Google AI、xAI、Tripo3D），各自需要帳號和費用。

---

## 自動化測試：GUT 框架

**GUT（Godot Unit Test）** 是 Godot 最主流的測試框架，用 GDScript 寫測試。

### 安裝 GUT

```bash
# 方法一：從 Godot Asset Library（在編輯器內搜尋 "GUT"）

# 方法二：手動下載
# 下載 https://github.com/bitwes/Gut/releases
# 解壓後把 addons/gut 放進專案的 addons/ 目錄

# 啟用 Plugin：Project → Project Settings → Plugins → GUT → Enable
```

### 寫測試（Sample）

```gdscript
# test/unit/test_player.gd
extends GutTest

var player: Player

func before_each() -> void:
    player = Player.new()
    add_child_autofree(player)

# 測試初始血量
func test_player_starts_with_full_health() -> void:
    assert_eq(player.health, 100, "初始血量應為 100")

# 測試受傷
func test_take_damage_reduces_health() -> void:
    player.take_damage(30)
    assert_eq(player.health, 70, "受 30 傷後血量應為 70")

# 測試血量不會低於 0
func test_health_cannot_go_below_zero() -> void:
    player.take_damage(999)
    assert_eq(player.health, 0, "血量不應低於 0")

# 測試死亡信號
func test_player_emits_died_signal_at_zero_health() -> void:
    watch_signals(player)
    player.take_damage(100)
    assert_signal_emitted(player, "player_died")

# 測試治療
func test_heal_increases_health() -> void:
    player.take_damage(50)
    player.health += 20
    assert_eq(player.health, 70)
```

### 從命令列執行（headless 模式）

```bash
# 不開視窗，跑所有 test/ 目錄下的測試
godot --headless -s addons/gut/gut_cmdln.gd \
    -gdir=res://test \
    -ginclude_subdirs \
    -gexit

# 只跑特定測試檔
godot --headless -s addons/gut/gut_cmdln.gd \
    -gdir=res://test/unit \
    -gprefix=test_ \
    -gexit

# 輸出 JUnit XML（給 CI 用）
godot --headless -s addons/gut/gut_cmdln.gd \
    -gdir=res://test \
    -gjunit_xml_file=res://test_results.xml \
    -gexit
```

### GitHub Actions CI 設定

```yaml
# .github/workflows/test.yml
name: Godot Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      - name: Setup Godot
        uses: chickensoft-games/setup-godot@v2
        with:
          version: 4.4.0
          use-dotnet: false

      - name: Run GUT Tests
        run: |
          godot --headless -s addons/gut/gut_cmdln.gd \
            -gdir=res://test \
            -ginclude_subdirs \
            -gjunit_xml_file=res://test_results.xml \
            -gexit

      - name: Publish Test Results
        uses: dorny/test-reporter@v1
        if: always()
        with:
          name: GUT Test Results
          path: test_results.xml
          reporter: java-junit
```

---

## 完整工作流範例

### 用 Claude Code 開發一個小遊戲的完整流程

```
1. 在專案根目錄建立 CLAUDE.md（描述遊戲設計、場景結構、規範）

2. 對 Claude 說：
   「根據 CLAUDE.md 的設計，幫我實作 Player 腳本，
    包含移動、跳躍、受傷、死亡功能，並為每個功能寫 GUT 測試」

3. Claude 生成：
   - player.gd
   - test/unit/test_player.gd

4. 執行測試確認：
   godot --headless -s addons/gut/gut_cmdln.gd -gdir=res://test -gexit

5. 如果測試失敗，把錯誤輸出貼給 Claude：
   「測試失敗，錯誤如下：[貼上輸出]，幫我修正」

6. 重複 2–5，逐步完成各系統（敵人 AI、道具、UI、存檔）
```

> [!tip] 最有效的 prompting 策略
> - **設計文件當 context**：CLAUDE.md 寫清楚場景結構、節點命名、規範
> - **一次做一個系統**：不要說「幫我做整個遊戲」，說「先做 Player 的移動」
> - **測試先行（TDD）**：讓 Claude 先寫測試，再寫實作，更容易發現問題
> - **把 debug output 回饋給 Claude**：MCP 可以自動抓，或手動貼

---

## 實際使用心得

> [!warning] 已知風險：認知脫節
> 有開發者全程讓 Claude 寫程式碼，8 小時後發現**自己完全看不懂程式碼、無法獨立修 bug**。
> 建議：Claude 輔助開發，但你要理解每一段核心邏輯。
> AI 適合加速實作，不適合完全取代理解。

> [!tip] Godot 特別適合 AI 協作的原因
> - `.tscn` 場景檔是純文字，AI 可以直接讀懂和生成
> - GDScript 語法接近 Python，是 AI 訓練資料中的常見語言
> - 節點樹結構清晰，容易用自然語言描述（「在 Player 下加一個 CollisionShape2D」）

---

## 工具彙整

| 工具 | 用途 | 安裝 |
|---|---|---|
| **Godot MCP** | Claude 直接操控 Godot 編輯器 | `claude mcp add godot -- npx @coding-solo/godot-mcp` |
| **GUT** | GDScript 單元測試 | Godot Asset Library 或手動 |
| **GdUnit4** | 更完整的測試框架（含 C#、mocking）| Godot Asset Library |
| **Godogen** | 全自動生成完整遊戲 | `github.com/htdt/godogen` |
| **gdUnit4-action** | GitHub Actions CI 整合 | `github.com/MikeSchulze/gdUnit4-action` |

---

## Sources

- [Godogen GitHub（htdt/godogen）](https://github.com/htdt/godogen)
- [Godogen 介紹 — GIGAZINE](https://gigazine.net/gsc_news/en/20260317-godogen/)
- [Godot MCP（Coding-Solo/godot-mcp）](https://github.com/Coding-Solo/godot-mcp)
- [GUT：Godot Unit Test](https://github.com/bitwes/Gut)
- [GUT 文件](https://gut.readthedocs.io/)
- [GdUnit4 GitHub](https://github.com/godot-gdunit-labs/gdUnit4)
- [Run automated tests for your Godot game on CI — David Saltares](https://saltares.com/run-automated-tests-for-your-godot-game-on-ci/)
- [Building an RTS in Godot with Claude — DEV Community](https://dev.to/datadeer/part-1-building-an-rts-in-godot-what-if-claude-writes-all-code-49f9)
- [Godot MCP Pro — Asset Library](https://godotengine.org/asset-library/asset/4961)

---

## 相關筆記

- [[AI 101 - Claude Code 生態系]] — MCP、Skills、Hooks 概念
- [[AI 101 - Harness Engineering]] — AI Agent 執行基礎設施
- [[AI 101 - Context Engineering]] — 如何設計 CLAUDE.md 讓 AI 更懂專案
