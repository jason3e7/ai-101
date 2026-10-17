// LeetCode #226 Invert Binary Tree (Easy)
// 遞迴 swap 左右子樹
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        swap(root->left, root->right);
        invertTree(root->left); invertTree(root->right);
        return root;
    }
};
