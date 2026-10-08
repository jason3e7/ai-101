// LeetCode #153 Find Minimum in Rotated Sorted Array (Med)
// 二分: 跟 nums[r] 比, 若 mid > r 則最小在右半
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findMin(vector<int>& nums) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int m = (l + r) / 2;
            if (nums[m] > nums[r]) l = m + 1;
            else r = m;
        }
        return nums[l];
    }
};
