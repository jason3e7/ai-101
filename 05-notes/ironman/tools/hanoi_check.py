#!/usr/bin/env python3
"""驗證河內塔的移動序列。用法：python3 hanoi_check.py N < moves.txt"""
import sys, re

def main():
    n = int(sys.argv[1])
    raw = sys.stdin.read()
    moves = []
    numbered = re.compile(r'(\d+)\s*[.、)]\s*(?:盤\s*)?(\d+)\s*(?:號)?\s*[:：]?\s*([ABC])\s*(?:->|→|➔|=>|to)\s*([ABC])', re.I)
    bare     = re.compile(r'^\s*(?:盤\s*)?(\d+)\s*(?:號)?\s*[:：]?\s*([ABC])\s*(?:->|→|➔|=>|to)\s*([ABC])', re.I)
    for line in raw.splitlines():
        m = numbered.search(line)
        if m:
            moves.append((int(m.group(1)), int(m.group(2)), m.group(3).upper(), m.group(4).upper()))
            continue
        m = bare.search(line)          # 沒有步數前綴的格式：盤號 起點→終點
        if m:
            moves.append((len(moves)+1, int(m.group(1)), m.group(2).upper(), m.group(3).upper()))

    pegs = {'A': list(range(n, 0, -1)), 'B': [], 'C': []}
    errs = []
    for idx, (step, disk, src, dst) in enumerate(moves, 1):
        if step != idx:
            errs.append(f'第 {idx} 行：步數標成 {step}')
        if not pegs[src]:
            errs.append(f'步驟 {idx}：{src} 柱是空的'); break
        top = pegs[src][-1]
        if top != disk:
            errs.append(f'步驟 {idx}：{src} 柱頂端是盤 {top}，不是盤 {disk}'); break
        if pegs[dst] and pegs[dst][-1] < disk:
            errs.append(f'步驟 {idx}：盤 {disk} 不能疊在盤 {pegs[dst][-1]} 上'); break
        pegs[dst].append(pegs[src].pop())

    ideal = 2**n - 1
    solved = pegs['C'] == list(range(n, 0, -1))
    print(f'N = {n}')
    print(f'  最少步數     {ideal}')
    print(f'  它給了       {len(moves)} 步'
          + ('' if len(moves) == ideal else f'  ← 差 {len(moves)-ideal:+d}'))
    print(f'  步驟合法     {"是" if not errs else "否"}')
    print(f'  有沒有解開   {"有" if solved else "沒有"}')
    if errs:
        print('\n  第一個錯誤：')
        for e in errs[:3]:
            print('   ', e)
    if not moves:
        print('\n  ⚠ 一步都沒解析到 — 可能它根本沒逐步列出（改成給演算法或程式）')

main()
