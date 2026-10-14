title : [Day 30] 三十天心法總結, 跟著 AI 持續成長

> 如果 30 天的文章要看哪一篇, 就看這篇總結就好, 我把所有財寶都放在這了.

寫在前面 (jason3e7):

> 30 天跑完, 最確定一件事: 其他 29 篇都是短期的, 總結這篇最長命. 每篇記的是當天某個觀察的瞬間, 這篇是把全部串起來的那條線. 將來若只能保留一篇, 就留這篇, 其他可以忘.

```markdown
# 三十天心法總結: 所有財寶都放在這了
* 原理 — Day 10 的圖收 Day 1, 6-9
  * 猜下一個字 → 四層工程演進
* AI 現在能什麼, 不能什麼 — Day 2-4
  * 跨過你的門檻 / 三種極限 / 兩種能力全景圖
* 怎麼驗證 AI 產出, 人的瓶頸 — Day 11-15
  * 答案真假 / 鈍化 / 中產被抹平
* 不可逆的現在進行式 — Day 20 收 Day 16-19
  * 掃 AI 味 → 浮水印 → 文風入侵
* script kid 式盲打, 從程式到資安 — Day 21-29
  * ZeroJudge / LeetCode / CTF / HTB
  * AI 現在能取代到哪裡
* 活到老, 用到老, 用中學 — 結論
```

---

## 原理 — The Through-Line (Day 10 收 Day 1 / 6-9)

AI 不是通靈, **它在猜下一個字** (Day 1). 這句話推出來的是整個序列的脊椎 — 它會的事、它不會的事、怎麼餵它、怎麼驗它、怎麼讓它自己跑, 全部從這一句推下來.

Day 6-9 的四層工程演進, Day 10 把它們串成一張圖:

- **Prompt Engineering (Day 6)**: 哪些技巧有實證支持, 哪些只是文青傳說. CoT / few-shot 真的有用, "please" / "take a deep breath" 這類在對照實驗下撐不住.
- **Context Engineering (Day 7)**: 你丟進去的那串 prompt 只佔它看到的 5%, 另外 95% 是 CLAUDE.md、tool list、載入的檔案、agent-skill 自動裝進去的東西. 大多數「它怎麼突然變笨」的問題, 根因不在 prompt, 在 context.
- **Harness Engineering (Day 8)**: Claude Code、ChatGPT、Cursor 這些你每天在用的工具, 本體不是模型, 是 harness — 把模型綁在軌道上的那套支架. 你早就在用, 只是沒人這樣稱呼它.
- **Loop Engineering (Day 9)**: `/goal`、sub-agent、autonomous pentest — 不再是你一輪一輪餵, 是你給目標, agent 自己展開 N 輪動作. 這一步, 使用者從「提問者」變成「發包者」.

Day 10 一句話收尾: 「xxx」會一直換 (prompt → context → harness → loop → 下一個 ...), 但背後的直覺 —— 你知道「**要給它什麼它才能給對**」—— 是你自己的能力. 工具會停更, 這個直覺不會.

## AI 現在能什麼, 不能什麼 — The Capability Map (Day 2-4)

AI 看起來突然變強, 其實是**它慢慢跨過了你的門檻**. METR 2024 的量化結論: 它能處理的任務長度大約每 7 個月翻倍 (Day 2). 從單句問答, 到 10 分鐘的 bug fix, 到幾小時的 agentic 工程, 速度沒變, 是你一直站在門檻上. 等它跨過你那天, 你會以為是它變強, 其實是你被追上.

但有三類事它做不到 (Day 3):

1. **自我修正的 gap**: 它給錯答案不會自己回頭. prompt 救不了, 要外部驗證把錯誤壓出來.
2. **反轉詛咒 (reversal curse)**: 它學過「A 是 B 的父親」, 問「B 的父親是誰」答不出來. 對人很荒謬, 對 LLM 是訓練機制上的天生缺陷.
3. **鋸齒狀前沿 (jagged frontier)**: 它會 PhD 等級的題, 不會國小等級的題. 能力分布不是平滑山丘, 是鋸齒狀邊緣.

