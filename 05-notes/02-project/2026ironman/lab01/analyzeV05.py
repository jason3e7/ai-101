#!/usr/bin/env python3
"""analyzeV05.py — emoji 密度訊號.

假設 (待驗證):
    AI (尤其 ChatGPT) 特別愛在文字裡塞 emoji: ✅ 🎯 💡 🚀 📌 等. 這是最直觀
    的 AI 寫作 tell. 一般技術文章少用 emoji, AI 每段常常至少一顆.

    測正文裡的 emoji 字符數與密度 (per_1k).

指標:
    emoji_count:  正文 (去 <pre>, 去標籤) 裡 emoji codepoint 數
    chars:        全篇正文字元數 (去空白, 跟 V02/V03/V04 一致, emoji 也算 1 字)
    per_1k:       emoji_count / chars * 1000       ← 主指標 (頻率)

範圍 (Unicode 常見 emoji block):
    U+1F300-1F5FF  Misc Symbols and Pictographs
    U+1F600-1F64F  Emoticons (😀 😄 🙌)
    U+1F680-1F6FF  Transport and Map (🚀)
    U+1F700-1F8FF  Alchemical / Geometric Ext / Arrows-C
    U+1F900-1F9FF  Supplemental Symbols (🧠)
    U+1FA00-1FAFF  Chess / Symbols Ext-A
    U+2600-26FF    Misc Symbols (☀️ ★ ⚠️)
    U+2700-27BF    Dingbats (✂️ ✈️ ✅ ❌)
    U+1F1E6-1F1FF  Regional Indicator (國旗)

已知限制:
    * 複合 emoji (ZWJ 序列如 👨‍👩‍👧) 會被拆成多個 codepoint 分別計數.
      在技術文章少見, 對整體 signal 影響有限.
    * U+2600-27BF 範圍有少數非 emoji 字符 (如 ☎ ✉ 是舊符號), 一律都算.
    * Variation Selector (U+FE0F) 不算 emoji 本身, 不列入.

排除清單 (EXCLUDE):
    人類寫技術文章也常用的排版符號 (checklist, 星等, 圈叉), 不算 AI tell:
    ○ (U+25CB white circle)   ← 這個本來就不在 emoji range, 保險起見列入
    ✗ (U+2717 ballot X)
    ★ (U+2605 black star)
    ☆ (U+2606 white star)
    ☐ (U+2610 ballot box)

輸出:
    articles-v05.csv        每篇: emoji_count / chars / per_1k
    series-summary-v05.csv  每系列: 加總後的 per_1k
    results-v05.md          per_1k 最高的文章與系列排行

讀資料: 沿用 analyze.py 的 Raw class.

用法:
    python3 analyzeV05.py
    python3 analyzeV05.py --top 30
"""

import argparse
import csv
import html as htmlmod
import json
import os
import re
import statistics
import tarfile

HERE = os.path.dirname(os.path.abspath(__file__))
RAW_DIR = os.path.join(HERE, "raw")
RAW_TGZ = os.path.join(HERE, "raw.tgz")
SCALE = 1000
BASE = "https://ithelp.ithome.com.tw"
END_MARKERS = ("qa-action", "article-series-page", "ir-article__footer", "qa-panel")

RE_EMOJI = re.compile(
    "["
    "\U0001F300-\U0001F5FF"
    "\U0001F600-\U0001F64F"
    "\U0001F680-\U0001F6FF"
    "\U0001F700-\U0001F8FF"
    "\U0001F900-\U0001F9FF"
    "\U0001FA00-\U0001FAFF"
    "\U00002600-\U000026FF"
    "\U00002700-\U000027BF"
    "\U0001F1E6-\U0001F1FF"
    "]"
)
EXCLUDE = frozenset("○✗★☆☐")
RE_PRE = re.compile(r"(?is)<pre\b.*?</pre>")
RE_SCRIPT_STYLE = re.compile(r"(?is)<(script|style)\b.*?</\1>")
RE_TAG = re.compile(r"(?s)<[^>]+>")


class Raw:
    def __init__(self):
        if os.path.isdir(RAW_DIR):
            self.mode, self.src = "dir", RAW_DIR
        elif os.path.exists(RAW_TGZ):
            self.mode, self.src = "tgz", RAW_TGZ
            self.tar = tarfile.open(RAW_TGZ, "r:gz")
            self.members = {m.name.split("raw/", 1)[-1]: m for m in self.tar.getmembers() if m.isfile()}
        else:
            raise SystemExit("找不到 raw/ 或 raw.tgz, 先跑 fetch.py")

    def read(self, rel):
        if self.mode == "dir":
            p = os.path.join(RAW_DIR, rel)
            if not os.path.exists(p):
                return None
            with open(p, encoding="utf-8") as f:
                return f.read()
        m = self.members.get(rel)
        return self.tar.extractfile(m).read().decode("utf-8") if m else None


