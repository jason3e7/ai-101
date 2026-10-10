# PHM #94 Crypto — emoji

| 項目 | 值 |
|:---|:---|
| 類別 | Crypto |
| 分數 | 120 |

## 題目

> (´・ω・`)
>
> 檔案: [`/static/emoji.js`](https://ctf.hackme.quest/static/emoji.js)

## 分析

### Unpack 外層 packer

`emoji.js` 是 Dean Edwards 的 P.A.C.K.E.R.-style eval 殼. 把 `eval(...)` 換成 `console.log(...)` 直接 node 跑一次, 拿到 unpack 後的真正程式碼:

```js
if(typeof(require)=='undefined') return '(´・ω・`)';
var code = require('process').argv[2];
if(!code) return '(´・ω・`)';
String.prototype.zpad = function(l) { return this.length < l ? '0' + this.zpad(l-1) : this };
function encrypt(data) {
  return '"' + (Array.prototype.slice.call(data)
    .map(e => e.charCodeAt(0))
    .map(e => (e * 0xb1 + 0x1b) & 0xff)
    .map(e => '\\u' + e.toString(16).zpad(4))
  ).join('') + '"';
}
var crypted = "ÿÿÿH9hæ...";
if (JSON.parse(encrypt(code)) != crypted) return '(´・ω・`)';
try { eval(code) } catch(e) { return '(´・ω・`)' }
return '(*´∀`)~♥';
```

### 想通邏輯

程式接受 CLI 參數 `code`, 先用一個簡單的仿射密碼轉成 `\uXXXX` 格式字串, 跟內建 `crypted` 比對. 比對通過就 `eval(code)` 真的執行它.

所以 code = 「某段 JS 原始碼, 經 affine encrypt 等於 crypted」. 要還原 code → 反轉 affine.

### 反 affine

每個 byte: `enc = (c * 0xb1 + 0x1b) mod 256`.

0xb1 = 177 在 mod 256 下的乘法逆元: 177 * 81 ≡ 1 (mod 256), 所以逆元 = 81.

→ `c = (enc - 0x1b) * 81 mod 256`.

### 解出 jjencode / JSFuck 的 JS

把 crypted 反 affine 出來的 bytes 就是一段 JS. 開頭長這樣:

```
$$$=~[];$$$={___:++$$$,$$$$:(![]+"")[$$$],__$:++$$$,...
```

典型 **jjencode** 混淆 (把 JS 編碼成 `$`, `_` 這種符號組合). 直接 `node decrypted.js` 執行, 它會 alert / print flag.

## 解法

```python
import re, base64

# 1. Unpack: eval → console.log
import subprocess
result = subprocess.run(['node', '-e', """
const src = require('fs').readFileSync('emoji.js', 'utf8');
let r; eval(src.replace(/^eval/, 'r ='));
process.stdout.write(r);
"""], capture_output=True, text=True)
unpacked = result.stdout

# 2. Extract crypted \uXXXX bytes
m = re.search(r'var crypted="([\\s\\S]+?)";', unpacked)
cipher = [int(x, 16) for x in re.findall(r'\\u([0-9a-fA-F]{4})', m.group(1))]

# 3. Reverse affine
plain = bytes((b - 0x1b) * 81 % 256 for b in cipher)
open('decrypted.js', 'wb').write(plain)

# 4. Run the recovered JS
subprocess.run(['node', 'decrypted.js'])
# -> FLAG{JS Encoder Sucks}
```

## Flag

```
FLAG{JS Encoder Sucks}
```

## 感想

Packer → affine cipher → jjencode, 三層都是**可逆的程式變換**. 想通「要還原 code = 可執行 JS, 不是解密出字串」是關鍵: eval(decrypted) 直接 print flag.
