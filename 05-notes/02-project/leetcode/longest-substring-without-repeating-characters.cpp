// LeetCode #3 Longest Substring Without Repeating Characters (Med)
// 滑動窗口 + hash map 記每個 char 最後出現位置, 遇到重複就把 start 移到該位置的下一格
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char,int> last;
        int best = 0, start = 0;
        for (int i = 0; i < (int)s.size(); i++) {
            auto it = last.find(s[i]);
            if (it != last.end() && it->second >= start) start = it->second + 1;
            last[s[i]] = i;
            best = max(best, i - start + 1);
        }
        return best;
    }
};
