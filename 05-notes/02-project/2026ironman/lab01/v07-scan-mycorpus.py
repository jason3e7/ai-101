#!/usr/bin/env python3
"""v07-scan-mycorpus.py — 掃 jason3e7 的 publish/ 找重複的中文 n-gram.

用途:
    給 V07 (中文 AI 冗詞偵測) 找候選 signal. 掃 publish day01-15 (或未來更多天)
    的中文 n-gram, 出現 3 次以上的都列出來, 順便對照「已知 AI 冗詞清單」看
    自己文章有沒有踩到.

執行:
    python3 v07-scan-mycorpus.py

輸出: stdout, 或 pipe 到檔案.
"""
import re
from collections import Counter
from pathlib import Path

PUB = Path(__file__).resolve().parent.parent / "publish"  # 05-notes/ironman/publish
MIN_COUNT = 3
NGRAM_RANGE = range(4, 11)

KNOWN_AI_PHRASES = [
    "換句話說", "值得注意的是", "值得一提的是", "舉例來說", "舉個例子",
    "具體而言", "具體來說", "進一步而言", "進一步來說", "不僅如此",
    "事實上", "綜上所述", "更在於", "首先", "其次", "然後", "最後",
    "這樣一來", "從而", "此外", "另一方面", "一言以蔽之",
    "在這個過程中", "換言之", "以此為例", "的核心在於", "的關鍵在於",
    "不是而是", "這意味著", "不容忽視", "尤為重要", "深層次",
    "從本質上", "本質上", "值得深思", "至關重要", "無疑",
    "必不可少", "恰恰相反", "與此同時", "與其相對", "不失為",
    "無論如何", "毋庸置疑", "顯而易見", "由此可見",
    "在某種程度上", "從某種意義上", "從某個角度",
    "有著", "存在著", "面臨著", "起著",
    "的方式", "的話", "所需的", "所帶來的",
    "在於它", "之所以", "正是因為",
    "首要", "首當其衝", "重中之重",
    "一方面另一方面",
]


def strip_markdown(text):
    text = re.sub(r"```.*?```", " ", text, flags=re.S)
    text = re.sub(r"`[^`\n]*`", " ", text)
    text = re.sub(r"!\[[^\]]*\]\([^)]*\)", " ", text)
    text = re.sub(r"\[([^\]]*)\]\([^)]*\)", r"\1", text)
    text = re.sub(r"<[^>]+>", " ", text)
    text = re.sub(r"\*+", "", text)
    text = re.sub(r"^#+\s*", "", text, flags=re.M)
    text = re.sub(r"^>\s*\[![A-Z]+\]", " ", text, flags=re.M)
    text = re.sub(r"^>\s*", " ", text, flags=re.M)
    text = re.sub(r"^---+\s*$", " ", text, flags=re.M)
    text = re.sub(r"^---\n.*?\n---\n", "", text, flags=re.S)
    return text


def main():
    files = sorted(PUB.glob("day*/day*-ithome.md"))
    print(f"檔案數: {len(files)}  (從 {PUB})")
    for f in files:
        print(f"  {f.relative_to(PUB)}")

    corpus = ""
    for f in files:
        corpus += strip_markdown(f.read_text(encoding="utf-8")) + "\n"

    runs = re.findall(r"[一-鿿]{3,}", corpus)
    total = sum(len(r) for r in runs)
    print(f"\n中文字元 (去 markdown/程式碼): {total}")
    print(f"中文字串塊數: {len(runs)}")

    counter = Counter()
    for n in NGRAM_RANGE:
        for run in runs:
            for i in range(len(run) - n + 1):
                counter[run[i:i+n]] += 1

    freq = {g: c for g, c in counter.items() if c >= MIN_COUNT}

    def is_maximal(g, c):
        n = len(g)
        for key in freq:
            if len(key) == n + 1 and (key[:-1] == g or key[1:] == g) and freq[key] == c:
                return False
        return True

    maximal = {g: c for g, c in freq.items() if is_maximal(g, c)}
    ranked = sorted(maximal.items(), key=lambda x: (-x[1], -len(x[0])))

    print(f"\nn-gram (count >= {MIN_COUNT}, maximal): {len(maximal)}")
    print("=" * 60)
    print(f"Top maximal n-grams, sorted by count DESC then length DESC")
    print("=" * 60)
    for g, c in ranked:
        print(f"  {c:4}  {g}")

    print()
    print("=" * 60)
    print(f"命中已知 AI 冗詞清單 (共 {len(KNOWN_AI_PHRASES)} 條)")
    print("=" * 60)
    hits = sorted(((p, corpus.count(p)) for p in KNOWN_AI_PHRASES if corpus.count(p) > 0),
                  key=lambda x: -x[1])
    for phrase, c in hits:
        marker = "★" if c >= MIN_COUNT else " "
        print(f"  {marker} {c:4}  {phrase}")


if __name__ == "__main__":
    main()
