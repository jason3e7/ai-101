// LeetCode #703 Kth Largest Element in a Stream (Easy)
// 最小堆保留 k 個最大, 堆頂就是第 k 大
#include <bits/stdc++.h>
using namespace std;

class KthLargest {
    priority_queue<int, vector<int>, greater<int>> pq;
    int k;
public:
    KthLargest(int K, vector<int>& nums) : k(K) {
        for (int x : nums) { pq.push(x); if ((int)pq.size() > k) pq.pop(); }
    }
    int add(int val) {
        pq.push(val);
        if ((int)pq.size() > k) pq.pop();
        return pq.top();
    }
};
