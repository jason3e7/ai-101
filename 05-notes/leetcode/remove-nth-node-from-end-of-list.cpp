// LeetCode #19 Remove Nth Node From End of List (Med)
// 一次走過: fast 先走 n 步, 之後 fast slow 一起走到 fast 到尾, slow 就在要刪的前一個
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode dummy(0, head);
        ListNode *fast = &dummy, *slow = &dummy;
        for (int i = 0; i < n; i++) fast = fast->next;
        while (fast->next) { fast = fast->next; slow = slow->next; }
        slow->next = slow->next->next;
        return dummy.next;
    }
};
