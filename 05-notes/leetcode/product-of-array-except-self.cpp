// LeetCode #238 Product of Array Except Self (Med)
// 兩次掃: 先存左乘積, 再乘右乘積. 不能用除法 (有 0 的情況)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        int n = nums.size();
        vector<int> res(n, 1);
        int lf = 1;
        for (int i = 0; i < n; i++) { res[i] = lf; lf *= nums[i]; }
        int rt = 1;
        for (int i = n - 1; i >= 0; i--) { res[i] *= rt; rt *= nums[i]; }
        return res;
    }
};
