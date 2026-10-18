# 模型升級階梯：行政人式的 Claude 選型 — The Model Escalation Ladder

> [!TIP]
> 別預設一律用最強的模型。**先用便宜的試，設一條「過得了就收手」的門檻，不過才往上換。** 這篇講的是拿 Claude 時具體怎麼做：哪個檔位、用什麼指令換、門檻怎麼定、升級順序排哪個先。

> **TL;DR (EN):** Don't default to the top model. Satisfice: run the cheap tier first, gate on a verifiable threshold, and climb only when it fails. This note is the hands-on Claude playbook — the four tiers, the commands to switch (`/model`, `/fast`, effort, subagent model), how to set the bar, and what order to escalate in.

```markdown
# 模型升級階梯：先用便宜的試, 過門檻就收手, 不過才往上換
* 為什麼要階梯
  * 鋸齒前沿 + 價差, 一律最強是浪費
  * 一律最省又會卡關
  * 折衷 = 升級階梯
* Claude 四個檔位
  * Haiku 機械活
  * Sonnet 日常主力
  * Opus 難題
  * Fable 前沿
* 怎麼換 (實務)
  * /model 與別名
  * /fast 加速
  * effort 與 adaptive thinking
  * subagent 指定模型
* 門檻與升級順序
  * 門檻要可跑可驗
  * 先加 effort 再換模型
  * 重試有上限
* 什麼時候別省
  * 高風險不可逆
  * 難驗的任務
  * 門檻太鬆會蓋章
```

## 是什麼 — Satisficing, Not Maximizing

經濟學有兩種人：**經濟人**追求最佳解（maximizing），把所有選項都算過才挑最好的；**行政人（Administrative Man）**用的是**滿意即可（satisficing）**—先設一條「夠用就好」的門檻（抱負水準），拿手邊最省的選項試，過了就停，不過才往上找。這是 Herbert Simon 的「有限理性」。

把它套到選模型，就是這篇的主張：

> 「用**行政人的概念**選模型：先用舊的／便宜的，測幾次達不到最低門檻，再換模型。」—— jason3e7

理論到這裡就夠了（這不是理論筆記）。值得知道的是：這套在 LLM 工程早有人做成系統，叫 **model cascade**（最有名的是 Stanford 的 FrugalGPT，2023）—便宜模型先跑、打個分數、不過門檻才升級，實測能比「永遠用最強」省下最多 ~98% 成本、品質不掉。差別只在：FrugalGPT 用一個訓練過的小模型自動打分，我們**手動**用「跑得起來、驗得了」的判准當那個門檻。

為什麼要階梯，而不是挑一端站死：

- **一律用最強**：貴、慢，而且很多任務（改個字、搜個檔、套個樣板）Haiku 就做完了，多花的全是浪費。
- **一律用最省**：便宜模型會卡在你看不見的地方——[鋸齒狀前沿](../limits-and-verification/llm-limitations.md)讓它在某些題目突然離譜，而且它的錯**更難被你抓到**。
- **階梯**：預設踩在低檔，用門檻接住失敗，只在需要時往上爬一格。省的是常態，貴的是例外。

## Claude 的四個檔位 — The Four Tiers

由省到強、由快到慢（截至 2026-10，Claude 5 世代）：

| 檔位 | 模型 | 適合 | 心法 |
|:---|:---|:---|:---|
| 1 | **Haiku 5.5** | 機械活：分類、抽取、改字、搜檔、格式轉換、subagent 的廣度掃描 | 判准清楚、吃 pattern 的事，先丟它 |
| 2 | **Sonnet 5.5** | 日常主力：一般 coding、改 bug、寫筆記、多數 agent 回合 | 90% 的事的預設檔 |
| 3 | **Opus 5.5** | 難題：複雜重構、架構決策、難 debug、長鏈推理 | Sonnet 連試兩次沒過才上 |
| 4 | **Fable 5.1** | 前沿：最硬的推理、長時程 agent、別人都卡住的題 | 真的卡住、且值得那個成本才動 |

