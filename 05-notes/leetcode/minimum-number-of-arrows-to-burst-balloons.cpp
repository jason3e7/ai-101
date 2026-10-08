// LeetCode #452 Minimum Number of Arrows to Burst Balloons (Med)
// 按右端排序, 貪心: 不被當前箭覆蓋就射新一箭
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findMinArrowShots(vector<vector<int>>& points) {
        sort(points.begin(), points.end(), [](auto& a, auto& b){ return a[1] < b[1]; });
        int arrows = 0;
        long long last = LLONG_MIN;
        for (auto& p : points) {
            if (arrows == 0 || p[0] > last) { arrows++; last = p[1]; }
        }
        return arrows;
    }
};
