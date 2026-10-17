// LeetCode #312 Burst Balloons (Hard)
// 區間 DP, 外填 1 當邊界, 枚舉最後戳的 k, dp[l][r] = max(dp[l][k] + dp[k][r] + a[l]*a[k]*a[r])
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxCoins(vector<int>& nums) {
        vector<int> a = {1};
        a.insert(a.end(), nums.begin(), nums.end());
        a.push_back(1);
        int n = a.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int len = 2; len < n; len++) {
            for (int l = 0; l + len < n; l++) {
                int r = l + len;
                for (int k = l + 1; k < r; k++) {
                    dp[l][r] = max(dp[l][r], dp[l][k] + dp[k][r] + a[l]*a[k]*a[r]);
                }
            }
        }
        return dp[0][n-1];
    }
};