**最危險的是鋸齒**. 自我修正的 gap 你會發現 (答案明顯錯), 反轉詛咒你會察覺 (問對問題就露餡). 鋸齒前沿你**看不見** — 它在你以為它會的地方突然歪掉, 你還沒意識到就把錯答案當對的用了.

要看 AI 整體能力落在哪, 一張全景圖切兩軸就夠 (Day 4): **收斂/發散思考 × Bloom 認知層次** (記憶 / 理解 / 應用 / 分析 / 評估 / 創造). 自己工作任務對一下落在哪格, 就知道該用它跑什麼、不該用它跑什麼. 這張圖是 Day 1-10 所有建議的實作面對照表.

## 怎麼驗證 AI 產出, 人的瓶頸 — Verification and the Human Bottleneck (Day 11-15)

Day 1-10 的工具, 到 Day 11 都有一個共同前提: **你要能驗**. 不能驗的 AI 產出是負資產 — 看起來省時間, 其實是把除錯成本從現在挪到以後, 利息高.

Day 11 把驗證拆成**六種驗算法** (跑一下、對一下、問回去、找反例、第二個 agent 獨立驗、人眼細看). 哪種題配哪招有固定公式, 不是憑直覺. Day 12 做了兩個 skill (`condense-mindmap` / `expand-mindmap`) — 根本動機是**為了讓人類可以快速驗**: 大段內容壓成一張圖, 人掃一眼就知道結構對不對.

**但真的瓶頸不是驗, 是人會鈍化** (Day 13). 兩種鈍化、五招破的細節看 Day 13; 共同點是**不要讓 AI 幫你跳過成長的過程** — 練肌肉的動作省掉了, 肌肉就沒了.

Day 14 把視角拉遠: **AI 抹平的是中產, 頂層反而更貴**. 寫 CRUD、拼接 API、整理資料這類中段技能, AI 一秒出來, 時薪會塌. 但能給 AI 任務、能驗產出、能把 AI 當放大鏡用的頂層技能, 需求反而更強. 中產會被兩頭夾.

Day 15 收尾: 不想被夾, 得**主動用 5 個手段拓自己** — 讓 AI 當放大鏡, 不是拐杖. 具體 5 招詳見 Day 15, 核心一句就是「**別讓 AI 代你長大**」.

## AI 產出已是不可逆的現在進行式 — The Point of No Return (Day 20 收 Day 16-19)

想知道 AI 已經滲進多深, 自己掃一次最準. Day 16-18 寫了 **lab01** 掃當屆鐵人賽 15,057 篇全文:

- **Day 16**: 只用一個訊號 (全形破折號密度), 已經看得出全站 median 不低.
- **Day 17**: 擴到 6 個訊號 + 權重, 綜合分數再測一次, 分布更乾淨.
- **Day 18 (翻面)**: 問相反的問題 — **「哪些文章是純手寫的?」** 用 V12 (9 訊號 + 權重微調), 全體 density 69.66; 零命中 density 的文章幾乎沒有. 另附一個 Tampermonkey script 讓你瀏覽任何網頁即時看 AI 味分數.

Day 19 查另一條線: **浮水印能不能當證據?** Google SynthID、OpenAI 自家的統計標記, 技術上存在, 實務上**辨不出、擋不住**. 只要你把產出稍微改幾個字, 浮水印就斷了. 法院、人資、內容平台不能拿它當判定依據.

Day 20 把子系列收成一句: **AI 文風入侵, 五個記號你能看到, 但壓不回去**. 五個記號是結構性的 (bullet 風、過度 parallel、「不是 X 是 Y」、過度破折號、過度「其實」). 看得到不代表擋得住 — 下一代人讀的網路**就是這個樣子**, 他們對這些記號無感. 這不是風氣問題, 是**不可逆的現在進行式**: 回不去了, 接下來人跟 AI 文風的界線會越來越模糊.

## script kid 式盲打, 從程式到資安 — How Far Blind-Driving AI Goes (Day 21-29)

Day 1-20 都在打文字難驗的仗. **Day 21 轉場**: 回到好驗證的主場 —— 程式. 程式有一個文字沒有的東西: 你可以**跑跑看**. 編譯錯、runtime error、test fail、diff mismatch, 錯了會自己說. 這一轉, 整個實驗方法變了.

