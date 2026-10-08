// LeetCode #2 Add Two Numbers (Med)
// 鏈結串列逐位相加帶 carry, dummy head 建結果
#include <bits/stdc++.h>
using namespace std;

struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0), *cur = &dummy;
        int carry = 0;
        while (l1 || l2 || carry) {
            int s = carry;
            if (l1) { s += l1->val; l1 = l1->next; }
            if (l2) { s += l2->val; l2 = l2->next; }
            carry = s / 10;
            cur->next = new ListNode(s % 10);
            cur = cur->next;
        }
        return dummy.next;
    }
};
