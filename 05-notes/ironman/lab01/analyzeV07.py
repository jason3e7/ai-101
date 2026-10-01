#!/usr/bin/env python3
"""analyzeV07.py — 中文冗詞測試: 「最...的」形容密度.

假設 (待驗證):
    AI 寫中文技術文章特別愛用「最...的」superlative pattern
    (最重要的 / 最乾淨的 / 最有效的 / 最危險的 / 最好的 / ...).
    人類寫作偶爾會用, AI 傾向反覆使用強調語氣. 這類 superlative
    會讓文風「專家腔」, 是中文版的 "crucial/pivotal/essential" 的
    常見變體.

    對照組 V06B: 若「最...的」per_1k 跟 V06B density 正相關 → AI 用
    這個 pattern 比較多. 若無關 → 不是強 tell.

Regex 設計:
    pattern = 最[一-鿿]{2,5}的
    匹配: 最 + 2-5 個中文字 + 的 (總長 4-7 字)
    範例命中: 最重要的 / 最乾淨的 / 最有價值的 / 最常成功的 / 最後發生的
    刻意排除 3 字組合 (最+1字+的): 最好的 / 最大的 / 最後的 / 最高的 / 最小的
      因為這些是中文自然用詞 (甚至中性), 不是強 AI tell. 排除後抓到的 4-7 字
      superlative 比較偏向 AI 的「正式論述語氣」(最常成功的 / 最後發生的).

加權公式 (靈感來自 V05 emoji count × types):
    length_weight(match) = len(match) - 3     # 4字=1, 5字=2, 6字=3, 7字=4
    hit_sum  = Σ length_weight(m) for each match m    # 越長命中加權越大
    distinct = unique matches 數                       # 越多樣加權越大
    base     = hit_sum × distinct                      # 兩個加權相乘
    density  = base / chars * 1000                     # 主指標 (per_1k 型)

指標:
    hits:        正文裡 regex 命中次數 (去 <pre>, 去 HTML 標籤後計)
    hit_sum:     長度加權後的命中總和
    distinct:    unique match 數
    chars:       全篇正文字元數 (去空白, 跟 V01-V06 一致)
    base:        hit_sum × distinct
    density:     base / chars * 1000       ← 主指標

輸出:
    articles-v07.csv        每篇: hits / chars / per_1k
    series-summary-v07.csv  每系列: 加總後 per_1k
    results-v07.md          per_1k 最高的文章與系列排行

讀資料: 沿用 analyze.py 的 Raw class.

用法:
    python3 analyzeV07.py
    python3 analyzeV07.py --top 30
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

TARGET_DESC = "最...的 (regex: 最[一-鿿]{2,5}的, 排除 3 字如最好的)"
RE_TARGET = re.compile(r"最[一-鿿]{2,5}的")

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
    body = strip_pre(body_html)
    text = html_to_text(body)
    matches = RE_TARGET.findall(text)
    hits = len(matches)
    hit_sum = sum(len(m) - 3 for m in matches)   # 4字=1, 5=2, 6=3, 7=4
    distinct = len(set(matches))
    chars = len(re.sub(r"\s", "", text))
    return hits, hit_sum, distinct, chars


MIN_CHARS = 500  # 排行榜最小字數門檻, 排除短文灌水 false positive


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=20)
    ap.add_argument("--min-chars", type=int, default=MIN_CHARS,
                    help=f"排行榜最小字數門檻 (default {MIN_CHARS}). CSV 仍包含全部文章")
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
            hits, hit_sum, distinct, chars = measure(page_body(block))
            base = hit_sum * distinct
            density = base / chars * SCALE if chars else 0.0
            per_1k = hits / chars * SCALE if chars else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "hits": hits, "hit_sum": hit_sum, "distinct": distinct,
                "chars": chars, "base": base,
                "per_1k": per_1k, "density": density,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "hits", "hit_sum", "distinct", "chars", "base", "per_1k", "density"]
    with open(os.path.join(HERE, "articles-v07.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["density"]):
            w.writerow({**r, "per_1k": f"{r['per_1k']:.4f}",
                        "density": f"{r['density']:.4f}"})

    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "hits": 0, "chars": 0, "base": 0, "per": []})
        g["articles"] += 1
        g["hits"] += r["hits"]
        g["chars"] += r["chars"]
        g["base"] += r["base"]
        g["per"].append(r["density"])
    srows = []
    for g in series.values():
        density = g["base"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "hits", "chars", "base")},
                      "density": density,
                      "median_article_density": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["density"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "hits", "chars", "base", "density", "median_article_density"]
    with open(os.path.join(HERE, "series-summary-v07.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "density": f"{r['density']:.4f}",
                        "median_article_density": f"{r['median_article_density']:.4f}"})

    tot_hits = sum(r["hits"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    articles_with_hit = sum(1 for r in rows if r["hits"] > 0)
    tot_base = sum(r["base"] for r in rows)
    # 排行榜只收 chars >= min_chars 的文章, 排除短文灌水 false positive
    qualified = [r for r in rows if r["chars"] >= args.min_chars]
    skipped = len(rows) - len(qualified)
    top = sorted(qualified, key=lambda r: -r["density"])[:args.top]
    L = [f"# lab01 V07 結果：中文「{TARGET_DESC}」加權密度",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式:",
         f"> length_weight(m) = len(m) - 3   (4字=1, 5字=2, 6字=3, 7字=4)",
         f"> hit_sum  = Σ length_weight(m);  distinct = unique match 數",
         f"> base    = hit_sum × distinct     (越長 + 越多樣 加權越大)",
         f"> density = base / chars × 1000    ← 主指標 (per_1k 型)",
         f"> Pattern: {TARGET_DESC}. 範例命中: 最常成功的 / 最後發生的 / 最不可或缺的",
         f"> 假設 AI 愛用 superlative pattern 強調語氣. 對照組 V06B.",
         f"> **排行榜最小字數門檻 {args.min_chars} 字** (CSV 仍包含全部文章). 排除 {skipped} 篇短文.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |"]
    if rows:
        L.append(f"| 有命中的文章 | {articles_with_hit}（{articles_with_hit / len(rows):.1%}） |")
    L += [f"| 命中總次數 (raw hits) | {tot_hits:,} |",
          f"| base 總和 (hit_sum × distinct) | {tot_base:,} |",
          f"| 全篇總字數 | {tot_chars:,} |"]
    if tot_chars:
        L.append(f"| 全體 raw per_1k | {tot_hits / tot_chars * SCALE:.4f} |")
        L.append(f"| 全體 density (加權) | {tot_base / tot_chars * SCALE:.4f} |")
    L.append(f"| 排行榜門檻後文章 | {len(qualified)} (排除 {skipped} 篇 < {args.min_chars} 字) |")
    L += ["",
          f"## density (加權) 最高的 {len(top)} 篇 (chars >= {args.min_chars})",
          "",
          "| # | density | base | hit_sum | distinct | hits | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['base']} | {r['hit_sum']} | "
                 f"{r['distinct']} | {r['hits']} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## density (加權) 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | density | 各篇中位數 | 篇數 | base | hits | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['median_article_density']:.2f} | "
                 f"{r['articles']} | {r['base']} | {r['hits']} | {r['chars']} | "
                 f"{st} | {r['group_name']} |")
    L += ["", "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v07.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, 「{TARGET_DESC}」raw hits {tot_hits:,}, "
          f"base 總和 {tot_base:,}, 總字 {tot_chars:,}, "
          f"全體 density {tot_base/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v07.csv, series-summary-v07.csv, results-v07.md")


if __name__ == "__main__":
    main()
