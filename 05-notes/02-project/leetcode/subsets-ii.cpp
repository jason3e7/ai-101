// LeetCode #90 Subsets II (Med)
// 排序後 backtracking, 同層跳過重複
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> res;
        vector<int> cur;
        function<void(int)> dfs = [&](int i) {
            res.push_back(cur);
            for (int j = i; j < (int)nums.size(); j++) {
                if (j > i && nums[j] == nums[j-1]) continue;
                cur.push_back(nums[j]);
                dfs(j+1);
                cur.pop_back();
            }
        };
        dfs(0);
        return res;
    }
};
