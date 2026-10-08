// LeetCode #76 Minimum Window Substring (Hard)
// Sliding window + need/have counters + formed/required 計數器
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string minWindow(string s, string t) {
        int need[128] = {0}, have[128] = {0};
        int required = 0;
        for (char c : t) { if (need[(int)c]++ == 0) required++; }
        int formed = 0, l = 0, bestL = 0, bestLen = INT_MAX;
        for (int r = 0; r < (int)s.size(); r++) {
            char c = s[r];
            if (++have[(int)c] == need[(int)c]) formed++;
            while (formed == required) {
                if (r - l + 1 < bestLen) { bestLen = r - l + 1; bestL = l; }
                char d = s[l++];
                if (have[(int)d]-- == need[(int)d]) formed--;
            }
        }
        return bestLen == INT_MAX ? "" : s.substr(bestL, bestLen);
    }
};
