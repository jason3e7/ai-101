---
title: AI 101 - 全民抓 AI 文：破折號、「不是…而是」與這場抓鬼遊戲
tags: [ai, 文風, 辨識, em-dash, 不是而是, 社群現象, 進階]
created: 2026-10-02
---

# 全民抓 AI 文 — The Great AI-Text Witch Hunt

[← 回主頁](../../index.md)

> [!NOTE]
> 2025 年底開始，中文社群流行一種「抓鬼」遊戲：看到破折號 `——`、「不是⋯而是⋯」、「」引號、`｜` 分隔號，就斷定「這是 AI 寫的」。這篇不講破折號**為什麼**出現（那在 [AI 的文風與語氣](./ai-writing-style-tells.md) 裡），而是講這場社群現象本身：這些「鐵證」靠不靠得住、為什麼清單一直在變、以及另一派人「與其抓鬼不如調教」的說法。

> **TL;DR (EN):** Since late 2025 Chinese-language social media has turned AI-writing tells into a gotcha game - point at an em dash or a "not X, but Y" and call it AI. The problem: these signals produce constant false positives. The Economist compared 55,940 sentences / 1.2M words and found the em dash is **not** reliable (only Claude exceeds the human rate; ChatGPT now uses fewer than humans); the real statistical tell is **too little** punctuation, not too much. The accused tells keep shifting ("not X but Y" has already dropped to #2), because it is an arms race: both the models and the spotters adapt. Meanwhile a counter-camp argues the tells are just bad prompting, not proof of anything. The honest takeaway is the same as everywhere else in this repo: one symbol is never evidence; the real signal is hollow, accountability-free content, not a punctuation mark.

---

```markdown
# 看到破折號就喊 AI, 是一場會冤枉人的抓鬼遊戲
* 社群在抓鬼
    * 鐵證清單: 破折號 不是而是 引號 分隔號 emoji 殘留粗體
    * 教師改作業: 三個破折號就九成是 AI
* 為什麼會冤枉人
    * 符號是人類用了幾百年的標點
    * 經濟學人: 破折號不可靠, 只有 Claude 超過人類
    * 真正的統計訊號是標點太少, 不是太多
* 清單一直在變
    * 不是而是 已經掉到第二名
    * 第一名: 其實開頭 ＋ 空洞趨勢分析
    * 這是軍備競賽, 抓的人和被抓的都在演化
* 另一派聲音
    * 生意人: 這些都是沒調教好, 我會調
    * 玩梗派: 來寫最不像 AI 的 不是而是
* 怎麼看才對
    * 單一符號永遠不是證據
    * 看密度 ＋ 有沒有具體擔責任的內容
    * 健康的回應不是加錯字裝人, 是放進真東西
```

---

## 一場全民抓鬼遊戲 — The Spotting Game

2025 年底到 2026 年，中文圈社群（Threads、Instagram、各家媒體）玩起同一件事：整理一份「AI 文特徵清單」，看到就喊「這是 AI 寫的」。被點名的「鐵證」大致是這些：

| 被當成鐵證的特徵 | 長什麼樣 | 誰在喊 |
|:---|:---|:---|
| **破折號 `——`** | 「你不是不想醒 —— 而是太累了」 | 被 Z 世代叫「ChatGPT 連字號」；有老師說「作業裡三個破折號就九成是 AI」 |
| **「不是⋯而是⋯」** | 「重點不是 A，而是 B」 | 「AI 最愛的邏輯結構，每幾百字就來一次」 |
| **分隔號 `｜` `／`** | 「夏日穿搭指南｜涼感又時髦」 | 「沒有人平常講話會這樣分隔」 |
| **Emoji 堆疊** | 每條清單開頭都塞表情符號 | 「像貼圖大會，不像自然對話」 |
| **沒刪乾淨的粗體** | 直接貼上 AI 文字，連 `**粗體**` 語法都還在 | 最經典的「直接複製貼上」露餡 |
| **「」上下引號** | 動不動就把詞「框」起來 | 最新一波點名，連引號都算 |

有媒體直接下標「網點名 1 符號：99% 秒判」，指的就是破折號。這份清單本身沒什麼問題 - 這些確實是 AI 文常見的習慣。問題出在大家怎麼用它：**把「常見習慣」當成「有罪證據」。**

---

## 為什麼這些「鐵證」會冤枉人 — Why the "Proof" Backfires

把單一符號當證據，最大的毛病是**冤枉人**（false positive）。破折號、引號、「不是⋯而是」都是人類用了幾百年的正常工具，很多人本來就這樣寫。

社群上已經有人反彈。有位創作者在 Threads 上抱怨（大意）：這套「怎麼辨識 AI 文」的方法論裡，修辭點名「不是⋯而是」、符號點名破折號 - 結果**好多特徵根本就是他本來的寫作習慣**，現在為了「看起來不像 AI」要刻意改掉，偶爾還得故意打錯字。最後他吐槽：有人連上下引號「」都說是 AI 特色，「大哥，你命中是沒用過標點符號嗎？」

> [!IMPORTANT]
> 真正做了大規模統計的，結論剛好相反。**經濟學人（The Economist）2026 年比對了 55,940 句、120 萬字**（自家文章 ＋ ChatGPT／Claude／Gemini／Grok 生成版 ＋ 紐時、華郵、1950–2022 的小說），發現：
> - **破折號不是可靠證據。** 只有 Claude 用得比人類兇；ChatGPT 現在反而用得**比人類還少**。所以「看到破折號 = AI」在今天多半會抓錯。
> - **真正的統計破綻是標點用得太少。** AI 的逗號、分號、括號都比人類少，愛堆超長句子，讀起來「太乾淨、沒有呼吸感」。
>
> 換句話說，大家盯著「多出來的破折號」抓，而機器真正露餡的地方，是**少掉的那些逗號和分號**。

這跟這個 repo 自己的實測也對得起來：lab01 把 2026 iThome 鐵人賽約 929 個系列、1.5 萬篇文章全抓下來算破折號密度，結論一樣是「破折號密度只是排序訊號，不是判決」（見 [lab01 的 results](../../05-notes/ironman/lab01/results.md)）。**單一符號永遠抓不了人。**

---

## 清單一直在變：這是軍備競賽 — A Moving Target

這份「鐵證清單」還有一個特性：**它一直在變。** 這不是大家喜新厭舊，是因為抓的人和被抓的都在演化 - 典型的軍備競賽。

一個明顯的例子：曾經排第一的「不是⋯而是」，到 2026 年已經**掉到第二名**。現在被點名第一、「幾乎每篇都中」的是另一組：

- **第 1 名**：用「其實⋯」開頭，接一段空洞的宏觀「趨勢」分析 - 不談細節、只談整體，愛用「真正的問題是」「關鍵不在於」「擁抱」「賦能」這類詞。被嫌「缺乏具體的肉身記憶與在地情感」。
- **第 2 名**：「這不是⋯，而是⋯」的二元對立句 - 先否定一個現成標籤，後半句包裝一句看似深奧的新說法。
- **其他**：工整條列、三段式、四平八穩、用對比取代實質論證。

> [!WARNING]
> 軍備競賽有個很累的副作用：**為了「不像 AI」而故意寫爛。** 刻意加錯字、刻意避開自己原本的句型、刻意不用標點 - 這些努力換來的「人味」是假的，而且很快又會被歸納成新的一條「AI 特徵」（反正模型也會學）。盯著符號跑，永遠跑不贏。

---

## 另一派：與其抓鬼，不如調教 — The Counter-Camp

不是所有人都在抓鬼。社群上有明顯的另一派，覺得這整件事抓錯了重點。

**「這些都是沒調教好而已。」** 有做內容生意的人直接說：開頭那種「你不是⋯而是⋯」「搖了搖頭，長嘆一口氣」，全都能靠提示詞（prompt）改掉，他甚至會要求 AI 多講點粗話、台式諧音梗來增加溫度。他的論點很直白：大家看到的「AI 文」，多半是不懂調教、隨手用 ChatGPT 生出來的；做生意的人幹嘛排斥 AI 寫作？寫得快、寫得有人看、流量進來才是重點 - 他自己就在賣調教好的 AI 寫作機器人。（這派的立場要打個折看：**他是在賣工具**，誇大「調教就能解決」對他有利。）

**「那就來玩最不像 AI 的版本。」** 另一種回應是把它當遊戲。有人發起挑戰：既然大家說「不是⋯而是」就是 AI，那就來填「我不是＿＿＿，而是＿＿＿」，比誰寫得最不像 AI。發起人的話點到了核心：**「AI 寫得出結構，但只有人，才寫得出各種 AI 想不到的梗。」**

這兩派嘴上吵，底層其實同一個觀點：**句型本身是中性的，問題在內容空不空。** 多數人吵到最後也同意 - 原罪不是用 AI，是「直接複製貼上、沒刪冗詞、沒補進自己的經驗」。

---

## 那到底該怎麼看 — How to Actually Read This

把上面收束成幾條可用的判斷：

1. **單一符號不是證據，是提示。** 看到一個破折號、一句「不是⋯而是」，頂多讓你多留意，不能定罪。要看就看**密度**（一堆習慣擠在一起）加上**有沒有具體、會讓作者擔責任的內容**。空洞 ＋ 沒擔當，才是真訊號；符號不是。
2. **別信任何「一招秒判」。** 清單會過期（「不是⋯而是」已經退到第二），而且最會做統計的經濟學人說破折號根本不可靠、真正的破綻是標點太少。賣你「一眼識破」的，通常比被抓的 AI 文還不可靠。
3. **健康的回應不是裝人，是放真東西。** 與其加錯字、改掉自己原本的句型去躲偵測，不如把 AI 幫你起的稿，補進只有你寫得出來的東西：具體的人、事、數字、在地的經驗、你願意具名負責的判斷。這也正是「怎麼壓 AI 味」那篇的結論 - [點名具體習慣 ＋ 給替代](./ai-writing-style-tells.md#怎麼壓下去點名--給替代--how-to-suppress-them)，而不是對它喊「寫自然一點」。

> [!TIP]
> 一句話記：**抓鬼抓的是符號，但鬼從來不在符號裡。** 一篇文章像不像人，看的是裡面有沒有「人」 - 具體的經驗和敢下的判斷 - 不是它用了哪個標點。

---

## 相關筆記 — Related

- [AI 的文風與語氣](./ai-writing-style-tells.md) - 破折號**為什麼**出現、怎麼從 prompt 端壓下去（這篇的機制面）
- [AI 生成內容怎麼標記與辨識](../../01-fundamentals/ai-content-watermark.md) - 為什麼連官方浮水印都不能當定論，何況一個符號
- [lab01：AI 文風檢測基準線](../../05-notes/ironman/lab01/README.md) - 這個 repo 自己把全站鐵人賽文章抓下來算破折號密度的實測

## Sources

- [破折號不是元兇，經濟學人揭 AI 寫作真正破綻：標點符號用太少 — TechNews, 2026](https://technews.tw/2026/08/15/writers-are-creating-an-anti-ai-literary-counterculture/)
- [如何看出 AI 文？「不是…而是…」跌到第 2 名 第 1 名幾乎每篇都中 — 自由藝文網, 2026](https://art.ltn.com.tw/article/breakingnews/5501827)
- [還在貼 AI 稿不改？別再用「這」符號！5 個用法一眼識破 — udn 女子漾](https://woman.udn.com/woman/story/123164/9015473)
- [AI 文藏不住了！常見口頭禪曝光　網點名 1 符號：99% 秒判 — 鏡週刊 / LINE TODAY](https://today.line.me/tw/v3/article/2D3BNRe)
- [「不是…而是」最不像 AI 版本挑戰 — @stevenchen568, Threads](https://www.threads.com/@stevenchen568/post/DWNt9gPkn_e/)
- [做生意就該用 AI 寫作：這些用詞都能靠調教避免 — @sofi.life_official, Threads](https://www.threads.com/@sofi.life_official/post/DQl5AkDk-tl)
- [好多 AI 特徵根本是我本來的寫作習慣（抓鬼反彈） — @nine537, Threads](https://www.threads.com/@nine537/post/DQo9UQFkn5A/)
