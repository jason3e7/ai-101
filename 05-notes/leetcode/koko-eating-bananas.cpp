// LeetCode #875 Koko Eating Bananas (Med)
// 對速度 k 二分, 每個 pile ceil(p/k) 加總與 h 比較
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int lo = 1, hi = *max_element(piles.begin(), piles.end());
        auto hours = [&](long long k) {
            long long t = 0;
            for (int p : piles) t += (p + k - 1) / k;
            return t;
        };
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (hours(mid) <= h) hi = mid;
            else lo = mid + 1;
        }
        return lo;
    }
};
