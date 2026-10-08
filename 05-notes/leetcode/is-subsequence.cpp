// LeetCode #392 Is Subsequence (Easy)
// 兩指針, i 走 s, 掃 t 時配到就 i++
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool isSubsequence(string s, string t) {
        int i = 0;
        for (char c : t) if (i < (int)s.size() && s[i] == c) i++;
        return i == (int)s.size();
    }
};
