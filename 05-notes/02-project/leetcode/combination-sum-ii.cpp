// LeetCode #40 Combination Sum II (Med)
// 排序後 backtracking, 每個數字只用一次, 同層跳過重複
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& c, int target) {
        sort(c.begin(), c.end());
        vector<vector<int>> res;
        vector<int> cur;
        function<void(int,int)> dfs = [&](int start, int remain) {
            if (remain == 0) { res.push_back(cur); return; }
            for (int i = start; i < (int)c.size(); i++) {
                if (c[i] > remain) break;
                if (i > start && c[i] == c[i-1]) continue;
                cur.push_back(c[i]);
                dfs(i+1, remain - c[i]);
                cur.pop_back();
            }
        };
        dfs(0, target);
        return res;
    }
};
