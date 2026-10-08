// LeetCode #143 Reorder List (Med)
// 3 步: 找中點 (龜兔) → 反轉後半 → 兩段交織合併
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    void reorderList(ListNode* head) {
        if (!head || !head->next) return;
        ListNode *slow = head, *fast = head;
        while (fast->next && fast->next->next) { slow = slow->next; fast = fast->next->next; }
        ListNode *prev = nullptr, *cur = slow->next;
        slow->next = nullptr;
        while (cur) { ListNode* nx = cur->next; cur->next = prev; prev = cur; cur = nx; }
        ListNode *a = head, *b = prev;
        while (b) {
            ListNode *an = a->next, *bn = b->next;
            a->next = b; b->next = an;
            a = an; b = bn;
        }
    }
};
