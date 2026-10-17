// LeetCode #74 Search a 2D Matrix (Med)
// 把 2D 當 1D 排序陣列二分, mid 用 /cols, %cols 還原座標
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool searchMatrix(vector<vector<int>>& m, int target) {
        int rows = m.size(), cols = m[0].size();
        int lo = 0, hi = rows * cols - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int v = m[mid / cols][mid % cols];
            if (v == target) return true;
            if (v < target) lo = mid + 1; else hi = mid - 1;
        }
        return false;
    }
};
