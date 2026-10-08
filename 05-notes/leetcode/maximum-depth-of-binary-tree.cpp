// LeetCode #104 Maximum Depth of Binary Tree (Easy)
// 遞迴: 1 + max(左, 右)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
    }
};
