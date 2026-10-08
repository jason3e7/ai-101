// LeetCode #543 Diameter of Binary Tree (Easy)
// 遞迴求深度, 經過每點時更新 left+right 當候選直徑
#include <bits/stdc++.h>
using namespace std;

struct TreeNode { int val; TreeNode *left, *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };

class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int best = 0;
        function<int(TreeNode*)> depth = [&](TreeNode* n) -> int {
            if (!n) return 0;
            int l = depth(n->left), r = depth(n->right);
            best = max(best, l + r);
            return 1 + max(l, r);
        };
        depth(root);
        return best;
    }
};
