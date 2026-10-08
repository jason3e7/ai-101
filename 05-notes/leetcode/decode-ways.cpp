// LeetCode #91 Decode Ways (Med)
// DP 兩狀態 (prev, cur): 單字元 (非 0) + 雙字元 (10-26) 各加
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int numDecodings(string s) {
        int n = s.size();
        if (s[0] == '0') return 0;
        int prev = 1, cur = 1;
        for (int i = 1; i < n; i++) {
            int nx = 0;
            if (s[i] != '0') nx += cur;
            int two = (s[i-1] - '0') * 10 + (s[i] - '0');
            if (two >= 10 && two <= 26) nx += prev;
            prev = cur; cur = nx;
        }
        return cur;
    }
};
