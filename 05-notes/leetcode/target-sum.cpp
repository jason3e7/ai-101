// LeetCode #494 Target Sum (Med)
// 轉子集和: 設正子集和 P, 負子集和 N; P - N = target, P + N = sum => P = (sum+target)/2
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (abs(target) > sum || ((sum + target) & 1)) return 0;
        int S = (sum + target) / 2;
        vector<int> dp(S+1, 0);
        dp[0] = 1;
        for (int x : nums) for (int a = S; a >= x; a--) dp[a] += dp[a-x];
        return dp[S];
    }
};
