// LeetCode #647 Palindromic Substrings (Med)
// 中心擴散 O(n^2): 每個中心 (odd + even) 往兩邊走, 能走多遠就加多少
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int countSubstrings(string s) {
        int n = s.size(), cnt = 0;
        auto expand = [&](int l, int r) {
            while (l >= 0 && r < n && s[l] == s[r]) { l--; r++; cnt++; }
        };
        for (int i = 0; i < n; i++) { expand(i, i); expand(i, i + 1); }
        return cnt;
    }
};
