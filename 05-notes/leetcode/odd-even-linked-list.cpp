// LeetCode #328 Odd Even Linked List (Med)
// 兩條鏈 (奇/偶) 交織重接, 奇尾接偶頭
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if (!head) return nullptr;
        ListNode *o = head, *e = head->next, *eHead = e;
        while (e && e->next) {
            o->next = e->next; o = o->next;
            e->next = o->next; e = e->next;
        }
        o->next = eHead;
        return head;
    }
};
