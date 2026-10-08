// LeetCode #98 Validate Binary Search Tree (Med)
// 遞迴帶上下界 (long 防 INT 邊界)
#include <bits/stdc++.h>
using namespace std;

class Solution {
    bool valid(TreeNode* n, long lo, long hi) {
        if (!n) return true;
        if (n->val <= lo || n->val >= hi) return false;
        return valid(n->left, lo, n->val) && valid(n->right, n->val, hi);
    }
public:
    bool isValidBST(TreeNode* root) {
        return valid(root, LONG_MIN, LONG_MAX);
    }
};
