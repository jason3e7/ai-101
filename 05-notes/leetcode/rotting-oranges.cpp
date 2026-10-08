// LeetCode #994 Rotting Oranges (Med)
// Multi-source BFS, 每輪時間 +1, 剩 fresh 代表無法爛完
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int orangesRotting(vector<vector<int>>& g) {
        int m = g.size(), n = g[0].size(), fresh = 0;
        queue<pair<int,int>> q;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (g[i][j] == 2) q.push({i, j});
                else if (g[i][j] == 1) fresh++;
        int dr[4] = {-1,1,0,0}, dc[4] = {0,0,-1,1};
        int t = 0;
        while (!q.empty() && fresh > 0) {
            int sz = q.size();
            while (sz--) {
                auto [r, c] = q.front(); q.pop();
                for (int k = 0; k < 4; k++) {
                    int nr = r + dr[k], nc = c + dc[k];
                    if (nr<0||nr>=m||nc<0||nc>=n||g[nr][nc]!=1) continue;
                    g[nr][nc] = 2; fresh--;
                    q.push({nr, nc});
                }
            }
            t++;
        }
        return fresh ? -1 : t;
    }
};
