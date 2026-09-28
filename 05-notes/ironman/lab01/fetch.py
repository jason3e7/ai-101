#!/usr/bin/env python3
"""fetch.py — 抓 2026 iThome 鐵人賽全部文章 HTML 到 raw/.

策略 (2026-09 實測):
  iThome 有些路徑會被 Cloudflare 擋 (users/*/ironman/*, users/*), 有些不擋.
  能不能只用 curl+UA 通過, 差別很大. 目前這些路徑 OK:
    /2026ironman                  — 首頁 (組別列表)
    /2026ironman/{slug}?tab=latest&page=N — 組別 latest 頁 (article URLs)
    /articles/{aid}               — 文章
  這些會被 CF 擋, 不能直取:
    /users/{uid}/ironman/{sid}    — 系列頁
    /users/{uid}                  — 使用者頁

因此改採「以 article 為中心」策略:
  1. 首頁 /2026ironman → 各組 slug (regex: {slug}#ir-list)
  2. 每組 ?tab=latest&page=N → 全部 article IDs (跨系列, 系列資訊之後從文章頁抽)
  3. 每篇 /articles/{aid} → 存 raw/articles/{aid}.html
  4. 從文章 HTML 抽 series_id / author_id / series_title, 存 raw/index.json

Manifest 結構:
  {
    "aid": {
      "article_id": str,
      "series_id": str,
      "author_id": str,
      "series_title": str,
      "group_slug": str,
      "group_name": str
    }
  }

增量: raw/articles/{aid}.html 存在就跳過 fetch (但仍會 parse 更新 index).
Rate limit: fetch 之間 sleep SLEEP_SEC.
中斷可重跑.

用法:
  python3 fetch.py                    # 全部組別
  python3 fetch.py --groups claude-ai # 只抓指定組
  python3 fetch.py --max-articles 5   # 每組最多 N 篇 (測試)
"""

import argparse
import json
import os
import re
import sys
import time
import urllib.error
import urllib.request

BASE = "https://ithelp.ithome.com.tw"
YEAR = 2026
INDEX_URL = f"{BASE}/{YEAR}ironman"
UA = ("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
      "(KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")
SLEEP_SEC = 1.5

HERE = os.path.dirname(os.path.abspath(__file__))
RAW = os.path.join(HERE, "raw")
ART_DIR = os.path.join(RAW, "articles")
INDEX_FILE = os.path.join(RAW, "index.json")


def fetch(url, retries=3):
    """HTTPError 保留原型別讓 caller 判斷是否要視為分頁終點."""
    last = None
    for i in range(retries):
        try:
            req = urllib.request.Request(url, headers={"User-Agent": UA})
            with urllib.request.urlopen(req, timeout=30) as r:
                data = r.read()
                if len(data) < 8000 and b"Just a moment" in data:
                    raise RuntimeError("cloudflare challenge")
                return data.decode("utf-8", errors="replace")
        except urllib.error.HTTPError as e:
            last = e
            print(f"  ! {url}: HTTP {e.code}, retry {i+1}/{retries}", file=sys.stderr)
            time.sleep(2 ** i)
        except Exception as e:
            last = e
            print(f"  ! {url}: {e}, retry {i+1}/{retries}", file=sys.stderr)
            time.sleep(2 ** i)
    if isinstance(last, urllib.error.HTTPError):
        raise last
    raise RuntimeError(f"fetch failed after {retries}: {url} ({last})")


def find_unique(pat, html):
    seen = set()
    out = []
    for m in re.finditer(pat, html):
        v = m.groups() if m.lastindex and m.lastindex > 1 else m.group(1)
        if v not in seen:
            seen.add(v)
            out.append(v)
    return out


def get_groups():
    """首頁 → [(slug, name)]. 過濾 non-group (event, books 等) 靠 #ir-list."""
    html = fetch(INDEX_URL)
    pat = rf'href="[^"]*/{YEAR}ironman/([a-z0-9-]+)#ir-list"[^>]*>\s*(?:<[^>]+>\s*)?([^<]+?)\s*<'
    return find_unique(pat, html)


