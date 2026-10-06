// ZeroJudge a225 - 明明愛排列: 先依個位數由小到大, 個位數相同則數值由大到小
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        vector<long long> v(n);
        for (auto& x : v) cin >> x;
        sort(v.begin(), v.end(), [](long long a, long long b) {
            int ua = a % 10, ub = b % 10;
            if (ua != ub) return ua < ub;
            return a > b;
        });
        for (int i = 0; i < n; i++) { if (i) cout << " "; cout << v[i]; }
        cout << "\n";
    }
    return 0;
}
