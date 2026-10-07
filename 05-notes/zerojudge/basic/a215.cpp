// ZeroJudge a215 - 明明愛數數: 從 n 開始累加 n,n+1,... 直到總和超過 m, 問數了幾個
// 陷阱: n,m 可能很大 (只有 m-n<=1e5), 累加和會爆 long long -> 用 __int128
#include <iostream>
using namespace std;

int main() {
    long long n, m;
    while (cin >> n >> m) {
        __int128 sum = 0;
        long long cur = n, cnt = 0;
        while (sum <= (__int128)m) { sum += cur; cur++; cnt++; }
        cout << cnt << "\n";
    }
    return 0;
}
