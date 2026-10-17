// LeetCode #151 Reverse Words in a String (Med)
// stringstream 自動吃空白, reverse 後重新 join
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string reverseWords(string s) {
        stringstream ss(s);
        vector<string> w;
        string t;
        while (ss >> t) w.push_back(t);
        reverse(w.begin(), w.end());
        string r;
        for (int i = 0; i < (int)w.size(); i++) { if (i) r += ' '; r += w[i]; }
        return r;
    }
};
