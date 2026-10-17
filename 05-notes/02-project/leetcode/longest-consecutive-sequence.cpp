// LeetCode #128 Longest Consecutive Sequence (Med)
// O(n): 把全部放進 unordered_set, 只從「序列的起點」(n-1 不在 set 裡) 開始延伸計算
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s(nums.begin(), nums.end());
        int best = 0;
        for (int n : s) {
            if (!s.count(n - 1)) {
                int cur = n, len = 1;
                while (s.count(cur + 1)) { cur++; len++; }
                best = max(best, len);
            }
        }
        return best;
    }
};
