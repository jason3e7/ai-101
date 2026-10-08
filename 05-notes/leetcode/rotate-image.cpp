// LeetCode #48 Rotate Image (Med)
// 轉置 + 每列反轉 = 順時針 90 度
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void rotate(vector<vector<int>>& m) {
        int n = m.size();
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++) swap(m[i][j], m[j][i]);
        for (auto& row : m) reverse(row.begin(), row.end());
    }
};
