// LeetCode #643 Maximum Average Subarray I (Easy)
// 固定長度 sliding window
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        double sum = 0;
        for (int i = 0; i < k; i++) sum += nums[i];
        double best = sum;
        for (int i = k; i < (int)nums.size(); i++) {
            sum += nums[i] - nums[i-k];
            best = max(best, sum);
        }
        return best / k;
    }
};
