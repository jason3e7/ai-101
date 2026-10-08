// LeetCode #572 Subtree of Another Tree (Easy)
// isSame helper + 遍歷 root 比對
#include <bits/stdc++.h>
using namespace std;

class Solution {
    bool same(TreeNode* a, TreeNode* b) {
        if (!a || !b) return a == b;
        return a->val == b->val && same(a->left, b->left) && same(a->right, b->right);
    }
public:
    bool isSubtree(TreeNode* root, TreeNode* sub) {
        if (!root) return false;
        if (same(root, sub)) return true;
        return isSubtree(root->left, sub) || isSubtree(root->right, sub);
    }
};
