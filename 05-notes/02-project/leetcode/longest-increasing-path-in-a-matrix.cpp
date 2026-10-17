// LeetCode #329 Longest Increasing Path in a Matrix (Hard)
// 記憶化 DFS, 每格記最長下降路徑
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestIncreasingPath(vector<vector<int>>& m) {
        int n = m.size(), c = m[0].size(), best = 0;
        vector<vector<int>> memo(n, vector<int>(c, 0));
        int dr[] = {1,-1,0,0}, dc[] = {0,0,1,-1};
        function<int(int,int)> dfs = [&](int r, int cc) -> int {
            if (memo[r][cc]) return memo[r][cc];
            int res = 1;
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = cc + dc[k];
                if (nr < 0 || nc < 0 || nr >= n || nc >= c) continue;
                if (m[nr][nc] <= m[r][cc]) continue;
                res = max(res, 1 + dfs(nr, nc));
            }
            return memo[r][cc] = res;
        };
        for (int i = 0; i < n; i++) for (int j = 0; j < c; j++) best = max(best, dfs(i, j));
        return best;
    }
};
