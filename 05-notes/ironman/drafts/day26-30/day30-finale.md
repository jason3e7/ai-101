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

## 原理 — The Through-Line (Day 01, Day 06-10)

AI 不是通靈, 它只是在猜下一個字 (Day 01). 這句話是整個序列的核心 — 它會的事、它不會的事、怎麼餵它、怎麼驗它、怎麼讓它自己跑, 全部都從這一句延伸出來的.

![Day 10 cover — xxx Engineering, 名字會變, 智慧是自己的](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/ironman/assets/day06-10/day10-cover.png)

在原理不變的情況下, 「xxx」可能會一直迭代 (prompt → context → harness → loop → 下一個 ...), 但 **要給它什麼, 它才會對**, 是你自己的能力.

## AI 現在能什麼, 不能什麼 — The Capability Map (Day 02-05)

* The Capability Map

```text
　　收斂　＜ーーーーーーーーー　轉換　ーーーーーーーーー＞　發散
　　多變少　　　　　　　　　　　等量換形式　　　　　　　　　少變多
　　ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
　　摘要　統整　　　　　　　　　翻譯　改寫　　　　　　　　　續寫　舉例
　　抽取　分類　排序　　　　　　解釋　換格式　　　　　　　　發想　替代方案
　　ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー
　　必須忠於原文　　　　　　　　一樣不准加　　　　　　　　　要的就是無中生有
　　多加的就是錯　　　　　　　　最穩的一欄　　　　　　　　　亂想才是重點
```

* Limits

| 極限 | 類型 | 長什麼樣 | 你該做的 |
|---|---|---|---|
| 不會自己發現自己錯 | 結構性 | 「再檢查一次」→ 同一個錯講得更有信心 | 驗證要來自AI之外 |
| 反過來問就不會 | 結構性 | 答得出作者，答不出他寫過哪些書 | 換方向問，當成廉價的驗證 |
| 不會從對話中學會 | 結構性 | 新對話又用回你禁止過的寫法 | 用 `CLAUDE.md`、memory、RAG 補 |
| 精確計算不可靠 | 結構性 | `4823 × 7591` 直接算會錯 | 叫它寫程式算，別叫它心算 |
| 鋸齒狀前沿 | 看不見 | 兩題難度相仿，一題完美一題離譜 | 預設它隨時可能站在錯的那一邊 |

## 怎麼驗證 AI 產出, 人的瓶頸 — Verification and the Human Bottleneck (Day 11-15)

* Verification

| 手段 | 寫 prompt 時要求它 | 收到答案時自己做 | 學術對應 |
|:---|:---|:---|:---|
| 列步驟 (proof) | 「先列推理步驟再給結論, 不要跳」 | 檢查每一步之間有沒有跳、有沒有偷渡假設 | Chain-of-Thought (Wei 2022), 對推理模型已負收益, (但人類可能比較好驗證, 就算 AI 其實不是走這個路徑) |
| 列假設 | 「列出這結論依賴的假設」 | 看那些假設在你的實際情境是否成立 | — |
| 給依據 (citation) | 「每個斷言附可查證來源: 數字 / 引用 / 時間」 | 隨機抽一項回頭查, 特別是數字和人名 | attribution / RARR (Gao 2022); 長文變體 FActScore (Min 2023) |
| 舉反例 (counterexample) | 「舉一個能推翻這結論的情境」 | 自己想一個「如果 X 不是這樣」的反例 | counterexample probing |
| 極端 / 邊界 (edge case) | 「代入 0、極大值、負數、空集合會怎樣?」 | 用極端輸入代進去看它會不會崩 | adversarial / edge-case testing |
| 獨立驗證 | **寫進 prompt 沒用** | 換問法、**換模型**、查權威來源, 對得起來才信 | cross-family judge / generative reasoning judges (2025 SOTA) |

![Day 13 cover — AI 用久了驗證會鈍化](https://raw.githubusercontent.com/jason3e7/ai-101/master/05-notes/ironman/assets/day11-15/day13-cover.png)

## AI 滲入已是不可逆的現在進行式 — The Point of No Return (Day 16-20)

想知道 AI 已經滲進多深, 自己掃一次最準. Day 16-18 寫了程式檢視當屆鐵人賽 15,057 篇全文:

- Day 16: 只用 `——`, 已經看得出全站數量還不少.
- Day 17: 新增 6 個訊號 + 權重, 再測一次.
- Day 18: 加入中文檢查, 零命中的文章非常少. 另附一個 Tampermonkey script 讓你瀏覽任何網頁即時看 AI 味分數.

Day 20 : **AI 文化入侵**, 如果未來出生的人, 最一開始讀的文字就有 AI 風格, 寫出來的東西也會有 AI 的風格, 而 AI 風格席捲世界是**不可逆的現在進行式**, 接下來人跟 AI 文風的界線會越來越模糊. (或許某程度也是一種統一)

## script kid 式盲打, 從程式到資安 — How Far Blind-Driving AI Goes (Day 21-29)

Day 01-20 撰寫 ithome 鐵人賽都在打文字難驗的仗. **Day 21 轉場**: 回到好驗證的主場 —— 程式. 程式有一個文字沒有的東西: 你可以**跑跑看**. 編譯錯、runtime error、test fail、diff mismatch, 會有明確的結果知道對錯.

從 Day 21 開始, 刻意裝 **script kid**: **不理解、不提示, 題目直接丟給 AI 自己解**, 只做登入跟按送出. 看能走多遠.

| 階段 | 題庫 | 結果 |
|:---|:---|:---|
| ZeroJudge (Day 22-23) | 78 題 (基礎 42 + 競賽 16 + UVa 20) + 19 題進階 | **96.2% AC** (單次送出, 不重試) |
| LeetCode (Day 24-25) | Blind 75 + LC-75 + NC150 共 134 題 | **99%+ AC**, 唯一 WA 是 `pow(x, n)` 的浮點誤差 |
| PHM CTF (Day 26-28) | 101 題, 8 類 (Misc / Web / Pwn / Reversing / Crypto / Forensic / Programming / Lucky) | **74 題 / 6620 pts / 歷史全站 Rank 10** |
| HTB (Day 29) | 兩台 (Cap easy, Touch kiosk) | Cap **26 分鐘拿 user + root**; Touch 卡在 kiosk, **但 agent 自己架 Xvfb + xfreerdp + xdotool + ImageMagick 做出 agentic RDP pipeline** |

這就是**今天 AI 能取代到的範圍**: 有明確判准 + 吃 pattern 的任務幾乎全都能做, 真實系統串接可以到 7 環 exploit chain. 卡住的不是能力, 是 safeguard.

## Takeaways: 活到老, 用到老, 用中學 — The Core

收尾就一句: **活到老, 用到老, 用中學**. 這不是三件事, 是**同一件**: 一條連續動作, 中間不能停.

AI 每隔幾個月換一代 (Opus → Mythos → 下一個), 等觀望永遠慢一拍. 要跟上, 唯一的方法就是**在使用的過程中學** — 不是先讀完手冊才用, 不是等有需要才碰, 不是看完教學影片才動手. 是日常把它拉進每一個任務裡, 用壞再換, 錯了會留下記憶. 讀十篇文章不如跑一次, 這 30 天本身就是示範: 每篇都是先跑才知道結論跟直覺差多少.

沒有找到「用魔法打敗魔法」的辦法, 但找到了「**用魔法學習魔法**」的方法: 把你要的結果給 AI, 請它逆向出 prompt, 再用那個 prompt 去產生下一個迭代.

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
