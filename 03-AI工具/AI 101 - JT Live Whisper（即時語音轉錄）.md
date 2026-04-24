---
title: AI 101 - JT Live Whisper（即時語音轉錄）
tags: [ai, whisper, speech, transcription, translation, local, privacy, meeting]
created: 2026-04-24
updated: 2026-04-24
---

# JT Live Whisper — 本地即時語音轉錄與翻譯

[[AI 101 - 主頁|← 回主頁]]

> [!info]
> GitHub：[jasoncheng7115/jt-live-whisper](https://github.com/jasoncheng7115/jt-live-whisper)
> 授權：Apache 2.0
> 完全本地執行，不需要雲端、不需要訂閱、資料不離機。

---

## 什麼是 JT Live Whisper

一個整合多種 AI 模型的**本地語音工具包**，能同時處理：

- 即時轉錄（把說話的聲音即時轉成文字）
- 即時翻譯（英↔中、日↔中，邊聽邊翻）
- 錄音檔批次處理（mp3、wav、m4a、flac）
- 講者辨識（區分 A 說了什麼、B 說了什麼）
- 會議摘要（AI 整理重點）
- 浮動字幕（半透明視窗疊在畫面上）

**特色：** 捕捉系統音訊——任何 App 播放的聲音（Teams、Zoom、YouTube）都能即時轉錄，不限來源。

---

## 快速安裝

### macOS（一行搞定）

```bash
mkdir -p ~/Apps/jt-live-whisper && cd ~/Apps/jt-live-whisper
curl -fsSL https://raw.githubusercontent.com/jasoncheng7115/jt-live-whisper/main/install.sh -o install.sh
bash install.sh
```

### Windows（PowerShell 以系統管理員身分執行）

```powershell
mkdir C:\jt-live-whisper -Force | Out-Null; cd C:\jt-live-whisper
irm https://raw.githubusercontent.com/jasoncheng7115/jt-live-whisper/main/install.ps1 -OutFile install.ps1
powershell -ExecutionPolicy Bypass -File install.ps1
```

> [!tip] 安裝時間
> 腳本會自動下載模型、編譯 whisper.cpp，約需 **10–20 分鐘**。
> 磁碟空間：最小 ~3 GB，建議 ~8 GB，完整安裝 ~14 GB。

---

## 音訊設備設定（必要步驟）

### macOS — 安裝 BlackHole 虛擬音訊

BlackHole 把系統音訊「複製一份」給 JT Live Whisper 截取，同時你的喇叭照常出聲。

```bash
# 用 Homebrew 安裝 BlackHole
brew install blackhole-2ch
```

安裝後設定：

1. 打開 **Audio MIDI Setup**（Finder → 應用程式 → 工具程式）
2. 左下角 `+` → **建立多輸出裝置**
3. 勾選 **BlackHole 2ch** 和你的喇叭（AirPods / 內建喇叭）
4. 勾選 BlackHole 旁的「**時鐘主機**」
5. 系統偏好設定 → 音效 → 輸出 → 選「多輸出裝置」

> [!tip] 同時錄麥克風（開會兼轉錄自己說的話）
> 再建立一個「**聚合裝置**」，把 BlackHole 2ch 和你的麥克風合在一起，
> 啟動時加 `--mic` 參數即可雙軌錄製。

### Windows — 不需手動設定

WASAPI Loopback 自動偵測，通常不需要額外操作。
如果沒抓到，到音效設定啟用「**立體聲混音（Stereo Mix）**」。

---

## 啟動方式

### WebUI（推薦新手）

```bash
./start.sh --webui   # macOS
.\start.ps1 --webui  # Windows
```

瀏覽器開啟 `http://localhost:19781`，圖形化操作，手機也可以用。

### 互動式選單

```bash
./start.sh   # macOS
.\start.ps1  # Windows
```

進入選單，依提示選模式、引擎、模型。

### CLI 直接啟動（熟悉後最快）

```bash
# 英文 → 繁體中文即時翻譯（用本地 LLM）
./start.sh --mode en2zh --engine llm --llm-model qwen2.5:14b

# 雙語模式（系統音訊 + 麥克風同時轉錄）
./start.sh --mode en_zh --mic

# 離線處理錄音檔，含講者辨識 + 摘要
./start.sh --input meeting.mp3 --diarize --summarize

# 日文 → 中文
./start.sh --mode ja2zh --engine llm
```

---

## 常用 CLI 參數

| 參數 | 說明 | 預設 |
|---|---|---|
| `--mode` | 模式（見下表）| `en2zh` |
| `--asr` | 語音識別引擎 | 依設備自動選 |
| `-m / --model` | Whisper 模型大小 | 依設備自動選 |
| `-e / --engine` | 翻譯引擎（llm / nllb / argos）| `llm` |
| `--llm-model` | 指定本地 LLM 模型名稱 | 自動偵測 |
| `--input FILE` | 離線處理音訊檔案 | — |
| `--diarize` | 啟用講者辨識 | — |
| `--summarize` | 生成 AI 摘要 | — |
| `--mic` | 同時轉錄麥克風 | — |
| `--record` | 錄下音訊到本地 | — |
| `--denoise` | 背景降噪 | — |

### 支援的模式（`--mode`）

| 模式 | 說明 |
|---|---|
| `en2zh` | 英文 → 繁體中文翻譯 |
| `zh2en` | 中文 → 英文翻譯 |
| `ja2zh` | 日文 → 繁體中文翻譯 |
| `en_zh` | 英文 + 中文雙語顯示 |
| `ja_zh` | 日文 + 中文雙語顯示 |
| `transcribe` | 純轉錄（不翻譯）|

---

## 翻譯引擎選擇

| 引擎 | 品質 | 需要什麼 | 適合場景 |
|---|---|---|---|
| `llm`（推薦）| 最高 | 本地 LLM（Ollama 等）| 品質優先 |
| `nllb` | 中等 | 自動下載 NLLB 600M 模型 | 無 GPU、純離線 |
| `argos` | 普通 | 自動下載 Argos 模型 | 最輕量備用 |

### 本地 LLM 自動偵測支援清單

不需要手動設定，只要有在跑就自動找到：

| 服務 | Port |
|---|---|
| Ollama | 11434 |
| LM Studio | 1234 |
| Jan.ai | 1337 |
| vLLM | 8000 |
| LocalAI / llama.cpp | 8080 |
| LiteLLM | 4000 |

**推薦模型：**
- 翻譯：Qwen 14B 以上、Phi-4
- 摘要：需要較大模型（120B+ 效果最好）

---

## 語音識別引擎

| 引擎 | 平台 | 特色 |
|---|---|---|
| **whisper.cpp** | macOS | 即時轉錄主力，C++ 編譯，速度快 |
| **mlx-whisper** | Apple Silicon | 用 GPU 加速，比 whisper.cpp 更快 |
| **Moonshine** | Apple Silicon | 超低延遲，僅支援英文 |
| **faster-whisper** | Windows | CTranslate2 加速，Windows 主力 |

---

## 輸出檔案

每次 session 的結果儲存在 `logs/<session>/`：

| 檔案 | 內容 |
|---|---|
| `時間逐字稿_*.txt` | 時間戳記逐字稿 |
| `時間逐字稿_*.html` | 互動式逐字稿（可播音訊、跳轉）|
| `時間逐字稿_*.srt` / `.vtt` | 字幕檔 |
| `摘要_*.txt` / `.html` | AI 摘要 + 校正後逐字稿 |

---

## 硬體建議

### macOS（Apple Silicon）

| 記憶體 | 能做什麼 |
|---|---|
| 16 GB | 即時轉錄 + 離線批次處理 |
| 24 GB+ | 以上全部 + 在本機跑 Ollama LLM |

### Windows

| GPU | 效能 |
|---|---|
| 無 GPU | 可用但慢，建議搭配遠端 GPU server |
| RTX 4060（8 GB）| **CP 值最高，官方推薦** |
| RTX 4090（24 GB）| 旗艦效能 |

### 分散式架構（進階）

有多台機器時可以分工：
- **客戶端**（Mac / Windows）：負責音訊擷取和 UI
- **GPU 伺服器**（Linux + NVIDIA GPU）：負責語音識別和講者辨識

```bash
# 在 GPU 伺服器上啟動遠端 Whisper 服務
python remote_whisper_server.py

# 客戶端指定遠端伺服器
./start.sh --mode en2zh --server 192.168.1.100
```

伺服器離線時自動 fallback 回本機處理。

---

## 進階功能

### 字幕轉發到通訊工具

即時字幕可以同步推送到：Telegram、Slack、Discord、Teams、LINE

### 關鍵字警報

設定關鍵字，偵測到時觸發全螢幕通知 + 音效。

### 浮動字幕覆疊

PyQt6 半透明視窗疊在任何 App 上方，可拖移、自動縮放、滑鼠穿透（不影響底層操作）。

---

## 常見問題

> [!warning] 轉錄品質影響因素
> - 背景噪音（建議加 `--denoise`）
> - 麥克風與說話者的距離
> - 口音、快速說話、重疊說話
> 法律、醫療、財務用途請務必人工校對。

> [!warning] 講者辨識準確率
> 音訊品質差、說話者聲音相似、或人數過多時準確率下降。
> 手動指定說話者數量（`--speakers N`）通常比自動偵測準。

> [!tip] macOS 時鐘主機設為 BlackHole
> 多輸出裝置中，時鐘主機要選 BlackHole 而不是喇叭，
> 否則可能出現音訊漂移（轉錄逐漸對不上實際時間）。

---

## Sources

- [jasoncheng7115/jt-live-whisper GitHub](https://github.com/jasoncheng7115/jt-live-whisper)
- [whisper.cpp GitHub](https://github.com/ggerganov/whisper.cpp)
- [faster-whisper GitHub](https://github.com/SYSTRAN/faster-whisper)
- [BlackHole 虛擬音訊驅動](https://existential.audio/blackhole/)
- [Moonshine（超低延遲語音識別）](https://github.com/usefulsensors/moonshine)

---

## 相關筆記

- [[AI 101 - 輕量模型推薦]] — 翻譯 / 摘要用的本地 LLM 選型
- [[AI 101 - Ollama 指令教學]] — 設定本地 LLM 給翻譯引擎用
- [[AI 101 - Hermes Agent]] — 把轉錄結果接進 AI Agent 做後續處理
