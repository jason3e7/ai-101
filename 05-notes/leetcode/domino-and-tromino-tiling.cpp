// LeetCode #790 Domino and Tromino Tiling (Med)
// DP: dp[i] = 2*dp[i-1] + dp[i-3] (推導自 L 形共用情境)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numTilings(int n) {
        const int MOD = 1e9 + 7;
        if (n == 1) return 1;
        if (n == 2) return 2;
        if (n == 3) return 5;
        vector<long long> dp(n+1, 0);
        dp[1]=1; dp[2]=2; dp[3]=5;
        for (int i = 4; i <= n; i++) dp[i] = (2*dp[i-1] + dp[i-3]) % MOD;
        return dp[n];
    }
};
