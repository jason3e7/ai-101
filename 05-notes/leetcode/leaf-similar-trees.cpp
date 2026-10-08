// LeetCode #872 Leaf-Similar Trees (Easy)
// 收 leaves (preorder) 比較
#include <bits/stdc++.h>
using namespace std;

class Solution {
    void leaves(TreeNode* n, vector<int>& v) {
        if (!n) return;
        if (!n->left && !n->right) v.push_back(n->val);
        leaves(n->left, v); leaves(n->right, v);
    }
public:
    bool leafSimilar(TreeNode* r1, TreeNode* r2) {
        vector<int> a, b;
        leaves(r1, a); leaves(r2, b);
        return a == b;
    }
};
