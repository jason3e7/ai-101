// LeetCode #287 Find the Duplicate Number (Med)
// Floyd 龜兔跑把 nums 當隱式鏈結串列, 重複值就是迴圈入口
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int findDuplicate(vector<int>& nums) {
        int slow = nums[0], fast = nums[0];
        do { slow = nums[slow]; fast = nums[nums[fast]]; } while (slow != fast);
        slow = nums[0];
        while (slow != fast) { slow = nums[slow]; fast = nums[fast]; }
        return slow;
    }
};
