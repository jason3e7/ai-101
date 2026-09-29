#!/usr/bin/env python3
"""fetch.py — 抓 2026 iThome 鐵人賽全部已發布文章到 raw/, 並驗證沒有漏抓.

為什麼這樣抓 (2026-09 實測):
  * /users/{uid}/ironman/{sid} (系列頁) 被 Cloudflare 擋 (403), 不能直取.
  * 組別頁 ?tab=latest 是依時間排的「文章流」, 同系列會重複出現, 不適合拿來列系列.
  * 官方報名清單 /2026ironman/signup/list?group={slug} 列出每個報名系列,
    附「報名數」(可核對總數) 與進度「DAY N」(= 已發篇數).
  * /rss/series/{sid} 沒被擋, 而且每個 item 帶完整內文 (content:encoded).
  * 文章頁的「系列目錄」只露出前後幾篇的視窗, 但會寫「共 N 篇」.

所以:
  1. 首頁 → 各組別 slug
  2. 翻完各組報名清單 → 全部系列 + 進度 DAY N; 核對系列數 == 報名數
  3. 每系列抓 RSS → raw/rss/{sid}.xml (一次拿到整個系列的內文)
  4. 對帳: RSS 篇數 == DAY N ?  (兩個獨立來源)
       不相等 → 讀文章頁目錄的「共 N 篇」, 順著目錄視窗逐篇補抓
                raw/articles/{aid}.html, 直到收齊
  5. 逐篇抓文章頁, 只存正文區塊到 raw/pages/{aid}.html.
     ⚠ RSS 內文會濾掉 —、「」、、 等標點 (實測同一篇 RSS 0 個 —, 文章頁 20 個),
       所以 RSS 只拿來列文章清單, 內文一律以文章頁為準.
  6. 輸出 raw/index.json (系列 + 文章清單) 與 raw/completeness.json (對帳結果)
     有任何系列對不上 → 結束碼 1

增量: raw/ 底下已有的檔案不重抓 (--refresh 強制重抓 RSS).
Rate limit: 每個請求之間 sleep SLEEP_SEC. 中斷可重跑.

用法:
  python3 fetch.py                    # 全部組別
  python3 fetch.py --groups claude-ai # 只抓指定組 (逗號分隔)
  python3 fetch.py --refresh          # 重抓所有 RSS (系列有新文章時用)
"""

import argparse
import html as htmlmod
import json
import os
import re
import sys
import time
import urllib.error
import urllib.request
import xml.etree.ElementTree as ET

BASE = "https://ithelp.ithome.com.tw"
# 正文之後第一個出現的區塊 (按讚/留言列、系列上下篇、頁尾) → 正文到此為止
END_MARKERS = ("qa-action", "article-series-page", "ir-article__footer", "qa-panel")
YEAR = 2026
INDEX_URL = f"{BASE}/{YEAR}ironman"
UA = ("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
      "(KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36")
SLEEP_SEC = 1.0
NS = {"content": "http://purl.org/rss/1.0/modules/content/"}

HERE = os.path.dirname(os.path.abspath(__file__))
RAW = os.path.join(HERE, "raw")
RSS_DIR = os.path.join(RAW, "rss")
ART_DIR = os.path.join(RAW, "articles")      # 補抓用的完整文章頁 (少量)
PAGE_DIR = os.path.join(RAW, "pages")        # 每篇文章的正文區塊 (分析用)
WORKERS = 2
INDEX_FILE = os.path.join(RAW, "index.json")
REPORT_FILE = os.path.join(RAW, "completeness.json")


def fetch(url, retries=4):
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
            if e.code == 404:
                raise
        except Exception as e:  # noqa: BLE001
            last = e
        print(f"  ! {url}: {last}, retry {i+1}/{retries}", file=sys.stderr)
        time.sleep(2 ** i)
    raise RuntimeError(f"fetch failed after {retries}: {url} ({last})")


def polite(url):
    time.sleep(SLEEP_SEC)
    return fetch(url)


# ---------- 1. 組別 ----------

