// LeetCode #17 Letter Combinations of a Phone Number (Med)
// Backtracking: 枚舉每個 digit 對應字母
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if (digits.empty()) return {};
        vector<string> map = {"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
        vector<string> res;
        string cur;
        function<void(int)> dfs = [&](int i) {
            if (i == (int)digits.size()) { res.push_back(cur); return; }
            for (char c : map[digits[i]-'0']) { cur.push_back(c); dfs(i+1); cur.pop_back(); }
        };
        dfs(0);
        return res;
    }
};
