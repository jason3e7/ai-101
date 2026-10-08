// LeetCode #7 Reverse Integer (Med)
// 逐位 * 10 + 餘數, 用 long 檢查 int overflow
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int reverse(int x) {
        long r = 0;
        while (x != 0) {
            r = r * 10 + x % 10;
            x /= 10;
        }
        if (r > INT_MAX || r < INT_MIN) return 0;
        return (int)r;
    }
};
