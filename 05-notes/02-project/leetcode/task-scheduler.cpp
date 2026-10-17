// LeetCode #621 Task Scheduler (Med)
// 公式解: 最多次任務當骨架, 答案 = max(總任務數, (max-1)*(n+1) + 同為 max 的種類數)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        int cnt[26] = {0};
        for (char c : tasks) cnt[c-'A']++;
        int maxCnt = *max_element(cnt, cnt+26);
        int tie = count(cnt, cnt+26, maxCnt);
        return max((int)tasks.size(), (maxCnt - 1) * (n + 1) + tie);
    }
};
