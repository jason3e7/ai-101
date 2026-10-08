// LeetCode #133 Clone Graph (Med)
// DFS + hashmap(old -> new) 避免無限遞迴
// Node 定義由 LeetCode 提供, 不需要自己寫
#include <bits/stdc++.h>
using namespace std;

// class Node { public: int val; vector<Node*> neighbors; };

class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        unordered_map<Node*, Node*> mp;
        function<Node*(Node*)> dfs = [&](Node* u) -> Node* {
            auto it = mp.find(u);
            if (it != mp.end()) return it->second;
            Node* copy = new Node(u->val);
            mp[u] = copy;
            for (Node* v : u->neighbors) copy->neighbors.push_back(dfs(v));
            return copy;
        };
        return dfs(node);
    }
};
