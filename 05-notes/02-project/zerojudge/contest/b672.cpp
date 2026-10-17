// ZeroJudge b672 - A Special Automobile Race: 最少停靠站數 (Jump Game II)
// 起點(0)可達前5站; 站 i 可達 i+a[i]; 求抵達終點(>n)最少停靠站數
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n;
    while (cin >> n && n != 0) {
        vector<int> a(n + 1);
        a[0] = 5;
        for (int i = 1; i <= n; i++) cin >> a[i];
        int jumps = 0, curEnd = 0;
        long long far = 0;
        bool done = false;
        for (int i = 0; i <= n && !done; i++) {
            far = max(far, (long long)i + a[i]);
            if (far >= n + 1) { cout << jumps << "\n"; done = true; break; }
            if (i == curEnd) { jumps++; curEnd = (int)min((long long)n, far); }
        }
        if (!done) cout << jumps << "\n";
    }
    return 0;
}
