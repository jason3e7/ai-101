// LeetCode #39 Combination Sum (Med)
// 回溯 + 剪枝: 排序後只往後選 (避免重複), c[k] <= rem 剪掉無效分支
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& c, int t) {
        sort(c.begin(), c.end());
        vector<vector<int>> res;
        vector<int> cur;
        function<void(int,int)> dfs = [&](int i, int rem) {
            if (rem == 0) { res.push_back(cur); return; }
            for (int k = i; k < (int)c.size() && c[k] <= rem; k++) {
                cur.push_back(c[k]);
                dfs(k, rem - c[k]);
                cur.pop_back();
            }
        };
        dfs(0, t);
        return res;
    }
};
