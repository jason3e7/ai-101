// LeetCode #1584 Min Cost to Connect All Points (Med)
// Prim's MST: O(n²), 每輪選最近未加入點; 曼哈頓距離當邊權
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minCostConnectPoints(vector<vector<int>>& pts) {
        int n = pts.size();
        vector<int> dist(n, INT_MAX), vis(n, 0);
        dist[0] = 0;
        int res = 0;
        for (int i = 0; i < n; i++) {
            int u = -1;
            for (int j = 0; j < n; j++) if (!vis[j] && (u == -1 || dist[j] < dist[u])) u = j;
            vis[u] = 1;
            res += dist[u];
            for (int v = 0; v < n; v++) if (!vis[v]) {
                int d = abs(pts[u][0]-pts[v][0]) + abs(pts[u][1]-pts[v][1]);
                dist[v] = min(dist[v], d);
            }
        }
        return res;
    }
};
