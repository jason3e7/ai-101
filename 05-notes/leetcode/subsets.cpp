// LeetCode #78 Subsets (Med)
// bitmask 枚舉, 第 i 位代表 nums[i] 選不選
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> res;
        for (int mask = 0; mask < (1 << n); mask++) {
            vector<int> s;
            for (int i = 0; i < n; i++) if (mask & (1 << i)) s.push_back(nums[i]);
            res.push_back(s);
        }
        return res;
    }
};
