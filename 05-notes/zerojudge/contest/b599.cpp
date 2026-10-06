// ZeroJudge b599 - Graph Construction: 判斷度數序列是否可構成簡單圖 (Erdos-Gallai)
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool graphical(vector<long long> d) {
    int n = d.size();
    for (auto x : d) if (x < 0 || x > n - 1) return false;
    sort(d.rbegin(), d.rend());
    long long sum = 0; for (auto x : d) sum += x;
    if (sum % 2) return false;
    vector<long long> pre(n + 1, 0);
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + d[i];
    for (int k = 1; k <= n; k++) {
        long long lhs = pre[k], rhs = (long long)k * (k - 1);
        for (int i = k; i < n; i++) rhs += min(d[i], (long long)k);
        if (lhs > rhs) return false;
    }
    return true;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n && n != 0) {
        vector<long long> d(n);
        for (auto& x : d) cin >> x;
        cout << (graphical(d) ? "Y" : "N") << "\n";
    }
    return 0;
}