def get_groups():
    html = fetch(INDEX_URL)
    pat = rf'href="[^"]*/{YEAR}ironman/([a-z0-9-]+)#ir-list"[^>]*>\s*(?:<[^>]+>\s*)?([^<]+?)\s*<'
    seen, out = set(), []
    for slug, name in re.findall(pat, html):
        if slug not in seen:
            seen.add(slug)
            out.append((slug, htmlmod.unescape(name.strip())))
    return out


# ---------- 2. 報名清單 → 系列 ----------

def parse_signup_page(html):
    """回傳 (報名數, [dict]) ."""
    total = re.search(r'ppl-num">\s*報名數\s*<span>(\d+)', html)
    rows = []
    for card in html.split('class="list-card"')[1:]:
        m = re.search(r'/users/(\d+)/ironman/(\d+)"[^>]*class="contestants-list__title[^"]*">\s*([^<]*?)\s*</a>', card)
        if not m:
            continue
        day = re.search(r'team-dashboard__day">\s*DAY\s*(\d+)', card)
        st = re.search(r'team-progress--(\w+)', card)
        rows.append({"series_id": m.group(2), "author_id": m.group(1),
                     "series_title": htmlmod.unescape(m.group(3).strip()),
                     "expected": int(day.group(1)) if day else 0,
                     "status": st.group(1) if st else "none"})
    return (int(total.group(1)) if total else None), rows


def get_series_in_group(slug):
    series, declared, page = {}, None, 1
    while page <= 200:
        html = polite(f"{BASE}/{YEAR}ironman/signup/list?group={slug}&page={page}")
        total, rows = parse_signup_page(html)
        declared = declared or total
        new = [r for r in rows if r["series_id"] not in series]
        if not rows or not new:
            break
        for r in new:
            series[r["series_id"]] = r
        page += 1
    print(f"  {slug}: {len(series)} 系列 (報名數 {declared})")
    return series, declared


# ---------- 3. RSS ----------

def rss_path(sid):
    return os.path.join(RSS_DIR, f"{sid}.xml")


def get_rss(sid, refresh=False):
    fp = rss_path(sid)
    if refresh or not os.path.exists(fp):
        xml = polite(f"{BASE}/rss/series/{sid}")
        with open(fp, "w", encoding="utf-8") as f:
            f.write(xml)
    return parse_rss(fp)


def parse_rss(fp):
    """回傳 [{article_id, title, pub_date}] (內文留在 xml 裡, 由 analyze.py 讀)."""
    items = []
    for it in ET.parse(fp).findall(".//item"):
        link = it.findtext("link") or ""
        m = re.search(r"/articles/(\d+)", link)
        if m:
            items.append({"article_id": m.group(1), "title": (it.findtext("title") or "").strip(),
                          "pub_date": it.findtext("pubDate"), "source": "rss"})
    return items


# ---------- 4. 補抓: 順著文章頁的目錄視窗 ----------

def article_path(aid):
    return os.path.join(ART_DIR, f"{aid}.html")


def get_article(aid):
    fp = article_path(aid)
    if not os.path.exists(fp):
        html = polite(f"{BASE}/articles/{aid}")
        with open(fp, "w", encoding="utf-8") as f:
            f.write(html)
    with open(fp, encoding="utf-8") as f:
        return f.read()


def parse_catalog(html):
    """文章頁的系列目錄 → (共 N 篇, {序號: (aid, title)}, series_id)."""
    i = html.find("article-series-catalog")
    if i < 0:
        return None, {}, None
    seg = html[i:html.find("完整目錄", i) + 1 or i + 20000]
    total = re.search(r'article-series-catalog__num">\s*(\d+)', seg)
    sid = re.search(r'/users/\d+/ironman/(\d+)', seg)
    entries = {}
    for num, aid, title in re.findall(
            r'catalog__list-num">\s*(\d+)\s*</div>\s*<a href="[^"]*/articles/(\d+)"[^>]*>([^<]*)<', seg):
        entries[int(num)] = (aid, htmlmod.unescape(title.strip()))
    return (int(total.group(1)) if total else None), entries, (sid.group(1) if sid else None)


