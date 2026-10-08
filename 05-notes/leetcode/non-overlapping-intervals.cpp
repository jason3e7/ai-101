// LeetCode #435 Non-overlapping Intervals (Med)
// 貪心: 按 end 排序, 保留 end 最小的, 移除重疊
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int eraseOverlapIntervals(vector<vector<int>>& iv) {
        sort(iv.begin(), iv.end(), [](auto& a, auto& b){ return a[1] < b[1]; });
        int cnt = 0, end = INT_MIN;
        for (auto& x : iv) {
            if (x[0] >= end) end = x[1];
            else cnt++;
        }
        return cnt;
    }
};
