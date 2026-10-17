// LeetCode #283 Move Zeroes (Easy)
// Write pointer 先壓縮非零, 剩尾部補 0
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int w = 0;
        for (int x : nums) if (x) nums[w++] = x;
        while (w < (int)nums.size()) nums[w++] = 0;
    }
};
