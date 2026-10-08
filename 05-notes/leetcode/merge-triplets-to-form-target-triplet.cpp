// LeetCode #1899 Merge Triplets to Form Target Triplet (Med)
// 篩掉任一分量超過 target 的 triplet, 剩下取各分量 max 看能否配到 target
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool mergeTriplets(vector<vector<int>>& triplets, vector<int>& target) {
        int a = 0, b = 0, c = 0;
        for (auto& t : triplets) {
            if (t[0] > target[0] || t[1] > target[1] || t[2] > target[2]) continue;
            a = max(a, t[0]); b = max(b, t[1]); c = max(c, t[2]);
        }
        return a == target[0] && b == target[1] && c == target[2];
    }
};
