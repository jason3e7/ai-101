// LeetCode #1161 Maximum Level Sum of a Binary Tree (Med)
// BFS 分層累加, 記最大
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxLevelSum(TreeNode* root) {
        int bestLvl = 1, bestSum = INT_MIN, lvl = 0;
        queue<TreeNode*> q; q.push(root);
        while (!q.empty()) {
            lvl++;
            int sz = q.size(), sum = 0;
            while (sz--) {
                auto* n = q.front(); q.pop();
                sum += n->val;
                if (n->left) q.push(n->left);
                if (n->right) q.push(n->right);
            }
            if (sum > bestSum) { bestSum = sum; bestLvl = lvl; }
        }
        return bestLvl;
    }
};
