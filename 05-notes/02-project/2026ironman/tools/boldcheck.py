import re,sys
PUNCT='「『（《【〈〔"\'“‘'
def check(path):
    s=open(path,encoding='utf-8').read()
    s=re.sub(r'(?s)```.*?```','',s)
    bad=[]
    for n,m in enumerate(re.finditer(r'\*\*',s)):
        if n%2: continue                      # 只看開頭的 **
        prev=s[m.start()-1] if m.start() else '\n'
        nxt=s[m.end()] if m.end()<len(s) else '\n'
        # 開頭 ** 前面是中文/英數，後面是全形開引號 → CommonMark 不認為是粗體起點
        if re.match(r'[一-鿿A-Za-z0-9]',prev) and nxt in PUNCT:
            bad.append(s[max(0,m.start()-16):m.end()+16].replace('\n','\\n'))
    return bad
for f in sys.argv[1:]:
    b=check(f)
    print(f'{f.split("/")[-1]:<32} 真正會破版: {len(b)}')
    for x in b: print('     …'+x+'…')
