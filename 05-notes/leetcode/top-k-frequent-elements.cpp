// LeetCode #347 Top K Frequent Elements (Med)
// Bucket sort O(n): 以頻率當 bucket index, 從高到低掃取前 k 個
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int,int> cnt;
        for (int x : nums) cnt[x]++;
        int n = nums.size();
        vector<vector<int>> bucket(n + 1);
        for (auto& [v, c] : cnt) bucket[c].push_back(v);
        vector<int> res;
        for (int i = n; i >= 0 && (int)res.size() < k; i--)
            for (int v : bucket[i]) { res.push_back(v); if ((int)res.size() == k) break; }
        return res;
    }
};
