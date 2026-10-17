// LeetCode #371 Sum of Two Integers (Med)
// 不用 + : XOR 是不帶 carry 的加, (a & b) << 1 是 carry
// 反覆直到 carry = 0. 用 unsigned 避免 UB
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int getSum(int a, int b) {
        while (b) {
            unsigned carry = (unsigned)(a & b) << 1;
            a = a ^ b;
            b = carry;
        }
        return a;
    }
};
