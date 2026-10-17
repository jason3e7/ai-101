// ZeroJudge b589 - 超級馬拉松賽: 每條路徑可正常(P)或加速(2P但下一條休息得0), 最大化總分
// dp0=非休息狀態最佳, dp1=休息狀態最佳 (由後往前)
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n && n != 0) {
        vector<long long> p(n);
        for (auto& x : p) cin >> x;
        long long dp0 = 0, dp1 = 0;
        for (int i = n - 1; i >= 0; i--) {
            long long n0 = max(p[i] + dp0, 2 * p[i] + dp1);
            long long n1 = dp0;
            dp0 = n0; dp1 = n1;
        }
        cout << dp0 << "\n";
    }
    return 0;
}
