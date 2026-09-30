#!/usr/bin/env python3
"""analyzeV02.py — 計算每篇文章「我 + 你」同句共現的密度.

假設 (待驗證):
    技術文章多半用被動或第三人稱, 直接對話風格 (我來說明, 你可以試試) 較少見.
    AI 寫作愛用會話式語氣, 而且愛在同一句同時對讀者說 (「我覺得你應該...」).
    所以「同一句同時出現我跟你」的句子, 可能是 AI 家教式口吻的訊號.

指標:
    共現句: 一個句子裡同時出現「我」跟「你」, 算 1 句 (不管各出現幾次)
    句子邊界: 。？！ 或換行. 標題不計, 程式碼區塊整段排除
    範圍: 只算正文
    主排名: 共現句 / 總句數 (百分比)
    另附: 共現句 / 千字, 方便跟 analyze.py 的 `——` 密度單位比較

輸出:
    articles-v02.csv        每篇: 我/你/共現句/總句/共現比 + 每千字
    series-summary-v02.csv  每系列: 加總後的共現句佔比
    results-v02.md          共現比最高的文章與系列排行

讀資料: 沿用 analyze.py 的 Raw class.

用法:
    python3 analyzeV02.py
    python3 analyzeV02.py --top 30
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
SENT_SPLIT = re.compile(r"[。？！\n]+")


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


def html_to_text(body):
    body = re.sub(r"(?is)<(script|style)\b.*?</\1>", " ", body)
    body = re.sub(r"(?is)<pre\b.*?</pre>", " ", body)
    body = re.sub(r"(?s)<[^>]+>", " ", body)
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


def measure(text):
    """回傳 (me_hits, you_hits, sent_total, sent_coocc, sent_me_only, sent_you_only, chars)."""
    sentences = [s for s in SENT_SPLIT.split(text) if s.strip()]
    me_only = you_only = coocc = 0
    for s in sentences:
        has_me, has_you = "我" in s, "你" in s
        if has_me and has_you:
            coocc += 1
        elif has_me:
            me_only += 1
        elif has_you:
            you_only += 1
    me_hits = text.count("我")
    you_hits = text.count("你")
    chars = len(re.sub(r"\s", "", text))
    return me_hits, you_hits, len(sentences), coocc, me_only, you_only, chars


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
            me_h, you_h, sent_total, coocc, me_only, you_only, chars = measure(html_to_text(body))
            ratio = coocc / sent_total if sent_total else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "me_hits": me_h, "you_hits": you_h,
                "sent_total": sent_total, "sent_coocc": coocc,
                "sent_me_only": me_only, "sent_you_only": you_only,
                "chars": chars, "ratio": ratio,
                "coocc_pct": ratio * 100,
                "coocc_per_1k_chars": (coocc / chars * SCALE) if chars else 0.0,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v02.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "me_hits", "you_hits", "sent_total", "sent_coocc",
              "sent_me_only", "sent_you_only", "chars",
              "ratio", "coocc_pct", "coocc_per_1k_chars"]
    with open(os.path.join(HERE, "articles-v02.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["ratio"]):
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "coocc_pct": f"{r['coocc_pct']:.2f}",
                        "coocc_per_1k_chars": f"{r['coocc_per_1k_chars']:.2f}"})

    # series-summary-v02.csv (加總再除)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "sent_total": 0, "sent_coocc": 0, "me_hits": 0, "you_hits": 0, "chars": 0,
            "per": []})
        g["articles"] += 1
        g["sent_total"] += r["sent_total"]
        g["sent_coocc"] += r["sent_coocc"]
        g["me_hits"] += r["me_hits"]
        g["you_hits"] += r["you_hits"]
        g["chars"] += r["chars"]
        g["per"].append(r["ratio"])
    srows = []
    for g in series.values():
        ratio = g["sent_coocc"] / g["sent_total"] if g["sent_total"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "sent_total", "sent_coocc", "me_hits", "you_hits", "chars")},
                      "ratio": ratio, "coocc_pct": ratio * 100,
                      "median_article_pct": statistics.median(g["per"]) * 100})
    srows.sort(key=lambda r: -r["ratio"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "sent_total", "sent_coocc", "me_hits", "you_hits", "chars",
               "ratio", "coocc_pct", "median_article_pct"]
    with open(os.path.join(HERE, "series-summary-v02.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "coocc_pct": f"{r['coocc_pct']:.2f}",
                        "median_article_pct": f"{r['median_article_pct']:.2f}"})

    # results-v02.md
    tot_sent = sum(r["sent_total"] for r in rows)
    tot_coocc = sum(r["sent_coocc"] for r in rows)
    tot_me_only = sum(r["sent_me_only"] for r in rows)
    tot_you_only = sum(r["sent_you_only"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    hit_articles = sum(1 for r in rows if r["sent_coocc"])
    top = sorted(rows, key=lambda r: -r["ratio"])[:args.top]
    L = [f"# lab01 V02 結果：「我 + 你」同句共現密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}。共現句 = 一個句子裡同時出現「我」跟「你」;",
         f"> 句子邊界用 `。？！` 或換行. 主排名 = 共現句 / 總句數.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| 有共現句的文章 | {hit_articles}（{hit_articles / len(rows):.1%}） |" if rows else "",
         f"| 總句數 | {tot_sent} |",
         f"| 共現句 (我 + 你) | {tot_coocc} |",
         f"| 只有「我」的句 | {tot_me_only} |",
         f"| 只有「你」的句 | {tot_you_only} |",
         f"| 總字數 | {tot_chars} |",
         f"| 全體共現比 | {tot_coocc / tot_sent * 100:.2f}% |" if tot_sent else "",
         f"| 全體共現句每千字 | {tot_coocc / tot_chars * SCALE:.2f}" + " |" if tot_chars else "",
         "",
         f"## 共現比最高的 {len(top)} 篇（不設字數 / 句數門檻）",
         "",
         "| # | 共現比 | 共現句 | 總句 | 我 | 你 | 字數 | 文章 | 系列 | 組別 |",
         "|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['coocc_pct']:.2f}% | {r['sent_coocc']} | {r['sent_total']} | "
                 f"{r['me_hits']} | {r['you_hits']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## 共現比最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | 共現比 | 各篇中位數 | 篇數 | 共現句 | 總句 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        L.append(f"| {i} | {r['coocc_pct']:.2f}% | {r['median_article_pct']:.2f}% | "
                 f"{r['articles']} | {r['sent_coocc']} | {r['sent_total']} | "
                 f"{r['series_title'].replace('|', chr(92) + '|')} | {r['group_name']} |")
    L += ["", "> 短文 / 句數少的文章共現比會跳得很高 (1 句就能佔 50%), 看排行要一起看句數欄.",
          "> 這是共現訊號, 不是判決. 「我 + 你」多不代表一定是 AI 寫的.", ""]
    with open(os.path.join(HERE, "results-v02.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, 共現句 {tot_coocc}/{tot_sent} = {tot_coocc/tot_sent*100:.2f}%")
    print("輸出: articles-v02.csv, series-summary-v02.csv, results-v02.md")


if __name__ == "__main__":
    main()