def crawl_series(sid, seed_aids, have=0):
    """從已知文章出發, 反覆讀目錄視窗, 收齊整個系列. 回傳 (total, {num: (aid, title)}).
    have: 已經從 RSS 拿到的篇數; 讀到「共 N 篇」且 N <= have 就不必再抓."""
    known, total = {}, None
    todo, done = list(seed_aids), set()
    while todo:
        aid = todo.pop()
        if aid in done:
            continue
        done.add(aid)
        t, entries, s = parse_catalog(get_article(aid))
        if s and s != sid:
            continue
        total = max(total or 0, t or 0)
        if total and total <= have:
            break
        for num, (a, title) in entries.items():
            if num not in known:
                known[num] = (a, title)
                if a not in done:
                    todo.append(a)
        if total and len(known) >= total:
            break
    return total, known


# ---------- 5. 逐篇抓文章頁 (只存正文區塊) ----------

def page_path(aid):
    return os.path.join(PAGE_DIR, f"{aid}.html")


def body_block(page):
    """完整文章頁 → 正文區塊 (標題 h2 + markdown__style 內容, 到系列導覽/頁尾前)."""
    t = re.search(r'<h2\s+class="qa-header__title[^"]*">.*?</h2>', page, re.S)
    i = page.find("markdown__style")
    if i < 0:
        return None
    i = page.find(">", i) + 1
    ends = [page.rfind("<", i, j) for j in (page.find(k, i) for k in END_MARKERS) if j > 0]
    body = page[i:min(ends)] if ends else page[i:]
    return (t.group(0) if t else "") + '\n<div class="lab01-body">' + body + "</div>\n"


def download_page(aid):
    fp = page_path(aid)
    if os.path.exists(fp):
        return "skip"
    time.sleep(SLEEP_SEC)
    page = fetch(f"{BASE}/articles/{aid}")
    block = body_block(page)
    if block is None:
        raise RuntimeError("找不到正文區塊 (markdown__style)")
    tmp = fp + ".tmp"
    with open(tmp, "w", encoding="utf-8") as f:
        f.write(block)
    os.replace(tmp, fp)
    return "new"


def download_all_pages(aids):
    from concurrent.futures import ThreadPoolExecutor, as_completed
    failed, new = {}, 0
    with ThreadPoolExecutor(max_workers=WORKERS) as ex:
        futs = {ex.submit(download_page, a): a for a in aids}
        for i, f in enumerate(as_completed(futs), 1):
            a = futs[f]
            try:
                new += f.result() == "new"
            except Exception as e:  # noqa: BLE001
                failed[a] = str(e)
            if i % 200 == 0:
                print(f"  {i}/{len(aids)} (新抓 {new}, 失敗 {len(failed)})", flush=True)
    return new, failed