> [!NOTE]
> 絕對價格會一直變，這裡只記**相對順序**（Haiku < Sonnet < Opus < Fable，愈強愈貴愈慢），選型時搭 [模型費用與效果比較](../../01-fundamentals/models/model-cost-comparison.md) 看當下數字。能力面的「哪種任務該用多強」另見 [AI 怎麼知道該用哪種能力](how-ai-picks-capability.md) 與 [六種能力執行手冊](capabilities-playbook.md)。

### 為什麼表格鎖在 5.5，不留 5.0 或 4.8 — Why Pin to 5.5

常見問題：「我手上還能用 Opus 5.0 跟 4.8，為什麼要跟著換 5.5？」

**短答**：5.5 對 5.0 是**全面 Pareto 升級**（又便宜又強又快），升級階梯的「踩低檔」精神，到 5.5 正好是最低檔；繼續用 5.0 等於自願多付錢、拿舊能力。

**Opus 5.5 vs 5.0**（2026-09-22 發布，只隔 5.0 兩個月）：

| 指標 | Opus 5.0 | Opus 5.5 | 差距 |
|:---|---:|---:|:---|
| 輸入 token 單價 | $5 / M | **$4 / M** | **−20%** |
| 輸出 token 單價 | $25 / M | **$20 / M** | **−20%** |
| Prompt cache 讀取 | $0.50 / M | **$0.20 / M** | **−60%** |
| Terminal-Bench 4.0（agentic coding） | 52.3% | **66.4%** | **+14.1pp** |
| SWE-bench Pro | 基準 | **+10.7 分** | — |
| OSWorld 2.0（computer use） | 74.0% | **81.8%** | **+7.8pp** |
| 輸出速度 | 基準 | **+30%** | — |

加上 adaptive thinking **預設 effort 從 high 降到 medium**，單次 call 的 raw token 用量再降 **25–40%**。等於同一題：**更便宜、更快、分數更高**，這是罕見的全面 Pareto 升級。

**什麼時候用舊版（5.0 / 4.8）還是有理由**：

- **Reproducibility**：跟 2026 上半年的 benchmark／既有實測對照時，換版本就不是對照組了
- **Prompt / harness 已為舊版調校**：既有的 effort 設定、CLAUDE.md、hook 都針對 4.x 或 5.0 試過，換 5.5 要重跑一輪校正（特別是 default effort 改了，以前「夠深」的題現在想得淺）
- **邊界行為差**：refusal、tone、safeguard 觸發點，不同版本不同；壓過的 edge case 換版本可能再冒出
- **審計 / 合規**：鎖版本是審計要求，升版要走流程

**現狀（2026-10）**：

- **5.5 系列全線齊了**：Opus 5.5（2026-09-22）＋ Sonnet 5.5 ＋ Haiku 5.5（2026-10-07）＋ Fable 5.1
- **5.0 / 4.8 仍可用**：Anthropic 承諾退役前 60 天公告，Opus 5.0 / 4.8 的 tentative retirement 都在 2027
- **預設該用新**：升級階梯的精神是「踩在最便宜檔位」，既然 5.5 比 5.0 又便宜又強，新預設就該是 5.5

> **一句話**：不是「不能用 5.0 / 4.8」，是「用 5.0 / 4.8 當預設沒好處」。要麼為了可重現性鎖舊版，要麼直接用 5.5。

## 怎麼換 — The Levers in Claude Code

實務上「升級」有**三種**動作，成本由低到高：**加深思考（effort）< 換更大模型 < 開 opusplan／頂規**。先學會每個怎麼按。

**換模型 `/model`**：在 session 裡輸入 `/model` 開選單，或直接帶別名切換並記成預設：

```
/model haiku      # 切到 Haiku
/model sonnet     # 切到 Sonnet
/model opus       # 切到 Opus
/model fable      # 切到 Fable
/model opusplan   # Opus 負責「規劃」, Sonnet 負責「執行」—階梯自動化的現成組合
/model default    # 清掉覆寫, 回帳號預設
```

