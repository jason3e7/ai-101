// LeetCode #743 Network Delay Time (Med)
// Dijkstra 從 k 出發, 取最遠距離 (無法到達回 -1)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int,int>>> g(n+1);
        for (auto& t : times) g[t[0]].push_back({t[1], t[2]});
        vector<int> dist(n+1, INT_MAX);
        dist[k] = 0;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({0, k});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto& [v, w] : g[u]) if (d + w < dist[v]) { dist[v] = d + w; pq.push({dist[v], v}); }
        }
        int res = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) return -1;
            res = max(res, dist[i]);
        }
        return res;
    }
};
