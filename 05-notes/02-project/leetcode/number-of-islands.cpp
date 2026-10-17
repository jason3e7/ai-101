// LeetCode #200 Number of Islands (Med)
// DFS 淹沒遇到的 '1', 每次新 '1' 就 +1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numIslands(vector<vector<char>>& g) {
        int m = g.size(), n = g[0].size(), cnt = 0;
        function<void(int,int)> dfs = [&](int r, int c) {
            if (r < 0 || r >= m || c < 0 || c >= n || g[r][c] != '1') return;
            g[r][c] = '0';
            dfs(r+1,c); dfs(r-1,c); dfs(r,c+1); dfs(r,c-1);
        };
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (g[i][j] == '1') { cnt++; dfs(i, j); }
        return cnt;
    }
};
