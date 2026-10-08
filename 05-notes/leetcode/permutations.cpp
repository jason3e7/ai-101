// LeetCode #46 Permutations (Med)
// STL next_permutation 直接列舉, 需先排序
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        do { res.push_back(nums); } while (next_permutation(nums.begin(), nums.end()));
        return res;
    }
};
