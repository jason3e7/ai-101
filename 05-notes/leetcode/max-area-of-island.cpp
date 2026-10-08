// LeetCode #695 Max Area of Island (Med)
// DFS 吃掉連通塊同時數大小, 直接改成 0 避免 visited 陣列
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& g) {
        int n = g.size(), m = g[0].size(), best = 0;
        function<int(int,int)> dfs = [&](int r, int c) {
            if (r < 0 || c < 0 || r >= n || c >= m || g[r][c] != 1) return 0;
            g[r][c] = 0;
            return 1 + dfs(r+1,c) + dfs(r-1,c) + dfs(r,c+1) + dfs(r,c-1);
        };
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (g[i][j]) best = max(best, dfs(i, j));
        return best;
    }
};
