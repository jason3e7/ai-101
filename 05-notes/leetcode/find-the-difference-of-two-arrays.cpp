// LeetCode #2215 Find the Difference of Two Arrays (Easy)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<int>> findDifference(vector<int>& a, vector<int>& b) {
        unordered_set<int> sa(a.begin(), a.end()), sb(b.begin(), b.end());
        vector<int> r1, r2;
        for (int x : sa) if (!sb.count(x)) r1.push_back(x);
        for (int x : sb) if (!sa.count(x)) r2.push_back(x);
        return {r1, r2};
    }
};
