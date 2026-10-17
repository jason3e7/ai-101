// ZeroJudge c100 / UVa116 - Unidirectional TSP
// 左到右走格(上/平/下, 列首尾相鄰)最小重量路徑; 平手取字典序最小列序列
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    int m, n;
    while (scanf("%d %d", &m, &n) == 2) {
        vector<vector<ll>> g(m, vector<ll>(n));
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) scanf("%lld", &g[i][j]);
        vector<vector<ll>> dp(m, vector<ll>(n));
        vector<vector<int>> nxt(m, vector<int>(n, -1));
        for (int i = 0; i < m; i++) dp[i][n-1] = g[i][n-1];
        for (int j = n - 2; j >= 0; j--) {
            for (int i = 0; i < m; i++) {
                int cand[3] = { (i-1+m)%m, i, (i+1+m)%m };
                sort(cand, cand+3);                 // 遞增 -> 平手自然取小列
                ll best = LLONG_MAX; int bestR = -1;
                for (int t = 0; t < 3; t++) {
                    int r = cand[t];
                    if (t && r == cand[t-1]) continue; // 去重
                    ll c = dp[r][j+1];
                    if (c < best) { best = c; bestR = r; }
                }
                dp[i][j] = g[i][j] + best;
                nxt[i][j] = bestR;
            }
        }
        ll best = LLONG_MAX; int start = 0;
        for (int i = 0; i < m; i++) if (dp[i][0] < best) { best = dp[i][0]; start = i; }
        // 重建
        int r = start;
        for (int j = 0; j < n; j++) {
            printf("%d%c", r + 1, j+1<n?' ':'\n');
            r = nxt[r][j];
        }
        printf("%lld\n", best);
    }
    return 0;
}
