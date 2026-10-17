// LeetCode #295 Find Median from Data Stream (Hard)
// 兩個 heap: 下半 max-heap + 上半 min-heap, 維持 size diff <= 1
// add O(log n), find O(1)
#include <bits/stdc++.h>
using namespace std;

class MedianFinder {
    priority_queue<int> lo;
    priority_queue<int, vector<int>, greater<int>> hi;
public:
    MedianFinder() {}
    void addNum(int num) {
        lo.push(num);
        hi.push(lo.top()); lo.pop();
        if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
    }
    double findMedian() {
        if (lo.size() > hi.size()) return lo.top();
        return (lo.top() + hi.top()) / 2.0;
    }
};
