// LeetCode #198 House Robber (Med)
// DP: 當前 = max(前一, 前兩 + 當前房)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int rob(vector<int>& nums) {
        int prev = 0, cur = 0;
        for (int x : nums) { int nxt = max(cur, prev + x); prev = cur; cur = nxt; }
        return cur;
    }
};
