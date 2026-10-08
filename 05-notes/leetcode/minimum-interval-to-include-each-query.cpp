// LeetCode #1851 Minimum Interval to Include Each Query (Hard)
// 離線 + 區間/查詢各自排序, min-heap 按區間長度, pop 掉右端已過的
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> minInterval(vector<vector<int>>& intervals, vector<int>& queries) {
        sort(intervals.begin(), intervals.end());
        int n = queries.size();
        vector<int> idx(n), res(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b){ return queries[a] < queries[b]; });
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        int i = 0, m = intervals.size();
        for (int q : idx) {
            int qv = queries[q];
            while (i < m && intervals[i][0] <= qv) {
                pq.push({intervals[i][1] - intervals[i][0] + 1, intervals[i][1]});
                i++;
            }
            while (!pq.empty() && pq.top().second < qv) pq.pop();
            res[q] = pq.empty() ? -1 : pq.top().first;
        }
        return res;
    }
};
