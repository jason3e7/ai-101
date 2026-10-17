// LeetCode #1456 Maximum Number of Vowels in a Substring of Given Length (Med)
// 固定窗口 sliding, 進 window +1 出 window -1
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxVowels(string s, int k) {
        auto isV = [](char c) { return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'; };
        int cnt = 0;
        for (int i = 0; i < k; i++) if (isV(s[i])) cnt++;
        int best = cnt;
        for (int i = k; i < (int)s.size(); i++) {
            if (isV(s[i])) cnt++;
            if (isV(s[i-k])) cnt--;
            best = max(best, cnt);
        }
        return best;
    }
};
