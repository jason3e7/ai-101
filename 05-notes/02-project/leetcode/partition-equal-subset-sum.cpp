// LeetCode #416 Partition Equal Subset Sum (Med)
// 子集和背包: bitset 把 O(n·sum) 常數再砍 1/64
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canPartition(vector<int>& nums) {
        int sum = accumulate(nums.begin(), nums.end(), 0);
        if (sum & 1) return false;
        int target = sum / 2;
        bitset<10001> dp;
        dp[0] = 1;
        for (int x : nums) dp |= dp << x;
        return dp[target];
    }
};
