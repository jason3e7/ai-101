#!/usr/bin/env python3
"""analyzeV10.py — 自由時報 10 大 AI 文風指紋綜合 ranker.

來源:
    自由時報「AI 文自動偵測」特色文章列出 10 大常見指紋 (2026-07),
    https://art.ltn.com.tw/article/breakingnews/5501827

    V10 把這 10 組都掃一遍, 每個算 per_1k, composite = sum of per_1k.
    這是最接近 Kobak excess vocabulary 哲學的「清單式」檢測器, 但清單
    來自媒體整理, 不是統計推出.

10 組訊號 (有些是單一片語, 有些是家族詞群):

    s1  其實             — 常見開頭詞 (無法精確檢「開頭」, 直接 count 全文)
    s2  這不是…而是       — 典型對立句 (這 + 不是 X 而是 Y, 比 V08C 更特定)
    s3  真正的問題是      — 設問定調
    s4  關鍵不在於        — 反轉強調
    s5  表面上…更深層      — 對比句型
    s6  很簡單 / 很清楚   — 簡化表達 (count 兩個相加)
    s7  趨勢 / 關鍵 / 核心 — 公關文核心名詞 (count 三個相加)
    s8  擁抱 / 賦能 / 交織 — 公關文高頻動詞 (count 三個相加)
    s9  (條列式排版 — 已在 V06B 處理, 這裡略過)
    s10 對立取代論證 — 跟 s2 高度重疊, 這裡用 V08C regex 當代理

公式:
    每個訊號 per_1k_i = count_i / chars × 1000
    composite = Σ per_1k_i

輸出:
    articles-v10.csv        每篇: 10 個 signal count + 10 個 per_1k + composite
    series-summary-v10.csv  每系列: 合計
    results-v10.md          composite 最高排行 + 各訊號 per_1k 分佈

用法:
    python3 analyzeV10.py
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

MIN_CHARS = 500

RE_PRE = re.compile(r"(?is)<pre\b.*?</pre>")
RE_SCRIPT_STYLE = re.compile(r"(?is)<(script|style)\b.*?</\1>")
RE_TAG = re.compile(r"(?s)<[^>]+>")

# 10 組 signal 定義: (key, label, matcher)
# matcher 回傳 (count, matches_list) — 允許 regex 或多字串 count
RE_S2 = re.compile(r"這不是[^。！？\n]{1,25}而是")
RE_S5 = re.compile(r"表面上[^。！？\n]{1,25}更深層")
RE_S10 = re.compile(r"不(?:只)?是[^。！？\n]{1,25}(?:而是|更是)")

def _re_count(pattern):
    def fn(text):
        matches = pattern.findall(text)
        return len(matches), matches
    return fn

def _str_count(phrases):
    def fn(text):
        matches = []
        total = 0
        for p in phrases:
            c = text.count(p)
            total += c
            if c:
                matches.extend([p] * c)
        return total, matches
    return fn

SIGNALS = [
    ("s1",  "其實",                 _str_count(["其實"])),
    ("s2",  "這不是…而是",            _re_count(RE_S2)),
    ("s3",  "真正的問題是",          _str_count(["真正的問題是"])),
    ("s4",  "關鍵不在於",            _str_count(["關鍵不在於"])),
    ("s5",  "表面上…更深層",         _re_count(RE_S5)),
    ("s6",  "很簡單/很清楚",         _str_count(["很簡單", "很清楚"])),
    ("s7",  "趨勢/關鍵/核心",        _str_count(["趨勢", "關鍵", "核心"])),
    ("s8",  "擁抱/賦能/交織",        _str_count(["擁抱", "賦能", "交織"])),
    # s9 條列式排版 — skip, 已在 V06B
    ("s10", "對立句 (不是/不只是…而是/更是, V08C 代理)",  _re_count(RE_S10)),
]


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
    chars = len(re.sub(r"\s", "", text))
    counts = {}
    matches_per_signal = {}
    for key, _, fn in SIGNALS:
        c, ms = fn(text)
        counts[key] = c
        matches_per_signal[key] = ms
    return counts, matches_per_signal, chars


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
            counts, matches_per_signal, chars = measure(page_body(block))
            per_1k = {k: (c / chars * SCALE if chars else 0.0) for k, c in counts.items()}
            composite = sum(per_1k.values())
            rows.append({
                "article_id": aid, "url": f"{BASE}/articles/{aid}", "title": a.get("title", ""),
                "series_id": sid, "series_title": s.get("series_title", ""),
                "author_id": s.get("author_id", ""), "group_slug": s.get("group_slug", ""),
                "group_name": s.get("group_name", ""), "source": a.get("source", "rss"),
                "chars": chars,
                **{f"{k}_count": counts[k] for k, _, _ in SIGNALS},
                **{f"{k}_per_1k": per_1k[k] for k, _, _ in SIGNALS},
                "composite": composite,
                "_matches": matches_per_signal,
            })

    if missing:
        print(f"!! {len(missing)} 篇找不到正文: {missing[:10]}")

    # articles-v10.csv
    fields = ["article_id", "url", "title", "series_id", "series_title", "author_id",
              "group_slug", "group_name", "source", "chars"]
    for k, _, _ in SIGNALS:
        fields.append(f"{k}_count")
    for k, _, _ in SIGNALS:
        fields.append(f"{k}_per_1k")
    fields.append("composite")
    with open(os.path.join(HERE, "articles-v10.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        for r in sorted(rows, key=lambda r: -r["composite"]):
            formatted = {**r,
                         "composite": f"{r['composite']:.4f}",
                         **{f"{k}_per_1k": f"{r[f'{k}_per_1k']:.4f}" for k, _, _ in SIGNALS}}
            w.writerow(formatted)

    # series-summary-v10.csv
    series = {}
    for r in rows:
        g = series.setdefault(r["series_id"], {
            "series_id": r["series_id"], "series_title": r["series_title"],
            "group_name": r["group_name"], "articles": 0, "chars": 0,
            "composite_sum": 0.0,
            **{f"{k}_count": 0 for k, _, _ in SIGNALS}})
        g["articles"] += 1
        g["chars"] += r["chars"]
        g["composite_sum"] += r["composite"] * r["chars"]  # 加權 by chars
        for k, _, _ in SIGNALS:
            g[f"{k}_count"] += r[f"{k}_count"]
    srows = []
    for g in series.values():
        composite = sum(g[f"{k}_count"] for k, _, _ in SIGNALS) / g["chars"] * SCALE if g["chars"] else 0.0
        srows.append({**{k: g[k] for k in g if not k.startswith("composite_sum")},
                      "composite": composite})
    srows.sort(key=lambda r: -r["composite"])
    sfields = ["series_id", "series_title", "group_name", "articles", "chars"] + \
              [f"{k}_count" for k, _, _ in SIGNALS] + ["composite"]
    with open(os.path.join(HERE, "series-summary-v10.csv"), "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=sfields, extrasaction="ignore")
        w.writeheader()
        for r in srows:
            w.writerow({**r, "composite": f"{r['composite']:.4f}"})

    # 全體統計
    tot_chars = sum(r["chars"] for r in rows)
    tot_counts = {k: sum(r[f"{k}_count"] for r in rows) for k, _, _ in SIGNALS}
    qualified = [r for r in rows if r["chars"] >= args.min_chars]
    skipped = len(rows) - len(qualified)
    top = sorted(qualified, key=lambda r: -r["composite"])[:args.top]

    L = [f"# lab01 V10 結果：自由時報 10 大 AI 文風指紋綜合排行",
         "",
         f"> 抓取時間 {idx.get('fetched_at')}.",
         f"> 來源: 自由時報 2026-07 文章列出的 10 大 AI 中文指紋.",
         f"> composite = Σ per_1k_i (每個 signal 的 per_1k 直接加總).",
         f"> **排行榜最小字數門檻 {args.min_chars}** (排除 {skipped} 篇短文).",
         "",
         "## 總覽 — 10 個訊號的全體 per_1k",
         "",
         "| key | label | 命中總次數 | 全體 per_1k |",
         "|:---|:---|---:|---:|"]
    for k, label, _ in SIGNALS:
        c = tot_counts[k]
        p = c / tot_chars * SCALE if tot_chars else 0
        L.append(f"| {k} | {label} | {c:,} | {p:.4f} |")
    total_composite = sum(c / tot_chars * SCALE for c in tot_counts.values()) if tot_chars else 0
    L.append(f"| **composite** | 10 項相加 | — | **{total_composite:.4f}** |")

    L += ["",
          f"## composite 最高的 {len(top)} 篇 (chars >= {args.min_chars})",
          "",
          "| # | composite | " + " | ".join(k for k, _, _ in SIGNALS) + " | 總字 | 文章 | 系列 | 組別 |",
          "|---:|---:|" + "|".join(["---:" for _ in SIGNALS]) + "|---:|:---|:---|:---|"]
    for i, r in enumerate(top, 1):
        t = r["title"].replace("|", "\\|")
        st = r["series_title"].replace("|", "\\|")
        signal_cols = " | ".join(str(r[f"{k}_count"]) for k, _, _ in SIGNALS)
        L.append(f"| {i} | {r['composite']:.2f} | {signal_cols} | {r['chars']} | "
                 f"[{t}]({r['url']}) | {st} | {r['group_name']} |")

    L += ["", f"## composite 最高的 {min(args.top, len(srows))} 個系列", "",
          "| # | composite | 篇數 | " + " | ".join(k for k, _, _ in SIGNALS) + " | 總字 | 系列 | 組別 |",
          "|---:|---:|---:|" + "|".join(["---:" for _ in SIGNALS]) + "|---:|:---|:---|"]
    for i, r in enumerate(srows[:args.top], 1):
        st = r["series_title"].replace("|", "\\|")
        signal_cols = " | ".join(str(r[f"{k}_count"]) for k, _, _ in SIGNALS)
        L.append(f"| {i} | {r['composite']:.2f} | {r['articles']} | {signal_cols} | "
                 f"{r['chars']} | {st} | {r['group_name']} |")

    L += ["", "> composite 直接把 10 個 per_1k 相加 (未加權). 若某訊號密度很高會主宰總分. 看 top 排名時一併看各訊號 count 判斷.",
          "> 這是共現訊號, 不是判決.", ""]
    with open(os.path.join(HERE, "results-v10.md"), "w", encoding="utf-8") as f:
        f.write("\n".join(x for x in L if x is not None))

    print(f"文章 {len(rows)} 篇, 系列 {len(series)} 個, 全體 composite {total_composite:.4f}")
    print("輸出: articles-v10.csv, series-summary-v10.csv, results-v10.md")


if __name__ == "__main__":
    main()
