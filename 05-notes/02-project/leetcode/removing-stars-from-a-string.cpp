// LeetCode #2390 Removing Stars From a String (Med)
// Stack 的最直接應用: '*' 就 pop
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string removeStars(string s) {
        string r;
        for (char c : s) { if (c == '*') r.pop_back(); else r += c; }
        return r;
    }
};
