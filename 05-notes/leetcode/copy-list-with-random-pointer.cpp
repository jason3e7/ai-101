// LeetCode #138 Copy List with Random Pointer (Med)
// 兩遍 hashmap: 第一遍建新 node, 第二遍連 next / random
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int val;
    Node* next;
    Node* random;
    Node(int _val) : val(_val), next(nullptr), random(nullptr) {}
};

class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        unordered_map<Node*, Node*> mp;
        for (Node* p = head; p; p = p->next) mp[p] = new Node(p->val);
        for (Node* p = head; p; p = p->next) {
            mp[p]->next = mp[p->next];
            mp[p]->random = mp[p->random];
        }
        return mp[head];
    }
};
