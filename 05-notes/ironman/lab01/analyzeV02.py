#!/usr/bin/env python3
"""analyzeV02.py — 計算每篇文章「條列項以粗體開頭 + 後接一般文字」的比例.

假設 (待驗證):
    AI 產生的 markdown 常出現以下 pattern:
        - **項目名**: 說明文字
        - **另一個標籤**: 更多說明
    這是 AI 最愛的「粗體標籤 + 一般文字說明」條列格式. 一般人寫技術文章比較少
    這樣做, 通常直接寫成 `- 項目名: 說明` 或用 heading. 所以 `<li>` 是否
    以 `<strong>...</strong> 一般字型` 開頭 (以及佔比多高), 可能是 AI 排版的訊號.

嚴格條件 (V02d 起加嚴, 排除誤判):
    `</strong>` 後面必須直接接**一般字型的可見字元** (中文、英數、標點) 才算命中.
    以下情況**不算**:
      - `<li><strong>單獨粗體</strong></li>`             — 沒有後續說明
      - `<li><strong>xxx</strong><a>連結</a></li>`       — 後面直接接標籤
      - `<li><strong>xxx</strong>: <code>abc</code></li>` — 後面接程式碼
    允許 `:` 或 `：` 或空白隔開後才進正文.

指標:
    total_li:   正文裡 `<li>` 數量 (排除 `<pre>` 程式碼)
    bold_li:    以 `<strong>...</strong> 一般字型` 開頭的 `<li>` 數量
                (允許 li 內先有 `<p>` 再進 strong, 這是常見的 renderer 差異)
    ratio:      bold_li / total_li (該篇條列項中「粗體標籤 + 一般文字說明」的比例)
    另附:       bold_li / 千字, 給不同文章長度做量級比較

輸出:
    articles-v02.csv        每篇: total_li / bold_li / ratio / 每千字
    series-summary-v02.csv  每系列: 加總後的 ratio
    results-v02.md          bold_li ratio 最高的文章與系列排行

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

# li 內: 允許先出現 <p> 再進 <strong>, 涵蓋不同 markdown renderer
# 嚴格版: <strong>...</strong> 後面必須接一般字元 (可先隔一個 : 或 ： 或空白)
#   `[^<>]+` 抓 strong 內部文字 (不含巢狀標籤)
#   `[：:\s]*` 允許中英文冒號或空白 (0 到多個)
#   `[^\s<]` 一定要接一個「非空白、非標籤起始」的字元 → 就是「一般字型可見字」
RE_LI = re.compile(r"<li\b[^>]*>", re.IGNORECASE)
RE_BOLD_LI = re.compile(
    r"<li\b[^>]*>\s*(?:<p\b[^>]*>\s*)?<strong\b[^>]*>[^<>]+</strong>[：:\s]*[^\s<]",
    re.IGNORECASE)
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
    """回傳 (total_li, bold_li, chars)."""
    body = strip_pre(body_html)
    total_li = len(RE_LI.findall(body))
    bold_li = len(RE_BOLD_LI.findall(body))
    text = html_to_text(body)
    chars = len(re.sub(r"\s", "", text))
    return total_li, bold_li, chars


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
            total_li, bold_li, chars = measure(body)
            ratio = bold_li / total_li if total_li else 0.0
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "total_li": total_li, "bold_li": bold_li, "chars": chars,
                "ratio": ratio, "bold_pct": ratio * 100,
                "bold_per_1k_chars": (bold_li / chars * SCALE) if chars else 0.0,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v02.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source",
              "total_li", "bold_li", "chars",
              "ratio", "bold_pct", "bold_per_1k_chars"]
    with open(os.path.join(HERE, "articles-v02.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["ratio"]):
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "bold_pct": f"{r['bold_pct']:.2f}",
                        "bold_per_1k_chars": f"{r['bold_per_1k_chars']:.2f}"})

    # series-summary-v02.csv (加總再除)
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0,
            "total_li": 0, "bold_li": 0, "chars": 0, "per": []})
        g["articles"] += 1
        g["total_li"] += r["total_li"]
        g["bold_li"] += r["bold_li"]
        g["chars"] += r["chars"]
        g["per"].append(r["ratio"])
    srows = []
    for g in series.values():
        ratio = g["bold_li"] / g["total_li"] if g["total_li"] else 0.0
        srows.append({**{k: g[k] for k in ("series_id", "series_title", "group_name", "articles",
                                            "total_li", "bold_li", "chars")},
                      "ratio": ratio, "bold_pct": ratio * 100,
                      "median_article_pct": statistics.median(g["per"]) * 100})
    srows.sort(key=lambda r: -r["ratio"])
    sfields = ["series_id", "series_title", "group_name", "articles",
               "total_li", "bold_li", "chars",
               "ratio", "bold_pct", "median_article_pct"]
    with open(os.path.join(HERE, "series-summary-v02.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "ratio": f"{r['ratio']:.6f}",
                        "bold_pct": f"{r['bold_pct']:.2f}",
                        "median_article_pct": f"{r['median_article_pct']:.2f}"})

    # results-v02.md
    tot_li = sum(r["total_li"] for r in rows)
    tot_bold = sum(r["bold_li"] for r in rows)
    tot_chars = sum(r["chars"] for r in rows)
    articles_with_list = sum(1 for r in rows if r["total_li"])
    articles_with_bold = sum(1 for r in rows if r["bold_li"])
    top = sorted(rows, key=lambda r: -r["ratio"])[:args.top]
    L = [f"# lab01 V02 結果：條列項「粗體標籤 + 一般文字」比例 (嚴格版)",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}。ratio = bold_li / total_li",
         f"> bold_li = 以 `<strong>...</strong>` 開頭, 且後面**直接接一般字型文字**的 `<li>`.",
         f"> 排除「只有粗體」、「粗體後接標籤 (連結/程式碼)」的情況. 排名不設條列數門檻, 短列表仍會爆.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(series)} |",
         f"| 文章數 | {len(rows)} |",
         f"| 有條列的文章 | {articles_with_list}（{articles_with_list / len(rows):.1%}） |" if rows else "",
         f"| 有粗體條列的文章 | {articles_with_bold}（{articles_with_bold / len(rows):.1%}） |" if rows else "",
         f"| 總條列項 (li) | {tot_li} |",
         f"| 粗體開頭條列項 | {tot_bold} |",
         f"| 全體比例 | {tot_bold / tot_li * 100:.2f}%" + " |" if tot_li else "",
         f"| 全體粗體條列每千字 | {tot_bold / tot_chars * SCALE:.2f}" + " |" if tot_chars else "",
         "",
         f"## 粗體條列比例最高的 {len(top)} 篇（不設條列數門檻）",
         "",
         "| # | 比例 | 粗體 li | 總 li | 字數 | 文章 | 系列 | 組別 |",
         "|---:|---:|---:|---:|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        L.append(f"| {i} | {r['bold_pct']:.2f}% | {r['bold_li']} | {r['total_li']} | "
                 f"{r['chars']} | [{t}]({r['url']}) | {st} | {r['group_name']} |")
    L += ["", f"## 粗體條列比例最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | 比例 | 各篇中位數 | 篇數 | 粗體 li | 總 li | 系列 | 組別 |",
          "|---:|---:|---:|---:|---:|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        L.append(f"| {i} | {r['bold_pct']:.2f}% | {r['median_article_pct']:.2f}% | "
                 f"{r['articles']} | {r['bold_li']} | {r['total_li']} | "
                 f"{r['series_title'].replace('|', chr(92) + '|')} | {r['group_name']} |")
    L += ["", "> 沒條列 (total_li = 0) 的文章 ratio = 0, 全部沉底 (不代表沒 AI 味).",
          "> 短列表 (1-3 個 li) 中 1 個粗體就是 33-100%, 排行看時要一起看「總 li」欄.",
          "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v02.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, "
          f"粗體 li {tot_bold}/{tot_li} = {tot_bold/tot_li*100:.2f}% (全體)")
    print("輸出: articles-v02.csv, series-summary-v02.csv, results-v02.md")


if __name__ == "__main__":
    main()
