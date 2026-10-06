// ZeroJudge a042 - 平面圓形切割: n 個圓最多把平面切成 n^2 - n + 2 塊 (n>=1)
#include <iostream>
using namespace std;

int main() {
    long long n;
    while (cin >> n) {
        if (n == 0) cout << 1 << "\n";
        else cout << n * n - n + 2 << "\n";
    }
    return 0;
}
