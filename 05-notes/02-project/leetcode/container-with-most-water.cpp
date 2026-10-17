// LeetCode #11 Container With Most Water (Med)
// 兩指針: 低的那邊往內收 (保留高的那邊才有機會變大面積)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxArea(vector<int>& h) {
        int l = 0, r = h.size() - 1, best = 0;
        while (l < r) {
            best = max(best, min(h[l], h[r]) * (r - l));
            if (h[l] < h[r]) l++; else r--;
        }
        return best;
    }
};
