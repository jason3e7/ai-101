// LeetCode #162 Find Peak Element (Med)
// 二分: 比較 mid 與 mid+1, 往上坡方向收斂
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int lo = 0, hi = nums.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (nums[mid] > nums[mid+1]) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
};
