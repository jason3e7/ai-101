// LeetCode #110 Balanced Binary Tree (Easy)
// 遞迴求深度, 經過每點時檢查左右差 ≤ 1
#include <bits/stdc++.h>
using namespace std;

struct TreeNode { int val; TreeNode *left, *right; TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} };

class Solution {
public:
    bool isBalanced(TreeNode* root) {
        bool ok = true;
        function<int(TreeNode*)> depth = [&](TreeNode* n) -> int {
            if (!n) return 0;
            int l = depth(n->left), r = depth(n->right);
            if (abs(l - r) > 1) ok = false;
            return 1 + max(l, r);
        };
        depth(root);
        return ok;
    }
};
