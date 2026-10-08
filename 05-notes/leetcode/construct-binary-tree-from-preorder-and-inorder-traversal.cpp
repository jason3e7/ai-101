// LeetCode #105 Construct Binary Tree from Preorder and Inorder Traversal (Med)
// Preorder 第一個是 root, inorder 用 map 查 root 位置, 左右切兩段遞迴
#include <bits/stdc++.h>
using namespace std;

class Solution {
    unordered_map<int,int> idx;
    int pi = 0;
    vector<int> P, I;
    TreeNode* build(int l, int r) {
        if (l > r) return nullptr;
        int v = P[pi++];
        TreeNode* n = new TreeNode(v);
        int m = idx[v];
        n->left = build(l, m - 1);
        n->right = build(m + 1, r);
        return n;
    }
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        P = preorder; I = inorder;
        for (int i = 0; i < (int)I.size(); i++) idx[I[i]] = i;
        return build(0, I.size() - 1);
    }
};
