// LeetCode #334 Increasing Triplet Subsequence (Med)
// 維護 a (最小), b (次小且 > a), 第三個遇到 > b 就是答案
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int a = INT_MAX, b = INT_MAX;
        for (int x : nums) {
            if (x <= a) a = x;
            else if (x <= b) b = x;
            else return true;
        }
        return false;
    }
};
