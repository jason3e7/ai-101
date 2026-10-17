// LeetCode #102 Binary Tree Level Order Traversal (Med)
// BFS 按層, 用 queue size 分層
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if (!root) return res;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            vector<int> lvl;
            while (sz--) {
                auto* n = q.front(); q.pop();
                lvl.push_back(n->val);
                if (n->left) q.push(n->left);
                if (n->right) q.push(n->right);
            }
            res.push_back(lvl);
        }
        return res;
    }
};
