#!/usr/bin/env python3
"""analyzeV09.py — 中文「最容易...的」教學警告腔密度.

假設 (強):
    V09 7-字 only 版 (已廢) 掃出 corpus 熱門命中詞 top 20, 發現「最容易 X 的」
    family 佔 13 條 / 155 次, 遠超其他 superlative. 這組 phrase 是 AI 教學 /
    警告 / 總結腔的典型:
      最容易出錯的 ×18 / 最容易理解的 ×14 / 最容易漏掉的 ×14 /
      最容易搞混的 ×14 / 最容易忽略的 ×12 / 最容易出現的 ×11 /
      最容易出事的 ×10 / 最容易寫錯的 ×10 / 最容易做錯的 ×10 /
      最容易踩到的 ×9 / 最容易發生的 ×8 / 最容易誤會的 ×8 / 最容易誤判的 ×8

    V09 (revised) 刻意收窄到「最容易 X 的」, 看能不能做出更乾淨的 AI tell
    ranker.

Regex 設計:
    pattern = 最容易[一-鿿]{1,5}的
    匹配: 最容易 + 1-5 個中文字 + 的 (總長 5-9 字)
    範例命中: 最容易出錯的 / 最容易漏掉的 / 最容易被低估的 / 最容易踩到坑的

指標:
    hits:    正文裡 regex 命中次數 (去 <pre>, 去 HTML 標籤後計)
    chars:   全篇正文字元數 (去空白)
    per_1k:  hits / chars * 1000       ← 主指標

輸出:
    articles-v09.csv        每篇: hits / chars / per_1k
    series-summary-v09.csv  每系列: 加總後 per_1k
    results-v09.md          per_1k 最高的文章與系列排行 (chars >= 500)

用法:
    python3 analyzeV09.py
    python3 analyzeV09.py --top 30 --min-chars 500
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

TARGET_DESC = "最容易...的 (regex: 最容易[一-鿿]{1,5}的)"
RE_TARGET = re.compile(r"最容易[一-鿿]{1,5}的")

RE_PRE = re.compile(r"(?is)<pre\b.*?</pre>")
RE_SCRIPT_STYLE = re.compile(r"(?is)<(script|style)\b.*?</\1>")
RE_TAG = re.compile(r"(?s)<[^>]+>")

MIN_CHARS = 500


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
    body = strip_pre(body_html)
    text = html_to_text(body)
    matches = RE_TARGET.findall(text)
    hits = len(matches)
    chars = len(re.sub(r"\s", "", text))
    return matches, hits, chars


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=20)
    ap.add_argument("--min-chars", type=int, default=MIN_CHARS)
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
            matches, hits, chars = measure(page_body(block))
            per_1k = hits / chars * SCALE if chars else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "hits": hits, "chars": chars, "per_1k": per_1k,
                "_matches": matches,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source", "hits", "chars", "per_1k"]
    with open(os.path.join(HERE, "articles-v09.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["per_1k"]):
            w.writerow({**r, "per_1k": f"{r['per_1k']:.4f}"})

    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "hits": 0, "chars": 0, "per": []})
        g["articles"] += 1
        g["hits"] += r["hits"]
        g["chars"] += r["chars"]
        g["per"].append(r["per_1k"])
    srows = []
    for g in series.values():
        per_1k = g["hits"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "hits", "chars")},
                      "per_1k": per_1k,
                      "median_article_per_1k": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["per_1k"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "hits", "chars", "per_1k", "median_article_per_1k"]
    with open(os.path.join(HERE, "series-summary-v09.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "per_1k": f"{r['per_1k']:.4f}",
                        "median_article_per_1k": f"{r['median_article_per_1k']:.4f}"})

    tot_hits = sum(r["hits"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    articles_with_hit = sum(1 for r in rows if r["hits"] > 0)
    qualified = [r for r in rows if r["chars"] >= args.min_chars]
    skipped = len(rows) - len(qualified)
    top = sorted(qualified, key=lambda r: -r["per_1k"])[:args.top]

    L = [f"# lab01 V09 結果：中文「{TARGET_DESC}」密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式: per_1k = hits / chars * 1000",
         f"> Pattern: {TARGET_DESC}",
         f"> 範例命中: 最容易被低估的 / 最想多看幾眼的 / 最難以想像的",
         f"> V07 加權版發現 7 字長 superlative 特別像 AI 強調語氣. V09 刻意只抓 7 字版.",
         f"> **排行榜最小字數門檻 {args.min_chars} 字** (排除 {skipped} 篇短文).",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| 有命中的文章 | {articles_with_hit}（{articles_with_hit / len(rows):.1%}） |",
         f"| 命中總次數 | {tot_hits:,} |",
         f"| 全篇總字數 | {tot_chars:,} |"]
    if tot_chars:
        L.append(f"| 全體 per_1k | {tot_hits / tot_chars * SCALE:.4f} |")
    top_no_thresh = sorted(rows, key=lambda r: -r["per_1k"])[:args.top]

    L += ["",
          f"## [A] per_1k 最高的 {len(top)} 篇 (chars >= {args.min_chars})",
          "",
          "| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['per_1k']:.4f} | {r['hits']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")

    L += ["",
          f"## [B] per_1k 最高的 {len(top_no_thresh)} 篇 (無字數門檻, 全 {len(rows)} 篇)",
          "",
          "| # | per_1k | hits | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top_no_thresh, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        marker = "" if r["chars"] >= args.min_chars else " ⚠️"
        L.append(f"| {i}{marker} | {r['per_1k']:.4f} | {r['hits']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## per_1k 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | per_1k | 各篇中位數 | 篇數 | hits | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['per_1k']:.4f} | {r['median_article_per_1k']:.4f} | "
                 f"{r['articles']} | {r['hits']} | {r['chars']} | "
                 f"{st} | {r['group_name']} |")
    # 命中詞 top 20 (全 corpus) + 對應證據文章 top 3
    from collections import Counter, defaultdict
    global_counter = Counter()
    by_phrase_articles = defaultdict(Counter)
    article_lookup = {r["article_id"]: r for r in rows}
    for r in rows:
        for m in r["_matches"]:
            global_counter[m] += 1
            by_phrase_articles[m][r["article_id"]] += 1

    L += ["", "## 命中詞主要 Top 20 (附證據文章 Top 3)", "",
          f"> 全 corpus 掃出 {len(global_counter)} 種 unique 命中詞, 列出 top 20 跟其對應的 top 3 證據文章",
          "> 證據文章 = 該命中詞在哪 3 篇文章出現最多次, 讀者可直接點進去看語境",
          ""]
    for i, (phrase, total) in enumerate(global_counter.most_common(20), 1):
        L.append(f"**{i}. 「{phrase}」** (全 corpus ×{total})")
        for aid, cnt in by_phrase_articles[phrase].most_common(3):
            ar = article_lookup[aid]
            t = ar["title"].replace("|", "\\|")
            L.append(f"- [{t}]({ar['url']}) ×{cnt}  — {ar['series_title']} · {ar['group_name']}")
        L.append("")

    L += ["", "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v09.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, 「{TARGET_DESC}」命中 {tot_hits:,}, "
          f"總字 {tot_chars:,}, 全體 per_1k {tot_hits/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v09.csv, series-summary-v09.csv, results-v09.md")


if __name__ == "__main__":
    main()
