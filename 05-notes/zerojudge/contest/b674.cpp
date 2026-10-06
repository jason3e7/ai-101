// ZeroJudge b674 - Is It A Tree: 有向邊集合是否成樹
// 條件: 每點入度<=1、恰一個根(入度0)、E=V-1、從根可達所有點(連通無環)
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int m;
    while (cin >> m && m != 0) {
        map<int, vector<int>> adj;
        map<int, int> indeg;
        set<int> nodes;
        for (int i = 0; i < m; i++) {
            int u, v; cin >> u >> v;
            adj[u].push_back(v);
            indeg[v]++;
            nodes.insert(u); nodes.insert(v);
            if (!indeg.count(u)) indeg[u] = indeg[u]; // ensure key
        }
        int V = nodes.size();
        bool ok = true;
        for (int nd : nodes) if (indeg[nd] > 1) ok = false;
        // roots
        int root = -1, rootCnt = 0;
        for (int nd : nodes) if (indeg[nd] == 0) { rootCnt++; root = nd; }
        if (rootCnt != 1) ok = false;
        if (m != V - 1) ok = false;
        if (ok) {
            // BFS from root
            set<int> seen;
            queue<int> q; q.push(root); seen.insert(root);
            while (!q.empty()) {
                int x = q.front(); q.pop();
                for (int y : adj[x]) if (!seen.count(y)) { seen.insert(y); q.push(y); }
            }
            if ((int)seen.size() != V) ok = false;
        }
        cout << (ok ? "y" : "n") << "\n";
    }
    return 0;
}
