// LeetCode #518 Coin Change II (Med)
// 完全背包方案數 DP: 外迴圈硬幣, 內迴圈金額, 避免算到排列
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<unsigned long long> dp(amount+1, 0);
        dp[0] = 1;
        for (int c : coins) for (int a = c; a <= amount; a++) dp[a] += dp[a-c];
        return (int)dp[amount];
    }
};
