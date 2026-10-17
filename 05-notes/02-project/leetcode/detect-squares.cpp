// LeetCode #2013 Detect Squares (Med)
// 存點計數 + 原始清單, 查詢時掃對角線候選再乘另兩角出現次數
#include <bits/stdc++.h>
using namespace std;

class DetectSquares {
    unordered_map<long long, int> cnt;
    vector<pair<int,int>> pts;
    long long key(int x, int y) { return ((long long)x << 20) | y; }
public:
    DetectSquares() {}
    void add(vector<int> p) { cnt[key(p[0], p[1])]++; pts.push_back({p[0], p[1]}); }
    int count(vector<int> p) {
        int qx = p[0], qy = p[1], ans = 0;
        for (auto& [x, y] : pts) {
            if (x == qx || y == qy || abs(x - qx) != abs(y - qy)) continue;
            ans += cnt[key(x, qy)] * cnt[key(qx, y)];
        }
        return ans;
    }
};
