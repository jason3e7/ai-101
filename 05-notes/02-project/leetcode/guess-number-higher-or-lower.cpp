// LeetCode #374 Guess Number Higher or Lower (Easy)
// 標準二分搜, 用 guess() API 判斷方向
#include <bits/stdc++.h>
using namespace std;

int guess(int num); // 平台提供

class Solution {
public:
    int guessNumber(int n) {
        int lo = 1, hi = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            int r = guess(mid);
            if (r == 0) return mid;
            if (r < 0) hi = mid - 1;
            else lo = mid + 1;
        }
        return -1;
    }
};
