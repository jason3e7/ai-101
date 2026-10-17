// LeetCode #1679 Max Number of K-Sum Pairs (Med)
// Two Sum 變種, 用 hash count 配對
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxOperations(vector<int>& nums, int k) {
        unordered_map<int,int> cnt;
        int ops = 0;
        for (int x : nums) {
            int need = k - x;
            auto it = cnt.find(need);
            if (it != cnt.end() && it->second > 0) { it->second--; ops++; }
            else cnt[x]++;
        }
        return ops;
    }
};
