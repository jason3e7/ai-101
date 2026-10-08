// LeetCode #212 Word Search II (Hard)
// Trie + DFS 剪枝: 把所有單字塞 Trie, DFS board 時只走 Trie 存在的路徑
#include <bits/stdc++.h>
using namespace std;

class Solution {
    struct Node { Node* c[26] = {}; string word; };
public:
    vector<string> findWords(vector<vector<char>>& b, vector<string>& words) {
        Node* root = new Node();
        for (auto& w : words) {
            Node* n = root;
            for (char ch : w) { int i = ch - 'a'; if (!n->c[i]) n->c[i] = new Node(); n = n->c[i]; }
            n->word = w;
        }
        int m = b.size(), n = b[0].size();
        vector<string> res;
        function<void(int,int,Node*)> dfs = [&](int r, int c, Node* u) {
            if (r < 0 || r >= m || c < 0 || c >= n) return;
            char ch = b[r][c];
            if (ch == '#') return;
            Node* nx = u->c[ch - 'a'];
            if (!nx) return;
            if (!nx->word.empty()) { res.push_back(nx->word); nx->word.clear(); }
            b[r][c] = '#';
            dfs(r+1,c,nx); dfs(r-1,c,nx); dfs(r,c+1,nx); dfs(r,c-1,nx);
            b[r][c] = ch;
        };
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) dfs(i, j, root);
        return res;
    }
};
