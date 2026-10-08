// LeetCode #1372 Longest ZigZag Path in a Binary Tree (Med)
// DFS 傳 (lf, rt) 兩方向當前 zigzag 長度
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int best;
    void dfs(TreeNode* n, int lf, int rt) {
        if (!n) return;
        best = max({best, lf, rt});
        dfs(n->left, rt + 1, 0);
        dfs(n->right, 0, lf + 1);
    }
public:
    int longestZigZag(TreeNode* root) {
        best = 0;
        dfs(root, 0, 0);
        return best;
    }
};
