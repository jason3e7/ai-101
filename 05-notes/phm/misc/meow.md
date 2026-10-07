# PHM #4 Misc — meow (PENDING)

| 項目 | 值 |
|:---|:---|
| 類別 | Misc |
| 分數 | 50 |
| 解題數 | 278 |
| 附檔 | [`/static/meow.png`](https://ctf.hackme.quest/static/meow.png) |
| 狀態 | **未解** — ZIP 密碼未找到 |

## 題目

> Pusheen is cute!

296x279 的 PNG, 畫面是 4 個 Pusheen 配甜甜圈排 2x2.

## 分析

PNG IEND 之後附了一個 **ZIP 檔**:

```python
import re
with open('meow.png','rb') as f: d=f.read()
m = re.search(rb'IEND\xaeB\x60\x82', d)
with open('meow.zip','wb') as f: f.write(d[m.end():])
```

ZIP 內容:

```
meow/
meow/flag                                               47 bytes  ENCRYPTED  DEFLATE
meow/t39.1997-6/
meow/t39.1997-6/p296x100/
meow/t39.1997-6/p296x100/10173502_279586372215628_1950740854_n.png  48543 bytes  ENCRYPTED  DEFLATE
```

**關鍵線索**: 包含一個 Facebook CDN 風格的 PNG 路徑 `t39.1997-6/p296x100/...`. 看起來是 Pusheen 官方 FB 的某張甜甜圈相關圖, 檔案建立日期 2014-05-14.

## 嘗試過的方法 (都失敗)

### 1. 字典攻擊

試過:
- SecLists top 10k common passwords — no
- SecLists top 100k — no
- 完整 rockyou.txt (14M passwords) — no
- Pusheen 相關 (pusheen, Pusheen, PusheenTheCat, pusheenthecat, pusheencat, ...)
- Challenge-specific (meow, cute, donut, nom, nationaldonutday, I love pusheen, ...)
- FB ID 相關 (10173502, 279586372215628, 1950740854, concat 版本)
- Author/site 相關 (inndy, hackme, hackme.quest, phm, ...)

全部沒中, 證明這個密碼**不在常見字典裡**, 可能需要特定脈絡.

### 2. 短密碼 bruteforce

```bash
fcrackzip -b -c 'a1' -l 5-5 -u meow.zip  # ~24s, no
fcrackzip -b -c 'a1' -l 6-6 -u meow.zip  # kill 掉, 太慢
```

### 3. 已知明文攻擊 (bkcrack)

ZIP 用 ZipCrypto, 理論上可以用 bkcrack 做已知明文攻擊:
- 需要 12+ 位 contiguous plaintext of **compressed** bytes
- PNG 檔頭有 8 bytes 已知 (`89504e470d0a1a0a`), 但這是**解壓後**的, 不是壓縮串流
- 內層 PNG 壓縮率 99.7% (48543 → 48404), 所以是壓縮不是 stored, deflate 輸出不好預測
- 若能拿到原始 FB 圖重現壓縮, 可以攻擊 → 但 FB CDN 回 403, Wayback 也沒抓到

## 待試的方向

- [ ] 找 Pusheen 2014-05-14 國定甜甜圈日的 FB 貼文, 下載原圖
- [ ] 用原圖做 known-plaintext attack with bkcrack
- [ ] 或者用 CeWL 爬 Pusheen 官方 FB / 網站生成字典再 brute

## Flag

```
(pending)
```

## 感想

典型的「ZIP in PNG + 加密」套路. PHM 的 ORIGINAL 題目普遍不會乖乖讓 rockyou 直接撞出, 作者設定的密碼多半要靠**題目的 context clue** 推. 這題的 context 是 FB CDN 路徑 → Pusheen 甜甜圈 — 但目前還沒把 Pusheen 的那張原圖撈到.
