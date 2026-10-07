// ZeroJudge d904 - 換零錢: N 種硬幣湊 C 分, 最少硬幣數 (無限供應, 保證湊得出)
#include <bits/stdc++.h>
using namespace std;
int main(){
    int C, N;
    while (cin >> C >> N) {
        vector<int> v(N);
        for (auto& x : v) cin >> x;
        vector<int> dp(C + 1, INT_MAX);
        dp[0] = 0;
        for (int a = 1; a <= C; a++)
            for (int c : v) if (a >= c && dp[a-c] != INT_MAX) dp[a] = min(dp[a], dp[a-c] + 1);
        cout << dp[C] << "\n";
    }
    return 0;
}