def strip_pre(body):
    return RE_PRE.sub(" ", body)


def html_to_text(body_no_pre):
    body = RE_SCRIPT_STYLE.sub(" ", body_no_pre)
    body = RE_TAG.sub(" ", body)
    return htmlmod.unescape(body)


def article_page_body(page):
    i = page.find("markdown__style")
    if i < 0:
        return ""
    i = page.find(">", i) + 1
    ends = [page.rfind("<", i, j) for j in (page.find(k, i) for k in END_MARKERS) if j > 0]
    return page[i:min(ends)] if ends else page[i:]


def page_body(block):
    i = block.find('class="lab01-body"')
    if i < 0:
        return article_page_body(block)
    i = block.find(">", i) + 1
    j = block.rfind("</div>")
    return block[i:j if j > i else len(block)]


def measure(body_html):
    """回傳 (emoji_count, chars)."""
    body = strip_pre(body_html)
    text = html_to_text(body)
    emoji_count = sum(1 for c in RE_EMOJI.findall(text) if c not in EXCLUDE)
    chars = len(re.sub(r"\s", "", text))
    return emoji_count, chars


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=20)
    args = ap.parse_args()

    raw = Raw()
    idx = json.loads(raw.read("index.json"))
    print(f"讀取來源: {raw.src} (抓取時間 {idx.get('fetched_at')})")

    rows, missing = [], []
    for sid, s in sorted(idx["series"].items()):
        for a in s.get("articles", []):
            aid = a["article_id"]
            block = raw.read(f"pages/{aid}.html")
            if block is None:
                missing.append(aid)
                continue
            body = page_body(block)
            emoji_count, chars = measure(body)
            per_1k = emoji_count / chars * SCALE if chars else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "emoji_count": emoji_count, "chars": chars, "per_1k": per_1k,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v05.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "emoji_count", "chars", "per_1k"]
    with open(os.path.join(HERE, "articles-v05.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["per_1k"]):
            w.writerow({**r, "per_1k": f"{r['per_1k']:.4f}"})

    # series-summary-v05.csv (加總再除)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "emoji_count": 0, "chars": 0, "per": []})
        g["articles"] += 1
        g["emoji_count"] += r["emoji_count"]
        g["chars"] += r["chars"]
        g["per"].append(r["per_1k"])
    srows = []
    for g in series.values():
        per_1k = g["emoji_count"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "emoji_count", "chars")},
                      "per_1k": per_1k,
                      "median_article_per_1k": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["per_1k"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "emoji_count", "chars", "per_1k", "median_article_per_1k"]
    with open(os.path.join(HERE, "series-summary-v05.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "per_1k": f"{r['per_1k']:.4f}",
                        "median_article_per_1k": f"{r['median_article_per_1k']:.4f}"})

    # results-v05.md
    tot_emoji = sum(r["emoji_count"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    articles_with_emoji = sum(1 for r in rows if r["emoji_count"])
    top = sorted(rows, key=lambda r: -r["per_1k"])[:args.top]
    L = [f"# lab01 V05 結果：emoji 密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式:",
         f"> per_1k = emoji_count / total_chars * 1000  (每千字幾個 emoji)",
         f"> 範圍: Unicode emoji block (含 dingbats ✅❌, 國旗, 各類 pictograph).",
         f"> 複合 emoji (ZWJ 序列如 👨‍👩‍👧) 拆成多個 codepoint 計算.",
         f"> 排名不設字數門檻, 短文仍可能爆.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |"]
    if rows:
        L.append(f"| 有 emoji 的文章 | {articles_with_emoji}（{articles_with_emoji / len(rows):.1%}） |")
    L += [f"| emoji 總數 | {tot_emoji} |",
          f"| 全篇總字數 | {tot_chars} |"]
    if tot_chars:
        L.append(f"| 全體 per_1k | {tot_emoji / tot_chars * SCALE:.4f} |")
    L += ["",
          f"## per_1k 最高的 {len(top)} 篇（不設字數門檻）",
          "",
          "| # | per_1k | emoji_count | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['per_1k']:.4f} | {r['emoji_count']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## per_1k 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | per_1k | 各篇中位數 | 篇數 | emoji_count | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['per_1k']:.4f} | {r['median_article_per_1k']:.4f} | "
                 f"{r['articles']} | {r['emoji_count']} | {r['chars']} | "
                 f"{st} | {r['group_name']} |")
    L += ["", "> 沒 emoji (emoji_count = 0) 的文章 per_1k = 0, 全部沉底 (不代表沒 AI 味).",
          "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v05.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, "
          f"emoji {tot_emoji} 個, 總字 {tot_chars}, "
          f"全體 per_1k {tot_emoji/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v05.csv, series-summary-v05.csv, results-v05.md")


if __name__ == "__main__":
    main()
