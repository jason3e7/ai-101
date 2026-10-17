// LeetCode #269 Alien Dictionary (Hard) — PREMIUM
// 比相鄰單字找出字母順序關係 -> 拓撲排序
// Edge case: 若 w1 是 w2 的 prefix 但 w1 比較長 -> 無效順序 (回空)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string alienOrder(vector<string>& words) {
        unordered_map<char, unordered_set<char>> g;
        unordered_map<char, int> indeg;
        for (auto& w : words) for (char c : w) indeg[c] = 0;
        for (int i = 0; i + 1 < (int)words.size(); i++) {
            auto& a = words[i]; auto& b = words[i+1];
            int sz = min(a.size(), b.size());
            if (a.size() > b.size() && a.substr(0, sz) == b) return "";
            for (int j = 0; j < sz; j++) {
                if (a[j] != b[j]) {
                    if (g[a[j]].insert(b[j]).second) indeg[b[j]]++;
                    break;
                }
            }
        }
        queue<char> q;
        for (auto& [c, d] : indeg) if (d == 0) q.push(c);
        string res;
        while (!q.empty()) {
            char u = q.front(); q.pop(); res += u;
            for (char v : g[u]) if (--indeg[v] == 0) q.push(v);
        }
        return (int)res.size() == (int)indeg.size() ? res : "";
    }
};
