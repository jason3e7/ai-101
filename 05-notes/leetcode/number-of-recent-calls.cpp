// LeetCode #933 Number of Recent Calls (Easy)
// Queue: 推進新 t, 彈出 < t-3000 的
#include <bits/stdc++.h>
using namespace std;

class RecentCounter {
    queue<int> q;
public:
    RecentCounter() {}
    int ping(int t) {
        q.push(t);
        while (q.front() < t - 3000) q.pop();
        return q.size();
    }
};
