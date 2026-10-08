// LeetCode #70 Climbing Stairs (Easy)
// Fibonacci: dp[n] = dp[n-1] + dp[n-2]
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int climbStairs(int n) {
        int a = 1, b = 1;
        for (int i = 2; i <= n; i++) { int c = a + b; a = b; b = c; }
        return b;
    }
};
