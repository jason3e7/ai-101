// LeetCode #22 Generate Parentheses (Med)
// Backtracking: 保持 ')' <= '(' 的計數條件
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        string cur;
        function<void(int,int)> dfs = [&](int o, int c) {
            if ((int)cur.size() == 2*n) { res.push_back(cur); return; }
            if (o < n) { cur.push_back('('); dfs(o+1, c); cur.pop_back(); }
            if (c < o) { cur.push_back(')'); dfs(o, c+1); cur.pop_back(); }
        };
        dfs(0, 0);
        return res;
    }
};
