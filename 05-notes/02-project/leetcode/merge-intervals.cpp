// LeetCode #56 Merge Intervals (Med)
// 排序後線性掃, 跟最後一段重疊就延伸, 不重疊就 push 新段
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& iv) {
        sort(iv.begin(), iv.end());
        vector<vector<int>> res;
        for (auto& x : iv) {
            if (!res.empty() && x[0] <= res.back()[1]) res.back()[1] = max(res.back()[1], x[1]);
            else res.push_back(x);
        }
        return res;
    }
};
