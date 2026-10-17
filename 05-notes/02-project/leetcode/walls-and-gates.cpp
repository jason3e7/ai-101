// LeetCode #286 Walls and Gates (Med, PREMIUM — 本地解未送判)
// 多源 BFS: 全部門 (0) 同時入 queue, 一波波擴散, 每格距離就是最近門距離
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void wallsAndGates(vector<vector<int>>& rooms) {
        int n = rooms.size(), m = rooms[0].size();
        queue<pair<int,int>> q;
        for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) if (rooms[i][j] == 0) q.push({i, j});
        int dr[] = {1,-1,0,0}, dc[] = {0,0,1,-1};
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nc < 0 || nr >= n || nc >= m) continue;
                if (rooms[nr][nc] != INT_MAX) continue;
                rooms[nr][nc] = rooms[r][c] + 1;
                q.push({nr, nc});
            }
        }
    }
};
