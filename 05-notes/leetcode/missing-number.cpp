// LeetCode #268 Missing Number (Easy)
// XOR: n ^ 0 ^ 1 ^ ... ^ n-1 ^ nums[0..n-1], 重複的互相抵銷
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = nums.size(), x = n;
        for (int i = 0; i < n; i++) x ^= i ^ nums[i];
        return x;
    }
};
