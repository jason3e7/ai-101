// LeetCode #1143 Longest Common Subsequence (Med)
// 經典 2D DP 用 1D 滾動: 保留 prev 當對角線值
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestCommonSubsequence(string a, string b) {
        int m = a.size(), n = b.size();
        vector<int> dp(n + 1, 0);
        for (int i = 1; i <= m; i++) {
            int prev = 0;
            for (int j = 1; j <= n; j++) {
                int tmp = dp[j];
                dp[j] = (a[i-1] == b[j-1]) ? prev + 1 : max(dp[j], dp[j-1]);
                prev = tmp;
            }
        }
        return dp[n];
    }
};
