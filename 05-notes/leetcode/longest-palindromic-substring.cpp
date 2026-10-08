// LeetCode #5 Longest Palindromic Substring (Med)
// 中心擴散 O(n^2): 每個位置當 odd 中心 + even 中心兩次 expand
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string longestPalindrome(string s) {
        int n = s.size(), start = 0, best = 1;
        auto expand = [&](int l, int r) {
            while (l >= 0 && r < n && s[l] == s[r]) { l--; r++; }
            int len = r - l - 1;
            if (len > best) { best = len; start = l + 1; }
        };
        for (int i = 0; i < n; i++) {
            expand(i, i);
            expand(i, i + 1);
        }
        return s.substr(start, best);
    }
};
