// LeetCode #567 Permutation in String (Med)
// 固定窗口 sliding, 比對兩個 26 大小 counter
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if (s1.size() > s2.size()) return false;
        int c1[26] = {0}, c2[26] = {0};
        int k = s1.size();
        for (int i = 0; i < k; i++) { c1[s1[i]-'a']++; c2[s2[i]-'a']++; }
        auto eq = [&]() { for (int i = 0; i < 26; i++) if (c1[i] != c2[i]) return false; return true; };
        if (eq()) return true;
        for (int i = k; i < (int)s2.size(); i++) {
            c2[s2[i]-'a']++;
            c2[s2[i-k]-'a']--;
            if (eq()) return true;
        }
        return false;
    }
};
