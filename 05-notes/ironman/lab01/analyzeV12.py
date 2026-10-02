#!/usr/bin/env python3
"""analyzeV12.py — V06B 加三個 clean prose 訊號的 meta-composite.

公式延續 V06B 的 base = B_total × N_sum, 加入:
    - V08C 不是/不只是…而是/更是  (N=3)
    - V09  最容易...的              (N=3)
    - V11  標題含 ｜               (N=2, binary)

完整 N 設定:
    V06B 原有:
      em (V01) : N = 4
      emoji    : N = min(distinct_types, 5)   ← 上限 5, 超過當 5
      strict   : N = 3
      all      : N = 0 (不計 N_sum 但進 B_total)
      bq (V03) : N = 1
      hr (V04) : N = 1.5
    V12 新加:
      v08c     : N = 3
      v09      : N = 3
      v11      : N = 2 (binary, title 含 ｜ 加 2)

注意: 預設不設字數門檻 (min-chars = 0), 全部文章進榜.

公式:
    B_total = em + emoji + strict + all + bq + hr + v08c + v09 + v11
    N_sum   = Σ (Nᵢ where Bᵢ > 0)
    base    = B_total × N_sum
    density = base / chars × 1000

輸出:
    articles-v12.csv        每篇: 9 個 B + base + n_sum + density
    series-summary-v12.csv  每系列: 加總後 density
    results-v12.md          density 最高排行

用法:
    python3 analyzeV12.py
    python3 analyzeV12.py --top 30 --min-chars 500
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
MIN_CHARS = 0
EMOJI_TYPES_CAP = 5

RE_EM_DASH = re.compile("——")
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
EMOJI_EXCLUDE = frozenset("○✗★☆☐")
RE_ALL_STRONG = re.compile(r"<strong\b[^>]*>(.*?)</strong>", re.IGNORECASE | re.DOTALL)
RE_STRICT_BOLD_LI = re.compile(
    r"<li\b[^>]*>\s*(?:<p\b[^>]*>\s*)?<strong\b[^>]*>([^<>]+)</strong>[：:\s]*[^\s<]",
    re.IGNORECASE)
RE_BLOCKQUOTE = re.compile(r"(?is)<blockquote\b[^>]*>")
RE_HR = re.compile(r"(?i)<hr\b[^>]*/?>")
RE_V08C = re.compile(r"不(?:只)?是[^。！？\n]{1,25}(?:而是|更是)")
RE_V09 = re.compile(r"最容易[一-鿿]{1,5}的")
PIPE = "｜"  # U+FF5C

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


def measure(body_html, title):
    body = strip_pre(body_html)
    text = html_to_text(body)
    em_count = len(RE_EM_DASH.findall(text))
    emojis = [c for c in RE_EMOJI.findall(text) if c not in EMOJI_EXCLUDE]
    emoji_count = len(emojis)
    emoji_types = len(set(emojis))
    all_bold = len(RE_ALL_STRONG.findall(body))
    strict_bold = len(RE_STRICT_BOLD_LI.findall(body))
    bq_count = len(RE_BLOCKQUOTE.findall(body))
    hr_count = len(RE_HR.findall(body))
    v08c_count = len(RE_V08C.findall(text))
    v09_count = len(RE_V09.findall(text))
    v11_has_pipe = 1 if PIPE in title else 0
    chars = len(re.sub(r"\s", "", text))
    return {
        "em_B": em_count, "emoji_B": emoji_count, "emoji_types": emoji_types,
        "strict_B": strict_bold, "all_B": all_bold,
        "bq_B": bq_count, "hr_B": hr_count,
        "v08c_B": v08c_count, "v09_B": v09_count, "v11_B": v11_has_pipe,
        "chars": chars,
    }


def compute_v12(m):
    b_total = (m["em_B"] + m["emoji_B"] + m["strict_B"] + m["all_B"]
               + m["bq_B"] + m["hr_B"] + m["v08c_B"] + m["v09_B"] + m["v11_B"])
    emoji_n = min(m["emoji_types"], EMOJI_TYPES_CAP) if m["emoji_B"] > 0 else 0
    n_sum = ((4 if m["em_B"] > 0 else 0)
             + emoji_n
             + (3 if m["strict_B"] > 0 else 0)
             + (0 if m["all_B"] > 0 else 0)
             + (1 if m["bq_B"] > 0 else 0)
             + (1.5 if m["hr_B"] > 0 else 0)
             + (3 if m["v08c_B"] > 0 else 0)
             + (3 if m["v09_B"] > 0 else 0)
             + (2 if m["v11_B"] > 0 else 0))
    return b_total, n_sum, b_total * n_sum


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
            title = a.get("title", "")
            m = measure(page_body(block), title)
            b_total, n_sum, base = compute_v12(m)
            density = base / m["chars"] * SCALE if m["chars"] else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": title,
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                **m, "b_total": b_total, "n_sum": n_sum, "base": base, "density": density,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source", "chars",
              "em_B", "emoji_B", "strict_B", "all_B", "bq_B", "hr_B",
              "v08c_B", "v09_B", "v11_B",
              "b_total", "n_sum", "base", "density"]
    with open(os.path.join(HERE, "articles-v12.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["density"]):
            w.writerow({**r, "density": f"{r['density']:.4f}"})

    # series summary
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0, "chars": 0, "base": 0,
            **{f"{k}_B": 0 for k in ["em", "emoji", "strict", "all", "bq", "hr", "v08c", "v09", "v11"]},
            "per": []})
        g["articles"] += 1
        g["chars"] += r["chars"]
        g["base"] += r["base"]
        for k in ["em", "emoji", "strict", "all", "bq", "hr", "v08c", "v09", "v11"]:
            g[f"{k}_B"] += r[f"{k}_B"]
        g["per"].append(r["density"])
    srows = []
    for g in series.values():
        density = g["base"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in g if k != "per"},
                      "density": density,
                      "median_article_density": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["density"])
    sfields = ["series_id", "series_title", "group_name", "articles", "chars",
               "em_B", "emoji_B", "strict_B", "all_B", "bq_B", "hr_B",
               "v08c_B", "v09_B", "v11_B",
               "base", "density", "median_article_density"]
    with open(os.path.join(HERE, "series-summary-v12.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields, extrasaction="ignore")
        w.writeheader()
        for r in srows:
            w.writerow({**r, "density": f"{r['density']:.4f}",
                        "median_article_density": f"{r['median_article_density']:.4f}"})

    tot_base = sum(r["base"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    qualified = [r for r in rows if r["chars"] >= args.min_chars]
    skipped = len(rows) - len(qualified)
    top = sorted(qualified, key=lambda r: -r["density"])[:args.top]

    L = [f"# lab01 V12 結果：V06B 加 V08C/V09/V11 的 meta-composite",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}.",
         f"> 公式: base = B_total × N_sum, density = base / chars × 1000",
         f"> V06B 骨架 (em 4 / emoji min(types,{EMOJI_TYPES_CAP}) / strict 3 / all 0 / bq 1 / hr 1.5)",
         f"> 新加 signals: V08C N=3 (不是…而是), V09 N=3 (最容易...的), V11 N=2 (title ｜)",
         f"> 字數門檻 = {args.min_chars} (0 = 不排除短文)" + (f", 排除 {skipped} 篇" if skipped else ""),
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| base 總和 | {tot_base:,.0f} |",
         f"| 全篇總字數 | {tot_chars:,} |"]
    if tot_chars:
        L.append(f"| 全體 density | {tot_base / tot_chars * SCALE:.4f} |")
    L += ["",
          f"## density 最高的 {len(top)} 篇 (chars >= {args.min_chars})",
          "",
          "| # | density | base | B_total | N_sum | em | emj | str | all | bq | hr | v08c | v09 | v11 | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['base']:.0f} | {r['b_total']} | {r['n_sum']:.1f} | "
                 f"{r['em_B']} | {r['emoji_B']}({r['emoji_types']}) | {r['strict_B']} | {r['all_B']} | "
                 f"{r['bq_B']} | {r['hr_B']} | {r['v08c_B']} | {r['v09_B']} | {r['v11_B']} | "
                 f"{r['chars']} | [{t}]({r['url']}) | {st} | {r['group_name']} |")

    L += ["", f"## density 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | density | 各篇中位數 | 篇數 | em | emj | str | all | bq | hr | v08c | v09 | v11 | base | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['median_article_density']:.2f} | "
                 f"{r['articles']} | {r['em_B']} | {r['emoji_B']} | {r['strict_B']} | {r['all_B']} | "
                 f"{r['bq_B']} | {r['hr_B']} | {r['v08c_B']} | {r['v09_B']} | {r['v11_B']} | "
                 f"{r['base']:.0f} | {r['chars']} | {st} | {r['group_name']} |")

    L += ["", "> V12 = V06B 骨架 + 3 個 clean prose signal (V08C/V09) + 1 個 title signal (V11).",
          "> 這是共現訊號的加權排序, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v12.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, base 總和 {tot_base:,.0f}, "
          f"總字 {tot_chars:,}, 全體 density {tot_base/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v12.csv, series-summary-v12.csv, results-v12.md")


if __name__ == "__main__":
    main()
