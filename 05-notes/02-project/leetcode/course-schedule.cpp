// LeetCode #207 Course Schedule (Med)
// Kahn 拓撲排序: 若能排完 n 門課則無環
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canFinish(int n, vector<vector<int>>& pre) {
        vector<vector<int>> g(n);
        vector<int> indeg(n, 0);
        for (auto& e : pre) { g[e[1]].push_back(e[0]); indeg[e[0]]++; }
        queue<int> q;
        for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push(i);
        int done = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop(); done++;
            for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
        }
        return done == n;
    }
};
