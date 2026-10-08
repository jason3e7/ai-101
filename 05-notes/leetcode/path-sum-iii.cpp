// LeetCode #437 Path Sum III (Med)
// Prefix sum + hashmap: dfs 時維護 sum, 找 sum-target 出現過幾次
#include <bits/stdc++.h>
using namespace std;

class Solution {
    unordered_map<long,int> cnt;
    int target;
    int res;
    void dfs(TreeNode* n, long sum) {
        if (!n) return;
        sum += n->val;
        auto it = cnt.find(sum - target);
        if (it != cnt.end()) res += it->second;
        cnt[sum]++;
        dfs(n->left, sum); dfs(n->right, sum);
        cnt[sum]--;
    }
public:
    int pathSum(TreeNode* root, int targetSum) {
        target = targetSum; res = 0;
        cnt[0] = 1;
        dfs(root, 0);
        return res;
    }
};
