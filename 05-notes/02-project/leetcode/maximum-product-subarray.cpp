// LeetCode #152 Maximum Product Subarray (Med)
// 需要同時追 max 跟 min, 因為負乘負變正
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int mx = nums[0], mn = nums[0], best = nums[0];
        for (int i = 1; i < (int)nums.size(); i++) {
            int a = nums[i], b = mx * a, c = mn * a;
            mx = max({a, b, c});
            mn = min({a, b, c});
            best = max(best, mx);
        }
        return best;
    }
};
