// ZeroJudge a215 - 明明愛數數: 從 n 開始累加 n,n+1,... 直到總和「超過」(嚴格 >) m, 問數了幾個
// 陷阱: n,m 可以是負數 -> 從 n 開始至少數一個 (do-while, k>=1); 累加和用 __int128 防溢位
#include <iostream>
using namespace std;

int main() {
    long long n, m;
    while (cin >> n >> m) {
        __int128 sum = 0;
        long long cur = n, cnt = 0;
        do { sum += cur; cur++; cnt++; } while (sum <= (__int128)m);
        cout << cnt << "\n";
    }
    return 0;
}
