// LeetCode #417 Pacific Atlantic Water Flow (Med)
// 從兩大洋邊界反向 DFS, 找同時被 pac/atl 到達的 cell
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& h) {
        int m = h.size(), n = h[0].size();
        vector<vector<char>> pac(m, vector<char>(n, 0)), atl(m, vector<char>(n, 0));
        function<void(int,int,vector<vector<char>>&)> dfs = [&](int r, int c, vector<vector<char>>& v) {
            if (v[r][c]) return;
            v[r][c] = 1;
            int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && h[nr][nc] >= h[r][c])
                    dfs(nr, nc, v);
            }
        };
        for (int i = 0; i < m; i++) { dfs(i, 0, pac); dfs(i, n-1, atl); }
        for (int j = 0; j < n; j++) { dfs(0, j, pac); dfs(m-1, j, atl); }
        vector<vector<int>> res;
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++)
            if (pac[i][j] && atl[i][j]) res.push_back({i, j});
        return res;
    }
};
