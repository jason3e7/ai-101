// LeetCode #2462 Total Cost to Hire K Workers (Med)
// 左右各一個 min-heap, 每輪取較小者; 用兩個指針擴充窗口
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long totalCost(vector<int>& costs, int k, int candidates) {
        int n = costs.size();
        priority_queue<int, vector<int>, greater<int>> L, R;
        int l = 0, r = n - 1;
        while (l < candidates && l <= r) { L.push(costs[l++]); }
        while (r >= n - candidates && r >= l) { R.push(costs[r--]); }
        long long total = 0;
        for (int i = 0; i < k; i++) {
            if (R.empty() || (!L.empty() && L.top() <= R.top())) {
                total += L.top(); L.pop();
                if (l <= r) L.push(costs[l++]);
            } else {
                total += R.top(); R.pop();
                if (l <= r) R.push(costs[r--]);
            }
        }
        return total;
    }
};
