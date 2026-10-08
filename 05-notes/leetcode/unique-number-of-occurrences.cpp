// LeetCode #1207 Unique Number of Occurrences (Easy)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool uniqueOccurrences(vector<int>& arr) {
        unordered_map<int,int> cnt;
        for (int x : arr) cnt[x]++;
        unordered_set<int> s;
        for (auto& [_, c] : cnt) if (!s.insert(c).second) return false;
        return true;
    }
};
