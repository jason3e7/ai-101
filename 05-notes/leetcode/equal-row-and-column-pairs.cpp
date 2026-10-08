// LeetCode #2352 Equal Row and Column Pairs (Med)
// 把 rows 存成 map<vector<int>,int>, 掃 col 查 count
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int equalPairs(vector<vector<int>>& grid) {
        int n = grid.size();
        map<vector<int>, int> rows;
        for (auto& row : grid) rows[row]++;
        int cnt = 0;
        for (int c = 0; c < n; c++) {
            vector<int> col(n);
            for (int r = 0; r < n; r++) col[r] = grid[r][c];
            cnt += rows[col];
        }
        return cnt;
    }
};
