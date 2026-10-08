// LeetCode #190 Reverse Bits (Easy)
// 32 iterations, 每次把 n LSB 推到 r 最前面
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        uint32_t r = 0;
        for (int i = 0; i < 32; i++) { r = (r << 1) | (n & 1); n >>= 1; }
        return r;
    }
};