# ---------- 主流程 ----------

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--groups", help="逗號分隔 group slug, 不指定就抓全部")
    ap.add_argument("--refresh", action="store_true", help="重抓所有 RSS")
    args = ap.parse_args()
    for d in (RSS_DIR, ART_DIR):
        os.makedirs(d, exist_ok=True)

    print("[1/5] 組別")
    groups = get_groups()
    if args.groups:
        want = set(args.groups.split(","))
        groups = [g for g in groups if g[0] in want]
    print(f"  {len(groups)} 組: {[g[0] for g in groups]}")

    print("\n[2/5] 報名清單 → 系列")
    series, group_check = {}, []
    for slug, name in groups:
        got, declared = get_series_in_group(slug)
        group_check.append({"group": slug, "declared": declared, "collected": len(got)})
        for sid, s in got.items():
            s.update(group_slug=slug, group_name=name)
            series[sid] = s
    print(f"  共 {len(series)} 個系列, 報名數合計 {sum(g['declared'] or 0 for g in group_check)}, "
          f"宣告篇數合計 {sum(s['expected'] or 0 for s in series.values())}")

    print("\n[3/5] 各系列 RSS (列文章清單)")
    for i, (sid, s) in enumerate(sorted(series.items()), 1):
        try:
            s["articles"] = get_rss(sid, refresh=args.refresh)
        except Exception as e:  # noqa: BLE001
            print(f"  ! RSS {sid} 失敗: {e}", file=sys.stderr)
            s["articles"] = []
        if i % 50 == 0:
            print(f"  {i}/{len(series)}")

    print("\n[4/5] 對帳")
    # 規則 (2026-09 以 claude-ai 組 63 系列實測歸納):
    #   報名清單的 DAY = 「連續發文天數」.
    #   challenge (挑戰中) / success (完賽): DAY == 已發篇數, RSS 必須相等.
    #   fail (斷賽): DAY 是斷掉前的天數, 作者之後可能續發 → RSS >= DAY 屬正常.
    #   RSS 篇數 >= 30 時另讀文章頁「共 N 篇」, 排除 RSS 有上限的可能.
    report = {"groups": group_check, "ok": [], "fixed_by_crawl": [], "suspected_deleted": [], "unresolved": []}
    for g in group_check:
        if g["declared"] != g["collected"]:
            report["unresolved"].append({"group": g["group"], "problem": "系列數 != 報名數", **g})
    for sid, s in sorted(series.items()):
        got = {a["article_id"] for a in s["articles"]}
        n, day, st = len(got), s["expected"], s["status"]
        need_catalog = n >= 30
        if not need_catalog:
            if st in ("challenge", "success") and n == day:
                report["ok"].append(sid); continue
            if st in ("fail", "none") and n >= day:
                report["ok"].append(sid); continue
        if n == 0:
            # RSS 空、DAY > 0: 沒有任何起點可補抓. 實測都是斷賽系列, 視為作者已刪文
            report["suspected_deleted"].append({"series_id": sid, "title": s["series_title"],
                                                "status": st, "day": day})
            continue
        print(f"  系列 {sid} 「{s['series_title'][:24]}」狀態 {st}, DAY {day}, RSS {n} → 讀目錄確認")
        total, known = crawl_series(sid, list(got), have=n)
        for num, (aid, title) in sorted(known.items()):
            if aid not in got:
                s["articles"].append({"article_id": aid, "title": title, "pub_date": None,
                                      "source": "article_html", "num": num})
                got.add(aid)
        s["catalog_total"] = total
        entry = {"series_id": sid, "title": s["series_title"], "status": st, "day": day,
                 "rss": n, "catalog_total": total, "collected": len(got)}
        (report["fixed_by_crawl"] if total and len(got) >= total else report["unresolved"]).append(entry)

    print("\n[5/5] 逐篇抓文章頁 (只存正文)")
    os.makedirs(PAGE_DIR, exist_ok=True)
    all_aids = sorted({a["article_id"] for s in series.values() for a in s["articles"]})
    new, failed = download_all_pages(all_aids)
    missing = [a for a in all_aids if not os.path.exists(page_path(a))]
    report["pages"] = {"articles": len(all_aids), "downloaded_now": new,
                       "missing": missing, "failed": failed}
    print(f"  文章 {len(all_aids)} 篇, 本次新抓 {new}, 缺 {len(missing)}")
    if missing:
        report["unresolved"].append({"problem": "文章頁未下載", "count": len(missing), "sample": missing[:20]})

    idx = {"fetched_at": time.strftime("%Y-%m-%d %H:%M:%S"), "series": series}
    with open(INDEX_FILE, "w", encoding="utf-8") as f:
        json.dump(idx, f, ensure_ascii=False, indent=1, sort_keys=True)
    total_articles = sum(len(s["articles"]) for s in series.values())
    report["summary"] = {"series": len(series), "articles": total_articles,
                         "ok": len(report["ok"]), "fixed_by_crawl": len(report["fixed_by_crawl"]),
                         "suspected_deleted": len(report["suspected_deleted"]),
                         "pages_missing": len(report["pages"]["missing"]),
                         "unresolved": len(report["unresolved"])}
    with open(REPORT_FILE, "w", encoding="utf-8") as f:
        json.dump(report, f, ensure_ascii=False, indent=1)

    print(f"\n完成: {report['summary']}")
    if report["suspected_deleted"]:
        print("\n注意: 以下系列 RSS 為空但報名清單有進度 (斷賽後疑似刪文, 網站上已無文章):")
        for e in report["suspected_deleted"]:
            print("  ", e)
    if report["unresolved"]:
        print("\n!! 以下系列仍對不上, 可能漏抓:")
        for e in report["unresolved"]:
            print("  ", e)
        sys.exit(1)


if __name__ == "__main__":
    main()
