// LeetCode #547 Number of Provinces (Med)
// Union-Find: 從 n 開始每成功 union -1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findCircleNum(vector<vector<int>>& m) {
        int n = m.size();
        vector<int> par(n);
        iota(par.begin(), par.end(), 0);
        function<int(int)> find = [&](int x) { return par[x] == x ? x : par[x] = find(par[x]); };
        int cnt = n;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (m[i][j]) { int a = find(i), b = find(j); if (a != b) { par[a] = b; cnt--; } }
        return cnt;
    }
};
