// LeetCode #443 String Compression (Med)
// in-place 原地壓縮: run-length encoding
#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int compress(vector<char>& chars) {
        int n = chars.size(), w = 0, i = 0;
        while (i < n) {
            int j = i;
            while (j < n && chars[j] == chars[i]) j++;
            chars[w++] = chars[i];
            int c = j - i;
            if (c > 1) {
                string s = to_string(c);
                for (char d : s) chars[w++] = d;
            }
            i = j;
        }
        return w;
    }
};
