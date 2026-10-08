// LeetCode #216 Combination Sum III (Med)
// Backtracking: 從 start 枚舉 1..9, 選 k 個總和為 n
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        vector<vector<int>> res;
        vector<int> cur;
        function<void(int,int)> dfs = [&](int start, int remain) {
            if ((int)cur.size() == k) { if (remain == 0) res.push_back(cur); return; }
            for (int i = start; i <= 9; i++) {
                if (i > remain) break;
                cur.push_back(i);
                dfs(i+1, remain-i);
                cur.pop_back();
            }
        };
        dfs(1, n);
        return res;
    }
};
