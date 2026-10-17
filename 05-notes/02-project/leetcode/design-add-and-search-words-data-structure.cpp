// LeetCode #211 Design Add and Search Words Data Structure (Med)
// Trie + '.' 時 DFS 所有子節點
#include <bits/stdc++.h>
using namespace std;

class WordDictionary {
    struct Node { Node* c[26] = {}; bool end = false; };
    Node* root;
public:
    WordDictionary() { root = new Node(); }
    void addWord(string word) {
        Node* n = root;
        for (char ch : word) { int i = ch - 'a'; if (!n->c[i]) n->c[i] = new Node(); n = n->c[i]; }
        n->end = true;
    }
    bool search(string word) {
        function<bool(Node*,int)> dfs = [&](Node* n, int k) -> bool {
            if (!n) return false;
            if (k == (int)word.size()) return n->end;
            char ch = word[k];
            if (ch == '.') {
                for (int i = 0; i < 26; i++) if (dfs(n->c[i], k + 1)) return true;
                return false;
            }
            return dfs(n->c[ch - 'a'], k + 1);
        };
        return dfs(root, 0);
    }
};
