// LeetCode #136 Single Number (Easy)
// XOR 全部, 配對相消剩下單獨那個
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int x = 0;
        for (int v : nums) x ^= v;
        return x;
    }
};
