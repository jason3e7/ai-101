// ZeroJudge c500 - AEWE-645的傷害: 直線 n 座位, 兔吉在 m, k 隻滑鼠間隔 >= f.
// 關鍵(對照判題): 滑鼠從第 1 個座位起、每 f 格放一隻 -> 位置 1,1+f,...,1+(k-1)f
// (作者解題報告即 1,4,7,10 這樣排). 位置 > m 的才算傷害, 傷害 = d*(位置-m).
// 位置 <= m 的(含剛好落在 m 的)不計傷害; 故把最大的傷害值擺在這些「安全」位置,
// 右側位置由遠到近配最小、次小... 的傷害值即最小化. 多筆測資讀到 EOF.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    long long k, n, m, f;
    while (cin >> k >> n >> m >> f) {
        vector<long long> d(k);
        for (auto& x : d) cin >> x;
        sort(d.begin(), d.end());                 // 由小到大
        long long Lcnt = (m - 1) / f + 1;          // 位置(1+j*f)落在 <= m 的數量(含落在 m)
        if (Lcnt > k) Lcnt = k;
        long long R = k - Lcnt;                    // 需計傷害(位置 > m)的滑鼠數
        long long sum = 0;
        for (long long j = 0; j < R; j++) {
            long long pos = 1 + (Lcnt + j) * f;    // 第 (Lcnt+j) 個位置(0-indexed)
            long long dist = pos - m;              // 在兔吉右邊的格數
            long long dd = d[R - 1 - j];           // 近的配大傷害, 遠的配小傷害
            sum += dd * dist;
        }
        cout << sum << "\n";
    }
    return 0;
}
