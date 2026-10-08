// LeetCode #79 Word Search (Med)
// DFS 回溯 + 用 '#' 當 visited mark 再還原
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool exist(vector<vector<char>>& b, string word) {
        int m = b.size(), n = b[0].size();
        function<bool(int,int,int)> dfs = [&](int r, int c, int k) -> bool {
            if (k == (int)word.size()) return true;
            if (r < 0 || r >= m || c < 0 || c >= n || b[r][c] != word[k]) return false;
            char tmp = b[r][c]; b[r][c] = '#';
            bool ok = dfs(r+1,c,k+1) || dfs(r-1,c,k+1) || dfs(r,c+1,k+1) || dfs(r,c-1,k+1);
            b[r][c] = tmp;
            return ok;
        };
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (dfs(i, j, 0)) return true;
        return false;
    }
};
