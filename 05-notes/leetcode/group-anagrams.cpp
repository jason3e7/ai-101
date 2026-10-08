// LeetCode #49 Group Anagrams (Med)
// 排序後字串當 key, 同 key 的 anagram 收一起
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        for (auto& s : strs) {
            string k = s;
            sort(k.begin(), k.end());
            mp[k].push_back(s);
        }
        vector<vector<string>> res;
        for (auto& [_, v] : mp) res.push_back(move(v));
        return res;
    }
};
