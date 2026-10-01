#!/usr/bin/env python3
"""analyzeV06B.py — 綜合訊號分數 (和的乘積型), V06 的姊妹算法.

跟 analyzeV06.py 用同一組測量 (em / emoji / strict / all / bq / hr), 差別在
base 公式:

    V06  current: base = Σᵢ (Bᵢ × Nᵢ)        每個 signal 自己的 N 乘上自己的 B
    V06B (本份): base = (ΣBᵢ) × (ΣNᵢ)        痕跡總量 × 權重 budget

V06B 的動機:
    V06 下 V02 all N=0 → V02 all 命中再多也完全不影響分數. 但「文章有大量粗體」
    其實是 AI markdown 排版的一部分, 不該完全消失. V06B 把 all 的 count 也加
    進 B_total, 讓它作為「痕跡放大器」— 單獨出現仍不計分 (N_sum=0), 但配合
    其他 signal 命中時能放大整體 base.

公式細節:
    B_total = em + emoji + strict + all + bq + hr          所有 signal 的 count 加總
    N_sum   = Σ (Nᵢ where Bᵢ > 0)                          只對命中的 signal 加 N
            = (4 if em>0) + (emoji_types if emoji>0) + (2 if strict>0)
              + (0 if all>0) + (1 if bq>0) + (1 if hr>0)
    base    = B_total × N_sum
    density = base / chars × 1000  (per_1k 型)

跟 V06 的語意差別 (舉例):
    1 em + 50 all:
        V06  base = 4
        V06B base = (1+50) × 4 = 204            ← all 把 B_total 推高, 乘上 N_sum=4
    50 all, 其他 0:
        V06  base = 0
        V06B base = 50 × 0 = 0                  ← all 單獨仍不計分
    emoji 多樣 (types=15), 其他小:
        V06  強調 emoji 自己貢獻 (15 × count)
        V06B emoji types 進 N_sum, 放大「整個 B_total」

輸出:
    articles-v06b.csv        每篇: 各 signal B + n_sum + B_total + base + density
    series-summary-v06b.csv  每系列: 加總後 density
    results-v06b.md          density 最高的文章 / 系列排行

用法:
    python3 analyzeV06B.py
    python3 analyzeV06B.py --top 30
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
    em_count = len(RE_EM_DASH.findall(text))
    emojis = [c for c in RE_EMOJI.findall(text) if c not in EMOJI_EXCLUDE]
    emoji_count = len(emojis)
    emoji_types = len(set(emojis))
    all_bold = len(RE_ALL_STRONG.findall(body))
    strict_bold = len(RE_STRICT_BOLD_LI.findall(body))
    bq_count = len(RE_BLOCKQUOTE.findall(body))
    hr_count = len(RE_HR.findall(body))
    chars = len(re.sub(r"\s", "", text))
    return {
        "em_count": em_count,
        "emoji_count": emoji_count, "emoji_types": emoji_types,
        "all_bold": all_bold, "strict_bold": strict_bold,
        "bq_count": bq_count, "hr_count": hr_count,
        "chars": chars,
    }


def compute_v06b(m):
    """V06B: base = B_total × N_sum."""
    b_total = m["em_count"] + m["emoji_count"] + m["strict_bold"] + m["all_bold"] + m["bq_count"] + m["hr_count"]
    n_sum = ((4 if m["em_count"] > 0 else 0)
             + (m["emoji_types"] if m["emoji_count"] > 0 else 0)
             + (2 if m["strict_bold"] > 0 else 0)
             + (0 if m["all_bold"] > 0 else 0)
             + (1 if m["bq_count"] > 0 else 0)
             + (1 if m["hr_count"] > 0 else 0))
    return b_total, n_sum, b_total * n_sum


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
            m = measure(page_body(block))
            b_total, n_sum, base = compute_v06b(m)
            density = base / m["chars"] * SCALE if m["chars"] else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                **m,
                "em_B": m["em_count"],
                "emoji_B": m["emoji_count"],
                "strict_B": m["strict_bold"],
                "all_B": m["all_bold"],
                "bq_B": m["bq_count"],
                "hr_B": m["hr_count"],
                "b_total": b_total, "n_sum": n_sum,
                "base": base, "density": density,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "chars",
              "em_B", "emoji_B", "strict_B", "all_B", "bq_B", "hr_B",
              "b_total", "n_sum",
              "base", "density"]
    with open(os.path.join(HERE, "articles-v06b.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["density"]):
            w.writerow({**r, "density": f"{r['density']:.4f}"})

    # series-summary-v06b.csv (加總再除)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "em_count": 0, "emoji_count": 0, "strict_bold": 0, "all_bold": 0,
            "bq_count": 0, "hr_count": 0, "chars": 0, "base": 0, "per": []})
        g["articles"] += 1
        g["em_count"] += r["em_count"]
        g["emoji_count"] += r["emoji_count"]
        g["strict_bold"] += r["strict_bold"]
        g["all_bold"] += r["all_bold"]
        g["bq_count"] += r["bq_count"]
        g["hr_count"] += r["hr_count"]
        g["chars"] += r["chars"]
        g["base"] += r["base"]
        g["per"].append(r["density"])
    srows = []
    for g in series.values():
        density = g["base"] / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "em_count", "emoji_count",
                                            "strict_bold", "all_bold", "bq_count", "hr_count",
                                            "chars", "base")},
                      "density": density,
                      "median_article_density": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["density"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "em_count", "emoji_count", "strict_bold", "all_bold",
               "bq_count", "hr_count", "chars", "base", "density", "median_article_density"]
    with open(os.path.join(HERE, "series-summary-v06b.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "density": f"{r['density']:.4f}",
                        "median_article_density": f"{r['median_article_density']:.4f}"})

    # results-v06b.md
    tot_base = sum(r["base"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    top = sorted(rows, key=lambda r: -r["density"])[:args.top]
    L = [f"# lab01 V06B 結果：綜合訊號分數 (B 總和 × N 總和型)",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式:",
         f"> B_total = em + emoji + strict + all + bq + hr",
         f"> N_sum   = Σ (Nᵢ where Bᵢ > 0)",
         f"> base    = B_total × N_sum",
         f"> density = base / total_chars × 1000  (per_1k 型)",
         f"> V02 all 的 N=0 仍進 B_total 當放大器, 但單獨命中 N_sum=0 不計分.",
         f"> 跟 V06 (每個 signal 自己 B×N 加總) 比較見 lab01/v06-vs-v06b.md.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| base 總和 | {tot_base:,} |",
         f"| 全篇總字數 | {tot_chars:,} |"]
    if tot_chars:
        L.append(f"| 全體 density | {tot_base / tot_chars * SCALE:.4f} |")
    L += ["",
          f"## density 最高的 {len(top)} 篇（不設字數門檻）",
          "",
          "| # | density | base | B_total | N_sum | —— | emj(種) | strict | all | bq | hr | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['base']} | {r['b_total']} | {r['n_sum']} | "
                 f"{r['em_count']} | {r['emoji_count']}({r['emoji_types']}) | "
                 f"{r['strict_bold']} | {r['all_bold']} | {r['bq_count']} | {r['hr_count']} | "
                 f"{r['chars']} | [{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## density 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | density | 各篇中位數 | 篇數 | —— | emoji | strict | all | bq | hr | base | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['median_article_density']:.2f} | "
                 f"{r['articles']} | {r['em_count']} | {r['emoji_count']} | "
                 f"{r['strict_bold']} | {r['all_bold']} | {r['bq_count']} | {r['hr_count']} | "
                 f"{r['base']} | {r['chars']} | {st} | {r['group_name']} |")
    L += ["", "> 這是共現訊號的加權排序, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v06b.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, base 總和 {tot_base:,}, "
          f"總字 {tot_chars:,}, 全體 density {tot_base/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v06b.csv, series-summary-v06b.csv, results-v06b.md")


if __name__ == "__main__":
    main()
