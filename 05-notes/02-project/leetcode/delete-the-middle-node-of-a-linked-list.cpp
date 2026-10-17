// LeetCode #2095 Delete the Middle Node of a Linked List (Med)
// 龜兔找中點 + prev 保留前一個
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    ListNode* deleteMiddle(ListNode* head) {
        if (!head->next) return nullptr;
        ListNode *slow = head, *fast = head, *prev = nullptr;
        while (fast && fast->next) { prev = slow; slow = slow->next; fast = fast->next->next; }
        prev->next = slow->next;
        return head;
    }
};
