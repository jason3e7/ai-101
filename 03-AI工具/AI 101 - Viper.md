---
title: AI 101 - Viper
tags: [ai, 工具, 資安, 滲透測試, c2, red-team, llm, docker, 開源]
created: 2026-06-02
---

# AI 101 - Viper

> [!info]
> FunnyWolf 開發的開源紅隊對抗模擬平台（C2 框架），內建 LLM Agent，提供 100+ 後滲透模組、反追蹤、多人協作與視覺化操作介面。定位是 Cobalt Strike（$12,600/年）的免費替代方案。GitHub 5.1k ⭐，最新版 v3.1.11（2026-03-31）。

---

## 快速定位

| 維度 | Viper |
|---|---|
| 類型 | C2 框架 / 紅隊平台 |
| 語言 | TypeScript（前端）+ Python（模組） |
| AI 整合 | 內建 LLM Agent（OpenAI、DeepSeek） |
| 部署 | Docker（推薦）|
| 授權 | 開源 |
| 官網 | [viperrtp.com](https://www.viperrtp.com) |

---

## 是什麼

Viper 是一個有 Web UI 的 **C2（Command & Control）框架**，讓紅隊在一個介面裡完成：

```
偵察 → 建立 Payload → 部署監聽器 → 後滲透操作 → 資料外洩 → 報告
```

相比 Cobalt Strike，Viper 的差異在於：
- **免費**，且支援 Python 自訂模組
- **內建 LLM Agent**，可輔助決策、生成釣魚郵件
- **MCP 整合**，可呼叫 nmap、nuclei 等工具
- **支援 Android**（Cobalt Strike 不支援）

---

## 系統需求

- OS：Linux Kernel 5.X+（Ubuntu 22.04、Kali Linux 2024.4、Debian 11）
- RAM：≥ 2 GB 可用記憶體
- 硬碟：≥ 5 GB

---

## 安裝

### 方式一：自動安裝腳本（最快）

```bash
bash <(curl -fsSL https://viperrtp.com/install.sh)
```

### 方式二：Docker Compose（推薦生產環境）

```bash
# 1. 安裝 Docker
curl -fsSL https://get.docker.com | bash -s docker

# 2. 建立工作目錄
export VIPER_DIR=/root/VIPER
mkdir -p $VIPER_DIR && cd $VIPER_DIR

# 3. 設定密碼並啟動
export VIPER_PASSWORD=yourpassword
docker compose up -d
```

約 1 分鐘後可在 `https://your-server-ip:60000` 存取。

---

## 基本使用

### 登入

```
URL：https://your-server-ip:60000
帳號：root
密碼：你在安裝時設定的密碼
```

> [!warning]
> 防火牆需放行 port 60000 及後續 Handler 使用的端口。

### 介面概覽

- **Sessions**：管理所有已建立的連線（類似 msfconsole 的 sessions）
- **Handler**：設定監聽器（Listener）等待目標回連
- **Modules**：100+ 後滲透模組，按 MITRE ATT&CK 分類
- **File**：管理上傳 / 下載的檔案與戰利品（loot）
- **AI Agent**：與 LLM 互動，取得攻擊建議

---

## 進階

### LLM Agent 整合

Viper 支援多種 LLM 後端，會根據任務自動選擇最適合的模型：

| 任務類型 | LLM 功能 |
|---|---|
| 攻擊決策 | 根據目標資訊建議下一步模組 |
| 社交工程 | 生成針對性釣魚郵件 |
| 漏洞分析 | 解析掃描結果並建議利用路徑 |

### MCP 整合

Viper 內建 MCP Server，可讓 LLM 直接呼叫工具：

```
Viper LLM Agent
      ↓ MCP
nmap、nuclei（被動漏洞掃描）
```

### 反追蹤（Anti-Tracking）

內建多層代理與反追蹤機制，讓 C2 流量難以被威脅情報平台偵測。建議在部署後立即設定。

### 多人協作

支援多使用者 + 角色分權，適合紅隊多人同時作業。

### 自訂模組

用 Python 開發模組，放入模組目錄後直接在 Web UI 使用：

```python
# 模組範例結構
class PostModule:
    NAME = "我的模組"
    DESC = "模組說明"
    
    def run(self, *args, **kwargs):
        # 執行邏輯
        pass
```

---

## 與其他 C2 框架比較

| 框架 | 費用 | AI 整合 | 開源 | 特色 |
|---|---|---|---|---|
| **Viper** | 免費 | ✅ LLM Agent | ✅ | Web UI、Python 模組、Android |
| Cobalt Strike | $12,600/年 | ❌ | ❌ | 業界標準、生態最成熟 |
| Havoc | 免費 | ❌ | ✅ | 現代化架構、Evasion 強 |
| Metasploit | 免費 | ❌ | ✅ | 模組最多、社群最大 |

---

> [!warning]
> Viper 是功能完整的攻擊框架，**只在你有書面授權的目標上使用**。未授權使用是違法行為，且工具能力足以造成嚴重危害。

---

## Sources

- [GitHub: FunnyWolf/Viper](https://github.com/FunnyWolf/Viper)
- [Viper 官網](https://www.viperrtp.com/)
- [Viper 快速開始](https://www.viperrtp.com/guide/getting_start)
