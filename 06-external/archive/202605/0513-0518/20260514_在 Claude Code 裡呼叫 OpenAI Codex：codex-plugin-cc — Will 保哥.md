---
title: 在 Claude Code 裡呼叫 OpenAI Codex：codex-plugin-cc
tags: [ai, 外部觀點, claude-code, codex, openai, 跨平台, plugin, multi-model]
source: https://www.facebook.com/will.fans/posts/pfbid02Qoxrs9waRfvPcH4tHDFMtRp8RrMkLV2JQSpdNJhUMzRUrcBb8F2yfTprvmNbuYUQl
author: Will 保哥（保哥的技術交流中心）
created: 2026-05-14
---

# 在 Claude Code 裡呼叫 OpenAI Codex：codex-plugin-cc

> [!quote]
> "輸了就來當監工。" — 留言區神評論

---

## 是什麼

**codex-plugin-cc** 是一個讓你在 Claude Code 環境裡直接呼叫 OpenAI Codex 的 plugin。

安裝後，Claude Code 可以把程式碼審查、任務委派等子任務轉交給 Codex 執行，
兩個競品在同一個工作流裡協作——Will 保哥形容這是「OpenAI 的滲透策略」。

---

## 實際用法

安裝 `codex-plugin-cc` 後，在 Claude Code 對話裡：

```
# 把程式碼審查交給 Codex
請用 Codex 幫我審查這段 PR 的程式碼品質

# 任務委派
把這個重構任務交給 Codex 處理
```

Claude 當 orchestrator，Codex 負責執行特定子任務，結果回到 Claude 整合。

---

## 留言區的真實使用場景

社群裡已有人在跑**三模型並行配置**：

```
Claude Code + Codex + Gemini
        ↓
  依任務類型用設定檔路由
  （routing rules in config）
```

不同任務交給最擅長的模型，config 檔控制分流邏輯。

**被提到的實際整合點：**
- **Stop Hook** 整合 Codex 做 code review（每次 Claude 完成一段工作後自動觸發）
- 版本穩定性問題：1.0.0–1.0.3 有 timeout，建議等更新版本

---

## 為什麼值得關注

> [!tip] 這是跨平台 Agent 協作的真實落地案例
> 我們在 [[AI 101 - Subagent 使用與計費]] 裡討論了「方法一：shell 呼叫 Codex」和「方法三：MCP Bridge」，
> codex-plugin-cc 是更直接的第四種方式——**以 plugin 形式原生整合進 Claude Code**，
> 不需要自己寫 MCP Server，也不需要手動執行 shell 指令。

| 方式 | 難度 | 穩定性 |
|---|---|---|
| Shell 呼叫 Codex CLI | 低 | 中 |
| Python 呼叫 OpenAI API | 低 | 高 |
| 自建 MCP Server | 高 | 高 |
| **codex-plugin-cc（Plugin）** | **極低** | **目前有 bug，待觀察** |

---

## 延伸思考

- 當兩家競品的工具可以互相呼叫，「選哪家模型」的邊界是否正在消失？
- 三模型路由配置（Claude + Codex + Gemini）代表未來的工作流可能不是「選一個 AI」，而是「組一個 AI 團隊」
- OpenAI 主動讓 Codex 能被 Claude Code 呼叫，是搶市占還是真的相信互通性更重要？

---

## 相關筆記

- [[AI 101 - Subagent 使用與計費]] — 跨平台 Agent 協作的方法比較（方法一～三）
- [[AI 101 - Claude Code 生態系]] — Plugin、MCP、Skills 的架構關係
