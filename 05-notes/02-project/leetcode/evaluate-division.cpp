// LeetCode #399 Evaluate Division (Med)
// 建加權圖 (a/b -> edge weight), BFS 找 path 累乘
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& eqs, vector<double>& vals, vector<vector<string>>& qs) {
        unordered_map<string, vector<pair<string,double>>> g;
        for (int i = 0; i < (int)eqs.size(); i++) {
            g[eqs[i][0]].push_back({eqs[i][1], vals[i]});
            g[eqs[i][1]].push_back({eqs[i][0], 1.0 / vals[i]});
        }
        vector<double> res;
        for (auto& q : qs) {
            if (!g.count(q[0]) || !g.count(q[1])) { res.push_back(-1.0); continue; }
            if (q[0] == q[1]) { res.push_back(1.0); continue; }
            unordered_set<string> vis; vis.insert(q[0]);
            queue<pair<string,double>> bfs; bfs.push({q[0], 1.0});
            double ans = -1.0;
            while (!bfs.empty()) {
                auto [u, v] = bfs.front(); bfs.pop();
                if (u == q[1]) { ans = v; break; }
                for (auto& [nb, w] : g[u]) if (vis.insert(nb).second) bfs.push({nb, v * w});
            }
            res.push_back(ans);
        }
        return res;
    }
};
