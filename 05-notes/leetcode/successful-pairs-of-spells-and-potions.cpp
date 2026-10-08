// LeetCode #2300 Successful Pairs of Spells and Potions (Med)
// potions 排序後, 對每個 spell 二分找 ceil(success/spell) 的 lower_bound
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> successfulPairs(vector<int>& spells, vector<int>& potions, long long success) {
        sort(potions.begin(), potions.end());
        int m = potions.size();
        vector<int> res;
        for (int s : spells) {
            long long need = (success + s - 1) / s;
            auto it = lower_bound(potions.begin(), potions.end(), need);
            res.push_back(m - (int)(it - potions.begin()));
        }
        return res;
    }
};
