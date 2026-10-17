// LeetCode #297 Serialize and Deserialize Binary Tree (Hard)
// Preorder + '#' 代表 null, 遞迴還原
#include <bits/stdc++.h>
using namespace std;

class Codec {
public:
    string serialize(TreeNode* root) {
        string s;
        function<void(TreeNode*)> dfs = [&](TreeNode* n) {
            if (!n) { s += "# "; return; }
            s += to_string(n->val) + " ";
            dfs(n->left); dfs(n->right);
        };
        dfs(root);
        return s;
    }
    TreeNode* deserialize(string data) {
        stringstream ss(data);
        function<TreeNode*()> build = [&]() -> TreeNode* {
            string tok; if (!(ss >> tok)) return nullptr;
            if (tok == "#") return nullptr;
            TreeNode* n = new TreeNode(stoi(tok));
            n->left = build(); n->right = build();
            return n;
        };
        return build();
    }
};
