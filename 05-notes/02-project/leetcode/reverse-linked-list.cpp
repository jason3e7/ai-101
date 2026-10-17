// LeetCode #206 Reverse Linked List (Easy)
// 經典三指針反轉
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *prev = nullptr, *cur = head;
        while (cur) { ListNode* nx = cur->next; cur->next = prev; prev = cur; cur = nx; }
        return prev;
    }
};