從 Day 21 開始, 刻意裝 **script kid**: **不理解、不提示, 題目直接丟給 AI 自己解**, 只做登入跟按送出. 看能走多遠.

| 階段 | 題庫 | 結果 |
|:---|:---|:---|
| ZeroJudge (Day 22-23) | 78 題 (基礎 42 + 競賽 16 + UVa 20) + 19 題進階 | **96.2% AC** (單次送出, 不重試) |
| LeetCode (Day 24-25) | Blind 75 + LC-75 + NC150 共 134 題 | **99%+ AC**, 唯一 WA 是 `pow(x, n)` 的浮點誤差 |
| PHM CTF (Day 26-28) | 101 題, 8 類 (Misc / Web / Pwn / Reversing / Crypto / Forensic / Programming / Lucky) | **74 題 / 6620 pts / 歷史全站 Rank 10** |
| HTB (Day 29) | 兩台 (Cap easy, Touch kiosk) | Cap **26 分鐘拿 user + root**; Touch 卡在 kiosk, **但 agent 自己架 Xvfb + xfreerdp + xdotool + ImageMagick 做出 agentic RDP pipeline** |

收斂:

- **吃 pattern 的題 agent 吃爆**: 經典密碼、classic reverse、OJ 題、Blind 75 這種 FAANG 面試題庫, 看一眼就會.
- **真實服務也串得起來**: HTB Cap 的「`cap_setuid` 讀 pcap 拿 FTP creds 轉 SSH」整條鏈 agent 自己串. 真實攻擊面對 agent 反而比 CTF 的 novelty 梗友善 — 真實系統有文件、有歷史 writeup, novelty 全是作者臨時發明.
- **卡的是罕見 novelty + 政策禁區**: PHM 剩 27 題收斂成四面牆 (md5 agent 沒上網查、libc 版本沒環境、強模型未試、單題撞 cyber safeguard). HTB Touch 卡在 kiosk escape + safeguard.

這就是**今天 AI 能取代到的範圍**: 有明確判准 + 吃 pattern 的任務幾乎全沒了, 真實系統串接可以到 7 環 CVE chain. 卡住的不是能力, 是 novelty 跟政策.

## 結論: 活到老, 用到老, 用中學 — The Core

收尾就一句: **活到老, 用到老, 用中學**. 這不是三件事, 是**同一件**: 一條連續動作, 中間不能停.

AI 每隔幾個月換一代 (Opus → Mythos → 下一個), 等觀望永遠慢一拍. 要跟上, 唯一的方法就是**在使用的過程中學** — 不是先讀完手冊才用, 不是等有需要才碰, 不是看完教學影片才動手. 是日常把它拉進每一個任務裡, 用壞再換, 錯了會留下記憶. 讀十篇文章不如跑一次, 這 30 天本身就是示範: 每篇都是先跑才知道結論跟直覺差多少.

不是**追上** AI, 是**跟著** AI 一起走. 「追上」隱含有終點, 「跟著」是陪跑. AI 這幾年會一直變, 跟著它跑, 你會一直在它前面一點點 — 這一點點就是你的位置.

到這篇為止, 財寶都在上面了. 剩下的是**你自己的手感** — 不是我能給你的, 是你跑一次就會長出來的那個東西.

---

## Sources

- [Day 1: 它只是在猜下一個字 — LLM 的原理](https://ithelp.ithome.com.tw/articles/10411345) — 整個系列的脊椎
- [Day 10: xxx Engineering, 名字會變, 智慧是自己的](https://ithelp.ithome.com.tw/articles/10416410) — 承段四層工程的收尾
- [Day 11: AI 給的答案, 你怎麼知道是對的](https://ithelp.ithome.com.tw/articles/10417119) — 轉段起手, 六種驗算法
- [Day 20: AI 文風入侵, 五個記號 + 抗體 + 子系列小結](https://ithelp.ithome.com.tw/articles/10420569) — 文風測量子系列封頂
- [Day 29: 從 CTF 換到 HTB, 用 goal 跑了兩次](https://ithelp.ithome.com.tw/articles/10424068) — 實戰段最後一次 agent 展演
- [三十篇標題與素材對照 (本 repo)](https://github.com/jason3e7/ai-101/blob/master/05-notes/ironman/titles.md)
