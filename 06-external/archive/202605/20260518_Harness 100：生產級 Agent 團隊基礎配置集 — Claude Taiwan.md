---
title: Harness 100：生產級 Agent 團隊基礎配置集
tags: [ai, 外部觀點, claude-code, harness, agent-team, 生產環境, 配置]
source: https://www.facebook.com/groups/1224997379198346/posts/1289738669390883/
author: Claude Taiwan 社群
created: 2026-05-18
---

# Harness 100：生產級 Agent 團隊基礎配置集

> [!info]
> 來源：Claude Taiwan Facebook 社群
> **一句話：** 一套專為 Claude Code 設計、具備生產環境應用水準的代理團隊基礎配置集合。

---

## 是什麼

**Harness 100** 是針對 Claude Code 開發的 Agent 團隊配置套件，
定位是「生產環境等級」——不是實驗性的，而是可以直接落地使用的基礎建設。

> [!info] 原文描述
> "Harness 100 是一套具備生產環境應用水準的代理團隊基礎配置集合"

---

## 背景概念

Harness Engineering 的核心公式：

```
Agent = Model + Harness
```

Harness 決定了 Agent 的可靠性、安全性與效率，
而 Harness 100 的目標是提供一個「開箱即用」的 Claude Code Agent 團隊起點。

Claude Code 的 Harness 通常包含五層：

| 層次 | 對應元件 |
|---|---|
| Memory | `CLAUDE.md`（專案記憶）|
| Tools | MCP Server |
| Permissions | `settings.json` |
| Hooks | PreToolUse / PostToolUse |
| Observability | Session logs |

---

> [!warning] 筆記待補充
> 此貼文內容有限，後續有更多細節（安裝方式、配置範例、GitHub 連結）再補充。

---

## 相關筆記

- [[AI 101 - Harness Engineering]] — Harness 的完整概念與設計原則
- [[AI 101 - Claude Code 生態系]] — Skills、Hooks、MCP 的架構關係
- [[AI 101 - Subagent 使用與計費]] — Agent 團隊的協作與計費
