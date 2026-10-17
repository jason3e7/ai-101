# PHM #15 Web — hide and seek

| 項目 | 值 |
|:---|:---|
| 類別 | Web |
| 分數 | 10 |

## 題目

> Can you see me? I'm so close to you but you can't see me.
>
> 連結: [ctf.hackme.quest](https://ctf.hackme.quest/)

## 分析

主頁上肉眼看不到 flag. 題目說「很近但看不見」, 典型**藏在 HTML / CSS 的隱形元素**.

`curl` 下載主頁後 grep `FLAG`:

```bash
curl -s https://ctf.hackme.quest/ | grep -n FLAG
# 207: <h2 style="color: rgba(255, 255, 255, 0)">FLAG{0h U C meeeeeeeeeeeeeeeeeeee!}</h2>
```

`<h2 style="color: rgba(255, 255, 255, 0)">` — 白色文字且 **alpha=0** (完全透明), 疊在白底上根本看不到.

## Flag

```
FLAG{0h U C meeeeeeeeeeeeeeeeeeee!}
```

## 感想

Web 類別最入門暖身, `View Source` 或 `curl | grep` 一次就撈到.
