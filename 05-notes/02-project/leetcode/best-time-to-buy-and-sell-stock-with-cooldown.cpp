// LeetCode #309 Best Time to Buy and Sell Stock with Cooldown (Med)
// 三狀態 DP: hold (持股) / sold (剛賣) / rest (空手且可買), 冷卻靠 rest 延遲 sold
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int hold = INT_MIN, sold = 0, rest = 0;
        for (int p : prices) {
            int prevSold = sold;
            sold = hold + p;
            hold = max(hold, rest - p);
            rest = max(rest, prevSold);
        }
        return max(sold, rest);
    }
};
