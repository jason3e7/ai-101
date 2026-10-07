# PHM #99 Lucky — you-guess

| 項目 | 值 |
|:---|:---|
| 類別 | Lucky |
| 分數 | 110 |
| 解題數 | 74 |
| 附檔 | [`/static/you-guess.py`](https://ctf.hackme.quest/static/you-guess.py) |

## 題目

> Can you guess my password?

附一支 Python 腳本:

```python
import hashlib, sys
def sha512(s): return hashlib.sha512(s.encode()).hexdigest()
password = sys.argv[1]
h = sha512('your hash is ' + sha512(password) + ' but password is not password')
if h == '2a9b881b84d4386e39518c8802cc8167ec84d37118efd3949dbedd5e73bf74b62d80bf1531b7505a197565660bf452b2641cd5cd12f0c99c502a4d72c28197f2':
    key = bytes.fromhex(sha512('%s really hates her ex.' % password))
    encrypted = bytes.fromhex('20a6b2b83f17...178c0f')
    flag = bytearray(i ^ j for i, j in zip(bytearray(key), bytearray(encrypted)))
    print(flag.decode().strip(',.~'))
```

## 分析

- 密碼本身**沒給任何線索**除了題名 "you-guess" 跟 "Lucky" 分類 — 這題定位就是字典攻擊
- 線索:
  - 檢查式含註解 `but password is not password`, 排除 `password` 本身
  - key 生成模板 `'<password> really hates her ex.'`, 字面上密碼是一個 (女性) 名字
  - "Lucky" 類暗示: 不是推理, 是跑 wordlist 碰運氣

## 解法

跑 SecLists 的 top 10k 常見密碼:

```bash
curl -sL -o /tmp/rockyou10k.txt \
  https://raw.githubusercontent.com/danielmiessler/SecLists/master/Passwords/Common-Credentials/10k-most-common.txt
```

```python
import hashlib
target = '2a9b881b84d4386e39518c8802cc8167ec84d37118efd3949dbedd5e73bf74b62d80bf1531b7505a197565660bf452b2641cd5cd12f0c99c502a4d72c28197f2'
def check(p):
    inner = hashlib.sha512(p.encode()).hexdigest()
    return hashlib.sha512(('your hash is ' + inner + ' but password is not password').encode()).hexdigest() == target

for p in open('/tmp/rockyou10k.txt'):
    p = p.strip()
    if check(p):
        print('MATCH:', p); break
```

Hit at index **1537**: `sophia`.

跑一次腳本:

```bash
python3 you-guess.py sophia
# -> FLAG{Wow, you know sophia?}
```

## Flag

```
FLAG{Wow, you know sophia?}
```

## 感想

- 字典攻擊基本題, 挑戰在**選對字典**. 一開始猜 Taylor Swift / Adele 這類「hates her ex」的名人完全沒中, 回到基本面跑 top 10k 立刻解 — 這類題別過度解讀 hint, 直接 brute
- "Lucky" 分類本身就是 PHM 作者的誠實標籤: 這題吃字典命中率, 不是智力題
- 110 分 / 74 解, 偏少的解題數大概是因為很多人卡在「猜」而不去跑 wordlist
