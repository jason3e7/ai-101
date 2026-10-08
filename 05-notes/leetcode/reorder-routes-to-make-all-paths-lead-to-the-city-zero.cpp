// LeetCode #1466 Reorder Routes to Make All Paths Lead to the City Zero (Med)
// 建無向圖含方向資訊: 原方向 cost=1 (需反轉), 反方向 cost=0
// 從 0 DFS, 累計 cost 就是答案
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minReorder(int n, vector<vector<int>>& conn) {
        vector<vector<pair<int,int>>> g(n);
        for (auto& e : conn) {
            g[e[0]].push_back({e[1], 1});
            g[e[1]].push_back({e[0], 0});
        }
        vector<char> vis(n, 0);
        int res = 0;
        function<void(int)> dfs = [&](int u) {
            vis[u] = 1;
            for (auto& [v, c] : g[u]) if (!vis[v]) { res += c; dfs(v); }
        };
        dfs(0);
        return res;
    }
};
