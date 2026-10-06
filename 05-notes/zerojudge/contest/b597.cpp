// ZeroJudge b597 - Stickst (經典 Sticks): 求最小原始木棍長度, DFS + 剪枝
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int n, L;
vector<int> a;
vector<bool> used;

bool dfs(int sticksLeft, int curLen, int pos) {
    if (sticksLeft == 0) return true;
    if (curLen == L) return dfs(sticksLeft - 1, 0, 0);
    for (int i = pos; i < n; i++) {
        if (!used[i] && curLen + a[i] <= L) {
            used[i] = true;
            if (dfs(sticksLeft, curLen + a[i], i + 1)) return true;
            used[i] = false;
            if (curLen == 0 || curLen + a[i] == L) return false;
            while (i + 1 < n && a[i + 1] == a[i]) i++;
        }
    }
    return false;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    while (cin >> n && n != 0) {
        a.assign(n, 0);
        int sum = 0, mx = 0;
        for (auto& x : a) { cin >> x; sum += x; mx = max(mx, x); }
        sort(a.rbegin(), a.rend());
        int ans = sum;
        for (L = mx; L <= sum; L++) {
            if (sum % L) continue;
            used.assign(n, false);
            if (dfs(sum / L, 0, 0)) { ans = L; break; }
        }
        cout << ans << "\n";
    }
    return 0;
}
