// LeetCode #124 Binary Tree Maximum Path Sum (Hard)
// 遞迴回傳「以 n 為端點能提供給父的最大 gain」(>=0)
// 更新 best 時可以同時用左+根+右 (這條路徑的 top 是 n)
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int best;
    int gain(TreeNode* n) {
        if (!n) return 0;
        int l = max(0, gain(n->left));
        int r = max(0, gain(n->right));
        best = max(best, n->val + l + r);
        return n->val + max(l, r);
    }
public:
    int maxPathSum(TreeNode* root) {
        best = INT_MIN;
        gain(root);
        return best;
    }
};
