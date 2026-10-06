// ZeroJudge a215 - 明明愛數數: 從 n 開始累加 n,n+1,... 直到總和超過 m, 問數了幾個
#include <iostream>
using namespace std;

int main() {
    long long n, m;
    while (cin >> n >> m) {
        long long sum = 0, cur = n, cnt = 0;
        while (sum <= m) { sum += cur; cur++; cnt++; }
        cout << cnt << "\n";
    }
    return 0;
}
