// LeetCode #73 Set Matrix Zeroes (Med)
// O(1) 空間: 用第 0 列/行當 flag, 兩個額外 bool 處理第 0 列/行本身
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void setZeroes(vector<vector<int>>& m) {
        int R = m.size(), C = m[0].size();
        bool row0 = false, col0 = false;
        for (int j = 0; j < C; j++) if (m[0][j] == 0) row0 = true;
        for (int i = 0; i < R; i++) if (m[i][0] == 0) col0 = true;
        for (int i = 1; i < R; i++)
            for (int j = 1; j < C; j++)
                if (m[i][j] == 0) { m[i][0] = m[0][j] = 0; }
        for (int i = 1; i < R; i++)
            for (int j = 1; j < C; j++)
                if (m[i][0] == 0 || m[0][j] == 0) m[i][j] = 0;
        if (row0) for (int j = 0; j < C; j++) m[0][j] = 0;
        if (col0) for (int i = 0; i < R; i++) m[i][0] = 0;
    }
};
