// LeetCode #139 Word Break (Med)
// DP: dp[i] = 存在 j<i 使 dp[j] && s[j..i-1] 在字典裡
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> dict(wordDict.begin(), wordDict.end());
        int n = s.size();
        vector<char> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < i; j++) {
                if (dp[j] && dict.count(s.substr(j, i - j))) {
                    dp[i] = 1; break;
                }
            }
        }
        return dp[n];
    }
};
