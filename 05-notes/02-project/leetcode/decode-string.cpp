// LeetCode #394 Decode String (Med)
// 兩個 stack: 存 count 跟 prev string, 遇到 ']' 時展開
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string decodeString(string s) {
        vector<string> strSt;
        vector<int> cntSt;
        string cur;
        int k = 0;
        for (char c : s) {
            if (isdigit(c)) k = k * 10 + (c - '0');
            else if (c == '[') { cntSt.push_back(k); strSt.push_back(cur); k = 0; cur.clear(); }
            else if (c == ']') {
                int t = cntSt.back(); cntSt.pop_back();
                string prev = strSt.back(); strSt.pop_back();
                string rep;
                for (int i = 0; i < t; i++) rep += cur;
                cur = prev + rep;
            } else cur += c;
        }
        return cur;
    }
};
