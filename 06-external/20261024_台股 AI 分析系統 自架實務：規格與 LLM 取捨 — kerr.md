---
title: "台股 AI 分析系統 自架實務: 規格與 LLM 取捨"
tags: [ai, 外部觀點, 量化交易, 台股, 自架, 硬體規格, 本地llm, ollama, claude, anthropic-sdk]
source: https://github.com/kerr20801/taiwan-stock-ai-analyzer
author: kerr (@kerr20801)
created: 2026-10-24
---

# 台股 AI 分析系統 自架實務 — Standing It Up: Specs & the LLM Choice

[← 回主頁](../index.md) · 介紹篇見 [台股 AI 分析系統: 五模型集成 + 動態權重](20261022_台股%20AI%20分析系統：五模型集成＋動態權重的完整量化閉環%20—%20kerr.md)

> [!NOTE]
> 這是 [kerr20801/taiwan-stock-ai-analyzer](https://github.com/kerr20801/taiwan-stock-ai-analyzer) 的**自架實務篇**: 如果你真的想把這套建起來, 實務上會遇到什麼、要什麼機器、那顆本地 LLM 能不能換成 Claude 或乾脆拔掉。研究方法是直接讀原始碼 (README、`requirements.txt`、`config.example.yaml`、`.env.example`、`ai_decision_engine.py`、`llm_analyzer.py`、`main.py`), 不是看 README 猜。

> **TL;DR (EN):** Hands-on feasibility notes for self-hosting kerr's Taiwan-stock quant system. It installs (standard pip stack), but three gotchas: it's a personal project (placeholder clone URL, config to fill), one data source (`jiansoft.freeddns.org`) is the author's own home server with no auth — likely unreachable for others, swap or stub it, and Telegram + Ollama are both optional. Hardware: the five ML models are tiny (30-day sequences, 4 features — CPU, no GPU); the **only** spec driver is the optional local LLM (gemma4:e4b ~CPU-ok, up to phi4:14b ~12GB VRAM). The LLM is an **analysis/report layer, not the decision path** — the decision engine never calls it, and the author ships a "pure-math ML mode" for running without it. So you can (a) keep Ollama, (b) swap just the post-market diagnosis to the Claude API via one method (`_generate_response`), or (c) remove it almost losslessly.

## 能不能自己建 — Can You Stand It Up?

能，是標準 Python 專案 (`pip install -r requirements.txt`, 建議 Python 3.10+; TensorFlow 或 PyTorch 擇一、TA-Lib 選用會自動降級)。但有三個「不是一鍵就能跑」的坑:

1. **它是個人專案** (0 star, README 的 `git clone` 還留著 `your-username` 佔位符)。設定得自己補: `cp config.example.yaml config.yaml`、`cp .env.example .env` 再填。
2. **有一個資料源是作者的家用伺服器** ⚠️ 最大的坑。`data_sources/jiansoft_api.py` 指向 `https://jiansoft.freeddns.org` (freeddns = 動態 DNS 的家用機), **沒金鑰、也不是公開服務**, 給別人用很可能連不到或不穩。營收／籌碼那塊你大概得**自己換掉或 stub**。股價本身走 `yfinance` + `twstock` (免費、沒問題)。
3. **Telegram 與 Ollama 都是選填**: 沒 TG → 報告印在終端機; 沒 Ollama → 跳過 LLM 會診, ML pipeline 照跑 (見下節)。

## 要什麼機器 — Hardware

兩塊天差地遠:

**ML 模型 → 幾乎不吃資源。** 看 `config.example.yaml`: LSTM 只用 30 天序列、4 個特徵 (`close/volume/rsi/ma5_ratio`)、最多 365 天資料、50 epochs; 樹模型 100 棵。資料很小, 任何現代 CPU 幾秒到幾分鐘訓完一檔, **不需要 GPU**。

**本地 LLM (選填) → 唯一的規格變數。** 它一天只呼叫 LLM 幾次 (盤前、盤後各一份會診), **不是併發服務**, 所以不用想得太大:

| 跑法 | LLM 模型 | 記憶體需求 | 代表機器 |
|:---|:---|:---|:---|
| 最省 / 先跑起來 | 不開 LLM，或 `gemma4:e4b` 用 CPU | 16GB RAM | 任何現代筆電 / 桌機 |
| 順跑含會診 | `gemma4:e4b` ~ `llama3.1:8b` | 8–12GB VRAM 或 16–24GB 統一記憶體 | RTX 3060 12GB / 4060 Ti 16GB；Apple Silicon 16GB+ |
| 跑大模型會診 | `phi4:14b`（`config` 預設） | ~12GB+ VRAM 或 32GB RAM(CPU 慢) | 4070/4080 級 GPU；M-series 32GB |

因為它是**每日排程常駐** (crontab/systemd, 盤中 09:00–13:30 要在線), 機器最好是**開著的小主機**: Mac mini、NUC/mini-PC、舊桌機掛 Linux 都行。

> [!NOTE]
> kerr 自己寫過 [DGX Spark 記憶體頻寬牆](20260921_DGX%20Spark%20記憶體頻寬牆%20—%20kerr.md), 但那是**併發服務 30 個人**才撞的牆。這套是**單人、一天幾次批次**, 完全不用那種等級, 一張中階 GPU (甚至純 CPU + 小模型) 就夠, 頻寬牆咬不到。

## LLM 這塊的三條路 — Keep / Swap / Remove

先講最關鍵的發現: **那顆 LLM 是分析/報告層, 不是預測或決策層。** 證據: `ai_decision_engine.py` 裡**完全沒有 `llm`/`ollama` 呼叫**, 決策靠技術指標 + 規則情緒 (`bullish_ratio > 0.6` 這種) + 風險評分; README 說的「AI 60% + 技術 25% + 情緒 15%」裡的「AI」= **五個 ML 模型的預測**, 不是 Ollama。LLM 只負責把數字寫成人話 (盤前研判、盤後會診、部分操作建議句)。

### 路線 B: 換成 Claude — 單點替換

所有 LLM 呼叫收斂在 `llm_analyzer.py` 的 **`_generate_response(prompt, max_tokens)`** (盤後那支 `enhanced_llm_analyzer.py` 同構)。換它 = 換 provider, 上游的 prompt 組裝、`max_tokens`、規則預判全部不動。Python → 官方 `anthropic` SDK:

| Ollama 現在 | 換成 Claude |
|:---|:---|
| `requests.post(.../api/generate, json={model,prompt,options})` | `client.messages.create(model=..., messages=[{"role":"user","content":prompt}])` |
| 「只輸出操作建議」的指示混在 prompt | 抽到 `system=` 更穩 |
| `options.num_predict` | `max_tokens` |
| `options.stop` | `stop_sequences` |
| `response.json()['response']` | `response.content[0].text` |

```python
import anthropic
client = anthropic.Anthropic()   # 讀 ANTHROPIC_API_KEY

def _generate_response(self, prompt, max_tokens=None):
    max_tokens = max_tokens or self.max_response_length
    resp = client.messages.create(
        model="claude-haiku-4-5",          # 這用途短輸入短輸出高頻, Haiku 最合身
        max_tokens=max_tokens,
        system="你是台股操作助手，只輸出精簡操作建議，不要前言與總結。",
        messages=[{"role": "user", "content": prompt[:self.max_prompt_length]}],
        stop_sequences=["總結", "結論", "最後", "綜合"],
    )
    return resp.content[0].text.strip()[:max_tokens]
```

**兩個坑**:

1. **取樣參數要拿掉。** 原碼有 `temperature=0.2, top_p=0.7` —— 現行的 `claude-opus-5-5` / `claude-sonnet-5-5` 傳這些會 **400 拒絕**; 不傳即可, 或用 `claude-haiku-4-5` (它仍吃 `temperature`)。
2. **成本 + 隱私。** 本地 Ollama 的賣點是「零 API 成本、資料不出本機」; 換 Claude = 每次計費 + 資料上雲。若每檔股票、盤中每分鐘都叫, 帳單很有感。

> **jason3e7 的建議**: 不要整個拔掉 Ollama, 而是**加一個 `LLM_PROVIDER=ollama|claude` 開關** —— 日常／盤中高頻用本地小模型, **只有盤後那份「深度會診」** (輸出長、一天一次、品質要求高) 切 Claude, 最划算也不太犧牲隱私。模型選型的「先便宜、過門檻再升級」心法見 [模型升級階梯](../02-advanced/capabilities/model-escalation-ladder.md)。

### 路線 A: 拔掉 LLM — 幾乎無痛

作者**自己就支援**。`main.py` 自檢訊息直接寫: 「本地 Ollama AI 大腦 — 離線 (**若無安裝 Ollama 仍可跑純數理 ML 模式**)」。

| 會失去 (只有「人話」那層) | 完全保留 |
|:---|:---|
| 盤前的 LLM 跳空研判評論 | 五模型預測 + 每日動態權重 |
| 盤後的 LLM 深度會診報告文字 | 技術指標掃描 (RSI/MACD/布林) |
| 部分 narrative 操作建議句 (規則版還在) | 規則情緒 + 風險評分 + 買/賣/觀望決策 |
| | 模擬撮合、損益追蹤、Telegram 推播、所有數據 |

跳空缺口、三大法人、籌碼、類股這些數據照算照推, 只是少了 LLM 幫你寫成白話會診, 報告變「數字 + 規則」味。

### 怎麼關 (不改程式)

1. `config.yaml`: `features.enable_ai_analysis: false`
2. `.env`: 不要設 `OLLAMA_*` (留空)

這樣就跑純數理 ML 模式。要連 import 都不留才需動那兩支 analyzer, 通常不必。

> **一句話** (jason3e7): LLM 在這套裡是「把數字翻成人話的祕書」, 不是預測或決策引擎 —— 拔掉少一個祕書, 核心量化引擎毫髮無傷。把它當**量化工程範本**來讀, 收穫比當印鈔機多。

## Sources

- kerr20801/taiwan-stock-ai-analyzer (原始碼, MIT) — <https://github.com/kerr20801/taiwan-stock-ai-analyzer>
- 介紹篇: [台股 AI 分析系統: 五模型集成 + 動態權重的完整量化閉環](20261022_台股%20AI%20分析系統：五模型集成＋動態權重的完整量化閉環%20—%20kerr.md)
- ai-101: [模型升級階梯](../02-advanced/capabilities/model-escalation-ladder.md) (先便宜過門檻再升級)
- ai-101: [Ollama 指令教學](../04-local-llm/ollama-guide.md) · [輕量模型推薦](../04-local-llm/lightweight-models.md)
