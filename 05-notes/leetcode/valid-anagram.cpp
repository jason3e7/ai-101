// LeetCode #242 Valid Anagram (Easy)
// 計數 + 反向抵銷, 任何為負代表 t 有多的字母
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isAnagram(string s, string t) {
        if (s.size() != t.size()) return false;
        int cnt[26] = {0};
        for (char c : s) cnt[c - 'a']++;
        for (char c : t) if (--cnt[c - 'a'] < 0) return false;
        return true;
    }
};
