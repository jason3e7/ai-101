// LeetCode #167 Two Sum II - Input Array Is Sorted (Med)
// 排序後雙指針夾擠, 回傳 1-indexed
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int s = nums[l] + nums[r];
            if (s == target) return {l+1, r+1};
            if (s < target) l++; else r--;
        }
        return {};
    }
};
