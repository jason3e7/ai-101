// LeetCode #678 Valid Parenthesis String (Med)
// 維護可能的開括號範圍 [lo, hi], '*' 讓範圍擴大
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    bool checkValidString(string s) {
        int lo = 0, hi = 0;
        for (char c : s) {
            if (c == '(') { lo++; hi++; }
            else if (c == ')') { lo--; hi--; }
            else { lo--; hi++; }
            if (hi < 0) return false;
            lo = max(lo, 0);
        }
        return lo == 0;
    }
};
