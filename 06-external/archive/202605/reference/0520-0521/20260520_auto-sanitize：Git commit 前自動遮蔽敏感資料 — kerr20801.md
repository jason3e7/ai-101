---
title: auto-sanitize：Git commit 前自動遮蔽敏感資料
tags: [ai, 外部觀點, 資安, pii, git, pre-commit, 敏感資料, python]
source: https://github.com/kerr20801/auto-sanitize
author: kerr20801
created: 2026-05-20
---

# auto-sanitize：Git commit 前自動遮蔽敏感資料

> [!info]
> GitHub：[kerr20801/auto-sanitize](https://github.com/kerr20801/auto-sanitize)
> **一句話：** 不需要關鍵字清單，靠格式辨識自動偵測敏感資料，在 git commit 前遮蔽，也有瀏覽器版可直接用。

---

## 是什麼

**auto-sanitize** 是一個 Git pre-commit hook 工具，
在你 `git commit` 之前自動掃描、遮蔽程式碼裡的敏感資訊。

**特色：不靠關鍵字，靠格式識別。**
不需要你事先列出「哪些字要遮」，而是根據已知格式（IP 段、token prefix、webhook URL）自動判斷。

---

## 安裝

```bash
# 複製到專案根目錄
cp sanitize.py /path/to/repo/

# 安裝 pre-commit hook
cp pre-commit /path/to/repo/.git/hooks/
chmod +x /path/to/repo/.git/hooks/pre-commit
```

安裝後，每次 `git commit` 前會自動觸發掃描。

---

## 基本用法

```bash
# 預覽模式（只看，不修改）
python3 sanitize.py config.py --dry

# 實際遮蔽（自動建立 .bak 備份）
python3 sanitize.py config.py

# 手動指定要遮蔽的值
python3 sanitize.py config.py --mask "MyPassword123" --mask "secret-token"

# 遮蔽目前 OS 使用者名稱
python3 sanitize.py config.py --mask-user

# 略過私有 IP 偵測
python3 sanitize.py config.py --no-ip

# 掃描整個目錄
python3 sanitize.py ./scripts/ --dry
```

---

## 自動偵測的格式

| 類型 | 偵測方式 | 遮蔽後格式 |
|---|---|---|
| 私有 IP（RFC 1918）| 10.x / 172.16-31.x / 192.168.x | `[[PRIVATE_IP_xxxxxx]]` |
| GitLab / GitHub PAT | 已知 token prefix | `[[PAT_xxxxxx]]` |
| Telegram Bot Token | 格式辨識 | `[[TG_TOKEN_xxxxxx]]` |
| Webhook URL | `/hooks/` 路徑格式 | `/hooks/[[HOOK_TOKEN_xxxxxx]]` |
| 自訂網域 | `.sanitize.yaml` 設定 | `[[DOMAIN_xxxxxx]]` |
| 自訂字串 | `.sanitize.yaml` 設定 | `[[LITERAL_xxxxxx]]` |

> [!tip] Stable ID 設計
> 遮蔽後的 ID 是用 MD5 hash 產生的——**同一個值在不同檔案永遠得到相同的 tag**。
> 這讓你還原或交叉比對時不會混亂。

---

## 自訂設定（`.sanitize.yaml`）

```yaml
# 自訂要遮蔽的網域（包含子網域）
custom_domains:
  - yourcompany.com
  - internal.example.net

# 固定字串
custom_literals:
  - MyDatabasePassword
  - some-fixed-api-key

# 自訂 token prefix
custom_token_prefixes:
  - myapp-

# 關閉特定內建規則
disable_builtin:
  - PRIVATE_IP    # 不偵測私有 IP
  - HEURISTIC     # 不使用啟發式偵測

# 自訂 regex 規則
custom_replacements:
  - pattern: 'specific_pattern'
    replacement: '[REDACTED]'
    label: 'My secret'
```

---

## 瀏覽器版（無後端）

開啟 `sanitize.html`，可以直接在瀏覽器做即時遮蔽：
- 所有處理在前端進行，**資料不會傳出去**
- 可切換開關各個內建規則
- 可加自訂網域和字串

適合想快速處理一段文字但不想在系統上安裝工具的情況。

---

## 與 ai4privacy / OpenAI Privacy Filter 的差異

| | auto-sanitize | ai4privacy | OpenAI Privacy Filter |
|---|---|---|---|
| **偵測方式** | 格式 / regex | NLP 模型 | NLP 模型（Token 分類）|
| **需要 GPU** | 否 | 否（CPU 可跑）| 否 |
| **整合方式** | Git hook / CLI | Python API | Python API / CLI |
| **強項** | 程式碼、設定檔、已知格式 | 自然語言中的 PII | 自然語言中的 PII |
| **弱點** | 自由格式文字 | 程式碼中的 token 格式 | 程式碼中的 token 格式 |

> [!tip] 兩者互補
> auto-sanitize 適合保護程式碼和設定檔（有固定格式的敏感資料），
> ai4privacy / OpenAI Privacy Filter 適合處理自由文字（姓名、地址、電話等自然語言 PII）。

---

## 相關筆記

- [[AI 101 - PII Masking（隱私遮蔽）]] — ai4privacy 套件，自然語言 PII 偵測
- [[AI 101 - OpenAI Privacy Filter]] — OpenAI 官方 PII 模型
