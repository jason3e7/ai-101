// LeetCode #450 Delete Node in a BST (Med)
// 找到 key 後: 一邊空 -> 回另一邊; 兩邊都在 -> 用 inorder successor 替代, 再刪 successor
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;
        if (key < root->val) root->left = deleteNode(root->left, key);
        else if (key > root->val) root->right = deleteNode(root->right, key);
        else {
            if (!root->left) return root->right;
            if (!root->right) return root->left;
            TreeNode* s = root->right;
            while (s->left) s = s->left;
            root->val = s->val;
            root->right = deleteNode(root->right, s->val);
        }
        return root;
    }
};
