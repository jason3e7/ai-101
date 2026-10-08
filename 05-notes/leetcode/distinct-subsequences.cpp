// LeetCode #115 Distinct Subsequences (Hard)
// 2D DP: dp[i][j] = s 前 i 子序列匹配 t 前 j 的方法數
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numDistinct(string s, string t) {
        int n = s.size(), m = t.size();
        vector<vector<unsigned long long>> dp(n+1, vector<unsigned long long>(m+1, 0));
        for (int i = 0; i <= n; i++) dp[i][0] = 1;
        for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
            dp[i][j] = dp[i-1][j];
            if (s[i-1] == t[j-1]) dp[i][j] += dp[i-1][j-1];
        }
        return (int)dp[n][m];
    }
};
