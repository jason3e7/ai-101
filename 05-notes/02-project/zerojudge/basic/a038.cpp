// ZeroJudge a038 - 數字翻轉 (倒轉數字, 前導 0 消除)
#include <iostream>
using namespace std;

int main() {
    long long n;
    while (cin >> n) {
        bool neg = n < 0;
        long long x = neg ? -n : n, r = 0;
        while (x) { r = r * 10 + x % 10; x /= 10; }
        if (neg) cout << "-";
        cout << r << "\n";
    }
    return 0;
}
