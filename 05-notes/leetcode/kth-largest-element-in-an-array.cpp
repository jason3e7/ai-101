// LeetCode #215 Kth Largest Element in an Array (Med)
// 最小堆保留 k 個, 堆頂就是答案
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        priority_queue<int, vector<int>, greater<int>> pq;
        for (int x : nums) { pq.push(x); if ((int)pq.size() > k) pq.pop(); }
        return pq.top();
    }
};
