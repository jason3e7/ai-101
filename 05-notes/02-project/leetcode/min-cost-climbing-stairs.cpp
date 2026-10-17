// LeetCode #746 Min Cost Climbing Stairs (Easy)
// DP: dp[i] = min(dp[i-1]+cost[i-1], dp[i-2]+cost[i-2])
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int n = cost.size(), a = 0, b = 0;
        for (int i = 2; i <= n; i++) {
            int c = min(b + cost[i-1], a + cost[i-2]);
            a = b; b = c;
        }
        return b;
    }
};
