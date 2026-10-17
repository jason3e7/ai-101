// LeetCode #253 Meeting Rooms II (Med) — PREMIUM
// 最小堆存目前所有會議的 end time, 新會議開始前先 pop 已結束的, 剩餘堆 size 就是需要房間數
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& iv) {
        sort(iv.begin(), iv.end());
        priority_queue<int, vector<int>, greater<int>> pq;
        for (auto& m : iv) {
            if (!pq.empty() && pq.top() <= m[0]) pq.pop();
            pq.push(m[1]);
        }
        return pq.size();
    }
};
