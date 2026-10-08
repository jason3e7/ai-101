// LeetCode #1657 Determine if Two Strings Are Close (Med)
// 兩條件: (1) 字母集合相同 (2) 計數的 multiset 相同
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool closeStrings(string a, string b) {
        if (a.size() != b.size()) return false;
        int ca[26] = {0}, cb[26] = {0};
        for (char c : a) ca[c-'a']++;
        for (char c : b) cb[c-'a']++;
        for (int i = 0; i < 26; i++) if ((ca[i] > 0) != (cb[i] > 0)) return false;
        vector<int> va(ca, ca+26), vb(cb, cb+26);
        sort(va.begin(), va.end()); sort(vb.begin(), vb.end());
        return va == vb;
    }
};
