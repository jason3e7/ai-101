// LeetCode #100 Same Tree (Easy)
// 遞迴: 兩個 null 相等, 否則 val 相等且左右子樹都相等
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isSameTree(TreeNode* a, TreeNode* b) {
        if (!a || !b) return a == b;
        return a->val == b->val && isSameTree(a->left, b->left) && isSameTree(a->right, b->right);
    }
};
