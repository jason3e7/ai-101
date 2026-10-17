// LeetCode #10 Regular Expression Matching (Hard)
// 2D DP, 處理 . (任一字元) 跟 * (前一字元 0 或多次)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isMatch(string s, string p) {
        int n = s.size(), m = p.size();
        vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
        dp[0][0] = 1;
        for (int j = 1; j <= m; j++) if (p[j-1] == '*') dp[0][j] = dp[0][j-2];
        for (int i = 1; i <= n; i++) for (int j = 1; j <= m; j++) {
            if (p[j-1] == '*') {
                dp[i][j] = dp[i][j-2];
                if (p[j-2] == '.' || p[j-2] == s[i-1]) dp[i][j] = dp[i][j] || dp[i-1][j];
            } else if (p[j-1] == '.' || p[j-1] == s[i-1]) {
                dp[i][j] = dp[i-1][j-1];
            }
        }
        return dp[n][m];
    }
};