def get_article_ids_in_group(slug, max_articles=None):
    """組別 ?tab=latest → 全部 article IDs (跨系列)."""
    articles = []
    seen = set()
    page = 1
    while True:
        url = f"{BASE}/{YEAR}ironman/{slug}?tab=latest&page={page}"
        try:
            html = fetch(url)
        except urllib.error.HTTPError as e:
            print(f"  {slug} page {page}: HTTP {e.code}, 分頁終止")
            break
        pat = r'href="[^"]*/articles/(\d+)"'
        found = find_unique(pat, html)
        new = [a for a in found if a not in seen]
        if not new:
            print(f"  {slug} page {page}: 無新 article, 分頁終止")
            break
        articles.extend(new)
        seen.update(new)
        print(f"  {slug} page {page}: +{len(new)} 篇 (累計 {len(articles)})")
        if max_articles and len(articles) >= max_articles:
            return articles[:max_articles]
        page += 1
        time.sleep(SLEEP_SEC)
        if page > 500:
            print(f"  ! {slug}: hit page cap 500", file=sys.stderr)
            break
    return articles


def extract_meta(html):
    """從文章 HTML 抽 series/author/title metadata."""
    meta = {"series_id": None, "author_id": None, "series_title": None, "article_title": None}
    m = re.search(r'/users/(\d+)/ironman/(\d+)', html)
    if m:
        meta["author_id"] = m.group(1)
        meta["series_id"] = m.group(2)
    m = re.search(r'ir-article__topic"><a[^>]*>\s*([^<]+?)\s*</a>', html)
    if m:
        meta["series_title"] = m.group(1).strip()
    m = re.search(r'<h2\s+class="qa-header__title[^"]*">\s*([^<]+?)\s*</h2>', html)
    if m:
        meta["article_title"] = m.group(1).strip()
    return meta


def save_article(aid):
    fp = os.path.join(ART_DIR, f"{aid}.html")
    if os.path.exists(fp):
        return "skip"
    url = f"{BASE}/articles/{aid}"
    html = fetch(url)
    with open(fp, "w", encoding="utf-8") as f:
        f.write(html)
    return "new"


def load_html(aid):
    fp = os.path.join(ART_DIR, f"{aid}.html")
    with open(fp, encoding="utf-8") as f:
        return f.read()


def load_index():
    if os.path.exists(INDEX_FILE):
        with open(INDEX_FILE, encoding="utf-8") as f:
            return json.load(f)
    return {}


def save_index(idx):
    tmp = INDEX_FILE + ".tmp"
    with open(tmp, "w", encoding="utf-8") as f:
        json.dump(idx, f, ensure_ascii=False, indent=2, sort_keys=True)
    os.replace(tmp, INDEX_FILE)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--groups", help="逗號分隔 group slug, 不指定就抓全部")
    ap.add_argument("--max-articles", type=int, help="每組最多抓 N 篇 (測試)")
    ap.add_argument("--skip-fetch", action="store_true", help="不下載, 只更新 index")
    args = ap.parse_args()

    os.makedirs(ART_DIR, exist_ok=True)
    idx = load_index()

    print("[1/3] 抓組別...")
    groups = get_groups()
    if args.groups:
        want = set(args.groups.split(","))
        groups = [g for g in groups if g[0] in want]
    print(f"  組別 {len(groups)}: {[g[0] for g in groups]}")

    print(f"\n[2/3] 抓各組 article URLs (via ?tab=latest)...")
    group_articles = {}
    for slug, name in groups:
        aids = get_article_ids_in_group(slug, max_articles=args.max_articles)
        group_articles[slug] = (name, aids)
        print(f"  {slug} ({name}): {len(aids)} 篇")
        time.sleep(SLEEP_SEC)

    if args.skip_fetch:
        print("\n--skip-fetch, 不下載文章.")
        return

    print("\n[3/3] 抓文章 HTML + 抽 metadata...")
    new = 0
    skip = 0
    all_pairs = [(slug, aid, name) for slug, (name, aids) in group_articles.items() for aid in aids]
    for i, (slug, aid, name) in enumerate(all_pairs, 1):
        res = save_article(aid)
        if res == "new":
            new += 1
            print(f"  [{i}/{len(all_pairs)}] {aid} new ({slug})")
            time.sleep(SLEEP_SEC)
        else:
            skip += 1
        # 抽 metadata 存 index (即使 skip 也更新, 因 group_slug 可能新)
        html = load_html(aid)
        meta = extract_meta(html)
        meta["article_id"] = aid
        meta["group_slug"] = slug
        meta["group_name"] = name
        idx[aid] = meta
        if i % 20 == 0:
            save_index(idx)
    save_index(idx)

    print(f"\n完成: {new} 新抓, {skip} 已存在, 共 {len(all_pairs)} 篇")


if __name__ == "__main__":
    main()
