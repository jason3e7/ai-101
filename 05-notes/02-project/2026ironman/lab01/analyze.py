#!/usr/bin/env python3
"""analyze.py — 計算每篇文章的雙破折號 `——` 密度.

指標 (見 README「指標」段):
    密度 = 命中次數 / 總字數
    命中次數: 正文裡 `——` 出現幾次 (兩個 U+2014 相連算 1 次)
    總字數:   正文去掉空白後的字元數 (中文、英數、標點各算 1)
    範圍:     只算正文; 標題不在正文裡, 程式碼區塊 (<pre>) 整段排除

輸出:
    articles.csv        每篇: 命中次數、總字數、原始比值、每千字
    series-summary.csv  每系列: 加總後的比值 (命中總和 / 字數總和) 與各篇中位數
    results.md          原始比值最高的文章與系列排行

讀資料: 優先讀 raw/ (解壓後), 沒有就直接讀 raw.tgz, 不必先解壓.
內文來源: raw/pages/{aid}.html (fetch.py 從文章頁切出的正文區塊).
    不用 RSS 內文: RSS 會濾掉 —、「」 等標點, 拿來算會全部變 0.

用法:
    python3 analyze.py
    python3 analyze.py --top 30      # 排行榜列幾名 (預設 20)
"""

import argparse
import csv
import html as htmlmod
import io
import json
import os
import re
import statistics
import tarfile

HERE = os.path.dirname(os.path.abspath(__file__))
RAW_DIR = os.path.join(HERE, "raw")
RAW_TGZ = os.path.join(HERE, "raw.tgz")
FEATURE = "——"          # U+2014 U+2014
SCALE = 1000            # 原始比值很小, 乘 1000 = 「每千字 X 次」
BASE = "https://ithelp.ithome.com.tw"
# 正文之後第一個出現的區塊 (按讚/留言列、系列上下篇、頁尾) → 正文到此為止
END_MARKERS = ("qa-action", "article-series-page", "ir-article__footer", "qa-panel")


class Raw:
    """統一讀取 raw/ 資料夾或 raw.tgz."""

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


# ---------- 正文 → 純文字 ----------

def html_to_text(body):
    body = re.sub(r"(?is)<(script|style)\b.*?</\1>", " ", body)
    body = re.sub(r"(?is)<pre\b.*?</pre>", " ", body)        # 程式碼區塊整段排除
    body = re.sub(r"(?s)<[^>]+>", " ", body)
    return htmlmod.unescape(body)


def article_page_body(page):
    """文章頁 HTML → 正文 HTML (markdown__style 區塊到系列目錄前)."""
    i = page.find("markdown__style")
    if i < 0:
        return ""
    i = page.find(">", i) + 1
    ends = [page.rfind("<", i, j) for j in (page.find(k, i) for k in END_MARKERS) if j > 0]
    return page[i:min(ends)] if ends else page[i:]


def measure(text):
    hits = text.count(FEATURE)
    chars = len(re.sub(r"\s", "", text))
    ratio = hits / chars if chars else 0.0
    return hits, chars, ratio


# ---------- 主流程 ----------

def page_body(block):
    """raw/pages 的區塊 → 只取正文 div (排除標題 h2)."""
    i = block.find('class="lab01-body"')
    if i < 0:
        return article_page_body(block)     # 相容: 若存的是完整文章頁
    i = block.find(">", i) + 1
    j = block.rfind("</div>")
    return block[i:j if j > i else len(block)]


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
            hits, chars, ratio = measure(html_to_text(body))
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "hits": hits, "chars": chars, "ratio": ratio, "per_1k": ratio * SCALE,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source", "hits", "chars", "ratio", "per_1k"]
    with open(os.path.join(HERE, "articles.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["ratio"]):
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}", "per_1k": f"{r['per_1k']:.2f}"})

    # series-summary.csv (加總再除 + 各篇中位數)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {"series_id": r["series_id"], "series_title": r["series_title"],
                                               "group_name": r["group_name"], "articles": 0, "hits": 0,
                                               "chars": 0, "per": []})
        g["articles"] += 1
        g["hits"] += r["hits"]
        g["chars"] += r["chars"]
        g["per"].append(r["ratio"])
    srows = []
    for g in series.values():
        ratio = g["hits"] / g["chars"] if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles", "hits", "chars")},
                      "ratio": ratio, "per_1k": ratio * SCALE,
                      "median_article_per_1k": statistics.median(g["per"]) * SCALE})
    srows.sort(key=lambda r: -r["ratio"])
    sfields = ["series_id", "series_title", "group_name", "articles", "hits", "chars",
               "ratio", "per_1k", "median_article_per_1k"]
    with open(os.path.join(HERE, "series-summary.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}", "per_1k": f"{r['per_1k']:.2f}",
                        "median_article_per_1k": f"{r['median_article_per_1k']:.2f}"})

    # results.md
    tot_hits = sum(r["hits"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    hit_articles = sum(1 for r in rows if r["hits"])
    top = sorted(rows, key=lambda r: -r["ratio"])[:args.top]
    L = [f"# lab01 結果：雙破折號 `——` 密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}。密度 = 命中次數 / 總字數；「每千字」= 原始比值 × {SCALE}。",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| 有出現 `——` 的文章 | {hit_articles}（{hit_articles / len(rows):.1%}） |" if rows else "",
         f"| 命中總次數 | {tot_hits} |",
         f"| 總字數 | {tot_chars} |",
         f"| 全體原始比值 | {tot_hits / tot_chars:.6f} |" if tot_chars else "",
         f"| 全體每千字 | {tot_hits / tot_chars * SCALE:.2f} |" if tot_chars else "",
         "",
         f"## 原始比值最高的 {len(top)} 篇（不設字數門檻）",
         "",
         "| # | 原始比值 | 每千字 | 命中 | 字數 | 文章 | 系列 | 組別 |",
         "|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['ratio']:.6f} | {r['per_1k']:.2f} | {r['hits']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## 原始比值最高的 {min(args.top, len(srows))} 個系列（命中總和 / 字數總和）", "",
          "| # | 原始比值 | 每千字 | 各篇中位數（每千字） | 篇數 | 命中 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        L.append(f"| {i} | {r['ratio']:.6f} | {r['per_1k']:.2f} | {r['median_article_per_1k']:.2f} | "
                 f"{r['articles']} | {r['hits']} | {r['series_title'].replace('|', chr(92) + '|')} | {r['group_name']} |")
    L += ["", "> 這是共現訊號，用來排序，不是判決。`——` 多不代表一定是 AI 寫的。", ""]
    with open(os.path.join(HERE, "results.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, 命中 {tot_hits} 次, 總字數 {tot_chars}")
    print("輸出: articles.csv, series-summary.csv, results.md")


if __name__ == "__main__":
    main()
