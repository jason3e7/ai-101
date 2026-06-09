---
title: AI 101 - BoxPwnr
tags: [ai, 工具, 資安, ctf, htb, pentest, 自動化, python, docker]
created: 2026-06-09
---

# AI 101 - BoxPwnr

> [!info]
> 開源 AI 自動化滲透測試框架，把 LLM 放進 Kali Linux Docker 容器讓它自主解 HTB / CTF。支援 15+ 平台、8 種 Solver 架構、20+ LLM，可用 `--resume-from progress.md` 銜接上次中斷的進度。

---

## 快速定位

| 維度 | BoxPwnr |
|---|---|
| 語言 | Python 3.10+ |
| 執行環境 | Docker（Kali Linux 容器） |
| 支援平台 | HTB、PortSwigger、XBOW、picoCTF、TryHackMe 等 15+ |
| 支援 LLM | Claude、GPT、DeepSeek、Grok、Gemini、Ollama 等 20+ |
| 授權 | MIT |

---

## 是什麼

BoxPwnr 是一個**評測與執行框架**，不自己定義「怎麼滲透」，而是把問題交給 LLM 在隔離的 Kali Docker 容器內自主解決。核心流程：

```
你下指令（target + model + solver）
     ↓
BoxPwnr 啟動 Kali Docker 容器
     ↓
LLM 規劃 → 執行指令 → 讀回 stdout → 繼續決策
     ↓
找到 flag / 超出限制 → 輸出報告
```

**三個核心設計讓它比直接用 Claude Code 跑更穩定：**

1. **Solver 分離**：不同的 agent 架構（single loop、planner-summarizer、claude_code）可以切換
2. **進度持久化**：`--generate-progress` 產生 `progress.md`，下次 `--resume-from` 重新注入
3. **成本與時間上限**：`--max-cost` 和 `--max-turns` 防止失控

---

## 安裝

```bash
# 需要：Python 3.10+、Docker（要先跑起來）
git clone --recurse-submodules https://github.com/0ca/BoxPwnr
cd BoxPwnr

# 安裝 uv（Python package manager）
curl -LsSf https://astral.sh/uv/install.sh | sh

# 安裝依賴
uv sync
```

第一次執行時會提示輸入 API key，自動存到 `.env`。

> [!tip]
> 第一次 build Docker 容器約需 10 分鐘。之後用 `--keep-container` 保留容器，後續啟動快很多。

---

## 基本使用

```bash
# 最簡單的起手式：對 HTB 機器跑
uv run boxpwnr --platform htb --target meow

# 指定 Claude 模型
uv run boxpwnr --platform htb --target meow \
  --model claude-sonnet-4-6

# 設成本上限（省錢必備）
uv run boxpwnr --platform htb --target meow \
  --model claude-haiku-4-5-20251001 \
  --max-cost 0.5

# 限制回合數（快速試水）
uv run boxpwnr --platform htb --target meow \
  --max-turns 30 --max-time 30
```

執行完成後，結果存在：

```
~/BoxPwnr-Traces/htb/meow/20260609_180000/
├── config.json          # 這次使用的設定
├── stats.json           # token 用量、費用、時間
├── conversation.json    # 完整 LLM 對話紀錄
├── progress.md          # 偵察摘要（--generate-progress 才有）
└── summary.md           # 解題過程（--generate-summary 才有）
```

---

## 進階

### Solver 選擇

BoxPwnr 把「LLM 架構」抽象成可換的 Solver，不同複雜度選不同的：

| Solver | 說明 | 適合場景 |
|---|---|---|
| `single_loop_xmltag` | 預設，指令包在 `<COMMAND>` tag | 一般 CTF / HTB |
| `single_loop` | 互動式 tool use | 需要多步驟工具呼叫 |
| `hacksynth` | Planner → Executor → Summarizer 三模組 | 複雜多步驟挑戰 |
| `claude_code` | 用 Claude Code 作為 agent | 你已有 Claude Code 環境 |
| `external` | 接外部工具（Claude Code、Aider 等） | 自訂工具鏈 |

