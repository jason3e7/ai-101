// LeetCode #787 Cheapest Flights Within K Stops (Med)
// Bellman-Ford k+1 輪, 用 tmp 複本避免同輪污染
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<int> dist(n, INT_MAX);
        dist[src] = 0;
        for (int i = 0; i <= k; i++) {
            vector<int> tmp = dist;
            for (auto& f : flights) if (dist[f[0]] != INT_MAX) tmp[f[1]] = min(tmp[f[1]], dist[f[0]] + f[2]);
            dist = tmp;
        }
        return dist[dst] == INT_MAX ? -1 : dist[dst];
    }
};
