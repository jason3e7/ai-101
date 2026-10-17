// LeetCode #208 Implement Trie (Prefix Tree) (Med)
// 26 子節點指針陣列 + end 旗標
#include <bits/stdc++.h>
using namespace std;

class Trie {
    struct Node { Node* c[26] = {}; bool end = false; };
    Node* root;
public:
    Trie() { root = new Node(); }
    void insert(string word) {
        Node* n = root;
        for (char ch : word) { int i = ch - 'a'; if (!n->c[i]) n->c[i] = new Node(); n = n->c[i]; }
        n->end = true;
    }
    Node* find(const string& s) {
        Node* n = root;
        for (char ch : s) { int i = ch - 'a'; if (!n->c[i]) return nullptr; n = n->c[i]; }
        return n;
    }
    bool search(string word) { auto n = find(word); return n && n->end; }
    bool startsWith(string prefix) { return find(prefix) != nullptr; }
};