```bash
# 用 HackSynth 架構（planner + summarizer 分離）
uv run boxpwnr --platform htb --target meow --solver hacksynth

# 用 Claude Code 作為 agent
uv run boxpwnr --platform htb --target meow \
  --solver claude_code --model claude-sonnet-4-6
```

### 中斷後繼續（最重要的功能）

```bash
# 第一次跑，加上進度記錄
uv run boxpwnr --platform htb --target meow \
  --generate-progress --max-cost 1.0

# 跑到一半卡住或超出預算後，從 progress.md 繼續
uv run boxpwnr --platform htb --target meow \
  --resume-from ~/BoxPwnr-Traces/htb/meow/20260609_180000/progress.md
```

`progress.md` 的結構是 BoxPwnr 框架設計的精華——它不只是 log，而是**結構化的狀態快照**：

```markdown
## 🔍 Discoveries
- Port 80: Apache 2.4.49 (vulnerable to CVE-2021-41773)
- /cgi-bin/ directory exists

## 🚫 Dead Ends (DO NOT RETRY)
- SSH brute force: blocked by fail2ban
- /admin login: no default creds work

## 🎯 Recommended Next Steps
- Try path traversal via CVE-2021-41773
```

這個格式也可以手動建立，搭配你自己的 Claude Code `/goal` 使用。

### 多平台支援

```bash
# PortSwigger Web Security Labs
uv run boxpwnr --platform portswigger \
  --target "SQL injection UNION attack"

# TryHackMe
uv run boxpwnr --platform tryhackme --target linux-fundamentals

# picoCTF
uv run boxpwnr --platform picoctf --target "general-skills"

# CTFd 自架賽事
uv run boxpwnr --platform ctfd \
  --ctfd-url https://ctf.example.com --target "Web Challenge"
```

### 自訂指示

```bash
# 加入額外的 prompt 指示
uv run boxpwnr --platform htb --target meow \
  --custom-instructions "Focus on web vectors first. Do not attempt SSH."
```

---

## 為什麼 BoxPwnr 比直接跑 /goal 更穩

| 問題 | BoxPwnr 的解法 |
|---|---|
| LLM 重複走死路 | `progress.md` 的「Dead Ends」區塊在 resume 時重新注入 |
| Context 滿了就忘事 | `--solver hacksynth` 有專門的 Summarizer 在每輪壓縮狀態 |
| 亂跑互動式指令卡住 | Docker executor 限制 non-interactive，timeout 自動截斷 |
| 不知道花了多少錢 | `--max-cost` 強制上限，`stats.json` 記錄每次費用 |
| 每次從頭來過 | `--resume-from progress.md` 銜接上次進度 |

---

## 成功率參考

基於 BoxPwnr 公開的 [boxpwnr.info](https://boxpwnr.info) 資料：

| 平台 | 成功率 | 樣本數 |
|---|---|---|
| HTB Starting Point | 100% | 25/25 |
| picoCTF | 99.8% | 502/503 |
| XBOW | 97.1% | 101/104 |
| HTB Labs | 51.0% | 268/526 |
| PortSwigger | 60.4% | 163/270 |
| TryHackMe | 44.8% | 213/477 |

---

## 常見問題

**Q: 不需要 HTB API token 就能跑嗎？**
需要，HTB 平台要在 `.env` 設 `HTB_TOKEN`。Starting Point 機器不需要額外的 VPN 設定，一般 labs 需要。

**Q: 沒有 Anthropic API key，只有 Claude Max 訂閱可以用嗎？**
不行，BoxPwnr 直接呼叫 Anthropic API，需要 API key。可以改用本地模型：`--model ollama:llama3.1`。

**Q: Docker 不是跑 ARM 嗎？（M1/M2 Mac）**
加 `--architecture amd64` 強制跑 x86_64，大多數資安工具只有 AMD64 版本。

---

## Sources

- [GitHub: 0ca/BoxPwnr](https://github.com/0ca/BoxPwnr)
- [BoxPwnr Traces（公開解題記錄）](https://boxpwnr.info)
- [PentestGPT GitHub（USENIX 2024）](https://github.com/GreyDGL/PentestGPT)
- [HackSynth arXiv 2412.01778](https://arxiv.org/abs/2412.01778)
