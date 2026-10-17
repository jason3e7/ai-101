// LeetCode #51 N-Queens (Hard)
// 回溯, 用 col / diag1 (r+c) / diag2 (r-c+n) 三個 flag 加速衝突檢查
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        vector<int> col(n);
        vector<int> usedCol(n, 0), diag1(2*n, 0), diag2(2*n, 0);
        function<void(int)> dfs = [&](int r) {
            if (r == n) {
                vector<string> b(n, string(n, '.'));
                for (int i = 0; i < n; i++) b[i][col[i]] = 'Q';
                res.push_back(b);
                return;
            }
            for (int c = 0; c < n; c++) {
                if (usedCol[c] || diag1[r+c] || diag2[r-c+n]) continue;
                col[r] = c;
                usedCol[c] = diag1[r+c] = diag2[r-c+n] = 1;
                dfs(r+1);
                usedCol[c] = diag1[r+c] = diag2[r-c+n] = 0;
            }
        };
        dfs(0);
        return res;
    }
};
