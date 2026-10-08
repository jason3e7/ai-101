// LeetCode #252 Meeting Rooms (Easy) — PREMIUM
// 排序後檢查相鄰時段是否重疊
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& iv) {
        sort(iv.begin(), iv.end());
        for (int i = 1; i < (int)iv.size(); i++)
            if (iv[i][0] < iv[i-1][1]) return false;
        return true;
    }
};
