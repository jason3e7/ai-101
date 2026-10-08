// LeetCode #199 Binary Tree Right Side View (Med)
// BFS 分層, 每層最後一個是 right side view
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> rightSideView(TreeNode* root) {
        vector<int> res;
        if (!root) return res;
        queue<TreeNode*> q; q.push(root);
        while (!q.empty()) {
            int sz = q.size();
            for (int i = 0; i < sz; i++) {
                auto* n = q.front(); q.pop();
                if (i == sz - 1) res.push_back(n->val);
                if (n->left) q.push(n->left);
                if (n->right) q.push(n->right);
            }
        }
        return res;
    }
};
