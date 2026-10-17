// LeetCode #700 Search in a Binary Search Tree (Easy)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        while (root && root->val != val) root = val < root->val ? root->left : root->right;
        return root;
    }
};
