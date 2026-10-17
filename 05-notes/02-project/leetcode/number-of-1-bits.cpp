// LeetCode #191 Number of 1 Bits (Easy)
// 直接用 GCC builtin __builtin_popcount
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int hammingWeight(int n) {
        return __builtin_popcount((unsigned)n);
    }
};
