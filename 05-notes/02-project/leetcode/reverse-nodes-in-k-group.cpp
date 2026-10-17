// LeetCode #25 Reverse Nodes in k-Group (Hard)
// 逐 k 段反轉, 用 prevGroup 錨點縫接
#include <bits/stdc++.h>
using namespace std;

struct ListNode { int val; ListNode *next; ListNode(int x) : val(x), next(nullptr) {} };

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0); dummy.next = head;
        ListNode *prevGroup = &dummy;
        while (true) {
            ListNode *kth = prevGroup;
            for (int i = 0; i < k && kth; i++) kth = kth->next;
            if (!kth) break;
            ListNode *groupStart = prevGroup->next, *groupNext = kth->next;
            ListNode *prev = groupNext, *cur = groupStart;
            while (cur != groupNext) {
                ListNode *nx = cur->next;
                cur->next = prev;
                prev = cur;
                cur = nx;
            }
            prevGroup->next = prev;
            prevGroup = groupStart;
        }
        return dummy.next;
    }
};
