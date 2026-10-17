// LeetCode #2542 Maximum Subsequence Score (Med)
// 依 n2 降序, 掃過時用 min-heap 保留 k 個最大 n1, 當前 n2 當 min
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    long long maxScore(vector<int>& nums1, vector<int>& nums2, int k) {
        int n = nums1.size();
        vector<int> idx(n);
        iota(idx.begin(), idx.end(), 0);
        sort(idx.begin(), idx.end(), [&](int a, int b){ return nums2[a] > nums2[b]; });
        priority_queue<int, vector<int>, greater<int>> pq;
        long long sum = 0, best = 0;
        for (int i = 0; i < n; i++) {
            int id = idx[i];
            sum += nums1[id];
            pq.push(nums1[id]);
            if ((int)pq.size() > k) { sum -= pq.top(); pq.pop(); }
            if ((int)pq.size() == k) best = max(best, sum * (long long)nums2[id]);
        }
        return best;
    }
};
