// LeetCode #424 Longest Repeating Character Replacement (Med)
// Sliding window: window 有效當 (size - maxCount) <= k
// maxCount 不用每次重算, 只增不減 (因為 best 跟著 maxCount 增加才有可能變大)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int characterReplacement(string s, int k) {
        int cnt[26] = {0}, maxc = 0, best = 0, l = 0;
        for (int r = 0; r < (int)s.size(); r++) {
            maxc = max(maxc, ++cnt[s[r] - 'A']);
            while (r - l + 1 - maxc > k) --cnt[s[l++] - 'A'];
            best = max(best, r - l + 1);
        }
        return best;
    }
};
