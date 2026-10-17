// LeetCode #236 Lowest Common Ancestor of a Binary Tree (Med)
// 不同於 BST 版 — 一般樹: 遞迴找 p / q, 左右都有就是 LCA
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* l = lowestCommonAncestor(root->left, p, q);
        TreeNode* r = lowestCommonAncestor(root->right, p, q);
        if (l && r) return root;
        return l ? l : r;
    }
};
