// LeetCode #97 Interleaving String (Med)
// 2D DP: dp[i][j] = s1 前 i + s2 前 j 能拼出 s3 前 i+j
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int n = s1.size(), m = s2.size();
        if (n + m != (int)s3.size()) return false;
        vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        dp[0][0] = 1;
        for (int i = 0; i <= n; i++) for (int j = 0; j <= m; j++) {
            if (i > 0 && s1[i-1] == s3[i+j-1]) dp[i][j] |= dp[i-1][j];
            if (j > 0 && s2[j-1] == s3[i+j-1]) dp[i][j] |= dp[i][j-1];
        }
        return dp[n][m];
    }
};
