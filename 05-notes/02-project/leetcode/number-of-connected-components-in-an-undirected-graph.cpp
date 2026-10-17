// LeetCode #323 Number of Connected Components in an Undirected Graph (Med) — PREMIUM
// Union-Find: 從 n 開始, 每成功 union 就 -1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        function<int(int)> find = [&](int x) { return par[x] == x ? x : par[x] = find(par[x]); };
        int cnt = n;
        for (auto& e : edges) {
            int a = find(e[0]), b = find(e[1]);
            if (a != b) { par[a] = b; cnt--; }
        }
        return cnt;
    }
};
