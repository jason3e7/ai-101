#!/usr/bin/env python3
"""analyzeV06.py — 綜合訊號分數 (絕對值加權型).

假設 (待驗證):
    多個訊號一起看比單一訊號準. 各訊號用「命中次數 × 權重」相加成 base,
    再除總字數化成密度 (per_1k), 就是「AI 味濃度」的一個直觀分數.

公式:
    base(article)
        = em_count × 4                      # V01 —— (最乾淨的 tell)
        + emoji_count × emoji_types         # V05 emoji (種類越多越 AI, 排除 ○ ✗ ★ ☆ ☐)
        + strict_bold × 2                   # V02 嚴格 `- **標籤**: xxx`
        + all_bold × 1                      # V02 一般 `**標籤**`  (strict 是 all 的子集, 所以 strict 命中會多算一次)
        + bq_count × 1                      # V03 blockquote
        + hr_count × 1                      # V04 <hr>

    density(article) = base / chars × 1000   ← 主指標 (per_1k 型)

範圍:
    strict 是 all 的子集 → strict 命中會被 all 也計到, 等於 strict 效果總權重 × 3.
    這是刻意加倍計 (延續 V02e 的 strict 加算精神).

    emoji 排除清單延續 V05: ○ ✗ ★ ☆ ☐ (checklist / 星等 / 圈叉 排版符號).
    emoji 種類數只算「非排除」的 distinct codepoint.

輸出:
    articles-v06.csv        每篇: 各訊號 count + base + density (per_1k)
    series-summary-v06.csv  每系列: 加總後的 density
    results-v06.md          density 最高的文章與系列排行 (含各訊號分解)

讀資料: 沿用 analyze.py 的 Raw class.

用法:
    python3 analyzeV06.py
    python3 analyzeV06.py --top 30
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

# 訊號 regex
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


def score_base(m):
    return (m["em_count"] * 4
            + m["emoji_count"] * m["emoji_types"]
            + m["strict_bold"] * 2
            + m["all_bold"] * 1
            + m["bq_count"] * 1
            + m["hr_count"] * 1)


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
            base = score_base(m)
            density = base / m["chars"] * SCALE if m["chars"] else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                **m, "base": base, "density": density,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v06.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "chars",
              "em_count", "emoji_count", "emoji_types",
              "strict_bold", "all_bold", "bq_count", "hr_count",
              "base", "density"]
    with open(os.path.join(HERE, "articles-v06.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["density"]):
            w.writerow({**r, "density": f"{r['density']:.4f}"})

    # series-summary-v06.csv
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "em_count": 0, "emoji_count": 0, "emoji_types_sum": 0,
            "strict_bold": 0, "all_bold": 0, "bq_count": 0, "hr_count": 0,
            "chars": 0, "base": 0, "per": []})
        g["articles"] += 1
        g["em_count"] += r["em_count"]
        g["emoji_count"] += r["emoji_count"]
        g["emoji_types_sum"] += r["emoji_types"]
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
                                            "em_count", "emoji_count", "emoji_types_sum",
                                            "strict_bold", "all_bold", "bq_count", "hr_count",
                                            "chars", "base")},
                      "density": density,
                      "median_article_density": statistics.median(g["per"])})
    srows.sort(key=lambda r: -r["density"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "em_count", "emoji_count", "emoji_types_sum",
               "strict_bold", "all_bold", "bq_count", "hr_count",
               "chars", "base", "density", "median_article_density"]
    with open(os.path.join(HERE, "series-summary-v06.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "density": f"{r['density']:.4f}",
                        "median_article_density": f"{r['median_article_density']:.4f}"})

    # results-v06.md
    tot_em = sum(r["em_count"] for r in rows)
    tot_emoji = sum(r["emoji_count"] for r in rows)
    tot_strict = sum(r["strict_bold"] for r in rows)
    tot_all = sum(r["all_bold"] for r in rows)
    tot_bq = sum(r["bq_count"] for r in rows)
    tot_hr = sum(r["hr_count"] for r in rows)
    tot_base = sum(r["base"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    top = sorted(rows, key=lambda r: -r["density"])[:args.top]
    L = [f"# lab01 V06 結果：綜合訊號分數 (絕對值加權型)",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}. 公式:",
         f"> base = ——×4 + emoji_count×emoji_types + strict×2 + all×1 + bq×1 + hr×1",
         f"> density = base / total_chars × 1000  (per_1k 型)",
         f"> emoji 排除清單: ○ ✗ ★ ☆ ☐. 排名不設字數門檻, 短文仍可能爆.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| —— 總數 | {tot_em} |",
         f"| emoji 總數 (排除後) | {tot_emoji} |",
         f"| 嚴格粗體命中 | {tot_strict} |",
         f"| 一般粗體命中 | {tot_all} |",
         f"| blockquote 總數 | {tot_bq} |",
         f"| `<hr>` 總數 | {tot_hr} |",
         f"| base 總和 | {tot_base} |",
         f"| 全篇總字數 | {tot_chars} |"]
    if tot_chars:
        L.append(f"| 全體 density | {tot_base / tot_chars * SCALE:.4f} |")
    L += ["",
          f"## density 最高的 {len(top)} 篇（不設字數門檻）",
          "",
          "| # | density | base | —— | emj(種) | strict | all | bq | hr | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['density']:.2f} | {r['base']} | {r['em_count']} | "
                 f"{r['emoji_count']}({r['emoji_types']}) | "
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
    L += ["", "> 表頭縮寫: emj(種) = emoji_count (distinct types), strict = V02 嚴格 pattern 命中, all = V02 一般粗體命中.",
          "> 這是共現訊號的加權排序, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v06.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, base 總和 {tot_base}, "
          f"總字 {tot_chars}, 全體 density {tot_base/tot_chars*SCALE:.4f}" if tot_chars else "")
    print("輸出: articles-v06.csv, series-summary-v06.csv, results-v06.md")


if __name__ == "__main__":
    main()
