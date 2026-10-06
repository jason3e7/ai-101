// ZeroJudge c500 - AEWE-645的傷害 (未驗證: 僅對樣例)
// 模型: 老鼠彼此間距 >= f; 傷害大的放左邊(0傷害), 其餘放右邊最靠近位置, 配最小傷害值.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    long long k, n, m, f;
    while (cin >> k >> n >> m >> f) {
        vector<long long> d(k);
        for (auto& x : d) cin >> x;
        sort(d.begin(), d.end());                    // 由小到大
        long long Lcap = (m <= 1) ? 0 : (m - 2) / f + 1;  // 左側 [1,m-1] 以間距 f 最多可放幾隻
        long long L = min(Lcap, k);
        long long R = k - L;
        long long sum = 0;
        if (R > 0) {
            long long start = 1 + L * f;
            if (start <= m) start = m + 1;
            for (long long j = 0; j < R; j++) {
                long long dist = (start + j * f) - m;    // 距離遞增
                long long dd = d[R - 1 - j];             // 右側用最小的 R 個, 大的配近的
                sum += dd * dist;
            }
        }
        cout << sum << "\n";
    }
    return 0;
}
