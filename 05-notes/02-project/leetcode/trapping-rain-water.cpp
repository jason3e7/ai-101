// LeetCode #42 Trapping Rain Water (Hard)
// 雙指針, 以較矮一側的 max 當水位基準
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int trap(vector<int>& h) {
        int l = 0, r = h.size() - 1, lmax = 0, rmax = 0, res = 0;
        while (l < r) {
            if (h[l] < h[r]) {
                lmax = max(lmax, h[l]);
                res += lmax - h[l];
                l++;
            } else {
                rmax = max(rmax, h[r]);
                res += rmax - h[r];
                r--;
            }
        }
        return res;
    }
};
