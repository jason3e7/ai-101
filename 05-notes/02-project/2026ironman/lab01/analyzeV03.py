#!/usr/bin/env python3
"""analyzeV03.py — blockquote (`> `) 密度訊號.

假設 (待驗證):
    AI 產生的 markdown 特別愛用 `> ` (blockquote) 排版, 放重點、警告、TL;DR
    等. 一般人寫作沒有這個習慣, 頂多引用他人文字才用 blockquote.

    測 `<blockquote>` 元素在正文的佔比 (字元) 與頻率 (per_1k).

指標:
    bq_count:  正文裡 `<blockquote>` 數量 (排除 `<pre>` 內)
    bq_chars:  所有 blockquote 內部字元加總 (去空白)
    chars:     全篇正文字元數 (去空白, 跟 V02 一致)
    ratio:     bq_chars / chars                    ← 主指標 (視覺佔比)
    per_1k:    bq_count / chars * 1000             ← 次指標 (頻率)

輸出:
    articles-v03.csv        每篇: bq_count / bq_chars / ratio / per_1k
    series-summary-v03.csv  每系列: 加總後的 ratio
    results-v03.md          ratio 最高的文章與系列排行

讀資料: 沿用 analyze.py 的 Raw class.

用法:
    python3 analyzeV03.py
    python3 analyzeV03.py --top 30
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

RE_BLOCKQUOTE = re.compile(r"(?is)<blockquote\b[^>]*>(.*?)</blockquote>")
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


def _inner_chars(raw):
    """blockquote 內部 raw HTML → 去標籤去空白後的字元數."""
    stripped = RE_TAG.sub("", raw)
    return len(re.sub(r"\s", "", htmlmod.unescape(stripped)))


def measure(body_html):
    """回傳 (bq_count, bq_chars, chars)."""
    body = strip_pre(body_html)
    bqs = RE_BLOCKQUOTE.findall(body)
    bq_chars = sum(_inner_chars(x) for x in bqs)
    text = html_to_text(body)
    chars = len(re.sub(r"\s", "", text))
    return len(bqs), bq_chars, chars


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
            bq_count, bq_chars, chars = measure(body)
            ratio = bq_chars / chars if chars else 0.0
            per_1k = bq_count / chars * SCALE if chars else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "bq_count": bq_count, "bq_chars": bq_chars, "chars": chars,
                "ratio": ratio, "pct": ratio * 100, "per_1k": per_1k,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v03.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "bq_count", "bq_chars", "chars",
              "ratio", "pct", "per_1k"]
    with open(os.path.join(HERE, "articles-v03.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["ratio"]):
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "pct": f"{r['pct']:.2f}",
                        "per_1k": f"{r['per_1k']:.2f}"})

    # series-summary-v03.csv (加總再除)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "bq_count": 0, "bq_chars": 0, "chars": 0, "per": []})
        g["articles"] += 1
        g["bq_count"] += r["bq_count"]
        g["bq_chars"] += r["bq_chars"]
        g["chars"] += r["chars"]
        g["per"].append(r["ratio"])
    srows = []
    for g in series.values():
        ratio = g["bq_chars"] / g["chars"] if g["chars"] else 0.0
        per_1k = g["bq_count"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "bq_count", "bq_chars", "chars")},
                      "ratio": ratio, "pct": ratio * 100, "per_1k": per_1k,
                      "median_article_pct": statistics.median(g["per"]) * 100})
    srows.sort(key=lambda r: -r["ratio"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "bq_count", "bq_chars", "chars",
               "ratio", "pct", "per_1k", "median_article_pct"]
    with open(os.path.join(HERE, "series-summary-v03.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "pct": f"{r['pct']:.2f}",
                        "per_1k": f"{r['per_1k']:.2f}",
                        "median_article_pct": f"{r['median_article_pct']:.2f}"})

    # results-v03.md
    tot_bq = sum(r["bq_count"] for r in rows)
    tot_bq_chars = sum(r["bq_chars"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    articles_with_bq = sum(1 for r in rows if r["bq_count"])
    top = sorted(rows, key=lambda r: -r["ratio"])[:args.top]
    L = [f"# lab01 V03 結果：blockquote (`> `) 密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式:",
         f"> ratio  = bq_chars / total_chars      (blockquote 佔全文的視覺比例)",
         f"> per_1k = bq_count / total_chars * 1000  (每千字幾個 blockquote)",
         f"> 排名不設字數門檻, 短文仍可能爆.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |"]
    if rows:
        L.append(f"| 有 blockquote 的文章 | {articles_with_bq}（{articles_with_bq / len(rows):.1%}） |")
    L += [f"| blockquote 總數 | {tot_bq} |",
          f"| blockquote 內字元總和 | {tot_bq_chars} |",
          f"| 全篇總字數 | {tot_chars} |"]
    if tot_chars:
        L += [f"| 全體 ratio (字元佔比) | {tot_bq_chars / tot_chars * 100:.2f}% |",
              f"| 全體 per_1k (頻率) | {tot_bq / tot_chars * SCALE:.2f} |"]
    L += ["",
          f"## ratio 最高的 {len(top)} 篇（不設字數門檻）",
          "",
          "| # | ratio% | per_1k | bq_count | bq_chars | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['pct']:.2f}% | {r['per_1k']:.2f} | "
                 f"{r['bq_count']} | {r['bq_chars']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## ratio 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | ratio% | 各篇中位數 | per_1k | 篇數 | bq_count | bq_chars | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['pct']:.2f}% | {r['median_article_pct']:.2f}% | "
                 f"{r['per_1k']:.2f} | {r['articles']} | {r['bq_count']} | {r['bq_chars']} | "
                 f"{r['chars']} | {st} | {r['group_name']} |")
    L += ["", "> 沒 blockquote (bq_count = 0) 的文章 ratio = 0, 全部沉底 (不代表沒 AI 味).",
          "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v03.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, "
          f"blockquote {tot_bq} 個 / 內字元 {tot_bq_chars}, 總字 {tot_chars}, "
          f"全體 ratio {tot_bq_chars/tot_chars*100:.2f}% / per_1k {tot_bq/tot_chars*SCALE:.2f}")
    print("輸出: articles-v03.csv, series-summary-v03.csv, results-v03.md")


if __name__ == "__main__":
    main()
