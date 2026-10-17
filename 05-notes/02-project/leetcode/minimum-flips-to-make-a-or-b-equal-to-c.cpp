// LeetCode #1318 Minimum Flips to Make a OR b Equal to c (Med)
// 逐 bit 檢查: c 位為 0 要翻掉 a,b 的 1; c 位為 1 需至少一邊是 1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minFlips(int a, int b, int c) {
        int flips = 0;
        for (int i = 0; i < 31; i++) {
            int ba = (a>>i)&1, bb = (b>>i)&1, bc = (c>>i)&1;
            if (bc == 0) flips += ba + bb;
            else if ((ba | bb) == 0) flips += 1;
        }
        return flips;
    }
};