**加速而非降級 `/fast`**：Fast mode 用的仍是 **Opus**、只是輸出更快，**不是**偷偷換成小模型。要「又強又快」時用它，不要誤以為它在省模型。

**加深思考（先於換模型）**：很多時候便宜模型不是不行，是**想得不夠深**。先把 effort 調高，常常比換模型更省：

```
# effort 五檔: low / medium / high / xhigh / max
CLAUDE_CODE_EFFORT_LEVEL=high     # 這個環境變數的優先序最高
CLAUDE_CODE_EFFORT_LEVEL=max      # 不設上限、想到飽, 留給最硬的題
CLAUDE_CODE_EFFORT_LEVEL=auto     # 交回模型自己決定
```

- **Adaptive thinking**：2026 起模型會**自己決定每回合想多久**，不再吃固定預算。好處是省，壞處是難題上它可能**想得太淺**（社群有記錄到因此冒出假的 commit SHA、不存在的套件名）。
- 要它**每回合都想好想滿**，關掉自適應、配上 max：

```
CLAUDE_CODE_DISABLE_ADAPTIVE_THINKING=1
CLAUDE_CODE_EFFORT_LEVEL=max
```

**讓 subagent 踩低檔**：把廣度、機械的活丟給 Haiku／Sonnet 的 subagent，主線留強模型。開 subagent 時指定 `model`（細節與計費見 [Subagent 使用與計費](../engineering/subagent-usage-and-billing.md)）。這是「主貴、僕便宜」的省法。

### Remote Control（Web / 手機）切 `/model` 會遇到的坑 — Switching Models Remotely

Claude Code 2026-02 推出 **Remote Control**（本地 session 跑 `/remote-control` 或 `/rc`，從 claude.ai / 手機 app 控制同一個 session）。本質是把本地 session **stream 到 web / 手機**，真正跑的還是本地那台。好處是出門也能看 agent 進度、下新指令、切模型。

**但切 `/model` 這件事，遠端跟本地有三種行為差異**：

