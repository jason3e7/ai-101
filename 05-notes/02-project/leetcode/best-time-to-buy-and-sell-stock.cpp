// LeetCode #121 Best Time to Buy and Sell Stock (Easy)
// 一遍掃: 追最小買入價, 每天算當天賣的 profit, 取 max
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int mn = INT_MAX, best = 0;
        for (int p : prices) { mn = min(mn, p); best = max(best, p - mn); }
        return best;
    }
};
