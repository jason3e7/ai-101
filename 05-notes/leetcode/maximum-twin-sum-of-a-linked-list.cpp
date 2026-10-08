// LeetCode #2130 Maximum Twin Sum of a Linked List (Med)
// 龜兔找中點 → 反轉後半 → 兩段同時走取 max(a+b)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int pairSum(ListNode* head) {
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) { slow = slow->next; fast = fast->next->next; }
        ListNode *prev = nullptr, *cur = slow;
        while (cur) { ListNode* nx = cur->next; cur->next = prev; prev = cur; cur = nx; }
        int best = 0;
        ListNode *a = head, *b = prev;
        while (b) { best = max(best, a->val + b->val); a = a->next; b = b->next; }
        return best;
    }
};
