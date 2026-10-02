#!/usr/bin/env python3
"""analyzeV11.py — 標題含 ｜ (U+FF5C 全形直線) 的分類統計.

假設:
    AI 生成標題特別愛用 `｜` 當分隔符, 例如:
      「Day 1｜我為什麼要教最愛的人用 Google AI？」
      「Day 02｜從 0 想法到使用 ChatGPT&Codex 開發 App」
    人類寫標題多用全形冒號 ：、半形冒號 :、全形空格、或純文字分段.
    ｜ 是 AI 特別愛用的 cosmetic separator.

    這是 title-level 分類訊號 (binary per article, pct per series),
    跟 V01-V10 的 per_1k 密度不一樣.

指標:
    has_pipe:   title 含 ｜ → 1, 否則 0
    pipe_pct:   series 中含 ｜ 標題的篇數 / 總篇數

輸出:
    articles-v11.csv        每篇: title + has_pipe
    series-summary-v11.csv  每系列: 篇數 / pipe 篇數 / pipe_pct
    results-v11.md          總覽 + 全 ｜ 系列 (pct=100%) + 完全不用的系列

用法:
    python3 analyzeV11.py
    python3 analyzeV11.py --top 30
"""

import argparse
import csv
import json
import os
import statistics
import tarfile

HERE = os.path.dirname(os.path.abspath(__file__))
RAW_DIR = os.path.join(HERE, "raw")
RAW_TGZ = os.path.join(HERE, "raw.tgz")
BASE = "https://ithelp.ithome.com.tw"
TARGET = "｜"  # U+FF5C


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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--top", type=int, default=30)
    args = ap.parse_args()

    raw = Raw()
    idx = json.loads(raw.read("index.json"))
    print(f"讀取來源: {raw.src} (抓取時間 {idx.get('fetched_at')})")

    rows = []
    for sid, s in sorted(idx["series"].items()):
        for a in s.get("articles", []):
            title = a.get("title", "")
            has_pipe = 1 if TARGET in title else 0
            rows.append({
                "article_id": a["article_id"],
                "url": f"{BASE}/articles/{a['article_id']}",
                "title": title,
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""),
                "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""),
                "has_pipe": has_pipe,
            })

    # articles-v11.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "has_pipe"]
    with open(os.path.join(HERE, "articles-v11.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(rows, key=lambda r: (-r["has_pipe"], r["series_id"])):
            w.writerow(r)

    # series-summary-v11.csv
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0, "pipe": 0})
        g["articles"] += 1
        g["pipe"] += r["has_pipe"]
    srows = []
    for g in series.values():
        pct = g["pipe"] / g["articles"] * 100 if g["articles"] else 0
        srows.append({**g, "pipe_pct": pct})
    srows.sort(key=lambda r: (-r["pipe_pct"], -r["articles"]))
    sfields = ["series_id", "series_title", "group_name", "articles", "pipe", "pipe_pct"]
    with open(os.path.join(HERE, "series-summary-v11.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields)
        w.writeheader()
        for r in srows:
            w.writerow({**r, "pipe_pct": f"{r['pipe_pct']:.2f}"})

    # 各組 pipe_pct
    from collections import defaultdict
    by_group = defaultdict(lambda: {"articles": 0, "pipe": 0})
    for r in rows:
        g = by_group[r["group_name"]]
        g["articles"] += 1
        g["pipe"] += r["has_pipe"]
    group_rows = []
    for name, g in by_group.items():
        pct = g["pipe"] / g["articles"] * 100 if g["articles"] else 0
        group_rows.append((name, g["articles"], g["pipe"], pct))
    group_rows.sort(key=lambda x: -x[3])

    # 統計
    tot_articles = len(rows)
    tot_pipe = sum(r["has_pipe"] for r in rows)
    tot_pct = tot_pipe / tot_articles * 100
    all_pipe_series = [s for s in srows if s["pipe_pct"] == 100.0 and s["articles"] >= 5]
    no_pipe_series = [s for s in srows if s["pipe_pct"] == 0.0 and s["articles"] >= 10]
    mixed_series = [s for s in srows if 0 < s["pipe_pct"] < 100]
    print(f"文章 {tot_articles}, 標題含 ｜ 的 {tot_pipe} ({tot_pct:.1f}%)")
    print(f"系列 {len(srows)}: 全 ｜ (pct=100%, 篇數>=5) {len(all_pipe_series)}, "
          f"完全不用 (pct=0, 篇數>=10) {len(no_pipe_series)}, 混用 {len(mixed_series)}")

    L = [f"# lab01 V11 結果：標題含 `｜` (U+FF5C 全形直線)",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}.",
         f"> AI 生成標題特別愛用 ｜ 當分隔符 (「Day 1｜XXX」). 人類多用 ：、:、或純文字.",
         f"> 這是 title-level 分類訊號, 跟 V01-V10 的 per_1k 密度不一樣.",
         "",
         "## 總覽",
         "",
         "| 項目 | 數值 |",
         "|:---|---:|",
         f"| 系列數 | {len(srows)} |",
         f"| 文章數 | {tot_articles:,} |",
         f"| 標題含 ｜ 的篇數 | {tot_pipe:,} ({tot_pct:.1f}%) |",
         f"| 全 ｜ 系列 (pct=100%, 篇數>=5) | {len(all_pipe_series)} |",
         f"| 完全不用系列 (pct=0, 篇數>=10) | {len(no_pipe_series)} |",
         f"| 混用系列 (0 < pct < 100) | {len(mixed_series)} |",
         "",
         "## 各組 pipe_pct 排行",
         "",
         "| # | 組別 | 篇數 | ｜ 篇數 | pct |",
         "|---:|:---|---:|---:|---:|"]
    for i, (name, art, p, pct) in enumerate(group_rows, 1):
        L.append(f"| {i} | {name} | {art:,} | {p:,} | {pct:.1f}% |")

    # 全 ｜ 系列
    L += ["", f"## 全 ｜ 系列 (pct=100%, 篇數 >=5), 共 {len(all_pipe_series)} 系列", "",
          "| # | pct | 篇數 | 系列 | 組別 |",
          "|---:|---:|---:|:---|:---|"]
    for i, s in enumerate(all_pipe_series[:args.top], 1):
        st = s['series_title'].replace("|", "\\|")
        L.append(f"| {i} | 100% | {s['articles']} | {st} | {s['group_name']} |")
    if len(all_pipe_series) > args.top:
        L.append(f"| ... | | | 剩餘 {len(all_pipe_series)-args.top} 個系列省略 | |")

    # 混用系列 top (pct 高但未滿 100 的)
    high_mixed = [s for s in mixed_series if s["articles"] >= 10 and s["pipe_pct"] >= 50]
    high_mixed.sort(key=lambda r: (-r["pipe_pct"], -r["articles"]))
    L += ["", f"## 高度混用系列 (pct >=50%, 篇數>=10, 未滿 100%), top {min(args.top, len(high_mixed))} / 共 {len(high_mixed)}", "",
          "| # | pct | ｜ / 篇數 | 系列 | 組別 |",
          "|---:|---:|---:|:---|:---|"]
    for i, s in enumerate(high_mixed[:args.top], 1):
        st = s['series_title'].replace("|", "\\|")
        L.append(f"| {i} | {s['pipe_pct']:.1f}% | {s['pipe']}/{s['articles']} | {st} | {s['group_name']} |")

    L += ["", "> 全 ｜ 系列 = 作者一致用這個 cosmetic separator, 高度風格化 (可能是 AI framing 或人類風格).",
          "> 混用系列 = 作者沒統一, 可能是後期才用 AI 幫忙擬標題, 或心情決定.",
          "> 完全不用系列 = 作者 register 偏傳統 (：/ 空白 / 純文字 標題).",
          "> 這是 cosmetic signal, 跟內容 AI 味正交 — 很多「有 ｜」的系列內文完全沒 AI tell.", ""]

    with open(os.path.join(HERE, "results-v11.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(L))

    print("輸出: articles-v11.csv, series-summary-v11.csv, results-v11.md")


if __name__ == "__main__":
    main()
