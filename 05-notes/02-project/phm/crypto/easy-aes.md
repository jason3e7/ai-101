# PHM #87 Crypto — easy AES

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 60 |

## 題目

> 檔案: [`/static/what-is-aes.py.xz`](https://ctf.hackme.quest/static/what-is-aes.py.xz)

解開看到以下 python 腳本:

```python
import base64
from Crypto.Cipher import AES

key = b'Hello, World...!'
iv  = b'1234567887654321'

c = AES.new(key, AES.MODE_ECB)
plain_block = c.decrypt(b'Good Plain Text!')  # 16 bytes
real_key = plain_block[::-1]  # 反轉當真正 AES CBC 的 key

c2 = AES.new(real_key, AES.MODE_CBC, IV=iv)

# server-side: 用 c2 加密 JPG 得到 b64 字串, 嵌在 task 說明裡
# 我們拿到的是加密後 base64
b64 = """ ...長 base64... """
```

(實際檔案裡 b64 就寫在腳本尾, 不貼了)

## 分析

邏輯直覺:

1. 先用 `AES-ECB(key='Hello, World...!').decrypt(b'Good Plain Text!')` 拿一個 16-byte block.
2. **反轉** 這個 block, 就是真正的 AES-CBC key.
3. 用這把 key + IV `'1234567887654321'` CBC 解密 base64 內容, 得到 JPEG 檔.

## 解法

```python
import base64
from Crypto.Cipher import AES

c = AES.new(b'Hello, World...!', AES.MODE_ECB)
plain_block = c.decrypt(b'Good Plain Text!')
real_key = plain_block[::-1]

data = base64.b64decode(open('b64_payload.txt').read())
c2 = AES.new(real_key, AES.MODE_CBC, IV=b'1234567887654321')
img = c2.decrypt(data)

open('out.jpg', 'wb').write(img)
```

打開 `out.jpg` 直接看到一張圖, 上面寫:

```
FLAG{I can encrypt AES}
```

## Flag

```
FLAG{I can encrypt AES}
```

## 感想

照腳本做就對了的題目. 唯一有梗是 `plain_block[::-1]` 反轉當 key, 不看腳本就是黑箱, 看了就是直通車.
