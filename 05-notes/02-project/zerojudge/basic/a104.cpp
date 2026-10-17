// ZeroJudge a104 - 排序 (由小到大), 多筆到 EOF
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        vector<long long> v(n);
        for (auto& x : v) cin >> x;
        sort(v.begin(), v.end());
        for (int i = 0; i < n; i++) { if (i) cout << " "; cout << v[i]; }
        cout << "\n";
    }
    return 0;
}
