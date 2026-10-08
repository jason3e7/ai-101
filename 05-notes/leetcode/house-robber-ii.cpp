// LeetCode #213 House Robber II (Med)
// 房子成環: 分兩情況, [0..n-2] 跟 [1..n-1] 各跑一次 House Robber, 取 max
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int robRange(vector<int>& a, int l, int r) {
        int prev = 0, cur = 0;
        for (int i = l; i <= r; i++) { int nx = max(cur, prev + a[i]); prev = cur; cur = nx; }
        return cur;
    }
public:
    int rob(vector<int>& nums) {
        int n = nums.size();
        if (n == 1) return nums[0];
        return max(robRange(nums, 0, n - 2), robRange(nums, 1, n - 1));
    }
};
