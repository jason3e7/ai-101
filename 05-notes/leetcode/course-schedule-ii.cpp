// LeetCode #210 Course Schedule II (Med)
// Kahn's topo sort: 入度為 0 入 queue, 吃掉後把後繼減 1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> findOrder(int n, vector<vector<int>>& prereqs) {
        vector<vector<int>> g(n);
        vector<int> indeg(n, 0);
        for (auto& p : prereqs) { g[p[1]].push_back(p[0]); indeg[p[0]]++; }
        queue<int> q;
        for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
        vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
        }
        return (int)order.size() == n ? order : vector<int>();
    }
};
