// LeetCode #217 Contains Duplicate (Easy)
// hash set 插入, 失敗代表重複
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> s;
        for (int x : nums) { if (!s.insert(x).second) return true; }
        return false;
    }
};
