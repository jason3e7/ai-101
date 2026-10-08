// LeetCode #45 Jump Game II (Med)
// 貪心 BFS, 走到當前 end 時再開新一跳
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int jump(vector<int>& nums) {
        int jumps = 0, end = 0, far = 0, n = nums.size();
        for (int i = 0; i < n - 1; i++) {
            far = max(far, i + nums[i]);
            if (i == end) { jumps++; end = far; }
        }
        return jumps;
    }
};
