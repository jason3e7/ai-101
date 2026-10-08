// LeetCode #1071 Greatest Common Divisor of Strings (Easy)
// 若 s + t == t + s 則存在共同周期, 周期長 = gcd(len)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string gcdOfStrings(string s, string t) {
        if (s + t != t + s) return "";
        int g = __gcd((int)s.size(), (int)t.size());
        return s.substr(0, g);
    }
};
