# ZeroJudge 競賽題庫 解題

[競賽題庫](https://zerojudge.tw/Problems?tabid=CONTEST) 的解題。難度較高、多為 **Strictly** 評分。

> ✅ **已上傳 ZeroJudge 判題**：本頁非跳過者全數 AC（c500 於 2026-10-07 補 AC，見下方說明）。

| 題號 | 題名 | 語言 | 狀態 |
|:---|:---|:---|:---|
| [c500](https://zerojudge.tw/ShowProblem?problemid=c500) | AEWE-645的傷害 | CPP | AC |
| [m930](https://zerojudge.tw/ShowProblem?problemid=m930) | 正方型池塘水深問題 | - | 跳過 (題意不明, 樣例對不上) |
| [s016](https://zerojudge.tw/ShowProblem?problemid=s016) | 樹上有隻毛毛蟲 (Caterpillar) | CPP | AC |
| [b599](https://zerojudge.tw/ShowProblem?problemid=b599) | Graph Construction (度數序列) | CPP | AC |
| [b591](https://zerojudge.tw/ShowProblem?problemid=b591) | 最小容量造船問題 | CPP | AC |
| [b588](https://zerojudge.tw/ShowProblem?problemid=b588) | 撿石頭遊戲 (博弈 DP) | CPP | AC |
| [b589](https://zerojudge.tw/ShowProblem?problemid=b589) | 超級馬拉松賽 (DP) | CPP | AC |
| [b585](https://zerojudge.tw/ShowProblem?problemid=b585) | 來開派對唷 (剝殼最大子集) | CPP | AC |
| [b584](https://zerojudge.tw/ShowProblem?problemid=b584) | 過橋問題 (手電筒) | CPP | AC |
| [b597](https://zerojudge.tw/ShowProblem?problemid=b597) | Stickst (Sticks DFS) | CPP | AC |
| [b596](https://zerojudge.tw/ShowProblem?problemid=b596) | Less is better (凸包頂點數) | CPP | AC |
| [b586](https://zerojudge.tw/ShowProblem?problemid=b586) | 文章壓縮 (Move-to-Front) | CPP | AC |
| [b598](https://zerojudge.tw/ShowProblem?problemid=b598) | Minimize the Number of Coins (DP) | CPP | AC |
| [b579](https://zerojudge.tw/ShowProblem?problemid=b579) | 恢復分數 | - | 跳過 (難, 整數線性系統) |
| [b672](https://zerojudge.tw/ShowProblem?problemid=b672) | A Special Automobile Race (Jump Game) | CPP | AC |
| [b673](https://zerojudge.tw/ShowProblem?problemid=b673) | How Big Is It (圓裝箱) | CPP | AC |
| [b674](https://zerojudge.tw/ShowProblem?problemid=b674) | Is It A Tree (有向邊判樹) | CPP | AC |
| [a554](https://zerojudge.tw/ShowProblem?problemid=a554) | NCPC SHA-4 (hash 反推) | CPP | AC |
| [b590](https://zerojudge.tw/ShowProblem?problemid=b590) | 單位分數分解 | - | 跳過 (樣例對不上, 模型未定) |
| [i236](https://zerojudge.tw/ShowProblem?problemid=i236) | 邊緣人 (NPSC2020) | - | 跳過 (難, 除數分塊數論) |

## c500 補 AC 筆記（2026-10-07）

> 這題一開始 NA 0%（全 4 子題 WA）。坑在於**判題用的模型跟「物理最佳解」不一樣**，而兩組公開範例都剛好測不出差別。

- **題意**：n 個座位排一直線，兔吉在第 m 個；k 隻滑鼠彼此至少間隔 f 格。滑鼠在兔吉右邊第 t 格會造成 `d·t` 傷害（左邊不計），要分配顏色(各有固定每格傷害值)使總傷害最小。
- **判題模型（對照作者解題報告）**：滑鼠就是**從第 1 格起、每 f 格放一隻** → 位置 `1, 1+f, …, 1+(k-1)f`（報告的 `1,4,7,10` 即此）。位置 > m 的才算傷害；位置 ≤ m（**含剛好落在 m 的那隻**）都當 0。把最大的傷害值擺在這些安全位置、右側由遠到近配最小/次小… 即最小化。
- **我原本的錯誤**：把 m 當成「不能放、要跳過」而把右側第一隻擠到 `m+1`（距離 1）。但判題模型容許一隻**剛好落在 m（距離 0、免費）**、其餘從 `m+f` 起算。
  - 兩者**只有在 `m` 剛好落在格點上（`m ≡ 1 (mod f)`，例如 `f=1` 必中）時才會不同**，所以兩組範例（f=3 無碰撞、f=1 但 k 太小沒越過 m）都驗不出來。
  - 修正：左側數量用 `⌊(m-1)/f⌋+1`（含落在 m 的），右側位置直接 `1+(Lcnt+j)·f`，不再把 m 當禁止格。
- 教訓：公開範例全過又 WA 時，先懷疑**判題模型**而非實作；拿「與自己同模型的 brute」對拍只會一起錯。要找一個**和題目來源獨立**的參照（這裡是作者的排法敘述）。
