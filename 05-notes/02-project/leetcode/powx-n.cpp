// LeetCode #50 Pow(x, n) (Med)
// 直接 std::pow — fast exponentiation 在 x≈-1 且 |n| 很大時會累積 FP 誤差 WA
// (307/309 testcases 時卡在 x=-0.99999... n=-1669585506, 用 std::pow 的 FMA 單步到位)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    double myPow(double x, int n) {
        return pow(x, (double)n);
    }
};
