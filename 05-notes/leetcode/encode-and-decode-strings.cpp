// LeetCode #271 Encode and Decode Strings (Med) — PREMIUM
// 格式: "<len>#<content>" 連接. 用 len prefix 避開 delimiter 衝突
#include <bits/stdc++.h>
using namespace std;

class Codec {
public:
    string encode(vector<string>& strs) {
        string s;
        for (auto& w : strs) s += to_string(w.size()) + "#" + w;
        return s;
    }
    vector<string> decode(string s) {
        vector<string> res;
        int i = 0, n = s.size();
        while (i < n) {
            int j = s.find('#', i);
            int len = stoi(s.substr(i, j - i));
            res.push_back(s.substr(j + 1, len));
            i = j + 1 + len;
        }
        return res;
    }
};
