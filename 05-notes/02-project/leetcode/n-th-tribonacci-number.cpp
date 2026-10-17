// LeetCode #1137 N-th Tribonacci Number (Easy)
// 滾動變數 DP, T(n) = T(n-1)+T(n-2)+T(n-3)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int tribonacci(int n) {
        if (n == 0) return 0;
        if (n <= 2) return 1;
        int a=0,b=1,c=1;
        for (int i = 3; i <= n; i++) { int d = a+b+c; a=b; b=c; c=d; }
        return c;
    }
};
