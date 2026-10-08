// LeetCode #714 Best Time to Buy and Sell Stock with Transaction Fee (Med)
// 兩個狀態 DP: cash (未持有) 與 hold (持有), 賣出時扣 fee
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int cash = 0, hold = -prices[0];
        for (int i = 1; i < (int)prices.size(); i++) {
            cash = max(cash, hold + prices[i] - fee);
            hold = max(hold, cash - prices[i]);
        }
        return cash;
    }
};
