// LeetCode #778 Swim in Rising Water (Hard)
// Dijkstra 變種: 路徑成本 = max(格子值), 用 min-heap 取每一步的瓶頸高
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int swimInWater(vector<vector<int>>& g) {
        int n = g.size();
        vector<vector<int>> dist(n, vector<int>(n, INT_MAX));
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
        pq.push({g[0][0], 0, 0});
        dist[0][0] = g[0][0];
        int dr[] = {1,-1,0,0}, dc[] = {0,0,1,-1};
        while (!pq.empty()) {
            auto [d, r, c] = pq.top(); pq.pop();
            if (r == n-1 && c == n-1) return d;
            if (d > dist[r][c]) continue;
            for (int k = 0; k < 4; k++) {
                int nr = r + dr[k], nc = c + dc[k];
                if (nr < 0 || nc < 0 || nr >= n || nc >= n) continue;
                int nd = max(d, g[nr][nc]);
                if (nd < dist[nr][nc]) { dist[nr][nc] = nd; pq.push({nd, nr, nc}); }
            }
        }
        return -1;
    }
};
