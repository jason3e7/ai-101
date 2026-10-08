// LeetCode #1448 Count Good Nodes in Binary Tree (Med)
// DFS 傳 path 的 max, node >= max 就算 good
#include <bits/stdc++.h>
using namespace std;

class Solution {
    int cnt;
    void dfs(TreeNode* n, int mx) {
        if (!n) return;
        if (n->val >= mx) { cnt++; mx = n->val; }
        dfs(n->left, mx); dfs(n->right, mx);
    }
public:
    int goodNodes(TreeNode* root) {
        cnt = 0;
        dfs(root, INT_MIN);
        return cnt;
    }
};