| 情境 | web / 手機行為 |
|:---|:---|
| **`/model opus`（帶模型名）** | 直接套用，不需回終端。但要 Claude Code **2.1.202 以上**（2026-08 之後的版本），之前的版本遠端打 `/model <name>` 會當普通訊息送給模型。 |
| **`/model`（不帶參數）** | 本地會跳出互動 picker 讓你選。**picker 遠端跑不起來** —— 要回原本終端按方向鍵選。這是 jason3e7 踩的那個坑。 |
| **外部觸發的 remote execution session**（非 `/rc` 的那種） | 已知 bug (issue #68402)：所有 slash command 都當普通訊息 fall through，`/model`、`/compact`、`/clear` 都失效。 |

**實務處理**：

- **遠端切模型永遠帶名字**：`/model haiku`、`/model sonnet`、`/model opus`、`/model fable`、`/model opusplan`、`/model default`
- **別用 `/model` 單打開 picker**：遠端 session 這樣做會卡住，要起身走回電腦前
- **版本要新**：舊版 (< 2.1.202) 的 Claude Code，遠端打 `/model opus` 會變成「跟 AI 說 '/model opus'」，不會真的切

**其他遠端限制順便一次講**：

- **一個 session 只能一個遠端 viewer**：兩支手機同時看同一個 session 不行
- **本地終端要保持開著 + 連線**：本地關掉或網路斷 10 分鐘以上，遠端就斷
- **picker 類指令都只能本地**：`/mcp`、`/plugin`、`/resume`、`/rewind` 跟 `/model`（不帶參數）一樣，都開 picker、都只能本地按
- **Bypass permissions 模式不能遠端開**：手機 / web 只能選 Manual / Accept edits / Plan / Auto 四種，要跳過許可必須本地啟動時先設
- **訂閱 / 設備**：Remote Control 僅 Pro / Max 可用；公司通常開 Trusted Devices，要先驗證設備

> [!TIP]
> 跟升級階梯的關係：Remote Control 不改變選型，但會改變**你願意切多頻繁**。手機按一下 `/model opus` 比回電腦順，所以升級成本變低，容易過度升級。設定門檻這一條還是要守，不要因為「反正手機一按就好」就直接跳頂。

### claudecodeui — 第三方 Web UI 切模型 — Third-Party UI Model Switching

jason3e7 試用 [claudecodeui](https://github.com/siteboon/claudecodeui) 後確認：**當前 session 跑的是 `claude-sonnet-4-6`（Sonnet 4.6）**。

claudecodeui 是一個開源（AGPL-3.0）自架 Web UI，把 Claude Code、Cursor CLI、OpenAI Codex 統一在同一個介面。切模型的方式跟原生 Remote Control 根本不同——**它不靠 `/model` slash command，而是走自己的 REST API**：

```
POST /api/providers/claude/sessions/:sessionId/active-model
{ "model": "opus" }
```

所以原生 Remote Control 的三個坑（picker 遠端跑不起來、externally-triggered session slash command fall through、版本要 ≥ 2.1.202）**在 claudecodeui 裡全部消失**——UI 裡有個 dropdown，點就換，不需要打指令，不需要回終端。

**claudecodeui 支援的 Claude model 別名**（跟 `/model` 的別名相同）：

| 別名 | 說明 |
|:---|:---|
| `default` | 帳號推薦預設 |
| `best` | Fable 5 或最新 Opus |
| `fable` | Fable 5，最頂 |
| `sonnet` | 日常主力 |
| `sonnet[1m]` | Sonnet + 1M context |
| `opus` | 複雜推理 |
| `opus[1m]` | Opus + 1M context |
| `haiku` | 快速省錢 |
| `opusplan` | Opus 規劃 + Sonnet 執行 |

effort 支援：`low / medium / high / xhigh / max / ultracode`（Haiku 不支援 effort 欄位）。

**已知限制**：Claude Code 的 `getSupportedModels()` 在 claudecodeui 裡被停用（呼叫會產生多餘 session），所以 claudecodeui 用**硬編碼 catalog**，不動態偵測你帳號能用的模型。如果帳號沒有 Fable，選了也無法用——要自己確認訂閱權限。

> [!TIP]
> claudecodeui 切模型比原生 Remote Control 順（純 UI 操作、無 picker / slash command 問題），但它是自架工具，要自己管更新與資安。若只是想遠端切模型，原生 Remote Control + `/model <name>` 已夠，不一定要架 claudecodeui。

## 門檻與升級順序 — The Bar and the Climb

**門檻要「可跑可驗」，不能靠感覺。** 這是整套的關鍵——門檻定得鬆，等於沒有階梯，你只是在對便宜模型的錯誤蓋橡皮章。好的門檻是機器能判、你一眼能信的東西：

- 程式：**compile 過 / test 綠 / lint 乾淨 / diff 對得上 / benchmark 達標**。
- 文字或分析：先[收整再驗](../limits-and-verification/converge-before-verify.md)，用可逐條核對的 checklist，而不是「讀起來順不順」。
- 一般原則見 [AI 產出怎麼驗](../limits-and-verification/verifying-ai-output.md)：外部證據、跨模型獨立驗算、原子事實分解。

**升級順序（成本由低到高，依序往上，不要一步跳頂）：**

1. **同檔位重試一次**——但要給**新資訊**（補 context、貼錯誤訊息、縮小範圍），原封不動重跑通常只會得到同一個錯。
2. **加 effort / 關 adaptive**——先讓現在這個模型想深一點。
3. **換上一檔模型**（Haiku→Sonnet→Opus）。
4. **開 opusplan 或上 Fable**——留給真正硬的題。

**重試要有上限。** 訂死「同一步最多 N 次（通常 2–3）就往上爬一格」，否則你會在便宜檔位無限打轉，耗掉的 token 比一開始就用貴的還多。這條界線本身就是 satisficing：不是找最佳打法，是**設一個夠用的停損點**。

> [!TIP]
> 把門檻寫進可自動跑的地方（test、CI、[`/goal`](../../01-fundamentals/claude-code/goal.md) 的完成條件），升級就能半自動：達標才停、連試不過才換檔，不用你每回合盯著決定。

## 什麼時候別省 — When Not to Satisfice

階梯是常態，但有三種情況**直接跳頂、不要在便宜檔位浪費回合**：

- **高風險、不可逆**：動 production、改 migration、刪資料、牽錢。省下的那點錢，遠不值一次便宜模型的低級錯。
- **難驗的任務**：你根本沒有好門檻接得住錯誤時（例如難以查證的判斷），階梯的前提就不成立——與其用測不準的門檻放行便宜模型，不如直接上強模型。
- **門檻本身很鬆**：當「過關」只是你草草掃一眼，便宜模型的錯會被你**蓋章放行**（rubber-stamp）。這正是 [AI 用久了會鈍化](../limits-and-verification/ai-atrophy.md) 說的驗證疲勞——階梯反而放大了風險。

一句話收尾：**行政人省的是力氣，不是品質。** 門檻接得住，就放心踩低檔；門檻接不住，就老實上強模型。判斷「門檻到底可不可靠」，才是這套唯一需要你動腦的地方。

## Sources

- Herbert A. Simon — *Administrative Behavior*（行政人 / satisficing / bounded rationality 的原始出處）— <https://en.wikipedia.org/wiki/Satisficing>
- FrugalGPT: How to Use LLMs While Reducing Cost and Improving Performance (Chen, Zaharia, Zou; Stanford, 2023) — <https://www.alphaxiv.org/abs/2305.05176>
- Model Cascade (FrugalGPT) — agent orchestration 模式卡 — <https://agentfieldbook.org/design/orchestration/frugalgpt/>
- Claude Code Model configuration（`/model`、別名、opusplan）— <https://code.claude.com/docs/en/model-config>
- Opus 4.7: The Five Effort Levels in Claude Code Explained — <https://anthonymaio.substack.com/p/opus-47-the-five-effort-levels-in>
- Effort, Thinking, and How Claude Opus 4.7 Changed the Rules — <https://www.ibuildwith.ai/blog/effort-thinking-opus-4-7-changed-the-rules/>
- Introducing Claude Opus 5.5 (Anthropic, 2026-09-22) — <https://www.anthropic.com/claude-opus-5-5>
- Claude Opus 5.5 Benchmarks Explained (Vellum) — <https://www.vellum.ai/blog/claude-opus-5-5-benchmarks-explained>
- Claude Opus 5.5: Benchmarks, Pricing, vs Fable 5.1 (ComputingForGeeks) — <https://computingforgeeks.com/claude-opus-5-5-released-features-benchmarks/>
- Model deprecations — Claude Platform Docs — <https://platform.claude.com/docs/en/about-claude/model-deprecations>
- Continue local sessions from any device with Remote Control — Claude Code Docs — <https://code.claude.com/docs/en/remote-control>
- Claude Code on mobile — Claude Code Docs — <https://code.claude.com/docs/en/mobile>
- Approve Claude Code permission requests remotely — DEV Community — <https://dev.to/dan_288d398451d57/approve-claude-code-permission-requests-remotely-3bgm>
- [BUG] Slash commands fall through as prompts in externally-triggered remote execution (web) sessions — GitHub Issue #68402 — <https://github.com/anthropics/claude-code/issues/68402>
- Slash Commands on Claude Code iOS: Essential Smart Guide (2026-09) — <https://www.progressiverobot.com/2026/09/04/claude-code-ios-slash-commands/>
- claudecodeui — Open-source Web UI for Claude Code (siteboon, AGPL-3.0) — <https://github.com/siteboon/claudecodeui>
