// LeetCode #230 Kth Smallest Element in a BST (Med)
// 中序迭代, 用 stack 走左→根→右, 第 k 個 pop 就是答案
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        stack<TreeNode*> st;
        TreeNode* cur = root;
        while (cur || !st.empty()) {
            while (cur) { st.push(cur); cur = cur->left; }
            cur = st.top(); st.pop();
            if (--k == 0) return cur->val;
            cur = cur->right;
        }
        return -1;
    }
};
