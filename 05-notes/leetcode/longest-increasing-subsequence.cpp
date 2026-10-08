// LeetCode #300 Longest Increasing Subsequence (Med)
// Patience sort O(n log n): tails[i] = 長度 i+1 的 LIS 最小尾值, lower_bound 更新
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;
        for (int x : nums) {
            auto it = lower_bound(tails.begin(), tails.end(), x);
            if (it == tails.end()) tails.push_back(x);
            else *it = x;
        }
        return tails.size();
    }
};
