// ZeroJudge a676 / UVa111 - History Grading
// 分數 = 標準與學生「年代順序」的最長共同子序列(LCS)
// 作法: 依標準年代順序走訪事件, 取學生排名的 LIS
#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    if (scanf("%d", &n) != 1) return 0;
    vector<int> S(n + 1), eventByRank(n + 1);
    for (int i = 1; i <= n; i++) { scanf("%d", &S[i]); eventByRank[S[i]] = i; }
    vector<int> R(n + 1);
    // 逐個學生
    while (true) {
        bool got = false;
        for (int i = 1; i <= n; i++) { if (scanf("%d", &R[i]) != 1) { got = (i>1); goto done; } }
        got = true;
        {
            // T[r] = 學生對「標準第 r 名事件」給的排名
            vector<int> T(n);
            for (int r = 1; r <= n; r++) T[r-1] = R[eventByRank[r]];
            // LIS (嚴格遞增) O(n^2)
            vector<int> dp(n, 1); int best = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < i; j++) if (T[j] < T[i]) dp[i] = max(dp[i], dp[j]+1);
                best = max(best, dp[i]);
            }
            printf("%d\n", best);
        }
    }
done:
    return 0;
}
