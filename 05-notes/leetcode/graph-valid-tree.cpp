// LeetCode #261 Graph Valid Tree (Med) — PREMIUM, 無法在 LeetCode 送判
// 樹的條件: n 個節點 + 剛好 n-1 條邊 + 連通 + 無環
// 用 Union-Find: 任一 union 時發現 roots 已相同 -> 有環
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        if ((int)edges.size() != n - 1) return false;
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        function<int(int)> find = [&](int x) { return par[x] == x ? x : par[x] = find(par[x]); };
        for (auto& e : edges) {
            int a = find(e[0]), b = find(e[1]);
            if (a == b) return false;  // 有環
            par[a] = b;
        }
        return true;
    }
};
