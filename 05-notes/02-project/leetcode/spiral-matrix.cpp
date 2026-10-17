// LeetCode #54 Spiral Matrix (Med)
// 四邊界 top/bot/lf/rt, 一圈一圈收
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& m) {
        int top = 0, bot = m.size() - 1, lf = 0, rt = m[0].size() - 1;
        vector<int> res;
        while (top <= bot && lf <= rt) {
            for (int j = lf; j <= rt; j++) res.push_back(m[top][j]);
            top++;
            for (int i = top; i <= bot; i++) res.push_back(m[i][rt]);
            rt--;
            if (top <= bot) { for (int j = rt; j >= lf; j--) res.push_back(m[bot][j]); bot--; }
            if (lf <= rt) { for (int i = bot; i >= top; i--) res.push_back(m[i][lf]); lf++; }
        }
        return res;
    }
};
