// LeetCode #235 Lowest Common Ancestor of a Binary Search Tree (Med)
// BST 性質: 兩節點值分別落在 root 的兩側, root 就是 LCA
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        while (root) {
            if (p->val < root->val && q->val < root->val) root = root->left;
            else if (p->val > root->val && q->val > root->val) root = root->right;
            else return root;
        }
        return nullptr;
    }
};
