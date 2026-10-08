// LeetCode #1768 Merge Strings Alternately (Easy)
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    string mergeAlternately(string a, string b) {
        string r;
        int i = 0, m = a.size(), n = b.size();
        while (i < m || i < n) {
            if (i < m) r += a[i];
            if (i < n) r += b[i];
            i++;
        }
        return r;
    }
};
