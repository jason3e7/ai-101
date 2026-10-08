// LeetCode #1493 Longest Subarray of 1s After Deleting One Element (Med)
// 類 max-consecutive-ones-iii, 固定刪 1 個, 答案是 r-l (必須刪一個)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        int l = 0, zeros = 0, best = 0;
        for (int r = 0; r < (int)nums.size(); r++) {
            if (nums[r] == 0) zeros++;
            while (zeros > 1) if (nums[l++] == 0) zeros--;
            best = max(best, r - l);
        }
        return best;
    }
};
