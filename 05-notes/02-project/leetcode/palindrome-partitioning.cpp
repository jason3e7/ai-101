// LeetCode #131 Palindrome Partitioning (Med)
// Backtracking: 枚舉切點, 子串是迴文才遞迴
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        vector<string> cur;
        int n = s.size();
        auto isPal = [&](int l, int r) {
            while (l < r) if (s[l++] != s[r--]) return false;
            return true;
        };
        function<void(int)> dfs = [&](int i) {
            if (i == n) { res.push_back(cur); return; }
            for (int j = i; j < n; j++) {
                if (isPal(i, j)) {
                    cur.push_back(s.substr(i, j-i+1));
                    dfs(j+1);
                    cur.pop_back();
                }
            }
        };
        dfs(0);
        return res;
    }
};
