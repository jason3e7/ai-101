// LeetCode #23 Merge k Sorted Lists (Hard)
// 最小堆 pq: 每次挑最小的 head, pop 接上後把它的 next 入堆
// 時間 O(N log k), N = 總節點數, k = list 數
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b){ return a->val > b->val; };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp)> pq(cmp);
        for (auto* h : lists) if (h) pq.push(h);
        ListNode dummy, *tail = &dummy;
        while (!pq.empty()) {
            auto* n = pq.top(); pq.pop();
            tail->next = n; tail = n;
            if (n->next) pq.push(n->next);
        }
        tail->next = nullptr;
        return dummy.next;
    }
};
