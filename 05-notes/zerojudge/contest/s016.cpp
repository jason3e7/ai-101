// ZeroJudge s016 - 毛毛蟲樹 (Caterpillar): 去掉葉子後剩下的須是一條路徑
// 等價: 每個非葉節點 (度>=2), 其非葉鄰居數 <= 2
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    vector<vector<int>> adj(n + 1);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v); adj[v].push_back(u);
        deg[u]++; deg[v]++;
    }
    bool ok = true;
    for (int v = 1; v <= n && ok; v++) {
        if (deg[v] >= 2) {
            int c = 0;
            for (int u : adj[v]) if (deg[u] >= 2) c++;
            if (c > 2) ok = false;
        }
    }
    cout << (ok ? "Yes" : "No") << "\n";
    return 0;
}
